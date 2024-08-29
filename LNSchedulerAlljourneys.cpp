/*
 * LNSchedulerAlljourneys.cpp
 *
 *  Created on: Sep 7, 2021
 *      Author: BrianRoot
 */

#include "LNSchedulerAlljourneys.h"

LNSchedulerAlljourneys::LNSchedulerAlljourneys() {
	// TODO Auto-generated constructor stub

}

LNSchedulerAlljourneys::~LNSchedulerAlljourneys() {
	// TODO Auto-generated destructor stub
}

void LNSchedulerAlljourneys::EnableScheduleExecution(int schedulenumber)
{
	SchedMaster[schedulenumber].ScheduleEnabled = 1;
}

void LNSchedulerAlljourneys::DisableScheduleExecution(int schedulenumber)
{
	SchedMaster[schedulenumber].ScheduleEnabled = 0;
}

void LNSchedulerAlljourneys::IncrementNumberofSchedulesinTable(int nn)
{
	NumberofSchedulesinTable = NumberofSchedulesinTable + nn;
}

int LNSchedulerAlljourneys::GetNumberofSchedulesinTable()
{
	return NumberofSchedulesinTable;
}

int LNSchedulerAlljourneys::InitializeScheduleStartNG(int Inputschedule, LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,\
		LNwebLsectionsLdevices* LNLSLD, LNRouteSchedMaster* LNRteSchedMast)
{

	// Prepare list to track locos scheduled

	#define SCHEDULEDLOCOCOUNT		8

	int Locosetuplist[SCHEDULEDLOCOCOUNT];

	int Numberoflocosprocessed = 0;

	int initjourney 		= 0;
	int initRoute			= 0;
	int initPS				= 0;
	int initLS				= 0;
	int initLocoDCCcode		= 0;
	int initlocoAlreadyset 	= 0;

	for (int lsc = 0; lsc < SCHEDULEDLOCOCOUNT; lsc++)
	{
		Locosetuplist[lsc] = 0;
	}

	for (int Schedinitcc = 1; Schedinitcc < MAXNUMBEROFSCHEDULESRECS; Schedinitcc++)
		{
			if (SchedMaster[Schedinitcc].Schedulenumber == Inputschedule)			// check that there is a schedule to update
			{
				for (int Schedstepcc = 1; Schedstepcc < MAXNUMBEROFSCHEDULESTEPS; Schedstepcc++)
				{
					if (SchedMaster[Schedinitcc].Schedulestepsjourneycode[Schedstepcc] > 0)		// check there is a valid journey to process on a step
					{
						std::cout<<"\n71 Initialization action for Sched:"<<Schedinitcc<<" Step:"<<Schedstepcc;

						// extract the journey code

						initjourney = SchedMaster[Schedinitcc].Schedulestepsjourneycode[Schedstepcc];

						// fetch the route code for the journey

						initRoute = LNRtimeallrecsNG->GetRouteNum(initjourney);

						initPS = LNRtimeallrecsNG->GetSLSpsection(initjourney);

						// fetch the DCC code

						initLocoDCCcode = LNRtimeallrecsNG->GetDCCcodeNumber(initjourney);

						initLS = LNRteSchedMast->GetCSvalue(initRoute, 1, 1);

						// check to see if loco has already been setup

						initlocoAlreadyset = 0;		// clear flag before starting search

						std::cout<<"\n93 checking for DCC code "<<initLocoDCCcode;

						for (int lsc = 0; lsc < SCHEDULEDLOCOCOUNT; lsc++)
						{
							if (Locosetuplist[lsc] ==  initLocoDCCcode)
							{
								initlocoAlreadyset = 1;   // flag loco as already setup
								std::cout<<"\n100 Loco already setup so no action taken ";
								break;
							}
						}

						if (initlocoAlreadyset == 0)
						{
							Locosetuplist[Numberoflocosprocessed] = initLocoDCCcode;
							Numberoflocosprocessed++;
						}
						else
						{					// skip setup as loco already setup in previous step

						}

						if (initlocoAlreadyset == 0)
						{
							std::cout<<"\n63 Journey:"<<initjourney<<" Route:"<<initRoute<<" PS:"<<initPS<<" DCC:"<<initLocoDCCcode<<" Init LS:"<<initLS;

							// initialize PS section to occupied and move DCCC code to LS section

							PSectionset(initPS,1);

							LNLSLD->RefreshLastupdatedetails(1);

							// move Loco code to LS section

							LNLSLD->UpdateTcode(initLS,initLocoDCCcode);

							//

							LNLSLD->LogicalSectionTcodeRefresh();

							LNLSLD->LogicalDetectorupdateDetectorstatus();

							LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover
						}

					}
				}
			}
		}

	// Close all point switches

	LNLSLD->Closeallswitches();

	LNLSLD->LogicalSectionTcodeRefresh();

	LNLSLD->LogicalDetectorupdateDetectorstatus();

	LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

	std::cout<<"\n147 Scheduler reset - All switches closed";

	std::cout<<"\n150 Schedule "<<Inputschedule<<" initialization complete";


	return 0;
}

