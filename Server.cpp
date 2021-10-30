
#include "Server.h"

Server::Server(int port) throw (const char*) {
	fd = socket(AF_INET, SOCK_STREAM, 0);
	isRun = true;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	//htons convert to liitle\big endian
	server.sin_port = htons(port);
	//exeptions, same to lacturer code
	if(fd<0){
		throw "socket failed";
	}

	if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
	 {
		 throw "bind falure";
	 }

	 if(listen(fd,3)<0){
		 throw "listen failure";
	 }
};

void Server::start(ClientHandler& ch)throw(const char*){
	t = new thread([&ch,this](){	
	while(isRun){
	socklen_t size = sizeof(client);
	int aclient = accept(fd, (struct sockaddr *)&client, &size);
	if(aclient<0){
		throw "accept failure";
	}
	ch.handle(aclient);
	close(aclient);
	}
	});  
};

void Server::stop(){
	isRun = false;
	t->join(); // do not delete this!
	close(fd);
}

Server::~Server() {
}

