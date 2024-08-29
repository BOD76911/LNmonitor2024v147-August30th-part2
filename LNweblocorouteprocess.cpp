/*
 * LNweblocorouteprocess.cpp
 *
 *  Created on: Jul 25, 2019
 *      Author: BrianRoot
 */

#include "LNweblocorouteprocess.h"
#include "LNwebLsectionsLdevices.h"

#include "TCPprocessing.h"
#include <string>
#include <string.h>
#include <cstring>
#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <stdio.h>			// libraries

LNweblocorouteprocess::LNweblocorouteprocess(LNwebLsectionsLdevices* LRLSLDparam, LNlocoAlldata* LNlocodatatables, LNallTrains* LNallTrainsinformationparam)
{
	// TODO Auto-generated constructor stub

	LSLD = LRLSLDparam;

	LCSPDtable = LNlocodatatables;

	LNallTrainsinformation = LNallTrainsinformationparam;

	LocodatarecordsInit(0);		// clear loco records

	RoutedatarecordsInit(0);	 // clear route data records
}

LNweblocorouteprocess::~LNweblocorouteprocess() 
{
	// TODO Auto-generated destructor stub
}

void LNweblocorouteprocess::Passloggingpointer(LNlogging* LNlogging)
{
	LNloggingptr = LNlogging;
}

int LNweblocorouteprocess::Locodatarecordsdisplayonconsole(int loco, char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];
	
	std::cout<<"\nLNweblocorouteprocess line 44 - Selected loco:"<< loco;

	int mcc;

	mcc = 0;
	
	//return mcc;
	
	bzero(outputrec,OUTPUTRECSIZE);
	
	//if (Locoinformation[loco].Route > 0)
	//{

	printf("\nLoco route details for Loco %02d on Route %02d \n DCC code 	  %03d\
	 \n Route %03d \n Step %02d \n Substep %02d \n Enabled %01d \
					\nJourney %02d \nJourneystatus %02d \nSignal hold %01d\
					\nPS: %03d CS: %03d  NS: %03d \nDirection %01d Speed %2d Planspeed %02d ActDCCvalue %02d \
					\nCmdinprogress %02d Loco missing flag %01d Stop detector %02d LastknownLS %03d \
					 \nTraincode %03d Rteflagsdvce %02d Rteflagsdvcecntplan %02d \nSlot number %02d \
					 \nDescription: %s \nSimulatornumber %02d\nSystemhold %02d\
					 \nLoop counter target %02d\nLoop counter act %02d\nLoopstopflag %01d Sectionlogicflag %01d",\
	loco,\
	Locoinformation[loco].Route,\
	Locoinformation[loco].LocoDCCcode,\
	Locoinformation[loco].Route,\
	Locoinformation[loco].Rtestep,\
	Locoinformation[loco].Rtesubstep,\
	Locoinformation[loco].Enabled,\
	Locoinformation[loco].Journey,\
	Locoinformation[loco].Journeystatus,\
	Locoinformation[loco].Signalhold,\
	Locoinformation[loco].PS,\
	Locoinformation[loco].CS,\
	Locoinformation[loco].NS,\
	Locoinformation[loco].Direction,\
	Locoinformation[loco].Speed,\
	Locoinformation[loco].Planspeed,\
	Locoinformation[loco].ActDCCvalue,\
	Locoinformation[loco].Cmdinprogress,\
	Locoinformation[loco].Locomissingflag,\
	Locoinformation[loco].Stopdetector,\
	Locoinformation[loco].LastknownLS,\
	Locoinformation[loco].Traincode,\
	Locoinformation[loco].Rteflagsdvce[0],\
	Locoinformation[loco].Rteflagsdvcecntplan[0],\
	Locoinformation[loco].Slotnumber,\
	Locoinformation[loco].description,\
	Locoinformation[loco].Simulatornumber,\
	Locoinformation[loco].Systemhold,\
	Locoinformation[loco].Loopcountertarget,\
	Locoinformation[loco].Loopcounteractual,\
	Locoinformation[loco].Loopstopflag,\
	Locoinformation[loco].Sectionlogicflag);

	printf("\nLS route history\n");
	for (int lsc = 1; lsc < Routelinesmax; lsc++)
	{
		printf("\nStep:%03d LS:%03d",lsc,Locoinformation[loco].Routesectionhistory[lsc]);
	}
	//}
	//else
	//{
	//	std::cout<<"\n\nLoco not executing a route";
	//}
	
	//isc =  std::strlen(outputrec);

	//				for (ic = 0;ic <isc;ic++)
	//				{
	//					*(replymessage + mcc) = outputrec[ic];
	//					mcc++;
	//				}
	return mcc;
}

int LNweblocorouteprocess::Locodatarecordsdisplay(int loco, char* replymessage)
{
	/*
	Locodatarecord
		{
		unsigned int Locoindex;
		unsigned int LocoDCCcode;
		unsigned int Route;
		unsigned int Rtestep;
		unsigned int Rtesubstep;
		unsigned int Enabled;
		unsigned int Journey;
		unsigned int Journeystatus;
		unsigned int Signalhold;
		unsigned int PS;
		unsigned int CS;
		unsigned int NS;
		unsigned int Direction;
		unsigned int Speed;
		unsigned int Planspeed;
		unsigned int Cmdinprogress;
		unsigned int Loopcountertarget;
		unsigned int Loopcounteractual;
		unsigned int Loopstopflag;
		unsigned int Locomissingflag;
		unsigned int Stopdetector;
		unsigned int LastknownLS;
		unsigned int Traincode;
		unsigned int Rteflagsdvce[Routeflagscnt];
		unsigned int Rteflagsdvcecntplan[Routeflagscnt];
		unsigned char description[Locodescriptionlen];
	};

struct Locodatarecord Locoinformation[Lococountmax];
	*/

	//std::cout << "Display loco details  \n";

	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (scc = 1;scc < Lococountmax ;scc++)
				{
					bzero(outputrec,OUTPUTRECSIZE);

					sprintf(outputrec,"\n+-%02d-%04d-%03d-%02d-%02d-%01d-%01d-%01d-%01d-%03d-%03d-%03d-%01d-%02d-%02d-%02d-%02d-%01d-%01d-%03d-%04d-%02d-%02d-%02d-%s-%03d-%01d-%03d-%03d-%01d-%01d-%03d-%03d-%03d-",\
							scc,\
							Locoinformation[scc].LocoDCCcode,\
							Locoinformation[scc].Route,\
							Locoinformation[scc].Rtestep,\
							Locoinformation[scc].Rtesubstep,\
							Locoinformation[scc].Enabled,\
							Locoinformation[scc].Journey,\
							Locoinformation[scc].Journeystatus,\
							Locoinformation[scc].Signalhold,\
							Locoinformation[scc].PS,\
							Locoinformation[scc].CS,\
							Locoinformation[scc].NS,\
							Locoinformation[scc].Direction,\
							Locoinformation[scc].Speed,\
							Locoinformation[scc].Planspeed,\
							Locoinformation[scc].ActDCCvalue,\
							Locoinformation[scc].Cmdinprogress,\
							Locoinformation[scc].Locomissingflag,\
							Locoinformation[scc].Stopdetector,\
							Locoinformation[scc].LastknownLS,\
							Locoinformation[scc].Traincode,\
							Locoinformation[scc].Rteflagsdvce[0],\
							Locoinformation[scc].Rteflagsdvcecntplan[0],\
							Locoinformation[scc].Slotnumber,\
							Locoinformation[scc].description,\
							Locoinformation[scc].Simulatornumber,\
							Locoinformation[scc].Systemhold,\
							Locoinformation[scc].Loopcountertarget,\
							Locoinformation[scc].Loopcounteractual,\
							Locoinformation[scc].Loopstopflag,\
							Locoinformation[scc].Sectionlogicflag,\
							Locoinformation[scc].PSTrainsection,\
							Locoinformation[scc].CSTrainsection,\
							Locoinformation[scc].NSTrainsection);

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}
				}

	return mcc++;


}

void LNweblocorouteprocess::SaveRouteline(int Rte, int Rtestep, int Rtesubstep,\
			int RtePS,int RteCS, int RteNS, int Rtecmd, \
			int Rtecmdparam1,int Rtecmdparam2, int Rtecmdparam3,\
			int Rtecmdparam4,char *cmd, int dispflag)
{
	int cc;

	RouteData[Rte][Rtestep][Rtesubstep].RoutePS = RtePS;
	RouteData[Rte][Rtestep][Rtesubstep].RouteCS = RteCS;
	RouteData[Rte][Rtestep][Rtesubstep].RouteNS = RteNS;
	RouteData[Rte][Rtestep][Rtesubstep].RouteCmd = Rtecmd;

	RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1 = Rtecmdparam1;
	RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2 = Rtecmdparam2;
	RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam3 = Rtecmdparam3;
	RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam4 = Rtecmdparam4;

	for (cc = 0;cc < COMMMANDCHARLENGTH;cc++)
	{
		RouteData[Rte][Rtestep][Rtesubstep].Rtecommand[cc] = '=';
	}
}

