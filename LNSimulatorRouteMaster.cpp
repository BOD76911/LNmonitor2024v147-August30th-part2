/*
 * LNSimulatorRouteMaster.cpp
 *
 *  Created on: Sep 5, 2021
 *      Author: BrianRoot
 */

#include "LNSimulatorRouteMaster.h"


LNSimulatorRouteMaster::LNSimulatorRouteMaster() {
	// TODO Auto-generated constructor stub
}

LNSimulatorRouteMaster::~LNSimulatorRouteMaster() {
	// TODO Auto-generated destructor stub
}

void LNSimulatorRouteMaster::InitializeSimRteMasters()
{
	// Initialize the Simulator Route Master array

	/*
	struct SimRteMasters
	{
		int recRte;
		int recRtestep;
		int timervalue;	// Time applied to simulator cycles
		int TypeFlag1;	// 1 = physical section, 2 = clock cycles , 3 = detectors
		int Type1code;
		int Type1value;
		int TypeFlag2;  // 1 = physical section, 2 = clock cycles , 3 = detectors
		int Type2code;
		int Type2value;
		int TypeFlag3;  // 1 = physical section, 2 = clock cycles , 3 = detectors
		int Type3code;
		int Type3value;
	};

	SimRteMasters SimRouteMasterRec[MAXNUMSIMROUTES][MAXNUMSIMROUTESTEPS];
	*/

	int SimRtesc, SimRtesubsc;

	for (SimRtesc = 0; SimRtesc < MAXNUMSIMROUTES;SimRtesc++)
	{
		for (SimRtesubsc = 0; SimRtesubsc < MAXNUMSIMROUTESTEPS; SimRtesubsc++)
		{
			SimRouteMasterRec[SimRtesc][SimRtesubsc].recRte		= 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].recRtestep = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].timervalue	= 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag1	= 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1code  = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1value = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag2	= 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2code  = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2value = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag3	= 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3code  = 0;
			SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3value = 0;
		}
	}
}

int LNSimulatorRouteMaster::DisplaySimRteMasters(char* replymessage, int browserconsoleflag)
{
	int isc,ic,mcc,SimRtesc, SimRtesubsc;

	mcc = 0;

	char outputmessage[OUTPUTMSGBUFFERSIZE];

	if (browserconsoleflag == 0)
	{
		for (SimRtesc = 0; SimRtesc < MAXNUMSIMROUTES;SimRtesc++)
		{
			for (SimRtesubsc = 0; SimRtesubsc < MAXNUMSIMROUTESTEPS; SimRtesubsc++)
			{
				bzero(outputmessage,OUTPUTMSGBUFFERSIZE);
				sprintf(outputmessage,"\nRoute: %03d Step: %02d Timer: %02d Flag1: %i Code1: %i Value1: %i Flag2: %i Code2: %i Value2: %i Flag3: %i Code3: %i Value3: %i",\
						SimRouteMasterRec[SimRtesc][SimRtesubsc].recRte, SimRouteMasterRec[SimRtesc][SimRtesubsc].recRtestep, SimRouteMasterRec[SimRtesc][SimRtesubsc].timervalue, \
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag1, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1value, \
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag2,	SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2value,\
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag3, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3value);

				isc =  std::strlen(outputmessage);

				for (ic = 0;ic <isc;ic++)
				{
				*(replymessage + mcc) = outputmessage[ic];
				mcc++;
				}
			}
		}
		return mcc;
	}
	else
	{
		// browser feedback
		for (SimRtesc = 0; SimRtesc < MAXNUMSIMROUTES;SimRtesc++)
		{
			for (SimRtesubsc = 0; SimRtesubsc < MAXNUMSIMROUTESTEPS; SimRtesubsc++)
			{
				bzero(outputmessage,OUTPUTMSGBUFFERSIZE);
				sprintf(outputmessage,"<p><pre>Route: %03d Step: %02d Timer: %02d Flag1: %i Code1: %i Value1: %i Flag2: %i Code2: %i Value2: %i Flag3: %i Code3: %i Value3: %i</pre></p>",\
						SimRouteMasterRec[SimRtesc][SimRtesubsc].recRte, SimRouteMasterRec[SimRtesc][SimRtesubsc].recRtestep, SimRouteMasterRec[SimRtesc][SimRtesubsc].timervalue, \
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag1, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type1value, \
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag2,	SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type2value,\
						SimRouteMasterRec[SimRtesc][SimRtesubsc].TypeFlag3, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3code, SimRouteMasterRec[SimRtesc][SimRtesubsc].Type3value);

						isc =  std::strlen(outputmessage);

						for (ic = 0;ic <isc;ic++)
						{
						*(replymessage + mcc) = outputmessage[ic];
						mcc++;
						}
			}
		}
		return mcc;
	}

}

