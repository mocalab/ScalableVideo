package edu.sdsu.server.util;

import java.util.List;

import edu.sdsu.cameraserver.message.EncoderParams;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.SurfaceHolder.Callback;
import android.widget.Toast;

/**
 * This class provides an interface to activate recording and encoding frames
 * It will be used by both the server as well as the UI
 *
 */
public class EncoderActivationInterface implements PreviewCallback, Callback{
	
//	//LOAD SHARED LIBRARIES
//	static
//	{
//		try {
//			System.loadLibrary("qcomomxsample_ics");
//			System.loadLibrary("qcomomx4ndk_encode_decode_ics");
//
//		} catch (UnsatisfiedLinkError e) {
//			
//		} 		
//	}
	
	//Native methods
	//Set the receiver based on address received by server
	public native String encoderSetRecipient(String address);
	//Start the encoder with a certain width, height, and frame rate
	public native String encoderStart(int width, int height, int frame_rate, int bps, int variableRate);
	//Encode a frame
	public native String encoderFrame(byte[] frameData, Camera camera, int frame_rate);
	//Tear down the encoder
	public native String encoderFinish();
	//Update the encoder bitrate
	public native String updateBitrate(int rate, int vary_rate);
	//Shut down client socket
	public native void closeSocket();
	

	//Camera elements
	private static Camera			m_camera = null;
	private boolean					m_isCameraRunning = false;
	private boolean					m_cameraAlreadyCreated = false;
	
	//Encoder elements
	private int						m_previewWidth = 320; 
	private int						m_previewHeight = 240;
	private int						m_frameRate = 30;
	private int						m_bps = m_previewWidth * m_previewHeight * 3;
	private boolean					m_variableRate = false;
	private final int				PORT = 39081;
	
	//SurfaceHolder -- When we wish to show a preview in the UI
	protected SurfaceHolder			m_sh_cameraHolder;
	
	//Surface texture -- to show no camera preview
	protected SurfaceTexture		m_texture;
	
	//Whether we are streaming or not
	private boolean					m_recordingStatus = false;
	
	//Threads to stop and start streaming
	private StartEncoder			m_startEncoderThread;
	private StopEncoder				m_stopEncoderThread;
	
	private int						m_frameRateControl = 0;
	private int						m_actualFrameRate;
	
	//Application context
	private Context					m_context;
	
	//Constructor
	public EncoderActivationInterface(SurfaceHolder holder, Context context)
	{	
		this.m_context = context;
		Initialize(holder);
	}
	//Initialize the camera and UI's holder for camera
	public void Initialize(SurfaceHolder surface)
	{
		m_sh_cameraHolder = surface;
		m_sh_cameraHolder.addCallback(this); 
		m_sh_cameraHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		
		m_texture = new SurfaceTexture(1);
	}
	//Set the new encoder parameters
	public void setEncoderParams(EncoderParams params)
	{
		m_previewWidth = params.getWidth();
		m_previewHeight = params.getHeight();
		m_frameRate = params.getFps();
		m_bps = params.getBitrate();
		m_variableRate = params.isVariableBitrate();
	}
	//Overloaded parameter set
	public void setEncoderParams(int width, int height, int fps)
	{
		m_previewWidth = width;
		m_previewHeight = height;
		m_frameRate = fps;
		m_bps = width * height * 3;
		m_variableRate = false;
	}
	//Getters for encoder params
	public EncoderParams getEncoderParams()
	{
		EncoderParams params = new EncoderParams();
		params.setWidth(m_previewWidth);
		params.setHeight(m_previewHeight);
		params.setFps(m_frameRate);
		params.setBitrate(m_bps);
		params.setVariableBitrate(m_variableRate);
		
		return params;
	}
	//Restart the camera
	public void restartCamera()
	{
		if(m_isCameraRunning)
		{
			stopCamera();
			startCamera();
			setupCamera(m_sh_cameraHolder);
		}
	}
	//Toggle the camera on/off
	public void updateRecordingStatus()
	{
		boolean statusOn = !m_recordingStatus;
		if(statusOn)
		{
			m_startEncoderThread = new StartEncoder();
			m_startEncoderThread.run();
		}
		else
		{
			m_recordingStatus = false; 
			m_stopEncoderThread = new StopEncoder(); 
			m_stopEncoderThread.run();
		}
		
	}
	//Start streaming 
	public void startStreaming(String ipAddress)
	{
		encoderSetRecipient(ipAddress);
		//m_recordingStatus = true;
		m_startEncoderThread = new StartEncoder();
		m_startEncoderThread.run();	
	}
	//End streaming
	public void endStreaming()
	{
		m_recordingStatus = false; 
		m_stopEncoderThread = new StopEncoder(); 
		m_stopEncoderThread.run();
	}
	//Switch the stream to new scaled video
	public void scaleVideo()
	{
		m_recordingStatus = false;
		new SmartSwitch().run();
	}
	
