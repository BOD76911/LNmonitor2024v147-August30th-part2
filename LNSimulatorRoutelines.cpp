/*
 * LNSimulatorRoutelines.cpp
 *
 *  Created on: Sep 5, 2021
 *      Author: BrianRoot
 */

#include "LNSimulatorRoutelines.h"

LNSimulatorRoutelines::LNSimulatorRoutelines() {
	// TODO Auto-generated constructor stub

}

LNSimulatorRoutelines::~LNSimulatorRoutelines() {
	// TODO Auto-generated destructor stub
}

int LNSimulatorRoutelines::Getstepnumber()
{
	return recRtestep;
}

std::vector<int> LNSimulatorRoutelines::Getstepdetail()
{
	std::vector<int> Stepdetail;

	/*
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
	*/

	Stepdetail.push_back(recRtestep);
	Stepdetail.push_back(timervalue);
	Stepdetail.push_back(TypeFlag1);
	Stepdetail.push_back(Type1code);
	Stepdetail.push_back(Type1value);
	Stepdetail.push_back(TypeFlag2);
	Stepdetail.push_back(Type2code);
	Stepdetail.push_back(Type2value);
	Stepdetail.push_back(TypeFlag3);
	Stepdetail.push_back(Type3code);
	Stepdetail.push_back(Type3value);

	return Stepdetail;

}

void LNSimulatorRoutelines::StoreSimRteline(int Rte,int step,int timerval, int TFflag1, int TF1code, int TF1value,\
			int TFflag2, int TF2code, int TF2value, int TFflag3, int TF3code, int TF3value)
{
	/*
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
	*/

	recRte = Rte;
	recRtestep = step;
	timervalue = timerval;
	TypeFlag1 	= TFflag1;
	Type1code	= TF1code;
	Type1value	= TF1value;
	TypeFlag2 	= TFflag2;
	Type2code	= TF2code;
	Type2value	= TF2value;
	TypeFlag3 	= TFflag3;
	Type3code	= TF3code;
	Type3value	= TF3value;
}


int LNSimulatorRoutelines::DisplaySimRtestep(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE * 4];

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,OUTPUTRECSIZE * 4);

	/*

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
	*/

	//printf("\nPF->Rte Num: %03d Step: %02d Timerval: %02d TF1: %01d TC1: %01d TV1: %01d TF2: %01d \
				TC2: %01d TV2: %01d TF3: %01d TC3: %01d TV3: %01d ", recRte,recRtestep,timervalue,TypeFlag1, \
				Type1code,Type1value, TypeFlag2,Type2code,Type2value, TypeFlag3,Type3code,Type3value);

	sprintf(outputrec,"\nRte Num: %03d Step: %02d Timerval: %02d TF1: %01d TC1: %01d TV1: %01d TF2: %01d TC2: %01d TV2: %01d TF3: %01d TC3: %01d TV3: %01d ", recRte,recRtestep,timervalue,TypeFlag1, \
			Type1code,Type1value, TypeFlag2,Type2code,Type2value, TypeFlag3,Type3code,Type3value);

			isc =  std::strlen(outputrec);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
			}

	return isc;
}