int LNSchedulerAlljourneys::InitializeScheduleStart(LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,\
		LNwebLsectionsLdevices* LNLSLD, LNRouteSchedMaster* LNRteSchedMast)
{
	int initjourney 	= 0;
	int initRoute		= 0;
	int initPS			= 0;
	int initLS			= 0;
	int initLocoDCCcode	= 0;

	// Clear all Psections

	PSectionreset();

	InitTCDbuffers();

	//std::cout<<"\n44 Scheduler reset - All Psections cleared";

	// clear all current LS section loco codes

	LNLSLD->ClearTcodes();

	//std::cout<<"\n50 Scheduler reset - All LSsections cleared";

	// Close all point switches

	LNLSLD->Closeallswitches();

	//std::cout<<"\n56 Scheduler reset - All switches closed";

	// Read all Schedule route details and extract the details of the PS and LS sections to be initialized in order to run the schedule

	for (int Schedinitcc = 1; Schedinitcc < MAXNUMBEROFSCHEDULESRECS; Schedinitcc++)
	{
		if (SchedMaster[Schedinitcc].Schedulenumber > 0)			// check that there is a schedule to update
		{
			for (int Schedstepcc = 1; Schedstepcc < MAXNUMBEROFSCHEDULESTEPS; Schedstepcc++)
			{
				if (SchedMaster[Schedinitcc].Schedulestepsjourneycode[Schedstepcc] > 0)		// check there is a valid journey to process on a step
				{
					std::cout<<"\n39 Initialization action for Sched:"<<Schedinitcc<<" Step:"<<Schedstepcc;

					// extract the journey code

					initjourney = SchedMaster[Schedinitcc].Schedulestepsjourneycode[Schedstepcc];

					// fetch the route code for the journey

					initRoute = LNRtimeallrecsNG->GetRouteNum(initjourney);

					initPS = LNRtimeallrecsNG->GetSLSpsection(initjourney);

					// fetch the DCC code

					initLocoDCCcode = LNRtimeallrecsNG->GetDCCcodeNumber(initjourney);

					initLS = LNRteSchedMast->GetCSvalue(initRoute, 1, 1);

					std::cout<<"\n63 Journey:"<<initjourney<<" Route:"<<initRoute<<" PS:"<<initPS<<" DCC:"<<initLocoDCCcode<<" Init LS:"<<initLS;

					// initialize PS section to occupied and move DCCC code to LS section

					PSectionset(initPS,1);

					LNLSLD->RefreshLastupdatedetails(1);

					// move Loco code to LS section

					LNLSLD->UpdateTcode(initLS,initLocoDCCcode);

					//

					LNLSLD->LogicalSectionTcodeRefresh();

					LNLSLD->LogicalDetectorupdateDetectorstatus();

					LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

					//
				}
			}
		}
	}

	std::cout<<"\n44 Scheduler initialization complete";

	return 0;
}


void LNSchedulerAlljourneys::InitSchedMaster()
{
	int scm, scms;

	for (scm = 0; scm < MAXNUMBEROFSCHEDULESRECS;scm++)
	{
		SchedMaster[scm].Schedulenumber 			= 0;

		SchedMaster[scm].Schedulerunningstatus		= 0;

		SchedMaster[scm].NumberofstepsinSchedule 	= 0;

		SchedMaster[scm].StepcurrentlyExecuting		= 0;

		SchedMaster[scm].Executornumber				= 0;

		SchedMaster[scm].ScheduleEnabled 			= 0;


		for (scms = 0; scms < MAXNUMBEROFSCHEDULESTEPS; scms++)
		{
			SchedMaster[scm].Schedulesteps[scms] 					= 0;
			SchedMaster[scm].Schedulestepsjourneycode[scms]			= 0;
			SchedMaster[scm].SchedulestepStartHH[scms]	 			= 0;
			SchedMaster[scm].SchedulestepStartMM[scms]	 			= 0;
			SchedMaster[scm].SchedulestepStartSS[scms]	 			= 0;
			SchedMaster[scm].SchedulestepStartScheduleticks[scms]	= 0;
			SchedMaster[scm].SchedulestepStartstatus[scms]			= 0;
			SchedMaster[scm].SchedulestepCommand[scms]				= 0;
			SchedMaster[scm].SchedulestepCommandparam1[scms]		= 0;
			SchedMaster[scm].SchedulestepCommandparam2[scms]		= 0;
			SchedMaster[scm].SchedulestepCommandparam3[scms]		= 0;
			SchedMaster[scm].SchedulestepCommandparam4[scms]		= 0;
		}
	}

	NumberofSchedulesinTable = 0;

	//SchedMaster[1].Executornumber				= 5;

	//SchedMaster[1].ScheduleEnabled 			= 6;

	//SchedMaster[1].Schedulerunningstatus	= 7;

}