	//Update the bitrate of the already playing video
	public boolean updateRate(int rate, boolean vary_rate)
	{
		boolean success = true;
		int vary = (vary_rate ? 1 : 0);
		if(updateBitrate(rate, vary).equals("OKAY"))
		{
			this.m_bps = rate;
			this.m_variableRate = vary_rate; 
		}
		else
			success = false;
		return success;
	}
	
	public void deleteThreads()
	{
		if (m_startEncoderThread != null) {
			m_startEncoderThread = null;
		}

		if (m_stopEncoderThread != null) {
			m_stopEncoderThread = null;
		}
	}
	
	/*
	 * This method will start the camera
	 * 
	 */
	private void startCamera()
	{
		if(!m_isCameraRunning && m_camera == null)
		{
			try
			{
				m_camera = Camera.open();
			}
			catch(Exception ex)
			{
				
			}
		}
	}
	
	/*
	 * This Method will set up the camera
	 */
	private boolean setupCamera(SurfaceHolder holder)
	{
		boolean status = true;
		try
		{
			//Get parameters
			Camera.Parameters params = m_camera.getParameters();
			if(m_camera == null || m_cameraAlreadyCreated)
			{
				//Camera already running
				return true;
			}
			if(m_isCameraRunning)
			{
				m_camera.stopPreview();
			}
			
			//Set up camera
			m_camera.setDisplayOrientation(90); 
			
			//Set up parameters
			params.setPreviewFormat(ImageFormat.NV21);
			params.setPictureFormat(PixelFormat.JPEG); 
			params.setPreviewSize(m_previewWidth, m_previewHeight);
			//params.setPreviewFpsRange(30000, 30000);
			params.set("orientation", "landscape");
			params.setRotation(90); 
			
			m_actualFrameRate = params.getPreviewFrameRate();
			Toast.makeText(m_context, "Actual frame rate: " + m_actualFrameRate,	Toast.LENGTH_LONG).show();
			System.out.println("Actual frame rate: " + m_actualFrameRate);
			m_camera.setParameters(params);
			
			
			//Set up surface holder to show preview
			//When wanting to show preview, use surface holder; will use 
			//texture when don't want to show a preview
			//i.e. when we take the SoM off board
			m_camera.setPreviewDisplay(holder);
			//m_camera.setPreviewTexture(m_texture);
			m_camera.setPreviewCallback(this); 
			m_camera.startPreview(); 
			
			m_isCameraRunning = true;
			m_cameraAlreadyCreated = true;
			
		}
		catch(Exception ex)
		{
			Toast.makeText(m_context, ex.getMessage() + " -- Restoring Defaults", Toast.LENGTH_LONG).show();
			System.out.println(ex.getMessage());
			stopCamera();
			return false;
		}
		return status;
	}
	
	//Will stop the camera
	private void stopCamera()
	{
		if(m_camera != null)
		{
			m_camera.setPreviewCallback(null); 
			m_camera.stopPreview(); 
			m_camera.release();
		}
		m_camera = null; 
		m_isCameraRunning = false;
		m_cameraAlreadyCreated = false;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		if(holder == m_sh_cameraHolder)
		{
			if(!setupCamera(holder)) 
				Toast.makeText(m_context, "Failed to set up camera!!! Should handle this in some way...", Toast.LENGTH_LONG).show();
				//finish();
		}
				
		
	}