int LNweblocorouteprocess::CheckIfrouteloaded(int route)
{
	/*
	struct Routerecord
{
	unsigned int Route;
	unsigned int Routestep;
	unsigned int Routesubstep;
	unsigned int RoutePS;
	unsigned int RouteCS;
	unsigned int RouteNS;
	unsigned int RouteCmd;
	unsigned int RouteCmdparam1;
	unsigned int RouteCmdparam2;
	unsigned int RouteCmdparam3;
	unsigned int RouteCmdparam4;
	unsigned char Rtecommand[COMMMANDCHARLENGTH];
};

struct Routerecord RouteData[Routecount][Routelinesmax][Routemaxsubsteps];

	*/

	if (RouteData[route][1][1].RouteCS > 0)		// check to see if current section details exist for step 1 of route
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

int LNweblocorouteprocess::Routedatarecordsdisplay(int Rte,  int Step, int Substep, char* replymessage)
{
/*
  struct Routerecord
{
	unsigned int Route;
	unsigned int Routestep;
	unsigned int Routesubstep;
	unsigned int RoutePS;
	unsigned int RouteCS;
	unsigned int RouteNS;
	unsigned int RouteCmd;
	unsigned int RouteCmdparam1;
	unsigned int RouteCmdparam2;
	unsigned int RouteCmdparam3;
	unsigned int RouteCmdparam4;
	unsigned char Rtecommand[COMMMANDCHARLENGTH];
};

struct Routerecord RouteData[Routecount][Routelinesmax][Routemaxsubsteps];

 */
char outputrec[OUTPUTRECSIZE];

	int isc,ic,sc,ssc,sssc,mcc;

	mcc = 0;

	if (Rte == 0)  // display all records
	{

	for (sc = 1;sc < Routecount;sc++)
			{
			for (ssc = 1;ssc < Routelinesmax;ssc++)
			  {
				for (sssc = 1;sssc < Routemaxsubsteps;sssc++)
				{
					if (RouteData[sc][ssc][sssc].RouteCS > 0)
					{
						bzero(outputrec,OUTPUTRECSIZE);

						sprintf(outputrec,"\n:-%03d-%02d-%02d-%03d-%03d-%03d-%02d-%02d-%02d-%02d-%02d-%s", sc, ssc,sssc,RouteData[sc][ssc][sssc].RoutePS,\
				    		RouteData[sc][ssc][sssc].RouteCS,RouteData[sc][ssc][sssc].RouteNS,RouteData[sc][ssc][sssc].RouteCmd,\
							RouteData[sc][ssc][sssc].RouteCmdparam1,RouteData[sc][ssc][sssc].RouteCmdparam2,RouteData[sc][ssc][sssc].RouteCmdparam3,\
							RouteData[sc][ssc][sssc].RouteCmdparam4,RouteData[sc][ssc][sssc].Rtecommand);

						isc =  std::strlen(outputrec);

						for (ic = 0;ic <isc;ic++)
						{
							*(replymessage + mcc) = outputrec[ic];
							mcc++;
						}
					 }
				}
			 }
		}
	}

	if ((Rte != 0) && (Step != 0) && (Substep != 0))  // display selected record
	{
		sc = Rte;
		ssc = Step;
		sssc = Substep;

		bzero(outputrec,OUTPUTRECSIZE);

		sprintf(outputrec,"\n:-%03d-%02d-%02d-%03d-%03d-%03d-%02d-%02d-%02d-%02d-%02d-%s", sc, ssc,sssc,RouteData[sc][ssc][sssc].RoutePS,\
				RouteData[sc][ssc][sssc].RouteCS,RouteData[sc][ssc][sssc].RouteNS,RouteData[sc][ssc][sssc].RouteCmd,\
				RouteData[sc][ssc][sssc].RouteCmdparam1,RouteData[sc][ssc][sssc].RouteCmdparam2,RouteData[sc][ssc][sssc].RouteCmdparam3,\
				RouteData[sc][ssc][sssc].RouteCmdparam4,RouteData[sc][ssc][sssc].Rtecommand);

				isc =  std::strlen(outputrec);

				for (ic = 0;ic <isc;ic++)
				{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
				}
	}

	if ((Rte != 0) && (Step == 0)) // display selected route records provided they have Current Section data on them
		{
			sc = Rte;

			for (ssc = 1;ssc < Routelinesmax;ssc++)
			{
				for (sssc = 1;sssc < Routemaxsubsteps;sssc++)
					{
					bzero(outputrec,OUTPUTRECSIZE);

					  if ((sssc > 1) && (RouteData[sc][ssc][sssc].RouteCS > 0))
					  {
						if (RouteData[sc][ssc][sssc].RouteCS > 0)
						{
							sprintf(outputrec,"\n:-%03d-%02d-%02d-%03d-%03d-%03d-%02d-%02d-%02d-%02d-%02d-%s", sc, ssc,sssc,RouteData[sc][ssc][sssc].RoutePS,\
									RouteData[sc][ssc][sssc].RouteCS,RouteData[sc][ssc][sssc].RouteNS,RouteData[sc][ssc][sssc].RouteCmd,\
									RouteData[sc][ssc][sssc].RouteCmdparam1,RouteData[sc][ssc][sssc].RouteCmdparam2,RouteData[sc][ssc][sssc].RouteCmdparam3,\
									RouteData[sc][ssc][sssc].RouteCmdparam4,RouteData[sc][ssc][sssc].Rtecommand);

							isc =  std::strlen(outputrec);

							for (ic = 0;ic <isc;ic++)
							{
								*(replymessage + mcc) = outputrec[ic];
								mcc++;
							}
						}
					  }
					  else
					  {
						  if (sssc == 1)
						  {
							  if (RouteData[sc][ssc][sssc].RouteCS > 0)
							  {
						    	  sprintf(outputrec,"\n:-%03d-%02d-%02d-%03d-%03d-%03d-%02d-%02d-%02d-%02d-%02d-%s", sc, ssc,sssc,RouteData[sc][ssc][sssc].RoutePS,\
						  			RouteData[sc][ssc][sssc].RouteCS,RouteData[sc][ssc][sssc].RouteNS,RouteData[sc][ssc][sssc].RouteCmd,\
						  			RouteData[sc][ssc][sssc].RouteCmdparam1,RouteData[sc][ssc][sssc].RouteCmdparam2,RouteData[sc][ssc][sssc].RouteCmdparam3,\
						  			RouteData[sc][ssc][sssc].RouteCmdparam4,RouteData[sc][ssc][sssc].Rtecommand);

						    		isc =  std::strlen(outputrec);

						    		for (ic = 0;ic <isc;ic++)
						    		{
						    			*(replymessage + mcc) = outputrec[ic];
						    			  		mcc++;
						    		}
							  }
						  }

					  }

					}
			}
		}

	return mcc++;

}

int LNweblocorouteprocess::GetLocoplanspeed(int Locoindx)
{
	return Locoinformation[Locoindx].Planspeed;
}

int LNweblocorouteprocess::GetDCCLocospeed(int Locoindx)
{
	return Locoinformation[Locoindx].ActDCCvalue;

}

int LNweblocorouteprocess::GetLocoEnabled(int Locoindx)
{
	return Locoinformation[Locoindx].Enabled;
}


void LNweblocorouteprocess::RoutedatarecordsInit(int Rte)
{
	int sc;

	int scc;

	int sccc;

	int cc;

	/*
	 struct Routerecord
	{
	unsigned int Route;
	unsigned int Routestep;
	unsigned int Routesubstep;
	unsigned int RoutePS;
	unsigned int RouteCS;
	unsigned int RouteNS;
	unsigned int RouteCmd;
	unsigned int RouteCmdparam1;
	unsigned int RouteCmdparam2;
	unsigned int RouteCmdparam3;
	unsigned int RouteCmdparam4;
	unsigned char Rtecommand[COMMMANDCHARLENGTH];
	};

	struct Routerecord RouteData[Routecount][Routelinesmax][Routemaxsubsteps];

	 */

	if (Rte == 0)
	{
		for (sc = 0;sc < Routecount;sc++)
		{
			for (scc = 0;scc < Routelinesmax;scc++)
			{
				for (sccc = 0;sccc < Routemaxsubsteps;sccc++)
				{
					RouteData[sc][scc][sccc].Route = 0;
					RouteData[sc][scc][sccc].Routestep = 0;
					RouteData[sc][scc][sccc].Routesubstep = 0;
					RouteData[sc][scc][sccc].RoutePS = 0;
					RouteData[sc][scc][sccc].RouteCS = 0;
					RouteData[sc][scc][sccc].RouteNS = 0;
					RouteData[sc][scc][sccc].RouteCmd = 0;
					RouteData[sc][scc][sccc].RouteCmdparam1 = 0;
					RouteData[sc][scc][sccc].RouteCmdparam2 = 0;
					RouteData[sc][scc][sccc].RouteCmdparam3 = 0;
					RouteData[sc][scc][sccc].RouteCmdparam4 = 0;

					for (cc = 0; cc < COMMMANDCHARLENGTH; cc++)
					{
						RouteData[sc][scc][sccc].Rtecommand[cc] = '*';
					}
				}

			}

		}
	//	std::cout<<"\n404 ALL Route data records cleared\n\n";
	}

	if (Rte >  0)
		{
			sc = Rte;

				for (scc = 0;scc < Routelinesmax;scc++)
				{
					for (sccc = 0;sccc < Routemaxsubsteps;sccc++)
					{
						RouteData[sc][scc][sccc].Route = 0;
						RouteData[sc][scc][sccc].Routestep = 0;
						RouteData[sc][scc][sccc].Routesubstep = 0;
						RouteData[sc][scc][sccc].RoutePS = 0;
						RouteData[sc][scc][sccc].RouteCS = 0;
						RouteData[sc][scc][sccc].RouteNS = 0;
						RouteData[sc][scc][sccc].RouteCmd = 0;
						RouteData[sc][scc][sccc].RouteCmdparam1 = 0;
						RouteData[sc][scc][sccc].RouteCmdparam2 = 0;
						RouteData[sc][scc][sccc].RouteCmdparam3 = 0;
						RouteData[sc][scc][sccc].RouteCmdparam4 = 0;

						for (cc = 0; cc < COMMMANDCHARLENGTH; cc++)
						{
							RouteData[sc][scc][sccc].Rtecommand[cc] = ' ';
						}
					}
				}
		}

	//std::cout<<"\n435 Route data records cleared\n\n";
}

int LNweblocorouteprocess::GetSimulatornumber(int Locoindx)
{
	return Locoinformation[Locoindx].Simulatornumber;
}

void LNweblocorouteprocess::SetEnableLocoflag(int Locoindx, int flag)
{
	Locoinformation[Locoindx].Enabled = flag;

}

void LNweblocorouteprocess::SaveLocojourneydata(int Locoindx, int Rtenum,int Stepnum, \
			int Substepnum,int PS, int CS,int NS, int speed, \
			int direction, int Signalhold,int Enabled, \
			int Planspeed,int Cmdinprogress,int Journey,\
			int Journeystatus, int Stopdevice, int Traincode, int Slotcode, int Simcode, int Tnumber, int Reverselocodirection, int Sectionlogicflag)

/*
	Locodatarecord
		{
		unsigned int Locoindex;
		unsigned int LocoDCCcode;
		unsigned int Route;
		unsigned int Rtestep;
		unsigned int Rtesubstep;
		unsigned int Enabled;
		unsigned int Journey;
		unsigned int Journeystatus;
		unsigned int Signalhold;
		unsigned int PS;
		unsigned int CS;
		unsigned int NS;
		unsigned int Direction;
		unsigned int Speed;
		unsigned int Planspeed;
		unsigned int Cmdinprogress;
		unsigned int Loopcountertarget;
		unsigned int Loopcounteractual;
		unsigned int Loopstopflag;
		unsigned int Locomissingflag;
		unsigned int Stopdetector;
		unsigned int LastknownLS;
		unsigned int Traincode;
		unsigned int Rteflagsdvce[Routeflagscnt];
		unsigned int Rteflagsdvcecntplan[Routeflagscnt];
		unsigned char description[Locodescriptionlen];
	};

struct Locodatarecord Locoinformation[Lococountmax];
	*/
{
//	printf("\n568-2 Saving Loco %02d Journey %02d Rte %02d",Locoindx,Journey,Rtenum);

	Locoinformation[Locoindx].Route = Rtenum;
	Locoinformation[Locoindx].Rtestep = Stepnum;
	Locoinformation[Locoindx].Rtesubstep = Substepnum;
	Locoinformation[Locoindx].PS = PS;
	Locoinformation[Locoindx].CS = CS;
	Locoinformation[Locoindx].NS = NS;
	Locoinformation[Locoindx].Speed = speed;
	Locoinformation[Locoindx].Direction = direction;
	Locoinformation[Locoindx].LocoDCCcode = Traincode;
	Locoinformation[Locoindx].Slotnumber = Slotcode;
	Locoinformation[Locoindx].Locodirectionreversed = Reverselocodirection;
	Locoinformation[Locoindx].Signalhold = Signalhold;
	Locoinformation[Locoindx].Enabled = Enabled;
	Locoinformation[Locoindx].Planspeed = Planspeed;
	Locoinformation[Locoindx].Cmdinprogress = Cmdinprogress;
	Locoinformation[Locoindx].Journey = Journey;
	Locoinformation[Locoindx].Journeystatus = Journeystatus;
	Locoinformation[Locoindx].Stopdetector = Stopdevice;
	Locoinformation[Locoindx].Traincode = Traincode;
	Locoinformation[Locoindx].Simulatornumber = Simcode;
	Locoinformation[Locoindx].TrainNumber = Tnumber;
	Locoinformation[Locoindx].Sectionlogicflag = Sectionlogicflag;

	// check for direction reversal

	if (Reverselocodirection == 1)  // if enabled, reverse the stored direction
	{
		if (Locoinformation[Locoindx].Direction == 0)
		{
			Locoinformation[Locoindx].Direction = 1;
		}
		else
		{
			Locoinformation[Locoindx].Direction = 0;
		}
	}

	char lmessage[LRMESSAGELENGTH];

	int cc, sl;

	bzero(lmessage,LRMESSAGELENGTH);
	bzero(LRprocessmonitormessage,LRMESSAGELENGTH);

	sprintf(lmessage,"Loco %02d Route %02d downloaded",Locoindx,Rtenum);

	sl = 0;

	for (cc = 0;cc<LRMESSAGELENGTH;cc++)
	{
		if (lmessage[cc] != 0x0)
		{
			sl++;
		}
	}

	std::cout<<"\n\n\n 607 Loco index:"<<Locoindx<<" Journey "<<Journey<<" Route "<<Rtenum<<" downloaded - SL = "<<sl<<" TrainNumber:"<<Locoinformation[Locoindx].TrainNumber;

	for (cc = 0;cc < sl;cc++)
	{
		LRprocessmonitormessage[cc] 					= lmessage[cc];
		Locoinformation[Locoindx].LRlocoprocmessage[cc] = lmessage[cc];
	}
	Locoinformation[Locoindx].LRlocoprocmessage[cc]		= ' ';
	Locoinformation[Locoindx].LRlocoprocmessage[cc+1]	= '.';
}


void LNweblocorouteprocess::LocodatarecordsInit(int loco)
{
	int sc;

	int scc;

	int cc;

	char lmessage[LRMESSAGELENGTH];

	/*
		unsigned int Locoindex;
		unsigned int LocoDCCcode;
		unsigned int Route;
		unsigned int Rtestep;
		unsigned int Rtesubstep;
		unsigned int Enabled;
		unsigned int Journey;
		unsigned int Journeystatus;
		unsigned int Signalhold;
		unsigned int PS;
		unsigned int CS;
		unsigned int NS;
		unsigned int Direction;
		unsigned int Speed;
		unsigned int Planspeed;
		unsigned int Cmdinprogress;
		unsigned int Loopcountertarget;
		unsigned int Loopcounteractual;
		unsigned int Loopstopflag;
		unsigned int Locomissingflag;
		unsigned int Stopdetector;
		unsigned int LastknownLS;
		unsigned int Traincode;
		unsigned int Rteflagsdvce[Routeflagscnt];
		unsigned int Rteflagsdvcecntplan[Routeflagscnt];
		unsigned char description[Locodescriptionlen];

	*/

	if (loco == 0)
	{
		for (sc = 0;sc < Lococountmax;sc++)
		{
			Locoinformation[sc].Locoindex = 0;
			Locoinformation[sc].LocoDCCcode = 0;
			Locoinformation[sc].Route = 0;
			Locoinformation[sc].Rtestep = 0;
			Locoinformation[sc].Rtesubstep = 0;
			Locoinformation[sc].Enabled = 0;
			Locoinformation[sc].Journey = 0;
			Locoinformation[sc].Journeystatus = 0;
			Locoinformation[sc].Schedulerunstatus = 0;
			Locoinformation[sc].Signalhold = 0;
			Locoinformation[sc].Systemhold = 0;

			Locoinformation[sc].PS = 0;
			Locoinformation[sc].CS = 0;
			Locoinformation[sc].NS = 0;

			Locoinformation[sc].PS_previous = 0;
			Locoinformation[sc].CS_previous = 0;
			Locoinformation[sc].NS_previous = 0;

			Locoinformation[sc].Slotnumber = 0;
			Locoinformation[sc].Locodirectionreversed = 0;
			Locoinformation[sc].Direction = 0;
			Locoinformation[sc].STPCflag = 0;
			Locoinformation[sc].STOPflag = 0;
			Locoinformation[sc].STPDflag = 0;
			Locoinformation[sc].STPBflag = 0;
			Locoinformation[sc].STPMflag = 0;
			Locoinformation[sc].STCCflag = 0;
			Locoinformation[sc].STPXflag = 0;
			Locoinformation[sc].STDCflag = 0;
			Locoinformation[sc].NextsectionSTOPflag = 0;
			Locoinformation[sc].Cmdparam1 = 0;
			Locoinformation[sc].Cmdparam2 = 0;
			Locoinformation[sc].Cmdparam3 = 0;
			Locoinformation[sc].Cmdparam4 = 0;
			Locoinformation[sc].Speed = 0;
			Locoinformation[sc].Planspeed = 0;
			Locoinformation[sc].ActDCCvalue = 0;
			Locoinformation[sc].Cmdinprogress = 0;
			Locoinformation[sc].Loopcountertarget = 0;
			Locoinformation[sc].Loopcounteractual = 0;
			Locoinformation[sc].Loopstopflag = 0;
			Locoinformation[sc].Locomissingflag = 0;
			Locoinformation[sc].Stopdetector = 0;
			Locoinformation[sc].LastknownLS = 0;
			Locoinformation[sc].Traincode = 0;
			Locoinformation[sc].ConnectedTraincode = 0;
			Locoinformation[sc].TrainLSswitchoverflag = 0;
			Locoinformation[sc].TrainswitchoverfromLS = 0;
			Locoinformation[sc].TrainswitchovertoLS = 0;
			Locoinformation[sc].TrainNumber			= 0;
			Locoinformation[sc].Simulatornumber 	= 0;
			Locoinformation[sc].Sectionlogicflag 	= 0;

			Locoinformation[sc].PSTrainsection 		= 0;
			Locoinformation[sc].CSTrainsection 		= 0;
			Locoinformation[sc].NSTrainsection 		= 0;

			Locoinformation[sc].PSTrainsection_previous = 0;
			Locoinformation[sc].CSTrainsection_previous = 0;
			Locoinformation[sc].NSTrainsection_previous = 0;

			Locoinformation[sc].NSTrainsectionoccupancystatus = 0;

			Locoinformation[sc].Trainsectioncrossingtype = 0;


			for (scc = 0;scc < MAXPSinTS;scc++)
			{
				Locoinformation[sc].PSsectionPS[scc] 		= 0;
				Locoinformation[sc].PSsectionPSstatus[scc]	= 0;
				Locoinformation[sc].CSsectionPS[scc] 		= 0;
				Locoinformation[sc].CSsectionPSstatus[scc]	= 0;
				Locoinformation[sc].NSsectionPS[scc] 		= 0;
				Locoinformation[sc].NSsectionPSstatus[scc]	= 0;
			}


			for (scc = 0;scc < Routeflagscnt;scc++)
			{
				Locoinformation[sc].Rteflagsdvce[scc] = 0;
			}

			for (scc = 0;scc < Routeflagscnt;scc++)
			{
				Locoinformation[sc].Rteflagsdvcecntplan[scc] = 0;
			}

			for (cc = 0;cc < Locodescriptionlen;cc++)
			{
				Locoinformation[sc].description[cc] = '*';
			}

			Locoinformation[sc].filler1 = 0;

			Locoinformation[sc].description[cc-1] = '\n';

			bzero(lmessage,LRMESSAGELENGTH);

			sprintf(lmessage,"Loco %02d not initialized .",sc);

			for (cc = 0;cc < LRMESSAGELENGTH;cc++)
			{
				Locoinformation[sc].LRlocoprocmessage[cc] = lmessage[cc];
			}

			for (scc = 0; scc<Routelinesmax; scc++)
			{
				Locoinformation[sc].Routesectionhistory[scc] = 0;
			}
			//std::cout<<"\nLR procmsg->"<<lmessage;
		}
	}

	if ((loco > 0) && (loco < Lococountmax))  // clear specified loco
		{
			sc = loco;

			std::cout<<"\n\nInitialize details for loco-->"<<sc<<" \n";

			Locoinformation[sc].Locoindex = 0;
			Locoinformation[sc].LocoDCCcode = 0;
			Locoinformation[sc].Route = 0;
			Locoinformation[sc].Rtestep = 0;
			Locoinformation[sc].Rtesubstep = 0;
			Locoinformation[sc].Enabled = 0;
			Locoinformation[sc].Journey = 0;
			Locoinformation[sc].Journeystatus = 0;
			Locoinformation[sc].Schedulerunstatus = 0;
			Locoinformation[sc].Signalhold = 0;
			Locoinformation[sc].Systemhold = 0;
			Locoinformation[sc].PS = 0;
			Locoinformation[sc].CS = 0;
			Locoinformation[sc].NS = 0;
			Locoinformation[sc].Slotnumber = 0;
			Locoinformation[sc].Locodirectionreversed = 0;
			Locoinformation[sc].Direction = 0;
			Locoinformation[sc].STPCflag = 0;
			Locoinformation[sc].STOPflag = 0;
			Locoinformation[sc].STPDflag = 0;
			Locoinformation[sc].STPMflag = 0;
			Locoinformation[sc].STPBflag = 0;
			Locoinformation[sc].STPXflag = 0;
			Locoinformation[sc].NextsectionSTOPflag = 0;
			Locoinformation[sc].Cmdparam1 = 0;
			Locoinformation[sc].Cmdparam2 = 0;
			Locoinformation[sc].Cmdparam3 = 0;
			Locoinformation[sc].Cmdparam4 = 0;
			Locoinformation[sc].Speed = 0;
			Locoinformation[sc].Planspeed = 0;
			Locoinformation[sc].ActDCCvalue = 0;
			Locoinformation[sc].Cmdinprogress = 0;
			Locoinformation[sc].Loopcountertarget = 0;
			Locoinformation[sc].Loopcounteractual = 0;
			Locoinformation[sc].Loopstopflag = 0;
			Locoinformation[sc].Locomissingflag = 0;
			Locoinformation[sc].Stopdetector = 0;
			Locoinformation[sc].LastknownLS = 0;
			Locoinformation[sc].Traincode = 0;
			Locoinformation[sc].TrainNumber = 0;
			Locoinformation[sc].Simulatornumber = 0;
			Locoinformation[sc].PSTrainsection	= 0;
			Locoinformation[sc].CSTrainsection  = 0;
			Locoinformation[sc].PSTrainsection  = 0;

			for (scc = 0;scc < Routeflagscnt;scc++)
				{
							Locoinformation[sc].Rteflagsdvce[scc] = 0;
				}

			for (scc = 0;scc < Routeflagscnt;scc++)
				{
							Locoinformation[sc].Rteflagsdvcecntplan[scc] = 0;
				}

			for (cc = 0;cc < Locodescriptionlen;cc++)
				{
							Locoinformation[sc].description[cc] = ' ';
				}

			for (cc = 0;cc < LRMESSAGELENGTH;cc++)
				{
					Locoinformation[sc].LRlocoprocmessage[cc] = lmessage[cc];
				}

			bzero(lmessage,LRMESSAGELENGTH);

			sprintf(lmessage,"Loco %02d initialized .",sc);
	}

	// LRprocessmonitormessage[LRMESSAGELENGTH];

	// char lmessage[LRMESSAGELENGTH];

	snprintf(lmessage,LRMESSAGELENGTH,"Loco Route process overall monitoring");

	for (cc = 0;cc < LRMESSAGELENGTH;cc++)
	{
		LRprocessmonitormessage[cc] = lmessage[cc];
	}

	//std::cout<<"\nLoco data records cleared\n\n";
}

int  LNweblocorouteprocess::GetLoopcountertarget(int Locoindx)
{
	return Locoinformation[Locoindx].Loopcountertarget;
}

int  LNweblocorouteprocess::GetLoopcounteractual(int Locoindx)
{
	return Locoinformation[Locoindx].Loopcounteractual;
}

void LNweblocorouteprocess::SetLoopcountertarget(int Locoindx, int value)
{
	Locoinformation[Locoindx].Loopcountertarget = value;
}

void LNweblocorouteprocess::SetLoopcounteractual(int Locoindx, int value)
{
	Locoinformation[Locoindx].Loopcounteractual = value;
}

void LNweblocorouteprocess::SetLoopstopflag(int Locoindx)
{
	Locoinformation[Locoindx].Loopstopflag = 1;
}
	
void LNweblocorouteprocess::ResetLoopstopflag(int Locoindx)
{
	Locoinformation[Locoindx].Loopstopflag = 0;
}

int LNweblocorouteprocess::GetJourneystatusvalue(int Loco)
{
	return Locoinformation[Loco].Journeystatus;
}

int LNweblocorouteprocess::GetJourneystatus(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;	
	
	mcc = 0;

	for (scc = 1;scc < Lococountmax;scc++)
		{
			bzero(outputrec,OUTPUTRECSIZE);

			sprintf(outputrec,":%02d-%03d-%01d-%04d-",scc,Locoinformation[scc].Journey, Locoinformation[scc].Journeystatus, Locoinformation[scc].TrainNumber);

			isc =  std::strlen(outputrec);			

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
			}
		}

	//printf("\nJourney status details %s",replymessage);
	
	return mcc;
	
}