int LNSchedulerAlljourneys::StoreSchedMast(int Schedule,int step, int journey,int StartHH, int StartMM, int StartSS, int Startclicks, int Startstatus,\
		int Stepcmd, int Stepcmdp1, int Stepcmdp2,int Stepcmdp3, int Stepcmdp4)
{
	int scm = Schedule;
	int scms = step;

	SchedMaster[scm].Schedulenumber 				= Schedule;
	SchedMaster[scm].Schedulesteps[scms]			= step;
	SchedMaster[scm].Schedulestepsjourneycode[scms]	= journey;
	SchedMaster[scm].SchedulestepStartHH[scms]	 	= StartHH;
	SchedMaster[scm].SchedulestepStartMM[scms]	 	= StartMM;
	SchedMaster[scm].SchedulestepStartSS[scms]	 	= StartSS;
	SchedMaster[scm].SchedulestepStartScheduleticks[scms]	= Startclicks;
	SchedMaster[scm].SchedulestepStartstatus[scms]			= Startstatus;
	SchedMaster[scm].NumberofstepsinSchedule++;
	SchedMaster[scm].SchedulestepCommand[scms]			= Stepcmd;
	SchedMaster[scm].SchedulestepCommandparam1[scms]	= Stepcmdp1;
	SchedMaster[scm].SchedulestepCommandparam2[scms]	= Stepcmdp2;
	SchedMaster[scm].SchedulestepCommandparam3[scms]	= Stepcmdp3;
	SchedMaster[scm].SchedulestepCommandparam4[scms]	= Stepcmdp4;

	return 0;
}

int LNSchedulerAlljourneys::BrowserDisplaySchedJnyRtedetail(int detailflag,char* replymessage, LNRouteSchedMaster* LNRteSchedmast, LNLocoRoutemovementMasterNG* LNRtimeallrecsNG)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	int isc,ic,icc,scc,mcc;

	int schedcodetrack;

	int startLS;

	int endLS;

	int Jnynumber;

	int Rtenum;

	int DCCLococode;

	int Trainnumber;

	int resultsflag;

	int* resultsflagptr = &resultsflag;

	// Fields output
	/*
	Schedule number
	Schedule step number
	Schedule step Journey number
	Schedule step Journey Route number
	Schedule step Journey Route startLS
	Schedule step Journey Route finishLS
	Schedule step Journey Loco number
	Schedule step Journey Train number
	*/

	mcc = 0;

	for (scc = 1;scc < MAXNUMBEROFSCHEDULESRECS; scc++)
	{
		for (icc = 1;icc < MAXNUMBEROFSCHEDULESTEPS;icc++)
		{
			bzero(outputrec,OUTPUTMSGBUFFERSIZE);

			if ((SchedMaster[scc].Schedulenumber > 0) && (SchedMaster[scc].Schedulesteps[icc] > 0))
			{
				// Fetch journey details

				Jnynumber = SchedMaster[scc].Schedulestepsjourneycode[icc];

				int LRMmatchedflagNG;

				int Jnyrecordnumber = LNRtimeallrecsNG->GetRuntimerecord(LRMmatchedflagNG, Jnynumber);

				//std::cout<<"\n374 Schedule:"<<SchedMaster[scc].Schedulenumber<<" Jny:"<<Jnynumber<<" Jnyrecord:"<<Jnyrecordnumber;

				Rtenum 		= 0;
				DCCLococode = 0;
				Trainnumber = 0;
				startLS		= 0;
				endLS		= 0;

				if (Jnyrecordnumber > 0)
				{
					Rtenum =  LNRtimeallrecsNG->GetRouteNum(Jnyrecordnumber);
					DCCLococode = LNRtimeallrecsNG->GetDCCcodeNumber(Jnyrecordnumber);
					Trainnumber = LNRtimeallrecsNG->GetTrainNumber(Jnyrecordnumber);
					startLS = LNRteSchedmast->GetRoutesummaryStartLS(Rtenum);
					endLS = LNRteSchedmast->GetRoutesummaryFinishLS(Rtenum);
				//	std::cout<<"\n383 ScheduleJny:"<<Jnynumber<<" Rte:"<<Rtenum<<" DCClococode:"<<DCCLococode<<" TrainNumber:"<<Trainnumber<<" StartLS:"<<startLS<<" EndLS:"<<endLS;
				}
					sprintf(outputrec,"#%02d+%02d+%01d+%02d+%02d+%02d+%03d+%04d+%03d+%03d+%03d+%02d+%02d+%02d+%04d+%01d+%02d+%02d+%02d+%02d+%02d+%02d+%02d+",\
						NumberofSchedulesinTable,\
						SchedMaster[scc].Schedulenumber,\
						SchedMaster[scc].ScheduleEnabled,\
						SchedMaster[scc].Executornumber,\
						SchedMaster[scc].Schedulesteps[icc],\
						SchedMaster[scc].Schedulestepsjourneycode[icc],	\
						Rtenum,DCCLococode,Trainnumber,startLS,endLS,
						SchedMaster[scc].SchedulestepStartHH[icc],\
						SchedMaster[scc].SchedulestepStartMM[icc],\
						SchedMaster[scc].SchedulestepStartSS[icc],\
						SchedMaster[scc].SchedulestepStartScheduleticks[icc],\
						SchedMaster[scc].SchedulestepStartstatus[icc],\
						SchedMaster[scc].NumberofstepsinSchedule,\
						SchedMaster[scc].StepcurrentlyExecuting,\
						SchedMaster[scc].SchedulestepCommand[icc],\
						SchedMaster[scc].SchedulestepCommandparam1[icc],\
						SchedMaster[scc].SchedulestepCommandparam2[icc],\
						SchedMaster[scc].SchedulestepCommandparam3[icc],\
						SchedMaster[scc].SchedulestepCommandparam4[icc]);

					  isc =  std::strlen(outputrec);

					  for (ic = 0;ic <isc;ic++)
						  {
						  *(replymessage + mcc) = outputrec[ic];
						  mcc++;
						  }

					  schedcodetrack = SchedMaster[scc].Schedulenumber;
				}
		}
	}

	return mcc;
}