	//Some callbacks for camera
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		if(holder == m_sh_cameraHolder)
		{
			startCamera();			
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		if(holder == m_sh_cameraHolder)
			stopCamera();
	}

	
	//Callback method invoked on each frame capture
	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		// TODO Auto-generated method stub
		// TODO THIS IS WHERE WE ENCODE THE FRAMES, GET THEM IN A BUFFER, AND SEND PACKETS
		if(m_recordingStatus)
		{
			//COMMENT OUT IFS TO GO BACK TO ORIGINAL FUNCTIONALITY
			if(m_frameRateControl % (m_actualFrameRate / m_frameRate) == 0)
			{
				int length = data.length;
				int expectedLength = (this.m_previewWidth * this.m_previewHeight * 3)/2;
				if (length >= expectedLength) {
					for (int i = this.m_previewWidth * this.m_previewHeight; i < expectedLength - 1; i += 2) {
						byte tmp = data[i];
						data[i] = data[i+1];
						data[i+1] = tmp;
					}
				}

				encoderFrame(data, camera, m_frameRate);
				m_frameRateControl = 0;
			}
		}
		m_frameRateControl++;
		//new MessageListener("TOGGLER").execute();
		
	}
	
	//A message handler invoked from other threads
	Handler hRefresh = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			String message = (String)msg.obj;
			switch(msg.what)
			{
				case 0: 
					
					//Toast.makeText(m_context, message, Toast.LENGTH_LONG).show();
					if (message.equals("No errors.")) {
				    	
						m_recordingStatus = true;	
						
			    	} else {
			    		    		
			    	}
					break;
				case 1: 
					//Toast.makeText(m_context, message, Toast.LENGTH_LONG).show();
					
					m_recordingStatus = false;
					closeSocket();
					break;
				case 2: 
					if(message.equals("TOGGLE"))
						updateRecordingStatus();
					break;
				case 3:
					closeSocket();
					//Restart the camera -- this message is passed when scaling the video
					restartCamera();
					break;
			
			}
		}
		
		
	};
	
	public void externalThreadRestartCamera()
	{
		Message msg = new Message();
		msg.what = 3;
		msg.obj = "ServerThread";
		hRefresh.sendMessage(msg); 
		
	}
	private class StartEncoder extends Thread
	{
		@Override
		public void run() {
			try
			{
				Thread.sleep(1000);
			}
			catch(Exception ex)
			{
				
			}
			//TURN ON ENCODER
			int var_rate = m_variableRate ? 1 : 0;
			String result = encoderStart(m_previewWidth, m_previewHeight, m_frameRate, m_bps, var_rate);
			//Toast.makeText(m_context, result, Toast.LENGTH_LONG).show();
			Message msg = new Message();
			msg.what = 0;
			msg.obj = result;
			hRefresh.sendMessage(msg); 
			Log.v("H264 STREAMER", "Encoder started");
		}
	}
	
	private class StopEncoder extends Thread
	{
		@Override
		public void run() {
			try
			{
				Thread.sleep(1000);
			}
			catch(Exception ex)
			{
				
			}
			//TURN OFF ENCODER
			String result = encoderFinish();
			//Toast.makeText(m_context, result, Toast.LENGTH_LONG).show();
			Message msg = new Message();
			msg.what = 1;
			msg.obj = result;
			hRefresh.sendMessage(msg); 
			Log.v("H264 STREAMER", "Encoder finished");
		}
	}
	
	//CLASS TO RESET ENCODER PARAMETERS
	private class SmartSwitch extends Thread
	{

		@Override
		public void run() {
			// TODO Auto-generated method stub
			try
			{
				Thread.sleep(10);				
			}
			catch(Exception ex)
			{
								
			}
			//Stop and tear down current encoder
			//m_recordingStatus = false;
			String result = encoderFinish();
			//Toast.makeText(m_context, result, Toast.LENGTH_LONG).show();
			Message msg = new Message();
			msg.what = 3;
			msg.obj = result;
			hRefresh.sendMessage(msg); 
			Log.v("H264 STREAMER", "Encoder finished");
			
			//TODO: Potentially wait before beginning again
//			try{
//				Thread.sleep(50);
//				
//			}
//			catch(Exception ex)
//			{
//				
//				
//			}
			//Start new instance of encoder
			int var_rate = m_variableRate ? 1 : 0;
			String result1 = encoderStart(m_previewWidth, m_previewHeight, m_frameRate, m_bps, var_rate);
			//Toast.makeText(m_context, result1, Toast.LENGTH_LONG).show();
			Message msg1 = new Message();
			msg1.what = 0;
			msg1.obj = result;
			hRefresh.sendMessage(msg1); 
			Log.v("H264 STREAMER", "Encoder started");
		}
		
		
	}
	
}
