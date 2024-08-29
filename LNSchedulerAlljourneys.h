/*
 * LNSchedulerAlljourneys.h
 *
 *  Created on: Sep 7, 2021
 *      Author: BrianRoot
 */

#ifndef LNSCHEDULERALLJOURNEYS_H_
#define LNSCHEDULERALLJOURNEYS_H_

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

#include "LNwebconfig.h"
#include "LNLocoRoutemovementMasterNG.h"
#include "LNweblocorouteprocess.h"
#include "LNwebLsectionsLdevices.h"
#include "LNRouteSchedMaster.h"

#include "LNScheduleMaster.h"

class LNSchedulerAlljourneys {
public:
	LNSchedulerAlljourneys();
	virtual ~LNSchedulerAlljourneys();

int Getnumberofstoredschedules();

void ResetSchedulerstatus(int schednumber);

void InitSchedMaster();

int StoreSchedMast(int Schedule,int step, int journey,int StartHH, int StartMM, int StartSS, int Startclicks, int Startstatus,\
		int Stepcmd, int Stepcmdp1, int Stepcmdp2,int Stepcmdp3, int Stepcmdp4);

int DisplaySchedMast(char* replymessage);

int BrowserDisplaySchedMast(int detailflag,char* replymessage);

int BrowserDisplaySchedJnyRtedetail(int detailflag,char* replymessage, LNRouteSchedMaster* LNRteSchedmast,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG);

void IncrementNumberofSchedulesinTable(int nn);

int Checkschedulestartreadiness(int schedule, int* schedsteptr, int* startjourneyptr, int schedclock);

int Updateschedulestartstatus(int schedule, int step, int statusvalue);

int GetStepcommand(int schedule, int step);

int GetStepcommandparam1(int schedule, int step);

int GetStepcommandparam2(int schedule, int step);

int GetStepcommandparam3(int schedule, int step);

void Storeschedexecutornumber(int schedule, int executor);

int Getschedexecutornumber(int schedule);

int GetStepcommandparam4(int schedule, int step);

int UpdateschedulestepExecutionstatus(int schedule,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess);

int InitializeScheduleStart(LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess, LNwebLsectionsLdevices* LNLSLD, LNRouteSchedMaster* LNRteSchedMast);

int InitializeScheduleStartNG(int Inputschedule, LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNweblocorouteprocess* LNLRprocess,\
		LNwebLsectionsLdevices* LNLSLD, LNRouteSchedMaster* LNRteSchedMast);

int GetNumberofSchedulesinTable();

void EnableScheduleExecution(int schedulenumber);

void DisableScheduleExecution(int schedulenumber);

int Getnextschedulesteptorun(int schedule);

int Fetchcurrentsteprunning(int schedule);

void Updatecurrentsteprunning(int schedule, int step);

int Getnextschedulesteptorunold(int schedule);

int Getschedulestepjourneycode(int schedule, int step);

void SetstatusonMasterTableSchedulestep(int schedule, int schedstep,int status);

int FetchSpecificScheduleStep(int schedulenum, int stepnum, int &Trainjourney, int &TotalstepsinSchedule, int &Schedulestepstatus);

int Resetscheduletostart(int schedule);

private:

//#define MAXNUMBEROFSCHEDULESRECS    32
//#define MAXNUMBEROFSCHEDULESTEPS    32


    struct ScheduleMasterTable
	{
    	int Schedulenumber;
    	int Schedulerunningstatus;
    	int ScheduleEnabled;
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
	};

    ScheduleMasterTable SchedMaster[MAXNUMBEROFSCHEDULESRECS];

    int NumberofSchedulesinTable = 0;

	int NumberofStoredschedules = 0;

	std::vector<LNScheduleMaster> SchedMasters;
};

#endif /* LNSCHEDULERALLJOURNEYS_H_ */
