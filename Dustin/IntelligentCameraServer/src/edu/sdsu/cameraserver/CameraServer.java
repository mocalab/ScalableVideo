package edu.sdsu.cameraserver;



import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import edu.sdsu.cameraserver.prefs.EncoderPreferences;
import edu.sdsu.server.ServerDaemon;
import edu.sdsu.server.util.EncoderActivationInterface;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
/** The main activity of the application.
 * NOTE--- The UI in this application will be for testing purposes only.
 * Once we start using the server to fulfill requests, attempts to use the UI may conflict 
 * with expected operation. 
 * */
public class CameraServer extends Activity implements View.OnClickListener/*, PreviewCallback, Callback*/{

	public static final byte[] RECIPIENT_IPADDRESS = {(byte)146, (byte)244, (byte)146, (byte)209};
	
	//LOAD SHARED LIBRARIES
	static
	{
		try {
			System.loadLibrary("qcomomxsample_ics");
			System.loadLibrary("qcomomx4ndk_encode_decode_ics");

		} catch (UnsatisfiedLinkError e) {
			
		} 		
	}
	
	//UI ELEMENTS -- NOW MOVED TO EncoderActivationInterface
//	private static Camera			m_camera = null;
//	private boolean					m_isCameraRunning = false;
//	private boolean					m_cameraAlreadyCreated = false;
//	
	private int						m_previewWidth = 320; 
	private int						m_previewHeight = 240;
	private int						m_frameRate = 30;
//	private final int				PORT = 39081;
	
	protected SurfaceHolder			m_sh_cameraHolder;
	protected SurfaceView			m_sv_camPreview; 
	
	protected SurfaceTexture		m_texture;
	
	protected Button				m_b_beginStream;
	protected Button				m_b_smartSwitch;
	
	private boolean					m_recordingStatus = false;
	
//	private StartEncoder			m_startEncoderThread;
//	private StopEncoder				m_stopEncoderThread;
	
	private SharedPreferences	    m_defaultPrefs;

	private PowerManager.WakeLock	m_wake_lock;
	
//	private int						m_frameRateControl = 0;
//	private int						m_actualFrameRate;
	
	//Actual server
	private	ServerDaemon			m_server;
	
	//The main interface to activate and disable streaming -- defined in this class
	//to provide a common interface between the UI (for testing) and the TCP server
	private EncoderActivationInterface m_encoder;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.camera_server); 
		
		//Acquire wake lock
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		m_wake_lock = pm.newWakeLock(pm.FULL_WAKE_LOCK, "Wake lock for streamer");
		
	
		//Initialize UI ELements
		Initialize();
		
		//Need to instantiate and start running the server
	}
	
	private void Initialize()
	{
		//SET UP SURFACE VIEW/HOLDER
		m_sv_camPreview = (SurfaceView) findViewById(R.id.svCamPreview); 
		m_sv_camPreview.setZOrderMediaOverlay(true);
//		m_sh_cameraHolder = m_sv_camPreview.getHolder();
//		m_sh_cameraHolder.addCallback(this); 
//		m_sh_cameraHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
//		
//		m_texture = new SurfaceTexture(1);
		//No need for setting up the camera-- all done in here
		
		m_encoder = new EncoderActivationInterface(m_sv_camPreview.getHolder(), this);
		
		//The TCP server -- pass it the same encoder interface the UI uses
		m_server = new ServerDaemon(m_encoder);
		//Start the server
		m_server.execute();
		
		//Some buttons for testing
//		m_b_beginStream = (Button) findViewById(R.id.bBeginStream); 
//		m_b_beginStream.setOnClickListener(this);
//		
//		m_b_smartSwitch = (Button) findViewById(R.id.bSwitchStream); 
//		m_b_smartSwitch.setOnClickListener(this);
		
		
		m_defaultPrefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		m_wake_lock.release();
		//stopCamera();
	}
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		m_wake_lock.acquire();
		//startCamera();
		
		//Toast.makeText(this, "RESUME", Toast.LENGTH_LONG).show();
		
		//Set up encoder params
		try
		{
			m_previewWidth = Integer.parseInt(m_defaultPrefs.getString("width", "320"));
			m_previewHeight = Integer.parseInt(m_defaultPrefs.getString("height", "240"));
			m_frameRate = Integer.parseInt(m_defaultPrefs.getString("frame_rate", "30"));
			//Reset the camera preview 
			m_encoder.setEncoderParams(Integer.parseInt(m_defaultPrefs.getString("width", "320")), 
					Integer.parseInt(m_defaultPrefs.getString("height", "240")), 
					Integer.parseInt(m_defaultPrefs.getString("frame_rate", "30")));
		}
		catch(NumberFormatException ex)
		{
			Toast.makeText(this, ex.getMessage(), Toast.LENGTH_LONG).show();
		}
		Toast.makeText(this, 
					   "ENCODER PARAMS -- \nWIDTH: " + m_previewWidth + 
					   "\nHEIGHT: " + m_previewHeight + 
					   "\nFRAME RATE: " + m_frameRate, Toast.LENGTH_LONG).show();
		
