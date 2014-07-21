package edu.sdsu.server;


import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import edu.sdsu.cameraserver.message.EncoderParams;
import edu.sdsu.cameraserver.message.MalformedMessageException;
import edu.sdsu.cameraserver.message.StreamerMessageFactory;
import edu.sdsu.server.util.EncoderActivationInterface;
import edu.sdsu.server.util.SocketReaderUtility;

public class ServerThread extends Thread{
	private Socket							client_socket;
	private PrintWriter						send;
	private SocketReaderUtility				receiver;
	
	//Interface to activate encoder, assumes UI is set up
	private EncoderActivationInterface		m_encoder;
	
	public ServerThread(Socket client_socket, EncoderActivationInterface encoder)
	{
		this.client_socket = client_socket;
		m_encoder = encoder;
		
		//m_encoder.updateRecordingStatus();
	}
	
	
	@Override
	public void run() {
		try {
			//Start streaming video to the client
			m_encoder.startStreaming(this.client_socket.getInetAddress().toString().substring(1));
			//Set up send and receive streams
			send = new PrintWriter(client_socket.getOutputStream(), true);
			receiver = new SocketReaderUtility(
					new InputStreamReader(client_socket.getInputStream()));
			//Continuous loop while socket is open
			String request = "";
			String response = "";
			//Peek will return true if the client is still connected
			while(receiver.peek())
			{
				request = receiver.readLine();
				System.out.println(request);
				//TODO: Do some parsing of the request...
				
				//To be filled with some response
				if(!request.equals("") && (request != null))
				{
					StreamerMessageFactory parser = new StreamerMessageFactory();
					try {
						EncoderParams params = parser.parse(request);
						if(params.isToStart())
						{
							if(params.equals(m_encoder.getEncoderParams()))
							{
								response = "Video parameters already set.";
							}
							else
							{
								response = "Updating the camera";
								if(params.bitrateOnlyChanged(m_encoder.getEncoderParams()))
								{
									m_encoder.updateRate(params.getBitrate(), params.isVariableBitrate());
								}
								else
								{
									//Set params
									m_encoder.setEncoderParams(params);
		//							try {
		//								Thread.sleep(50);
		//							} catch (InterruptedException e) {
		//								// TODO Auto-generated catch block
		//								e.printStackTrace();
		//							}
									//Restart camera
									
									//Scale video
									m_encoder.scaleVideo();
								}
							}
						}
						else
						{
							response = "Stopping camera";
							m_encoder.endStreaming();
						}
					} catch (MalformedMessageException e) {
						// TODO Auto-generated catch block
						response = e.getMessage();
					}
					response += "\0";
					send.println(response);
				}
				
				
			}
			System.out.println("They disconnected!!!");	
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			System.out.println("They disconnected!!!");
		}
		finally{
			m_encoder.endStreaming();
			send.close();
			try {
				client_socket.close();
				receiver.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(EncoderActivationInterface.USE_STANDARD_VIDEO)
				m_encoder.setEncoderParams(352, 288, 30);
			else
				m_encoder.setEncoderParams(320, 240, 30);
				
			m_encoder.externalThreadRestartCamera();
			
		}
	}

	

}
