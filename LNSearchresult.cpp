/*
 * LNSearchresult.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#include "LNSearchresult.h"

LNSearchresult::LNSearchresult() {
	// TODO Auto-generated constructor stub

}

LNSearchresult::~LNSearchresult() {
	// TODO Auto-generated destructor stub
}


std::vector<int> LNSearchresult::Vectcopy(std::vector<int> &srcevect, std::vector<int> &destvect)
//std::vector<int> Searchprocess::Vectcopy(std::vector<int> &srcevect, std::vector<int> &destvect)
{
	 int i;

	 for (i=0; i<srcevect.size(); i++)
        destvect.push_back(srcevect[i]);

     return  destvect;

}

int LNSearchresult::GetBranchLS()
{
	return BranchLS;
}


void LNSearchresult::Storeresult(int Sstep, int BLS, std::vector<int> Rsteps)
{
	// Store search details when a branch is found

	/*
	int Step;
	int BranchLS;

	std::vector<int> Resultsteps;	*/

	Step = Sstep;
	BranchLS = BLS;
	Checkedstatus = 0;

	Vectcopy(Rsteps,Resultsteps);
}


std::vector<int> LNSearchresult::Getresultsteps()
{
	return Resultsteps;
}