void LNweblocorouteprocess::GetLRprocessmonmsg(char* replymessage)
{
	int ic,mcc;

	mcc = 0;

	for (ic = 0;ic <LRMESSAGELENGTH;ic++)
	{
		*(replymessage + mcc) =  LRprocessmonitormessage[ic];
		mcc++;
	}
}

void LNweblocorouteprocess::GetLRlocoprocessmonmsg(int loco, char* replymessage)
{
	int ic,isc,mcc,lc;

		mcc = 0;

		isc = LRMESSAGELENGTH;

		if (loco > 0)
		{
			*(replymessage + mcc) = '+';
			mcc++;

			for (ic = 0;ic <isc;ic++)
			{
			*(replymessage + mcc) =  Locoinformation[loco].LRlocoprocmessage[ic];
				mcc++;
			}
			*(replymessage + mcc) = '+';
			mcc++;
		}
		else
		{
			*(replymessage + mcc) = '+';
			mcc++;

			for (lc = 1;lc < 7;lc++)
			{
				for (ic = 0;ic <isc;ic++)
				{
					if (Locoinformation[lc].LRlocoprocmessage[ic] != '.')
					{
						*(replymessage + mcc) =  Locoinformation[lc].LRlocoprocmessage[ic];
							mcc++;
					}
					else
					{
						ic = isc;
						*(replymessage + mcc) = '+';
					}
				}
				//std::cout<<"\nlc:"<<lc<<" " << Locoinformation[lc].LRlocoprocmessage <<" reply message->"<<replymessage;
				*(replymessage + mcc) = '+';
				mcc++;
			}
		//	std::cout<<"\nComplete replymessage->"<<replymessage;
		}
}

int LNweblocorouteprocess::Lococommandexecute(int loco, int cmd, char* message)
{
	int result = 0;

	std::cout<<"\nLoco "<< loco <<" LRE command "<< cmd <<" executed ";

	result = LNsectionCrossingprocessing(loco,cmd);

	if (result == 95)
	{
		snprintf(message,SHORTMESSAGELENGTH,"%s","Loco not found in CS");
	}

	if (result == 0)
	{
		snprintf(message,SHORTMESSAGELENGTH,"%s","Found OK in CS");
	}
	else
	{
		snprintf(message,SHORTMESSAGELENGTH,"%s  %i","RC back = %02d",result);
	}



	return result;

}

int LNweblocorouteprocess::LNlocosectionrefresh()
{
	int rc = 0;

	int AC;

	int lc = 0;

	for (lc = 1;lc < Lococountmax;lc++)
	{
		//std::cout<<"\n982 Checking Loco section refresh for loco:"<<lc<<" Enabled flag:"<<Locoinformation[lc].Enabled;

		if (Locoinformation[lc].Enabled > 0)
		{
			//std::cout<<"\n987 LNweblocorouteprocess - Loco section refresh for loco:"<<lc<<" Enabled flag:"<<Locoinformation[lc].Enabled;
			AC = LNsectionCrossingprocessing(lc,5);			// execute section crossover check for each loco that is enabled
			LNSTPDprocessing();								// process any STOP command flags
		}

	}

	return rc;
}

void LNweblocorouteprocess::LNPRoverallmessage(char* message)
{


}



void LNweblocorouteprocess::LNprmessage(int loco, char* message)
{

	//std::cout<<"\nLoco "<<Locoindx<<" Route "<<Rtenum<<" downloaded - SL = "<<sl;

	int cc,sl;

	for (cc = 0;cc < LRMESSAGELENGTH;cc++)
	{
		if (message[cc] != '.')
		{
			sl++;
		}
		else
		{
			sl++;
			cc = LRMESSAGELENGTH;
			//std::cout<<"\nPRmsg length ="<<sl;
		}
	}


	for (cc = 0;cc < LRMESSAGELENGTH;cc++)
	{
		if (cc< sl)
		{
			Locoinformation[loco].LRlocoprocmessage[cc] = message[cc];
		}
		else
		{
			Locoinformation[loco].LRlocoprocmessage[cc] = '%';
		}
	}
}

void LNweblocorouteprocess::LNsectionCrossingprocessingNG_Datarefresh(int loco)
{
		PS_LS_NG 		= 0;
		PS_LS_type_NG 	= 0;
		CS_LS_NG		= 0;
		CS_LS_type_NG 	= 0;
		NS_LS_NG 		= 0;

		NSS_LS_NG		= 0;

		NS_LS_type_NG 	= 0;

		PS_ps1_val_NG	= 0;
		PS_ps1_data_NG	= 0;

		PS_ps2_val_NG	= 0;
		PS_ps2_data_NG	= 0;

		CS_ps1_val_NG	= 0;
		CS_ps1_data_NG	= 0;

		CS_ps2_val_NG	= 0;
		CS_ps2_data_NG	= 0;

		NS_ps1_val_NG	= 0;
		NS_ps1_data_NG	= 0;

		NS_ps2_val_NG	= 0;
		NS_ps2_data_NG	= 0;

		PS_Tcode_NG		= 0;
		CS_Tcode_NG		= 0;
		NS_Tcode_NG		= 0;

		bzero(PSmessage_NG,SHORTMESSAGELENGTH);

		// step2 - initialize LS from Loco records

		PS_LS_NG 		= Locoinformation[loco].PS;
		CS_LS_NG 		= Locoinformation[loco].CS;
		NS_LS_NG		= Locoinformation[loco].NS;

		PS_LS_type_NG	= LSLD->GetPtype(PS_LS_NG);
		CS_LS_type_NG	= LSLD->GetPtype(CS_LS_NG);
		NS_LS_type_NG	= LSLD->GetPtype(NS_LS_NG);

		if (PS_LS_NG > 0)
		{
			PS_Tcode_NG = 	LSLD->GetLogicalSectionTcode(PS_LS_NG);
		}
		else
		{
			PS_Tcode_NG = 0;
		}

		if (CS_LS_NG > 0)
		{
			CS_Tcode_NG = 	LSLD->GetLogicalSectionTcode(CS_LS_NG);
		}
		else
		{
			CS_Tcode_NG = 0;
		}

		if (NS_LS_NG > 0)
		{
			NS_Tcode_NG = 	LSLD->GetLogicalSectionTcode(NS_LS_NG);
		}
		else
		{
			NS_Tcode_NG = 0;
		}

		PS_ps1_val_NG 	= LSLD->GetP1code(PS_LS_NG);

		if (PS_ps1_val_NG > 0)
		{
			PS_ps1_data_NG	= PSectiondatareturn(PS_ps1_val_NG,&PSmessage_NG[0]);
		}

		PS_ps2_val_NG 	= LSLD->GetP2code(PS_LS_NG);

		if (PS_ps2_val_NG > 0)
		{
			PS_ps2_data_NG	= PSectiondatareturn(PS_ps2_val_NG,&PSmessage_NG[0]);
		}

		CS_ps1_val_NG 	= LSLD->GetP1code(CS_LS_NG);

		if (CS_ps1_val_NG > 0)
		{
			CS_ps1_data_NG = PSectiondatareturn(CS_ps1_val_NG,&PSmessage_NG[0]);
		}

		CS_ps2_val_NG 	= LSLD->GetP2code(CS_LS_NG);

		if (CS_ps2_val_NG > 0)
		{
			CS_ps2_data_NG = PSectiondatareturn(CS_ps2_val_NG,&PSmessage_NG[0]);
		}

		NS_ps1_val_NG 	= LSLD->GetP1code(NS_LS_NG);

		if (NS_ps1_val_NG > 0)
		{
			NS_ps1_data_NG = PSectiondatareturn(NS_ps1_val_NG,&PSmessage_NG[0]);
		}

		NS_ps2_val_NG 	= LSLD->GetP2code(NS_LS_NG);

		if (NS_ps2_val_NG > 0)
		{
			NS_ps2_data_NG = PSectiondatareturn(NS_ps2_val_NG,&PSmessage_NG[0]);
		}

		Crossing_inprogress_flag_NG = 0;

		Tcode_NG = Locoinformation[loco].Traincode;
}

