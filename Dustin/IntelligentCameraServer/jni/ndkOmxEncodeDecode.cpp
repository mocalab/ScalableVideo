/*==============================================================================
            Copyright (c) 2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Proprietary

	File: ndkOmxEncodeDecode.cpp

	This class is used to illustrate how to integrate the built shared
	libraries for using the hardware-accelerated video codecs into an
	Android apk by way of native calls.

	These calls focus on the encoding and decoding of image files to and from
	yuv and rgb formats.  The libraries for Froyo and Gingerbread here are
	cross-compatible, but are not cross-compatible with Honeycomb and as such
	to run on Honeycomb must be built against the Honeycomb shared library.

==============================================================================*/
#include <string.h>
#include <jni.h>
#include "QcomOmxPublicInterface.h"
#include <android/log.h>
#include <semaphore.h>
//FOR SENDING PACKETS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

// In some cases a bitrate test may be found to be useful.  In order to perform
// a bitrate test, the following steps must be performed.
//    1. Rebuild the shared library from source if necessary, modifying
//       BitrateTest.cpp searching for BITRATE CONFIGURATION to set custom
//       values for the different bitrates and frame intervals.  If doing so,
//       the newly built shared library needs to be recopied to the lib/ folder.
//    2. Copy BitrateTest.h from the qcomomxsample directory in source
//       to the jni/ directory in the project.
//    3. Set the USE_BITRATE_ENCODING_TEST flag to 1.
//
#define USE_BITRATE_ENCODING_TEST 0
//The port we are sending over
#define PORT 39082
#define IP_ADDRESS "192.168.1.7"
#define MAX_PAYLOAD 1500

#if USE_BITRATE_ENCODING_TEST
#include "BitrateTest.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//Global encoder values
void 		 *g_encoder = NULL;
encoderParams g_params;
long 		  g_timeStamp;

//Socket file descriptor
int 		  g_sockfd;

//////TESTING FRAME RATE////////
long		  g_count;
int			  g_print = 1;
time_t		  start, end;

//FOR BITRATE SWITCHING
int mult = 0;
int variable_rate = 0;

////////////////////////////////
char 		 *g_address = (char *)"140.154.1.1";
int			  g_port = 39082;

FILE 		 *f = NULL;

//Struct for RTP header
typedef struct rtpheader{
	char vpxcc;
	char marker_payload;
	unsigned short sequence_number;
	unsigned int timestamp;
	unsigned int ssrc;
} rtpheader;

//Union for RTP packets
union rtppacket {
	char data[65535];
	rtpheader header;
} rtppacket;

///////////////////////////////////////////////////////////////////////////////////////
// DWRIGHT
// NATIVE SOURCE FOR H.264 STREAMING
///////////////////////////////////////////////////////////////////////////////////////
int pipeEncodedtoPacket(void *obj, void *buffer, size_t bufferSize, void *iomxBuffer, void *userData)
{

	if(g_print)
	{
		/*char ticks[16];
		sprintf(ticks, "%d %d %d", g_count, g_params.frameRate, (int)bufferSize);
		__android_log_print(ANDROID_LOG_ERROR, "QCOMOMXINTERFACE", ticks);*/
		omx_interface_print_avc_params(g_encoder);

		g_count = 0;
		time(&start);
	}
	g_count++;
	time(&end);
	double timediff = difftime(end, start);
	g_print = timediff >= 1;

	//NOT STREAMING RTP -- UNCOMMENT TO STREAM RTP
//	union rtppacket packet;
////	//Set up RTP header
//	memset(packet.data, 0, 1500);
//	//packet.data = (char *) malloc(12 + bufferSize);
//	packet.header.vpxcc = 0x80; //10000000
//	packet.header.marker_payload = 0xE0;//11100000
//	packet.header.sequence_number = (unsigned short)htons(0x022E);
//	packet.header.timestamp = (unsigned int)htonl(g_timeStamp);//0x00000000;
//	packet.header.ssrc = (unsigned int)htonl(0x0FE00DC0);
//	memcpy((packet.data+12), buffer, sizeof(buffer)+1);
	//packet.data[13] = (char *)buffer;


	//Send buffer to receiver address (server side)
	sockaddr_in receiver;

	receiver.sin_family = AF_INET;
	receiver.sin_addr.s_addr = inet_addr(g_address);
	receiver.sin_port = htons(g_port);

	int sent_bytes = sendto(g_sockfd, (const char *) buffer/*packet.data*/,
			bufferSize/*1500*/, 0, (sockaddr*) &receiver, sizeof(sockaddr_in));
	if(sent_bytes != bufferSize /*+ 12*/)
	{
		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to send packet: %s", strerror(errno));

	}

	return 0;
}

//Open up the socket
jstring Java_edu_sdsu_server_util_EncoderActivationInterface_encoderSetRecipient
	(JNIEnv *env, jclass cls, jstring address)
{
	g_address = (char *)env->GetStringUTFChars(address, 0);

	//strcpy(g_address, new_addr);
	return env->NewStringUTF("Set receiver address");
}

