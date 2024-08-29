/*
 * LNSimRouteMaster.cpp
 *
 *  Created on: Oct 30, 2021
 *      Author: BrianRoot
 */

#include "LNSimRouteMaster.h"

LNSimRouteMaster::LNSimRouteMaster() {
	// TODO Auto-generated constructor stub

}

LNSimRouteMaster::~LNSimRouteMaster() {
	// TODO Auto-generated destructor stub
}


void LNSimRouteMaster::InitializeSIMRoutelinedetailsarray()
{
	int SimRte, SimRtestep;

	NumberofSimRtestored = 0;

	TotalSimrecordswritten = 0;

	//std::cout<<"\n26---> Simulator table initialized...\n";

	for (SimRte = 0; SimRte < MAXNUMRTESTORED; SimRte++)
	{
		// Initialize summary details

		SimRouteMasterSummary[SimRte].SimulatedRoutenumber				= 0;
		SimRouteMasterSummary[SimRte].SimulatedRoutedirection			= 0;
		SimRouteMasterSummary[SimRte].deleteflag 						= 0;
		SimRouteMasterSummary[SimRte].NumberofRtelinesstored			= 0;

		SimRouteMasterSummary[SimRte].StartLSpsection					= 0;
		SimRouteMasterSummary[SimRte].Stopdetectordevice				= 0;
		SimRouteMasterSummary[SimRte].StopdetectordevicePsection 		= 0;
		SimRouteMasterSummary[SimRte].Stopdetectordevicetriggercount	= 0;

		SimRouteMasterSummary[SimRte].Uncouplernumber					= 0;
		SimRouteMasterSummary[SimRte].Uncouplertrigdevice				= 0;
		SimRouteMasterSummary[SimRte].UncouplertrigdevicePsection		= 0;
		SimRouteMasterSummary[SimRte].Uncouplertrigdevicetarget			= 0;


		for (int cc = 0;cc < SIMROUTEDESCRIPTIONLENGTH;cc++)
		{
			SimRouteMasterSummary[SimRte].SimRtedescription[cc] = ' ';
			SimRouteMasterSummary[SimRte].Autodescription[cc]	= ' ';
		}

		SimRouteMasterSummary[SimRte].SimRtedescription[SIMROUTEDESCRIPTIONLENGTH - 1] = '\0';
		SimRouteMasterSummary[SimRte].Autodescription[SIMROUTEDESCRIPTIONLENGTH - 1] = '\0';

		// Initialize individual lines

		for (SimRtestep = 0; SimRtestep < MAXNUMSIMROUTESTEPS; SimRtestep++)
		{
		/*
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
			*/

			SimRouteMasterData[SimRte][SimRtestep].recRte 		= 0;
			SimRouteMasterData[SimRte][SimRtestep].recRtestep	= 0;
			SimRouteMasterData[SimRte][SimRtestep].timervalue	= 0;
			SimRouteMasterData[SimRte][SimRtestep].TypeFlag1	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type1code	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type1value	= 0;
			SimRouteMasterData[SimRte][SimRtestep].TypeFlag2	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type2code	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type2value	= 0;
			SimRouteMasterData[SimRte][SimRtestep].TypeFlag3	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type3code	= 0;
			SimRouteMasterData[SimRte][SimRtestep].Type3value	= 0;
		}
	}
}

