/*
 * LNSearchresult.h
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#ifndef LNSEARCHRESULT_H_
#define LNSEARCHRESULT_H_

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

class LNSearchresult
{
	public:

	LNSearchresult();

	~LNSearchresult();

	void Storeresult(int Sstep, int BLS, std::vector<int> Rsteps);

	std::vector<int> Getresultsteps();

	int Getresultstepnumber();

	int GetBranchLS();

	std::vector<int> Vectcopy(std::vector<int> &srcevect, std::vector<int> &destvect);

	void Displayvector(std::vector<int> &srcevect);

	void DisplaySrchBRresult();

	int GetCheckedstatus();

	void SetCheckedstatus();

	private:

	int Checkedstatus;
	int Step;
	int BranchLS;

	std::vector<int> Resultsteps;
};

#endif /* LNSEARCHRESULT_H_ */
