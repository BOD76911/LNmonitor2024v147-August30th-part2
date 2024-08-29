/*
 * Clocktime.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: BrianRoot
 */

#include "LNwebclocktime.h"
#include <iostream>			// include standard library of io routines
#include <string>
#include <stdio.h>
#include <iostream>

LNwebclocktime::LNwebclocktime() {
	// TODO Auto-generated constructor stub

}

LNwebclocktime::~LNwebclocktime() {
	// TODO Auto-generated destructor stub
}

void LNwebclocktime::Get_time(char& Datetimeout)
{
	char* msgptr = &Datetimeout;

	char Timestamp[DATETIMESTRING];

	char* Timeptr = &Timestamp[0];

	int msgcc = 0;

	my_time = time(NULL);

	// ctime() used to give the present time
	sprintf(Timestamp,"%s", ctime(&my_time));

	for (int cc = 0;cc <= DATETIMESTRING;cc++)
	{
		*(msgptr + msgcc) = *(Timeptr + cc);				// copy the Line with the header data to the Output buffer

		 msgcc++;
	}

}

