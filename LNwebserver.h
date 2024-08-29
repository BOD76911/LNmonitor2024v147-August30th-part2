/*
 * LNGwebserver.h
 *
 *  Created on: Jun 25, 2019
 *      Author: BrianRoot
 */

#ifndef LNWEBSERVER_H_
#define LNWEBSERVER_H_

#include "TCPprocessing.h"
#include "Stringprocessing.h"
#include "LNwebserver.h"
#include "LNwebLsectionsLdevices.h"
#include "LNwebconfig.h"
#include "LNwebmonSimulator.h"
#include "LNweblocorouteprocess.h"
#include "LNdisplayconfig.h"
#include "LNRFidsensorprocessing.h"
#include "LNRouteprocess.h"
#include "LNSchedulerExecutor.h"
#include "LNSimRouteMaster.h"
#include "LNSchedulerAlljourneys.h"
#include "LNlogging.h"


//struct itimerval timer;

class LNwebserver: public TCPprocessing
{
public:
	LNwebserver();
	virtual ~LNwebserver();

int WebServerrun(LNlogging* LNlogptr,LNweblocorouteprocess* LNLRprocess, LNwebmonSimulator* LNSIM, int port, \
		LNwebLsectionsLdevices* LNLSLD, MsgTimestamp* MSGptr, \
		LNdisplayconfig* LNDC, LNRFidsensorprocessing* LNRFIDSP, LNRouteprocess* LNRteproc,\
		LNSimRouteMaster* LNSimRteMaster, LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,LNSchedulerAlljourneys* LNSchedAlljourneys,\
		LNRteDevicereferencedata* LNRteDRfd,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNlocoAlldata* LNlocodatatables, bool *Tflagptr);

void Buildteststring();


private:

	int connFd;

	char TCPlogmsg[TCPMSGLEN];

	int pId, portNo, listenFd, n, msglen;

	int Filemsglength;

	//std::vector<char> Outputstream;

	std::string Headerl1 = "\HTTP/1.1 200 OK\n";
	std::string Headerl2 = "Server: LNTrainmanager Web Server\n";
	std::string Headerl3 = "Content-length: ";
	std::string Headerl4a = "\n";
	std::string Headerl5 = "Content-type:text/html\n";
	std::string Headerl6 = "\r\n";

	std::string Testmessage = "LNTrainManager Webserver test message";

};

#endif /* LNWEBSERVER_H_ */