int LNweblocorouteprocess::LNsectionCrossingprocessing(int loco, int action)
{
	/* New modular LNsectioncrossingprocessing function - introduced to improve 'lost loco' processing

		// action values  -> 0 = initialize run
		// 1 = Initialize a new loco running on a route
		// 2 = loco stop command
		// 3 = Reset loco details
		// 4 = Clear loco stop command
		// 5 = process existing route that is executing

		// Initialize run

		---------------------------------------------------------------------------------

		Extract and store PS, CS and NS
		Routestep = 1;
		validate loco is present in current section
		Process all route steps Routesubstep;
		Process loco command


	struct Locodatarecord
			{
			unsigned int Locoindex;
			unsigned int LocoDCCcode;
			unsigned int Route;
			unsigned int Rtestep;
			unsigned int Rtesubstep;
			unsigned int Enabled;
			unsigned int Journey;
			unsigned int Journeystatus;
			unsigned int Signalhold;
			unsigned int Systemhold;
			unsigned int Slotnumber;
			unsigned int Locodirectionreversed;
			unsigned int PS;
			unsigned int CS;
			unsigned int NS;
			unsigned int Direction;
			unsigned int Speed;
			unsigned int Planspeed;
			unsigned int ActDCCvalue;
			unsigned int Cmdinprogress;
			unsigned int Loopcountertarget;
			unsigned int Loopcounteractual;
			unsigned int Loopstopflag;
			unsigned int Locomissingflag;
			unsigned int Stopdetector;
			unsigned int LastknownLS;
			unsigned int Traincode;
			unsigned int Simulatornumber;
			unsigned int Rteflagsdvce[Routeflagscnt];
			unsigned int Rteflagsdvcecntplan[Routeflagscnt];
			unsigned char description[Locodescriptionlen];
		};

	struct Locodatarecord Locoinformation[Lococountmax];

	struct Routerecord
	{
		unsigned int Route;
		unsigned int Routestep;
		unsigned int Routesubstep;
		unsigned int RoutePS;
		unsigned int RouteCS;
		unsigned int RouteNS;
		unsigned int RouteCmd;
		unsigned int RouteCmdparam1;
		unsigned int RouteCmdparam2;
		unsigned int RouteCmdparam3;
		unsigned int RouteCmdparam4;
		unsigned char Rtecommand[COMMMANDCHARLENGTH];
	};


	   Route device commands

	   0  =  Null - no action
	   1  = MOVE - move to next section
	   2  = STPD(x) - stop at device x
	   3  = SLS(y) - set loco speed to be y
	   4  = STPC   - stop if previous section is clear
	   5  = STOP
	   6  = RLD(z) - reset logical device z
	   7  = RLA    - reset all logical devices
	   8  = ELT    - Enable logical device trigger
	   9  = RLT    - reset  logical device trigger
	  10  = DLD    - disable device scanner
	  11  = ETCS   - enable train car scanning
	  12  = DTCS   - disable train car scanning
	  13  = SDC(a,b) - set device a counter to b
	  14  = RDC(a)   - reset device a counter to zero
	  15  = STPB     - stop at device or previous section clear
	  16  = STPM     - stop at device or move to next section
	  17  = RRSC(x,y)   - reset set route step counter to step x, substep y
	  18  = ILCC   		- increment the actual loop counter
	  19  = BLCT(x,y)   - branch if loop counter actual = target
	  20  = SLCT   - set loop counter target value
	  21  = BLFS(x,y)   - branch if loop stop flag is set
	  22  = STCC(d,v)   - stop if count on device d = value v
	  24  = EUD(a,b,c) - Enable uncoupler a with device b on target c

	    struct Routerecord RouteData[Routecount][Routelinesmax][Routemaxsubsteps];

		-------------------------------------------------------------------------------- */


	int RC = 0;

	switch(action)
	{

		//************************************************************************************************
		//
		// Action code 1 -> Initiate a new journey
		//
		//************************************************************************************************

		case 1:  // initiate new journey
		{
			RC = LNsectionCrossingprocessingNG_Newjourney(loco);
		}
		break;

		//************************************************************************************************
		//
		// Action code 2 -> the loco stop command
		//
		//************************************************************************************************

		case 2: // Action code 2 -> the loco stop command
		{
			CSpeedcmdprocess(66,Locoinformation[loco].Slotnumber,0,0);	// stop the selected loco
			Locoinformation[loco].Systemhold = 1;					// Mark loco as having a System stop
			sprintf(lprmessage,"Loco %02d Route %02d RUN STOPPED .       ",loco,Rte);
			LNprmessage(loco,lprmessage);
			RC = 2;
		}
		break;

		//************************************************************************************************
		//
		// Action code 3 -> Reset the loco details
		//
		//************************************************************************************************

		case 3: // Reset loco details
		{
			CSpeedcmdprocess(65,Locoinformation[loco].Slotnumber,0,0);	// stop the selected loco
			printf("\nCommand to reset loco \n");
			LocodatarecordsInit(loco);			// reset and initialize selected loco
			RC = 5;
		}
		break;

		//************************************************************************************************
		//
		// Action code 4 -> Clear the loco stop command
		//
		//************************************************************************************************

		case 4:  // Clear the Loco stop command
		{
			Locoinformation[loco].Systemhold = 0;					// Clear loco System stop
			sprintf(lprmessage,"Loco %02d Route %02d RUN RE-STARTED .       ",loco,Rte);
			LNprmessage(loco,lprmessage);
			RC = 4;
		}
		break;

		//************************************************************************************************
		//
		// Action code 5 -> Section crossing processing
		//
		//************************************************************************************************

		case 5:  // Section crossing processing
		{
			RC = LNsectionCrossingprocessingNG_Crossoverprocess(loco);
		}
		break;
	}

	return RC;
}

int LNweblocorouteprocess::LNsectionCrossingprocessingNG_Newjourney(int loco)
{
	/*------------------------------------------------------------------------
	New journey initiation steps

	1. Check to see if journey is already in progress - abort if found to be already executing a journey
	2. Extract the PS,CS,NS details for starting the journey
	3. Fetch the loco Tcode
	4. Write the log message
	5. Test the uncoupling flag for the train
	6. Test the LS switchover flag
	7. Fetch the CS physical details to check if loco is located in CS
	8. Fetch the direction of travel
	9. Check for the loco direction of travel reversed flag
	10. Install Tcode in CS
	11. set loco enabled flag
	12. Set journey status flag = 2
	13. Perform follow-on processing



	--------------------------------------------------------------------------*/
	int RC = 0;

	DisplayLocoInformation(1,loco);

	RC = 1;

	// 1. Check to see if journey is already in progress - abort if found to be already executing a journey

	if ((Locoinformation[loco].Journeystatus != 9) && (Locoinformation[loco].Journeystatus != 0))	// Check for duplicate journey request
	{
		printf("\n\n1448 Duplicate journey request for Loco %i Journey status %i",loco,Locoinformation[loco].Journeystatus);

		RC = 99;
	}

	if (((Locoinformation[loco].Journeystatus == 9) || (Locoinformation[loco].Journeystatus == 0)))	// Initialize a new route
	{

		// 2. Extract the PS,CS,NS details for starting the journey

		Rtestep 		= 1;
		Rtesubstep 		= 1;

		Locoinformation[loco].Rtestep 		= Rtestep;
		Locoinformation[loco].Rtesubstep 	= Rtesubstep;

		Rte = Locoinformation[loco].Route;

		PS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RoutePS;
		CS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteCS;
		NS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteNS;

		Locoinformation[loco].PS = PS_LS_NG;
		Locoinformation[loco].CS = CS_LS_NG;
		Locoinformation[loco].NS = NS_LS_NG;

		LCSPDtable->SetlococurrentLS(loco, CS_LS_NG);				// store current section location on locoAlldata table

		// 2.1 Extract the PSTrainsection,CSTrainsection and NSTrainsection details for starting the journey

		Locoinformation[loco].CSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].CS);
		Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].NS);

		std::cout<<"\n1462 Journey start - setting TrainsectionCS:"<<Locoinformation[loco].CSTrainsection<<" TrainsectionNS:"<<Locoinformation[loco].NSTrainsection;

		if (Locoinformation[loco].CSTrainsection == Locoinformation[loco].NSTrainsection)
		{
			// current and NS are the same so fetch next LS and retrieve the TM again

			Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(RouteData[Rte][Rtestep + 1][Rtesubstep].RouteNS);
		}

		// 3. Fetch the loco Tcode

		Tcode_NG = Locoinformation[loco].Traincode;

		//Tcode = Tcode_NG;  // for backwards compatibility

		char LNweblogroutemessage[LOGRECORDLENGTH];

		bzero(LNweblogroutemessage,LOGRECORDLENGTH);

		char* LNweblogroutemessageptr = &LNweblogroutemessage[0];


		// 4. Write the log message

		sprintf(LNweblogroutemessage,"C-Starting journey %i for loco %i on route %i",Locoinformation[loco].Journey,Locoinformation[loco].LocoDCCcode,Locoinformation[loco].Route);

		InsertGUItextlineMain(LNweblogroutemessageptr);

		LNloggingptr->Writelogmessage(1,LNweblogroutemessage);

		std::cout<<"\n\nLNweblocorouteprocess ===> 1495 Starting new Route:"<<Locoinformation[loco].Route<<" for Loco "<<loco<<" DCC code "<<Locoinformation[loco].LocoDCCcode<<"\n";

		// 5. Test the uncoupling flag for the train

		if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1 > 0)  // test Train uncoupling flag to see if it is set.
		{
			Locoinformation[loco].STDCflag = 1;		// flag is set to set indicator on the loco record
			std::cout<<"\n1427 New route - Train coupling flag is set";
		}

		// 6. Test the LS switchover flag

		if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2 > 0)  // test TrainLSswitchoverflag to see if it is set.
		{
			Locoinformation[loco].TrainLSswitchoverflag = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;
			Locoinformation[loco].TrainswitchoverfromLS = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam3;
			Locoinformation[loco].TrainswitchovertoLS 	= RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam4;
		}

		// 7. Fetch the CS physical details to check if loco is located in CS

		P1y = LSLD->GetP1code(CS_LS_NG);
		P2y = LSLD->GetP2code(CS_LS_NG);
		typey = LSLD->GetPtype(CS_LS_NG);

		bzero(PSmessage_NG,SHORTMESSAGELENGTH);

		Py1oc = 0;
		Py2oc = 0;

		Py1oc = PSectiondatareturn(P1y,&PSmessage_NG[0]);

		if (P2y > 0)
		{
			Py2oc = PSectiondatareturn(P2y,&PSmessage_NG[0]);
		}

		if (((typey == 1) || (typey == 2)) && (Py1oc == 0))   // check to see if loco is in start Current Section
		{
				// loco is missing so set loco missing return code

			std::cout<<"\n\n1561 *Route initiation ******* Loco not found in CS section ************* \n"<<CS_LS_NG ;
			std::cout<<"\n\n1562 *Route initiation typey:"<<typey<<" ******* P1y:"<<P1y<<" Py1oc"<<Py1oc<<"\n";

			Locoinformation[loco].Journeystatus = 1;

			RC = 99;

			return RC;
		}

		// 8. Fetch the direction of travel

		Direction = LSLD->GetLogicalSectiondirection(CS_LS_NG);

		Locoinformation[loco].Direction = Direction;

		//	9. Check for the loco direction of travel reversed flag

		if (Locoinformation[loco].Locodirectionreversed == 1)
		{
		   	if (Locoinformation[loco].Direction == 0)
		   	{
		   		Locoinformation[loco].Direction = 1;
		   	}
		   	else
		   	{
		   		Locoinformation[loco].Direction = 0;
		   	}
		}

		 // 10. install Tcode  for the locomotive in current section and also install the TrainID if it is greater than zero.

		//std::cout<<"\n\n1599 Tcode_NG :"<<Tcode_NG;

		if (LSLD->GetLogicalSectionTcode(CS_LS_NG) == 999)
		{
			LSLD->UpdateTcode(CS_LS_NG,Tcode_NG);

			if (Locoinformation[loco].TrainNumber > 0)  // if loco is connected to a train, check the train is in the section.
			{
				int TrainInsectionflag = LSLD->CheckTrainIDinsection(CS_LS_NG, Locoinformation[loco].TrainNumber);

					std::cout<<"\n";

				// 11. check to see if train is already positioned in the section

					if (TrainInsectionflag > 0)
					{
						std::cout<<"\n\n1613 Journey initiation - Train:"<<Locoinformation[loco].TrainNumber<<" found OK in journey start section:"<<CS_LS_NG;

						LSLD->UpdateTrainID(CS_LS_NG,1,Locoinformation[loco].TrainNumber,4);  // action flag set at 4 as the train is coupled to the loco.

						LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber, 0, CS_LS_NG);  // Update AllTrains object
					}
					else
					{
						std::cout<<"\n\n1621 *Route initiation ******* Train:"<<Locoinformation[loco].TrainNumber<<" not found in CS section ************* "<<CS_LS_NG ;
						std::cout<<"\n\n1622 *Loco movement only - Train movement NOT started  ";
						RC = 99;
					}
				}
				else
				{
					std::cout<<"\n\n1628 Route initiation for loco:"<<loco<<" TrainNumber :"<<Locoinformation[loco].TrainNumber;
				}
			}
			else
			{

				int TrainInsectionflag = LSLD->CheckTrainIDinsection(CS_LS_NG, Locoinformation[loco].TrainNumber);

				if (TrainInsectionflag > 0)
				{
					std::cout<<"\n\n1530 Journey initiation - Train:"<<Locoinformation[loco].TrainNumber<<" found OK in journey start section:"<<CS_LS_NG;

					LSLD->UpdateTrainID(CS_LS_NG,1,Locoinformation[loco].TrainNumber,4);  // action flag set at 4 as the train is coupled to the loco.

					LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber, 0, CS_LS_NG);  // Update AllTrains object
				}
			}

			// Update Trainsection details if Journey is using Trainsection logic

			if (Locoinformation[loco].Sectionlogicflag > 0)
			{
				Locoinformation[loco].CSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].CS);

				Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].NS);

				// install Traincode in Current Train section and Next Train section

				LSLD->TSupdateSection(Locoinformation[loco].CSTrainsection, 0,Locoinformation[loco].TrainNumber,3);

				LSLD->TSupdateSection(Locoinformation[loco].NSTrainsection, 0,Locoinformation[loco].TrainNumber,4);

				Locoinformation[loco].PSTrainsection_previous = Locoinformation[loco].CSTrainsection;

				PSTrainsectionprevious = Locoinformation[loco].CSTrainsection;

				LSLD->ProcessTrainRailcargroupmatch(Locoinformation[loco].CSTrainsection, 1);  // check to see if Train code replaces Railcar group code
			}

			// 13. Perform follow-on processing ;

			FollowonprocessingRC = FollowOnSubsteprocessing(loco);

			char logmsg[LOGRECORDLENGTH];

			bzero(logmsg, LOGRECORDLENGTH);

			if (FollowonprocessingRC == 0)
			{
				LSLD->LogicalSectionTcodeRefresh();	// trigger section processing in case points need to be switched.

				//std::cout<<"\n1666 Loco:"<<loco<<" Route:"<<Rte<<" Tcode:"<<Tcode_NG<<" RUN started\n";

				sprintf(lprmessage,"\n 1668 Loco %02d Route %02d RUN started .       ",loco,Rte);

				LNprmessage(loco,lprmessage);

				sprintf(logmsg,"1672 LNweblocorouteprocess Loco %02d Route %02d RUN started ",loco,Rte);

				// 11. Set the enabled flag

				Locoinformation[loco].Enabled = 1;				// Enable loco run

				Locoinformation[loco].Locomissingflag = 0;		// clear loco missing flag

				// 12. Set journey status flag

				Locoinformation[loco].Journeystatus = 2;

				LNloggingptr->Writelogmessage(1, logmsg);

				RC = 1;
			}
			else
			{
				std::cout<<"\n1682 Loco:"<<loco<<" Route:"<<Rte<<" RUN NOT STARTED - Followonprocessing error\n\n";

				sprintf(lprmessage,"\n 1684 Loco %02d Route %02d RUN ERROR .  ",loco,Rte);

				LNprmessage(loco,lprmessage);

				sprintf(logmsg,"LNweblocorouteprocess Loco %02d Route %02d RUN ERROR RC %02d - loco disabled ",loco,Rte,FollowonprocessingRC);

				LNloggingptr->Writelogmessage(1, logmsg);

				Locoinformation[loco].Enabled = 0;				// disable loco run

				RC = 99;
			}
		}

	//std::cout<<"\n\n1696 ===>  NG start sequence completed";

	return RC;
}

