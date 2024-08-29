/*
 * LNSchedulerExecutor.cpp
 *
 *  Created on: Sep 14, 2021
 *      Author: BrianRoot
 */

#include "LNSchedulerExecutor.h"


LNSchedulerExecutor::LNSchedulerExecutor()
{
	// TODO Auto-generated constructor stub
	SchedulerExecutionenabled = 0;   // enables scheduler on startup
}

LNSchedulerExecutor::~LNSchedulerExecutor() {
	// TODO Auto-generated destructor stub
}

void LNSchedulerExecutor::EnableSchedulerexecution(int schedulenumber)
{
	SchedulerExecutionenabled = 1;
}

void LNSchedulerExecutor::DisableSchedulerexecution(int schedulenumber)
{
	SchedulerExecutionenabled = 0;
	std::cout<<"\n29 Stopping schedule";
}

void LNSchedulerExecutor::Resettickcounter()
{
	schedclickcounter = 0;

	TotalSchedulerclicksfromstart = 0;
}

int LNSchedulerExecutor::Findnextexecutor()
{
	int nextexecutor = 0;

	int ne;

	for (ne = 1; ne < (MAXNUMBEROFSCHEDULES + 1);ne++)
	{
		if (ScheduleMonitor[ne].Schedulenumber == 0)
		{
			nextexecutor = ne;
			break;
		}
	}

	return nextexecutor;
}

void LNSchedulerExecutor::InitializeSchedulerclocktimestamp()
{
	char CTDatetime[DATETIMESTRING] = "Not set ";

	char* CTDatetimeptr = &CTDatetime[0];

	Clocktime CT;

	CT.Get_time(*CTDatetimeptr);
	/*
	 int Start_HH;
	 int Start_MM;
	 int Start_SS;
	 int Current_HH;
	 int Current_MM;
	 int Current_SS;
	 int TotalSchedulerclicksfromstart;
	 */

	StartHH[0] = *(CTDatetimeptr + 11);
	StartHH[1] = *(CTDatetimeptr + 12);
	StartMM[0] = *(CTDatetimeptr + 14);
	StartMM[1] = *(CTDatetimeptr + 15);
	StartSS[0] = *(CTDatetimeptr + 17);
	StartSS[1] = *(CTDatetimeptr + 18);

	CurrHH[0] = *(CTDatetimeptr + 11);
	CurrHH[1] = *(CTDatetimeptr + 12);
	CurrMM[0] = *(CTDatetimeptr + 14);
	CurrMM[1] = *(CTDatetimeptr + 15);
	CurrSS[0] = *(CTDatetimeptr + 17);
	CurrSS[1] = *(CTDatetimeptr + 18);


	Startepochtime = CT.Getepochtime();
	Currentepochtime = CT.Getepochtime();

	//std::cout<<"\nStart Epoch time:"<<Startepochtime;
	//std::cout<<"\nScheduler HH:"<<StartHH[0]<<StartHH[1]<<" MM:"<<StartMM[0]<<StartMM[1]<<" SS:"<<StartSS[0]<<StartSS[1]<<"\n";
}

void LNSchedulerExecutor::UpdateSchedulertimestamp()
{
	char CTDatetime[DATETIMESTRING] = "Not set ";

	char* CTDatetimeptr = &CTDatetime[0];

	Clocktime CT;

	CT.Get_time(*CTDatetimeptr);

	CurrHH[0] = *(CTDatetimeptr + 11);
	CurrHH[1] = *(CTDatetimeptr + 12);
	CurrMM[0] = *(CTDatetimeptr + 14);
	CurrMM[1] = *(CTDatetimeptr + 15);
	CurrSS[0] = *(CTDatetimeptr + 17);
	CurrSS[1] = *(CTDatetimeptr + 18);

	/*
	struct tm {
   int tm_sec;   // seconds of minutes from 0 to 61
   int tm_min;   // minutes of hour from 0 to 59
   int tm_hour;  // hours of day from 0 to 24
   int tm_mday;  // day of month from 1 to 31
   int tm_mon;   // month of year from 0 to 11
   int tm_year;  // year since 1900
   int tm_wday;  // days since sunday
   int tm_yday;  // days since January 1st
   int tm_isdst; // hours of daylight savings time
}
	*/
	time_t now = time(0);

	tm *ltm = localtime(&now);

	Current_HH = ltm->tm_hour;
	Current_MM = ltm->tm_min;
	Current_SS = ltm->tm_sec;

	Currentepochtime = CT.Getepochtime();

	//std::cout<<"\nCurrent Epoch time:"<<Currentepochtime;
	//std::cout<<"\nScheduler integertime HH:"<<Current_HH<<" MM:"<<Current_MM<<" SS:"<<Current_SS<<"\n";
}