int LNSchedulerAlljourneys::BrowserDisplaySchedMast(int detailflag, char* replymessage)
{

	// if detailflag > 0, display the detail for the specified schedule number

	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	int isc,ic,icc,scc,mcc;

	int schedcodetrack;

	mcc = 0;

	schedcodetrack = 0;

	if (detailflag > 0)  // show details for selected schedule
	{
		for (scc = 1;scc < MAXNUMBEROFSCHEDULESRECS; scc++)
		{
			for (icc = 1;icc < MAXNUMBEROFSCHEDULESTEPS;icc++)
			{
			bzero(outputrec,OUTPUTMSGBUFFERSIZE);

			 if ((SchedMaster[scc].Schedulenumber > 0) && (SchedMaster[scc].Schedulesteps[icc] > 0) && (SchedMaster[scc].Schedulenumber == detailflag))
			 {

			  sprintf(outputrec,"#%02d+%02d+%01d+%02d+%02d+%02d+%02d+%02d+%02d+%04d+%01d+%02d+%02d+%02d+%02d+%02d+%02d+%02d+",NumberofSchedulesinTable,\
					SchedMaster[scc].Schedulenumber,SchedMaster[scc].ScheduleEnabled,SchedMaster[scc].Executornumber,SchedMaster[scc].Schedulesteps[icc],SchedMaster[scc].Schedulestepsjourneycode[icc],	\
					SchedMaster[scc].SchedulestepStartHH[icc],SchedMaster[scc].SchedulestepStartMM[icc],SchedMaster[scc].SchedulestepStartSS[icc],\
					SchedMaster[scc].SchedulestepStartScheduleticks[icc],SchedMaster[scc].SchedulestepStartstatus[icc],SchedMaster[scc].NumberofstepsinSchedule,\
					SchedMaster[scc].StepcurrentlyExecuting,SchedMaster[scc].SchedulestepCommand[icc],SchedMaster[scc].SchedulestepCommandparam1[icc], SchedMaster[scc].SchedulestepCommandparam2[icc],\
					SchedMaster[scc].SchedulestepCommandparam3[icc],SchedMaster[scc].SchedulestepCommandparam4[icc]);

				  isc =  std::strlen(outputrec);

				  if ((SchedMaster[scc].Schedulenumber != schedcodetrack) && (schedcodetrack > 0))
				   {
					// *(replymessage + mcc) = '\n';
					// mcc++;
				   }

				  for (ic = 0;ic <isc;ic++)
					  {
					  *(replymessage + mcc) = outputrec[ic];
					  mcc++;
					  }

				  schedcodetrack = SchedMaster[scc].Schedulenumber;
				 }
			}
		}
	}
	else		// show Summary summary only - select 1st record from each schedule
	{
		for (scc = 1;scc < MAXNUMBEROFSCHEDULESRECS; scc++)
		{
			icc = 1;

			bzero(outputrec,OUTPUTMSGBUFFERSIZE);

			if ((SchedMaster[scc].Schedulenumber > 0) && (SchedMaster[scc].Schedulesteps[icc] > 0))
	    	 {

				sprintf(outputrec,"#%02d+%02d+%01d+%02d+%02d+%02d+%02d+%02d+%02d+%04d+%01d+%02d+%02d+%02d+%02d+%02d+%02d+%02d+",NumberofSchedulesinTable,\
							SchedMaster[scc].Schedulenumber,SchedMaster[scc].ScheduleEnabled,SchedMaster[scc].Executornumber,SchedMaster[scc].Schedulesteps[icc],SchedMaster[scc].Schedulestepsjourneycode[icc],	\
							SchedMaster[scc].SchedulestepStartHH[icc],SchedMaster[scc].SchedulestepStartMM[icc],SchedMaster[scc].SchedulestepStartSS[icc],\
							SchedMaster[scc].SchedulestepStartScheduleticks[icc],SchedMaster[scc].SchedulestepStartstatus[icc],SchedMaster[scc].NumberofstepsinSchedule,\
							SchedMaster[scc].StepcurrentlyExecuting,SchedMaster[scc].SchedulestepCommand[icc],SchedMaster[scc].SchedulestepCommandparam1[icc], SchedMaster[scc].SchedulestepCommandparam2[icc],\
							SchedMaster[scc].SchedulestepCommandparam3[icc],SchedMaster[scc].SchedulestepCommandparam4[icc]);

						  isc =  std::strlen(outputrec);

						  if ((SchedMaster[scc].Schedulenumber != schedcodetrack) && (schedcodetrack > 0))
						   {
							// *(replymessage + mcc) = '\n';
							// mcc++;
						   }

						  for (ic = 0;ic <isc;ic++)
							  {
							  *(replymessage + mcc) = outputrec[ic];
							  mcc++;
							  }

						  schedcodetrack = SchedMaster[scc].Schedulenumber;
						 }
			}


	}


	return mcc;
}

