/*
 * TCPprocessing.cpp
 *
 *  Created on: Jul 4, 2018
 *      Author: BrianRoot
 */

#include "TCPprocessing.h"
#include "LNwebconfig.h"
#include "Bufferprocessing.h"
#include "LNwebmonNGfileload.h"
#include "LNwebmonCommands.h"

TCPprocessing::TCPprocessing() {
	// TODO Auto-generated constructor stub

	//std::cout <<"\nStarting TCP object ";

}

TCPprocessing::~TCPprocessing() {
	// TODO Auto-generated destructor stub

	//std::cout <<"\nClosing TCP object ";
}

int TCPprocessing::Servermainrun(int port)
{
	//static int connFd;

	/* -- int connFd;

	int pId, portNo, listenFd, n, msglen; --*/

	    socklen_t len; //store size of the address

	    bool loop = false;

	    struct sockaddr_in svrAdd, clntAdd;


	    char test[WEBINPUTBUFFERSIZE];
	    char Webinboundbuffer[WEBINPUTBUFFERSIZE ];
	    bzero(test, WEBINPUTBUFFERSIZE);
	    bzero(Webinboundbuffer, WEBINPUTBUFFERSIZE );

	    char Outboundmessage[OUTPUTMSGBUFFERSIZE];

	    pthread_t threadA[3];

	    portNo = port;

	    //create socket

	        listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	        if(listenFd < 0)
	        {
	            std::cerr << "Cannot open socket" << std::endl;
	            return 98;
	        }

	        bzero((char*) &svrAdd, sizeof(svrAdd));

	        svrAdd.sin_family = AF_INET;
	        svrAdd.sin_addr.s_addr = INADDR_ANY;
	        svrAdd.sin_port = htons(portNo);

	        //bind socket
	        if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
	        {
	            std::cerr << "\nCannot bind at Port " << portNo << "\n " << std::endl;
	            return 99;
	        }

	        listen(listenFd, 5);

	        len = sizeof(clntAdd);


	        //Fileloader* FLserver = new Fileloader();

	        //TMCommands* TMserver = new TMCommands();					 // Create the command processing object

	        //FLserver->Fileread("Commands.dat",0);

	        //for (int cm = 1; cm <= FLserver->GetNumberofDatarecordsloaded();cm++)
	        //	 {
	        //	 	TMserver->Storecommands(FLserver->GetRecordsFieldValue(cm,1),FLserver->GetRecordFieldtext(cm,2),FLserver->GetRecordFieldtext(cm,4));
	        //	 }

	        //TMserver->Storecommandcount(FLserver->GetNumberofDatarecordsloaded());

	        //delete FLserver;

	        while (!loop) //&& (TMRunManA.GetGlobalflagval() == 0))
	           {
	               std::cout << "\nListening on port " << portNo <<std::endl;

	               //this is where client connects. svr will hang in this mode until client conn
	               connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

	               if (connFd < 0)
	               {
	                   std::cout << "Cannot accept connection";
	                   return 97;
	               }
	               else
	               {
	                   //std::cout << "Connection successful";
	               }

	               bzero(Webinboundbuffer,WEBINPUTBUFFERSIZE);

	               n = read(connFd, Webinboundbuffer, 1024);

	               printf("\nMsg in %s ",Webinboundbuffer);

	               Bufferprocessing* BP = new Bufferprocessing();

	               BP->Dataparse(Webinboundbuffer,' ',0);  // parse the command received

	               char* Paramchar;

	               Paramchar = BP->Paramdata(0);

	               if ((*Paramchar == 'x')|| (*(Paramchar+1) == 'x'))
	               {
	            	   std::cout << "LNTrainManager Server quit command received ";

	            	   write(connFd, "LNTrainmanager Server closing \n", 17);

	            	   close(connFd);

	            	   break;
	               }

	               BP->Dataparse(Webinboundbuffer,' ',0);  // parse the command received

	               /*

	               if (TMserver->Commandprocess(BP->Paramdata(0)) < 98)
	               		{
	               			CommandExecute* CEserver = new CommandExecute();

	               			CEserver->GetTMCommands(*TMserver);

	               			CEserver->ClearOutputmessage();

	               			char *Webmsgptr = &Webinboundbuffer[0];

	               			sprintf(Outboundmessage,"\n%s",CEserver->Runcommand(Schedlist,JnyList,DSPconfiglist,RLSG,LSRC,DDCF, TMRunManA,TMserver->Commandprocess(BP->Paramdata(0)),Webinboundbuffer));

	               			msglen = 0;

	               			for (int icc = 0;icc < OUTPUTMSGBUFFERSIZE;icc++)
	               			{
	               				if (Outboundmessage[icc] == 0X0D)
	               				{
	               					msglen++;
	               					break;
	               				}
	               				else
	               				{
	               					msglen++;
	               				}
	               			}

	               			write(connFd, Outboundmessage,msglen);

	               			delete CEserver;
	               		}

	               std::cout << "\n";  */

	               write(connFd, "\nRead OK\n", 8);

	               close(connFd);
	           }

	        std::cout << "\n\nServer Closing....";

	       // TMRunManA.SetGlobalflagval(89);

	        return 0;
}


