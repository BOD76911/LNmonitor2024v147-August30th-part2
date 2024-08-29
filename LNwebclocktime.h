/*
 * Clocktime.h
 *
 *  Created on: Aug 3, 2018
 *      Author: BrianRoot
 */

#ifndef LNWEBCLOCKTIME_H_
#define LNWEBCLOCKTIME_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LNwebconfig.h"

class LNwebclocktime {
public:
	LNwebclocktime();
	virtual ~LNwebclocktime();

	void Get_time(char& Datetimeout);
private:

	time_t my_time;

};

#endif /* CLOCKTIME_H_ */