int LNSchedulerAlljourneys::DisplaySchedMast(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	int isc,ic,icc,scc,mcc;

	int schedcodetrack;

	mcc = 0;

	if (NumberofSchedulesinTable == 0)
	{
		sprintf(outputrec,"\n\nNo Schedules to display from Schedule Master Table");

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

		return 0;
	}
	else
	{
		std::cout<<"\nNumber of Schedules to display: "<<NumberofSchedulesinTable;

		schedcodetrack = 0;

		for (scc = 1;scc < MAXNUMBEROFSCHEDULESRECS; scc++)
		{
			for (icc = 1;icc < MAXNUMBEROFSCHEDULESTEPS;icc++)
			{
			bzero(outputrec,OUTPUTMSGBUFFERSIZE);

			 if ((SchedMaster[scc].Schedulenumber > 0) && (SchedMaster[scc].Schedulesteps[icc] > 0))
			 {

			  sprintf(outputrec,"\nScheduleNumber:%02d Enabled:%01d Step:%02d Journey: %02d StartHH:%02d StartMM:%02d StartSS:%02d StartTicks:%04d Status: %01d Total steps:%02d Step currently executing:%02d StepCmd:%02d CmdP1:%02d CmdP2:%02d CmdP3:%02d CmdP4:%02d",\
					SchedMaster[scc].Schedulenumber,SchedMaster[scc].ScheduleEnabled,SchedMaster[scc].Schedulesteps[icc],SchedMaster[scc].Schedulestepsjourneycode[icc],	\
					SchedMaster[scc].SchedulestepStartHH[icc],SchedMaster[scc].SchedulestepStartMM[icc],SchedMaster[scc].SchedulestepStartSS[icc],\
					SchedMaster[scc].SchedulestepStartScheduleticks[icc],SchedMaster[scc].SchedulestepStartstatus[icc],SchedMaster[scc].NumberofstepsinSchedule,\
					SchedMaster[scc].StepcurrentlyExecuting,SchedMaster[scc].SchedulestepCommand[icc],SchedMaster[scc].SchedulestepCommandparam1[icc], SchedMaster[scc].SchedulestepCommandparam2[icc],\
					SchedMaster[scc].SchedulestepCommandparam3[icc],SchedMaster[scc].SchedulestepCommandparam4[icc]);

			  isc =  std::strlen(outputrec);

			  if ((SchedMaster[scc].Schedulenumber != schedcodetrack) && (schedcodetrack > 0))
			   {
				 *(replymessage + mcc) = '\n';
				 mcc++;
			   }

			  for (ic = 0;ic <isc;ic++)
				  {
				  *(replymessage + mcc) = outputrec[ic];
				  mcc++;
				  }

			  schedcodetrack = SchedMaster[scc].Schedulenumber;
			 }
			}
		}
	}
  return mcc;
}

