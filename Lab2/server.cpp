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
#include <ctime>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include "util.h"
//------------------Namespaces----------------------------------
using namespace std;
using boost::asio::ip::udp;
using boost::asio::deadline_timer;
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
//------------------Globals-------------------------------------
int nextport = 1338;
void tcp_threadFun();
//------------------Classes-------------------------------------
//------------------UDP-----------------------------------------
class udp_server
{
public:
  udp_server(boost::asio::io_service& io_service, int port)
    : socket_(io_service, udp::endpoint(udp::v4(), port))
  {
    start_receive();
  }

private:
  void start_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  string make_response(string name)
  {
  	char* username = (char*)calloc(100,1);
  	sscanf(name.c_str(),"MnxoidClient:%s",username);
  	string res = "Hello, ";
  	res+=string(username);
  	res+=", I am ready to listen on port ";
  	res+=to_string(nextport);
  	thread* thr = new thread(tcp_threadFun);
  	return string(res);
  }

  void handle_receive(const boost::system::error_code& error,
      std::size_t len)
  {
  	cout << "Message received! Length: "<< len << endl;
  	string got_mess(recv_buffer_.begin(), recv_buffer_.begin()+len);
  	//cout.write(recv_buffer_.data(), len);
  	cout << got_mess;
  	cout << endl;
    if (!error || error == boost::asio::error::message_size)
    {
      boost::shared_ptr<std::string> message(
          new string(make_response(got_mess)));

      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
          boost::bind(&udp_server::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

      start_receive();
    }
  }

  void handle_send(boost::shared_ptr<std::string> /*message*/,
      const boost::system::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, 256> recv_buffer_;
};
//------------------TCP-----------------------------------------
using boost::asio::ip::tcp;

std::string make_daytime_string(string orig_mess)
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  if (orig_mess != "")
  {
    return (string(ctime(&now)) + "Message received - ") + orig_mess + "\n";
  } else {
    return ctime(&now);
  }
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;
bool enabled = true;
  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }
 void check_deadline()
  {
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (dt.expires_at() <= deadline_timer::traits_type::now())
    {
      // The deadline has passed. The outstanding asynchronous operation needs
      // to be cancelled so that the blocked receive() function will return.
      //
      // Please note that cancel() has portability issues on some versions of
      // Microsoft Windows, and it may be necessary to use close() instead.
      // Consult the documentation for cancel() for further information.
      socket_.cancel();
      cout << "Timeout actually works" << endl;
      // There is no longer an active deadline. The expiry is set to positive
      // infinity so that the actor takes no action until a new deadline is set.
      dt.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    dt.async_wait(boost::bind(&tcp_connection::check_deadline, this));
  }
  void start()
  {
  	if(enabled)
  	{
      boost::system::error_code error;
    boost::array<char, 128> buf;
    
    dt.expires_from_now(boost::posix_time::seconds(60));
    size_t len = socket_.read_some(boost::asio::buffer(buf), error);
    string got_mess(buf.begin(), buf.begin()+len);
    
    dt.expires_at(boost::posix_time::pos_infin);
	    message_ = make_daytime_string(got_mess);

	    boost::asio::async_write(socket_, boost::asio::buffer(message_),
	        boost::bind(&tcp_connection::handle_write, shared_from_this(),
	          boost::asio::placeholders::error,
	          boost::asio::placeholders::bytes_transferred));
      cout << got_mess << endl;
      if(got_mess != "quit" && got_mess != "") start();
      else throw boost::system::system_error(1,boost::system::system_category());
	}
  }

private:
  tcp_connection(boost::asio::io_service& io_service1)
    : socket_(io_service1), dt(io_service1)
  {
    check_deadline();
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  	
  }

  tcp::socket socket_;
  std::string message_;
  deadline_timer dt;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service, int port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
      start_accept();
    }
  }

  tcp::acceptor acceptor_;
};
//------------------Other functions-----------------------------
void tcp_threadFun()
{
	try
  {
    boost::asio::io_service io_service;
    tcp_server server(io_service,nextport);
    nextport++;
    io_service.run();
  }
  catch (std::exception& e)
  {
    cout << "Client disconnected! \n" << endl;
    //std::cerr << e.what() << std::endl;
  }
}
//------------------Main function-------------------------------
int main(int argc, char* argv[])
{
	Disclaimer();
  try
	{
		boost::asio::io_service io_service;
		udp_server server(io_service,atoi(argv[1]));
    nextport = 1338+20*(atoi(argv[1])-1336);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	CleanInput();
	return 0;
}