//SET UP ENCODER
jstring Java_edu_sdsu_server_util_EncoderActivationInterface_encoderStart
  	(JNIEnv *env, jclass cls, /*jstring outfile,*/ jint width,
  			jint height, jint frame_rate, jint bps, jint variableRate) {

	//Open a socket
	g_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(g_sockfd <= 0)
	{
		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to create socket");
		return env->NewStringUTF("Failed to open socket");
	}

	//Bind to port on host side
	sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short) PORT);

	int bound = bind(g_sockfd, (sockaddr*) &address, sizeof(address));
	if(bound < 0)
	{
		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to bind to socket: %s", strerror(errno));
		close(g_sockfd);
		return env->NewStringUTF("Failed to open socket");
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Bound to socket!: %s", inet_ntoa(address.sin_addr));
	}

	//Set non-blocking
	int nonBlocking = 1;
	if(fcntl(g_sockfd, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to set nonblocking");
		close(g_sockfd);
		return env->NewStringUTF("Failed to open socket");
	}

	//Set up encoder params
	g_encoder = NULL;
	g_timeStamp = 0;

	g_params.frameWidth = width;
	g_params.frameHeight = height;
	g_params.frameRate = frame_rate;//30; //Specify frame rate at 30fps
	g_params.rateControl = 3;//2;
	//Need better bitrate calculation
	g_params.bitRate = bps == 0 ? width * height * 3 : bps;
	g_params.codecString = NULL;

	//Flag to vary bitrate
	variable_rate = variableRate;

	//const char *outputFileName = env->GetStringUTFChars(outfile,0);
	//__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "FRAME WxH = %dx%d TO %s", width, height, outputFileName);

	//f = fopen(outputFileName, "wb");

	int status = 0;
	g_encoder = encoder_create(&status, &g_params);
	if (g_encoder == NULL) {
		return env->NewStringUTF(resultDescription(status));
	}

	// The input semaphore is important; by using it, the procedure for
	// feeding the component will wait for an available input buffer when
	// the component is busy.
	//
	omx_setup_input_semaphore(g_encoder);

//#if USE_BITRATE_ENCODING_TEST
//	// Uses the variable bitrate encoding as given in BitrateTest.cpp.  The
//	// bitrate setting in g_params is ignored in this version.
//	omx_interface_register_output_callback(g_encoder, handleOutputEncodedToFile, f);
//
//#else
//	// Default implementation uses arbitrary bitrate of width * height * 3 kbps
//	omx_interface_register_output_callback(g_encoder, handleOutputEncoded, f);
//
//#endif

	omx_interface_register_output_callback(g_encoder, pipeEncodedtoPacket, NULL);

	// Initialize the component and set to the execution state, waiting
	// for frames from the camera.
	status = omx_interface_init(g_encoder);

//	status = omx_interface_set_region_of_interest(g_encoder, 0);
//	if(status != 0)
//	{
//		__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to set region of interest (input)");
//
//	}
//	else
//	{
//		status = omx_interface_set_region_of_interest(g_encoder, 1);
//		if(status != 0)
//		{
//			__android_log_print(ANDROID_LOG_ERROR, "DWRIGHT--UDPSEND", "Failed to set region of interest (output)");
//
//		}
//	}

	// Return the state
	const char *result = resultDescription(status);
	return env->NewStringUTF(result);
}

//TEAR DOWN ENCODER
jstring Java_edu_sdsu_server_util_EncoderActivationInterface_encoderFinish
  	(JNIEnv *env, jclass cls) {
  	//fclose(f);
	//Close socket
	close(g_sockfd);
	__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "ABOUT TO FINISH ENCODER");

	int status = 0;
	do {
		// Send the end of input flag until unable to.  This could be better
		// handled by a semaphore instead of a tight while loop.
		status = omx_interface_send_end_of_input_flag(g_encoder, g_timeStamp);

	} while (status != 0);

	__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "ENCODER DE-INIT");

	// Clean up the input semaphore
	omx_teardown_input_semaphore();

	// Deinitialize the component.
	status = omx_interface_deinit(g_encoder);
	if (status == 0) {
		// On success, destroy the component
		__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "ENCODER DESTROY");
		status = omx_interface_destroy(g_encoder);
	}

	// Return status back to the application.
	__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "ENCODER STATUS CHECK");
	const char *result = resultDescription(status);

	__android_log_print(ANDROID_LOG_ERROR,"QCOMOMXINTERFACE", "FINISHED");
	return env->NewStringUTF(result);
}

//SEND FRAMES TO ENCODER
jstring Java_edu_sdsu_server_util_EncoderActivationInterface_encoderFrame
  	(JNIEnv *env, jclass cls, jbyteArray frameData, jobject camera, jint rate) {
	jbyte *frameBytes = (jbyte *)env->GetByteArrayElements(frameData, 0);

	if(variable_rate)
	{
		//Update bitrate every frame (should come up with better algo)
		int base_rate = g_params.bitRate / 2;
		mult = (mult + 1) % 2;
		omx_interface_update_bitrate(g_encoder, base_rate * (mult + 1));
	}
	// Directly sends the bytes to the encoder
	omx_send_data_frame_to_encoder(g_encoder, (unsigned short *)frameBytes,
                                   g_params.frameWidth, g_params.frameHeight, g_timeStamp);



    // Sets the timestamp to 1/30 of a frame.  Actual applications
    // should use more accurately measured timestamping.
	//^ see above... may need to change this for different frame rates
	g_timeStamp += 1000000/rate;

	// Free up the byte elements that were referenced
	env->ReleaseByteArrayElements(frameData, frameBytes, 0);

	return env->NewStringUTF("Frame!");
}

jstring Java_edu_sdsu_server_util_EncoderActivationInterface_updateBitrate
  	(JNIEnv *env, jclass cls, jint rate, jint vary_rate) {

	int ret;
	variable_rate = vary_rate;
	//Get the effective bitrate
	int effective_rate = (rate == 0 ? g_params.frameWidth * g_params.frameHeight * 3 : rate);
	omx_interface_update_bitrate(g_encoder, effective_rate);
	g_params.bitRate = effective_rate;

	return env->NewStringUTF("OKAY");
}

#ifdef __cplusplus
}
#endif
