/*
 * LNRteLogicalsectionData.h
 *
 *  Created on: Aug 17, 2021
 *      Author: BrianRoot
 */

#ifndef LNRTELOGICALSECTIONDATA_H_
#define LNRTELOGICALSECTIONDATA_H_


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

#include "LNwebLsectionsLdevices.h"
#include "LNRteDevicereferencedata.h"

class LNRteLogicalsectionData {
public:
	LNRteLogicalsectionData();

	virtual ~LNRteLogicalsectionData();

	void  DisplayLSdata(char* replymessage);

	//void BuildLSdataoutputrecord();

	int BuildLSdataoutputrecord(char *Weboutput);

	int GetLSdataspeed();

	int GetLSdirection();

	int GetLSpsection();

	int GetLSpsection2();

	std::vector<int> GetNextsteps();

	void  StoreLSdata(int LSnumber,int LSdirection, int LStype, int Partner, int Psection1, int Psection2, int speed, int NextLS1, int NextLS2, int NextLS3, int NextLS4 \
			, int NextLS5, int NextLS6, int NextLS7, int NextLS8, int NextLS9, int NextLS10, int NextLS11, int NextLS12);

private:

			/*
			/  Record number 		  000
			/  Logical section  	  000
			/  Direction              0 = backwards  1 = forward
			/  Physical section 1     000
			/  Type                   1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch
			/  Physical section 2 	  000
			/  Logical partner        000
			/  Section speed            0
			/  Next section 1         000
			/  Next section 2         000
			/  Next section 3         000
			/  Next section 4         000
			*/

	unsigned int Logicalsection;
	unsigned int Direction;
	unsigned int Type;
	unsigned int Physicalsection1;
	unsigned int Physicalsection2;
	unsigned int LSpartner;
	unsigned int LSspeed;
	unsigned int LSNS1;
	unsigned int LSNS2;
	unsigned int LSNS3;
	unsigned int LSNS4;
	unsigned int LSNS5;
	unsigned int LSNS6;
	unsigned int LSNS7;
	unsigned int LSNS8;
	unsigned int LSNS9;
	unsigned int LSNS10;
	unsigned int LSNS11;
	unsigned int LSNS12;
};

#endif /* LNRTELOGICALSECTIONDATA_H_ */
