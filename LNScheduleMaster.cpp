/*
 * LNScheduleMaster.cpp
 *
 *  Created on: Sep 12, 2021
 *      Author: BrianRoot
 */

#include "LNScheduleMaster.h"

LNScheduleMaster::LNScheduleMaster() {
	// TODO Auto-generated constructor stub

}

LNScheduleMaster::~LNScheduleMaster() {
	// TODO Auto-generated destructor stub
}

int LNScheduleMaster::GetExecutionstatus()
{
	return Executionstatus;
}

int LNScheduleMaster::Getcurrentexecutionstep()
{
	return Currentexecutionstep;
}

void LNScheduleMaster::Setexecutionstatus(int status)
{
	Executionstatus = status;
}

void LNScheduleMaster::Setcurrentexecutionstep(int step)
{
	Currentexecutionstep = step;
}

int LNScheduleMaster::GetSchedulenumber()
{
	return Schedulenumber;
}

int LNScheduleMaster::Checkstartreadiness(int &stepready, int &Trainjourney, int &TotalstepsinSchedule, int scheduleticks)
{
	int readinessflag = 0;

	stepready = 0;

	Trainjourney = 0;

	TotalstepsinSchedule = 0;

	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	char* outputrecptr = &outputrec[0];

	LNSchedulestep CheckSchedulerStep;

	// check each step in the schedule master to see if it should be started


	for (std::vector<LNSchedulestep>::iterator DispSchStep = ScheduleMasterrecord.begin(); DispSchStep != ScheduleMasterrecord.end(); ++DispSchStep)
	{
		//CheckSchedulerStep = *DispSchStep;

		DispSchStep->DisplaySchedstep(outputrecptr);

		//std::cout<<"\n\n69 checking start readiness - Schedule step->"<<outputrec<<"\n";

		stepready = DispSchStep->checkstartconditions(scheduleticks);

		//stepready = CheckSchedulerStep.checkstartconditions(scheduleticks);

		if (stepready > 0)
		{
			Trainjourney 			= DispSchStep->GetTrainjourney();
			readinessflag 			= Schedulenumber;
			TotalstepsinSchedule 	= ScheduleMasterrecord.size();
			return readinessflag;
		}
	}

	return 0;	// return the number of the schedule if it is to be started
}

int LNScheduleMaster::DisplaySchedule(char* replymessage)
{
	LNSchedulestep DispSchedulerStep; // = new LNSchedulestep();

	char outputrec[OUTPUTMSGBUFFERSIZE];

	char SCHrecstep[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	char* SCHstepptr = &SCHrecstep[0];

	int isc,ic,scc,mcc;

	mcc = 0;

	int Numberofstoredschedulesteps = ScheduleMasterrecord.size();

	isc =  std::strlen(AutoSimdescription);

	*(replymessage + mcc) = '\n';
		mcc++;

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = AutoSimdescription[ic];
			mcc++;
	}

	*(replymessage + mcc) = '\n';
	mcc++;


	for (std::vector<LNSchedulestep>::iterator DispSchStep = ScheduleMasterrecord.begin(); DispSchStep != ScheduleMasterrecord.end(); ++DispSchStep)
	{
		DispSchedulerStep = *DispSchStep;

		bzero(SCHrecstep,OUTPUTMSGBUFFERSIZE);

		DispSchedulerStep.DisplaySchedstep(SCHstepptr);

		isc =  std::strlen(SCHrecstep);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = SCHrecstep[ic];
				mcc++;
		}
	}

	return 0;
}

LNSchedulestep* LNScheduleMaster::LinktoselectedSchedulestep(int &schedstepfound, int schedstepnum)
{
	LNSchedulestep* LNSchedstepsearch = new LNSchedulestep();

	schedstepfound = 0;

	//std::cout<<"\n132 - Searching for step:"<<schedstepnum;

	for (std::vector<LNSchedulestep>::iterator DispSchStep = ScheduleMasterrecord.begin(); DispSchStep != ScheduleMasterrecord.end(); ++DispSchStep)
	{
			*LNSchedstepsearch = *DispSchStep;

			if (LNSchedstepsearch->GetSchedStepnumber() == schedstepnum)
			{
				schedstepfound = 1;
				std::cout<<"\n141 - Found step:"<<schedstepnum;
				return LNSchedstepsearch;
			}

	}


	return LNSchedstepsearch;
}

int LNScheduleMaster::BuildnewSchedulefromfile(int Schednum,int stepnum, int stepjourney,int StHH, int StMM, int StSS, int StTicks)
{
	LNSchedulestep Schstep;

	Schstep.SaveStepdetail(Schednum,stepnum,stepjourney,StTicks, StHH, StMM, StSS);

	ScheduleMasterrecord.push_back(Schstep);

	std::cout<<"\n175 Saving schedule:"<<Schednum<<" step:"<<stepnum<<" stepjourney:"<<stepjourney;

	Schedulenumber 			= Schednum;

	NumberofstepsinSchedule = stepnum;

	Executionstatus			= 0;

	Currentexecutionstep	= 0;

	bzero(AutoSimdescription,LNSCHEDULEDESCRIPTIONLEN);

	sprintf(AutoSimdescription,"\nSchedule %02d - contains %02d steps", Schednum,stepnum);

	return 0;

}

int LNScheduleMaster::BuildnewSchedule(int schednum)
{
	std::cout<<"\nPrompting for schedule details.........";

	char Schardatareply[64];

	bzero(Schardatareply,64);

	char* Schardatareplyptr = &Schardatareply[0];

	LNCLidataentry* Schdata = new LNCLidataentry();

	int Schedstep = 0;

	int STJourney = 0;

	int SLRMmatchedflag = 0;

	int StHH = 0;

	int StMM = 0;

	int StSS = 0;

	int Stclicktarget = 0;

	int Sresponse;

	/*
	unsigned int Schedulenumber;
	unsigned int Schedulestep;
	unsigned int Journey;
	unsigned int PrereqJny;
	unsigned int Nextjourney;
	unsigned int StartHH;
	unsigned int StartMM;
	unsigned int StartSS;
	unsigned int Status;
	unsigned int Enabled;
	*/

	int runflag = 1;

	while(runflag == 1)
	{

		Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", Schedstep,"\nEnter Step number or '0' for end: ");

		if (Schedstep > 0)
			{

				Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", STJourney,"\nEnter TrainJourney number: ");

				Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", StHH,"\nEnter Step Start HH: ");

				Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", StMM,"\nEnter Step Start MM: ");

				Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", StSS,"\nEnter Step Start SS: ");

				Sresponse = Schdata->DataEntryrun(1,Schardatareplyptr," ", Stclicktarget,"\nEnter Scheduler ticks start number: ");

				// Create new Schedulestep object

				// AutoSimdescription[LNSCHEDULEDESCRIPTIONLEN];

				LNSchedulestep Schstep;

				Schstep.SaveStepdetail(schednum,Schedstep,STJourney,Stclicktarget, StHH, StMM, StSS);

				ScheduleMasterrecord.push_back(Schstep);
			}
		else
			{
				runflag = 0;
				std::cout<<"\n All steps entered.... ";
			}
	}

	int stepcount 			= ScheduleMasterrecord.size();

	Schedulenumber 			= schednum;

	NumberofstepsinSchedule = stepcount;

	Executionstatus			= 0;

	Currentexecutionstep	= 0;

	bzero(AutoSimdescription,LNSCHEDULEDESCRIPTIONLEN);

	sprintf(AutoSimdescription,"\nSchedule %02d - contains %02d steps", schednum,stepcount);

	delete Schdata;

	return 0;
}
