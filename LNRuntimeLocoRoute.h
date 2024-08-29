/*
 * LNRuntimeLocoRoute.h
 *
 *  Created on: Aug 28, 2021
 *      Author: BrianRoot
 */

#ifndef LNRUNTIMELOCOROUTE_H_
#define LNRUNTIMELOCOROUTE_H_

#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <vector>

#include <list>
#include <ctime>
#include <time.h>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <string>

#include "LNwebconfig.h"

class LNRuntimeLocoRoute {
public:
	LNRuntimeLocoRoute();
	virtual ~LNRuntimeLocoRoute();

	void StoreDCCcode(int code);

	void StoreTrainRoutecode(int TRcode);

	void StoreTraintype(int type);

	void StoreLoconumberforRun(int Loconumber);

	void StoreTrainnumber(int Trainnum);

	void StoreRoutenumber(int Route);

	void StoreJourneynumber(int LRjourney);

	void StoreStartdirection(int Sdirection);

	void StoreSimulationflag(int Simflag);

	int GetTrainRouteMovementNumber();

	int GetRouteNumber();

	int GetJourneyNumber();

	int GetDCCcodeNumber();

	int GetSlotNumber();

	int GetTrainNumber();

	int GetLoconumberforRun();

	int GetSimulationflag();

	int GetSLSpsection();

	int DisplayLRdata(char *replymessage);

	int StoreLocoRouterecord(int CS_SRRT_TrainRoutecode, int CS_SRRT_Traintype, \
			int CS_SRRT_LocoDCCcode, int CS_SRRT_DCCslotnumber, int CS_SRRT_LocoNumberforRun,int CS_SRRT_Trainnumber, \
			int CS_SRRT_Routenumber,int CS_SRRT_Journeynumber, int CS_SRRT_Start_Rtedirection, int CS_SRRT_Simulation, int CS_SRRT_SLSpsection);

private:

	// Locos running in simulation mode have a 9 as a prefix

	int SRRT_TrainRoutecode = 0;

	int SRRT_Traintype = 0;  // 01 = loco (running light), 02 = Loco with train -Freight, 03 = Loco with train passenger

	int SRRT_LocoDCCcode = 0;

	int SRRT_DCCslotnumber = 0;

	int SRRT_LocoNumberforRun = 0;;

	int SRRT_Trainnumber = 0;

	int SRRT_Routenumber = 0;

	int SRRT_Journeynumber = 0;

	int SRRT_Start_Rtedirection = 0;

	int SRRT_Simulation = 0;

	int SRRT_SLSpsection = 0;
};

#endif /* LNRUNTIMELOCOROUTE_H_ */
