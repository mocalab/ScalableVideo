package edu.sdsu.cameraserver.message;

public class MalformedMessageException extends Exception{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public MalformedMessageException()
	{
		super();
	}
	
	public MalformedMessageException(String msg)
	{
		super(msg);
	}

}
