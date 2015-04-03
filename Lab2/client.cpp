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
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <netdb.h>
#include "util.h"
//------------------Namespaces----------------------------------
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::asio::deadline_timer;
//------------------Disclaimer----------------------------------
boost::array<char, 256> recv_buffer_;
string got;
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
int hostname_to_ip(char * hostname , char* ip)
{
    hostent *he;
    in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}
void udpHandler(const boost::system::error_code& ec,
    std::size_t len)
{
	got = string(recv_buffer_.begin(),recv_buffer_.begin()+len);
    cout << got << endl;
}
//------------------Classes-------------------------------------
void thrfun(int port,string name)
{
	io_service io_serviceSend;
	udp::resolver resolver(io_serviceSend);
	//Multicast:
    udp::resolver::query query(udp::v4(),"224.0.0.251",to_string(port).c_str(),boost::asio::ip::resolver_query_base::numeric_service);
    udp::endpoint receiver_endpoint = *resolver.resolve(query);
    //udp::endpoint receiver_endpoint(multicast_address, port);

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
	thread** thrs = new thread*[10];
	cout << "Scanning servers..." << endl;
	for(int i=0;i<10;i++)
	{
		//cout << "Trying port " << 1337+i << endl;
		thrs[i] = new thread(thrfun,1337+i, name);
	}
 	cout << endl;
 	this_thread::sleep_for(chrono::milliseconds(5000));
	//----------------------------------------------------------------------------------
	{
		string ipstr;
		cout << "Input IP please ";
		cin >> ipstr;
		int port1;
		cout << "Input port please ";
		cin >> port1;
		CleanInput();
		// char* ign = (char*)malloc(100);
		// sscanf(got.c_str(),"Hello, %s I am ready to listen on port %i",ign,&port1);
		tcp::resolver resolver1(io_serviceSend);
		tcp::resolver::query query(ipstr.c_str(),to_string(port1).c_str());
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
			boost::system::error_code ignored_error;
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
	
			boost::array<char, 128> buf;
			boost::system::error_code error;
	
			size_t len = socket1.read_some(boost::asio::buffer(buf), error);
			
			std::cout.write(buf.data(), len);
			
			if (message=="quit") break;		
			
		}
	}
	//----------------------------------------------------------------------------------
	CleanInput();
	return 0;
}