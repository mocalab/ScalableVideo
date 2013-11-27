package edu.sdsu.cameraserver.message;

//POJO class for encoder params
public class EncoderParams {
	private int				width;
	private int				height;
	private int				fps;
	private int				bitrate;
	private boolean			toStart;
	private boolean			variableBitrate;
	
	
	
	public EncoderParams()
	{
		this.width = 320;
		this.height = 240;
		this.bitrate = width * height * 3;
		this.fps = 30;
		this.toStart = false;
		this.variableBitrate = false;
	}
	
	//Accessors
	
	public int getWidth() {
		return width;
	}
	public boolean isToStart() {
		return toStart;
	}
	public int getHeight() {
		return height;
	}
	public int getFps() {
		return fps;
	}
	public int getBitrate() {
		return bitrate;
	}
	public boolean isVariableBitrate() {
		return variableBitrate;
	}

	public void setWidth(int width) {
		this.width = width;
	}
	public void setHeight(int height) {
		this.height = height;
	}
	public void setFps(int fps) {
		this.fps = fps;
	}
	public void setBitrate(int bitrate) {
		this.bitrate = bitrate;
	}
	public void setToStart(boolean toStart) {
		this.toStart = toStart;
	}
	public void setVariableBitrate(boolean variableBitrate) {
		this.variableBitrate = variableBitrate;
	}

	public String toString()
	{
		return "Encoder parameters: width=" + this.width + " height=" + this.height + " fps=" + this.fps;
	}
	
	//Equals
	public boolean equals(EncoderParams rhs)
	{
		return (this.width == rhs.width) && (this.height == rhs.height) 
				&& (this.fps == rhs.fps) && (this.bitrate == rhs.bitrate)
				&& (this.variableBitrate == rhs.variableBitrate); 
	}
	
	/**
	 * 
	 */
	public boolean bitrateOnlyChanged(EncoderParams rhs)
	{
		return (this.width == rhs.width) && (this.height == rhs.height) 
				&& (this.fps == rhs.fps) && ((this.bitrate != rhs.bitrate)
				|| (this.variableBitrate != rhs.variableBitrate)); 
	}
}