//		if(m_isCameraRunning)
//		{
//			stopCamera();
//			startCamera();
//			setupCamera(m_sh_cameraHolder);
//		}
		m_encoder.restartCamera();
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
//		switch(v.getId())
//		{
//			case R.id.bBeginStream: 
//				//TODO: Begin streaming/Stop streaming logic
//				m_recordingStatus = !m_recordingStatus;
//				m_b_beginStream.setText(m_recordingStatus ? "End Stream" : "Begin Streaming");
//				m_encoder.updateRecordingStatus();
//				break;
//			case R.id.bSwitchStream: 
//				m_encoder.scaleVideo();
//				break;
//		
//		}
	}
	/**
	 * THE COMMENTED SECTIONS BELOW ARE FOR THE ENCODER AND THE CAMERA. THEY ARE NOW
	 * DEFINED BY THE EncoderActivationInterface
	 * 
	 */
	
//	private void updateRecordingStatus(boolean statusOn)
//	{
//		m_b_beginStream.setText(statusOn ? "End Stream" : "Begin Streaming");
//		
//		if(statusOn)
//		{
//			m_startEncoderThread = new StartEncoder();
//			m_startEncoderThread.run();
//		}
//		else
//		{
//			m_recordingStatus = false; 
//			m_stopEncoderThread = new StopEncoder(); 
//			m_stopEncoderThread.run();
//		}
//		
//	}
	
	/*
	 * This method will start the camera
	 * 
	 */
//	private void startCamera()
//	{
//		if(!m_isCameraRunning && m_camera == null)
//		{
//			try
//			{
//				m_camera = Camera.open();
//			}
//			catch(Exception ex)
//			{
//				Toast.makeText(this, ex.getMessage(), Toast.LENGTH_LONG).show();
//			}
//		}
//	}
	
	public void onDestroy() {
		super.onDestroy();
//		if (m_startEncoderThread != null) {
//			m_startEncoderThread = null;
//		}
//
//		if (m_stopEncoderThread != null) {
//			m_stopEncoderThread = null;
//		}
		m_encoder.deleteThreads();
	}
	
	/*
	 * This Method will set up the camera
	 */
//	private boolean setupCamera(SurfaceHolder holder)
//	{
//		boolean status = true;
//		try
//		{
//			//Get parameters
//			Camera.Parameters params = m_camera.getParameters();
//			if(m_camera == null || m_cameraAlreadyCreated)
//			{
//				//Camera already running
//				return true;
//			}
//			if(m_isCameraRunning)
//			{
//				m_camera.stopPreview();
//			}
//			
//			//Set up camera
//			m_camera.setDisplayOrientation(90); 
//			
//			//Set up parameters
//			params.setPreviewFormat(ImageFormat.NV21);
//			params.setPictureFormat(PixelFormat.JPEG); 
//			params.setPreviewSize(m_previewWidth, m_previewHeight);
//			params.set("orientation", "portrait");
//			params.setRotation(90); 
//			
//			m_actualFrameRate = params.getPreviewFrameRate();
//			//Toast.makeText(this, "Actual frame rate: " + m_actualFrameRate,	Toast.LENGTH_LONG).show();
//			
//			m_camera.setParameters(params);
//			
//			//Set up surface holder to show preview
//			//When wanting to show preview, use surface holder; will use 
//			//texture when don't want to show a preview
//			//i.e. when we take the SoM off board
//			//m_camera.setPreviewDisplay(holder);
//			m_camera.setPreviewTexture(m_texture);
//			m_camera.setPreviewCallback(this); 
//			m_camera.startPreview(); 
//			
//			m_isCameraRunning = true;
//			m_cameraAlreadyCreated = true;
//			
//		}
//		catch(Exception ex)
//		{
//			Toast.makeText(this, ex.getMessage() + " -- Restoring Defaults", Toast.LENGTH_LONG).show();
//			stopCamera();
//			return false;
//		}
//		return status;
//	}
	
	//Restore Default values of encoder
