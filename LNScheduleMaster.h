/*
 * LNScheduleMaster.h
 *
 *  Created on: Sep 12, 2021
 *      Author: BrianRoot
 */

#ifndef LNSCHEDULEMASTER_H_
#define LNSCHEDULEMASTER_H_



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
#include "LNSchedulestep.h"
#include "LNCLidataentry.h"

class LNScheduleMaster
{
public:
	LNScheduleMaster();
	virtual ~LNScheduleMaster();

	int DisplaySchedule(char* replymessage);

	int BuildnewSchedule(int schednum);

	int BuildnewSchedulefromfile(int Schednum,int stepnum, int stepjourney,int StHH, int StMM, int StSS, int StTicks);

	int GetExecutionstatus();

	int Getcurrentexecutionstep();

	void Setexecutionstatus(int status);

	int GetSchedulenumber();

	void Setcurrentexecutionstep(int step);

	int Checkstartreadiness(int &Trainjourney,int &Stepready,int &TotalstepsinSchedule, int scheduleticks);

	LNSchedulestep* LinktoselectedSchedulestep(int &schedstepfound, int schedstepnum);

private:

	int Schedulenumber;

	int NumberofstepsinSchedule;

	int Executionstatus;

	int ExecutionClickcounter;

	int Currentexecutionstep;

	char AutoSimdescription[LNSCHEDULEDESCRIPTIONLEN];

	char Scheduledescription[LNSCHEDULEDESCRIPTIONLEN];

	std::vector <LNSchedulestep> ScheduleMasterrecord;

};

#endif /* LNSCHEDULEMASTER_H_ */
