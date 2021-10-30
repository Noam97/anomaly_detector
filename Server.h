/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "commands.h"
#include "CLI.h"
#include <thread>

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <cstring>  ///??
#include <string.h> //?
#include <stdio.h>
#include <stdlib.h>


using namespace std;

class ClientHandler
{
public:
    virtual void handle(int clientID)=0;
};

// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	bool isRun = true;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

class StandardIO: public DefaultIO{
	//functions from MainTrain of Ex5
	//function to read and write
 	string read(){
		string s;
		cin>>s;
		return s;
	}
	 void write(string text){
		cout<<text<<endl;
	}

	 void write(float f){
		cout<<f<<endl;
	}

	 void read(float* f){
		cin>>*f;
	}
};

//class SocketIO write and read
class SocketIO : public DefaultIO
{
	int clientID; 
public:
	SocketIO(int clientID) : DefaultIO(){
		this->clientID = clientID;
	}

	void write(string s){
		send(clientID, s.c_str(), strlen(s.c_str()), 0);
	}

	void write(float f)
	{
		string s = to_string(f);
		s.erase(s.find_last_not_of('0') + 1, std::string::npos); 
		write(s);
	}

	string read()
	{
		string s;
		char chars = 0;
		::read(clientID, &chars, sizeof(char));
		while (chars != '\n')
		{
			s += chars;
			::read(clientID, &chars, sizeof(char));
		}
		return s;
	}

	void read(float *f)
	{
		 ::read(clientID, f, sizeof(float));
	}
};


class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID)
	{
		//create socket and and send it to cli. 
		SocketIO socket(clientID);   
		CLI cli(&socket);
		cli.start();
	}
};




#endif /* SERVER_H_ */