int LNweblocorouteprocess::LNsectionCrossingprocessingNG_Crossoverprocess(int loco)
{
	int RC  = 0;

	int StopcheckRC = 0;

	// Reset variables and fetch section status data

	LNsectionCrossingprocessingNG_Datarefresh(loco);

	//LNsectionCrossingprocessing_displaySTOPflags(loco);

	// Check to see if loco is detected in either Current Section or Next Section in case it is missing

	if ((NS_ps1_data_NG == 0) && (NS_ps2_data_NG == 0) && (CS_ps1_data_NG == 0) && (CS_ps2_data_NG == 0) && (Locoinformation[loco].Locomissingflag == 0))
	{
		Locoinformation[loco].Locomissingflag++;		// Loco detected missing for the first time

		sprintf(lprmessage,"\n1654-> Loco %02d on Route %02d MISSING from PS %03d and CS %03d and NS %03d Flag %01d.  ",loco,Locoinformation[loco].Route,PS_LS_NG,CS_LS_NG,NS_LS_NG,Locoinformation[loco].Locomissingflag);
		printf("\n1657-> Loco %02d on Route %02d MISSING from PS %03d and CS %03d and NS %03d Flag %01d.  ",loco,Locoinformation[loco].Route,PS_LS_NG,CS_LS_NG,NS_LS_NG,Locoinformation[loco].Locomissingflag);
		printf("\n");

		LNloggingptr->Writelogmessage(1, lprmessage);

		LNprmessage(loco,lprmessage);

		FollowOnSubsteprocessing(loco);

		Locoinformation[loco].Journeystatus = 3;

		RC = 99;
	}
	else
	{
		if (((NS_ps1_data_NG > 0) || (NS_ps2_data_NG > 0)) && (Locoinformation[loco].Signalhold == 0))	// loco detected in NS, with no signal hold, crossing already started
		{
			Crossing_inprogress_flag_NG = 1;					// set crossing in progress flag
			Locoinformation[loco].Locomissingflag = 0;			// clear loco missing flag
		//	std::cout<<"\n1674 ====> diag - Crossing already in progress <========";
			if (LNsectionCrossingprocessingNG_CheckPSclearance() > 0)
			{
				// Check for cleared PS so check if any pending STOP flags to execute

				StopcheckRC = LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(loco);
			}
		}
		else			// loco still in current section or still missing
		{
			if ((Locoinformation[loco].Locomissingflag > 0) && (CS_ps1_data_NG == 0) && (CS_ps2_data_NG == 0))	// Missing Loco still not found
			{
				Crossing_inprogress_flag_NG = 0;					// Reset crossing in progress flag
				Locoinformation[loco].Locomissingflag++;			// Increment the loco missing flag counter

				sprintf(lprmessage,"\n1679-> Loco %02d on Route %02d STILLMISSING from PS %03d and CS %03d and NS %03d Flag %01d.  ",loco,Locoinformation[loco].Route,PS_LS_NG,CS_LS_NG,NS_LS_NG,Locoinformation[loco].Locomissingflag);
				printf("\n1692-> Loco %02d on Route %02d STILL MISSING from PS %03d and CS %03d and NS %03d Flag %01d.  ",loco,Locoinformation[loco].Route,PS_LS_NG,CS_LS_NG,NS_LS_NG,Locoinformation[loco].Locomissingflag);
				printf("\n");
				RC = 99;
			}
			else
			{
				if ((Locoinformation[loco].Locomissingflag > 0) && ((CS_ps1_data_NG >  0) || (CS_ps2_data_NG > 0)))  // Missing loco recovered in CS
				{
					sprintf(lprmessage,"\n1687-> MISSING Loco %02d on Route %02d RECOVERED in CS %03d  ",loco,Locoinformation[loco].Route,CS_LS_NG);
					printf("\n1701-> MISSING Loco %02d on Route %02d RECOVERED in CS %03d ",loco,Locoinformation[loco].Route,CS_LS_NG);
					printf("\n");
					FollowOnSubsteprocessing(loco);
					Crossing_inprogress_flag_NG = 0;					// Reset crossing in progress flag
					Locoinformation[loco].Locomissingflag = 0;			// clear loco missing flag
					RC = 0;
				}
				else
				{
					if ((Locoinformation[loco].Locomissingflag == 0) && ((CS_ps1_data_NG >  0) || (CS_ps2_data_NG > 0)))
					{
						Crossing_inprogress_flag_NG = 0;
						//std::cout<<"\n1700 => Crossing NOT YET started, still in CS : "<<CS;
					}
				}

			}
		}
	}

	// check to see if loco is at signal hold and whether or not the hold has cleared

	if (Locoinformation[loco].Signalhold == 1)
	{
		if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0) && (NS_LS_NG > 0))
		{
		//	printf("\n1726 - diag Signal hold - Next section now clear - ready to restart");
			sprintf(lprmessage,"1727 - LocoRouteprocess - Signal hold clear for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
			LNloggingptr->Writelogmessage(1, lprmessage);
		//	printf("\n1729 - LocoRouteprocess - Signal hold cleared for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
		//	printf("\n");
			FollowOnSubsteprocessing(loco);
			StopcheckRC = LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(loco);
			return 0;
		}
	}



	//std::cout<<"\n1711 - diag Crossing flag:"<<Crossing_inprogress_flag_NG<<" Missing flag:"<<Locoinformation[loco].Locomissingflag;
	//return RC;

	if ((Locoinformation[loco].Locomissingflag == 0) && (StopcheckRC == 0)) // check to see if either the loco is not missing or it has already stopped at the destination
	{
		if (Crossing_inprogress_flag_NG ==  0) // check to see if there is a crossing to process
		{
			//std::cout<<"\n1701 => Crossing NOT YET started still in CS : "<<CS_LS_NG;
			//LNsectionCrossingprocessing_displaySTOPflags(loco);

			if (LNsectionCrossingprocessingNG_CheckPSclearance() > 0)
			{
				// cleared PS so check if any pending STOP flags to execute

				StopcheckRC = LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(loco);
			}

			if (Locoinformation[loco].Signalhold == 1)		// check for signal hold to see if it has been cleared
			{
				if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0) && (NS_LS_NG > 0))
				{
					//printf("\n1728 Next section now clear - ready to restart");
					sprintf(lprmessage,"1712 - LocoRouteprocess - Signal hold clear for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
					LNloggingptr->Writelogmessage(1, lprmessage);
					printf("\n1731 - LocoRouteprocess - Signal hold cleared for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
					printf("\n");

					FollowOnSubsteprocessing(loco);
					return 0;
			    }
				else
				{
					sprintf(lprmessage,"1738 - LocoRouteprocess - Signal HOLD still in place for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
					LNloggingptr->Writelogmessage(1, lprmessage);
					printf("\n1740 - LocoRouteprocess - Signal HOLD still in place for Loco %02d Tcode %04d Route %02d in CS %03d to goto NS %03d", loco, Locoinformation[loco].Traincode,Rte,CS_LS_NG,NS_LS_NG);
					return 0;
				}
			}
			else
			{


			}
		}
		else
		{
		//	std::cout<<"\n1716 diag => Crossing in progress to NS: "<<NS;
		//	LNsectionCrossingprocessing_displaySTOPflags(loco);

			PS_LS_NG = Locoinformation[loco].CS;
			CS_LS_NG = Locoinformation[loco].NS;

			Rtestep = Locoinformation[loco].Rtestep;

		//	std::cout<<"\n1756 - diag starting crossing -> Rtestep:"<<Rtestep;
			Rtestep++;
			Rtesubstep = 1;

			// extract the next route step;

			Rte = Locoinformation[loco].Route;
			NS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteNS;
			NSS_LS_NG = RouteData[Rte][Rtestep + 1][Rtesubstep].RouteNS;

		//	std::cout<<"\n1766 - diag Crossing in progress Rte:"<<Rte<<" Rtestep:"<<Rtestep;
		//	std::cout<<"\n1767 - diag Crossing in progress PS:"<<PS<<" CS:"<<CS<<" NS:"<<NS<<" NSS:"<<NSS;

			// save the previous logical section information

			Locoinformation[loco].PS_previous = Locoinformation[loco].PS;
			Locoinformation[loco].CS_previous = Locoinformation[loco].CS;
			Locoinformation[loco].NS_previous = Locoinformation[loco].NS;

			// save previous Train section information

			Locoinformation[loco].PSTrainsection_previous = LSLD->TSmatchedLS(Locoinformation[loco].PS_previous);
			Locoinformation[loco].CSTrainsection_previous = LSLD->TSmatchedLS(Locoinformation[loco].CS_previous);
			Locoinformation[loco].NSTrainsection_previous = LSLD->TSmatchedLS(Locoinformation[loco].NS_previous);

			// get the new section information

			Locoinformation[loco].PS = PS_LS_NG;
			Locoinformation[loco].CS = CS_LS_NG;
			Locoinformation[loco].NS = NS_LS_NG;
			Locoinformation[loco].Rtestep = Rtestep;

			LCSPDtable->SetlococurrentLS(loco, CS_LS_NG);

			// Update the Train section information

			Locoinformation[loco].PSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].PS);
			Locoinformation[loco].CSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].CS);
			Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].NS);

			Tcode_NG = Locoinformation[loco].Traincode;

			Direction = LSLD->GetLogicalSectiondirection(CS_LS_NG);

			Locoinformation[loco].Direction = Direction;

			if (Locoinformation[loco].Locodirectionreversed == 1)
			{
				if (Locoinformation[loco].Direction == 0)
				{
						Locoinformation[loco].Direction = 1;
				}
				else
				{
						Locoinformation[loco].Direction = 0;
				}
			}

			// check to see if any of the STOP flags are live

			StopcheckRC = LNsectionCrossingprocessingNG_Check_NS_STOP_flags(loco);

			if (StopcheckRC == 0)   // no live STOP flags
			{
			// install loco Tcode in next section if it is not already there and the section is clear

				if (Locoinformation[loco].Sectionlogicflag > 0)
				{

					int TSCSlastPS = LSLD->TSgetlastvalidPS(Locoinformation[loco].CSTrainsection_previous);

					//printf("\n\n1946 TSCSlastPS:%03d",TSCSlastPS);

					Locoinformation[loco].Trainsectioncrossingtype = 0;   // reset crossing type flag

					int CSlastPSoccupancy = LSLD->TSgetPSoccupancyflag(Locoinformation[loco].CSTrainsection, TSCSlastPS);

					if ((Locoinformation[loco].CSTrainsection == Locoinformation[loco].CSTrainsection_previous) && (CSlastPSoccupancy > 0))
					{
						//printf("\n\n1950 Setting type 1 Trainsection crossing:  Loco:%02d CSTrainsection:%03d CSTrainsection_previous:%03d CSlastPSoccupancy:%02d", loco, Locoinformation[loco].CSTrainsection, \
								Locoinformation[loco].CSTrainsection_previous, CSlastPSoccupancy);

						Locoinformation[loco].Trainsectioncrossingtype = 1;
					}

					if (Locoinformation[loco].CSTrainsection != Locoinformation[loco].CSTrainsection_previous)
					{
						Locoinformation[loco].Trainsectioncrossingtype = 2;
						//printf("\n\n1964 setting Train section crossing type as 2");
					}

					//std::cout<<"\n2434 Train section crossing type = "<<Locoinformation[loco].Trainsectioncrossingtype;
					//std::cout<<"\n2435 Current Trainsection is      :"<<Locoinformation[loco].CSTrainsection;
					//std::cout<<"\n2436 Previous Current Trainsection:"<<Locoinformation[loco].CSTrainsection_previous;
				}



				if (Locoinformation[loco].Sectionlogicflag > 0)
				{
					//std::cout<<"\n1847 loco:"<<loco<<" Sectionlogicflag GT 0:"<<Locoinformation[loco].Sectionlogicflag;

					Trainsection = LSLD->TSmatchedLS(NS_LS_NG);

					Trainsectionstatus = LSLD->TSgetoccupancyflag(Trainsection);

					//std::cout<<"\n1853 Trainsection check for NS-LS:"<<NS_LS_NG<<" to matching TM found:"<<Trainsection<<" status:"<<Trainsectionstatus;

					Locoinformation[loco].PSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].PS);

					Locoinformation[loco].CSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].CS);

					Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(Locoinformation[loco].NS);

					if (Locoinformation[loco].CSTrainsection == Locoinformation[loco].NSTrainsection)
					{
						// current and NS are the same so fetch next LS and retrieve the TM again

						Locoinformation[loco].NSTrainsection = LSLD->TSmatchedLS(RouteData[Rte][Rtestep + 1][Rtesubstep].RouteNS);

						//std::cout<<"\n1889 Fetching NS + 1";
					}

					if (Locoinformation[loco].CSTrainsection == Locoinformation[loco].PSTrainsection)
					{
						// current and PS are the same so fetch next LS and retrieve the TM again

						PSTrainsectionprevious = Locoinformation[loco].PSTrainsection;

						Locoinformation[loco].PSTrainsection = LSLD->TSmatchedLS(RouteData[Rte][Rtestep -1][Rtesubstep].RouteNS);

						//std::cout<<"\n1889 Fetching NS + 1";
					}

					// Update the Trainsection information

					/*
						unsigned int PSTrainsection;
						unsigned int CSTrainsection;
						unsigned int NSTrainsection;
						unsigned int CSsectionPS[MAXPSinTS];
						unsigned int CSsectionPSstatus[MAXPSinTS];
						unsigned int NSTrainsectionoccupancystatus;
					*/

					// build table of Physical sections linked to Current Train section

					for (int CScc = 0; CScc < MAXPSinTS; CScc++)
					{
						Locoinformation[loco].CSsectionPS[CScc] = LSLD->TSgetPS(Locoinformation[loco].CSTrainsection, CScc);
					}

					 TrainsectionactualPScount = 0;

					 for (int pscount = 0;pscount < MAXPSinTS;pscount++)
					 {
						 if (Locoinformation[loco].CSsectionPS[pscount] > 0)
						 {
							 TrainsectionactualPScount++;
						 }
					 }

					// build table of Physical sections current status - linked to Current Train section

					for (int CScs = 0;CScs < MAXPSinTS; CScs++)
					{
						if (Locoinformation[loco].CSsectionPS[CScs] > 0)
						{
							Locoinformation[loco].CSsectionPSstatus[CScs] = PSectiondatareturn(Locoinformation[loco].CSsectionPS[CScs],&PSmessage_NG[0]);
						}
					}

					// Set Next section train section status

					Locoinformation[loco].NSTrainsectionoccupancystatus = LSLD->TSgetoccupancyflag(Locoinformation[loco].NSTrainsection);

					//std::cout<<"\n1944 Crossing section check - setting TrainsectionCS:"<<Locoinformation[loco].CSTrainsection<<" TrainsectionNS:"<<Locoinformation[loco].NSTrainsection;

					//std::cout<<"\n1946 Checking Next Trainsectionstatus:"<<Locoinformation[loco].NSTrainsectionoccupancystatus;

					//std::cout<<"\n1958 TrainsectionactualPScount:"<<TrainsectionactualPScount;

					//std::cout<<"\n1960 CS-PS1:"<<Locoinformation[loco].CSsectionPS[0]<<" CS-PS2:"<<Locoinformation[loco].CSsectionPS[1]<<" CS-PS3:"<<Locoinformation[loco].CSsectionPS[2]<<" CS-PS4:"<<Locoinformation[loco].CSsectionPS[3];

					//std::cout<<"\n1962 CS-PS1status:"<<Locoinformation[loco].CSsectionPSstatus[0]<<" CS-PS2status:"<<Locoinformation[loco].CSsectionPSstatus[1]<<" CS-PS3status:"<<Locoinformation[loco].CSsectionPSstatus[2]<<" CS-PS4status:"<<Locoinformation[loco].CSsectionPSstatus[3];

					if ((Locoinformation[loco].NSTrainsectionoccupancystatus > 0) && (Locoinformation[loco].CSsectionPSstatus[TrainsectionactualPScount - 1] > 0))
					{
						//std::cout<<"\n\n1966 TrainsectionSTOP -> NSTS:"<<Locoinformation[loco].NSTrainsection<<" CSTS physical section number:"<<TrainsectionactualPScount<<" at PS:"<<Locoinformation[loco].CSsectionPS[TrainsectionactualPScount - 1]<<"\n";

						Locoinformation[loco].ActDCCvalue = 0;						// set actual DCC speed as zero

						Locoinformation[loco].Signalhold = 1;						// set signal hold

						printf("\n");
						sprintf(lprmessage,"Loco %02d Route %02d - Next Trainsection not ready, NS Tcode Signal hold %04d ",loco,Rte, LSLD->GetLogicalSectionTcode(NS_LS_NG));

						LNprmessage(loco,lprmessage);

						Locoinformation[loco].Journeystatus = 4;

						CSpeedcmdprocess(59,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);

						RC = 99;
					}

				}

				if ((Locoinformation[loco].Sectionlogicflag > 0) && (Locoinformation[loco].Trainsectioncrossingtype == 1))
				{
					if (Locoinformation[loco].TrainNumber == 0)
					{
						printf("\n\n\n 2079 - No Train number found for loco:%02d on Journey:%02d on Route:%02d",\
								loco,Locoinformation[loco].Journey, Locoinformation[loco].Route);
					}
					else
					{
						if (Locoinformation[loco].PSTrainsection_previous > 0)
						{
							LSLD->TSupdateSection(Locoinformation[loco].PSTrainsection_previous, Locoinformation[loco].TrainNumber, 0,5);
						}
						else
						{
							printf("\n\n2090 loco:%02d on Journey:%02d on Route:%02d -> Previous section zero - so no action ",loco,Locoinformation[loco].Journey, Locoinformation[loco].Route);
						}
					}
				}

				if (Locoinformation[loco].Sectionlogicflag == 0)
				{
					if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0) && (NS_LS_NG > 0))
					{
						LSLD->UpdateTcode(NS_LS_NG,Tcode_NG);

						int TrIdsub = LSLD->CheckTrainIDinsection(CS_LS_NG, Locoinformation[loco].TrainNumber); // Get the TrainID subscript for current section

						int TrIdcouplerflag = LSLD->GetLSTrainIDcoupled(CS_LS_NG,TrIdsub);	// fetch the coupler flag for the current section

						if (TrIdcouplerflag > 0)
						{
							LSLD->UpdateTrainID(NS_LS_NG,1,Locoinformation[loco].TrainNumber,4);  	// action flag set at 4 as the train is coupled to the loco.

							if (NS_LS_NG > 0)
							{
									LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber,0, NS_LS_NG);
							}
							else
							{
								std::cout<<"\n1887 No Train info update as NS = 0";
							}
						}
					}

					if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) != 0) && (NS_LS_NG > 0) && (LSLD->GetLogicalSectionTcode(NS_LS_NG) != Tcode_NG))	// next section is occupied so stop the loco
					{
						// check to see if the loco should proceed to the next section as there is a train there for it to couple to

						if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 994) && (Locoinformation[loco].STDCflag == 1))
						{
							std::cout<<"\n1954 Loco, with STDCflag reaches NS which has 994 Tcode so proceeding ";
							LSLD->UpdateTcode(NS_LS_NG,Tcode_NG);
							RC = 0;
						}
						else
						{
							Locoinformation[loco].ActDCCvalue = 0;						// set actual DCC speed as zero

							Locoinformation[loco].Signalhold = 1;						// set signal hold

							// set the loco speed to zero

							//std::cout<<"\n1832 NS not ready, Signal hold set on loco "<<loco<<" at section "<<NS;
							printf("\n");
							sprintf(lprmessage,"Loco %02d Route %02d - NS not ready, NS Tcode Signal hold %04d ",loco,Rte, LSLD->GetLogicalSectionTcode(NS_LS_NG));

							LNprmessage(loco,lprmessage);

							Locoinformation[loco].Journeystatus = 4;

							CSpeedcmdprocess(59,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);
							RC = 99;
						}
					}

				}



				if (RC == 0)
				{
					// Process the remaining Route sub step records

					if ((Locoinformation[loco].Sectionlogicflag > 0) && (Locoinformation[loco].Trainsectioncrossingtype == 2))
					{
						LSLD->TSupdateSection(Locoinformation[loco].CSTrainsection,0,Locoinformation[loco].TrainNumber,6);
					}

					FollowonprocessingRC = FollowOnSubsteprocessing(loco);

					StopcheckRC = LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(loco);

					Crossing_inprogress_flag_NG = 0;
				}
			}

			// ================ end of Stopcheck == 0 section
		}
	}
	else
	{
		// Loco is still missing so no action or has already finished
		RC = 99;
	}

	if (StopcheckRC > 0)
	{
	//	std::cout<<"\n1872 diag - StockcheckRC:"<<StopcheckRC;
		RC = 98;
	}

	//std::cout<<"\n1876 - diag - End of section processing";
	//LNsectionCrossingprocessing_displaySTOPflags(loco);
	return RC;
}

