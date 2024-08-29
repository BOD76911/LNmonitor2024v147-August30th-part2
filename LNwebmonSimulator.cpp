/*
 * LNwebmonSimulator.cpp
 *
 *  Created on: Jul 20, 2019
 *      Author: BrianRoot
 */

#include "LNwebmonSimulator.h"
#include "LNwebmonNGfileload.h"


#include <sstream>
#include <string>
#include <cstring>
#include <string.h>
#include <strings.h>
#include <iostream>			// include standard library of io routines
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

LNwebmonSimulator::LNwebmonSimulator() {
	// TODO Auto-generated constructor stub

	SimRunstatus = 0;

	NumberofSimRoutesloaded = 0;

	StepToExecute = 0;

	SimulatorRteRecordscount = 0;

	InitMultiSimexecutor(0);
}

LNwebmonSimulator::~LNwebmonSimulator() {
	// TODO Auto-generated destructor stub
}

int LNwebmonSimulator::GetSimInitializatiodata(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,mcc;

	mcc = 0;

	sprintf(outputrec,"\nSimdat-%03d-%s-%s-%s-%s-",NumberofSimRoutesloaded,"Sim Route 1","Sim Route 2","Sim Route 3","Sim Route 4");

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}

	*(replymessage + mcc) = '\n';

	return mcc;
}

void LNwebmonSimulator::Simstepexecute(LNwebLsectionsLdevices* LNLSLD)
{
	int simexecutestatusflag;

	Simstepnumber++;

	simexecutestatusflag = ProcessRtestep(SimRteselected,Simstepnumber,LNLSLD);
}

void LNwebmonSimulator::IncrementRteRecordscount()
{
	SimulatorRteRecordscount++;
}

int LNwebmonSimulator::GetSimulatorRteRecordscount()
{
	return SimulatorRteRecordscount;
}

int LNwebmonSimulator::Findrecordindex(int Rte, int step)
{
	// finds the index for extracting record and step details

	int sc = 0;

	for (sc = 0;sc < MAXSIMROUTESLINES;sc++)
	{
		if ((SimulatorRteRecords[sc].recRte == Rte) && (SimulatorRteRecords[sc].recRtestep == step))
		{
			return sc;
		}
	}

	return 0;
}

