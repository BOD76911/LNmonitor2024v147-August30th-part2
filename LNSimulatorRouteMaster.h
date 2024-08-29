/*
 * LNSimulatorRouteMaster.h
 *
 *  Created on: Sep 5, 2021
 *      Author: BrianRoot
 */

#ifndef LNSIMULATORROUTEMASTER_H_
#define LNSIMULATORROUTEMASTER_H_

#include "LNwebconfig.h"
#include "LNSimulatorRoutelines.h"

class LNSimulatorRouteMaster {
public:
	LNSimulatorRouteMaster();
	virtual ~LNSimulatorRouteMaster();

	void SaveSimRoutesdescription(char Description[], char Autodescript[], int SimRtenumber, int SimRtedirection);

	void GetSimRoutedescription(char* replymessage);

	void SaveRouteline(LNSimulatorRoutelines LNSimRteline);

	void DisplaySimRoute(char* replymessage);

	int Getdeleteflag();

	int UpdateLinecount();

	int GetRoutenumber();

	int GetCountofRtelines();

	void InitializeSimRteMasters();

	std::vector<int> GetStepdetail(int step);

	int DisplaySimRteMasters(char* replymessage, int browserconsoleflag);

private:

	int SimulatedRoutenumber;

	int SimulatedRoutedirection;

	int deleteflag = 0;

	int NumberofRtelinesstored;

	char SimRtedescription[SIMDESCRIPTIONLEN];

	char Autodescription[SIMDESCRIPTIONLEN];

	std::vector <LNSimulatorRoutelines> SimRtelines;

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
};

#endif /* LNSIMULATORROUTEMASTER_H_ */