void LNweblocorouteprocess::LNsectionCrossingprocessing_displaySTOPflags(int loco)
{
	/*
		unsigned int STPCflag;
		unsigned int STPDflag;
		unsigned int STPBflag;
		unsigned int STPMflag;
		unsigned int STCCflag;
		unsigned int STPXflag;
		unsigned int STDCflag;
		unsigned int STOPflag;
	*/
	std::cout<<"\n1894 Stop flags display for loco:"<<loco;
	std::cout<<"\nSTPC "<<Locoinformation[loco].STPCflag;
	std::cout<<"\nSTPD "<<Locoinformation[loco].STPDflag;
	std::cout<<"\nSTPB "<<Locoinformation[loco].STPBflag;
	std::cout<<"\nSTPM "<<Locoinformation[loco].STPMflag;
	std::cout<<"\nSTCC "<<Locoinformation[loco].STCCflag;
	std::cout<<"\nSTPX "<<Locoinformation[loco].STPXflag;
	std::cout<<"\nSTDC "<<Locoinformation[loco].STDCflag;
	std::cout<<"\nSTOP "<<Locoinformation[loco].STOPflag;
	std::cout<<"\n\n";
}


int LNweblocorouteprocess::LNsectionCrossingprocessingNG_Check_NS_STOP_flags(int loco)
{
	int RC = 0;

	// check for stop code STPX

	if (Locoinformation[loco].STPXflag > 0)
	{
						std::cout<<"\n1812 STPX command processed 1"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep;

						int Rte = Locoinformation[loco].Route;

						CSpeedcmdprocess(60,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);

						Journeycompletedprocessing(loco,1);

						sprintf(lprmessage,"Loco %02d Route %02d RUN Completed - STOP or STPX ",loco,Rte);

						LNprmessage(loco,lprmessage);

						RC = 98;
	}


	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 5)  // STOP - Stop when entering the section
	{
						//std::cout<<"\n1680 STOP command processed 1"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep;

						int Rte = Locoinformation[loco].Route;

						CSpeedcmdprocess(68,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);

						Journeycompletedprocessing(loco,2);

						sprintf(lprmessage,"Loco %02d Route %02d RUN Completed - STOP command ",loco,Rte);

						LNprmessage(loco,lprmessage);

						RC = 98;

	}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 23)   // STPX - Stop at next section crossing
	{
						Locoinformation[loco].STPXflag = 1;
						STPXprocessflag = 1;
						std::cout<<"\nLoco:"<<loco<<" STPX command initialized 1850 "<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1;

						RC = 0;
	}



	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 2)   // STPD - Stop when device triggered command
	{
						Locoinformation[loco].STPDflag = 1;
						Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
						STPDprocessflag = 1;
						std::cout<<"\nLoco:"<<loco<<" STPD command processed 1447"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1;
						RC = 0;
	}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 16)   // STPM - Stop when device triggered command
	{
						Locoinformation[loco].STPMflag = 1;
						Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
						RC = 0;
						//if ((Crossing_flag == 3) || (Crossing_flag == 2))
						//	{
						//		Crossing_flag = 4;								// Upgrade to crossing flag as it is a current STPM command
						//	}
						//	std::cout<<"\nLoco:"<<loco<<" STPM command processed 1552 "<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1;
						//}
	}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 22)   // STCC - Stop when device triggered command
	{
						Locoinformation[loco].STCCflag = 1;
						Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
						Locoinformation[loco].Cmdparam2 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;

						std::cout<<"\nLoco:"<<loco<<" STCC command processed 1447"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1<<" value:"<<Locoinformation[loco].Cmdparam2;
						RC = 0;
	}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 25)   // STDC - Stop when device triggered command and couple the train
	{
						Locoinformation[loco].STDCflag = 1;
						Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
						Locoinformation[loco].Cmdparam2 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;

						std::cout<<"\n1890 Loco:"<<loco<<" STDC command processed "<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1<<" value:"<<Locoinformation[loco].Cmdparam2;
						RC = 0;
	}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 4)   // STPC - Stop when previous section is clear
						{
							std::cout<<"\nLoco:"<<loco<<" STPC command processed 1297"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep;
							Locoinformation[loco].STPCflag = 1;
							RC = 0;
						}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 15)   // STPB - Stop when previous section is clear
						{
							std::cout<<"\nLoco:"<<loco<<" STPB command processed 1521"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep;
							Locoinformation[loco].STPBflag = 1;
							RC = 0;
						}

	if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 26)   // BLCN command
	{
				    	std::cout<<"\nLNweblocoroueprocess 1900 - BLCN command found as substep 1";

						std::cout<<"\nLNweblocorouteprocess 2003 BLCN command processed for loco "<<loco<<" in step:"<<Rtestep;
						std::cout<<"\nActual:"<<Locoinformation[loco].Loopcounteractual<<" Target:"<<Locoinformation[loco].Loopcountertarget;
						if (Locoinformation[loco].Loopcounteractual != Locoinformation[loco].Loopcountertarget) // test to see if loop stop flag is set
						{
							int RRSCRtestep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;  // route step number

							//int RRSCRtesubstep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;  // route sub step number

							Locoinformation[loco].Rtestep 		= RRSCRtestep;

							Locoinformation[loco].Rtesubstep 	= 1; //RRSCRtesubstep;

							std::cout<<"\nLNweblocorouteprocess 1914 BLCN command processed";
							std::cout<<"\nRte:"<<Rte<<" Rtestep:"<<Locoinformation[loco].Rtestep<<" substep:"<<Locoinformation[loco].Rtesubstep;

										// update the PS, CS and NS details

							Rte = Locoinformation[loco].Route;

							PS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RoutePS;
							CS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteCS;
							NS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteNS;

							Locoinformation[loco].PS = PS_LS_NG;
							Locoinformation[loco].CS = CS_LS_NG;
							Locoinformation[loco].NS = NS_LS_NG;

							std::cout<<"\n2030 PS:"<<Locoinformation[loco].PS<<" CS:"<<Locoinformation[loco].CS<<" NS:"<<Locoinformation[loco].NS;

							if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0) && (NS_LS_NG > 0))
							{
								LSLD->UpdateTcode(NS_LS_NG,Tcode_NG);

								int TrIdsub = LSLD->CheckTrainIDinsection(CS_LS_NG, Locoinformation[loco].TrainNumber); // Get the TrainID subscript for current section

								int TrIdcouplerflag = LSLD->GetLSTrainIDcoupled(CS_LS_NG,TrIdsub);	// fetch the coupler flag for the current section

								if (TrIdcouplerflag > 0)
								{
									LSLD->UpdateTrainID(NS_LS_NG,1,Locoinformation[loco].TrainNumber,4);  	// action flag set at 4 as the train is coupled to the loco.

									if (NS_LS_NG > 0)
									{
												LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber,0, NS_LS_NG);
												}
												else
												{
												std::cout<<"\n2045 No Train info update as NS = 0";
												}
									}
								}

								std::cout<<"\nLNweblocorouteprocess 2050 BLCN command processed for loco "<<loco;
								std::cout<<"\nRoute step counter set to "<<Locoinformation[loco].Rtestep;
								std::cout<<"\nRoute Sub step counter set to "<<Locoinformation[loco].Rtesubstep;
								RC = 0;
							}

							// =================== end of command 26 ============================
						}
	return RC;
}

int LNweblocorouteprocess::LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(int loco)
{
	int RC =  0;

	//std::cout<<"\n2066 - diag - Checking STPC and STPB flags";

	if ((PS_ps1_data_NG ==  0) && (PS_ps2_data_NG == 0) && (Locoinformation[loco].STPCflag == 1))
	{
			RC = 4;
		//	std::cout<<"\n====> 2069 - diag - STPC stop found";
	}

	if (Locoinformation[loco].STPBflag == 1)
	{
			RC = 4;
		//	std::cout<<"\n====> 2075 - diag - STPB stop found";
	}

	if (RC == 4)
	{
			// set the loco speed to zero

			Rte = Locoinformation[loco].Route;

			CSpeedcmdprocess(62,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);

			if (Locoinformation[loco].TrainLSswitchoverflag > 0)
			{
					std::cout<<"\n1568 Train switchover command processed - from LS:"<<Locoinformation[loco].TrainswitchoverfromLS<<" to LS:"<<Locoinformation[loco].TrainswitchovertoLS;

					LSLD->TrainIDLSswitchover(Locoinformation[loco].TrainswitchoverfromLS, Locoinformation[loco].TrainswitchovertoLS);
			}

			Journeycompletedprocessing(loco,3);
			sprintf(lprmessage,"1757 - LocoRouteprocess - Loco %02d Route %02d 1202 RUN Completed - STPB / STPC.       ",loco,Rte);
			LNloggingptr->Writelogmessage(1, lprmessage);
			LNprmessage(loco,lprmessage);
	}

	return RC;
}