int LNwebmonSimulator::ProcessRtestep(int Rte,int step,LNwebLsectionsLdevices* LNLSLD)
{
	int rc = 0;

	//int S1,S2,S3,S1C,S2C,S3C;

	//int S1n,S2n,S3n,S1Cn,S2Cn,S3Cn;

	int recindex = 0;

	//int LRMmatchedflag = 0;

	std::cout<<"\nProcessing Rte step Mode:"<<SimRunmode<<" SteptoExecute:"<<StepToExecute;

	if (SimRunmode == 0)  // simulator is in STEP mode
		{

		if (StepToExecute > 0)
			{

			recindex = Findrecordindex(SimRteselected,Simstepnumber);

			//std::vector<int> Datafetch;

			//Datafetch = LNSimprocessAllRtes->FindselectedSimRoutestep(LRMmatchedflag,SimRteselected,Simstepnumber);

			//std::vector<int>::iterator iDatacomponent = Datafetch.begin();

			//S1n 		= *(iDatacomponent + 3);
			//S1Cn 	= *(iDatacomponent + 4);

			//S2n 		= *(iDatacomponent + 6);
			//S2Cn 	= *(iDatacomponent + 7);

			//S3n 		= *(iDatacomponent + 9);
			//S3Cn 	= *(iDatacomponent + 10);

			S1 = SimulatorRteRecords[recindex].Type1code;

			S1C = SimulatorRteRecords[recindex].Type1value;

			S2 = SimulatorRteRecords[recindex].Type2code;

			S2C = SimulatorRteRecords[recindex].Type2value;

			S3 = SimulatorRteRecords[recindex].Type3code;

			S3C = SimulatorRteRecords[recindex].Type3value;

			printf("\n152 Data comparison S1:%i S1n:%i S1C:%i S1Cn:%i S2:%i S2n:%i",S1,S1n,S1C,S1Cn,S2,S2n);

			if (S2 == 0)
				{
					rc = 2;

					StepToExecute = 0;

					return rc;
				}
			else
				{
					rc = 1;

					PSectionset(S1,S1C);

					PSectionset(S2,S2C);

					std::cout<<"\nSimulator executing a step in STEP mode - PS:"<< S2;

					//SetTimestamp(1,1, SimMSGptr);

				    LNLSLD->LogicalSectionTcodeRefresh();

				    LNLSLD->LogicalDetectorupdateDetectorstatus();

				    LNLSLD->StopFlagReviewprocess();

				    LNLSLD->RefreshLastupdatedetails(1);

				 //   LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

				    StepToExecute = 0;

				    return rc;
				}
			}
		else
			{
				rc = 1;		// maintain processing mode
			}
		}

	if (SimRunmode == 1)  // simulator is in RUN mode
		{

		recindex = Findrecordindex(SimRteselected,Simstepnumber);

		//std::vector<int> Datafetch;

		//Datafetch = LNSimprocessAllRtes->FindselectedSimRoutestep(LRMmatchedflag,SimRteselected,Simstepnumber);

		//std::vector<int>::iterator iDatacomponent = Datafetch.begin();


		//S1n 	= *(iDatacomponent + 3);
		//S1Cn 	= *(iDatacomponent + 4);

		//S2n 	= *(iDatacomponent + 6);
		//S2Cn 	= *(iDatacomponent + 7);

		//S3n 	= *(iDatacomponent + 9);
		//S3Cn 	= *(iDatacomponent + 10);

		S1 = SimulatorRteRecords[recindex].Type1code;

		S1C = SimulatorRteRecords[recindex].Type1value;

		S2 = SimulatorRteRecords[recindex].Type2code;

		S2C = SimulatorRteRecords[recindex].Type2value;

		S3 = SimulatorRteRecords[recindex].Type3code;

		S3C = SimulatorRteRecords[recindex].Type3value;

		printf("\n228 Data comparison S1:%i S1n:%i S1C:%i S1Cn:%i S2:%i S2n:%i",S1,S1n,S1C,S1Cn,S2,S2n);

	//std::cout<<"\nIndex:"<<recindex<<" Rte:"<<Rte<<" step:"<<step<<" Type1:"<<S1<<"-"<<S1C<<" Type2:"<<S2<<"-"<<S2C<<" Type3:"<<S3<<"-"<<S3C;

		if (S2 == 0)
		{
			rc = 2;
		}
		else
		{
			rc = 1;

			PSectionset(S1,S1C);

			PSectionset(S2,S2C);

			std::cout<<"\nSimulator executing a step in RUN mode - PS:"<< S2;

			//SetTimestamp(1,1, SimMSGptr);

	    	LNLSLD->LogicalSectionTcodeRefresh();

	    	LNLSLD->LogicalDetectorupdateDetectorstatus();

	    	LNLSLD->StopFlagReviewprocess();

	    	LNLSLD->RefreshLastupdatedetails(1);
		}

		return rc;
	}

	return rc;
}