std::vector<int> LNSimulatorRouteMaster::GetStepdetail(int step)
{
	std::vector<int> Selectedstepdetail;

	LNSimulatorRoutelines LNSimrtelines; // = new LNSimulatorRoutelines();

	for (std::vector<LNSimulatorRoutelines>::iterator iSimRtestep = SimRtelines.begin(); iSimRtestep != SimRtelines.end(); ++iSimRtestep)
			{
				LNSimrtelines = *iSimRtestep;

				if (LNSimrtelines.Getstepnumber() == step)
				{
					Selectedstepdetail = LNSimrtelines.Getstepdetail();
					break;
				}
			}

	return Selectedstepdetail;
}

void LNSimulatorRouteMaster::SaveSimRoutesdescription(char Description[], char Autodescript[], int SimRtenumber, int SimRtedirection)
{
	int di = std::strlen(Description);

	int ic = 0;

	bzero(SimRtedescription,SIMDESCRIPTIONLEN);

	for (ic = 0;ic < di;ic++)
	{
		SimRtedescription[ic] = Description[ic];
	}

	bzero(Autodescription,SIMDESCRIPTIONLEN);

	for (ic = 0;ic < di;ic++)
	{
		Autodescription[ic] = Autodescript[ic];
	}

	SimulatedRoutenumber = SimRtenumber;

	SimulatedRoutedirection = SimRtedirection;

}

int LNSimulatorRouteMaster::GetCountofRtelines()
{
	return SimRtelines.size();
}

int LNSimulatorRouteMaster::GetRoutenumber()
{
	return SimulatedRoutenumber;
}

void LNSimulatorRouteMaster::SaveRouteline(LNSimulatorRoutelines LNSimRteline)
{
	SimRtelines.push_back(LNSimRteline);
	NumberofRtelinesstored++;
}

void LNSimulatorRouteMaster::DisplaySimRoute(char* replymessage)
{
	int isc,ic,mcc;

	mcc = 0;

	char TestOutboundmessage[OUTPUTMSGBUFFERSIZE];

	bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	char* TestOutboundmessageptr = &TestOutboundmessage[0];

	sprintf(TestOutboundmessage,"\nSimulated Route: %03d : Route description: %s : Short description:",SimulatedRoutenumber, Autodescription);

	isc =  std::strlen(TestOutboundmessage);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = TestOutboundmessage[ic];
		mcc++;
	}

	int di = std::strlen(SimRtedescription);

	for (ic = 0;ic <di;ic++)
	{
		*(replymessage + mcc) = SimRtedescription[ic];
		mcc++;
	}

	LNSimulatorRoutelines LNSimrtelines; // = new LNSimulatorRoutelines();

	bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	if (SimRtelines.size() > 0)
	{
		for (std::vector<LNSimulatorRoutelines>::iterator iSimRtestep = SimRtelines.begin(); iSimRtestep != SimRtelines.end(); ++iSimRtestep)
		{
			LNSimrtelines = *iSimRtestep;

			bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

			LNSimrtelines.DisplaySimRtestep(TestOutboundmessageptr);

			isc =  std::strlen(TestOutboundmessage);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = TestOutboundmessage[ic];
				mcc++;
			}
		}
	}
	else
	{
		char Nullreturnmsg[64];

		bzero(Nullreturnmsg,64);

		sprintf(Nullreturnmsg,"\nNo Simulator Route steps found\n");

		di = std::strlen(SimRtedescription);

		for (ic = 0;ic <di;ic++)
			{
				*(replymessage + mcc) = SimRtedescription[ic];
				mcc++;
			}
	}
}