void LNSimRouteMaster::StoreSIMRoutesummarydetailsarray(int SimRte,int Rtedirection,int Numoflines,int StartLSpsection,\
		int Stopdetectordevice,int StopdetectorPS,int Stopdetectordevicetrigcnt,\
		int Uncoupler,int Uncplrtrigdev,int UncplrtrigdevPS,int Uncplrtrigdevtarget,char Autodesc[], char Rtedesc[])
{
		if (SimRte > MAXNUMSIMROUTES)
		{
			std::cout<<"\n\n\n ERROR -----> Simulated Route number "<<SimRte<<" is greater than table size \n\n";

		}
		else
		{
		//std::cout<<"\n95 Storing Sim Route summary details\n";

		SimRouteMasterSummary[SimRte].SimulatedRoutenumber				= SimRte;
		SimRouteMasterSummary[SimRte].SimulatedRoutedirection			= Rtedirection;
		SimRouteMasterSummary[SimRte].deleteflag 						= 0;
		SimRouteMasterSummary[SimRte].NumberofRtelinesstored			= Numoflines;

		SimRouteMasterSummary[SimRte].StartLSpsection					= StartLSpsection;
		SimRouteMasterSummary[SimRte].Stopdetectordevice				= Stopdetectordevice;
		SimRouteMasterSummary[SimRte].StopdetectordevicePsection 		= StopdetectorPS;
		SimRouteMasterSummary[SimRte].Stopdetectordevicetriggercount	= Stopdetectordevicetrigcnt;

		SimRouteMasterSummary[SimRte].Uncouplernumber					= Uncoupler;
		SimRouteMasterSummary[SimRte].Uncouplertrigdevice				= Uncplrtrigdev;
		SimRouteMasterSummary[SimRte].UncouplertrigdevicePsection		= UncplrtrigdevPS;
		SimRouteMasterSummary[SimRte].Uncouplertrigdevicetarget			= Uncplrtrigdevtarget;

		int cc = 0;

		for (cc = 0;cc < std::strlen(Autodesc);cc++)
		{
			SimRouteMasterSummary[SimRte].Autodescription[cc] = Autodesc[cc];
		}

		for (cc = 0;cc < std::strlen(Autodesc);cc++)
		{
			SimRouteMasterSummary[SimRte].SimRtedescription[cc] = Rtedesc[cc];
		}

		SimRouteMasterSummary[SimRte].SimRtedescription[SIMROUTEDESCRIPTIONLENGTH - 1] = '\0';
		SimRouteMasterSummary[SimRte].Autodescription[SIMROUTEDESCRIPTIONLENGTH - 1] = '\0';

		NumberofSimRtestored++;
		}

}

std::vector<int> LNSimRouteMaster::Getstepdetail(int Rtenum, int Rtestep)
{
	std::vector<int> Stepdetail;

		/*
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
		*/

		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].recRte);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].timervalue);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].TypeFlag1);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type1code);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type1value);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].TypeFlag2);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type2code);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type2value);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].TypeFlag3);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type3code);
		Stepdetail.push_back(SimRouteMasterData[Rtenum][Rtestep].Type3value);

		return Stepdetail;


}

int LNSimRouteMaster::CheckRtedevicematch(int CheckPS, int& LSdevice, int& LSdevicetarget, int &LSdevicePsectionref, std::vector<int>  RteDetectorcounters)
{
	//std::cout<<"\nChecking device match in section:"<<CheckPS;

		// each device record has four entries : 1 - Physical section where the device is locatedS,
		//										 2 - the device number,
		//										 3 - device counter target,
		//										 4 - the physical section used by the Looconet processor to log the deice has been activated

		//std::cout<<"\n272... CheckRtedevicematch - Number of device records to check:"<<(RteDetectorcounters.size()/3);

		int devicechecktotal = RteDetectorcounters.size()/3;

		std::vector<int>::iterator RtedeviceITM = RteDetectorcounters.begin();


		int dvcc = 0;

		for (dvcc = 0; dvcc <= 1;dvcc++)
		{
			if (*RtedeviceITM == CheckPS)
			{
			 LSdevice = *(RtedeviceITM + 1);
			 LSdevicetarget = *(RtedeviceITM + 2);
			 LSdevicePsectionref = *(RtedeviceITM + 3);
			 return CheckPS;
			}
			else
			{
			//std::cout<<"\nNo match found at:"<<CheckPS;
			}
			RtedeviceITM++;
			RtedeviceITM++;
			RtedeviceITM++;
			RtedeviceITM++; // move to next device record
		}

		//std::cout<<"\nChecked:"<<devicechecktotal<<" records with no match found at:"<<CheckPS;

		return 0;
}

void  LNSimRouteMaster::StorenumberofSimRoutelines(int Rte, int Numberoflines)
{
	SimRouteMasterSummary[Rte].NumberofRtelinesstored = Numberoflines;

}

int LNSimRouteMaster::GetselectedSimRoutestepcount(int Rtenum)
{
	return SimRouteMasterSummary[Rtenum].NumberofRtelinesstored;
}