int TCPprocessing::Clientrun(int Rteport, char *Messagereceived, char *outboundmsg, char *servernumber, int waitflag)
{
    int sockfd, portno, n,rc;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char* Messagereceivedptr = Messagereceived;

    rc = 0;

	char buffer[255];
	char Mbuffer[WEBINPUTBUFFERSIZE];
    char LNDatabuffer[WEBINPUTBUFFERSIZE];

    portno = Rteport;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "\nERROR opening socket";
    server = gethostbyname(servernumber);
    if (server == NULL) {
        std::cout << "\nERROR, no such host\n";
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        std::cout <<"\nERROR connecting";
        return 99;
    }

    int charcount,chartarget, msglen;

		bzero(buffer,255);

		bzero(Mbuffer,WEBINPUTBUFFERSIZE);

		strcpy(Mbuffer,outboundmsg);

		strcat(Mbuffer," \n\r\n");

		//printf("\nClient sends :%s\n",Mbuffer);

		n = write(sockfd,Mbuffer,strlen(Mbuffer));

		if (n < 0)
		{
         std::cout << "\nERROR writing to socket";
         return 98;
		}

		bzero(buffer,WEBINPUTBUFFERSIZE);

		bzero(LNDatabuffer,WEBINPUTBUFFERSIZE);


		charcount = 0;

		if (waitflag == 0)
		{
			chartarget = WEBINPUTBUFFERSIZE;  	// maximum characters to be read
		}
		else
		{
			chartarget = 15;
		}

		while (charcount < chartarget)  // read characters one byte at a time to avoid the read stopping early
		{
			n = read(sockfd,buffer,1);
			if (n < 0)
			{
			std::cout<< "\nERROR reading from socket";
			}
			else
			{
			if (buffer[0] == '\r')
				{
					break;
				}

			LNDatabuffer[charcount] = buffer[0];
			*(Messagereceivedptr +charcount) = buffer[0];
			bzero(buffer,255);

			//if (buffer[0] == '\r')
			//	{
			//		break;
			//	}
			}
			++charcount;
		}

		++charcount;
		LNDatabuffer[charcount] = '\0';

		close(sockfd);

		//std::cout <<"\nMsg back :"<< LNDatabuffer;

		if (Messagecheck(LNDatabuffer, "SchedOK") == 0)
		{
			std::cout<<"\n\nSchedule uploaded OK";
			rc = 0;
		}
		else
		{
			rc = 1;
			//std::cout<<"\n\nSchedule uploaded FAILED :"<< LNDatabuffer;
		}

		if (waitflag != 0)  // if waitflag > 0, do not parse the message received
		{
			return rc;
		}

    return rc;
}

int TCPprocessing::Messagecheck(char* msg1, char* msg2)
{

	// compares msg1 to msg 2 . If they are the same if return 0 else returns 1

	int sc, rc, cc;

	rc = 0;

	sc = 0;

	cc = 1;

	while (sc < 1)
	{
		//printf("\n char %i  msg1 %c  msg2 %c ",cc,*msg1,*msg2);

		if (*msg1 != '\0')
		{
			if (*msg1 != *msg2)
			{
				rc = 1;
			}

			msg1++;
			msg2++;
			cc++;
		}
		else
		{
			sc = 1;
		}

	}

	return rc;
}