void LNSchedulerExecutor::GetSchedulertimestamp(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	char* outputptr = &outputrec[0];

	int isc,ic,scc,mcc;

	mcc = 0;

	sprintf(outputrec,"\nScheduler Time status - Totalticks:%03d Current scheduler time %c%c:%c%c:%c%c Epochtime:%08d",\
			schedclickcounter,CurrHH[0],CurrHH[1],CurrMM[0],CurrMM[1],CurrSS[0],CurrSS[1],Currentepochtime);

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
				mcc++;
	}
		*(replymessage + mcc) = '\n';
		mcc++;
}

void LNSchedulerExecutor::InitExecutors()
{
	// Initialize all executors
	/*
		unsigned int Schedulenumber;
		unsigned int Schedulestep;
		unsigned int Trainjourneynumber;
		unsigned int Status;
		unsigned int Currentstep;
		unsigned int Totalsteps;
		unsigned int Clickscounted;

	*/

	int ic = 0;

	for (ic = 0;ic < (MAXNUMBEROFSCHEDULES + 1) ;ic++)
	{
		ScheduleMonitor[ic].Currentstepbeingexecuted 		= 0;
		ScheduleMonitor[ic].Schedulenumber 					= 0;
		ScheduleMonitor[ic].Statusofcurrentstep				= 0;
		ScheduleMonitor[ic].Totalsteps						= 0;
		ScheduleMonitor[ic].Clickscounted					= 0;
		ScheduleMonitor[ic].Trainjourneynumber			 	= 0;
		ScheduleMonitor[ic].Schedulestep					= 0;
	}

}

void LNSchedulerExecutor::BrowserSchedulerExecutionresponses(char* replymessage)
{
	char outputrec[HEAPDISPLAYBUFFERLENGTH];

	int sc,ssc,ic,isc,mcc;

	mcc = 0;

	// Transmits Scheduler execution status for each Executor
	/*
	struct ScheduleMonitorRecord
	{
		unsigned int Schedulenumber;             // 2
		unsigned int Schedulestep;               // 3
		unsigned int Trainjourneynumber;         // 4
		unsigned int Loconumber;                 // 5
		unsigned int Statusofcurrentstep;        // 6
		unsigned int Currentstepbeingexecuted;   // 7
		unsigned int Totalsteps;                 // 8
		unsigned int Clickscounted;              // 9
		unsigned int HHcheck;                    // 10
		unsigned int MMcheck;                    // 11
		unsigned int SScheck;                    // 12
	};


	*/

	char executionstate[9];

	for (ssc = 1;ssc <= (MAXNUMBEROFSCHEDULES);ssc++)
	{
		bzero(outputrec,HEAPDISPLAYBUFFERLENGTH);

		if (ScheduleMonitor[ssc].Statusofcurrentstep == 0)
			{
				sprintf(executionstate,"Stopped \0");
			}
		else
			{
				sprintf(executionstate,"Running \0");
			}

		sprintf(outputrec,";%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%02d;%s+",\
				ssc,ScheduleMonitor[ssc].Schedulenumber,ScheduleMonitor[ssc].Schedulestep,\
				ScheduleMonitor[ssc].Trainjourneynumber,ScheduleMonitor[ssc].Loconumber,\
				ScheduleMonitor[ssc].Statusofcurrentstep,ScheduleMonitor[ssc].Currentstepbeingexecuted,\
				ScheduleMonitor[ssc].Totalsteps,ScheduleMonitor[ssc].Clickscounted,\
				ScheduleMonitor[ssc].HHcheck,ScheduleMonitor[ssc].MMcheck,ScheduleMonitor[ssc].SScheck,ScheduleMonitor[ssc].Statusofcurrentstep,executionstate);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	// include data for overall scheduler operations

	bzero(outputrec,HEAPDISPLAYBUFFERLENGTH);

	if (SchedulerExecutionenabled == 0)
	{
		sprintf(executionstate,"Stopped \0");
	}
	else
	{
		sprintf(executionstate,"Running \0");
	}

	sprintf(outputrec,"#%01d;%s;%02d;%02d;%02d;%04d",SchedulerExecutionenabled,executionstate,Current_HH,Current_MM,Current_SS,TotalSchedulerclicksfromstart);

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}
}