//	private void restoreDefaults()
//	{
//		m_previewWidth = 320;
//		m_previewHeight = 240;
//		m_frameRate = 30; 
//		
//		m_defaultPrefs.edit().putString("width", "320").commit();
//		m_defaultPrefs.edit().putString("height", "240").commit();
//		m_defaultPrefs.edit().putString("frame_rate", "30").commit();
//	}
	
	//Will stop the camera
//	private void stopCamera()
//	{
//		if(m_camera != null)
//		{
//			m_camera.setPreviewCallback(null); 
//			m_camera.stopPreview(); 
//			m_camera.release();
//		}
//		m_camera = null; 
//		m_isCameraRunning = false;
//		m_cameraAlreadyCreated = false;
//	}
	
	//FOR CREATING OPTIONS MENU (to alter encoding parameters)
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		super.onCreateOptionsMenu(menu);
		
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.settingsmenu, menu); 
		
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		switch(item.getItemId())
		{
			case R.id.itemParams: 
				Intent i = new Intent(this, EncoderPreferences.class);
				startActivity(i);
				break;
			case R.id.cancel: 
				
				break;
		
		}
		
		return false;
	}
	
	//////////////////////////IMPLEMENTED CALLBACK METHODS///////////////////////////////////////
	
//	@Override
//	public void surfaceChanged(SurfaceHolder holder, int format, int width,
//			int height) {
//		// TODO Auto-generated method stub
//		if(holder == m_sh_cameraHolder)
//		{
//			if(!setupCamera(holder))
//				finish();
//		}
//		
//	}
//
//	@Override
//	public void surfaceCreated(SurfaceHolder holder) {
//		// TODO Auto-generated method stub
//		if(holder == m_sh_cameraHolder)
//		{
//			startCamera();			
//		}
//		
//	}
//
//	@Override
//	public void surfaceDestroyed(SurfaceHolder holder) {
//		// TODO Auto-generated method stub
//		if(holder == m_sh_cameraHolder)
//			stopCamera();
//		
//	}

	//Native methods
//	public native String encoderStart(int width, int height, int frame_rate);
//	public native String encoderFrame(byte[] frameData, Camera camera);
//	public native String encoderFinish();
//	
//	@Override
//	public void onPreviewFrame(byte[] data, Camera camera) {
//		// TODO THIS IS WHERE WE ENCODE THE FRAMES, GET THEM IN A BUFFER, AND SEND PACKETS
//		if(m_recordingStatus)
//		{
//			//if(m_frameRateControl % (m_actualFrameRate / m_frameRate) == 0)
//			//{
//				int length = data.length;
//				int expectedLength = (this.m_previewWidth * this.m_previewHeight * 3)/2;
//				if (length >= expectedLength) {
//					for (int i = this.m_previewWidth * this.m_previewHeight; i < expectedLength - 1; i += 2) {
//						byte tmp = data[i];
//						data[i] = data[i+1];
//						data[i+1] = tmp;
//					}
//				}
//
//				encoderFrame(data, camera);
//				m_frameRateControl = 0;
//			}
//		//}
//		//m_frameRateControl++;
//		//new MessageListener("TOGGLER").execute();
//	}
	
	//STATUS REFRESH HANDLER 