int LNSchedulerAlljourneys::Getnumberofstoredschedules()
{
	return SchedMasters.size();
}

void LNSchedulerAlljourneys::ResetSchedulerstatus(int schednumber)
{
	int scm, scms;

	if (schednumber > 0)
	{
		// reset status for all steps on a specific schedule
		for (scms = 0; scms < MAXNUMBEROFSCHEDULESTEPS; scms++)
		{
			SchedMaster[schednumber].SchedulestepStartstatus[scms]			= 0;
		}
	}
	else
	{
		// reset status for all steps on all schedules

		for (scm = 0; scm < MAXNUMBEROFSCHEDULESRECS;scm++)
			{
				for (scms = 0; scms < MAXNUMBEROFSCHEDULESTEPS; scms++)
				{
					SchedMaster[scm].SchedulestepStartstatus[scms]			= 0;
				}
			}
	}

}

void LNSchedulerAlljourneys::Updatecurrentsteprunning(int schedule, int step)
{
	SchedMaster[schedule].StepcurrentlyExecuting = step;
}

int LNSchedulerAlljourneys::Getnextschedulesteptorun(int schedule)
{
	// returns the number of the next step to run if the schedule is active and any prior steps have completed

	int nextstep = 0;

	int stepsinprogress = 0;

	int blanksteps = 0;

	int currentlyexecutingstep = 0;

	int numberofcompletedsteps = 0;

	int Numberofstepswithcommands = 0;

	int Numberofstepsfinishedexecution = 0;

	if (SchedMaster[schedule].Schedulerunningstatus == 0) // || (SchedMaster[schedule].ScheduleEnabled)
	{
		nextstep = 0;	// return zero as the schedule is not active or not enabled
	}
	else
	{
		for (int numberofschedsteps = 1;numberofschedsteps < MAXNUMBEROFSCHEDULESTEPS;numberofschedsteps++)
		{
			if (SchedMaster[schedule].SchedulestepCommand[numberofschedsteps] > 0)
			{
				Numberofstepswithcommands++;
			}

			if (SchedMaster[schedule].SchedulestepStartstatus[numberofschedsteps] == 9)
			{
				Numberofstepsfinishedexecution++;
			}
		}

		if ((Numberofstepswithcommands == Numberofstepsfinishedexecution) && (Numberofstepswithcommands > 0))
		{
			nextstep = 999; 	// all scheduler steps are completed
		}
		else
		{
			// check to see if currently executing step is completed

			currentlyexecutingstep = SchedMaster[schedule].StepcurrentlyExecuting;

			if (SchedMaster[schedule].SchedulestepStartstatus[currentlyexecutingstep] != 9)
			{
				nextstep = 0;
			}

			if (SchedMaster[schedule].SchedulestepStartstatus[currentlyexecutingstep] == 9)  // previous step has completed so get the next one
			{
				if (currentlyexecutingstep == (SchedMaster[schedule].NumberofstepsinSchedule - 1))
				{
					nextstep = SchedMaster[schedule].NumberofstepsinSchedule;
					std::cout<<"\n\n===> 253 Get next step - next step is LAST STEP step\n\n";
				}
				else
				{
					nextstep  = currentlyexecutingstep + 1;
				}
			}
		}
	}

	return nextstep;
}

int LNSchedulerAlljourneys::Getnextschedulesteptorunold(int schedule)
{
	int nextstep = 0;

	int stepsinprogress = 0;

	int blanksteps = 0;

	int numberofcompletedsteps = 0;

	// search down list of steps until next one with a valid journey and status of zero or else the end  of the list.

	for (int numberofschedsteps = 1;numberofschedsteps < MAXNUMBEROFSCHEDULESTEPS;numberofschedsteps++)
	{
		if ((SchedMaster[schedule].SchedulestepStartstatus[numberofschedsteps] == 9) && (SchedMaster[schedule].Schedulestepsjourneycode[numberofschedsteps] > 0))
		{
			numberofcompletedsteps++;
		}
		//else
		//{
			if (SchedMaster[schedule].Schedulestepsjourneycode[numberofschedsteps] == 0)
			{
				blanksteps++;
			}
		//}

		if ((SchedMaster[schedule].SchedulestepStartstatus[numberofschedsteps] == 0)  && (SchedMaster[schedule].SchedulestepStartstatus[numberofschedsteps - 1] == 9) && (SchedMaster[schedule].Schedulestepsjourneycode[numberofschedsteps] > 0))
		{
			nextstep = numberofschedsteps;
			break;
		}
	}

	if ((numberofcompletedsteps + blanksteps) == (MAXNUMBEROFSCHEDULESTEPS - 1))
	{
		return 999;
	}
	else
	{
		return nextstep;
	}
}

