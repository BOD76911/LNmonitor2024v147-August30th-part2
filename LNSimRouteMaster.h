/*
 * LNSimRouteMaster.h
 *
 *  Created on: Oct 30, 2021
 *      Author: BrianRoot
 *
 *      Stores the Simulator records for each Route
 *
 */

#ifndef LNSIMROUTEMASTER_H_
#define LNSIMROUTEMASTER_H_

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

class LNSimRouteMaster
{
public:
	LNSimRouteMaster();

	virtual ~LNSimRouteMaster();

	void InitializeSIMRoutelinedetailsarray();

	void StoreSIMRoutelinedetailinarray(int Rtesc, std::vector<int> ListPsections, std::vector<int> RteDetectorcounters);

	void StoreSIMRoutesummarydetailsarray(int Rtenum,int Rtedirection,int Numoflines,int StartLSpsection,\
			int Stopdetectordevice,int StopdetectorPS,int Stopdetectordevicetrigcnt,\
			int Uncoupler,int Uncplrtrigdev,int UncplrtrigdevPS,int Uncplrtrigdevtarget,char Autodesc[], char Rtedesc[]);

	std::vector<int> Getstepdetail(int Rtenum, int Rtestep);

	int DisplaySIMRoutelinedetailsarray(char* replymessage, int browserconsoleflag);

	int CheckRtedevicematch(int CheckPS, int& LSdevice, int& LSdevicetarget, int &LSdevicePsectionref, std::vector<int>  RteDetectorcounters);

	void IncrementSimRtestoredcounter();

	int GetselectedSimRoutestepcount(int Rtenum);

	void StorenumberofSimRoutelines(int Rte, int Numberoflines);

	int DisplayTotalNumberofSimrecordsloaded();

private:

	int NumberofSimRtestored;

	int TotalSimrecordswritten;

	struct SimRoutedatasummary
	{
		int SimulatedRoutenumber;
		int SimulatedRoutedirection;
		int deleteflag = 0;
		int NumberofRtelinesstored;
		char SimRtedescription[SIMDESCRIPTIONLEN];
		char Autodescription[SIMDESCRIPTIONLEN];
		int StartLSpsection;
		int Stopdetectordevice;
		int StopdetectordevicePsection;
		int Stopdetectordevicetriggercount;
		int Uncouplernumber;
		int Uncouplertrigdevice;
		int UncouplertrigdevicePsection;
		int Uncouplertrigdevicetarget;
	};

	SimRoutedatasummary SimRouteMasterSummary[MAXNUMRTESTORED];


	struct SimRoutedatalines
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

	SimRoutedatalines SimRouteMasterData[MAXNUMRTESTORED][MAXNUMSIMROUTESTEPS];
};

#endif /* LNSIMROUTEMASTER_H_ */