int LNwebmonSimulator::Simprocessing(MsgTimestamp* MSGptr,LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess,\
		LNSimulatorAllRoutes* LNSimAllRtesdata,LNSimRouteMaster* LNSimRteMaster)
{
	// Execute simulator

	LNSimprocessAllRtes = LNSimAllRtesdata;

	SimMSGptr = MSGptr;

	int recindex = 0;

	int dataitem = 0;

	int sc;

	while(1)
	{
	  std::this_thread::sleep_for (std::chrono::seconds(1));

	  if (SimExecutionpaused == 0)
	  {

	  for (sc = 1; sc <= NUMBEROFSIMEXECUTORS;sc++)
		{

		if (MultiSimExecutor[sc].Simenabled == 1)		// check for each simulator enabled
		{
			if (LNLRprocess->GetLocoEnabled(MultiSimExecutor[sc].Loconumber) == 0)
			{
				std::cout<<"\n436 Loco not enabled so executor:"<<sc<<" now disabled";
				MultiSimExecutor[sc].Simenabled = 0;
			}

			// If enabled, checking for when Simulator is in RUN mode

			if ((MultiSimExecutor[sc].Timercounter == MultiSimExecutor[sc].Timertarget) && (MultiSimExecutor[sc].Timertarget > 0) && \
					(MultiSimExecutor[sc].Runmode > 0) && (MultiSimExecutor[sc].Simenabled == 1))     // Run mode , Timer counter target > 0
			{

				MultiSimExecutor[sc].Routestep++;			// counter matches target so increment route counter
				MultiSimExecutor[sc].Timercounter = 0;		// Reset the counter timer

				recindex = Findrecordindex(MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

				//Datafetchrecordcount = LNSimprocessAllRtes->GetselectedSimRoutestepcount(LRMmatchedflag,MultiSimExecutor[sc].SimRoute);

				Datafetchrecordcount = LNSimRteMaster->GetselectedSimRoutestepcount(MultiSimExecutor[sc].SimRoute);

				if (Datafetchrecordcount < MultiSimExecutor[sc].Routestep)
				{
					// have processed the last step on the route

					MultiSimExecutor[sc].Simenabled = 2;  // mark status as completed

					return 0;
				}

				//if (sc == 1)
				//{
				//	std::cout<<"\n313 Sim Exec Datafetchreccount:"<<Datafetchrecordcount<<" on Route:"<< MultiSimExecutor[sc].SimRoute<<" for Rtestep:"<<MultiSimExecutor[sc].Routestep;
//
				//}

				if (Datafetchrecordcount > 0)
				{
				//	Datafetch = LNSimprocessAllRtes->FindselectedSimRoutestep(LRMmatchedflag,MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

					Datafetch = LNSimRteMaster->Getstepdetail(MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

					iDatacomponent = Datafetch.begin();

					//std::cout<<"\nSim record details Comp0:"<<*(iDatacomponent + 0)<<" Comp 1:"<<*(iDatacomponent + 1)<<" Comp 2:"<<*(iDatacomponent + 2);

					Commandcode = *(iDatacomponent + 2);

					if (Commandcode == 1)	// check to determine processing required for physical section update record type
					{

					S1 		= *(iDatacomponent + 3);
					S1C		= *(iDatacomponent + 4);

					S2		= *(iDatacomponent + 6);
					S2C 	= *(iDatacomponent + 7);

					S3 		= *(iDatacomponent + 9);
					S3C 	= *(iDatacomponent + 10);

					MultiSimExecutor[sc].CurrentPS = S1;

					PSectionset(S1,S1C);

					PSectionset(S2,S2C);

					LNLSLD->LogicalSectionTcodeRefresh();

					LNLSLD->LogicalDetectorupdateDetectorstatus();

					LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

					MultiSimExecutor[sc].Timertarget = *(iDatacomponent + 1);  // set the simulator timer target for next cycle
					}

					if (Commandcode == 3)	// check to determine processing required for physical section update record type
					{

						S1 		= *(iDatacomponent + 3);
						S1C		= *(iDatacomponent + 4);

						S2		= *(iDatacomponent + 6);
						S2C 	= *(iDatacomponent + 7);

						S3 		= *(iDatacomponent + 9);
						S3C 	= *(iDatacomponent + 10);

						std::cout<<"\nActioning device 364--> comp0:"<<*(iDatacomponent + 0)<<" comp1:"<<*(iDatacomponent + 1)<<" comp2:"<<*(iDatacomponent + 2)<<" comp3:"<<*(iDatacomponent + 3)<<\
								" comp4:"<<*(iDatacomponent + 4)<<" comp5:"<<*(iDatacomponent + 5)<<\
								" comp6:"<<*(iDatacomponent + 6)<<" comp7:"<<*(iDatacomponent + 7)<<" comp8:"<<*(iDatacomponent + 8)<<" comp9:"<<*(iDatacomponent + 9)<<\
								" comp10:"<<*(iDatacomponent + 10);

						int Simdevice = *(iDatacomponent + 5);
						int SimdevicePSref = *(iDatacomponent + 8);
						int SimdevicePSrefaction = *(iDatacomponent + 9);

						PSectionset(SimdevicePSref,SimdevicePSrefaction);

					//	std::cout<<"\n377 PSectionset for device:"<<SimdevicePSref<<" to value:"<<SimdevicePSrefaction;

						LNLSLD->LogicalSectionTcodeRefresh();

						LNLSLD->LogicalDetectorupdateDetectorstatus();

						LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

						MultiSimExecutor[sc].Timertarget = *(iDatacomponent + 1);  // set the simulator timer target for next cycle
					}

					if (Commandcode == 4)	// check to determine processing required for physical section update record type
					{
						std::cout<<"\n361....;Actioning Device command in Simulator step:"<<*(iDatacomponent + 1)<<" Device:"<<S1<<" Target:"<<S3;

						LNLSLD->UpdateDcode(S1,1);
					}


					MultiSimExecutor[sc].Timercounter = 0;			// reset the timer counter

					if ((S2 == 0) || (Datafetchrecordcount == MultiSimExecutor[sc].Routestep)) // check to see if the next section is zero
					{
							MultiSimExecutor[sc].Simenabled = 2;  // mark status as completed

							return 0;
					}
				}
				else
				{
					std::cout<<"\n345 Simulator records NOT found for requested route";
					return 0;
				}
			}
			else
			{
				// Not yet reached the timer target in RUN mode

				if ((MultiSimExecutor[sc].Timercounter != MultiSimExecutor[sc].Timertarget) && (MultiSimExecutor[sc].Timertarget > 0) && \
					(MultiSimExecutor[sc].Runmode > 0))
				{
					MultiSimExecutor[sc].CurrentLocospeed = LNLRprocess->GetDCCLocospeed(MultiSimExecutor[sc].Loconumber);

					if (MultiSimExecutor[sc].CurrentLocospeed > 0)
					{
						MultiSimExecutor[sc].Timercounter++;  // increment counter
					}
					else
					{
						//std::cout<<"\n363 Sim executor:"<<sc<<" timer count held as Current locospeed at zero";
						if (LNLRprocess->GetLocoEnabled(MultiSimExecutor[sc].Loconumber) == 0)
						{
						//	std::cout<<"\n371 Loco no longer enabled so executor:"<<sc<<" so Executor now disabled";
							MultiSimExecutor[sc].Simenabled = 0;
						}

						// check if loco is still enabled , if not then reset executor
					}
				}
			}		// end of RUN mode checking


			// Checking Simulator when in STEP mode

			if ((MultiSimExecutor[sc].Runmode == 0) && (MultiSimExecutor[sc].Simenabled == 1) && \
					(MultiSimExecutor[sc].Timercounter == MultiSimExecutor[sc].Timertarget))
			{
				MultiSimExecutor[sc].Routestep++;			// counter matches target so increment route counter
				MultiSimExecutor[sc].Timercounter = 0;

				//recindex = Findrecordindex(MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

				//Datafetchrecordcount = LNSimprocessAllRtes->GetselectedSimRoutestepcount(LRMmatchedflag,MultiSimExecutor[sc].SimRoute);

				Datafetchrecordcount = LNSimRteMaster->GetselectedSimRoutestepcount(MultiSimExecutor[sc].SimRoute);

				if (Datafetchrecordcount < MultiSimExecutor[sc].Routestep)
				{
					// have processed the last step on the route in STEP mode

					MultiSimExecutor[sc].Simenabled = 2;  // mark status as completed

					return 0;
				}

				//Datafetch = LNSimprocessAllRtes->FindselectedSimRoutestep(LRMmatchedflag,MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

				Datafetch = LNSimRteMaster->Getstepdetail(MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep);

				//std::vector<int>::iterator iDatacomponent = Datafetch.begin();

				Commandcode = *(iDatacomponent + 2);

				if (Commandcode == 1)	// check to determine processing required for record type
				{
				S1 		= *(iDatacomponent + 3);
				S1C		= *(iDatacomponent + 4);

				S2 		= *(iDatacomponent + 6);
				S2C 	= *(iDatacomponent + 7);

				S3 		= *(iDatacomponent + 9);
				S3C 	= *(iDatacomponent + 10);

				MultiSimExecutor[sc].CurrentPS = S1;

				PSectionset(S1,S1C);

				PSectionset(S2,S2C);

				LNLSLD->LogicalSectionTcodeRefresh();

				LNLSLD->LogicalDetectorupdateDetectorstatus();

				LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover
				}

				MultiSimExecutor[sc].Timercounter = 0;			// reset the timer counter

				MultiSimExecutor[sc].Timertarget = *(iDatacomponent + 1);  // set the simulator timer target for next cycle


				if ((S2 == 0) || (Datafetchrecordcount == MultiSimExecutor[sc].Routestep)) // check to see if the next section is zero
				{
					MultiSimExecutor[sc].Simenabled = 2;  // mark status as completed

					MultiSimExecutor[sc].Routestep = 0;
					MultiSimExecutor[sc].SimRoute = 0;
					MultiSimExecutor[sc].Timercounter = 0;

					MultiSimExecutor[sc].Loconumber = 0;
					MultiSimExecutor[sc].TrainrunonSimulator = 0;
					MultiSimExecutor[sc].LocoDCCcode = 0;

					std::cout<<"\n501 - STEP mode - Reached end of route:"<< MultiSimExecutor[sc].SimRoute<< " - simulator stopped";
				}
			} // end of STEP mode checking
		}		// end of Simulator SC checking
	}			// end of EXECUTORS checking
	  }
  }				// end of WHILE LOOP
}				// end of Function

int LNwebmonSimulator::ProcessSimcommand(char* replymessage, int command)
{
	int sc,ic, isc, icc,mcc;
	int Rcc;
	int lcc;

	mcc = 0;

	char outputrec[OUTPUTRECSIZE];

	if (command == 0)
	{
		SimExecutionpaused	= 0;
	}
	else
	{
		SimExecutionpaused	= 1;
	}

	if (SimExecutionpaused > 0)
	{
		sprintf(outputrec,"\nSimulator Run is Paused \n");
	}
	else
	{
		sprintf(outputrec,"\nSimulator Run is Enabled \n");
	}

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}

	*(replymessage + mcc) = '\n';

	return mcc;
}


int LNwebmonSimulator::DisplayMultiSimexecutorstatus(char* replymessage)
{
	/*
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
	};

	struct Simexecutionrecord MultiSimExecutor[NUMBEROFSIMEXECUTORS + 1];
	*/

	int sc, isc, icc,mcc;
	int Rcc;
	int lcc;

	mcc = 0;

	char outputrec[OUTPUTRECSIZE];

	Rcc = 0;

	lcc = 0;

	snprintf(outputrec,OUTPUTRECSIZE,"\nMSimExec");

	isc =  std::strlen(outputrec);

	for (icc = 0;icc <isc;icc++)
		{
		*(replymessage + mcc) = outputrec[icc];
		mcc++;
		}

	for (sc = 0;sc < (NUMBEROFSIMEXECUTORS + 1);sc++)
	{
		snprintf(outputrec,OUTPUTRECSIZE,"+-%02d-%03d-%02d-%01d-%01d-%03d-%02d-%02d",MultiSimExecutor[sc].Simulatornumber,\
				MultiSimExecutor[sc].SimRoute,MultiSimExecutor[sc].Routestep,MultiSimExecutor[sc].Simenabled,\
				MultiSimExecutor[sc].Runmode,MultiSimExecutor[sc].CurrentPS,MultiSimExecutor[sc].Timercounter,\
				MultiSimExecutor[sc].Timertarget);

		isc =  std::strlen(outputrec);

		for (icc = 0;icc <isc;icc++)
			{
			*(replymessage + mcc) = outputrec[icc];
			mcc++;
			}
	}

	//std::cout<<"\nMultiSim data->"<<replymessage;

	return mcc;
}

void LNwebmonSimulator::InitMultiSimexecutor(int simnum)
{
	/*
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
	};

	struct Simexecutionrecord SimulatorExecutor[NUMBEROFSIMEXECUTORS];

	*/
	int sc;

	int sctarget;

	int scstart;

	if (simnum == 0)
	{
		scstart = 0;

		sctarget = (NUMBEROFSIMEXECUTORS + 1);
	}
	else
	{
		scstart = simnum;

		sctarget = simnum + 1;
	}

	for (sc = scstart;sc < sctarget;sc++)
	{
		MultiSimExecutor[sc].Simulatornumber 		= sc;
		MultiSimExecutor[sc].CurrentPS 				= 0;
		MultiSimExecutor[sc].Routestep 				= 0;
		MultiSimExecutor[sc].Runmode 				= 0;
		MultiSimExecutor[sc].SimRoute 				= 0;
		MultiSimExecutor[sc].Simenabled 			= 0;
		MultiSimExecutor[sc].Timercounter 			= 0;
		MultiSimExecutor[sc].Timertarget 			= 0;
		MultiSimExecutor[sc].TrainrunonSimulator 	= 0;
		MultiSimExecutor[sc].Loconumber 			= 0;
		MultiSimExecutor[sc].LocoDCCcode 			= 0;
		MultiSimExecutor[sc].CurrentLocospeed 		= 0;
	}
}

int LNwebmonSimulator::ResetMultiSimrun(int sn)
{
	MultiSimExecutor[sn].Simenabled 	= 0;
	MultiSimExecutor[sn].Routestep 		= 0;
	MultiSimExecutor[sn].CurrentPS 		= 0;
	MultiSimExecutor[sn].Timertarget 	= 0;
	MultiSimExecutor[sn].Timercounter 	= 0;
	return 0;
}

int LNwebmonSimulator::StopMultiSimrun(int sn)
{
	MultiSimExecutor[sn].Simenabled 	= 0;
	return 0;
}

int LNwebmonSimulator::StepMultiSimrun(int sn)
{
	MultiSimExecutor[sn].Timercounter 	= 6;
	MultiSimExecutor[sn].Timertarget 	= 6;
	return 0;
}


int LNwebmonSimulator::InitMultiSimrun(int sn, int rte, int mode, int Loco, int Traincode, int LocoDCC, LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess,LNSimRouteMaster* LNSimRteMaster)
{

	// Initialize simulator sn on simulator route rte

	int recindex = 0;

	MultiSimExecutor[sn].Simenabled = 1;
	MultiSimExecutor[sn].Routestep = 1;
	MultiSimExecutor[sn].SimRoute = rte;
	MultiSimExecutor[sn].Timercounter = 0;

	MultiSimExecutor[sn].Loconumber = Loco;
	MultiSimExecutor[sn].TrainrunonSimulator = Traincode;
	MultiSimExecutor[sn].LocoDCCcode = LocoDCC;


	//recindex = Findrecordindex(MultiSimExecutor[sn].SimRoute,MultiSimExecutor[sn].Routestep);

	std::cout<<"\n\n728 WebmonSimulator - Initiating simulator for route:"<<MultiSimExecutor[sn].SimRoute<<" at step:"<<MultiSimExecutor[sn].Routestep;

	// Retrieve the number of simulator records stored for this route

	//Datafetchrecordcount = LNSimprocessAllRtes->GetselectedSimRoutestepcount(LRMmatchedflag,MultiSimExecutor[sn].SimRoute);

	Datafetchrecordcount = LNSimRteMaster->GetselectedSimRoutestepcount(MultiSimExecutor[sn].SimRoute);

	if (Datafetchrecordcount > 0)
	{

		//Datafetch = LNSimprocessAllRtes->FindselectedSimRoutestep(LRMmatchedflag,MultiSimExecutor[sn].SimRoute,MultiSimExecutor[sn].Routestep);

		Datafetch = LNSimRteMaster->Getstepdetail(MultiSimExecutor[sn].SimRoute,MultiSimExecutor[sn].Routestep);

		std::cout<<"\nSimulator details for route:"<<MultiSimExecutor[sn].SimRoute<<" Rec count:"<<Datafetchrecordcount<<"\n";

		std::vector<int>::iterator iDatacomponent = Datafetch.begin();

		S1 		= *(iDatacomponent + 3);
		S1C	= *(iDatacomponent + 4);

		S2		= *(iDatacomponent + 6);
		S2C 	= *(iDatacomponent + 7);

		S3 		= *(iDatacomponent + 9);
		S3C 	= *(iDatacomponent + 10);

	//	printf("\n756 Simulator initialization for Route:%i found %i simulator records to process\n",MultiSimExecutor[sn].SimRoute,Datafetchrecordcount);

	}
	else
	{
		std::cout<<"\n625 Simulator records not found for requested route";
		//return 99;
	}

//	std::cout<<"\n633 Multisimprocess";

	MultiSimExecutor[sn].CurrentPS = S2;

	PSectionset(S1,S1C);

	PSectionset(S2,S2C);

	MultiSimExecutor[sn].CurrentPS = S1;

//	std::cout<<"\n643 Multisimprocess";

	LNLSLD->LogicalSectionTcodeRefresh();

	LNLSLD->LogicalDetectorupdateDetectorstatus();

	LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

	MultiSimExecutor[sn].Timercounter = 0;			// reset the timer counter



	if (mode > 0)		// test for mode 0 = STEP, 1 = RUN
	{
		MultiSimExecutor[sn].Runmode = 1;			// set mode to RUN
		MultiSimExecutor[sn].Timertarget = 5;		// sent counter target = 5 for 5 second delay
	}
	else
	{
		MultiSimExecutor[sn].Runmode = 0;
		MultiSimExecutor[sn].Timertarget = 6;		// sent counter target = 6 for STEP mode
	}

	return 0;
}

void LNwebmonSimulator::IncrementNumberofSimroutesloaded()
{
	NumberofSimRoutesloaded++;
}

int LNwebmonSimulator::FindavailableSimulator()			// search for first available simulator
{
	int Simulatornumber = 0;

	int scn;

	for (scn = 1;scn <(NUMBEROFSIMEXECUTORS + 1);scn++)
	{

		if ((MultiSimExecutor[scn].Simenabled == 0) && (MultiSimExecutor[scn].TrainrunonSimulator == 0)) // check to see if simulator executor is available
		{
			return scn;
		}
	}

	return Simulatornumber;
}

void LNwebmonSimulator::Initsimulatortable(int linetoinit)
{
	/*
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
	 */
  int simlineinit;

  for (simlineinit = 0;simlineinit < linetoinit;simlineinit++)
	{
	  SimulatorRteRecords[simlineinit].recnumber = simlineinit;
	  SimulatorRteRecords[simlineinit].recRtestep = 0;
	  SimulatorRteRecords[simlineinit].timervalue = 0;
	  SimulatorRteRecords[simlineinit].TypeFlag1 = 0;
	  SimulatorRteRecords[simlineinit].Type1code = 0;
	  SimulatorRteRecords[simlineinit].Type1value = 0;
	  SimulatorRteRecords[simlineinit].TypeFlag2 = 0;
	  SimulatorRteRecords[simlineinit].Type2code = 0;
	  SimulatorRteRecords[simlineinit].Type2value = 0;
	  SimulatorRteRecords[simlineinit].TypeFlag3 = 0;
	  SimulatorRteRecords[simlineinit].Type3code = 0;
	  SimulatorRteRecords[simlineinit].Type3value = 0;
	}

	printf("\n%03d Simulator lines created and initialized\n",linetoinit);

	int cc;

	for (simlineinit = 0;simlineinit < MAXSIMROUTESLINES;simlineinit++)
	{
		SimulatorRteDescRecord[simlineinit].recnumber = simlineinit;
		SimulatorRteDescRecord[simlineinit].recRte = 0;

		for (cc = 0; cc < SIMDESCRIPTIONLEN;cc++)
		{
			SimulatorRteDescRecord[simlineinit].SimRtedescription[cc] = '-';
		}
		SimulatorRteDescRecord[simlineinit].SimRtedescription[SIMDESCRIPTIONLEN-1] = '\n'; // insert carriage return at last character
	}

	std::cout<<"\n"<< MAXSIMROUTESLINES <<" Simulator description records created and initialized\n";

	StepToExecute = 0;

	//DisplaySimulatordescriptions(0, 0);
}

void LNwebmonSimulator::Savesimrecorddescriptor(int line, int Rtenum, char Rtedescriptor[SIMDESCRIPTIONLEN])
{
	//printf("\nSaving descriptor Rec %03d Rte %03d Desc %s ", line,Rtenum,Rtedescriptor);

	SimulatorRteDescRecord[line].recnumber = line;
	SimulatorRteDescRecord[line].recRte = Rtenum;

	int ccm;

	for (ccm = 0;ccm<SIMDESCRIPTIONLEN;ccm++)
	{
		//printf(" ccm %i %c",ccm,Rtedescriptor[ccm]);
		SimulatorRteDescRecord[line].SimRtedescription[ccm] = Rtedescriptor[ccm];
	}

	std::cout<<"\nDesc saved "<<"line:"<<line<<" Rte:"<<Rtenum<<" Desc:"<<SimulatorRteDescRecord[line].SimRtedescription;

	//DisplaySimulatordescriptions(0, 0);
}

void LNwebmonSimulator::Savesimrecord(int line, int Rtenum,int Rstep,int Timervalue, int Flg1, int SC1, int C1F, int Flg2, int SC2, int C2F,int Flg3, int SC3, int C3F)
{
	SimulatorRteRecords[line].recnumber = line;
	SimulatorRteRecords[line].recRte = Rtenum;
	SimulatorRteRecords[line].recRtestep = Rstep;
	SimulatorRteRecords[line].timervalue = Timervalue;
	SimulatorRteRecords[line].TypeFlag1 = Flg1;
	SimulatorRteRecords[line].Type1code = SC1;
	SimulatorRteRecords[line].Type1value = C1F;
	SimulatorRteRecords[line].TypeFlag2 = Flg2;
	SimulatorRteRecords[line].Type2code = SC2;
	SimulatorRteRecords[line].Type2value = C2F;
	SimulatorRteRecords[line].TypeFlag3 = Flg3;
	SimulatorRteRecords[line].Type3code = SC3;
	SimulatorRteRecords[line].Type3value = C3F;

	Totalsimulatorlines++;
}

int LNwebmonSimulator::DisplaySimulatorTables(int line, char* replymessage)
{
	/*
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

	char outputrec[OUTPUTRECSIZE];

	*/

	char outputrec[OUTPUTRECSIZE];

	int isc,ic,mcc;

	mcc = 0;

	int simlineinit;

	if (line == 0)  // line = 0, display all lines
	{
		std::cout<<"\nDisplay all simulator records in table "<< Totalsimulatorlines <<"\n";

		for (simlineinit = 1;simlineinit < MAXSIMROUTESLINES;simlineinit++)
		{
			if (SimulatorRteRecords[simlineinit].recRte > 0)
				{
				sprintf(outputrec,"\nRec;%03d;Rte;%03d;step;%03d;Timer;%03d;Flag1;%03d;%03d;%03d;Flag2;%03d;%03d;%03d;Flag3;%03d;%03d;%03d",\
					SimulatorRteRecords[simlineinit].recnumber,\
					SimulatorRteRecords[simlineinit].recRte,\
					SimulatorRteRecords[simlineinit].recRtestep,\
					SimulatorRteRecords[simlineinit].timervalue,\
					SimulatorRteRecords[simlineinit].TypeFlag1,\
					SimulatorRteRecords[simlineinit].Type1code,\
					SimulatorRteRecords[simlineinit].Type1value,\
					SimulatorRteRecords[simlineinit].TypeFlag2,\
					SimulatorRteRecords[simlineinit].Type2code,\
					SimulatorRteRecords[simlineinit].Type2value,\
					SimulatorRteRecords[simlineinit].TypeFlag3,\
					SimulatorRteRecords[simlineinit].Type3code,\
					SimulatorRteRecords[simlineinit].Type3value);
					//std::cout<<"\n"<<outputrec;

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}

					*(replymessage + mcc) = '\n';
				}
		}
		return mcc;
	}
	else
	{
		simlineinit = line;

		sprintf(outputrec,"Rec;%03d;Rte;%03d;step;%03d;Timer;%03d;Flg1;%01d;%03d;%01d;Flag2;%01d;%03d;%01d;Flag3;%01d;%03d;%01d",\
		SimulatorRteRecords[simlineinit].recnumber,\
		SimulatorRteRecords[simlineinit].recRte,\
		SimulatorRteRecords[simlineinit].recRtestep,\
		SimulatorRteRecords[simlineinit].timervalue,\
		SimulatorRteRecords[simlineinit].TypeFlag1,\
		SimulatorRteRecords[simlineinit].Type1code,\
		SimulatorRteRecords[simlineinit].Type1value,\
		SimulatorRteRecords[simlineinit].TypeFlag2,\
		SimulatorRteRecords[simlineinit].Type2code,\
		SimulatorRteRecords[simlineinit].Type2value,\
		SimulatorRteRecords[simlineinit].TypeFlag3,\
		SimulatorRteRecords[simlineinit].Type3code,\
		SimulatorRteRecords[simlineinit].Type3value);

		isc =  std::strlen(outputrec);

		std::cout<<"\n"<<outputrec;

		return mcc;
	}
}

