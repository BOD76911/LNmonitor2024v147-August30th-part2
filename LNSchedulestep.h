/*
 * LNSchedulestep.h
 *
 *  Created on: Sep 12, 2021
 *      Author: BrianRoot
 */

#ifndef LNSCHEDULESTEP_H_
#define LNSCHEDULESTEP_H_

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

class LNSchedulestep
{
public:
	LNSchedulestep();

	virtual ~LNSchedulestep();

	void SaveStepdetail(int schedule, int step, int Trainjourneynumber, int startclickcnt, int StarttimeHH, int StarttimeMM, int StarttimeSS);

	int DisplaySchedstep(char* replymessage);

	void Initializestep();

	int checkstartconditions(int tickcounter);

	int GetTrainjourney();

	int GetSchedStepnumber();

private:

	unsigned int Schedulenumber;
	unsigned int Schedulestep;
	unsigned int Journey;
	unsigned int Startclicktarget;
	unsigned int StartHH;
	unsigned int StartMM;
	unsigned int StartSS;
};

#endif /* LNSCHEDULESTEP_H_ */