int LNweblocorouteprocess::LNsectionCrossingprocessingNG_CheckPSclearance()
{
	int RC = 0; // check to see if Tcode still in PS but no physical presence in section

	//printf("\n1736 PS clearance check - PS %03d PS_Tcode_NG:%04d Tcode_NG:%04d PS_ps1_data_NG:%01d PS_ps2_data_NG:%01d ",PS,PS_Tcode_NG,Tcode_NG,PS_ps1_data_NG,PS_ps2_data_NG);

	if ((PS_Tcode_NG == Tcode_NG) && (PS_ps1_data_NG == 0) && (PS_ps2_data_NG == 0))
	{
		//std::cout<<"\n1741 PS Tcode:"<<Tcode_NG<<" cleared from LS:"<<	PS;

		LSLD->UpdateTcode(PS_LS_NG,0);

		RC = 4;
	}
	else
	{
		RC = 0;
	}

	return RC;
}


void LNweblocorouteprocess::LNSTPDprocessing()
{
	int loco = 0;

	int devicenum = 0;
	
	int devicecounter = 0;

	// STPDprocessflag

	int devicestatus = 0;

	int 		PS_LS;
	int 		CS_LS;
	int 		NS_LS;

	//char lprmessage[LRMESSAGELENGTH];

	for (loco = 1;loco < Lococountmax;loco++)
	{
		if (Locoinformation[loco].Enabled > 0)
		{
		
		//std::cout<<"\nLoco STPD / STPB / STPM check  for loco:"<<loco<<" Enabled flag:"<<Locoinformation[loco].Enabled;


		if ((Locoinformation[loco].STPDflag > 0) || (Locoinformation[loco].STPBflag > 0) || (Locoinformation[loco].STDCflag > 0) || (Locoinformation[loco].STCCflag > 0) || (Locoinformation[loco].STPMflag > 0))
		{
			// std::cout<<"\n988...Loco STPD or STDC or STPB or STPM stop check for loco:"<<loco<<" Device num:"<<Locoinformation[loco].Cmdparam1;

			devicenum = Locoinformation[loco].Cmdparam1;

			devicestatus = 0;

			devicestatus = LSLD->GetLDevicestatus(devicenum);
			
			devicecounter = LSLD->GetLDevicecountervalue(devicenum);		
			
		//	std::cout<<"\n998 ...Loco STPD or STPB or STPM or STDC stop check for loco:"<<loco<<" Device num:"<<Locoinformation[loco].Cmdparam1<<" device status:"<<devicestatus<<" value:"<<Locoinformation[loco].Cmdparam2;

			if ((devicestatus > 0) && (Locoinformation[loco].STCCflag > 0)) 
			{
				std::cout<<"\n1002 Loco STCC command for loco:"<<loco<<" Device num:"<<Locoinformation[loco].Cmdparam1<<" status:"<<devicestatus<<" value:"<<Locoinformation[loco].Cmdparam2;
				std::cout<<"\nActual value:"<<devicecounter;
			}

			if ((devicestatus > 0) && (Locoinformation[loco].STDCflag > 0))
			{
				std::cout<<"\n1008 Loco STDC command for loco:"<<loco<<" Device num:"<<Locoinformation[loco].Cmdparam1<<" status:"<<devicestatus<<" value:"<<Locoinformation[loco].Cmdparam2;
				std::cout<<"\nActual value:"<<devicecounter;
			}

			if (devicestatus > 0)			// check to see if device is triggered
			{
				// set the loco speed to zero

				std::cout<<"\n889 Processing STOP command for loco:"<<loco<<" at Device:"<<devicenum;

				// step2 - initialize LS from Loco records

				PS_LS 		= Locoinformation[loco].PS;
				CS_LS 		= Locoinformation[loco].CS;
				NS_LS		= Locoinformation[loco].NS;

				int Rte = Locoinformation[loco].Route;

				CSpeedcmdprocess(67,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);

				//LSLD->UpdateTcode(CS_LS,0);				// clear loco code from current section

				Locoinformation[loco].Enabled = 0;		// stop loco processing

				Locoinformation[loco].Speed = 0;

				if (PS_LS > 0)
					{
						if (LSLD->GetLSTcode(PS_LS) != 994)			// check that previous section not showing a stationary train code
						{
							LSLD->UpdateTcode(PS_LS,0);				// clear loco code from previous section listed
						}
					}

				if (CS_LS > 0)
					{
					//	LSLD->UpdateTcode(CS_LS,0);				// clear loco code from current section
					}

				if (NS_LS > 0)
					{
						LSLD->UpdateTcode(NS_LS,0);				// clear loco code from current section
					}

				printf("\n1049 Device triggered Loco reset 933\n");
				
				if (Locoinformation[loco].STDCflag > 0)
				{
					std::cout<<"\n1008 Train to be stopped by command STDC at device:"<<devicenum<<" ready to be connected to train";

					LSLD->UpdateTrainID(CS_LS,1,0,8);  // action flag set at 8 so the train is coupled to the loco.

				//	LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrianNumber, 0, CS_LS);  // Update AllTrains object
				}

				if (Locoinformation[loco].TrainLSswitchoverflag > 0)
				{
					std::cout<<"\n1068 Train switchover command processed - from LS:"<<Locoinformation[loco].TrainswitchoverfromLS<<" to LS:"<<Locoinformation[loco].TrainswitchovertoLS;

					LSLD->TrainIDLSswitchover(Locoinformation[loco].TrainswitchoverfromLS, Locoinformation[loco].TrainswitchovertoLS);
				}

				Journeycompletedprocessing(loco,4);

				//int lastjourney = Locoinformation[loco].Journey;
				
				sprintf(lprmessage,"1073 Loco %02d Route %02d RUN Completed - STOPPED by device %02d. ",loco,Rte,devicenum);

				LNprmessage(loco,lprmessage);
			}
		}
	  }
	}

}

int LNweblocorouteprocess::StopFlagprocessing()
{
	int NScmd = RouteData[Rte][Rtestep + 1][1].RouteCmd;

	int StopflagRC;

	// NS command Stop commands
	// 2   STPD
	// 4   STPC
	// 5   STOP
	// 15  STPB
	// 16  STPM
	// 22  STCC
	// 23  STPX
	// 25  STPC

	//std::cout<<"\n\n2278 Rte:"<<Rte<<" Rtestep:"<<Rtestep<<" Stop flag check - NS command found:"<<NScmd;

	if ((NScmd == 5)|| (NScmd == 2) || (NScmd == 4) || (NScmd == 15) || (NScmd == 16) || (NScmd == 22) || (NScmd == 23) || (NScmd == 25))
	{
		//std::cout<<"\n\n2282 NS command STOP found ";
		StopflagRC = 9;
	}
	else
	{
		StopflagRC = 0;
	}

	return StopflagRC;
}


int LNweblocorouteprocess::FollowOnSubsteprocessing(int loco)
{
	int FollowonRC = 0;	
		
	Locoinformation[loco].Routesectionhistory[Rtestep] = Locoinformation[loco].CS;	

	for (Rtesubstep = 2; Rtesubstep <=Routemaxsubsteps;Rtesubstep++)
					{
					if (RouteData[Rte][Rtestep][Rtesubstep].RouteCS > 0) // check to see if valid line to process
						{
							Locoinformation[loco].Cmdinprogress = RouteData[Rte][Rtestep][Rtesubstep].RouteCmd;

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 3)  // SLS
								{
									Locoinformation[loco].NextsectionSTOPflag = StopFlagprocessing();

									if (Locoinformation[loco].STOPflag == 0)
									{
										Plannedspeed = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;

										if ((Plannedspeed > 1) && (Locoinformation[loco].NextsectionSTOPflag > 0)) // check to see if next section has a stop command
										{
											Plannedspeed = 1;
											std::cout<<"\n\n2330 - STOP command processing - Plannedspeed overridden and set to 1";
										}
									}
									else
									{
										Plannedspeed = 0;
										sprintf(lprmessage,"Loco %02d Route %02d - Reached STOP command at LS %03d  ",loco,Rte,CS_LS_NG);
										LNprmessage(loco,lprmessage);
									}

									//std::cout<<"\n 2555 Set speed on Rte:"<<Rte<<" Rtestep:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" Planned route speed:"<<Plannedspeed;

									// check to see if next section is free
									
								//	printf("\n1505 Checking Tcode in NS %i value is %i",NS, LSLD->GetLogicalSectionTcode(NS));

									if (LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0)
									{
										LSLD->UpdateTcode(NS_LS_NG,Tcode_NG);								// update the Next Section NS Tcode

										int TrIdsub = LSLD->GetLSTrainID(CS_LS_NG, Locoinformation[loco].TrainNumber); // Get the TrainID subscript

										int TrIdcouplerflag = LSLD->GetLSTrainIDcoupled(CS_LS_NG,TrIdsub);	// fetch the coupler flag

										//std::cout<<"\n\n==> 2048 CS:"<<CS<<" TrIdsub:"<<TrIdsub<<" coupler flag:"<<TrIdcouplerflag;

									//	TrIdcouplerflag = 1;

										if (TrIdcouplerflag > 0)
										{
											LSLD->UpdateTrainID(CS_LS_NG,1,Locoinformation[loco].TrainNumber,4); // update train ID code in NS
										}
										Locoinformation[loco].Planspeed	= Plannedspeed;

										if (LCSPDtable->LocospeedGet(loco,Plannedspeed) < 0)
										{
											std::cout<<"\nLNweblocorouteprocess - 2535 - ERROR - No Loco AllData record for loco index "<<loco;
											FollowonRC = 8;
											return FollowonRC;
										}
										else
										{
											//std::cout<<"\nSpeed load 2564";
											Locoinformation[loco].ActDCCvalue = LCSPDtable->LocospeedGet(loco,Plannedspeed);
										}

										Locoinformation[loco].Signalhold = 0;						// clear signal hold

									//	printf("\n2436 Signal cleared for Loco %02d for NS %03d ",loco, NS);
									//	printf("\n");

										if (TrIdcouplerflag > 0)
										{
											if (NS_LS_NG > 0)
											{
												LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber,0, NS_LS_NG);
											}
											else
											{
												std::cout<<"\n2028 No train info update as NS = 0";
											}
										}

										CSpeedcmdprocess(56,Locoinformation[loco].Slotnumber,Locoinformation[loco].ActDCCvalue,Locoinformation[loco].Direction);


									}
									else
									{
										if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) != Tcode_NG)  && (NS_LS_NG != 0))
										{
										// set loco as being on signal hold
											Locoinformation[loco].ActDCCvalue = 0;						// set actual DCC speed as zero
											Locoinformation[loco].Signalhold = 1;						// set signal hold

										// set the loco speed to zero

											std::cout<<"\nNS not ready, Signal hold set on section crossover - 1533 ";

											sprintf(lprmessage,"Loco %02d Route %02d - NS not ready, Signal hold in %02d .  ",loco,Rte,CS_LS_NG);

											printf("\nLoco %02d Route %02d - NS %02d Tcode not ready, Signal hold in CS %02d .  ",loco,Rte,NS_LS_NG,CS_LS_NG);

											LNprmessage(loco,lprmessage);

											std::cout<<"\n2641 SLS command processed";

											CSpeedcmdprocess(55,Locoinformation[loco].Slotnumber,0,Locoinformation[loco].Direction);
										}
										else
										{

											// update the Next Section NS Tcode
											Locoinformation[loco].Planspeed	= Plannedspeed;

											if (LCSPDtable->LocospeedGet(loco,Plannedspeed) < 0)
											{
												std::cout<<"\nLNweblocorouteprocess - 2654 - ERROR - No Loco AllData record for loco index "<<loco;
												FollowonRC = 8;
												return FollowonRC;
											}
											else
											{
												Locoinformation[loco].ActDCCvalue = LCSPDtable->LocospeedGet(loco,Plannedspeed);
											}

											Locoinformation[loco].ActDCCvalue = LCSPDtable->LocospeedGet(loco,Plannedspeed);
											Locoinformation[loco].Signalhold = 0;						// clear signal hold
										//	printf("\n2497 - Signal cleared for Loco %02d at NS %03d",loco,NS);
										//	printf("\n");
											CSpeedcmdprocess(54,Locoinformation[loco].Slotnumber,Locoinformation[loco].ActDCCvalue,Locoinformation[loco].Direction);

										}
									}

								}  // end of SLS processing

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 2)   // STPD - Stop when device triggered command
								{
									Locoinformation[loco].STPDflag = 1;
									Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
									STPDprocessflag = 1;

									//std::cout<<"\nSTPD command processed 1462"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1;
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 15)   // STPB - Stop when device triggered command or previous section clear
								{
									Locoinformation[loco].STPBflag = 1;
									Locoinformation[loco].Cmdparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;									

									std::cout<<"\nSTPB command processed 1702"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep<<" device:"<<Locoinformation[loco].Cmdparam1;
								}

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 4)   // STPC - Stop when previous section is clear
								{
									//std::cout<<"\nSTPC command processed 1452"<<" Rte:"<<Rte<<" Step:"<<Rtestep<<" Rtesubstep:"<<Rtesubstep;
									Locoinformation[loco].STPCflag = 1;
								}

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 6)   // RLD - reset logical device = LDset
								{
									int RLDparam = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;

									//std::cout<<"\n1570 RLD command processed Rte: "<<Rte<<" Step:"<<Rtestep<<" Substep:"<<Rtesubstep;

									LSLD->UpdateDcode(RLDparam,0);
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 11)   // ETCS - enable train car scanning
								{
									int ETCSTraincode  = Locoinformation[loco].TrainNumber;	// derived from current train code
									int ETCSlococode   = Locoinformation[loco].Traincode;	// derived from loco & journey details
									int ETCSlocoindex   = loco;
									
									//Enabletrainscancmd(int Locoindx, int Traincode, int Lococode)
									
									Enabletrainscancmd(ETCSlocoindex, ETCSTraincode, ETCSlococode);									
									
									printf("\nETCS command for Loco indx %i Traincode %i Lococode %i ", ETCSlocoindex,ETCSTraincode,ETCSlococode);
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 12)   // DTCS - disable train car scanning
								{
									int DTCSTraincode  = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
									int DTCSlococode   = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;
									int DTCSlocoindex   = loco;					
									
								}
								
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 8)   // Enable logical device trigger 
								{
									int ELTparam = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;

									//std::cout<<"\n1713 ELT command processed for device: "<<ELTparam<<"\n";

									LSLD->UpdateTrigger(ELTparam,1,1,0);				//Enable logical device trigger 
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 9)   // Reset logical device trigger 
								{
									int ELTparam = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;

									//std::cout<<"\n1713 RLT command processed for device: "<<ELTparam<<"\n";

									LSLD->UpdateTrigger(ELTparam,2,0,0);				//Reset logical device trigger 
								}
								

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 13)   // SDC - set device counter
								{
									int SDCparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
									int SDCparam2 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;

									//std::cout<<"\nSDC command processed";

									LSLD->SetCountertarget(SDCparam1,SDCparam2);
								}

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 14)   // RDC - Reset device counter
								{
									int SDCparam1 = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;

							//		std::cout<<"\nRDC command processed";

									LSLD->ResetCountertarget(SDCparam1);
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 17)   // 17 = RRSC(x,y)   - reset set route step counter to step x, substep y   
								{
									int RRSCRtestep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;  // route step number 
									
									int RRSCRtesubstep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;  // route sub step number 
									
									Locoinformation[loco].Rtestep 		= RRSCRtestep;
				
									Locoinformation[loco].Rtesubstep 	= RRSCRtesubstep;

									//std::cout<<"\nRRSC command processed for loco "<<loco;
									//std::cout<<"\nRoute step counter reset to "<<Locoinformation[loco].Rtestep;
									//std::cout<<"\nRoute Sub step counter reset to "<<Locoinformation[loco].Rtesubstep;
									return FollowonRC;
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 18)   // 18 = ILCC   - increment the actual loop counter    
								{
									
									Locoinformation[loco].Loopcounteractual++;
									std::cout<<"\nLNweblocorouteprocess 3067 ILCC command processed for loco "<<loco<<" at step:"<<Rtestep<<" substep:"<<Rtesubstep;
									std::cout<<"\nLoop counter actual set to  " <<Locoinformation[loco].Loopcounteractual<<" \n";																		
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 24)   // 24 = EUD(a,b,c) - Enable uncoupler a with device b on target c
								{
									std::cout<<"\n2154.. EUD processing";
									std::cout<<"\n2155 Uncoupler:"<<RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1<<" device:"\
											<<RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2<<" Target:"<<RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam3;

									// int Triggerdevice, int Uncouplernumber, int Target);

									LSLD->StoreDetectortriggeractionconfig(RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2,RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1,\
											RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam3,loco,Locoinformation[loco].TrainNumber,NS_LS_NG, NSS_LS_NG);
								}

								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 20)   // 20 = SLCT   - set loop counter target value     
								{
									int tvalue = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;
									Locoinformation[loco].Loopcountertarget = tvalue;
									std::cout<<"\nSLCT command processed for loco "<<loco;
									std::cout<<"\nLoop counter target set to  " <<Locoinformation[loco].Loopcountertarget<<" \n";																		
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 19)   // 19 = BLCE(x,y)   - branch if loop counter actual = target   
								{
									if (Locoinformation[loco].Loopcounteractual == Locoinformation[loco].Loopcountertarget) // test to see if loop stop flag is set 
									{
									int RRSCRtestep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;  // route step number 
									
									int RRSCRtesubstep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;  // route sub step number 
									
									Locoinformation[loco].Rtestep 		= RRSCRtestep;
				
									Locoinformation[loco].Rtesubstep 	= RRSCRtesubstep;								    

									std::cout<<"\nBLCE command processed for loco "<<loco;
									std::cout<<"\nRoute step counter set to "<<Locoinformation[loco].Rtestep;
									std::cout<<"\nRoute Sub step counter set to "<<Locoinformation[loco].Rtesubstep;
									return FollowonRC;
								    }								
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 26)   // 26 = BLCN(x,y)   - branch if loop counter actual <> target   
								{
									std::cout<<"\nLNweblocorouteprocess 3112 BLCN command processed for loco "<<loco<<" in step:"<<Rtestep;
									std::cout<<"\nActual:"<<Locoinformation[loco].Loopcounteractual<<" Target:"<<Locoinformation[loco].Loopcountertarget;
									if (Locoinformation[loco].Loopcounteractual != Locoinformation[loco].Loopcountertarget) // test to see if loop stop flag is set 
									{
									int RRSCRtestep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;  // route step number 
									
									int RRSCRtesubstep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;  // route sub step number 
									
									Locoinformation[loco].Rtestep 		= RRSCRtestep;
				
									Locoinformation[loco].Rtesubstep 	= 1; //RRSCRtesubstep;		
									
									// update the PS, CS and NS details
									
									Rte = Locoinformation[loco].Route;

									PS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RoutePS;
									CS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteCS;
									NS_LS_NG = RouteData[Rte][Rtestep][Rtesubstep].RouteNS;

									Locoinformation[loco].PS = PS_LS_NG;
									Locoinformation[loco].CS = CS_LS_NG;
									Locoinformation[loco].NS = NS_LS_NG;
									
									if ((LSLD->GetLogicalSectionTcode(NS_LS_NG) == 0) && (NS_LS_NG > 0))
									{
										LSLD->UpdateTcode(NS_LS_NG,Tcode_NG);

										int TrIdsub = LSLD->CheckTrainIDinsection(CS_LS_NG, Locoinformation[loco].TrainNumber); // Get the TrainID subscript for current section

										int TrIdcouplerflag = LSLD->GetLSTrainIDcoupled(CS_LS_NG,TrIdsub);	// fetch the coupler flag for the current section

										if (TrIdcouplerflag > 0)
										{
											LSLD->UpdateTrainID(NS_LS_NG,1,Locoinformation[loco].TrainNumber,4);  	// action flag set at 4 as the train is coupled to the loco.

											if (NS_LS_NG > 0)
											{
											LNallTrainsinformation->UpdateCurrentLSlocation(Locoinformation[loco].TrainNumber,0, NS_LS_NG);
											}
											else
											{
											std::cout<<"\n1887 No Train info update as NS = 0";
											}
										}
									}					    

									std::cout<<"\nLNweblocorouteprocess 3122 BLCN command processed for loco "<<loco;
									std::cout<<"\nRoute step counter set to "<<Locoinformation[loco].Rtestep;
									std::cout<<"\nRoute Sub step counter set to "<<Locoinformation[loco].Rtesubstep;
									return FollowonRC;
								    }
								    else
								    {
										std::cout<<"\n3131 Target = Actual so no branch, moving to next step";
									}								
								}
								
								if (RouteData[Rte][Rtestep][Rtesubstep].RouteCmd == 21)   // 21 = BLFS(x,y)   - branch if loop stop flag is set   
								{
									if (Locoinformation[loco].Loopstopflag == 1) // test to see if loop stop flag is set 
									{
									int RRSCRtestep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam1;  // route step number 
									
									int RRSCRtesubstep = RouteData[Rte][Rtestep][Rtesubstep].RouteCmdparam2;  // route sub step number 
									
									Locoinformation[loco].Rtestep 		= RRSCRtestep;
				
									Locoinformation[loco].Rtesubstep 	= RRSCRtesubstep;

									std::cout<<"\nBLSF command processed for loco "<<loco;
									std::cout<<"\nRoute step counter set to "<<Locoinformation[loco].Rtestep;
									std::cout<<"\nRoute Sub step counter set to "<<Locoinformation[loco].Rtesubstep;
									return FollowonRC;
								    }								
								}
																
						}
				}
	
	return FollowonRC;
}

