package edu.sdsu.cameraserver.message;

public class StreamerMessageFactory {
	
	/*POSSIBLE STRINGS:
	 * 		Start -- 
	 * 			start w h fps bps
	 * 		Stop -- 
	 * 			stop
	 * 		Update -- 
	 * 			update -option value
	 * 
	 * */
	
	/**
	 * @param msg
	 * */
	public EncoderParams parse(String msg) throws MalformedMessageException
	{
		EncoderParams params = new EncoderParams();
		//Make sure not empty string
		if(msg.equals(""))
		{
			throw new MalformedMessageException("Message passed is empty.");
		}
		//Get an array from msg
		String[] msgs = msg.split("\\s+");
		
		
		String operation = msgs[0];
		if(operation.equalsIgnoreCase("start"))
		{
			//Start operation
			//Set up the encoder params
			this.parseStartParams(msgs, params);
			
		}
		else if(operation.equalsIgnoreCase("stop"))
		{
			//Stop operation
			if(msgs.length != 1)
			{
				throw new MalformedMessageException("Stop operation does not take parameters");
			}
			params.setToStart(false);
			
		}
		//There may be a use case for this operation in the future
		else if(operation.equalsIgnoreCase("update"))
		{
			//Update operation
			
		}
		else
		{
			//Unrecognized operation
			throw new MalformedMessageException("Unrecognized operation.");
		}
		
		
		return params;
	}
	
	//Utility method to parse start parameters
	private void parseStartParams(String[] msgs, EncoderParams params) throws MalformedMessageException
	{
		//Ensure number of params is correct
		if(msgs.length != 5)
		{
			throw new MalformedMessageException("Start operation: incorrect number of parameters");
		}
		try{
			//See if bitrate param is marked for variable bitrate
			if(msgs[4].endsWith("'"))
			{
				params.setVariableBitrate(true);
				msgs[4] = msgs[4].substring(0, msgs[4].length() - 1);
				
			}
			int width = Integer.parseInt(msgs[1]);
			int height = Integer.parseInt(msgs[2]);
			int fps = Integer.parseInt(msgs[3]);
			int bps = Integer.parseInt(msgs[4]);
			
			//Only allow 30fps and 15fps for now
			if(fps != 30 && fps != 25 && fps != 15)
			{
				throw new MalformedMessageException("Invalid framerate. Acceptable frame rates: 30, 15");
			}
		
			//Set params
			params.setWidth(width);
			params.setHeight(height);
			params.setFps(fps);
			params.setBitrate(bps);
			params.setToStart(true);
		}catch(NumberFormatException ex)
		{
			throw new MalformedMessageException("Malformed message -- " + ex.getMessage());
		}
		
	}

}