int LNSchedulerAlljourneys::UpdateschedulestepExecutionstatus(int schedule, LNLocoRoutemovementMasterNG* LNRtimeallrecsNG,LNweblocorouteprocess* LNLRprocess)
{
	int Loconum;

	int LocoDCC;

	int Routenumber;

	int RouteExecutionstatus = 0;

	int Executingstep;

	int RC = 0;

	if (SchedMaster[schedule].Schedulerunningstatus != 1)
	{
		// not executing so set RC = 8;

		//std::cout<<"\n210 Schedule "<<schedule<<" not running";

		RC = 8;
		return RC;
	}
	else
	{
	// search schedule record to see which step is running

		for (int schedsteps = 1;schedsteps <= MAXNUMBEROFSCHEDULESTEPS; schedsteps++)
		{
			//std::cout<<"\n218 Checking status of schedule "<<schedule<<" step "<<schedsteps<<" start status "<<SchedMaster[schedule].SchedulestepStartstatus[schedsteps];

			if (SchedMaster[schedule].SchedulestepStartstatus[schedsteps] > 0)//  && (SchedMaster[schedule].SchedulestepStartstatus[schedsteps] != 9))
			{
				// found step with status = 1 which means it is executing so extract the journey code

				Routenumber = SchedMaster[schedule].Schedulestepsjourneycode[schedsteps];

				Loconum = LNRtimeallrecsNG->GetLoconumberforRun(Routenumber);

				LocoDCC = LNRtimeallrecsNG->GetDCCcodeNumber(Routenumber);

				RouteExecutionstatus = LNLRprocess->GetJourneystatusvalue(Loconum);

				//std::cout<<"\n206 Executing Schedule "<<schedule<<" Step "<<schedsteps<<" on Routenum  "<<Routenumber<<" with Loco "<<Loconum<<" DCC "<<LocoDCC<<" Routestatus "<<RouteExecutionstatus;

				// check journey status, if set to 9 e.g journey is completed so mark schedule step as completed

				if ((RouteExecutionstatus == 9) && (SchedMaster[schedule].SchedulestepStartstatus[schedsteps] != 9))
				{
					// schedule step is completed to move step status to 9

					SchedMaster[schedule].SchedulestepStartstatus[schedsteps] = 9;

					std::cout<<"\n222 Schedule "<<schedule<<" Step "<<schedsteps<<" on Routenum  "<<Routenumber<<" completed";
				}

			}

		}
	}

	return RC;

}

int LNSchedulerAlljourneys::GetStepcommand(int schedule, int step)
{
	return SchedMaster[schedule].SchedulestepCommand[step];
}

int LNSchedulerAlljourneys::GetStepcommandparam1(int schedule, int step)
{
	return SchedMaster[schedule].SchedulestepCommandparam1[step];
}

int LNSchedulerAlljourneys::GetStepcommandparam2(int schedule, int step)
{
	return SchedMaster[schedule].SchedulestepCommandparam2[step];
}

int LNSchedulerAlljourneys::GetStepcommandparam3(int schedule, int step)
{
	return SchedMaster[schedule].SchedulestepCommandparam3[step];
}

int LNSchedulerAlljourneys::GetStepcommandparam4(int schedule, int step)
{
	return SchedMaster[schedule].SchedulestepCommandparam4[step];
}

int LNSchedulerAlljourneys::Updateschedulestartstatus(int schedule, int step, int statusvalue)
{
	SchedMaster[schedule].SchedulestepStartstatus[step]	= statusvalue;

	SchedMaster[schedule].Schedulerunningstatus = 1;

	return 0;
}

void LNSchedulerAlljourneys::SetstatusonMasterTableSchedulestep(int schedule, int schedstep,int status)
{
	// update the status flag for Scheduler step

	SchedMaster[schedule].SchedulestepStartstatus[schedstep] = status;
}

int LNSchedulerAlljourneys::Getschedulestepjourneycode(int schedule, int step)
{
	return SchedMaster[schedule].Schedulestepsjourneycode[step];

}

int LNSchedulerAlljourneys::Getschedexecutornumber(int schedule)
{
	return SchedMaster[schedule].Executornumber;
}

void LNSchedulerAlljourneys::Storeschedexecutornumber(int schedule, int executor)
{
	SchedMaster[schedule].Executornumber = executor;

}