void LNweblocorouteprocess::Setschedulerrunstatus(int loco, int value)
{
	//std::cout<<"\n2640 Set scheduler status for loco "<<loco<<" to value "<<value;
	Locoinformation[loco].Schedulerunstatus = value;
}

int LNweblocorouteprocess::Getschedulerrunstatus(int loco)
{
	return Locoinformation[loco].Schedulerunstatus;
}

int LNweblocorouteprocess::Journeycompletedprocessing(int loco, int call_location)
{
	// processing for when a train or loco has completed a route execution.

	std::cout<<"\n\n ====> Journey completed - called from location:"<<call_location<<"\n";

	switch(call_location)
	{
		case 1:
		{
			std::cout<<"\n2747 - called by STPX stop";
		}
		break;

		case 2:
		{
			std::cout<<"\n2753 - called by STOP stop";
		}
		break;

		case 3:
		{
			std::cout<<"\n2759 - called by STPC stop";
		}
		break;

		case 4:
		{
			std::cout<<"\n2765 - called by STPD stop";
		}
		break;
	}

	//std::cout<<"\n3531 Clearing Train Number:"<<Locoinformation[loco].TrainNumber<<" from Train section:"<<Locoinformation[loco].PSTrainsection_previous;

	LSLD->TSupdateSection(Locoinformation[loco].PSTrainsection_previous,Locoinformation[loco].TrainNumber,0,7);			// clear train number from Previous Train section

	DisplayLocoInformation(3,loco);

	int JC_PS_LS 		= Locoinformation[loco].PS;
	int JC_CS_LS 		= Locoinformation[loco].CS;
	int JC_NS_LS		= Locoinformation[loco].NS;
	
	//std::cout<<"\n2828 JC_CS_LS "<<JC_CS_LS;
	
	ClearRouteLS(loco,JC_CS_LS);

	std::cout<<"\n2785 Simulator number:"<<Locoinformation[loco].Simulatornumber;

	if (Locoinformation[loco].Simulatornumber > 0)
	{
		// route is running on the simulator so clear Physical section linked to Previous logical section.

		int PSection_PS_LS = LSLD->GetLS_PS1section(JC_PS_LS);

		//std::cout<<"\n2793 For simulator journey completion ->  PS_LS:"<<JC_PS_LS<<" PSection:"<<PSection_PS_LS;

		PSectionset(PSection_PS_LS, 0);

		LSLD->LogicalSectionTcodeRefresh();

		LSLD->LogicalDetectorupdateDetectorstatus();
	}

	int JC_lastjourney = Locoinformation[loco].Journey;

	int JC_Tcode	   = Locoinformation[loco].LocoDCCcode;

	int JC_Schedulerstatus = Locoinformation[loco].Schedulerunstatus;

	LSLD->UpdateTcode(JC_PS_LS,0);				// clear loco from previous section

	LSLD->UpdateTcode(JC_NS_LS,0);				// clear loco from next section

	LocodatarecordsInit(loco);					// clear locoinformtion record.

	Locoinformation[loco].Journey = JC_lastjourney;

	Locoinformation[loco].Journeystatus = 9;

	Locoinformation[loco].LocoDCCcode = JC_Tcode;

	Locoinformation[loco].Schedulerunstatus = JC_Schedulerstatus;

	std::cout<<"\n3582 - Journeycompleted processing executed for Loco "<<loco<<" on Journey "<<JC_lastjourney;
	
	char Journeymessage[GUIMESSAGELINELENGTH];

	bzero(Journeymessage,GUIMESSAGELINELENGTH);

	sprintf(Journeymessage,"B-Journey  %02d completed\n",Locoinformation[loco].Journey);

	char* Jnymsgptr = &Journeymessage[0];

	InsertGUItextlineMain(Jnymsgptr);

	DisplayLocoInformation(4,loco);

	std::cout<<"\n";

	return 0;
}



void LNweblocorouteprocess::Enabletrainscancmd(int Locoindx, int Traincode, int Lococode)
{
	// create a client to send the message 
	
	char Outmsg[30];
	
	char Msgread[255];
	
	bzero(Outmsg,30);
	sprintf(Outmsg,"C;ETCS;%02d;%02d;%04d;X",Lococode,Traincode,Locoindx);
	
	TCPprocessing* TCPP = new TCPprocessing();
	
	char LNRFidserveraddr[14] = {"192.168.1.231"};
	
	int  LNRFidControllerport = 8098;
	
	// int TCPprocessing::Clientrun(int Rteport, char& Messagereceived, char *outboundmsg, char *servernumber, int waitflag)
	
	TCPP->Clientrun(LNRFidControllerport,Msgread,Outmsg,LNRFidserveraddr,1);
	
	return;	
}

void LNweblocorouteprocess::Disabletrainscancmd(int Locoindx, int Traincode, int Lococode)
{
	
	
	
	
}

void LNweblocorouteprocess::DisplayLocoInformation(int location , int loco)
{
	printf("\n\n3012 == Display  Call %02d information for loco %02d Enabled %01d DCC %04d Slot %02d Journey %03d Rte %03d Journeystatus %01d Schedulerstatus %01d",\
			location,loco,Locoinformation[loco].Enabled,Locoinformation[loco].LocoDCCcode,Locoinformation[loco].Slotnumber,Locoinformation[loco].Journey,Locoinformation[loco].Route,Locoinformation[loco].Journeystatus,Locoinformation[loco].Schedulerunstatus);


}

void LNweblocorouteprocess::ClearRoute_AllLS(int loco, int StopLS)
{

	int currentTcode;

	//printf("\n2859 Clearing Loco %02d with Tcode %04d from Route LS , stopped in LS %03d",loco,Locoinformation[loco].LocoDCCcode, StopLS);

	int CheckLS;

	int Route = Locoinformation[loco].Route;

	for (int lrcc = 1; lrcc < Routelinesmax; lrcc++)
	{
		if (RouteData[Route][lrcc][1].RouteCS > 0)			// extract Current LS for the step in the route if it is > 0
		{
			CheckLS = RouteData[Route][lrcc][1].RouteCS;

			if (CheckLS != StopLS)
			{
				currentTcode = LSLD->GetLogicalSectionTcode(CheckLS);

				if (currentTcode == Locoinformation[loco].LocoDCCcode)
				{
					// clear Tcode

				LSLD->UpdateTcode(CheckLS,0);

				printf("\n2880 Tcode %04d cleared from LS %03d ",currentTcode, CheckLS);
				}
				else
				{


				}
			}
		}
	}
}


void LNweblocorouteprocess::ClearRouteLS(int loco, int StopLS)
{
	// clear all sections used by loco on journey just completed

	int currentTcode;

	//printf("\n2859 Clearing Loco %02d with Tcode %04d from Route LS , stopped in LS %03d",loco,Locoinformation[loco].LocoDCCcode, StopLS);

	int CheckLS;

	for (int lrcc = 1; lrcc < Routelinesmax; lrcc++)
	{
		if (Locoinformation[loco].Routesectionhistory[lrcc] > 0)
		{
			// fetch Tcode from section			

			CheckLS = Locoinformation[loco].Routesectionhistory[lrcc];
			
			if (CheckLS != StopLS)
			{
				currentTcode = LSLD->GetLogicalSectionTcode(CheckLS);
			
			//std::cout<<"\nchecking LS "<<lrcc<<" tcode found "<<CheckLS;

				if (currentTcode == Locoinformation[loco].LocoDCCcode)
				{
				// clear Tcode

				LSLD->UpdateTcode(CheckLS,0);

				printf("\n2883 Tcode %04d cleared from LS %03d ",currentTcode, CheckLS);
				}
			}
			else
			{
			//	printf("\n2935 Not clearing LS %03d as same as Current LS %03d",CheckLS,Locoinformation[loco].CS);
			//	printf("\n");
			}

		}

	}
}



