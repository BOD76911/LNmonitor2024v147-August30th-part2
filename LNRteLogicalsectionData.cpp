/*
 * LNRteLogicalsectiondata.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: BrianRoot
 */

#include "LNRteLogicalsectionData.h"

LNRteLogicalsectionData::LNRteLogicalsectionData() {
	// TODO Auto-generated constructor stub

}

LNRteLogicalsectionData::~LNRteLogicalsectionData() {
	// TODO Auto-generated destructor stub
}

void  LNRteLogicalsectionData::StoreLSdata(int LSnumber,int LSdirection, int LStype, int Partner, int Psection1, int Psection2, int speed, \
		int NextLS1, int NextLS2, int NextLS3, int NextLS4, int NextLS5, int NextLS6, int NextLS7, int NextLS8, int NextLS9, int NextLS10, int NextLS11, int NextLS12)
{
	/*
	/
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
	*/


	Logicalsection = LSnumber;
	Direction = LSdirection;
	Type = LStype;
	Physicalsection1 = Psection1;
	Physicalsection2 = Psection2;
	LSpartner = Partner;
	LSspeed = speed;
	LSNS1 = NextLS1;
	LSNS2 = NextLS2;
	LSNS3 = NextLS3;
	LSNS4 = NextLS4;
	LSNS5 = NextLS5;
	LSNS6 = NextLS6;
	LSNS7 = NextLS7;
	LSNS8 = NextLS8;
	LSNS9 = NextLS9;
	LSNS10 = NextLS10;
	LSNS11 = NextLS11;
	LSNS12 = NextLS12;

}

std::vector<int> LNRteLogicalsectionData::GetNextsteps()
{
	std::vector<int> NSsteps;

	NSsteps.push_back(LSNS1);
	NSsteps.push_back(LSNS2);
	NSsteps.push_back(LSNS3);
	NSsteps.push_back(LSNS4);
	NSsteps.push_back(LSNS5);
	NSsteps.push_back(LSNS6);
	NSsteps.push_back(LSNS7);
	NSsteps.push_back(LSNS8);
	NSsteps.push_back(LSNS9);
	NSsteps.push_back(LSNS10);
	NSsteps.push_back(LSNS11);
	NSsteps.push_back(LSNS12);

	return NSsteps;
}

int LNRteLogicalsectionData::GetLSdataspeed()
{
	return LSspeed;
}

int LNRteLogicalsectionData::GetLSpsection()
{

	return Physicalsection1;
}

int LNRteLogicalsectionData::GetLSpsection2()
{

	return Physicalsection2;
}

int LNRteLogicalsectionData::BuildLSdataoutputrecord(char *Weboutput)
{
	char Lineoutput[OUTPUTLINELENGTH];

	int cd;

	int ce = 0;

	// printf("\nBuilt in ->%i", &Weboutput);

	//printf("\nReceived as built ->%s", Weboutput);

	//bzero(WebOutmsg,Outmsgbuffersize);

	//bzero(Lineoutput,OUTPUTLINELENGTH);

	//printf("\n\nDISPLAY LS object details \n");
	sprintf (Lineoutput,"#%03d+%03d+%03d+%03d+%03d+%03d+%03d+%03d+%03d+%03d+%03d", \
	  Logicalsection, \
	  Direction,\
	  Type, \
	  Physicalsection1, \
	  Physicalsection2,\
	  LSpartner,\
	  LSspeed,\
	  LSNS1,\
	  LSNS2,\
	  LSNS3,\
	  LSNS4);

	 // for (cd = 0;cd<(strlen(Lineoutput));cd++)
	 // {
	//	WebOutmsg[ce] = Lineoutput[cd];
	//	ce = ce + 1;
	 // }

	  for (cd = 0;cd<(strlen(Lineoutput));cd++)
	  {
		*Weboutput = Lineoutput[cd];
		Weboutput++;
		ce = ce + 1;
	  }

	 //printf("\nBuilt out ->%i", &Weboutput);

	  return ce;

	//	printf("\n%s", WebOutmsg);
}

int LNRteLogicalsectionData::GetLSdirection()
{
	return Direction;
}

void LNRteLogicalsectionData::DisplayLSdata(char* replymessage)
{
	char Lineoutput[OUTPUTLINELENGTH * 4];

	bzero (Lineoutput,OUTPUTLINELENGTH * 4);

	int cd, mcc, isc,ic;

	mcc = 0;

	int ce = 0;


	sprintf (Lineoutput,"\n LSsection %i, LSdirection %i LStype %i LSpsection1 %i LSpsection2 %i LSsecpartner %i  LSspeed %i LSns1 %i LSns2 %i LSns3 %i LSns4 %i ", \
	  Logicalsection, \
	  Direction,\
	  Type, \
	  Physicalsection1, \
	  Physicalsection2,\
	  LSpartner,\
	  LSspeed,\
	  LSNS1,\
	  LSNS2,\
	  LSNS3,\
	  LSNS4);


	isc = std::strlen(Lineoutput);

			for (ic = 0;ic <isc;ic++)
			{
			*(replymessage + mcc) = Lineoutput[ic];
			mcc++;
			}

	//	printf("\n%s", replymessage);
}