int LNSchedulerAlljourneys::FetchSpecificScheduleStep(int schedulenum, int stepnum, int &Trainjourney, int &TotalstepsinSchedule, int &Schedulestepstatus)
{
	//std::cout<<"\n206...Fetch check on Schedule:"<<schedulenum<<" step:"<<stepnum;

	Trainjourney = SchedMaster[schedulenum].Schedulestepsjourneycode[stepnum];
	TotalstepsinSchedule = SchedMaster[schedulenum].NumberofstepsinSchedule;
	Schedulestepstatus = SchedMaster[schedulenum].SchedulestepStartstatus[stepnum];

	//std::cout<<"\n212...Fetch check on Schedule:"<<schedulenum<<" step:"<<stepnum<<"returns Journey:"<< Trainjourney<<" Status of step:"<<Schedulestepstatus;

	return Trainjourney;
}

int LNSchedulerAlljourneys::Checkschedulestartreadiness(int schedule, int* schedsteptr,int* startjourneyptr, int schedclock)
{
	int scheduletostart = 0;

	int startscheduleclicks = 0;

	int nextsteptorun = 0;

	int schedulerclockcyclecount = 0;

	if ((SchedMaster[schedule].Schedulenumber > 0) && (SchedMaster[schedule].ScheduleEnabled > 0))
	{
		if (SchedMaster[schedule].Schedulerunningstatus == 0)    // if schedule not running check Step 1 to see if the schedule should start
		{
			startscheduleclicks = SchedMaster[schedule].SchedulestepStartScheduleticks[1];

			schedulerclockcyclecount = schedclock;

			//std::cout<<"\n210 Start target:"<<startscheduleclicks<<" clock counter "<<schedulerclockcyclecount;

			if (startscheduleclicks < schedulerclockcyclecount) // schedule is ready to be started so start at step 1
			{
				scheduletostart = schedule;

				*(schedsteptr) = 1;

				*(startjourneyptr) = SchedMaster[schedule].Schedulestepsjourneycode[1];

				//std::cout<<"\n216 Schedule "<<schedule<<" step "<< *(schedsteptr)<<" ready to start\n";
			}
			else
			{
				scheduletostart = 0;

				*(schedsteptr) = 0;
			}
		}
		else
		{
			// find next step to start

			nextsteptorun = Getnextschedulesteptorun(schedule);

			if (nextsteptorun == 999)
			{
		//		std::cout<<"\n\n348 No more steps will be run as all steps have run for schedule "<<schedule<<" - schedule now disabled ";

				// stop schedule running, reset status

				//SchedMaster[schedule].Schedulerunningstatus = 0;
			}
			else
			{
				if ((nextsteptorun > 0) && (nextsteptorun != 998))
				{
					std::cout<<"\n514 Next step to run will be "<<nextsteptorun;
				}
				else
				{
					if (nextsteptorun == 998)
					{
					//	std::cout<<"\n520 Next step to run will be "<<nextsteptorun;
					}
					else
					{

					}
				}
			}
		}
	}
	return scheduletostart;
}

int LNSchedulerAlljourneys::Resetscheduletostart(int schedule)
{
	// reset schedule to start condition
	/*
	int Schedulenumber;
    	int Schedulerunningstatus;
    	int NumberofstepsinSchedule;
    	int StepcurrentlyExecuting;
    	int Executornumber;
    	int Schedulesteps[MAXNUMBEROFSCHEDULESTEPS];
    	int Schedulestepsjourneycode[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepStartHH[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepStartMM[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepStartSS[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepStartScheduleticks[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepStartstatus[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepCommand[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepCommandparam1[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepCommandparam2[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepCommandparam3[MAXNUMBEROFSCHEDULESTEPS];
    	int SchedulestepCommandparam4[MAXNUMBEROFSCHEDULESTEPS];

	*/

	if (schedule > 0)
	{

	SchedMaster[schedule].Schedulerunningstatus 	= 0;
	SchedMaster[schedule].Executornumber			= 0;
	SchedMaster[schedule].StepcurrentlyExecuting	= 0;

	for (int ssteps = 0; ssteps < MAXNUMBEROFSCHEDULESTEPS;ssteps++)
	{
		SchedMaster[schedule].SchedulestepStartstatus[ssteps] = 0;
	}

	std::cout<<"\n\n\n===> 770 Schedule "<<schedule<<" reset to rerun ";
	}
	else
	{
		// Reset all schedules

		for (int Allschcc = 1; Allschcc < MAXNUMBEROFSCHEDULESRECS; Allschcc++)
		{
			SchedMaster[Allschcc].Schedulerunningstatus 	= 0;
			SchedMaster[Allschcc].Executornumber			= 0;
			SchedMaster[Allschcc].StepcurrentlyExecuting	= 0;

			for (int ssteps = 0; ssteps < MAXNUMBEROFSCHEDULESTEPS;ssteps++)
			{
				SchedMaster[schedule].SchedulestepStartstatus[ssteps] = 0;
			}

			std::cout<<"\n\n\n===> 787 Schedule "<<schedule<<" reset to rerun ";
		}
	}

	return 0;
}

