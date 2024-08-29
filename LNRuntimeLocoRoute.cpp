/*
 * LNRuntimeLocoRoute.cpp
 *
 *  Created on: Aug 28, 2021
 *      Author: BrianRoot
 */

#include "LNRuntimeLocoRoute.h"

LNRuntimeLocoRoute::LNRuntimeLocoRoute() {
	// TODO Auto-generated constructor stub

}

LNRuntimeLocoRoute::~LNRuntimeLocoRoute() {
	// TODO Auto-generated destructor stub
}

void LNRuntimeLocoRoute::StoreTrainRoutecode(int TRcode)
{
	SRRT_TrainRoutecode = TRcode;
}

int LNRuntimeLocoRoute::GetTrainRouteMovementNumber()
{
	return SRRT_TrainRoutecode;

}

int LNRuntimeLocoRoute::GetRouteNumber()
{
	return SRRT_Routenumber;

}

int LNRuntimeLocoRoute::GetJourneyNumber()
{
	return SRRT_Journeynumber;

}

int LNRuntimeLocoRoute::GetDCCcodeNumber()
{
	return SRRT_LocoDCCcode;

}


int LNRuntimeLocoRoute::GetSlotNumber()
{
	return SRRT_DCCslotnumber;

}

int LNRuntimeLocoRoute::GetTrainNumber()
{
	return SRRT_Trainnumber;

}

int LNRuntimeLocoRoute::GetLoconumberforRun()
{
	return SRRT_LocoNumberforRun;

}

int LNRuntimeLocoRoute::GetSimulationflag()
{
	return SRRT_Simulation;

}

int LNRuntimeLocoRoute::GetSLSpsection()
{
	return SRRT_SLSpsection;
}



int LNRuntimeLocoRoute::StoreLocoRouterecord(int CS_SRRT_TrainRoutecode,int CS_SRRT_Traintype, \
		int CS_SRRT_LocoDCCcode, int CS_SRRT_DCCslotnumber, int CS_SRRT_LocoNumberforRun,int CS_SRRT_Trainnumber, \
		int CS_SRRT_Routenumber,int CS_SRRT_Journeynumber, int CS_SRRT_Start_Rtedirection, int CS_SRRT_Simulation, int CS_SRRT_SLSpsection)
{

	/* SRRT_TrainRoutecode = 0;;

		int SRRT_Traintype = 0;  // 01 = loco (running light), 02 = Loco with train -Freight, 03 = Loco with train passenger

		int SRRT_LocoDCCcode = 0;

		int SRRT_DCCslotnumber = 0;

		int SRRT_LocoNumberforRun = 0;;

		int SRRT_Trainnumber = 0;

		int SRRT_Routenumber = 0;

		int SRRT_Journeynumber = 0;

		int SRRT_Start_Rtedirection = 0;

		int SRRT_Simulation = 0;

		*/

	SRRT_TrainRoutecode = CS_SRRT_TrainRoutecode;

	SRRT_Traintype = CS_SRRT_Traintype;

	SRRT_LocoDCCcode = CS_SRRT_LocoDCCcode;

	SRRT_DCCslotnumber = CS_SRRT_DCCslotnumber;

	SRRT_LocoNumberforRun = CS_SRRT_LocoNumberforRun;

	SRRT_Trainnumber = CS_SRRT_Trainnumber;

	SRRT_Routenumber = CS_SRRT_Routenumber;

	SRRT_Journeynumber = CS_SRRT_Journeynumber;

	SRRT_Start_Rtedirection = CS_SRRT_Start_Rtedirection;

	SRRT_Simulation = CS_SRRT_Simulation;

	SRRT_SLSpsection =  CS_SRRT_SLSpsection;

	return 0;
}

int LNRuntimeLocoRoute::DisplayLRdata(char *replymessage)
{
	char outputrec[OUTPUTRECSIZE * 2];

	bzero(outputrec,OUTPUTRECSIZE * 2);

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,OUTPUTRECSIZE);

	sprintf(outputrec,"\nTrainroute: %02d Train type: %i LocoDCCcode:%04d DCCslotcode: %i LocoNumforRun: %i TrainNumber: %02d Routenumber \ "
			"%02d JourneyNumber:%i Route direction: %i Simulationflag: %i StartPsection %i", SRRT_TrainRoutecode, SRRT_Traintype, SRRT_LocoDCCcode, SRRT_DCCslotnumber, \
						SRRT_LocoNumberforRun, SRRT_Trainnumber, SRRT_Routenumber, SRRT_Journeynumber, SRRT_Start_Rtedirection, SRRT_Simulation, SRRT_SLSpsection);


	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
					{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
					}
}
