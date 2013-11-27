package edu.sdsu.server.util;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;

public class SocketReaderUtility extends BufferedReader{

	public SocketReaderUtility(Reader arg0) {
		super(arg0);
		// TODO Auto-generated constructor stub
	}
	
	//Will return true if the client is still connected, false otherwise
	public boolean peek() throws IOException
	{
		//Set flag to true
		boolean is_open = true;
		
		//mark current position; can only read ahead 10 char
		this.mark(10);
		//read a char; not connected if returns -1

		is_open = this.read() != -1;
		
		if(is_open){
			//reset to last position
			this.reset();
		}
		//return flag
		return is_open;
	}

}
