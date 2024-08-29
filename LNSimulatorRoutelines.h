/*
 * LNSimulatorRoutelines.h
 *
 *  Created on: Sep 5, 2021
 *      Author: BrianRoot
 */

#ifndef LNSIMULATORROUTELINES_H_
#define LNSIMULATORROUTELINES_H_

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


class LNSimulatorRoutelines {
public:
	LNSimulatorRoutelines();
	virtual ~LNSimulatorRoutelines();

	int DisplaySimRtestep(char* replymessage);

	std::vector<int> Getstepdetail();

	int Getstepnumber();

	void StoreSimRteline(int Rte,int step,int timerval, int TFflag1, int TF1code, int TF1value,\
			int TFflag2, int TF2code, int TF2value, int TFflag3, int TF3code, int TF3value);

private:

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

#endif /* LNSIMULATORROUTELINES_H_ */
