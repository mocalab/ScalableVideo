package edu.sdsu.server;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class Main {

	private static List<ServerThread>	connections = new ArrayList<ServerThread>();
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		try {
			ServerSocket socket = new ServerSocket();
			socket.setReuseAddress(true);
			//socket.setSoTimeout(10000);
			socket.bind(new InetSocketAddress(37986));
			while(!socket.isClosed())
			{
				System.out.println("Before accept....");
				Socket skt = socket.accept();
				System.out.println("Connected! Host IP: " + skt.getInetAddress());
				ServerThread connection = new ServerThread(skt);
				connection.start();
				connections.add(connection);
			}
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