void LNSchedulerExecutor::DisplaySchedulerExecutionstatus(char* replymessage)
{
	#define SCHbuffersize 128

	char SCHmon[SCHbuffersize];

	char* SCHmonptr = &SCHmon[0];

	int isc,ic,scc,mcc, set;

	mcc = 0;

	set = 0;

	/*
	 	unsigned int Schedulenumber;
		unsigned int Schedulestep;
		unsigned int Trainjourneynumber;
		unsigned int Loconumber;
		unsigned int Statusofcurrentstep;
		unsigned int Currentstepbeingexecuted;
		unsigned int Totalsteps;
		unsigned int Clickscounted;
		unsigned int HHcheck;
		unsigned int MMcheck;
		unsigned int SScheck;
	 */


	for (set = 1;set < (MAXNUMBEROFSCHEDULES + 1);set++)
	{
		bzero(SCHmon,SCHbuffersize);

		sprintf(SCHmon,"\nExecutor:%01d Sched:%02d Step:%02d TrainJourney:%02d Loco: %02d status:%01d Totalsteps:%02d \n",set, ScheduleMonitor[set].Schedulenumber,\
				ScheduleMonitor[set].Schedulestep,ScheduleMonitor[set].Trainjourneynumber,ScheduleMonitor[set].Loconumber, ScheduleMonitor[set].Statusofcurrentstep,ScheduleMonitor[set].Totalsteps);

			isc =  std::strlen(SCHmon);

			for (ic = 0;ic <isc;ic++)
			{
					*(replymessage + mcc) = SCHmon[ic];
							mcc++;
			}
	}
}

int LNSchedulerExecutor::Initiateschedulerun(int Schedulenumber)
{
	// enable the execution of a schedule.

	// Find the schedule master and set the enabled flag;

	return 0;
}

int LNSchedulerExecutor::CheckifSchedulealreadystarted(int Schedulenumber)
{
	int ic = 0;

	//std::cout<<"\n175 Executor - checking to see if schedule:"<<Schedulenumber<<" is already started...";

	for (ic = 0;ic < (MAXNUMBEROFSCHEDULES + 1) ;ic++)
	{
		//std::cout<<"\nExecutor:"<<ic<<" running Schedule number:"<<ScheduleMonitor[ic].Schedulenumber;

		if (ScheduleMonitor[ic].Schedulenumber 	== Schedulenumber)
		{
			//std::cout<<"\n182 - Executor Schedule found ";
			return Schedulenumber;
		}
	}

	return 0;
}


void LNSchedulerExecutor::RunScheduler(LNSchedulerExecutor* LNSchedExecutor,LNweblocorouteprocess* LNLRprocess,\
		LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNwebmonSimulator* LNSIM,	LNSimulatorAllRoutes* LNSimAllRtes, LNSimRouteMaster* LNSimRteMaster,\
		LNSchedulerAlljourneys* LNSchedAlljourneys,LNwebLsectionsLdevices* LNLSLD)
{
	schedclickcounter 	= 0;

	int schedstatus		= 0;

	int Schedulesleep	= 5;

	std::cout<<"\n365 Starting scheduler processing....";

	while(1)
		{
		  std::this_thread::sleep_for (std::chrono::seconds(2));

		  schedclickcounter++;

		  TotalSchedulerclicksfromstart++;

		  UpdateSchedulertimestamp();

		//  std::cout<<"\n377 Scheduler checking..counter:"<<schedclickcounter;

		  // check each schedule to see if it should be run

		  if (SchedulerExecutionenabled == 1)
		  {
			  Checkschedulerexecutionstatus(LNSchedAlljourneys,LNRtimeallrecsNG, LNLRprocess,LNLSLD,LNSIM,LNSimRteMaster);			// check to see if any schedule steps have completed

			  ScheduleAutostart(LNSchedAlljourneys,LNRtimeallrecsNG, LNLRprocess,LNLSLD,LNSIM,LNSimRteMaster);			// check to see if any schedules should be autostarted
		  }

		 // std::cout<<"\n209 -> Scheduler Executor sleeping for:"<<Schedulesleep<<" seconds.....\n";

		  std::this_thread::sleep_for (std::chrono::seconds(Schedulesleep));
		}
}

