/*
 * Webmessagereply.h
 *
 *  Created on: Jul 25, 2018
 *      Author: BrianRoot
 */

#ifndef WEBMESSAGEREPLY_H_
#define WEBMESSAGEREPLY_H_

#include <string.h>
#include <string>
#include <iomanip>
#include <locale>
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include "LNwebLsectionsLdevices.h"
#include "LNwebmonSimulator.h"
#include "LNweblocorouteprocess.h"
#include "LNdisplayconfig.h"
#include "LNRFidsensorprocessing.h"
#include "LNSchedulerExecutor.h"


#include "TCPprocessing.h"

class Webmessagereply {
public:
	Webmessagereply();
	virtual ~Webmessagereply();

	//void Buildmessage(int& conn, char& message, char* filename, int displayflag, Displayconfiglist* DSPconfiglist,Logicalsectionrecord* LSRC,Logicaldetector_record* DDCF);

	int GetFinalmessagelength();

	void Processmessage(int& conn, char& message, char* filename, int displayflag, LNwebLsectionsLdevices* LNLSLD,MsgTimestamp* MSGptr, \
			LNwebmonSimulator* LNSIM,LNweblocorouteprocess* LNLRprocess,LNdisplayconfig* LNDC, LNRFidsensorprocessing* LNRFIDSP,\
			LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,LNRteDevicereferencedata* LNRteDRfd);

	void Buildmessage(int& conn, char& message, char* filename, int displayflag, LNwebLsectionsLdevices* LNLSLD,MsgTimestamp* MSGptr, \
			LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster, LNweblocorouteprocess* LNLRprocess,LNdisplayconfig* LNDC,LNSchedulerExecutor* LNSchedExecutor,\
			LNRouteSchedMaster* LNRteSchedMast,LNSchedulerAlljourneys* LNSchedAlljourneys,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNlocoAlldata* LNlocodatatables);

protected:

	std::string Hline1 = "\HTTP/1.1 200 OK\n";
	std::string Hline2 = "Server: LNTrainmanager Web Server\n";
	std::string Hline3 = "Content-length: ";
	std::string Hline4 = "\nContent-type:";

	std::string Browser_response_part1 = "\HTTP/1.1 200 OK\n";
	std::string Browser_response_part2 = "Server: LNTrainmanager Web Server\n";
	std::string Browser_response_part3 = "Content-length: ";
	std::string Browser_response_part4 = "\nContent-type:";

	std::string Browser_response_datastream = "";

	int Hlinelen;

	int File_processingcode;

	int File_errorcode;

	int File_typecode;

	int File_toolarge;

	std::string File_toolargestr;

	int Hlinemsglen;

	int Finalmessagelength;

	std::string Hlmsglenstr;

	std::string HlmsgHTMLtype = "text/html\n\r\n";	;
	std::string HlmsgCSStype = "text/css\n\r\n";
	std::string HlmsgJStype = "text/js\n\r\n";
	std::string HlmsgJPGtype = "image/jpg\n\r\n";
	std::string HlmsgTXTtype = "text/plain\n\r\n";

	std::string Hlmsgtype;

	std::string Filemissingmsgpt1 = "File ";
	std::string Filemissingmsgpt2 = " ";
	std::string Filemissingmsgpt3 = " is not found ";
};


//strcpy (Mbuffer,"\HTTP/1.1 200 OK\n");
//strcat (Mbuffer,"Server: LNTrainmanager Web Server\n");
//strcat (Mbuffer,"Content-length: 37\nContent-type:text/html\n\r\n");
//strcat (Mbuffer,"LNTrainManager Webserver test message\0");


#endif /* WEBMESSAGEREPLY_H_ */
