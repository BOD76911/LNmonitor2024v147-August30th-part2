/*
 * LNRtesteprecord.h
 *
 *  Created on: Aug 19, 2021
 *      Author: BrianRoot
 *
 *  Class to store the individual lines created when the routebuild process is executed
 *
 */

#ifndef LNRTELINERECORD_H_
#define LNRTELINERECORD_H_

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

#include "LNRteDevicereferencedata.h"
#include "LNRteLogicalsectionData.h"
#include "LNRteCMDlookuptable.h"
#include "LNRteLogicalsectionGroup.h"

class LNRtelinerecord {
public:
	LNRtelinerecord();
	virtual ~LNRtelinerecord();

	int Displaystep(LNRteCMDlookuptable* CMDtable, char *replymessage);

	void Buildstep(int* Storeflag,int* Rtedirection,LNRteLogicalsectionGroup* LNRteLSG, int Route, int Stepnum, int Substep, \
			std::vector<int> RteLSsteps, int Stoptypeflag, int Stopflagdevicenumber, int StopdeviceLS, int Stopflagdevicetarget,\
			int Uncouplernum, int Uncouplerdevice, int UncouplerdeviceLS, int Uncouplerdevicetarget, int Traincouplerflag,\
			int Sectionswitchoverflag, int SectionswitchoverfromLS, int SectionswitchovertoLS,\
			int counterbranchcheckstep, int counterincrementstep,int PlannedTypeofloop, int Lcounter,\    
			int StartloopLS,int EndloopLS,int Branchsteptarget, int Branchsubsteptarget);

	std::vector<int> Rtelinedata();

private:


#define DISPSHORTDESCLENGTH 	12
#define DISPLONGDESCLENGTH		128

unsigned int Routenumber;
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
};



#endif /* LNRTELINERECORD_H_ */