void LNSchedulerExecutor::ScheduleAutostart(LNSchedulerAlljourneys* LNSchedAlljourneys,\
		LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster)
{
	// checks each schedule under the control of the Scheduler to see if it should started

//	std::cout<<"\n398 Checking for Schedules to be started.....";

	LNScheduleMaster ExecuteSCH;// = new LNScheduleMaster();

	LNSchedulestep* ExecuteSCHstep = new LNSchedulestep();

	LNSchedulerAlljourneys* LNallmastschedules = new LNSchedulerAlljourneys(); // create local schedule master

	LNallmastschedules = LNSchedAlljourneys;									// point local schedule master to called schedule master

	char outputrec[OUTPUTMSGBUFFERSIZE];

	char SCHrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	char* outputrecptr = &outputrec[0];

	char* SCHptr = &SCHrec[0];

	int schedulechecked = 0;

	int startstep = 0;

	int nextsteptorun = 0;

	int nextjourney = 0;

	int* startsteptr = &startstep;

	int startjourney = 0;

	int Stepcmd = 0;

	int Stepcmdparam1 = 0;

	int* startjourneyptr = &startjourney;

	for (int sscc = 1;sscc < MAXNUMBEROFSCHEDULESRECS;sscc++)
	{
		schedulechecked = LNallmastschedules->Checkschedulestartreadiness(sscc, startsteptr,startjourneyptr,schedclickcounter);

		if (schedulechecked > 0)  // found a schedule to start
		{
			Stepcmd = LNallmastschedules->GetStepcommand(sscc, startstep);  // fetch the schedule step command

			if (Performschedulestepcommand(sscc,startstep,Stepcmd,LNSchedAlljourneys) == 1)	// Step command = 1 so run the journey in the step
			{
				std::cout<<"\n447 Schedule "<<sscc<<" ready to start step "<< startstep<<" with journey "<<startjourney<<" Step command:"<<Stepcmd;

				if (Startjourneyexecution(startjourney,sscc,startstep,LNRtimeallrecsNG,LNLRprocess,LNLSLD,LNSIM,LNSimRteMaster,LNallmastschedules) == 0)
				{
				// journey has started OK so update status on the master schedule record for the schedule and the step

					LNallmastschedules->Updateschedulestartstatus(sscc, startstep, 1);

					LNallmastschedules->Updatecurrentsteprunning(sscc, startstep);
				}
				else
				{
				std::cout<<"\n458 Schedule "<<sscc<<" step:"<< startstep<<" with journey "<<startjourney<<" FAILED TO START - cmd:"<<Stepcmd;
				}
			}
			else
			{
				std::cout<<"\n462 No journey run but Schedule "<<sscc<<" Step "<< startstep<<" Step command "<<Stepcmd<<" actioned";
			}
		}
		else
		{
			//std::cout<<"\n469 Check if another step to process ";

			nextsteptorun = LNallmastschedules->Getnextschedulesteptorun(sscc);

			if ( nextsteptorun == 999)
			{
				LNallmastschedules->Updateschedulestartstatus(sscc, 0, 0);
				//std::cout<<"\n456 All scheduler steps executed - mark the Schedule as finished,reset all steps status flags to zero";
			}
			else
			{

				if (nextsteptorun > 0)
				{
					Stepcmd = LNallmastschedules->GetStepcommand(sscc, nextsteptorun);

					std::cout<<"\n480 Found next step for Schedule "<<sscc<<" is schedule step "<< nextsteptorun<<" initiating start process for step - cmd is "<<Stepcmd;

					if (Performschedulestepcommand(sscc,nextsteptorun,Stepcmd,LNSchedAlljourneys) == 1)	// Step command = 1 so run the journey in the step
					{
						std::cout<<"\n486 Found next step for Schedule "<<sscc<<" is schedule step "<< nextsteptorun<<" initiating start process for step - cmd is "<<Stepcmd;

						nextjourney = LNallmastschedules->Getschedulestepjourneycode(sscc,nextsteptorun);

						std::cout<<"\n464 Journey will be "<<nextjourney;

						if (Startjourneyexecution(nextjourney,sscc,nextsteptorun,LNRtimeallrecsNG,LNLRprocess,LNLSLD,LNSIM,LNSimRteMaster,LNallmastschedules) == 0)
						{
						// journey has started OK so update status on the master schedule record for the schedule and the step

								LNallmastschedules->Updateschedulestartstatus(sscc, nextsteptorun, 1);

								LNallmastschedules->Updatecurrentsteprunning(sscc, nextsteptorun);
						}
						else
						{
						std::cout<<"\n478 Error on Journey start";
						}
					}
					else
					{
						std::cout<<"\n\n===> 503 No journey run but Schedule"<<sscc<<" step:"<<nextsteptorun<<" Step command "<<Stepcmd<<" actioned";
					}
				}
				else
				{
				//	std::cout<<"\n466 No next step ready to be run for Schedule "<<sscc<<" is schedule step "<< nextsteptorun;
				}
			}


		}
	}
}