//	Handler hRefresh = new Handler(){
//
//		@Override
//		public void handleMessage(Message msg) {
//			String message = (String)msg.obj;
//			switch(msg.what)
//			{
//				case 0: 
//					
//					Toast.makeText(CameraServer.this, message, Toast.LENGTH_LONG).show();
//					if (message.equals("No errors.")) {
//				    	m_b_beginStream.setText("End Stream");
//						m_recordingStatus = true;	
//						m_b_smartSwitch.setEnabled(true);
//			    	} else {
//			    		m_b_beginStream.setText("Begin Streaming");		    		
//			    	}
//					break;
//				case 1: 
//					Toast.makeText(CameraServer.this, message, Toast.LENGTH_LONG).show();
//					m_b_beginStream.setText("Begin Streaming");
//					m_recordingStatus = false;
//					m_b_smartSwitch.setEnabled(false);
//					break;
//				case 2: 
//					if(message.equals("TOGGLE"))
//						updateRecordingStatus(!m_recordingStatus);
//					break;
//			
//			}
//		}
//		
//		
//	};
//	
//	private class StartEncoder extends Thread
//	{
//		@Override
//		public void run() {
//			try
//			{
//				Thread.sleep(1000);
//			}
//			catch(Exception ex)
//			{
//				
//			}
//			//TODO: TURN ON ENCODER
//			String result = encoderStart(m_previewWidth, m_previewHeight, m_frameRate);
//			Message msg = new Message();
//			msg.what = 0;
//			msg.obj = result;
//			hRefresh.sendMessage(msg); 
//			Log.v("H264 STREAMER", "Encoder started");
//		}
//	}
//	
//	private class StopEncoder extends Thread
//	{
//		@Override
//		public void run() {
//			try
//			{
//				Thread.sleep(1000);
//			}
//			catch(Exception ex)
//			{
//				
//			}
//			//TODO: TURN OFF ENCODER
//			String result = encoderFinish();
//			Message msg = new Message();
//			msg.what = 1;
//			msg.obj = result;
//			hRefresh.sendMessage(msg); 
//			Log.v("H264 STREAMER", "Encoder finished");
//		}
//	}
//	
//	//CLASS TO RESET ENCODER PARAMETERS
//	private class SmartSwitch extends Thread
//	{
//
//		@Override
//		public void run() {
//			// TODO Auto-generated method stub
////			try
////			{
////				Thread.sleep(1000);				
////			}
////			catch(Exception ex)
////			{
////								
////			}
//			//Stop and tear down current encoder
//			String result = encoderFinish();
//			Message msg = new Message();
//			msg.what = 1;
//			msg.obj = result;
//			hRefresh.sendMessage(msg); 
//			Log.v("H264 STREAMER", "Encoder finished");
//			
//			//TODO: Potentially wait before beginning again
//			try{
//				Thread.sleep(50);
//				
//			}
//			catch(Exception ex)
//			{
//				
//				
//			}
//			//Start new instance of encoder
//			String result1 = encoderStart(m_previewWidth, m_previewHeight, m_frameRate);
//			Message msg1 = new Message();
//			msg1.what = 0;
//			msg1.obj = result;
//			hRefresh.sendMessage(msg1); 
//			Log.v("H264 STREAMER", "Encoder started");
//		}
//		
//		
//	}
	
	/*//THREAD TO HANDLE MESSAGES FOR REMOTE ACCESS TO STREAMING
	private class MessageListener extends AsyncTask<Void,Void,Void>
	{
		private DatagramSocket m_socket;
	
		public MessageListener(String name)
		{
			
		}
		@Override
		protected Void doInBackground(Void... params) {
			try {
				m_socket = new DatagramSocket(PORT);
				byte[] buffer = new byte[256]; 
				DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		
				m_socket.receive(packet);
				String data = new String(packet.getData()).trim();
	
				if(!data.equals(null) && 
						packet.getAddress().equals(InetAddress.getByAddress(RECIPIENT_IPADDRESS)))
				{
					//Handle message
					Message msg = new Message();
					msg.what = 2;
					msg.obj = data;
					hRefresh.sendMessage(msg);
					Log.e("INCOMING MESSAGE", data);
				}
					
			
			} catch (IOException e) {
		
				e.printStackTrace();
			}
			finally
			{
			
				m_socket.close();
			}
			return null;
		}
	
	}*/
}
