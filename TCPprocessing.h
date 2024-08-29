/*
 * TCPprocessing.h
 *
 *  Created on: Jul 4, 2018
 *      Author: BrianRoot
 *
 *      Handles the server and client connections
 */

#ifndef TCPPROCESSING_H_
#define TCPPROCESSING_H_

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <fcntl.h>

#include "LNwebconfig.h"

class TCPprocessing {

public:

	TCPprocessing();

	virtual ~TCPprocessing();

	int Servermainrun(int port);

	int Clientrun(int Rteport, char *Messagereceived, char *outboundmsg, char *servernumber, int waitflag);

	int Messagecheck(char* msg1, char* msg2);

private:

	int connFd;

	int pId, portNo, listenFd, n, msglen;

};

#endif /* TCPPROCESSING_H_ */