int LNSchedulerExecutor::Checkschedulerexecutionstatus(LNSchedulerAlljourneys* LNSchedAlljourneys,\
		LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster)
{
	// check and status of any steps running to see if they have completed

	LNSchedulerAlljourneys* LNallmastschedules = new LNSchedulerAlljourneys(); // create local schedule master

	LNallmastschedules = LNSchedAlljourneys;									// point local schedule master to called schedule master

	//std::cout<<"\n477 Checking scheduler execution status";

	for (int sscc = 1;sscc < MAXNUMBEROFSCHEDULESRECS;sscc++)
	{
		LNallmastschedules->UpdateschedulestepExecutionstatus(sscc, LNRtimeallrecsNG,LNLRprocess);
	}

}

int LNSchedulerExecutor::Performschedulestepcommand(int sched,int step, int stepcmd, LNSchedulerAlljourneys* LNSchedAlljourneys)
{
	std::cout<<"\n\n\n===>540 Performing schedule step command Schedule:"<<sched<<" Step:"<<step<<" Stepcmd:"<<stepcmd;

	if (stepcmd == 1)
	{
		return 1;
	}

	if (stepcmd == 3)
	{
		LNSchedAlljourneys->Resetscheduletostart(sched);
		return 0;
	}

	return 0;
}

