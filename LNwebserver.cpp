/*
 * LNGwebserver.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: BrianRoot
 */

#include "LNwebserver.h"

#include <string>
#include <iostream>
#include <string.h>

#include "Bufferprocessing.h"
#include "Webmessagereply.h"
#include "LNwebconfig.h"
#include "Clocktime.h"


LNwebserver::LNwebserver() {
	// TODO Auto-generated constructor stub

}

LNwebserver::~LNwebserver() {
	// TODO Auto-generated destructor stub
}


void LNwebserver::Buildteststring()
{




}


int LNwebserver::WebServerrun(LNlogging* LNlogptr, LNweblocorouteprocess* LNLRprocess, LNwebmonSimulator* LNSIM, int port, \
		LNwebLsectionsLdevices* LNLSLD, MsgTimestamp* MSGptr, LNdisplayconfig* LNDC, \
		LNRFidsensorprocessing* LNRFIDSP,  LNRouteprocess* LNRteproc,LNSimRouteMaster* LNSimRteMaster,\
		LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,LNSchedulerAlljourneys* LNSchedAlljourneys,\
		LNRteDevicereferencedata* LNRteDRfd,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG,LNlocoAlldata* LNlocodatatables, bool *Tflagptr)
{
	socklen_t len; //store size of the address

	bool loop = false;

	struct sockaddr_in svrAdd, clntAdd;

	char Searchstringresult[64];

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

		        	std::cerr << "\n\n ****** Cannot bind to Socket *******\n\n" << std::endl;
		            return 99;
		        }

		        bzero(TCPlogmsg,TCPMSGLEN);

		        sprintf(TCPlogmsg,"LNwebserver - master TCP socket listening on port %i",portNo);

		        LNlogptr->Writelogmessage(1,TCPlogmsg);

		        listen(listenFd, 5);

		        len = sizeof(clntAdd);

		        while ((!loop) && (Tflagptr[2] == true))
		           {
		              //std::cout << "\nWEB server listening on port " << portNo <<std::endl;

		               //this is where client connects. svr will hang in this mode until client conn

		               connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

		               if (connFd < 0)
		               {
		                   std::cout << "Cannot accept connection";
		                   return 97;
		               }
		               else
		               {						   
						   					   
		            	  bzero(TCPlogmsg,TCPMSGLEN);

		            	  sprintf(TCPlogmsg,"LNwebserver - new connection socket fd is:  %d ip is: %s port: %d",connFd,inet_ntoa(clntAdd.sin_addr),ntohs (clntAdd.sin_port)); //, ip is : %s , port : %d " , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));

		            	  //std::cout<<"\nConnection "<<TCPlogmsg;
		            	  
		            	  // LNlogptr->Writelogmessage(1,TCPlogmsg);
		               }

		               bzero(Webinboundbuffer,WEBINPUTBUFFERSIZE);

		              // timeout.tv_sec = 0;

		              // timeout.tv_usec = 10000;

		               n = read(connFd, Webinboundbuffer, WEBINPUTBUFFERSIZE);  // 1024

		              // std::cout << "\n113 Inbound message at Webserver :" << Webinboundbuffer;

		               Stringprocessing STRproc;

		               Bufferprocessing* BP = new Bufferprocessing();

		               BP->Dataparse(Webinboundbuffer,' ',0);  // parse the command received

		               char* Paramchar;

		               Paramchar = BP->Paramdata(0);  // setup pointer to first parameter found

		               	    if ((*Paramchar == 'q')|| (*(Paramchar+1) == 'q'))
		               	          {
		               	         	 std::cout << "\nWeb Server quit command received ";

		               	         	 write(connFd, "Server closing \n", 17);

		               	             close(connFd);

		               	             break;
		               	          }

		              //std::cout << "\nCompare GET check result " << BP.CharArraytoStringcompare(Paramchar, "GET",0);  // check to see if call is from Browser GET


		               //std::cout<<"\nWebserver 138 msgin:"<<Webinboundbuffer<<" \n";


		               if (BP->CharArraytoStringcompare(Paramchar, "GET",0) == 0)
		               {
		            	   // HTTP GET command processing

		            	   std::string response;

		            	   int txtmsglen = Testmessage.length();

		            	   //response = Headerl1 + Headerl2 + Headerl3;

		            	   int responselen = response.length();

		            	   char Mbuffer[WEBSERVERBUFFERSIZE];

		            	   bzero(Mbuffer,WEBSERVERBUFFERSIZE);

		            	   char GetFilename[3048];

		            	   char* GetFilenameptr = &GetFilename[0];

		            	   bzero(GetFilename,3048);

		            	   char* Webinboundptr = &Webinboundbuffer[0];

		            	   STRproc.Searchtext(*GetFilenameptr,"/",*Webinboundptr,0,' ',1024);

		            	   	Webmessagereply* WMR = new Webmessagereply();

		            		char* MessageOutptr = &Mbuffer[0];

		            		int* connFDptr = &connFd;

		            	   	//WMR.Buildmessage(*MessageOutptr,"http://localhost:8101/Index.html",0);

		            		//std::cout<<"\nWebserver 175 calling Buildmessage with GET command\n";

		            	    WMR->Buildmessage(*connFDptr ,*MessageOutptr,GetFilename,0,LNLSLD,MSGptr,LNSIM,LNSimRteMaster, LNLRprocess,LNDC,LNSchedExecutor,LNRteSchedMast,LNSchedAlljourneys,LNRtimeallrecsNG,LNlocodatatables);

		            	    responselen = WMR->GetFinalmessagelength();

		            	  // std::cout << "\n\n207 WMR ->GET Webmessage reply length BACK----> : "<< responselen;

		            	   write(connFd, Mbuffer, responselen);

		            	   delete WMR;

		            	  // delete[] Mbuffer;
		               }
		               else
		               {

		            	   if (BP->CharArraytoStringcompare(Paramchar, "PUT",0) == 0)
		            	   {
		            	   // HTTP PUT command processing

		            	   std::string response;

		            	   //std::cout<<"\nWebserver 217 processing PUT command\n"<<Webinboundbuffer;;

		            	   int txtmsglen = Testmessage.length();

		            	   //response = Headerl1 + Headerl2 + Headerl3;

		            	   int responselen = response.length();

		            	   char Mbuffer[WEBSERVERBUFFERSIZE];

		            	   bzero(Mbuffer,WEBSERVERBUFFERSIZE);

		            	   char GetFilename[3048];

		            	   char* GetFilenameptr = &GetFilename[0];

		            	   bzero(GetFilename,3048);

		            	   char* Webinboundptr = &Webinboundbuffer[0];

		            	   STRproc.Searchtext(*GetFilenameptr,"/",*Webinboundptr,0,' ',2048);

		            	   Webmessagereply* WPPR = new Webmessagereply();

		            	   char* MessageOutptr = &Mbuffer[0];

		            	   int* connFDptr = &connFd;

		            	   WPPR->Processmessage(*connFDptr ,*MessageOutptr,GetFilename,0,LNLSLD,MSGptr,LNSIM,LNLRprocess,LNDC,\
		            			   LNRFIDSP,LNSchedExecutor,LNRteSchedMast, LNRteDRfd);

		            	   responselen = WPPR->GetFinalmessagelength();

		            	//   std::cout << "\n\nPUT Part 1 sent BACK :" << Mbuffer << " len:" << responselen;

		            	   write(connFd, Mbuffer, responselen);

		            	  delete WPPR;

		            	   }

		            	   else
		            	   {
		            	   std::cout << "\nUnknown Inbound message at Webserver :" << Webinboundbuffer;
		            	   int responselen;
		             	   char Mbuffer[2048];

		            	   strcpy (Mbuffer,"\nRFC test call \n");
		            	   responselen = strlen(Mbuffer);
		            	   write(connFd, Mbuffer, responselen);
		            	  // delete[] Mbuffer;
		            	   }
		               }

		               delete BP;

		               close(connFd);

		               //printf("\nMsg in %s ",Webinboundbuffer);
		           }

		       // close(connFd);

		        std::cout << "\n\nThread flag 2 set to " << Tflagptr[2];

		        std::cout << "\n\nWeb Server Closing....";

		        std::cout << " " ;

		        //TMRunManA.SetGlobalflagval(89);

		        return 0;
}
