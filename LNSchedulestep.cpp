/*
 * LNSchedulestep.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: BrianRoot
 */

#include "LNSchedulestep.h"

LNSchedulestep::LNSchedulestep() {
	// TODO Auto-generated constructor stub

}

LNSchedulestep::~LNSchedulestep() {
	// TODO Auto-generated destructor stub
}

int LNSchedulestep::GetSchedStepnumber()
{
	return Schedulestep;
}

void LNSchedulestep::SaveStepdetail(int schedule, int step, int Trainjourneynumber,int startclickcnt, int StarttimeHH, int StarttimeMM, int StarttimeSS)
{
	Schedulenumber 		= schedule;
	Schedulestep 		= step;
	Journey 			= Trainjourneynumber;
	Startclicktarget 	= startclickcnt;
	StartHH				= StarttimeHH;
	StartMM				= StarttimeMM;
	StartSS				= StarttimeSS;
}

int LNSchedulestep::GetTrainjourney()
{
	return Journey;
}

int LNSchedulestep::checkstartconditions(int tickcounter)
{
	int schedstepready = 0;

	//std::cout<<"\n66 Checking Start conditions Enabled flag:"<<Enabled<<" Status flag:"<<Status;

	//std::cout<<"\n\n46 Schedule details Sch Number:"<<Schedulenumber<<" Step:"<<Schedulestep<<" Startclicktarget: "<<Startclicktarget;


	if (Startclicktarget == tickcounter) 		// tick counter criteria meet and step not enabled
	{
		//std::cout<<"\n\n51 MATCHED on Sch Number:"<<Schedulenumber<<" Step:"<<Schedulestep<<" Startclicktarget: "<<Startclicktarget<<"\n";

		schedstepready = Schedulestep;

		return schedstepready;
	}
	return 0;
}

int LNSchedulestep::DisplaySchedstep(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE * 4];

	int isc,ic,scc,mcc;

	mcc = 0;

	/*
	unsigned int Schedulenumber;
	unsigned int Schedulestep;
	unsigned int Schedulesubstep;
	unsigned int Journey;
	unsigned int PrereqJny;
	unsigned int Nextjourney;
	unsigned int StartHH;
	unsigned int StartMM;
	unsigned int StartSS;
	unsigned int Status;
	unsigned int Enabled;

	*/
	bzero(outputrec,OUTPUTRECSIZE * 4);

	sprintf(outputrec,"\nSched:%02d Step:%02d Journey:%02d Startticks: %03d StHH:%02d StMM:%02d StSS:%02d ",\
			Schedulenumber,Schedulestep,Journey,Startclicktarget, StartHH,StartMM,StartSS);

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
			*(replymessage + mcc) = outputrec[ic];
					mcc++;
	}

	return isc;
}

void LNSchedulestep::Initializestep()
{
	Schedulenumber 		= 0;
	Schedulestep  		= 0;
	Journey 			= 0;
	Startclicktarget 	= 0;
	StartHH 			= 0;
	StartMM 			= 0;
	StartSS 			= 0;
}
