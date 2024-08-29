/*
 * LNRteRoutemaster.h
 *
 *  Created on: Aug 19, 2021
 *      Author: BrianRoot
 */

#ifndef LNRTEROUTEMASTER_H_
#define LNRTEROUTEMASTER_H_

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

#include "LNRtelinerecord.h"
#include "LNRteCMDlookuptable.h"
#include "LNweblocorouteprocess.h"
#include "LNwebmonSimulator.h"
#include "LNSimulatorRouteMaster.h"
#include "LNRouteSchedMaster.h"


class LNRteRoutemaster {
public:
	LNRteRoutemaster();
	virtual ~LNRteRoutemaster();

	void StoreRouteline(LNRtelinerecord LNRteline,LNRouteSchedMaster* LNRteSchedMast);

	void StoreLocoRoutedetails(int Rtenum, LNweblocorouteprocess* LNLRprocess,LNRouteSchedMaster* LNRteSchedMast);


private:

};

#endif /* LNRTEROUTEMASTER_H_ */