void LNSimRouteMaster::StoreSIMRoutelinedetailinarray(int Rtesc, std::vector<int> ListPsections, std::vector<int> RteDetectorcounters)
{
	// Build up and store the route line details for the Simulator Route records .


	if (Rtesc > MAXNUMSIMROUTES)
	{
				std::cout<<"\n\n\n ERROR -----> Simulated Route number "<<Rtesc<<" is greater than table size \n\n";

				return;
	}

		int LSdevice = 0;

		int LSdevicetarget = 0;

		int LSdevicePsectionref = 0;

		int sp = 0; 	/// sections processed counter

		int SimRtenum;

		int Rstep = 0;

		int Timervalue = 0;

		int PSflagstr = 0;

		int P1strchar = 0;

		int Flag1 = 0;

		int Sectioncode1 = 0;

		int Code1F = 0;

		int P2str = 0;

		int P2strchar = 0;

		int Flag2 = 0;

		int Sectioncode2 = 0;

		int Code2F = 0;

		int P3str = 0;

		int P3strchar = 0;

		int Flag3 = 0;

		int Sectioncode3 = 0;

		int Code3F = 0;

		int NumberofPsectionstoprocess = ListPsections.size();

		int Simlinecount = 0;

		int Recordcycle = 1; // 1 = current section is CS, 2 = exiting PS, 3 = crossing into NS, 4 NS->CS

		for (std::vector<int>::iterator iRoutePsections = ListPsections.begin(); iRoutePsections != ListPsections.end(); ++iRoutePsections)
		{

			if (sp == 0) // building first record
			{
				Flag1 = 0;  		// set Flag 1 to 0 to denote blank section setting
				Sectioncode1 = 0; 	// set first section as blank
				Code1F = 0;			//	set as unoccupied

				Flag2 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode2 = *iRoutePsections;
				Code2F = 1; 		// set as occupied

				Flag3 = 2;

				Rstep++;

				Recordcycle = 2;

				Timervalue = 3;

							// StoreSimRteline(int Rte,int step,int timerval, int TFflag1, int TF1code, int TF1value,\
							int TFflag2, int TF2code, int TF2value, int TFflag3, int TF3code, int TF3value)


				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;
			}


			if ((sp > 0) && (sp != NumberofPsectionstoprocess))
			{
				Flag1 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode1 = *(iRoutePsections -1);
				Code1F = 1;

				Flag2 = 1;
				Sectioncode2 = *iRoutePsections;
				Code2F = 1;

				Rstep++;

				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;

				Flag1 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode1 = *(iRoutePsections -1);
				Code1F = 0;			// clear previous section

				Flag2 = 1;
				Sectioncode2 = *iRoutePsections;
				Code2F = 1;

				Timervalue = 3;

				Rstep++;


				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;

				if (RteDetectorcounters.size() > 0)
				{
					//std::cout<<"\n392 Printing detector details for SIM build of Route:"<<Rtesc;

					for(std::vector<int>::iterator RtedeviceITM = RteDetectorcounters.begin();RtedeviceITM != RteDetectorcounters.end();++RtedeviceITM)
					{
					//	std::cout<<" "<< *RtedeviceITM <<" \n";

					}

					//std::cout<<"\nPrinting data extract details....";

								//for (std::vector<int>::iterator DTLM = CheckPLmapping.begin(); DTLM != CheckPLmapping.end(); ++DTLM)
								//			  std::cout << ' ' << *DTLM;
								//			  std::cout << '\n';





					int CheckRteRC = CheckRtedevicematch(Sectioncode2, LSdevice,LSdevicetarget,LSdevicePsectionref,RteDetectorcounters);
				//	std::cout<<"\n393 RouteMaster Detector check RC:"<<CheckRteRC<<" LSdevice:"<<LSdevice<<" Target:"<<LSdevicetarget<<" LSpsectionref:"<<LSdevicePsectionref<<"\n";

						if (CheckRteRC > 0)
						{
						// create a device trigger record for the simulator - it creates a PSection update record for the physical sections that is linked with the device

							Flag1 = 3;			// set Flag 1 to 3 to denote device setting
							Code1F = 0;
							Flag2 = LSdevice;			// set Flag 2 to 3 to denote device setting
							Code2F = 1;
							Flag3 = LSdevicePsectionref;
							Rstep++;

							Timervalue = 1;

							SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
							SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
							SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
							SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
							SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode2;
							SimRouteMasterData[Rtesc][Rstep].Type1value = Code2F;
							SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
							SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
							SimRouteMasterData[Rtesc][Rstep].Type2value = LSdevice;
							SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
							SimRouteMasterData[Rtesc][Rstep].Type3code = LSdevicetarget;			// activate device trigger via Psection
							SimRouteMasterData[Rtesc][Rstep].Type3value = 0;

							Simlinecount++;

							Rstep++;

							SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
							SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
							SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
							SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
							SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode2;
							SimRouteMasterData[Rtesc][Rstep].Type1value = Code2F;
							SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
							SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;				// reset the physical device trigger
							SimRouteMasterData[Rtesc][Rstep].Type2value = LSdevice;
							SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
							SimRouteMasterData[Rtesc][Rstep].Type3code = 0;
							SimRouteMasterData[Rtesc][Rstep].Type3value = 0;

							Simlinecount++;
						}
					}
				else
				{
				//	std::cout<<"\n443 No route detectors found for Route number:"<<Rtesc;

				}


				Flag1 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode1 = *iRoutePsections;
				Code1F = 1;

				Flag2 = 1;
				Sectioncode2 = *iRoutePsections;
				Code2F = 1;

				Timervalue = 3;

				Rstep++;

				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;

			}

			if (sp == NumberofPsectionstoprocess)		// last section to simulate
			{
				Flag1 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode1 = *(iRoutePsections -1);
				Code1F = 1;

				Flag2 = 1;
				Sectioncode2 = *iRoutePsections;
				Code2F = 1;

				Rstep++;

				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;

				Flag1 = 1;			// set Flag 1 to 1 to denote Psection setting
				Sectioncode1 = *(iRoutePsections -1);
				Code1F = 0;			// clear previous section

				Flag2 = 1;
				Sectioncode2 = *iRoutePsections;
				Code2F = 1;

				Timervalue = 3;

				Rstep++;

				SimRouteMasterData[Rtesc][Rstep].recRte = Rtesc;						// Route number
				SimRouteMasterData[Rtesc][Rstep].recRtestep = Rstep;					// recRtestep;
				SimRouteMasterData[Rtesc][Rstep].timervalue = Timervalue;				// Time applied to simulator cycles
				SimRouteMasterData[Rtesc][Rstep].TypeFlag1 = Flag1;
				SimRouteMasterData[Rtesc][Rstep].Type1code = Sectioncode1;
				SimRouteMasterData[Rtesc][Rstep].Type1value = Code1F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag2 = Flag2;
				SimRouteMasterData[Rtesc][Rstep].Type2code = Sectioncode2;
				SimRouteMasterData[Rtesc][Rstep].Type2value = Code2F;
				SimRouteMasterData[Rtesc][Rstep].TypeFlag3 = Flag3;
				SimRouteMasterData[Rtesc][Rstep].Type3code = Sectioncode3;
				SimRouteMasterData[Rtesc][Rstep].Type3value = Code3F;

				Simlinecount++;

				if (RteDetectorcounters.size() > 0)
				{
					int CheckRteRC = CheckRtedevicematch(Sectioncode2,LSdevice,LSdevicetarget,LSdevicePsectionref, RteDetectorcounters);
					std::cout<<"\n202 Detector check RC:"<<CheckRteRC<<" LSdevice:"<<LSdevice<<" Target:"<<LSdevicetarget<<"\n";

				}
			}

		sp++;

		// ==== end of iteration
		}

		StorenumberofSimRoutelines(Rtesc,Simlinecount);

		TotalSimrecordswritten = TotalSimrecordswritten + Simlinecount;
}

