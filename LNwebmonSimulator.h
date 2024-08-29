/*
 * LNwebmonSimulator.h
 *
 *  Created on: Jul 20, 2019
 *      Author: BrianRoot
 */

#ifndef LNWEBMONSIMULATOR_H_
#define LNWEBMONSIMULATOR_H_

#include "LNwebconfig.h"
#include "LNweblocorouteprocess.h"
#include "LNwebLsectionsLdevices.h"
#include "LNSimulatorAllRoutes.h"
#include "LNSimRouteMaster.h"
//#include "LNRouteprocess.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

class LNwebmonSimulator {
public:
	LNwebmonSimulator();

	virtual ~LNwebmonSimulator();

//int Simulatorfileload();

void Initsimulatortable(int linetoinit);

void DisplaySimRecord(int SimRte);

int Simprocessing(MsgTimestamp* MSGptr,LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess,LNSimulatorAllRoutes* LNSimAllRtesdata,LNSimRouteMaster* LNSimRteMaster);

void Simstepexecute(LNwebLsectionsLdevices* LNLSLD);

void Savesimrecorddescriptor(int line, int Rtenum, char Rtedescriptor[SIMDESCRIPTIONLEN]);

int DisplaySimulatorTables(int line, char* replymessage);

void DisplaySimulatordescriptions(int displayflag,char* replymessage);

void Setsimrunstatus(int sc);

int Timerprocessing(MsgTimestamp* MSGptr,LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess);

int  Getsimrunstatus();

void ResetSimrunstatus(int Simnumber);


int DisplayMultiSimexecutorstatus(char* replymessage);

int GetSimInitializatiodata(char* replymessage);

int Findrecordindex(int Rte, int step);

int InitMultiSimrun(int sn,  int rte,  int mode, int Loco, int Traincode, int LocoDCC,LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess,LNSimRouteMaster* LNSimRteMaster);

int ResetMultiSimrun(int sn);

int StepMultiSimrun(int sn);

void IncrementNumberofSimroutesloaded();

int StopMultiSimrun(int sn);

void InitMultiSimexecutor(int simnum);

int FindavailableSimulator();

int ProcessSimcommand(char* replymessage, int command);

int GetSimulatorRteRecordscount();

void IncrementRteRecordscount();

void Savesimrecord(int line, int Rtenum,int Rstep,int Timervalue, int Flg1, int SC1, int C1F, int Flg2, int SC2, int C2F,int Flg3, int SC3, int C3F);

int ProcessRtestep(int Rte,int step,LNwebLsectionsLdevices* LNLSLD);  // returns status code 0 = not running, 1 = running, 2 = completed

private:

	LNSimulatorAllRoutes* LNSimprocessAllRtes = new LNSimulatorAllRoutes();

	std::vector<int> Datafetch;

	int Datafetchrecordcount = 0;

	int SimExecutionpaused	 = 0;

	std::vector<int>::iterator iDatacomponent;

	int S1,S2,S3,S1C,S2C,S3C,Commandcode;

	int S1n,S2n,S3n,S1Cn,S2Cn,S3Cn;

	int LRMmatchedflag = 0;

	MsgTimestamp* SimMSGptr;

	int NumberofSimRoutesloaded;

    int Totalsimulatorlines;

    int StepToExecute;

    int SimRunstatus;

    int Simexecutionstatus;

    int Simstepnumber;

    int NumofSimrunning;

    int Simselected;

    int SimRteselected;

    int SimulatorRteRecordscount;

	int SimRunmode;

	struct Simexecutionrecord
	{
		int Simulatornumber;
		int SimRoute;
		int Routestep;
		int Simenabled;
		int Runmode;
		int CurrentPS;
		int Timercounter;
		int Timertarget;
		int TrainrunonSimulator;
		int Loconumber;
		int LocoDCCcode;
		int CurrentLocospeed;
	};

	struct Simexecutionrecord MultiSimExecutor[NUMBEROFSIMEXECUTORS + 1];

	struct SimRoutedescription
	{
		int recnumber;
		int recRte;
		char SimRtedescription[SIMDESCRIPTIONLEN];
	};

	struct SimRoutedescription SimulatorRteDescRecord[MAXSIMROUTESLINES];

	struct Simrecordlayout
	{
		int recnumber;
		int recRte;
		int recRtestep;       // 1 = physical section, 2 = clock cycles , 3 = detectors
		int timervalue;
		int TypeFlag1;
		int Type1code;
		int Type1value;
		int TypeFlag2;
		int Type2code;
		int Type2value;
		int TypeFlag3;
		int Type3code;
		int Type3value;
	};

	struct Simrecordlayout SimulatorRteRecords[MAXSIMROUTESLINES];

	struct Timerprocessrecord
		{


		};

	struct Timerprocessrecord Timerprocesses[MAXTIMERLOOPS];

};

#endif /* LNWEBMONSIMULATOR_H_ */
