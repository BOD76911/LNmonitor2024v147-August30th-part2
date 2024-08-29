/*
 * LNSchedulerExecutor.h
 *
 *  Created on: Sep 14, 2021
 *      Author: BrianRoot
 */

#ifndef LNSCHEDULEREXECUTOR_H_
#define LNSCHEDULEREXECUTOR_H_

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


#include "LNweblocorouteprocess.h"
#include "LNwebLsectionsLdevices.h"
#include "LNdisplayconfig.h"
#include "LNRFidsensorprocessing.h"
#include "LNRouteprocess.h"
#include "LNCLidataentry.h"
#include "LNwebmonSimulator.h"
#include "LNRuntimeLocoRoute.h"
#include "LNSimulatorAllRoutes.h"
#include "LNLocoRoutemovementMasterNG.h"
#include "LNSchedulerAlljourneys.h"
#include "LNwebconfig.h"
#include "LNSimRouteMaster.h"
#include "LNSimRouteMaster.h"
#include "Clocktime.h"

class LNSchedulerExecutor
{
public:
	LNSchedulerExecutor();

	virtual ~LNSchedulerExecutor();

	void InitExecutors();

	void InitializeSchedulerclocktimestamp();

	void UpdateSchedulertimestamp();

	int GetSchedulercurrentclicks();

	void GetSchedulertimestamp(char* replymessage);

	void DisplaySchedulerExecutionstatus(char* replymessage);

	void BrowserSchedulerExecutionresponses(char* replymessage);

	int Findnextexecutor();

	void ScheduleAutostart(LNSchedulerAlljourneys* LNSchedAlljourneys,\
			LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster);

	void ScheduleAutostartNG(LNSchedulerAlljourneys* LNSchedAlljourneys,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, \
				LNweblocorouteprocess* LNLRprocess,LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster);

	int Initiateschedulerun(int Schedulenumber);

	int CheckifSchedulealreadystarted(int Schedulenumber);

	int Performschedulestepcommand(int sched,int step, int stepcmd, LNSchedulerAlljourneys* LNSchedAlljourneys);

	int Checkschedulerexecutionstatus(LNSchedulerAlljourneys* LNSchedAlljourneys,\
			LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster);

	int Startjourneyexecution(int journey,int sched, int schedstep,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,\
			LNwebLsectionsLdevices* LNLSLD,LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster, LNSchedulerAlljourneys* LNallmastschedules);

	void RunScheduler(LNSchedulerExecutor* LNSchedExecutor,LNweblocorouteprocess* LNLRprocess,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG,\
			LNwebmonSimulator* LNSIM,	LNSimulatorAllRoutes* LNSimAllRtes,LNSimRouteMaster* LNSimRteMaster,LNSchedulerAlljourneys* LNSchedAlljourneys,LNwebLsectionsLdevices* LNLSLD);

	void EnableSchedulerexecution(int schedulenumber);

	void DisableSchedulerexecution(int schedulenumber);

	void Resettickcounter();

private:

	struct ScheduleMonitorRecord
	{
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
	};

	struct ScheduleMonitorRecord ScheduleMonitor[MAXNUMBEROFSCHEDULES + 1];

	unsigned int ScheduleRunEnabled[MAXNUMBEROFSCHEDULES + 1];

	char StartHH[2];
	char StartMM[2];
	char StartSS[2];

	long int Startepochtime;
	long int Currentepochtime;

	char CurrHH[2];
	char CurrMM[2];
	char CurrSS[2];

	int Current_HH = 0;
	int Current_MM = 0;
	int Current_SS = 0;

	int schedclickcounter;

	int TotalSchedulerclicksfromstart = 0;

	int SchReadystartschedule;

	int Schtotalstepsinschedule;

	int SchReadystartschedulestep;

	int SchReadystartschedulestepTrainjourney;

	int Schedstartexecutor;

	int SchedstartexecutorSimPsection;

	int Enable = 0;

	int SchedulerExecutionenabled;

	int Locosimcheck = 0;

	int LRMmatchedflag = 0;

	int schedulefound = 0;

	int stepfound = 0;

	int LNTRexecuteNGindex = 0;

	struct ScheduleEnablement
	{
		int ScheduleApprovedtoRun;
	};

	struct ScheduleEnablement ScheduleEnabledTable[MAXNUMBEROFSCHEDULES + 1];


};

#endif /* LNSCHEDULEREXECUTOR_H_ */
