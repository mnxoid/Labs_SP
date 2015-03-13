/** 
 * @file	networking.cpp
 * @brief	C++ source of the app that ...
 *
 * 
 * Copyright 2014 by mnxoid,
 * 
 * This software is the confidential and proprietary information
 * of mnxoid ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with mnxoid.
 **/
//------------------Includes------------------------------------
#include <stdio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "util.h"
//------------------Namespaces----------------------------------
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
//------------------Disclaimer----------------------------------
/**
 * @brief       This function displays license agreement
 **/
void Disclaimer (  ) 
 {
 	printf ( "This is an app that ....\n\n" ) ; 
	printf ( "Copyright 2014 by mnxoid, \n\n" ) ; 
	printf ( "This software is the confidential and proprietary information\n" ) ; 
	printf ( "of mnxoid  (\"Confidential Information\") .  You\n" ) ; 
	printf ( "shall not disclose such Confidential Information and shall use\n" ) ; 
	printf ( "it only in accordance with the terms of the license agreement\n" ) ; 
	printf ( "you entered into with mnxoid.\n\n" ) ; 
	printf ( "By pressing [ENTER] you confirm that you are NOT A GEODESIST\n" ) ; 
	CleanInput (  ) ; //here CleanInput is used as a safe getchar (  ) 
 }
//------------------Main function-------------------------------
int main()
{
	Disclaimer();
	cout << "Enter your name please: ";
	string name;
	cin >> name;
	CleanInput();
	io_service io_serviceSend;
	udp::resolver resolver(io_serviceSend);
    udp::resolver::query query(udp::v4(),"127.0.0.1","1337",boost::asio::ip::resolver_query_base::numeric_service);
    udp::endpoint receiver_endpoint = *resolver.resolve(query);

    udp::socket socket(io_serviceSend);
    socket.open(udp::v4());

    string messStr = "MnxoidClient:";
    messStr += name;
    boost::shared_ptr<std::string> message(new string(messStr));

    socket.send_to(boost::asio::buffer(*message), receiver_endpoint);
    boost::array<char, 256> recv_buffer_;
    int len = socket.receive_from(boost::asio::buffer(recv_buffer_),receiver_endpoint);
    string got(recv_buffer_.begin(),recv_buffer_.begin()+len);
    cout << got << endl;
	//----------------------------------------------------------------------------------
	{
		int port1 = 1338;
		char* ign = (char*)malloc(100);
		sscanf(got.c_str(),"Hello, %s I am ready to listen on port %i",ign,&port1);
		tcp::resolver resolver1(io_serviceSend);
		tcp::resolver::query query("127.0.0.1",to_string(port1).c_str());
		tcp::resolver::iterator endpoint_iterator = resolver1.resolve(query);
		tcp::resolver::iterator end;
		tcp::socket socket1(io_serviceSend);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket1.close();
			socket1.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);
		string message="hello";
		for (;;)
		{

			boost::array<char, 128> buf;
			boost::system::error_code error;
			if (message=="quit") break;
			size_t len = socket1.read_some(boost::asio::buffer(buf), error);
			boost::system::error_code ignored_error;
			std::cout.write(buf.data(), len);
			a:
			cout << "Your message: ";
			
			getline(cin,message);
			if(message == "") goto a;

			boost::asio::write(socket1, boost::asio::buffer(message),
			boost::asio::transfer_all(), ignored_error);
			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.
	
			
			
		}
	}
	//----------------------------------------------------------------------------------
	CleanInput();
	return 0;
}