int LNSchedulerExecutor::Startjourneyexecution(int journey,int sched, int schedstep, LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,\
		LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster, LNSchedulerAlljourneys* LNallmastschedules)
{
	int LRMmatchedflagNG;

	SchReadystartschedule = sched;

	SchReadystartschedulestep = schedstep;

	if (CheckifSchedulealreadystarted(SchReadystartschedule) ==  0)  // check to see if schedule is already on Executor, if not then start it
	{
		Schedstartexecutor = Findnextexecutor();
	}
	else
	{
		Schedstartexecutor = LNallmastschedules->Getschedexecutornumber(sched);
	}

	//Schedstartexecutor = 1;

	LNallmastschedules->Storeschedexecutornumber(sched,Schedstartexecutor);

		std::cout<<"\n\n450 executor "<<Schedstartexecutor<<" selected to run schedule with journey "<<journey;

		LNTRexecuteNGindex = LNRtimeallrecsNG->GetRuntimerecord(LRMmatchedflagNG, journey);

		//std::cout<<"\n458 LNTRexecuteNGindex "<<LNTRexecuteNGindex<<" matchedflag "<<LRMmatchedflagNG;

		//return 0;

		if (LRMmatchedflagNG == 0)
		{
			std::cout<<"\nSchedule Trainjourney:"<<SchReadystartschedulestepTrainjourney<<" not found - schedule not started";
			return 8;
		}

		std::cout<<"\n\n\n433 Scheduler - starting schedule:"<<SchReadystartschedule<<" at step:"\
						<<SchReadystartschedulestep<<" for Trainjourney:"<<journey<<"\n";

		// check to see if it is a simulated journey

		if (LNRtimeallrecsNG->GetSimulationflag(LNTRexecuteNGindex) > 0)
		{
			// fetch the start Psection

			SchedstartexecutorSimPsection =  LNRtimeallrecsNG->GetSLSpsection(LNTRexecuteNGindex);

			PSectionset(SchedstartexecutorSimPsection,1);

			LNLSLD->RefreshLastupdatedetails(1);

			LNLSLD->LogicalSectionTcodeRefresh();

			LNLSLD->LogicalDetectorupdateDetectorstatus();

			LNLRprocess->LNlocosectionrefresh();
		}

		// store the journey details in the locoinformation table

		LNLRprocess->SaveLocojourneydata(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex), LNRtimeallrecsNG->GetRouteNum(LNTRexecuteNGindex), \
																 0,0,0,0,0,0,0,0,Enable,0,0, LNRtimeallrecsNG->GetJourneyNumber(LNTRexecuteNGindex),0,0,\
																 LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex), \
																 LNRtimeallrecsNG->GetSlotNumber(LNTRexecuteNGindex),\
																 LNRtimeallrecsNG->GetSimulationflag(LNTRexecuteNGindex),\
																 LNRtimeallrecsNG->GetTrainNumber(LNTRexecuteNGindex),\
																 LNRtimeallrecsNG->GetReversefacingflag(LNTRexecuteNGindex),\
																 LNRtimeallrecsNG->GetSectionlogicflag(LNTRexecuteNGindex));

		std::cout<<"\n574 Scheduler starting Loco num:"<<LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex)<<" Route:"<<LNRtimeallrecsNG->GetRouteNum(LNTRexecuteNGindex)<<" DCCcode:"<<LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex)\
								<<" TrainNo:"<<LNRtimeallrecsNG->GetTrainNumber(LNTRexecuteNGindex);

							LNLRprocess->Setschedulerrunstatus(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),1);

							LNLRprocess->LNsectionCrossingprocessing(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),1);

						    // Update the Schedmain table to show Schedule is running

							LNallmastschedules->SetstatusonMasterTableSchedulestep(SchReadystartschedule,SchReadystartschedulestep,1);

							Locosimcheck = LNLRprocess->GetSimulatornumber(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex));

							int Simexecutornumber =  SchReadystartschedule;    //LNSIM->FindavailableSimulator();

				//			std::cout<<"\nStarting simulated run for loco:"<<LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex)<<" on SIM Executor:"<<Simexecutornumber;

							if (Locosimcheck > 0) // check to see if simulator should be started in run mode
							{
								LNSIM->InitMultiSimrun(Simexecutornumber,LNRtimeallrecsNG->GetRouteNum(LNTRexecuteNGindex),1,LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),LNRtimeallrecsNG->GetTrainNumber(LNTRexecuteNGindex), LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex), LNLSLD,LNLRprocess,LNSimRteMaster);
							}

							// Update the details on the scheduler execution table

							time_t now = time(0);

							tm *ltm = localtime(&now);

							// set schedule start timestamp

							ScheduleMonitor[Schedstartexecutor].HHcheck = ltm->tm_hour;
							ScheduleMonitor[Schedstartexecutor].MMcheck	= ltm->tm_min;
							ScheduleMonitor[Schedstartexecutor].SScheck	= ltm->tm_sec;

							std::cout<<"\n530 Scheduler - Setting Scheduler execution Monitor status and enable flags ";

							ScheduleMonitor[Schedstartexecutor].Schedulenumber 						= SchReadystartschedule;
							ScheduleMonitor[Schedstartexecutor].Schedulestep						= SchReadystartschedulestep;
							ScheduleMonitor[Schedstartexecutor].Statusofcurrentstep		   			= 1;  // 1 = step initiated
							ScheduleMonitor[Schedstartexecutor].Currentstepbeingexecuted    		= SchReadystartschedulestep;
							ScheduleMonitor[Schedstartexecutor].Trainjourneynumber 					= SchReadystartschedulestepTrainjourney;
							ScheduleMonitor[Schedstartexecutor].Totalsteps							= Schtotalstepsinschedule;
							ScheduleMonitor[Schedstartexecutor].Loconumber							= LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex);
							return 0;
	//}
	//else
	//{
	//	std::cout<<"\n618 schedule is already running on Executor ";
	//}

}