void LNwebmonSimulator::DisplaySimulatordescriptions(int displayflag,char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,icc,mcc;

	mcc = 0;

	int hdrs = NumberofSimRoutesloaded;

	sprintf(outputrec,"SimHDR-%02d-",NumberofSimRoutesloaded);

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

	for (ic = 1;ic <= hdrs;ic++)
	{
		sprintf(outputrec,"%s+",SimulatorRteDescRecord[ic].SimRtedescription);

		//printf("\nOutputrec built %i %s",ic,outputrec);

		isc =  std::strlen(outputrec);

		for (icc = 0;icc <isc;icc++)
			{
				*(replymessage + mcc) = outputrec[icc];
				mcc++;
			}
	}

	if (displayflag > 0)  // flag = 0, display all descriptions in the table
	{
		std::cout<<"\n"<<hdrs<<" Sim descriptions to show";
		std::cout<<"\n"<<replymessage;
	}

	return;
}


int LNwebmonSimulator::Timerprocessing(MsgTimestamp* MSGptr,LNwebLsectionsLdevices* LNLSLD,LNweblocorouteprocess* LNLRprocess)
{
	// Execute simulator

  while(1)
  {

	std::this_thread::sleep_for (std::chrono::milliseconds(2000));

//	std::cout<<"\nTimer processor TICK ....\n";

  }
}