int LNSimRouteMaster::DisplayTotalNumberofSimrecordsloaded()
{
	std::cout<<"\n\nTotal number of Simulator route records loaded = "<<TotalSimrecordswritten;

	return TotalSimrecordswritten;

}

int LNSimRouteMaster::DisplaySIMRoutelinedetailsarray(char* replymessage, int browserconsoleflag)
{
	int isc,ic,scc,mcc;

	int SimRtesc 			= 0;
	int SimRtestepsc 		= 0;

	char Simoutputrec[OUTPUTRECSIZE * 4];

	std::cout<<"\nDisplay Simulator Route data - Number of Simulator Routes loaded:"<<NumberofSimRtestored<<"\n";

	mcc = 0;

	if (browserconsoleflag == 0)
	{
		// console response

		 if (NumberofSimRtestored > 0)
		 {
			 // Simulator record summary data

			 for (SimRtesc = 1;SimRtesc < MAXNUMRTESTORED;SimRtesc++)
			 {
				 if (SimRouteMasterSummary[SimRtesc].StartLSpsection > 0)
				 {
					 bzero(Simoutputrec,OUTPUTRECSIZE * 4);

				 	 sprintf(Simoutputrec,"\n\nRte:%03d Rtedirection: %01d Stored SimLines:%03d StartLSpsection:%03d StopDevice:%02d Stopdevicetarget: %02d StopdevicePsection: %03d Uncoupler: %02d Uncoupler Device: %02d Uncplrdevtrig: %02d %s %s\n",\
				 			 SimRouteMasterSummary[SimRtesc].SimulatedRoutenumber,SimRouteMasterSummary[SimRtesc].SimulatedRoutedirection,SimRouteMasterSummary[SimRtesc].NumberofRtelinesstored,\
							 SimRouteMasterSummary[SimRtesc].StartLSpsection,SimRouteMasterSummary[SimRtesc].Stopdetectordevice,SimRouteMasterSummary[SimRtesc].Stopdetectordevicetriggercount,\
							 SimRouteMasterSummary[SimRtesc].StopdetectordevicePsection,SimRouteMasterSummary[SimRtesc].Uncouplernumber,SimRouteMasterSummary[SimRtesc].Uncouplertrigdevice,\
							 SimRouteMasterSummary[SimRtesc].Uncouplertrigdevicetarget,SimRouteMasterSummary[SimRtesc].Autodescription,SimRouteMasterSummary[SimRtesc].SimRtedescription);

				 	isc =  std::strlen(Simoutputrec);

				 	for (ic = 0;ic <isc;ic++)
				 	{
				 		*(replymessage + mcc) = Simoutputrec[ic];
				 		mcc++;
				 	}

				 	for (SimRtestepsc = 1;SimRtestepsc < MAXNUMSIMROUTESTEPS;SimRtestepsc++)
				 	{
				 		bzero(Simoutputrec,OUTPUTRECSIZE * 4);

				 		if (SimRouteMasterData[SimRtesc][SimRtestepsc].recRte > 0)
				 		{

				 			sprintf(Simoutputrec,"\nRte:%03d Step:%02d Timer:%02d T1flag:%01d T1code:%01d T1value:%01d T2flag:%01d T2code:%01d T2value:%01d T3flag:%01d T3code:%01d T3value:%01d",
				 					SimRouteMasterData[SimRtesc][SimRtestepsc].recRte,SimRouteMasterData[SimRtesc][SimRtestepsc].recRtestep,SimRouteMasterData[SimRtesc][SimRtestepsc].timervalue,\
									SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag1,SimRouteMasterData[SimRtesc][SimRtestepsc].Type1code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type1value,\
									SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag2,SimRouteMasterData[SimRtesc][SimRtestepsc].Type2code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type2value,\
									SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag3,SimRouteMasterData[SimRtesc][SimRtestepsc].Type3code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type3value);

				 					isc =  std::strlen(Simoutputrec);

				 					for (ic = 0;ic <isc;ic++)
				 					{
				 						*(replymessage + mcc) = Simoutputrec[ic];
				 						mcc++;
				 					}
				 		}
				 	}
				 }
			 }
		 }
		 else
		 {
			 bzero(Simoutputrec,OUTPUTRECSIZE * 4);
			 sprintf(Simoutputrec,"\n.........No Simulator Route Master data details stored  \n");
			 isc =  std::strlen(Simoutputrec);

			 for (ic = 0;ic <isc;ic++)
			 {
			 	*(replymessage + mcc) = Simoutputrec[ic];
			 	mcc++;
			 }
		 }


		*(replymessage + mcc) = '\n';

		//std::cout<<"\n125 Size of Simulator Route display:"<<mcc;

		return mcc;
	}
	else
	{
		// browser response

		if (NumberofSimRtestored > 0)
				 {
					 // Simulator record summary data

					 for (SimRtesc = 1;SimRtesc < MAXNUMRTESTORED;SimRtesc++)
					 {
						 if (SimRouteMasterSummary[SimRtesc].StartLSpsection > 0)
						 {
							 bzero(Simoutputrec,OUTPUTRECSIZE * 4);

						 	 sprintf(Simoutputrec,"\n\nRte:%03d Rtedirection: %01d Stored SimLines:%03d StartLSpsection:%03d StopDevice:%02d Stopdevicetarget: %02d StopdevicePsection: %03d Uncoupler: %02d Uncoupler Device: %02d Uncplrdevtrig: %02d\n",\
						 			 SimRouteMasterSummary[SimRtesc].SimulatedRoutenumber,SimRouteMasterSummary[SimRtesc].SimulatedRoutedirection,SimRouteMasterSummary[SimRtesc].NumberofRtelinesstored,\
									 SimRouteMasterSummary[SimRtesc].StartLSpsection,SimRouteMasterSummary[SimRtesc].Stopdetectordevice,SimRouteMasterSummary[SimRtesc].Stopdetectordevicetriggercount,\
									 SimRouteMasterSummary[SimRtesc].StopdetectordevicePsection,SimRouteMasterSummary[SimRtesc].Uncouplernumber,SimRouteMasterSummary[SimRtesc].Uncouplertrigdevice,\
									 SimRouteMasterSummary[SimRtesc].Uncouplertrigdevicetarget);

						 	isc =  std::strlen(Simoutputrec);

						 	for (ic = 0;ic <isc;ic++)
						 	{
						 		*(replymessage + mcc) = Simoutputrec[ic];
						 		mcc++;
						 	}

						 	for (SimRtestepsc = 1;SimRtestepsc < MAXNUMSIMROUTESTEPS;SimRtestepsc++)
						 	{
						 		bzero(Simoutputrec,OUTPUTRECSIZE * 4);

						 		sprintf(Simoutputrec,"\nRte:%03d Step:%02d Timer:%02d T1flag:%01d T1code:%01d T1value:%01d T2flag:%01d T2code:%01d T2value:%01d T3flag:%01d T3code:%01d T3value:%01d",
						 		SimRouteMasterData[SimRtesc][SimRtestepsc].recRte,SimRouteMasterData[SimRtesc][SimRtestepsc].recRtestep,SimRouteMasterData[SimRtesc][SimRtestepsc].timervalue,\
						 		SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag1,SimRouteMasterData[SimRtesc][SimRtestepsc].Type1code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type1value,\
						 		SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag2,SimRouteMasterData[SimRtesc][SimRtestepsc].Type2code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type2value,\
						 		SimRouteMasterData[SimRtesc][SimRtestepsc].TypeFlag3,SimRouteMasterData[SimRtesc][SimRtestepsc].Type3code,SimRouteMasterData[SimRtesc][SimRtestepsc].Type3value);

						 		isc =  std::strlen(Simoutputrec);

						 		for (ic = 0;ic <isc;ic++)
						 		{
						 			*(replymessage + mcc) = Simoutputrec[ic];
						 			mcc++;
						 		}
						 	}
						 }
					 }
				 }
				 else
				 {
					 bzero(Simoutputrec,OUTPUTRECSIZE * 4);
					 sprintf(Simoutputrec,"\n.........No Simulator Route Master data details stored  \n");
					 isc =  std::strlen(Simoutputrec);

					 for (ic = 0;ic <isc;ic++)
					 {
					 	*(replymessage + mcc) = Simoutputrec[ic];
					 	mcc++;
					 }
				 }


				*(replymessage + mcc) = '\n';

				//std::cout<<"\n125 Size of Simulator Route display:"<<mcc;

				return mcc;
	}

	return mcc;
}
