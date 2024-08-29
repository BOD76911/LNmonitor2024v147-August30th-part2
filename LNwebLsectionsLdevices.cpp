/*
 * LNwebLsectionsLdevices.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: BrianRoot
 */

#include "LNwebLsectionsLdevices.h"
#include "LNwebmonNGfileload.h"
#include "LNwebconfig.h"
#include "Clocktime.h"
#include "LNsignalprocessing.h"


#include <string>
#include <string.h>
#include <cstring>
#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <stdio.h>

LNwebLsectionsLdevices::LNwebLsectionsLdevices() {
	// TODO Auto-generated constructor stub

	InitLastupdatedetails();					// initialize the sequence details.

}

void LNwebLsectionsLdevices::InitializePWMsections()
{
	for (int pwmsc = 0;pwmsc < MAXPWMPHYSECTIONS;pwmsc++)
	{
		PWMsectionsarray[pwmsc].SectionreferenceNumber = 0;
		PWMsectionsarray[pwmsc].SectionTransmissionreference = 0;
		PWMsectionsarray[pwmsc].Currentspeedvalue = 0;
		PWMsectionsarray[pwmsc].Prevspeedvalue = 0;
		PWMsectionsarray[pwmsc].Currentdirectionvalue = 0;
		PWMsectionsarray[pwmsc].Prevdirectionvalue = 0;
		PWMsectionsarray[pwmsc].GUIobjectreference = 0;
		PWMsectionsarray[pwmsc].LocoCode = 0;

		for (int lsc = 0; lsc < PWMPHYSECTIONSMAXLS;lsc++)
		{
			PWMsectionsarray[pwmsc].Linkedlogicalsections[lsc] = 0;
		}
		
		PWMcontrolmode = 0;
	}

	for (int mancss = 0; mancss < MAXCOUNTMANCONTROLLERS;mancss++)
	{
		Manualcontrollersarray[mancss].Direction = 0;
		Manualcontrollersarray[mancss].Speed	= 0;
		Manualcontrollersarray[mancss].Mode		= 0;
	}
}

void LNwebLsectionsLdevices::SwitchPWMsectionsmode(int mode)
{
	PWMcontrolmode = mode;

	std::cout<<"\n62 PWM mode value set to be:"<<PWMcontrolmode;
	printf("\n");
}

int LNwebLsectionsLdevices::GetPWMsectionmode()
{
	return PWMcontrolmode;
}

void LNwebLsectionsLdevices::UpdateAllPWMsections(int Speed, int Direction)
{
	for (int pwmsc = 0;pwmsc < MAXPWMPHYSECTIONS;pwmsc++)
		{
			PWMsectionsarray[pwmsc].Currentspeedvalue = Speed;
			PWMsectionsarray[pwmsc].Currentdirectionvalue = Direction;
		}
}

int LNwebLsectionsLdevices::SetPWMsectiondirectionandspeed(int section, int speed,int direction)
{
	int RC = 0;

	PWMsectionsarray[section].Currentspeedvalue = speed;
	PWMsectionsarray[section].Currentdirectionvalue = direction;

	return RC;
}

void LNwebLsectionsLdevices::RefreshsectionPWMcontrollers()
{
	for (int PWMsc = 1; PWMsc < MAXPWMPHYSECTIONS; PWMsc++)
	{
		if ((PWMsectionsarray[PWMsc].Currentspeedvalue != PWMsectionsarray[PWMsc].Prevspeedvalue) || \
				(PWMsectionsarray[PWMsc].Currentdirectionvalue != PWMsectionsarray[PWMsc].Prevdirectionvalue))
		{
			PiUartPWMSerialTransmission(PWMsc, PWMsectionsarray[PWMsc].Currentspeedvalue, PWMsectionsarray[PWMsc].Currentdirectionvalue,0);
			PWMsectionsarray[PWMsc].Prevspeedvalue = PWMsectionsarray[PWMsc].Currentspeedvalue;
			PWMsectionsarray[PWMsc].Prevdirectionvalue = PWMsectionsarray[PWMsc].Currentdirectionvalue;
			//printf("\n100 Section %02X updated to speed %02X",PWMsc,PWMsectionsarray[PWMsc].Currentspeedvalue);
			//printf("\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(35));
		}
	}
}


void LNwebLsectionsLdevices::UpdatePWMcontroller(int controller,int speed,int direction,int mode)
{
	Manualcontrollersarray[controller].Speed = speed;

	Manualcontrollersarray[controller].Direction = direction;

	Manualcontrollersarray[controller].Mode = mode;
}

int LNwebLsectionsLdevices::GetallcontrollerData(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	for (int tsc = 0; tsc < MAXCOUNTMANCONTROLLERS; tsc++)
	{
		sprintf(outputrec,"%02d-%03d-%01d-%01d:",tsc,Manualcontrollersarray[tsc].Speed,Manualcontrollersarray[tsc].Direction,Manualcontrollersarray[tsc].Mode);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	return mcc;
}

int LNwebLsectionsLdevices::GetallPWMsectionData(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	for (int tsc = 0; tsc < MAXPWMPHYSECTIONS; tsc++)
	{
		sprintf(outputrec,"%03d-%03d-%01d-%01d:", tsc,PWMsectionsarray[tsc].Currentspeedvalue,PWMsectionsarray[tsc].Currentdirectionvalue,0);

		isc =  std::strlen(outputrec);
		//printf("\n%s",outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	return mcc;
}



int LNwebLsectionsLdevices::TSgetLScount(int Trainsection)
{
	int LScount = 0;

	for (int LScc = 0; LScc < MAXLSinTS; LScc++)
	{
		if (TrainSectiondata[Trainsection].TSlsections[LScc] == 0)  // search for last LS
		{
			if ((LScc - 1) == 0)
			{
				LScount = 0;
				break;
			}
			else
			{
				LScount = LScc - 1;
				break;
			}
		}
	}

	return LScount;
}

int LNwebLsectionsLdevices::TSmatchedLS(int LS)
{
	int Trainsection = 0;

	for (int TScc = 0; TScc < MAXTRAINSECTIONS; TScc++)
	{
		//std::cout<<"\n35 checking Trainsection:"<<TrainSectiondata[TScc].TrainsectionNumber;
		for (int LScc = 0; LScc < MAXLSinTS; LScc++)
		{
			if (TrainSectiondata[TScc].TSlsections[LScc] == LS)
			{
				return TScc;  // matched on Train section
			}
		}
	}

	return Trainsection;
}

int LNwebLsectionsLdevices::TSgetPSoccupancyflag(int Trainsection, int PSsubscript)
{
	return TrainSectiondata[Trainsection].TSpsectionsOccupancyflag[PSsubscript];
}

int LNwebLsectionsLdevices::TSgetlastvalidPS(int Trainsection)
{
	int PSsubscript = 0;

	/*
		struct TSectionarray
		{
			int  TrainsectionNumber;
			int  Trainsectiontype;
			int  Traincode;
			int  Traintype;
			int  TSoccupancyflag;
			int  Trainlococode;
			int  TSdirection;
			int  TSpsections[MAXPSinTS];
			int  TSpsectionsOccupancyflag[MAXPSinTS];
			int  TSlsections[MAXLSinTS];
			int  TSlsectionsTcodes[MAXLSinTS];
			int  TSGUIref[MAXGUIrefTS];
			int  TSNSinTS[MAXNSinTS];
		};

	struct TSectionarray TrainSectiondata[MAXTRAINSECTIONS];

	*/

	//std::cout<<"\n\n98 Checking for last Physical section for Train section:"<<Trainsection;

	for (int TsPS = 0; TsPS < MAXPSinTS;TsPS++)
	{
		if ((TrainSectiondata[Trainsection].TSpsections[TsPS] > 0) && (TrainSectiondata[Trainsection].TSpsections[TsPS + 1] == 0))
		{
			PSsubscript == TsPS;
			//std::cout<<"\n105 Last Physical section subscript matched at:"<<TsPS;
			//std::cout<<"\n106 Last Physical section for Trainsection:"<<Trainsection<<" found as :"<<TrainSectiondata[Trainsection].TSpsections[TsPS];
			return PSsubscript;
		}
	}

	return PSsubscript;
}

int LNwebLsectionsLdevices::TSmatchedLSindex(int LS)
{
	int Trainsectionindex = 0;

	for (int TScc = 0; TScc < MAXTRAINSECTIONS; TScc++)
		{
			//std::cout<<"\n35 checking Trainsection:"<<TrainSectiondata[TScc].TrainsectionNumber;
			for (int LScc = 0; LScc < MAXLSinTS; LScc++)
			{
				if (TrainSectiondata[TScc].TSlsections[LScc] == LS)
				{
					return LScc;  // matched so return matching index
				}
			}
		}


	return Trainsectionindex;
}

// ---------------- Train section logic - Jan 2024 - introduced with version 11.0  ----

/*
	struct TSectionarray
	{
		int  TrainsectionNumber;
		int  Traincode;
		int  Traintype;
		int  Trainlococode;
		int  TSdirection;
		int  TSpsections[MAXPSinTS];
		int  TSlsections[MAXLSinTS];
	};

	struct TSectionarray TrainSectiondata[MAXTRAINSECTIONS];
*/
void LNwebLsectionsLdevices::TSUpdatetrainsectionsPhysectionstatus()
{
	char PSmessage[SHORTMESSAGELENGTH ];

	for (int TSC = 0; TSC < MAXTRAINSECTIONS; TSC++)
	{
		for (int TSCss = 0; TSCss < MAXPSinTS; TSCss++)
		{
			if (TrainSectiondata[TSC].TSpsections[TSCss] > 0)
			{
				bzero(PSmessage,SHORTMESSAGELENGTH);

				TrainSectiondata[TSC].TSpsectionsOccupancyflag[TSCss] = PSectiondatareturn(TrainSectiondata[TSC].TSpsections[TSCss],&PSmessage[0]);
			}
		}

		TrainSectiondata[TSC].TSoccupancyflag = 0;

		for (int TSCsf = 0; TSCsf < MAXPSinTS; TSCsf++)
		{
			if ((TrainSectiondata[TSC].TSpsectionsOccupancyflag[TSCsf] > 0) || (TrainSectiondata[TSC].Traincode > 0))
			{
				TrainSectiondata[TSC].TSoccupancyflag = 1;
			}
		}
	}
}

int LNwebLsectionsLdevices::TSgetoccupancyflag(int Trainsection)
{
	return TrainSectiondata[Trainsection].TSoccupancyflag;
}

void LNwebLsectionsLdevices::TSinitTrainsections()
{
	for (int TSC = 0; TSC < MAXTRAINSECTIONS; TSC++)
	{
		TrainSectiondata[TSC].TrainsectionNumber 	= 0;
		TrainSectiondata[TSC].Trainsectiontype		= 0;
		TrainSectiondata[TSC].Traincode				= 0;
		TrainSectiondata[TSC].Trainlococode			= 0;
		TrainSectiondata[TSC].Traintype				= 0;
		TrainSectiondata[TSC].RCGcode				= 0;

		TrainSectiondata[TSC].TSdirection			= 0;

		TrainSectiondata[TSC].TSoccupancyflag		= 0;

		for (int TSCss = 0; TSCss < MAXPSinTS; TSCss++)
		{
			TrainSectiondata[TSC].TSpsections[TSCss] = 0;
			TrainSectiondata[TSC].TSpsectionsOccupancyflag[TSCss] = 0;
		}

		for (int TSCls = 0; TSCls < MAXLSinTS; TSCls++)
		{
			TrainSectiondata[TSC].TSpsections[TSCls] = 0;
		}

		for (int TSClg = 0; TSClg < MAXGUIrefTS; TSClg++)
		{
			TrainSectiondata[TSC].TSGUIref[TSClg] = 0;
		}

		for (int TSCln = 0; TSCln < MAXNSinTS; TSCln++)
		{
			TrainSectiondata[TSC].TSNSinTS[TSCln] = 0;
		}
	}
}

void LNwebLsectionsLdevices::DisplayUncouplerdetails(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (int Ucc = 1; Ucc < NUMBEROFUNCOUPLERS;Ucc++)
	{
		bzero(outputrec,OUTPUTRECSIZE);

		sprintf(outputrec,"\nRecNumber:%02d Uncoupler:%02d: Psection:%03d: TriggerStatus:%01d",Ucc,UncouplerReferencedata[Ucc].Uncouplernumber,UncouplerReferencedata[Ucc].Psectionlocation,UncouplerReferencedata[Ucc].UCtriggerstatus);

							isc =  std::strlen(outputrec);

							if (UncouplerReferencedata[Ucc].Uncouplernumber > 0)
							{
								for (ic = 0;ic <isc;ic++)
									{
										*(replymessage + mcc) = outputrec[ic];
										mcc++;
									}
							}
	}

	*(replymessage + mcc) = '\n';
}

void LNwebLsectionsLdevices::SetUncouplerstatus(int uncouplernumber, int statusvalue)
{
	UncouplerReferencedata[uncouplernumber].UCtriggerstatus = statusvalue;
}


int LNwebLsectionsLdevices::GetUncouplerstatus(int uncouplernumber)
{
	return UncouplerReferencedata[uncouplernumber].UCtriggerstatus;
}

void LNwebLsectionsLdevices::StoreUncouplerdetails(char* lineinputbufferptr, int displayflag)
{
	Bufferprocessing* BPLL = new Bufferprocessing();

	int fieldcount = BPLL->Fileparse(lineinputbufferptr, ';', 0);

	int value = 0;

	int* valueptr = &value;

	int UncouplerNumber = 0;

	int UncouplerPS = 0;


	if (BPLL->ParamdataInteger(2,valueptr) > 0)  // Uncoupler number
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field for uncoupler";

	}
	else
	{
		UncouplerNumber = value;
	}

	if (BPLL->ParamdataInteger(3,valueptr) > 0)  // Physical section location
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field for uncoupler physical location";

	}
	else
	{
		UncouplerPS = value;
	}

	UncouplerReferencedata[UncouplerNumber].Uncouplernumber = UncouplerNumber;

	UncouplerReferencedata[UncouplerNumber].Psectionlocation = UncouplerPS;

	UncouplerReferencedata[UncouplerNumber].UCtriggerstatus = 0;

	Uncouplerloadcount++;

	if (displayflag > 0)
	{
		printf("\n\n266 Uncoupler %02d in PS %03d loaded",UncouplerReferencedata[UncouplerNumber].Uncouplernumber,UncouplerReferencedata[UncouplerNumber].Psectionlocation);
	}

	delete BPLL;
}

void LNwebLsectionsLdevices::TSstoreTrainsectionconfig(char* lineinputbufferptr, int displayflag)
{
	// processes messages from LNconfigloader which is processing configuration details from new AllConfiguration file. Function is called with a pointer to the
	// the Train section configuration record that is being processed.  1440

	int TrainsectionNumber;
	int Trainsectiondirection;
	int Trainsectiontype;
	int value = 0;

	int PSinTS[MAXPSinTS];

	int GUIinTS[MAXGUIrefTS];

	int NSinTS[MAXNSinTS];

	int Invalidfieldcount = 0;

	int conversionRC;

	int* valueptr = &value;

	if (displayflag > 0)
	{
		std::cout<<"\n73 Train section config record read->"<<lineinputbufferptr;
	}

	Bufferprocessing* BPLL = new Bufferprocessing();

	int fieldcount = BPLL->Fileparse(lineinputbufferptr, ';', 0);

	/*
	  Note: Fields delimited by ; character
		        Fields sub-delimited by + character

		/*
		/*	4		  	000			int type Record number
		/*  8           000 Rec 	int type 15 record number
		/*	12			000	TS		int Train section number
		/*  16            0 d   	int Train section direction
		/*	18			000	TS		int Physical sections split by '+'
		/*				0	t		int Section type  1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
		/*              000 GUI 	int GUI object reference number - split by + character
		/*				000	TSNS 	int Next Train section  - split by + character
		/*
		/         1         2         3         4         5         6
		/123456789012345678901234567890123456789012345678901234567890
		/       Rec TS  d  Phys    t  GUI             NS
		512;015;001;001;0;001+002+;1;+001+002+000;008+009+010;
	*/

	value = 0;

	if (BPLL->ParamdataInteger(3,valueptr) > 0)  // Train number
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field 3";
			Invalidfieldcount++;
	}
	else
	{
			TrainsectionNumber  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(4,valueptr) > 0)  // Train section direction
	{
				std::cout<<"\nNon-numeric char in lineinputbuffer field 4";
				Invalidfieldcount++;
	}
	else
	{
				Trainsectiondirection  = value;
	}

	 TrainSectiondata[TrainsectionNumber].TrainsectionNumber = TrainsectionNumber;

	 TrainSectiondata[TrainsectionNumber].TSdirection = Trainsectiondirection;

	// read Physical section list

	for (int PSd = 0; PSd < MAXPSinTS; PSd++)
	{
		PSinTS[PSd] = 0;
	}

	// split field 5 to extract Physical reference fields - delimited by + character

	Bufferprocessing* BPFX = new Bufferprocessing();

	int PSfc = BPFX->Fileparse(BPLL->Paramdata(5), '+', 0);

	//std::cout<<"\n160  Train section PSfc:"<<PSfc<<"\n";

	// extract PS references

	 for (int PScc = 0;PScc<PSfc;PScc++)
	 {
			value = 0;
			BPFX->ParamdataInteger(PScc,valueptr);
			PSinTS[PScc] = value;
	 }

	 value = 0;

	 for (int PScm = 0; PScm < PSfc;PScm++)
	 {
		 TrainSectiondata[TrainsectionNumber].TSpsections[PScm] = PSinTS[PScm];
	 }

	 // extract Train section type

	 if (BPLL->ParamdataInteger(6,valueptr) > 0)  // Train section type
	 {
	 		std::cout<<"\nNon-numeric char in lineinputbuffer field 6";
	 		Invalidfieldcount++;
	 }
	 else
	 {
	 		Trainsectiontype  = value;
	 }

	 // read GUI in Train section list

	 for (int GUd = 0; GUd < MAXGUIrefTS; GUd++)
	 {
	 		GUIinTS[GUd] = 0;
	 }

	 // split field 7 to extract GUI reference fields - delimited by + character

	 Bufferprocessing* BPFY = new Bufferprocessing();

	 int GUfc = BPFY->Fileparse(BPLL->Paramdata(7), '+', 0);

	// std::cout<<"\n192  Train section GUfc:"<<GUfc<<"\n";

	 // extract GUI references

	 for (int GUcc = 0;GUcc<GUfc;GUcc++)
	 {
	 	value = 0;
	 	BPFY->ParamdataInteger(GUcc,valueptr);
	 	GUIinTS[GUcc] = value;
	 }

	 for (int GUcm = 0; GUcm < GUfc;GUcm++)
	 {
		 TrainSectiondata[TrainsectionNumber].TSGUIref[GUcm] = GUIinTS[GUcm];
	 }

	 // read Next section in Train section list

	 for (int NSd = 0; NSd < MAXNSinTS; NSd++)
	 {
	 	 		NSinTS[NSd] = 0;
	 }

	 // split field 8 to extract NS (next section) reference fields - delimited by + character

	  Bufferprocessing* BPFZ = new Bufferprocessing();

	  int NSfc = BPFZ->Fileparse(BPLL->Paramdata(8), '+', 0);

	  //std::cout<<"\n2112  Train section NSfc:"<<NSfc<<"\n";

	  // extract NS references

	  for (int NScc = 0;NScc<NSfc;NScc++)
	 	 {
	 	 	value = 0;
	 	 	BPFZ->ParamdataInteger(NScc,valueptr);
	 	 	NSinTS[NScc] = value;
	 	 }

	  for (int NScm = 0; NScm < NSfc;NScm++)
	  {
	  	 TrainSectiondata[TrainsectionNumber].TSNSinTS[NScm] = NSinTS[NScm];
	  }

	  if (displayflag > 0)
	  {
		  std::cout<<"\n240 Train section config record -> Number:"<<TrainsectionNumber<<" direction:"<<Trainsectiondirection<<" section type:"<<Trainsectiontype;
		  std::cout<<"\n241 Train section PS0:"<<PSinTS[0]<<" PS1:"<<PSinTS[1]<<" PS2:"<<PSinTS[2]<<" PS3:"<<PSinTS[3];
		  std::cout<<"\n242 Train section GU0:"<<GUIinTS[0]<<" GU1:"<<GUIinTS[1]<<" GU2:"<<GUIinTS[2]<<" GU3:"<<GUIinTS[3];
		  std::cout<<"\n243 Train section NS0:"<<NSinTS[0]<<" NS1:"<<NSinTS[1]<<" NS2:"<<NSinTS[2]<<" NS3:"<<NSinTS[3];
	  }

	  TrainSectiondata[TrainsectionNumber].Trainsectiontype = Trainsectiontype;

	/*

	struct TSectionarray
		{
			int  TrainsectionNumber;
			int  Trainsectiontype;
			int  Traincode;
			int  Traintype;
			int  Trainlococode;
			int  TSdirection;
			int  TSpsections[MAXPSinTS];
			int  TSlsections[MAXLSinTS];
			int  TSGUIref[MAXGUIrefTS];
			int  TSNSinTS[MAXNSinTS];
		};

	struct TSectionarray TrainSectiondata[MAXTRAINSECTIONS];


	*/

	delete BPLL;
	delete BPFX;
	delete BPFY;
	delete BPFZ;

}

int LNwebLsectionsLdevices::TSgetPS(int TS, int PSss)
{
	return TrainSectiondata[TS].TSpsections[PSss];
}

void LNwebLsectionsLdevices::UpdatesignalsTrainSections()
{
	// find Train section linked to Physical section PS

	for (int TSL = 0;TSL < MAXTRAINSECTIONS;TSL++)
	{
		//std::cout<<"\n453 TSL:"<<TSL<<" Trainsection:"<<TrainSectiondata[TSL].TrainsectionNumber;
		LNLSLDsignalprocess->UpdateTrainsectionsconfig(TrainSectiondata[TSL].TrainsectionNumber,TrainSectiondata[TSL].TSpsections[0],TrainSectiondata[TSL].TSpsections[1]);
	}
}

int LNwebLsectionsLdevices::TSgetTrainNumber(int Trainsection)
{
	return TrainSectiondata[Trainsection].Traincode;
}

void LNwebLsectionsLdevices::TSupdateSection(int section, int CurrentTrainID, int NewTrainID, int CallID)
{
	//std::cout<<"\n558 Updating Traincode in Trainsection:"<<section<<" with value:"<<NewTrainID<<" with callID:"<<CallID;

	// If CurrentTrainID > 0, check it matches the current Trainsection TrainNumber.

	if (CurrentTrainID == 0)
	{
		TrainSectiondata[section].Traincode = NewTrainID;
	}
	else
	{
		if (CurrentTrainID == TrainSectiondata[section].Traincode)
		{
			TrainSectiondata[section].Traincode = NewTrainID;
		}
		else
		{
			std::cout<<"\n\n481 Train Number clearance from Train section:"<<section<<" cancelled - Train number does not match Train Number in section:"<<TrainSectiondata[section].Traincode;
		}
	}
}

void LNwebLsectionsLdevices::ProcessTrainRailcargroupmatch(int Trainsection, int fieldswitchflag)
{
	// If the fieldswitchflag = 1 then check to see if Railcargroup and TrainNumber are the same. If so, clear the Railcargroup field
	// If the fieldswitchflag = 2 then move Traincode to Railcargroup code field and clear Traincode field

	if ((fieldswitchflag == 1) && (TrainSectiondata[Trainsection].Traincode == TrainSectiondata[Trainsection].RCGcode))
	{
		TrainSectiondata[Trainsection].RCGcode = 0;
	}

	if (fieldswitchflag == 2)
	{
		TrainSectiondata[Trainsection].RCGcode = TrainSectiondata[Trainsection].Traincode;
	}
}


void LNwebLsectionsLdevices::TSallsectionsclear()
{
	// clear all train sections

	for (int TSC = 0; TSC < MAXTRAINSECTIONS; TSC++)
	{
		TrainSectiondata[TSC].Traincode	= 0;
	}
}

void LNwebLsectionsLdevices::LoadTrainstoTrainsections(char* lineinputbufferptr, int displayflag)
{


}

void LNwebLsectionsLdevices::LoadRailcargrouptoTrainsection(char* lineinputbufferptr, int displayflag)
{
	int Recordnumber;
	int Trainrecordnumber;
	int TrainIDdata;
	int Trainrecordtype;
	int Actualnumberofrailcarsontrain;
	int InitialLSlocation;

	int CarIDnumber[MAXIMUMOFCARSONTRAIN];

	int RailcarloadRC = 0;

		//std::cout<<"\n\n207 - Railcar record read:"<<lineinputbufferptr;

		// extract data from record

		Bufferprocessing* GTP = new Bufferprocessing();

		GTP->Fileparse(lineinputbufferptr,';',0);

		/* Railcar group records
		/
		/     Field       Description
		/       1          Railgroup type  020
		/       2          Railcar group number
		/       3          Current Trainsection location
		/       4          Current LS location
		/       5          Railcars - delimited by '+'
		*/

	int Railcargroupnumber;

	int RailcargroupCurrLS;

	int RailcargroupTrainSectionlocation;

	Railcargroupnumber = atoi(GTP->Paramdata(2));

	RailcargroupTrainSectionlocation = atoi(GTP->Paramdata(3));

	RailcargroupCurrLS = atoi(GTP->Paramdata(4));

	TrainSectiondata[RailcargroupTrainSectionlocation].RCGcode = Railcargroupnumber;

	delete GTP;
}

void LNwebLsectionsLdevices::TSbuildLSsectionlist()
{
	int TSLS[MAXLSinTS];

	int* TSLSptr = &TSLS[0];

	int TSLScount 		= 0;

	int stLScount		= 0;

	for (int TScc = 0;TScc < MAXTRAINSECTIONS;TScc++)
	{
		if (TrainSectiondata[TScc].TrainsectionNumber > 0)
		{
			for (int tslsclr = 0;tslsclr < MAXLSinTS; tslsclr++)
			{
				TSLS[tslsclr] = 0;
			}

			TSLScount = 0;

			stLScount = 0;

			for (int TSpscount = 0;TSpscount < MAXPSinTS;TSpscount++)
			{
				if (TrainSectiondata[TScc].TSpsections[TSpscount] > 0)
				{
					TSLScount = LSfetch(TSLSptr,TrainSectiondata[TScc].TSpsections[TSpscount],TrainSectiondata[TScc].TSdirection); // fetch list of LS sections

					for (int stLS = 0;stLS < TSLScount;stLS++)
					{
						TrainSectiondata[TScc].TSlsections[stLScount] = *(TSLSptr + stLS);     // store list of LS sections
						stLScount++;
					}

				//	std::cout<<"\n327 TS:"<<TScc<<" PS:"<<TrainSectiondata[TScc].TSpsections[TSpscount]<<" TSLScount:"<<TSLScount;
				//	std::cout<<"\n328 LS0:"<<TrainSectiondata[TScc].TSlsections[0]<<" LS1:"<<TrainSectiondata[TScc].TSlsections[1];
				//	std::cout<<"\n329 stLScount:"<<stLScount;
				}
			}
		}
	}

}

int LNwebLsectionsLdevices::TrainsectionNumberfetch(int PS)
{
	int TrainsectionNumber = 0;

	for (int tsc = 0; tsc < MAXTRAINSECTIONS;tsc++)
	{
		if (TrainSectiondata[tsc].TrainsectionNumber > 0)
		{
			for (int tscps = 0;tscps < MAXPSinTS;tscps++)
			{
				if (TrainSectiondata[tsc].TSpsections[tscps] == PS)
				{
					TrainsectionNumber = tsc;
					break;
				}
			}
		}
	}

	return TrainsectionNumber;
}

int LNwebLsectionsLdevices::GetRailcarInsection(int TS)
{
	return TrainSectiondata[TS].RCGcode;
}

void LNwebLsectionsLdevices::SetRailcarInsection(int TS, int railcargroupnumber)
{
	TrainSectiondata[TS].RCGcode = railcargroupnumber;

	//std::cout<<"\n665 Railcargroupcode:"<<railcargroupnumber<<" stored in TS:"<<TS;
}

void LNwebLsectionsLdevices::ClearRailcarGroupInsection(int TS)
{
	TrainSectiondata[TS].RCGcode = 0;
}

void LNwebLsectionsLdevices::TSallRCGsectionsclear()
{
	for (int TSD = 0; TSD < MAXTRAINSECTIONS;TSD++)
	{
		TrainSectiondata[TSD].RCGcode = 0;
	}
}

void LNwebLsectionsLdevices::TSallRCGlococouple(int Railcargroupsection)
{
	// Moves Railcargroup code to Traincode field.

	TrainSectiondata[Railcargroupsection].Traincode = TrainSectiondata[Railcargroupsection].RCGcode;

	TrainSectiondata[Railcargroupsection].RCGcode = 0;
}

void LNwebLsectionsLdevices::TSallRCGlocoUncouple(int Railcargroupsection)
{
	// Moves Traincode to Railcargroup field to Traincode field.

	TrainSectiondata[Railcargroupsection].RCGcode = TrainSectiondata[Railcargroupsection].Traincode;

	TrainSectiondata[Railcargroupsection].Traincode = 0;
}

int LNwebLsectionsLdevices::LSfetch(int* LSlist, int PS, int direction)
{
	int LSfound = 0;

	//std::cout<<"\n340 checking PS:"<<PS;

	for (int LSsearch = 0; LSsearch < NUMBEROFLOGICALSECTIONS; LSsearch++)
	{
		if ((Logicalsectiondata[LSsearch].Physectionref == PS) && (Logicalsectiondata[LSsearch].Logicalsectiondirection == direction))
		{
			*(LSlist + LSfound) = LSsearch;
		//	std::cout<<"\n 347 LSmatched:"<<LSsearch;
			LSfound++;
		}
	}

//	std::cout<<"\n351 LS found:"<<LSfound<<" LSlist0:"<<*(LSlist+0)<<" LSlist1:"<<*(LSlist+1)<<" LSlist2:"<<*(LSlist+2);

	return LSfound;
}

int LNwebLsectionsLdevices::TSGUIdisplayTrainsecstions(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	int isc,ic,scc,mcc;

	int Traincode;

	int Railcargroupcode;

	mcc = 0;

	for (int tsc = 1; tsc < MAXTRAINSECTIONS; tsc++)
	{
		bzero(outputrec,OUTPUTMSGBUFFERSIZE);

		if (TrainSectiondata[tsc].Traincode > 0)
		{
			sprintf(outputrec,":%03d-TR%03d-%04d", tsc,TrainSectiondata[tsc].Traincode, TrainSectiondata[tsc].Trainlococode);
		}
		else
		{
			if (TrainSectiondata[tsc].RCGcode > 0)
			{
				sprintf(outputrec,":%03d-RC%03d-%04d", tsc,TrainSectiondata[tsc].RCGcode, TrainSectiondata[tsc].Trainlococode);
			}
			else
			{
				sprintf(outputrec,":%03d-%03d-%04d", tsc,TrainSectiondata[tsc].Traincode, TrainSectiondata[tsc].Trainlococode);
			}
		}
		isc =  std::strlen(outputrec);
		//printf("\n%s",outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	return mcc;
}

int LNwebLsectionsLdevices::TSclidisplayTrainsections(int Trainsectiondisplay,char* replymessage)
{
	char outputrec[OUTPUTRECSIZE * 4];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (int TScount = 0; TScount < MAXTRAINSECTIONS; TScount++)
	{

		if (TrainSectiondata[TScount].TrainsectionNumber > 0)
		{
			bzero(outputrec,OUTPUTRECSIZE);
			sprintf(outputrec,"\n\nTrainSection:%03d Sectiontype:%01d Direction:%01d Occupancyflag:%01d Traincode:%03d Traintype:%02d Railcar group:%03d Loco:%04d PS0:%03d PS0flg:%01d PS1:%03d PS1flg:%01d PS2:%03d PS2flg:%01d PS3:%03d PS3flg:%01d GUI0:%03d GUI1:%03d GUI2:%03d GUI3:%03d\n\
					LS0:%03d LS1:%03d LS2:%03d LS3:%03d LS4:%03d LS5:%03d LS6:%03d LS7:%03d NS0:%03d NS1:%03d NS2:%03d NS3:%03d ",\
					TrainSectiondata[TScount].TrainsectionNumber, \
					TrainSectiondata[TScount].Trainsectiontype,\
					TrainSectiondata[TScount].TSdirection,\
					TrainSectiondata[TScount].TSoccupancyflag,\
					TrainSectiondata[TScount].Traincode,\
					TrainSectiondata[TScount].Traintype,\
					TrainSectiondata[TScount].Trainlococode,\
					TrainSectiondata[TScount].RCGcode,\
					TrainSectiondata[TScount].TSpsections[0],TrainSectiondata[TScount].TSpsectionsOccupancyflag[0],\
					TrainSectiondata[TScount].TSpsections[1],TrainSectiondata[TScount].TSpsectionsOccupancyflag[1],\
					TrainSectiondata[TScount].TSpsections[2],TrainSectiondata[TScount].TSpsectionsOccupancyflag[2],\
					TrainSectiondata[TScount].TSpsections[3],TrainSectiondata[TScount].TSpsectionsOccupancyflag[3],\
					TrainSectiondata[TScount].TSGUIref[0],TrainSectiondata[TScount].TSGUIref[1],TrainSectiondata[TScount].TSGUIref[2],\
					TrainSectiondata[TScount].TSGUIref[3],TrainSectiondata[TScount].TSlsections[0],\
					TrainSectiondata[TScount].TSlsections[1],TrainSectiondata[TScount].TSlsections[2],\
					TrainSectiondata[TScount].TSlsections[3],TrainSectiondata[TScount].TSlsections[4],\
					TrainSectiondata[TScount].TSlsections[5],TrainSectiondata[TScount].TSlsections[6],\
					TrainSectiondata[TScount].TSlsections[7],TrainSectiondata[TScount].TSNSinTS[0],TrainSectiondata[TScount].TSNSinTS[1],\
					TrainSectiondata[TScount].TSNSinTS[2],TrainSectiondata[TScount].TSNSinTS[3]);

			isc =  std::strlen(outputrec);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
			}
		}
	}

	return mcc;

}

void LNwebLsectionsLdevices::InitOploggingptr(LNoperationlogging* LNOplatestlogptr)
{
	LNOploggingptr = LNOplatestlogptr;			// Receive operations logging pointer
}

LNwebLsectionsLdevices::~LNwebLsectionsLdevices() {
	// TODO Auto-generated destructor stub
}

void LNwebLsectionsLdevices::LNallTrainslinkage(LNallTrains* LNallTrainsinformation)
{
	LNallTrainsinformationLSLD = LNallTrainsinformation;

}

int LNwebLsectionsLdevices::GetLS_PS1section(int ls)
{
		return Logicalsectiondata[ls].Physectionref;
}

int LNwebLsectionsLdevices::GetLS_PS2section(int ls)
{
	return Logicalsectiondata[ls].Physectionref2;
}

int LNwebLsectionsLdevices::GetPSforGUIref(int GUIref)
{
	int PSref = 0;

	//std::cout<<"\n690 GUIref:"<<GUIref;

	for (int grs = 1;grs < NUMBEROFLOGICALSECTIONS;grs++)
	{
		for (int grssc = 0;grssc < MAXGUIREFSFORLS;grssc++)
		{
			//std::cout<<"\n696 LS:"<<grs<<" Psection:"<<Logicalsectiondata[grs].Physectionref<<" GUIrefss:"<<grssc<< " GUIref:"<<Logicalsectiondata[grs].GUIobjectreference[grssc];
			if (Logicalsectiondata[grs].GUIobjectreference[grssc] == GUIref)
			{
				//std::cout<<"\n\n699 GUI ref:"<<GUIref<<" matched LS:"<<grs<<" Psection:"<<Logicalsectiondata[grs].Physectionref;
				PSref = Logicalsectiondata[grs].Physectionref;
				break;
			}
		}
	}

	return PSref;
}

void LNwebLsectionsLdevices::CcallLogicalsectionupdate()
{
	std::cout<<"\nLNLSLD device testcall ......";

}

void LNwebLsectionsLdevices::LNsignalprocesslinkage(LNsignalprocessing* LNsignalprocess)
{
	LNLSLDsignalprocess = LNsignalprocess;
}

int LNwebLsectionsLdevices::GetLSlistforPsection(int* LSlistptr ,int Psection)
{
	int LScount = 0;

	// for given PS(Physical section), build the list of associated LS (logical sections)

	for (int LSsc = 0; LSsc < NUMBEROFLOGICALSECTIONS; LSsc++)
	{
		if (Logicalsectiondata[LSsc].Physectionref == Psection)
		{
			*(LSlistptr + LScount) = LSsc;
			LScount++;
		}

		if (Logicalsectiondata[LSsc].Physectionref2 == Psection)
		{
			*(LSlistptr + LScount) = LSsc;
			LScount++;
		}

	}

	return LScount;
}

void LNwebLsectionsLdevices::Initialtrainpositionsetup(LNallTrains* LNallTrainsinformation)
{
	int initLS = 0;
	int initPS = 0;
	int TrainNum = 0;
	int LocoNum = 0;
	int LocoDCC = 0;

	
	for (int tc = 1; tc < LNallTrainsinformation->GetnumberofTrainsloaded();tc++)
	{
		initLS = LNallTrainsinformation->GetCurrentLSlocation(tc);

		initPS = Logicalsectiondata[initLS].Physectionref;

		TrainNum = LNallTrainsinformation->GetTrainID(tc);

		std::cout<<"\n\n45==> Initialize Train:"<<tc<<" in location:"<<initLS<<" PS section:"<<initPS;

		// find vacant Train ID slot in section 

		for (int TiUpd = 1;TiUpd < NUMBEROF_LS_TRAIN_IDS;TiUpd++)
		{
			if (Logicalsectiondata[initLS].LogicalsectionTrainID[TiUpd] == 0)
			{
				Logicalsectiondata[initLS].LogicalsectionTrainID[TiUpd] = TrainNum;

				Logicalsectiondata[initLS].TrainIDcoupled[TiUpd] = 0;		// set train as not coupled

				PSectionset(initPS,1);

				//RefreshLastupdatedetails(1);

				LogicalSectionTcodeRefresh();

				LogicalDetectorupdateDetectorstatus();

				//LNLRprocess->LNlocosectionrefresh();

				std::cout<<"\n58 Open TrainID slot:"<<TiUpd;

				break;
			}
		}

	}
	
	
	for (int tc = 1; tc < MAXNUMBEROFLOCOMOTIVES;tc++)
	{
		int fetchedDCC = LNallTrainsinformation->GetLocoDCC(tc);
		
	//	printf("\n\n96 Fetched DCC %04d ",fetchedDCC);
		
		if (LNallTrainsinformation->GetLocoDCC(tc) > 0)
		{
		//	std::cout<<"\n96 GetLocoDCC(tc): "<<tc;
			
		 	initLS = LNallTrainsinformation->GetCurrentLocoLSlocation(tc);

			initPS = Logicalsectiondata[initLS].Physectionref;

			LocoNum = LNallTrainsinformation->GetTrainID(tc);

			LocoDCC = LNallTrainsinformation->GetLocoDCC(tc);

		//	std::cout<<"\n\n102==> Initialize Loco:"<<tc<<" with DCC:"<<LocoDCC<<" in LS location:"<<initLS<<" PS section:"<<initPS;

			PSectionset(initPS,1);

			Logicalsectiondata[initLS].LogicalsectionTcode = LocoDCC;

			Logicalsectiondata[initLS].LogicalsectionTrainID[0] = LocoDCC;

			LogicalSectionTcodeRefresh();

			LogicalDetectorupdateDetectorstatus();		
		}
		
	}
	
}

std::vector<int> LNwebLsectionsLdevices::GetLSdataforRte(int section)
{
	std::vector<int> LSinformation;

	/*
	 struct LogicalsectionArray
	{
		int Physectionref;
		int LogicalsectionTcode;
		int Logicalsectiondirection;
		int Logicalsectiontype;
		int Physectionref2;
		int Logicalsectionpair;
		int Sectionspeed;
		int Nextsection1;
		int Nextsection2;
		int Nextsection3;
		int Nextsection4;
	};

	struct LogicalsectionArray Logicalsectiondata[NUMBEROFLOGICALSECTIONS];

	*/

	LSinformation.push_back(section);
	LSinformation.push_back(Logicalsectiondata[section].Physectionref);
	LSinformation.push_back(Logicalsectiondata[section].Logicalsectiondirection);
	LSinformation.push_back(Logicalsectiondata[section].Logicalsectiontype);
	LSinformation.push_back(Logicalsectiondata[section].Physectionref2);
	LSinformation.push_back(Logicalsectiondata[section].Logicalsectionpair);
	LSinformation.push_back(Logicalsectiondata[section].Sectionspeed);

	LSinformation.push_back(Logicalsectiondata[section].Nextsection[0]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[1]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[2]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[3]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[4]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[5]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[6]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[7]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[8]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[9]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[10]);
	LSinformation.push_back(Logicalsectiondata[section].Nextsection[11]);

	//printf("\n182 Printing details of LS data vector .....");

	//for (std::vector<int>::iterator i = LSinformation.begin(); i != LSinformation.end(); ++i)
	//    std::cout << *i << ' ';

	return LSinformation;
}

int LNwebLsectionsLdevices::DisplayPhysicaltoLSmapping(char* replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	char* outputrecdisptr = &outputrec[0];

	int displaycharcount = 0;

		int isc,ic,scc,mcc;

		mcc = 0;

		bzero(outputrec,OUTPUTMSGBUFFERSIZE);

		sprintf(outputrec,"Display Physical to LS mapping with Train data \n----------------------------------------------");

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

		int MaxPsectionfound = GetmaxPSnumber();

		//std::cout<<"\n161 Max Psection found:"<<Psectioncount;

		bzero(outputrec,OUTPUTMSGBUFFERSIZE);

		sprintf(outputrec,"\n\nNumber of logical sections loaded in system : %i with %i logical detectors  ",NumberofLogicalSectionsloaded,NumberofLogicaldetectorsloaded);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

	for (int Psectioncnt = 1;Psectioncnt <= MaxPsectionfound;Psectioncnt++)
	{
		bzero(outputrec,OUTPUTMSGBUFFERSIZE);

		isc = DisplayPhysicaltoLSmappingDetail(Psectioncnt,outputrecdisptr);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrecdisptr[ic];
			mcc++;
		}
	}

	return mcc;
}

int LNwebLsectionsLdevices::GetmaxPSnumber()
{
	int PScountmax = 0;

	for (int psl = 0; psl < NumberofLogicalSectionsloaded;psl++)
	{
		if (Logicalsectiondata[psl].Physectionref > PScountmax)
			{
				PScountmax = Logicalsectiondata[psl].Physectionref;
			}
		}

	return PScountmax;
}

int LNwebLsectionsLdevices::TrainIDLSswitchover(int FromLS, int ToLS)
{
	int RC = 0;

	// switches Tcode and Train IDs from one section to another

	Logicalsectiondata[ToLS].LogicalsectionTcode = Logicalsectiondata[FromLS].LogicalsectionTcode;

	for (int trt = 0; trt < NUMBEROF_LS_TRAIN_IDS; trt++)
	{
		Logicalsectiondata[ToLS].LogicalsectionTrainID[trt] = Logicalsectiondata[FromLS].LogicalsectionTrainID[trt];
		Logicalsectiondata[ToLS].TrainIDcoupled[trt] 		= Logicalsectiondata[FromLS].TrainIDcoupled[trt];

		Logicalsectiondata[FromLS].LogicalsectionTrainID[trt] = 0;
		Logicalsectiondata[FromLS].TrainIDcoupled[trt] = 0;
		Logicalsectiondata[FromLS].LogicalsectionTcode = 0;
	}

	return RC;
}

int LNwebLsectionsLdevices::DisplayPhysicaltoLSmappingDetail(int Psectionfetch, char* replymessage)
{
	char outputrec[OUTPUTRECSIZE * 4];

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,OUTPUTRECSIZE * 4);

	std::vector <int>  PSLSlist;

	std::vector <int>  LSdatalist;

	std::vector <int>  LSdetectorlist;

	int LScounter    = 0;

	int LSTcode		 = 0;

	int LSTrID1		 = 0;
	int LSTRID1cf	 = 0;

	int LSTrID2		 = 0;
	int LSTRID2cf	 = 0;

	int LSTrID3		 = 0;
	int LSTRID3cf	 = 0;

	int LSTrID4		 = 0;
	int LSTRID4cf	 = 0;

	PSLSlist = CreateLSlist(Psectionfetch);

	//LSdetectorlist = CreateDetectorlist(Psectionfetch);

	bzero(outputrec,OUTPUTRECSIZE * 4);

	sprintf(outputrec,"\n\nPhysical Section: %03d: Has %i logical sections\n\n",Psectionfetch,PSLSlist.size());

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}


	for (std::vector<int>::iterator PSit = PSLSlist.begin() ; PSit != PSLSlist.end(); ++PSit)
	{
		bzero(outputrec,OUTPUTRECSIZE * 4);

		LScounter++;

		sprintf(outputrec,"LS:%i-%03d",LScounter, *PSit);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

		LSdatalist.clear();

		LSdatalist = GetLSdatalist(*PSit);

		bzero(outputrec,OUTPUTRECSIZE * 4);

		std::vector<int>::iterator LSDit = LSdatalist.begin();

		sprintf(outputrec," Tcode:%04d TrID-0:%03d TrID0-Cflag:%i TrID-1:%03d TrID1-Cflag:%i TrID-2:%03d TrID2-Cflag:%i TrID-3:%03d TrID3-Cflag:%i\n\n"\
		, *LSDit,*(LSDit+1),*(LSDit+2),*(LSDit+3),*(LSDit+4),*(LSDit+5),*(LSDit+6),*(LSDit+7),*(LSDit+8));

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	return mcc;

}

std::vector <int> LNwebLsectionsLdevices::GetLSdatalist(int LSsection)
{
	std::vector <int>  LSinfolist;

	LSinfolist.clear();

	LSinfolist.push_back(Logicalsectiondata[LSsection].LogicalsectionTcode);

	LSinfolist.push_back(Logicalsectiondata[LSsection].LogicalsectionTrainID[0]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].TrainIDcoupled[0]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].LogicalsectionTrainID[1]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].TrainIDcoupled[1]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].LogicalsectionTrainID[2]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].TrainIDcoupled[2]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].LogicalsectionTrainID[3]);

	LSinfolist.push_back(Logicalsectiondata[LSsection].TrainIDcoupled[3]);

	return LSinfolist;
}

std::vector <int>  LNwebLsectionsLdevices::CreateLSlist(int Psection)
{
	std::vector <int>  LSlist;

	LSlist.clear();

	for (int psl = 0; psl < NumberofLogicalSectionsloaded;psl++)
	{
		if ((Logicalsectiondata[psl].Physectionref == Psection) || (Logicalsectiondata[psl].Physectionref2 == Psection))
		{
			LSlist.push_back(psl);
		}
	}

	return LSlist;
}

std::vector <int>  LNwebLsectionsLdevices::CreateDetectorlist(int Psection)
{
	std::vector <int>  Dtrlist;

	Dtrlist.clear();

	for (int psl = 0; psl < NumberofLogicaldetectorsloaded;psl++)
	{
		if (Logicaldetectors[psl].LocoNetPSectiontrigger == Psection)
		{
			Dtrlist.push_back(psl);
		}
	}

	return Dtrlist;
}

int LNwebLsectionsLdevices::CreateLSLDextract(char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (scc = 1;scc <= NUMBEROPERATIONALLOGSECT;scc++)
				{
					bzero(outputrec,OUTPUTRECSIZE);
					sprintf(outputrec,"#%03d:%03d:%03d:%01d:%02d:%04d:%03d:%02d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:%03d:", scc,Logicalsectiondata[scc].Physectionref,\
									Logicalsectiondata[scc].Physectionref2,	Logicalsectiondata[scc].Logicalsectiondirection, Logicalsectiondata[scc].Logicalsectiontype,       Logicalsectiondata[scc].LogicalsectionTcode,\
									Logicalsectiondata[scc].Logicalsectionpair, Logicalsectiondata[scc].Sectionspeed, Logicalsectiondata[scc].Nextsection[0], \
									Logicalsectiondata[scc].Nextsection[1],Logicalsectiondata[scc].Nextsection[2],Logicalsectiondata[scc].Nextsection[3],\
									Logicalsectiondata[scc].Nextsection[4],Logicalsectiondata[scc].Nextsection[5],Logicalsectiondata[scc].Nextsection[6],\
									Logicalsectiondata[scc].Nextsection[7], Logicalsectiondata[scc].Nextsection[8], Logicalsectiondata[scc].Nextsection[9],\
									Logicalsectiondata[scc].Nextsection[10],Logicalsectiondata[scc].Nextsection[11]);

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}

					//*(replymessage + mcc) = '#';
				}

	bzero(outputrec,OUTPUTRECSIZE);

	sprintf(outputrec,"#9999:999:999:999:");

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}

	return mcc++;
}

void LNwebLsectionsLdevices::InitSignalRelayTable()
{
	// Initializes table used to drive the relay controlled signals
	// Hard coded details for initial implementation - Jun 30th 2020
	//
	
	int sc = 0;
	
	// clear all entries in the table
	
	for (sc = 0;sc < NUMBEROFSIGNALRELAYS;sc++)
	{
		Sectionsignaldata[sc].PSectnumber = 0;
		Sectionsignaldata[sc].Signalstopcode = 0X00;		
	}	
	
	// Section P 31
	
	Sectionsignaldata[0].PSectnumber = 32;			// set code for physical section 31
	Sectionsignaldata[0].Signalstopcode = 0X01;		// set code for Relay 1 - relay number B1 
	
	// Section P 32
	
	Sectionsignaldata[1].PSectnumber = 33;			// set code for physical section 32
	Sectionsignaldata[1].Signalstopcode = 0X02;		// set code for Relay 2 - relay number B2 
	
	Currsignalrelaycode = 0X00;
	
	Prevsignalrelaycode = 0X00;	
		
	//std::cout<<"\n\nRelay signal table initialized \n";
}

void LNwebLsectionsLdevices::UpdateSignalRelayoutputcode()
{
	int sc = 0;
	int Psc = 0;
	int Pscdata = 0;
	char Sectionmessage[SHORTMESSAGELENGTH ];	
	
	unsigned char signalrelaycode = 0X00;
	int signalchanged = 0;
	
	//std::cout<<"\n\nChecking signals  \n";		
	
	for (sc = 0;sc < NUMBEROFSIGNALRELAYS;sc++)
	{
		if (Sectionsignaldata[sc].PSectnumber > 0)
		{
			Psc = Sectionsignaldata[sc].PSectnumber;
			
			Pscdata = PSectiondatareturn(Psc,&Sectionmessage[0]);
			
			if (Pscdata > 0) 	// check to see if physical section is occupied 
			{
				signalrelaycode = signalrelaycode + Sectionsignaldata[sc].Signalstopcode; // if yes then add signal stop code to relay code
				signalchanged = 1;
				//printf("\nSignal triggered in section %i %i ",Psc,Pscdata);
			}
			else
			{
				//printf("\nSignal not triggered in section %i %i ",Psc,Pscdata);
			}			
		}	
		
		Currsignalrelaycode = signalrelaycode;
	}	
	
	//if (signalchanged > 0)
	//{
		//printf("\nStored Signal relay code changed to %02X", signalrelaycode);		
	//}		
		
}

unsigned char LNwebLsectionsLdevices::GetCurrrelaysignalcode()
{
	return Currsignalrelaycode;	
}

unsigned char LNwebLsectionsLdevices::GetPrevrelaysignalcode()
{
	return Prevsignalrelaycode;	
}

void LNwebLsectionsLdevices::Updateprevsignalrelaycode()
{
	Prevsignalrelaycode = Currsignalrelaycode;
	
	//std::cout<<"\nRelay signal code updated to "<< Prevsignalrelaycode;	
}

void LNwebLsectionsLdevices::ClearTcodes()
{
	int scc = 0;

	int scct = 0;

	for (scc = 0;scc< NUMBEROPERATIONALLOGSECT;scc++)
	{
		UpdateTcode(scc,0);

		for (scct = 0; scct < NUMBEROF_LS_TRAIN_IDS; scct++)
		{
			UpdateTrainID(scc,scct,0,0);
		}
	}
}

void LNwebLsectionsLdevices::ClearDcodes()
{
	int scc = 0;

	for (scc = 1;scc<= LOGICALDETECTORS;scc++)
	{
		UpdateDcode(scc,0);
	}
}

void LNwebLsectionsLdevices::UpdateDcode(int sc, int Dcode)
{
	//std::cout<<"\n236 =>  update device:"<<sc<<" with Dcode:"<<Dcode;

	Logicaldetectors[sc].Status = Dcode;
}


void LNwebLsectionsLdevices::UpdateTcode(int sc, int Tcode)
{
	Logicalsectiondata[sc].LogicalsectionTcode = Tcode;
}

void LNwebLsectionsLdevices::UpdateTrainID(int sc, int TrID, int Trainnumber, int actionflag)
{
	if ((actionflag == 6) && (Trainnumber > 0))		// clear trainID from the section if the train is coupled
	{
		// search for TrainID to get the subscript number for the storage table

		int TRidsubscript = CheckTrainIDinsection(sc,Trainnumber);
		int TRidcouplerflag = Logicalsectiondata[sc].TrainIDcoupled[TRidsubscript];

		//std::cout<<"\n317 TrainID delete sc:"<<sc<<" Trainnum:"<<Trainnumber<<" subscript:"<<TRidsubscript<<" Coupler flag:"<<TRidcouplerflag;

		if (TRidcouplerflag > 0)
		{
			Logicalsectiondata[sc].LogicalsectionTrainID[TRidsubscript] = 0;
			Logicalsectiondata[sc].TrainIDcoupled[TRidsubscript] = 0;
		//	std::cout<<"\n323 Flag 6 TrainID:"<<Trainnumber<<" cleared from sc:"<<sc;
		}

		return;
	}

	if (actionflag == 7)		// clear trainID from the section if it is not coupled
	{
			Logicalsectiondata[sc].LogicalsectionTrainID[TrID] = 0;
			Logicalsectiondata[sc].TrainIDcoupled[TrID] = 0;
			//std::cout<<"\n323 Flag 7 TrainID cleared from sc:"<<sc<<" Idnum:"<<TrID;
			return;
	}


	if ((TrID < NUMBEROF_LS_TRAIN_IDS) && (sc < NUMBEROFLOGICALSECTIONS))
	{

		if (actionflag == 4)
		{
			Logicalsectiondata[sc].TrainIDcoupled[TrID] = 1;
			Logicalsectiondata[sc].LogicalsectionTrainID[TrID] = Trainnumber;
			std::cout<<"\n\n===> 345 Trainnumber:"<<Trainnumber<<" TrainID:"<< TrID<<" Coupled to loco";
		}

		if (actionflag == 5)
		{
			Logicalsectiondata[sc].TrainIDcoupled[TrID] = 0;
			Logicalsectiondata[sc].LogicalsectionTrainID[TrID] = Trainnumber;
			//std::cout<<"\n264 Trainnumber:"<<Trainnumber<<" TrainID:"<< TrID<<" UNncoupled from loco";
		}

		if (actionflag == 8)
		{
			Logicalsectiondata[sc].TrainIDcoupled[TrID] = 1;
			//Logicalsectiondata[sc].LogicalsectionTrainID[TrID] = Trainnumber;
			std::cout<<"\n359 TrainID:"<< TrID<<"TrainNumber "<< Logicalsectiondata[sc].LogicalsectionTrainID[1]<<" coupled to loco";
		}
	}
}

int LNwebLsectionsLdevices::DisplayTrainIDs(int sc, char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	if (sc > 0)
	{

		bzero(outputrec,OUTPUTRECSIZE);

		sprintf(outputrec,"Section %i TrID\n1:  %i  %i\n2:  %i  %i\n3:  %i  %i\n4:  %i  %i\n ", sc, \
				Logicalsectiondata[sc].LogicalsectionTrainID[0],Logicalsectiondata[sc].TrainIDcoupled[0],\
				Logicalsectiondata[sc].LogicalsectionTrainID[1],Logicalsectiondata[sc].TrainIDcoupled[1],\
				Logicalsectiondata[sc].LogicalsectionTrainID[2],Logicalsectiondata[sc].TrainIDcoupled[2],\
				Logicalsectiondata[sc].LogicalsectionTrainID[3],Logicalsectiondata[sc].TrainIDcoupled[3]);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}

	}

	return mcc;

}

int LNwebLsectionsLdevices::GetLogicalSectiondirection(int section)
{
	return Logicalsectiondata[section].Logicalsectiondirection;
}

int LNwebLsectionsLdevices::GetLSTcode(int sc)
{
	//check to see if logical section requested is > 0

	if (sc > 0)
	{
		return Logicalsectiondata[sc].LogicalsectionTcode;
	}
	else
	{
		return 0;
	}
}

int LNwebLsectionsLdevices::CheckTrainIDinsection(int sc, int TrainIDnum)
{
	// routine to check if train ID exists within a section. It returns the subscript number and will be zero if it is not found

	int rc = 0;

	int numTrIds;

	//std::cout<<"\n1336 Checking TrainID in section:"<<sc<<" for TrainIDnum:"<<TrainIDnum;

	for (numTrIds = 1;numTrIds < NUMBEROF_LS_TRAIN_IDS; numTrIds++)
	{
		//std::cout<<"\n1340 Checking LS TrainID:"<<sc<<" ID:"<<Logicalsectiondata[sc].LogicalsectionTrainID[numTrIds];
		if (TrainIDnum == Logicalsectiondata[sc].LogicalsectionTrainID[numTrIds])
		{
			rc = numTrIds;  // TrainID matched so set return code equal to the subscript number
			break;
		}
	}

	return rc;
}

int LNwebLsectionsLdevices::GetLSTrainID(int sc, int TrainIDnum)
{
	//check to see if logical section requested is > 0

	if ((sc > 0) && (TrainIDnum < NUMBEROF_LS_TRAIN_IDS))
	{
		return Logicalsectiondata[sc].LogicalsectionTrainID[TrainIDnum];
	}
	else
	{
		return 0;
	}
}

int  LNwebLsectionsLdevices::SetTrainIDcoupled(int sc, int TrIDsubscript, int value)
{
	int rc = 0;

	Logicalsectiondata[sc].TrainIDcoupled[TrIDsubscript] = value;

	return 1;
}

int LNwebLsectionsLdevices::GetLSTrainIDcoupled(int sc, int TrainIDnum)
{
	if ((sc > 0) && (TrainIDnum < NUMBEROF_LS_TRAIN_IDS))
	{
			return Logicalsectiondata[sc].TrainIDcoupled[TrainIDnum];
	}
	else
	{
			return 0;
	}
}

int LNwebLsectionsLdevices::GetP1code(int sc)
{
	return Logicalsectiondata[sc].Physectionref;
}

int LNwebLsectionsLdevices::GetP2code(int sc)
{
	return Logicalsectiondata[sc].Physectionref2;
}

int LNwebLsectionsLdevices::GetPtype(int sc)
{
	return Logicalsectiondata[sc].Logicalsectiontype;
}

int LNwebLsectionsLdevices::GetLogicalSectionTcode(int section)
{
	 // Retrieves train code from the logical section requested

	 //printf("\nGet LS %03d Tcode %04d ", section,Logicalsectiondata[section].LogicalsectionTcode);

	 return Logicalsectiondata[section].LogicalsectionTcode;

	 //Logicalsectiondata[sccl].LogicalsectionTcode
}

int LNwebLsectionsLdevices::GetLDevicestatus(int dc)
{
	return Logicaldetectors[dc].Status;
}

int LNwebLsectionsLdevices::GetLDevicecountervalue(int dc)
{
	return DetectorTriggerTable[dc].ActualDevicecounter;
}

void LNwebLsectionsLdevices::UpdateLdevicestatus(int ld, int status)
{
	Logicaldetectors[ld].Status = status;
}

int LNwebLsectionsLdevices::GetNumberofDetectorsloaded()
{
	return NumberofLogicaldetectorsloaded;

}

int LNwebLsectionsLdevices::GetNumberofUncouplersloaded()
{
	return Uncouplerloadcount;
}

std::vector<int> LNwebLsectionsLdevices::GetPStoLSmapping(int Physection)
{
	//provides mapping of physical to logical sections - returns an integer vector

	std::vector<int> PLmapping;

	int sc = 0;

	PLmapping.clear();

	for (sc = 0;sc <= NumberofLogicalSectionsloaded;sc++)
	{
		if ((Logicalsectiondata[sc].Physectionref == Physection) || (Logicalsectiondata[sc].Physectionref2 == Physection))
		{
			PLmapping.push_back(sc);
		}
	}

	return PLmapping;
}

std::vector<int> LNwebLsectionsLdevices::GetDetectordata(int detector)
{
	std::vector<int> Detectordata;

	int dc = detector;

	Detectordata.clear();

	// push detector data into the vector
	/*
	Logicaldetectors[LNsection].Logicaldetector = LNsection;
	Logicaldetectors[LNsection].DPhysectionref = PHSection1;
	Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
	Logicaldetectors[LNsection].Status = 0;
	Logicaldetectors[LNsection].Detectortype = FL->GetRecordsFieldValue(lc,7);
	Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);


	 */


	Detectordata.push_back(Logicaldetectors[dc].Logicaldetector);
	Detectordata.push_back(Logicaldetectors[dc].Detectortype);
	Detectordata.push_back(Logicaldetectors[dc].DPhysectionref);
	Detectordata.push_back(Logicaldetectors[dc].Instancecountofthistype);
	Detectordata.push_back(Logicaldetectors[dc].DetectortriggerPhysectionref);

	return Detectordata;
}

std::vector<int> LNwebLsectionsLdevices::GetUncouplerdata(int uncoupler)
{
	std::vector<int> Uncouplerdata;

	Uncouplerdata.clear();

	// push uncoupler data into the vector

	Uncouplerdata.push_back(UncouplerReferencedata[uncoupler].Uncouplernumber);
	Uncouplerdata.push_back(UncouplerReferencedata[uncoupler].Psectionlocation);

	return Uncouplerdata;
}

int LNwebLsectionsLdevices::DisplayDcodesNG(int dflg,char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (scc = 1;scc <= LOGICALDETECTORS ;scc++)
			{
				bzero(outputrec,OUTPUTRECSIZE);

				sprintf(outputrec,":%02d-%01d", scc, Logicaldetectors[scc].Status);

				isc =  std::strlen(outputrec);

				for (ic = 0;ic <isc;ic++)
				{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
				}
			}

	return mcc++;
}


int LNwebLsectionsLdevices::DisplayDcodes(int dflg,char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	for (scc = 1;scc <= LOGICALDETECTORS ;scc++)
			{
				bzero(outputrec,OUTPUTRECSIZE);

				sprintf(outputrec,":%02d-%01d-%02d-%03d-%s", scc, Logicaldetectors[scc].Status,\
						Logicaldetectors[scc].Detectortype,Logicaldetectors[scc].Triggercount,Logicaldetectors[scc].RFidcode);

				isc =  std::strlen(outputrec);

				for (ic = 0;ic <isc;ic++)
				{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
				}
			}

	return mcc++;
}

int LNwebLsectionsLdevices::DisplayTcodes(int dflg,char* replymessage)
{
	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	int TrIDcnt;

	mcc = 0;

	for (scc = 1;scc < NUMBEROPERATIONALLOGSECT;scc++)
		{
			bzero(outputrec,OUTPUTRECSIZE);

			sprintf(outputrec,":%03d-%04d", scc,Logicalsectiondata[scc].LogicalsectionTcode);

			isc =  std::strlen(outputrec);
			//printf("\n%s",outputrec);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
			}

			for (TrIDcnt = 0;TrIDcnt < NUMBEROF_LS_TRAIN_IDS; TrIDcnt++)
			{
				bzero(outputrec,OUTPUTRECSIZE);
				sprintf(outputrec,"-%04d",Logicalsectiondata[scc].LogicalsectionTrainID[TrIDcnt]);
				isc =  std::strlen(outputrec);
				//printf("\n%s",outputrec);

				for (ic = 0;ic <isc;ic++)
				{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
				}
			}
		}

	return mcc++;
}

int LNwebLsectionsLdevices::DisplayLdevices(int sc, char* replymessage)
{
	/*
	struct Logicaldetectorarray
		{
			int Logicaldetector;
			int DPhysectionref;
			int Status; 				// 0 = not set, 1 = set
		};

		struct Logicaldetectorarray Logicaldetectors[LOGICALDETECTORS + 1];

	*/

	char outputrec[OUTPUTRECSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	if (sc != 0)
		{
			sprintf(outputrec,"LD:%03d: Phys:%03d: Status:%03d: \0",sc,Logicaldetectors[sc].DPhysectionref,Logicaldetectors[sc].Status);

			isc =  std::strlen(outputrec);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + ic) = outputrec[ic];
			}
				*(replymessage + isc) = '\n';

			return isc;
		}
	else
		{
			if (sc == 0)
			{
				for (scc = 0;scc < LOGICALDETECTORS ;scc++)
				{
					bzero(outputrec,OUTPUTRECSIZE);

					sprintf(outputrec,"LD:%03d: Phys:%03d: Status:%03d: Type:%02d \n",scc,Logicaldetectors[scc].DPhysectionref, \
							Logicaldetectors[scc].Status, Logicaldetectors[scc].Detectortype);

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
						{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
						}
				}

				*(replymessage + mcc) = '\n';

				return mcc++;
			}
		}

	return 0;
}

int LNwebLsectionsLdevices::DisplayLsections(int sc, char* replymessage)
{
	char outputrec[OUTMSGBUFFERSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	if (sc != 0)
	{
		sprintf(outputrec,"LS:%03d: Phys1:%03d: Physy2:%03d: Dir: %03d: Tcode: %04d: LSpair: %03d: \0", sc,Logicalsectiondata[sc].Physectionref,\
				Logicalsectiondata[sc].Physectionref2,	Logicalsectiondata[sc].Logicalsectiondirection, Logicalsectiondata[sc].LogicalsectionTcode,\
				Logicalsectiondata[sc].Logicalsectionpair);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + ic) = outputrec[ic];
		}

		*(replymessage + isc) = '\n';

		return isc;
	}
	else
	{
		if (sc == 0)
		{
			for (scc = 0;scc <= NUMBEROPERATIONALLOGSECT;scc++)
			{
				bzero(outputrec,OUTMSGBUFFERSIZE);

				if (Logicalsectiondata[scc].Logicalsectionpair > 0)
				{
					sprintf(outputrec,"\nLS:%03d: LSpartner:%03d: Direction %01d Tcode: %04d: Speed:%02d", scc,Logicalsectiondata[scc].Logicalsectionpair,\
								Logicalsectiondata[scc].Logicalsectiondirection, Logicalsectiondata[scc].LogicalsectionTcode,Logicalsectiondata[scc].Sectionspeed);
					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}

					sprintf(outputrec," PS1:%03d PS2:%03d GUIref1:%03d GUIref2:%03d GUIref3:%03d GUIref4:%03d\n",Logicalsectiondata[scc].Physectionref,\
							Logicalsectiondata[scc].Physectionref2,Logicalsectiondata[scc].GUIobjectreference[0],Logicalsectiondata[scc].GUIobjectreference[1],\
							Logicalsectiondata[scc].GUIobjectreference[2],Logicalsectiondata[scc].GUIobjectreference[3]);

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}

					sprintf(outputrec,"NS1:%03d NS2:%03d NS3:%03d NS4:%03d NS5:%03d NS6:%03d\n",Logicalsectiondata[scc].Nextsection[0],\
							Logicalsectiondata[scc].Nextsection[1],Logicalsectiondata[scc].Nextsection[2],Logicalsectiondata[scc].Nextsection[3],\
							Logicalsectiondata[scc].Nextsection[4],Logicalsectiondata[scc].Nextsection[5]);

					isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
						*(replymessage + mcc) = outputrec[ic];
						mcc++;
					}

					*(replymessage + mcc) = '\n';
				}
			}

			return mcc++;
		}
	}

	return 0;
}

int LNwebLsectionsLdevices::GetNumberofLSloaded()
{
	return NumberofLogicalSectionsloaded;
}

int LNwebLsectionsLdevices::GetMaxsectionnumberloaded()
{
	return Maxsectionnumberloaded;
}

void LNwebLsectionsLdevices::RFidinit(int devicenum,int trigstatus)
{
	for (int rfidcc = 0;rfidcc < RFIDCHARCOUNT; rfidcc++)
	{
		Logicaldetectors[devicenum].RFidcode[rfidcc] = 'x';
	}

	Logicaldetectors[devicenum].RFidcode[RFIDCHARCOUNT-1] = '\0';

	Logicaldetectors[devicenum].Statustotriggertimer = trigstatus;       // status to trigger time

	Logicaldetectors[devicenum].Statustriggertimestamp = 0;     // time when detector went to triggered status

	Logicaldetectors[devicenum].Durationsincestatuschange = 0;
}

void LNwebLsectionsLdevices::InitLogicalsectionmap()
{
	// initialize LS mapping tables

	for (int Ilst = 0; Ilst <= NUMBEROFLOGICALSECTIONS;Ilst++)
	{
		Logicalsectiondata[Ilst].Physectionref = 0;
		Logicalsectiondata[Ilst].Physectionref2 = 0;
		Logicalsectiondata[Ilst].Logicalsectiondirection = 0;
		Logicalsectiondata[Ilst].Sectionspeed		= 0;
		Logicalsectiondata[Ilst].LogicalsectionTcode = 0;

		for (int grefs = 0; grefs < MAXGUIREFSFORLS; grefs++)
		{
			Logicalsectiondata[Ilst].GUIobjectreference[grefs] = 0;
		}

		Logicalsectiondata[Ilst].Logicalsectionpair = 0;
		Logicalsectiondata[Ilst].Logicalsectiontype = 0;

		for (int lns = 0; lns < MAXNUMBEROFLSNEXTSECTIONS; lns++)
		{
			Logicalsectiondata[Ilst].Nextsection[lns] = 0;
		}

		for (int Tid = 0; Tid <= NUMBEROF_LS_TRAIN_IDS; Tid++)
		{
			Logicalsectiondata[Ilst].LogicalsectionTrainID[Tid] = 0;
			Logicalsectiondata[Ilst].TrainIDcoupled[Tid]		= 0;
		}
	}

	NumberofLogicalSectionsloaded = 0;

	Maxsectionnumberloaded  = 0;
}

int LNwebLsectionsLdevices::GetLogicaldevicetriggercount(int device)
{
		return DetectorTriggerTable[device].ActualDevicecounter;
}

int LNwebLsectionsLdevices::LNlogicaldeviceconfigload(char* lineinputbufferptr, int displayflag)
{
	//std::cout<<"\n1171 Reed switch config record "<<lineinputbufferptr;

	int Logicaldetectornumber;				// detector number
	int DPhysectionref;					// Loconet Physical section that indicates the device is triggered
	int LocoNetPSectiontrigger;			// Loconet physical section that triggers the device
	int Status; 						// 0 = not set, 1 = set
	int Detectortype;					// 01 - reed switch, 02 RFid reader
	int DetectortriggerPhysectionref;
	int Instancecountofthistype;		// instance count of a detector type

	int RC = 0;

	int lc = 0;

	int value = 0;

	int Invalidfieldcount= 0;

	int conversionRC;

	int* valueptr = &value;

	Bufferprocessing* BPLD = new Bufferprocessing();

	BPLD->Fileparse(lineinputbufferptr, ';', 0);

	value = 0;

	if (BPLD->ParamdataInteger(5,valueptr) > 0)
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field 5";
			Invalidfieldcount++;
	}
	else
	{
		Logicaldetectornumber  = value;
	}

	if (BPLD->ParamdataInteger(6,valueptr) > 0)
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 6";
		Invalidfieldcount++;
	}
	else
	{
		Detectortype  = value;
	}

	if (BPLD->ParamdataInteger(7,valueptr) > 0)
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 7";
		Invalidfieldcount++;
	}
	else
	{
		DPhysectionref = value;
	}

	if (BPLD->ParamdataInteger(8,valueptr) > 0)
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 8";
		Invalidfieldcount++;
	}
	else
	{
		LocoNetPSectiontrigger = value;
	}

	if (Invalidfieldcount == 0)
	{
		Logicaldetectors[Logicaldetectornumber].Logicaldetector = Logicaldetectornumber;
		Logicaldetectors[Logicaldetectornumber].DPhysectionref = DPhysectionref;
		Logicaldetectors[Logicaldetectornumber].DetectortriggerPhysectionref = LocoNetPSectiontrigger;
		Logicaldetectors[Logicaldetectornumber].Status = 0;
		Logicaldetectors[Logicaldetectornumber].Detectortype = Detectortype;
		Logicaldetectors[Logicaldetectornumber].Instancecountofthistype = Logicaldetectornumber;
		NumberofReeddetectorsloaded++;
		NumberofLogicaldetectorsloaded++;
	}

	return Invalidfieldcount;
}

int LNwebLsectionsLdevices::LNlogicalsectionconfigload(char* lineinputbufferptr, int displayflag)
{
	// processes messages from LNconfigloader which is processing configuration details from new AllConfiguration file. Function is called with a pointer to the
	// the configuration record that is being processed.

	/***********************************************************************
			/
			/
			/  Record number 		  	  000
			/  Logical section  	  	  000
			/  Direction                  0 = backwards  1 = forward
			/  Physical section 1         000
			/  Type                       1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
			/  Physical section 2 	      000
			/  Logical partner            000
			/
			/  Data line read :003;003;0;002;1;000;004;
			/
			/ X - end of file

			struct LogicalsectionArray
			{
			int Physectionref;
			int LogicalsectionTcode;
			int Logicalsectiondirection;
			int Logicalsectiontype;
			int Physectionref2;
			int Logicalsectionpair;
			int Sectionspeed;
			int Nextsection1;
			int Nextsection2;
			int Nextsection3;
			int Nextsection4;
			};

			struct LogicalsectionArray Logicalsectiondata[NUMBEROFLOGICALSECTIONS];

			struct Logicaldetectorarray
			{
				int Logicaldetector;
				int DPhysectionref;
				int Status; 				// 0 = not set, 1 = set
			};

			struct Logicaldetectorarray Logicaldetectors[LOGICALDETECTORS + 1];

			*/

	int LNsection 					=  0; //BPLL->Paramdata(3);
	int LNDirection 				=  0; //BPLL->Paramdata(4);
	int PHSection1 					=  0; //BPLL->Paramdata(5);
	int LNType						=  0; //BPLL->Paramdata(6);
	int PHSection2      			=  0; //BPLL->Paramdata(7);
	int LNGUIobjectref				=  0; //BPLL->Paramdata(8);
	int LNsectionpair   			=  0; //BPLL->Paramdata(9);
	int LNsectionspeed				=  0; //BPLL->Paramdata(10);
	int LNsubType					=  0; //BPLL->Paramdata(10);
	int LNdetectortype  			=  0; //
	int LNlogicalsignalreference 	=  0;
	int LNnextsection[MAXNUMBEROFLSNEXTSECTIONS]; //BPLL->Paramdata(11);
	int LNnumberofNextsectionsloaded = 0;
	int LNGUIreferences[MAXGUIREFSFORLS];
	int LNnumberofGUIrerefencesloaded = 0;

	for (int nsc = 0;nsc < MAXNUMBEROFLSNEXTSECTIONS;nsc++)
	{
		LNnextsection[nsc] = 0;
	}

	for (int grc = 0;grc < MAXGUIREFSFORLS;grc++)
	{
		LNGUIreferences[grc] = 0;
	}

	int RC = 0;

	int lc = 0;

	int value = 0;

	int Invalidfieldcount= 0;

	int conversionRC;

	int* valueptr = &value;

	//std::cout<<"\n1383 LS record read->"<<lineinputbufferptr;

	Bufferprocessing* BPLL = new Bufferprocessing();

	/*  Note: Fields delimited by ; character
	/*        Fields sub-delimited by + character
	/*
	/*        Record ends with a # character
	/*
	/*	4		  	000		int type Record number
	/*  8           000     int type 10 record number
	/*	12			000		int logical section number
	/*	16			000		int Logical section partner e.g opposite direction
	/*  20			0		int logical section direction 0 = forwards  1 = backwards

	/*	22			000		int Physical section 1
	/*	26			000		int Physical section 2
	/*	30			00		int Section speed code

	/*  32          00      int detector type if applicable
	/*  35          000     int Logical signal reference
	/*	39			0		int Section type  1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
	/*  41          000     int GUI object reference number - split by + character
	/*	00			000		int Next logical section  - split by + character  */


	int fieldcount = BPLL->Fileparse(lineinputbufferptr, ';', 0);

	//std::cout<<"\n1415 Field count:"<< fieldcount;

	//delete BPLL;

	//return 0;

	value = 0;

	if (BPLL->ParamdataInteger(3,valueptr) > 0)  // LNsection number
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 3";
		Invalidfieldcount++;
	}
	else
	{
		LNsection  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(4,valueptr) > 0)		// LNsection partner / pair
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 4";
		Invalidfieldcount++;
	}
	else
	{
		LNsectionpair  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(5,valueptr) > 0)    // LNsection direction
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 5";
		std::cout<<"\nInvalid line:"<<lineinputbufferptr;
		Invalidfieldcount++;
	}
	else
	{
		LNDirection  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(6,valueptr) > 0)   // Physical section 1
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 6";
		std::cout<<"\nInvalid line:"<<lineinputbufferptr;
		Invalidfieldcount++;
	}
	else
	{
		PHSection1  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(7,valueptr) > 0)     // Physical section 2
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 7";
		std::cout<<"\nInvalid line:"<<lineinputbufferptr;
		Invalidfieldcount++;
	}
	else
	{
		PHSection2  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(8,valueptr) > 0)		// LN section speed
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field 8";
			std::cout<<"\nInvalid line:"<<lineinputbufferptr;
			Invalidfieldcount++;
	}
	else
	{
		LNsectionspeed = value;
	}

	if (BPLL->ParamdataInteger(9,valueptr) > 0)    // LN detector type
	{
		std::cout<<"\nNon-numeric char in lineinputbuffer field 9";
		std::cout<<"\nInvalid line:"<<lineinputbufferptr;
		Invalidfieldcount++;
	}
	else
	{
		LNdetectortype  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(10,valueptr) > 0)   // LN logical signal reference
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field 10";
			std::cout<<"\nInvalid line:"<<lineinputbufferptr;
			Invalidfieldcount++;
	}
	else
	{
		LNlogicalsignalreference  = value;
	}

	value = 0;

	if (BPLL->ParamdataInteger(11,valueptr) > 0)
	{
			std::cout<<"\nNon-numeric char in lineinputbuffer field 11";   // LN subtype
			std::cout<<"\nInvalid line:"<<lineinputbufferptr;
			Invalidfieldcount++;
	}
	else
	{
		LNType  = value;
	}

	// split field 12 to extract GUI reference fields - delimited by + character

	Bufferprocessing* BPFX = new Bufferprocessing();

	int GUIfc = BPFX->Fileparse(BPLL->Paramdata(12), '+', 0);

	//std::cout<<"\n1540 GUIfc:"<<GUIfc<<"\n";

	// extract GUI reference

	for (int Gcc = 0;Gcc<GUIfc;Gcc++)
	{
		value = 0;
		BPFX->ParamdataInteger(Gcc,valueptr);
		LNGUIreferences[Gcc] = value;
	}

	//std::cout<<"\n1551 GUIref0:"<<LNGUIreferences[0]<<" GUIref1:"<<LNGUIreferences[1]<<" GUIref2:"<<LNGUIreferences[2]<<" GUIref3:"<<LNGUIreferences[3];

	// extract Next section details

	int NSfc = BPFX->Fileparse(BPLL->Paramdata(13), '+', 0);

	for (int NScc = 0;NScc<NSfc;NScc++)
		{
			value = 0;
			BPFX->ParamdataInteger(NScc,valueptr);
			LNnextsection[NScc] = value;
		}

	//std::cout<<"\n1564 LNNS0:"<<LNnextsection[0]<<" LNNS1:"<<LNnextsection[1];

	delete BPFX;

	//return 0;

	if (Invalidfieldcount > 0)
	{
		std::cout<<"\nInvalid field on Lineinput->"<<lineinputbufferptr;
		RC = 8;
	}
	else
	{
		    /*	12			000		int logical section number
			/*	16			000		int Logical section partner e.g opposite direction
			/*  20			0		int logical section direction 0 = forwards  1 = backwards

			/*	22			000		int Physical section 1
			/*	26			000		int Physical section 2
			/*	30			00		int Section speed code

			/*  32          00      int detector type if applicable
			/*  35          000     int Logical signal reference
			/*	39			0		int Section type  1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
			/*  41          000     int GUI object reference number - split by + character
			/*	00			000		int Next logical section  - split by + character  */
		if (displayflag > 0)
		{
			printf("\nLS record\n");
			printf("\nLSection:%03d LSpartner:%03d Direction:%01d Psection1:%03d Psection2:%03d Speed:%01d Detector:%02d Logicalsignal:%02d SectionType:%01d",LNsection,LNsectionpair,\
					LNDirection,PHSection1,PHSection2,LNsectionspeed,LNdetectortype,LNlogicalsignalreference,LNType);
			printf("\n");

			for (int Gcc = 0;Gcc < GUIfc;Gcc++)
			{
				printf(" GUIref:%02d->%02d\n",Gcc,LNGUIreferences[Gcc]);
			}

			for (int NScc = 0;NScc<NSfc;NScc++)
			{
				printf(" NS:%02d->%02d\n",NScc,LNnextsection[NScc]);
			}
		}
	}

	if ((LNType == 1) || (LNType == 2))
	{
		Logicalsectiondata[LNsection].Physectionref = PHSection1;
		Logicalsectiondata[LNsection].LogicalsectionTcode = 0;
		Logicalsectiondata[LNsection].Logicalsectiondirection = LNDirection;
		Logicalsectiondata[LNsection].Physectionref2 = PHSection2;
		Logicalsectiondata[LNsection].Logicalsectiontype = LNType;
		Logicalsectiondata[LNsection].Logicalsectionpair = LNsectionpair;

		Logicalsectiondata[LNsection].Sectionspeed = LNsectionspeed;

		for (int Gcc = 0;Gcc < GUIfc;Gcc++)
		{
			Logicalsectiondata[LNsection].GUIobjectreference[Gcc] = LNGUIreferences[Gcc];
			if (Gcc == (MAXGUIREFSFORLS - 1))
			{
				std::cout<<"\n1625 GUI reference count overrun for record:"<<lineinputbufferptr;
				break;
			}
		}

		for (int NScc = 0;NScc<NSfc;NScc++)
		{
			Logicalsectiondata[LNsection].Nextsection[NScc] = LNnextsection[NScc];
			if (NScc == (MAXNUMBEROFLSNEXTSECTIONS - 1))
			{
				std::cout<<"\n1635 Next section count overrun for record:"<<lineinputbufferptr;
				break;
			}
		}

		int trid = 0;

		for (trid = 0; trid < NUMBEROF_LS_TRAIN_IDS; trid++)
		{
			Logicalsectiondata[LNsection].LogicalsectionTrainID[trid] = 0;  // initialize TRid fields
			Logicalsectiondata[LNsection].TrainIDcoupled[trid] 		  = 0;
		}

			NumberofLogicalSectionsloaded++;

	   if (LNsection > Maxsectionnumberloaded)
	   {
		   Maxsectionnumberloaded = LNsection;
	   }
	}

	if (LNType == 6)
	{
		//printf("\n1411...Uncoupler ramp record found %i %i %i \n",LNsection, PHSection1, LNDirection, LNType, PHSection2);
		Uncouplerloadcount++;
		UncouplerReferencedata[Uncouplerloadcount].Uncouplernumber = LNsection;
		UncouplerReferencedata[Uncouplerloadcount].Psectionlocation = PHSection1;
		UncouplerReferencedata[Uncouplerloadcount].UCtriggerstatus	= 0;

		UncouplerReferencedata[0].Uncouplernumber = 0;
		UncouplerReferencedata[0].Psectionlocation = 0;
		UncouplerReferencedata[0].UCtriggerstatus	= 0;
	}

	if (LNType == 7)
	{
		//	printf("\n1422...Vertical Infra-red detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

											/*
												int Logicaldetector;
												int DPhysectionref;			// Physical section location of the device
												int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
												int Status; 				// 0 = not set, 1 = set
												int Detectortype;			// 01 - reed switch, 02 RFid reader
												int DetectortriggerPhysectionref;
												int Instancecountofthistype;		// instance count of a detector type

											*/
			Logicaldetectors[LNsection].Logicaldetector = LNsection;
			Logicaldetectors[LNsection].DPhysectionref = PHSection1;
			Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
			Logicaldetectors[LNsection].Status = 0;
			Logicaldetectors[LNsection].Detectortype = LNType;
		//	Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
			RFidinit(LNsection,0);
			NumberofLogicaldetectorsloaded++;
			NumberofVerticalIFRdetectorsloaded++;
											//	printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
	}

	if (LNType == 8)
	{
		//	printf("\n1448...Horizontal Infra-red detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

														/*
															int Logicaldetector;
															int DPhysectionref;			// Physical section location of the device
															int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
															int Status; 				// 0 = not set, 1 = set
															int Detectortype;			// 01 - reed switch, 02 RFid reader
															int DetectortriggerPhysectionref;
															int Instancecountofthistype;		// instance count of a detector type

														*/
			Logicaldetectors[LNsection].Logicaldetector = LNsection;
			Logicaldetectors[LNsection].DPhysectionref = PHSection1;
			Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
			Logicaldetectors[LNsection].Status = 0;
			Logicaldetectors[LNsection].Detectortype = LNType;
			//	Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
			RFidinit(LNsection,1);
			NumberofLogicaldetectorsloaded++;
			NumberofHorizontalIFRdetectorsloaded++;
			//		printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
	}

	if (LNType == 9)
	{
	//	printf("\n1474...RFid detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

		/*
									int Logicaldetector;
									int DPhysectionref;			// Physical section location of the device
									int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
									int Status; 				// 0 = not set, 1 = set
									int Detectortype;			// 01 - reed switch, 02 RFid reader
									int DetectortriggerPhysectionref;
									int Instancecountofthistype;		// instance count of a detector type

		*/
			Logicaldetectors[LNsection].Logicaldetector = LNsection;
			Logicaldetectors[LNsection].DPhysectionref = PHSection1;
			Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
			Logicaldetectors[LNsection].Status = 0;
			Logicaldetectors[LNsection].Detectortype = LNType;
		//	Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
			RFidinit(LNsection,0);
			NumberofLogicaldetectorsloaded++;
			NumberofRFiddetectorsloaded++;
			//	printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
	}
	
	delete BPLL;

	return RC;
}

int LNwebLsectionsLdevices::LNLogicalsectionmapfileload()
{
	int RC = 0;
	
	LNwebmonNGfileload* FL = new LNwebmonNGfileload();

	std::string FileDir = WEBDirectory;	
	
	std::string Sectionmapfile = FileDir + SectionMappingnamestr;

	int fln = Sectionmapfile.length();

	char Filenamechar_array[fln+1];

	strcpy(Filenamechar_array, Sectionmapfile.c_str());
	
	std::cout<<"\nLoading LS section file :"<<Sectionmapfile;

	if (FL->Fileread(Filenamechar_array,0) > 0)
		{
			std::cout << "\n\n***** Section mapping file load processing for " << Sectionmapfile << " failed **************  \n";
			delete FL;
			RC = 8;
		}
	else
	{
		std::cout<<"\nNumber of LS section config records loaded "<< FL->GetNumberofDatarecordsloaded()<<" \n\n";

		//int LNsectionnumber;
		int LNsection;
		int PHSection1;
		int PHSection2;
		int LNDirection;
		int LNType;
		int LNsubType;
		//int LNdirectionflag;
		int LNsectionpair;
		int LNsectionspeed;

		int LNnextsection1;
		int LNnextsection2;
		int LNnextsection3;
		int LNnextsection4;

		int lc = 0;

		/***********************************************************************
		/
		/
		/  Record number 		  	  000
		/  Logical section  	  	  000
		/  Direction                  0 = backwards  1 = forward
		/  Physical section 1         000
		/  Type                       1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
		/  Physical section 2 	      000
		/  Logical partner            000
		/
		/  Data line read :003;003;0;002;1;000;004;
		/
		/ X - end of file

		struct LogicalsectionArray
		{
		int Physectionref;
		int LogicalsectionTcode;
		int Logicalsectiondirection;
		int Logicalsectiontype;
		int Physectionref2;
		int Logicalsectionpair;
		int Sectionspeed;
		int Nextsection1;
		int Nextsection2;
		int Nextsection3;
		int Nextsection4;
		};

		struct LogicalsectionArray Logicalsectiondata[NUMBEROFLOGICALSECTIONS];

		struct Logicaldetectorarray
		{
			int Logicaldetector;
			int DPhysectionref;
			int Status; 				// 0 = not set, 1 = set
		};

		struct Logicaldetectorarray Logicaldetectors[LOGICALDETECTORS + 1];

		*/

		for(lc = 1;lc <= FL->GetNumberofDatarecordsloaded();lc++)
		    {
			LNsectionnumber = FL->GetRecordsFieldValue(lc,1);   // section number
			LNsection = FL->GetRecordsFieldValue(lc,1);        // section code
			LNDirection = FL->GetRecordsFieldValue(lc,2);       // section direction
			PHSection1 = FL->GetRecordsFieldValue(lc,3);		// physical section
			PHSection2 = FL->GetRecordsFieldValue(lc,5);		// physical section
			LNType = FL->GetRecordsFieldValue(lc,4);
			LNsectionpair = FL->GetRecordsFieldValue(lc,6);
			LNsectionspeed = FL->GetRecordsFieldValue(lc,7);
			LNsubType = FL->GetRecordsFieldValue(lc,7);
			LNnextsection1 = FL->GetRecordsFieldValue(lc,8);
			LNnextsection2 = FL->GetRecordsFieldValue(lc,9);
			LNnextsection3 = FL->GetRecordsFieldValue(lc,10);
			LNnextsection4 = FL->GetRecordsFieldValue(lc,11);
			//std::cout<<"\n625 Section Record read:"<<LNsection<< " Direction:"<<LNDirection<<" PHsection1:"<<PHSection1<< " LNnextsection1:" << LNnextsection1<<" LNType:"<<LNType;

			// detector types   05 = Reed switch , 07 = Infra-red-vertical, 08 = Infra-red-horizontal, 09 = RFid reader
			/*
			if (LNType == 5)
			{
					printf("\n629...Reed switch Logical detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);


					int Logicaldetector;
					int DPhysectionref;			// Physical section location of the device
					int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
					int Status; 				// 0 = not set, 1 = set
					int Detectortype;			// 01 - reed switch, 02 RFid reader
					int DetectortriggerPhysectionref;
					int Instancecountofthistype;		// instance count of a detector type


					Logicaldetectors[LNsection].Logicaldetector = LNsection;
					Logicaldetectors[LNsection].DPhysectionref = PHSection1;
					Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
					Logicaldetectors[LNsection].Status = 0;
					Logicaldetectors[LNsection].Detectortype = LNType;
					Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
					RFidinit(LNsection,0);
					NumberofLogicaldetectorsloaded++;
					NumberofReeddetectorsloaded++;
				//	printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
			}
			*/
			/*
			if (LNType == 9)
			{
					printf("\n629...RFid detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

							/*
								int Logicaldetector;
								int DPhysectionref;			// Physical section location of the device
								int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
								int Status; 				// 0 = not set, 1 = set
								int Detectortype;			// 01 - reed switch, 02 RFid reader
								int DetectortriggerPhysectionref;
								int Instancecountofthistype;		// instance count of a detector type


								Logicaldetectors[LNsection].Logicaldetector = LNsection;
								Logicaldetectors[LNsection].DPhysectionref = PHSection1;
								Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
								Logicaldetectors[LNsection].Status = 0;
								Logicaldetectors[LNsection].Detectortype = LNType;
								Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
								RFidinit(LNsection,0);
								NumberofLogicaldetectorsloaded++;
								NumberofRFiddetectorsloaded++;
							//	printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
			}
			*/
			/*
			if (LNType == 7)
			{
								printf("\n629...Vertical Infra-red detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

										/*
											int Logicaldetector;
											int DPhysectionref;			// Physical section location of the device
											int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
											int Status; 				// 0 = not set, 1 = set
											int Detectortype;			// 01 - reed switch, 02 RFid reader
											int DetectortriggerPhysectionref;
											int Instancecountofthistype;		// instance count of a detector type


											Logicaldetectors[LNsection].Logicaldetector = LNsection;
											Logicaldetectors[LNsection].DPhysectionref = PHSection1;
											Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
											Logicaldetectors[LNsection].Status = 0;
											Logicaldetectors[LNsection].Detectortype = LNType;
											Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
											RFidinit(LNsection,0);
											NumberofLogicaldetectorsloaded++;
											NumberofVerticalIFRdetectorsloaded++;
										//	printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
			}
			*/
			/*
			if (LNType == 8)
			{
					printf("\n629...Horizontal Infra-red detector found %i %i %i %i %i %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,LNsectionspeed);

													/*
														int Logicaldetector;
														int DPhysectionref;			// Physical section location of the device
														int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
														int Status; 				// 0 = not set, 1 = set
														int Detectortype;			// 01 - reed switch, 02 RFid reader
														int DetectortriggerPhysectionref;
														int Instancecountofthistype;		// instance count of a detector type


														Logicaldetectors[LNsection].Logicaldetector = LNsection;
														Logicaldetectors[LNsection].DPhysectionref = PHSection1;
														Logicaldetectors[LNsection].DetectortriggerPhysectionref = PHSection2;
														Logicaldetectors[LNsection].Status = 0;
														Logicaldetectors[LNsection].Detectortype = LNType;
														Logicaldetectors[LNsection].Instancecountofthistype = FL->GetRecordsFieldValue(lc,8);
														RFidinit(LNsection,1);
														NumberofLogicaldetectorsloaded++;
														NumberofHorizontalIFRdetectorsloaded++;
												//		printf("\n1180 Logical detector record found %i %i %i type: %i %i ts: %i instc: %i\n",LNsection, PHSection1, LNDirection, LNType, PHSection2,FL->GetRecordsFieldValue(lc,7),FL->GetRecordsFieldValue(lc,8));
			}

			if (LNType == 6)
			{
					printf("\n645...Uncoupler ramp record found %i %i %i \n",LNsection, PHSection1, LNDirection, LNType, PHSection2);
					Uncouplerloadcount++;
					UncouplerReferencedata[Uncouplerloadcount].Uncouplernumber = LNsection;
					UncouplerReferencedata[Uncouplerloadcount].Psectionlocation = PHSection1;

					UncouplerReferencedata[0].Uncouplernumber = 0;
					UncouplerReferencedata[0].Psectionlocation = 0;
			}
			*/

		   }

	delete FL;
	
	std::cout<<"\n1354...Number of LS sections loaded :"<<NumberofLogicalSectionsloaded;

	std::cout<<"\n1356...Number of detectors  loaded:"<<NumberofLogicaldetectorsloaded;
	std::cout<<"\n1357...Number of uncouplers loaded:"<<Uncouplerloadcount;

	return RC;
	}
}

void LNwebLsectionsLdevices::Complexpointprocessing()
{
	// process points within complex point clusters

	int CFLStcode 	= 0;
	int CBLStcode	= 0;

	//std::cout<<"\n\nComplex point processing ....";

	/*
	 int LogicalPointnumber;
			int ForwardLS;
			int ForwardLSTcode;
			int ForwardprevLSTcode;
			int BackLS;
			int BackLSTcode;
			int BackprevLSTcode;
			int Numberofphysicalpoints;
			int Pointcode[POINTSINCLUSTER];
			int Pointpositions[POINTSINCLUSTER];
			int CurrOpenClose[POINTSINCLUSTER];
			int PrevOpenClose[POINTSINCLUSTER];
	 */

	for (int cpc = 1; cpc < NUMBEROFCOMPLEXPOINTS; cpc++)
	{
		//std::cout<<"\nChecking Logical point "<<ComplexPointprocessTable[cpc].LogicalPointnumber;
		if (ComplexPointprocessTable[cpc].LogicalPointnumber > 0)
		{
			// fetch current Tcodes for Forward and backward sections for all points

			ComplexPointprocessTable[cpc].ForwardLSTcode = GetLogicalSectionTcode(ComplexPointprocessTable[cpc].ForwardLS);

			ComplexPointprocessTable[cpc].BackLSTcode = GetLogicalSectionTcode(ComplexPointprocessTable[cpc].BackLS);

				if (ComplexPointprocessTable[cpc].ForwardLSTcode != ComplexPointprocessTable[cpc].ForwardprevLSTcode)  // check to see if forward LS code has changed
				{
					if ((ComplexPointprocessTable[cpc].ForwardLSTcode > 0) && (ComplexPointprocessTable[cpc].ForwardLSTcode != 999))
					{
						for (int pcs = 1; pcs < POINTSINCLUSTER; pcs++)
						{
							if (ComplexPointprocessTable[cpc].Pointcode[pcs] > 0)  // check to see which points need to be switched
							{
								CSwitchmsgprocess(ComplexPointprocessTable[cpc].Pointcode[pcs],ComplexPointprocessTable[cpc].Pointpositions[pcs],1,PointDCCcodetable[ComplexPointprocessTable[cpc].Pointcode[pcs]][ComplexPointprocessTable[cpc].Pointpositions[pcs]].DCCcode);
								Actionswitch(ComplexPointprocessTable[cpc].Pointcode[pcs],5,ComplexPointprocessTable[cpc].Pointpositions[pcs]);
								//	std::cout<<"\n\n\n2147 Switching point "<<ComplexPointprocessTable[cpc].Pointcode[pcs]<<" to status "<<ComplexPointprocessTable[cpc].Pointpositions[pcs];
						//	std::cout<<"\n\n";
							}
						}
					}
					ComplexPointprocessTable[cpc].ForwardprevLSTcode = ComplexPointprocessTable[cpc].ForwardLSTcode;
				//	std::cout<<"\n\nAfter update - Previous ForwardLSTcode "<<ComplexPointprocessTable[cpc].ForwardprevLSTcode;
				}

				if ((ComplexPointprocessTable[cpc].BackLSTcode != ComplexPointprocessTable[cpc].BackprevLSTcode) && (ComplexPointprocessTable[cpc].BackLSTcode != 999)) // check to see if forward LS code has changed
				{
					if (ComplexPointprocessTable[cpc].BackLSTcode > 0)
					{
						for (int pcs = 1; pcs < POINTSINCLUSTER; pcs++)
						{
							if (ComplexPointprocessTable[cpc].Pointcode[pcs] > 0)  // check to see which points need to be switched
							{
								CSwitchmsgprocess(ComplexPointprocessTable[cpc].Pointcode[pcs],ComplexPointprocessTable[cpc].Pointpositions[pcs],2,PointDCCcodetable[ComplexPointprocessTable[cpc].Pointcode[pcs]][ComplexPointprocessTable[cpc].Pointpositions[pcs]].DCCcode);
										//	std::cout<<"\n\n\n2147 Switching point "<<ComplexPointprocessTable[cpc].Pointcode[pcs]<<" to status "<<ComplexPointprocessTable[cpc].Pointpositions[pcs];
										//	std::cout<<"\n\n";
							}
						}
					}
					ComplexPointprocessTable[cpc].BackprevLSTcode = ComplexPointprocessTable[cpc].BackLSTcode;
								//	std::cout<<"\n\nAfter update - Previous ForwardLSTcode "<<ComplexPointprocessTable[cpc].ForwardprevLSTcode;
				}
		}
	}

}

void LNwebLsectionsLdevices::Loadcrossoverconfig(char* lineinputbufptr, int displayflag)
{
	if (displayflag > 1)
	{
			std::cout<<"\n\n2171 Crossover configuration rec->"<<lineinputbufptr;
	}

	int  Intvalue;

	int* Intvalueptr = &Intvalue;

	int LSsrce, LStar1, LStar2, LStar3, LStar4, LStar5, LStar6, LStar7, LStar8, LStar9, LStar10;

	Bufferprocessing* BPCO = new Bufferprocessing();

	BPCO->Fileparse(lineinputbufptr, ';',0);

	if (BPCO->ParamdataInteger(3,Intvalueptr) == 0)
	{
		Crossoverrecordloaded = Intvalue;
	}

	if (BPCO->ParamdataInteger(4,Intvalueptr) == 0)
	{
			LSsrce = Intvalue;
	}

	if (BPCO->ParamdataInteger(5,Intvalueptr) == 0)
	{
			LStar1 = Intvalue;
	}

	if (BPCO->ParamdataInteger(6,Intvalueptr) == 0)
	{
			LStar2 = Intvalue;
	}

	if (BPCO->ParamdataInteger(7,Intvalueptr) == 0)
	{
			LStar3 = Intvalue;
	}

	if (BPCO->ParamdataInteger(8,Intvalueptr) == 0)
	{
			LStar4 = Intvalue;
	}

	if (BPCO->ParamdataInteger(9,Intvalueptr) == 0)
	{
			LStar5 = Intvalue;
	}

	if (BPCO->ParamdataInteger(10,Intvalueptr) == 0)
	{
			LStar6 = Intvalue;
	}

	if (BPCO->ParamdataInteger(11,Intvalueptr) == 0)
	{
			LStar7 = Intvalue;
	}

	if (BPCO->ParamdataInteger(12,Intvalueptr) == 0)
	{
			LStar8 = Intvalue;
	}

	if (BPCO->ParamdataInteger(13,Intvalueptr) == 0)
	{
			LStar9 = Intvalue;
	}

	if (BPCO->ParamdataInteger(14,Intvalueptr) == 0)
	{
			LStar10 = Intvalue;
	}

	Savecrossoverconfig(Crossoverrecordloaded, LSsrce,LStar1,LStar2,LStar3,LStar4,LStar5,LStar6);


	//std::cout<<"\n\n2195 Crossover config LSsrce:"<<LSsrce<<" LStar1:"<<LStar1<<" LStar10:"<<LStar10;
}

void LNwebLsectionsLdevices::Loadcomplexpointlogicalconfig(char* lineinputbufptr, int displayflag)
{
	if (displayflag > 1)
	{
		std::cout<<"\n\n2094 Complex pt rec->"<<lineinputbufptr;
	}

	int  Intvalue;

	int* Intvalueptr = &Intvalue;

	int Cpoint;
	int CFLS;
	int CBLS;
	int Numberofpoints;
	int Cpt1;
	int Cpt1oc;
	int Cpt2;
	int Cpt2oc;
	int Cpt3;
	int Cpt3oc;
	int Cpt4;
	int Cpt4oc;
	int Cpt5;
	int Cpt5oc;
	int Cpt6;
	int Cpt6oc;
	int Cpt7;
	int Cpt7oc;
	int Cpt8;
	int Cpt8oc;

	Bufferprocessing* BPLC = new Bufferprocessing();

	BPLC->Fileparse(lineinputbufptr, ';',0);

	if (BPLC->ParamdataInteger(2,Intvalueptr) == 0)
	{
			Cpoint = Intvalue;
	}

	if (BPLC->ParamdataInteger(3,Intvalueptr) == 0)
	{
			CFLS = Intvalue;
	}

	if (BPLC->ParamdataInteger(4,Intvalueptr) == 0)
	{
			CBLS = Intvalue;
	}

	if (BPLC->ParamdataInteger(5,Intvalueptr) == 0)
	{
			Numberofpoints = Intvalue;
	}

	if (BPLC->ParamdataInteger(6,Intvalueptr) == 0)
	{
			Cpt1 = Intvalue;
	}

	if (BPLC->ParamdataInteger(7,Intvalueptr) == 0)
	{
			Cpt1oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(8,Intvalueptr) == 0)
	{
				Cpt2 = Intvalue;
	}

	if (BPLC->ParamdataInteger(9,Intvalueptr) == 0)
	{
				Cpt2oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(10,Intvalueptr) == 0)
	{
				Cpt3 = Intvalue;
	}

	if (BPLC->ParamdataInteger(11,Intvalueptr) == 0)
	{
				Cpt3oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(12,Intvalueptr) == 0)
	{
				Cpt4 = Intvalue;
	}

	if (BPLC->ParamdataInteger(13,Intvalueptr) == 0)
	{
				Cpt4oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(14,Intvalueptr) == 0)
	{
				Cpt5 = Intvalue;
	}

	if (BPLC->ParamdataInteger(15,Intvalueptr) == 0)
	{
				Cpt5oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(16,Intvalueptr) == 0)
	{
				Cpt6 = Intvalue;
	}

	if (BPLC->ParamdataInteger(17,Intvalueptr) == 0)
	{
				Cpt6oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(18,Intvalueptr) == 0)
	{
				Cpt7 = Intvalue;
	}

	if (BPLC->ParamdataInteger(19,Intvalueptr) == 0)
	{
				Cpt7oc = Intvalue;
	}

	if (BPLC->ParamdataInteger(20,Intvalueptr) == 0)
	{
				Cpt8 = Intvalue;
	}

	if (BPLC->ParamdataInteger(21,Intvalueptr) == 0)
	{
				Cpt8oc = Intvalue;
	}

	//std::cout<<"\nComplex pt rec result\n"<<Cpoint<<" "<<CFLS<<" "<<CBLS<<" "<<Numberofpoints<<" "<<Cpt1<<" "<<Cpt1oc;

	/*
	struct ComplexPointprocessing
			{
				int LogicalPointnumber;
				int ForwardLS;
				int ForwardLSTcode;
				int BackLS;
				int BackLSTcode;
				int Numberofphysicalpoints;
				int Pointcode[POINTSINCLUSTER];
				int Pointpositions[POINTSINCLUSTER];
				int CurrOpenClose[POINTSINCLUSTER];
				int PrevOpenClose[POINTSINCLUSTER];
			};

		struct ComplexPointprocessing ComplexPointprocessTable[NUMBEROFCOMPLEXPOINTS+1];
	*/

	if (Cpoint > 0)
	{
		// valid point so store record
		ComplexPointprocessTable[Cpoint].LogicalPointnumber = Cpoint;
		ComplexPointprocessTable[Cpoint].ForwardLS 			= CFLS;
		ComplexPointprocessTable[Cpoint].ForwardLSTcode		= 0;
		ComplexPointprocessTable[Cpoint].ForwardprevLSTcode	= 0;
		ComplexPointprocessTable[Cpoint].BackLS				= CBLS;
		ComplexPointprocessTable[Cpoint].BackLSTcode		= 0;
		ComplexPointprocessTable[Cpoint].BackprevLSTcode	= 0;
		ComplexPointprocessTable[Cpoint].Numberofphysicalpoints	= Numberofpoints;
		ComplexPointprocessTable[Cpoint].Pointcode[1]		= Cpt1;
		ComplexPointprocessTable[Cpoint].Pointpositions[1]	= Cpt1oc;
		ComplexPointprocessTable[Cpoint].Pointcode[2]		= Cpt2;
		ComplexPointprocessTable[Cpoint].Pointpositions[2]	= Cpt2oc;
		ComplexPointprocessTable[Cpoint].Pointcode[3]		= Cpt3;
		ComplexPointprocessTable[Cpoint].Pointpositions[3]	= Cpt3oc;
		ComplexPointprocessTable[Cpoint].Pointcode[4]		= Cpt4;
		ComplexPointprocessTable[Cpoint].Pointpositions[4]	= Cpt4oc;
		ComplexPointprocessTable[Cpoint].Pointcode[5]		= Cpt5;
		ComplexPointprocessTable[Cpoint].Pointpositions[5]	= Cpt5oc;
		ComplexPointprocessTable[Cpoint].Pointcode[6]		= Cpt6;
		ComplexPointprocessTable[Cpoint].Pointpositions[6]	= Cpt6oc;
		ComplexPointprocessTable[Cpoint].Pointcode[7]		= Cpt7;
		ComplexPointprocessTable[Cpoint].Pointpositions[7]	= Cpt7oc;
		ComplexPointprocessTable[Cpoint].Pointcode[8]		= Cpt8;
		ComplexPointprocessTable[Cpoint].Pointpositions[8]	= Cpt8oc;
	}

	delete BPLC;
}



void LNwebLsectionsLdevices::LoadpointDCCconfig(char* lineinputbufferptr, int displayflag)
{
	int Pointnumber;

	unsigned char LNHexcode[4];

	char* Codeptr;

	//int pointnumber;

	char* CodeDCCtextptr;

	int  Intvalue;

	int  Rectype;

	int* Intvalueptr = &Intvalue;

	unsigned char srcechar12[2];

	srcechar12[0] = ' ';
	srcechar12[1] = ' ';

	unsigned char* srcechar;

	Bufferprocessing* BPLL = new Bufferprocessing();

	BPLL->Fileparse(lineinputbufferptr, ';',0);

	// test for point or uncoupler record

	if (BPLL->ParamdataInteger(2,Intvalueptr) == 0)
	{
		Rectype = Intvalue;
	}

	switch(Rectype)
	{
	  case 1:
	  {

		  if (BPLL->ParamdataInteger(4,Intvalueptr) == 0)
		  {
			  Pointnumber = Intvalue;
		  }

		  Codeptr 	= BPLL->Paramdata(5);

		  CodeDCCtextptr = BPLL->Paramdata(6);

		  srcechar12[0] = (unsigned char)*(CodeDCCtextptr+0);
		  srcechar12[1] = (unsigned char)*(CodeDCCtextptr+1);

		  srcechar = &srcechar12[0];

		  LNHexcode[0] = StringtoH(srcechar);

		  srcechar12[0] = (unsigned char)*(CodeDCCtextptr+3);
		  srcechar12[1] = (unsigned char)*(CodeDCCtextptr+4);

		  srcechar = &srcechar12[0];

		  LNHexcode[1] = StringtoH(srcechar);

		  srcechar12[0] = (unsigned char)*(CodeDCCtextptr+6);
		  srcechar12[1] = (unsigned char)*(CodeDCCtextptr+7);

		  srcechar = &srcechar12[0];

		  LNHexcode[2] = StringtoH(srcechar);

		  srcechar12[0] = (unsigned char)*(CodeDCCtextptr+9);
		  srcechar12[1] = (unsigned char)*(CodeDCCtextptr+10);

		  srcechar = &srcechar12[0];

		  LNHexcode[3] = StringtoH(srcechar);

		 //printf("\n\n 3186 LNwebLsectionsLdevices PT %i %s Hex chars %02X %02X %02X %02X", Pointnumber,Codeptr, LNHexcode[0],LNHexcode[1],LNHexcode[2],LNHexcode[3]);

		  // 	C = closed, T = Open

		  if (*Codeptr == 'C')
		  {
			 // printf("\n 2585 PT %02d Closed switch record",Pointnumber);
			  StoreDCCpointcodes(Pointnumber,0,LNHexcode);
			  CPointDCCcodeupdate(Pointnumber,0,LNHexcode);  // store in the C code as well
		  }

		  if (*Codeptr == 'T')
		  {
		     // printf("\n 2592 PT %02d Open switch record",Pointnumber);
			  StoreDCCpointcodes(Pointnumber,1,LNHexcode);
			  CPointDCCcodeupdate(Pointnumber,1,LNHexcode);  // store in the C code as well
		  }
	  }
	  break;

	  case 2:
	  {

		//std::cout<<"\n1989 Uncoupler DCC record found";
	  }
	  break;
	}

	delete BPLL;

}

void LNwebLsectionsLdevices::StoreDCCpointcodes(int pnt,int Dir, unsigned char Hxcode[4])
{
	PointDCCcodetable[pnt][Dir].Pointnumber = pnt;
	PointDCCcodetable[pnt][Dir].Openclose = 0;
	PointDCCcodetable[pnt][Dir].DCCcode[0] = Hxcode[0];
	PointDCCcodetable[pnt][Dir].DCCcode[1] = Hxcode[1];
	PointDCCcodetable[pnt][Dir].DCCcode[2] = Hxcode[2];
	PointDCCcodetable[pnt][Dir].DCCcode[3] = Hxcode[3];

	//printf("\n2065 LNwebLsectionsLdevices Point %i OC %i switching message stored  %02X %02X %02X %02X\n",pnt, Dir, PointDCCcodetable[pnt][Dir].DCCcode[0],\
			PointDCCcodetable[pnt][Dir].DCCcode[1],PointDCCcodetable[pnt][Dir].DCCcode[2],PointDCCcodetable[pnt][Dir].DCCcode[3]);
}

void LNwebLsectionsLdevices::InitPointDCCcodes()
{
	int pdcc;
	//int pdccoc;

	for (pdcc = 0;pdcc <= NUMBEROFPOINTS; pdcc++)
	{
		PointDCCcodetable[pdcc][0].Pointnumber = pdcc;
		PointDCCcodetable[pdcc][0].Openclose = 0;
		PointDCCcodetable[pdcc][0].DCCcode[0] = 0x00;
		PointDCCcodetable[pdcc][0].DCCcode[1] = 0x00;
		PointDCCcodetable[pdcc][0].DCCcode[2] = 0x00;
		PointDCCcodetable[pdcc][0].DCCcode[3] = 0x00;

		PointDCCcodetable[pdcc][1].Pointnumber = pdcc;
		PointDCCcodetable[pdcc][1].Openclose = 1;
		PointDCCcodetable[pdcc][1].DCCcode[0] = 0x00;
		PointDCCcodetable[pdcc][1].DCCcode[1] = 0x00;
		PointDCCcodetable[pdcc][1].DCCcode[2] = 0x00;
		PointDCCcodetable[pdcc][1].DCCcode[3] = 0x00;
	}
}

void LNwebLsectionsLdevices::InitcomplexPointconfig()
{
	// initializes the tables for the complex point networks
	/*
	struct ComplexPointprocessing
		{
			int LogicalPointnumber;
			int ForwardLS;
			int ForwardLSTcode;
			int BackLS;
			int BackLSTcode;
			int Numberofphysicalpoints;
			int Pointcode[PointsinCluster];
			int Pointpositions[PointsinCluster];
			int CurrOpenClose;
			int PrevOpenClose;
		};

	struct ComplexPointprocessing ComplexPointprocessTable[NUMBEROFCOMPLEXPOINTS+1];
	*/

	for (int cps = 0; cps < NUMBEROFCOMPLEXPOINTS; cps++)
	{
		ComplexPointprocessTable[cps].LogicalPointnumber 		= 0;
		ComplexPointprocessTable[cps].ForwardLS					= 0;
		ComplexPointprocessTable[cps].ForwardLSTcode			= 0;
		ComplexPointprocessTable[cps].BackLS					= 0;
		ComplexPointprocessTable[cps].BackLSTcode				= 0;
		ComplexPointprocessTable[cps].Numberofphysicalpoints	= 0;

		for (int pic = 0; pic < POINTSINCLUSTER; pic++)
		{
			ComplexPointprocessTable[cps].Pointcode[pic] 		= 0;
			ComplexPointprocessTable[cps].Pointpositions[pic]	= 0;
			ComplexPointprocessTable[cps].CurrOpenClose[pic]	= 0;
			ComplexPointprocessTable[cps].PrevOpenClose[pic]	= 0;
		}
	}
}

void LNwebLsectionsLdevices::DisplaycomplexPointconfig()
{
	std::cout<<"\nComplex points logical configuration "<<"\n-----------------------------------\n";

	std::cout<<"\n Point Forward LS Curr-Tcode Prev-Tcode Back LS Curr-Tcode Prev-Tcode  Pt1 O/C  Pt2 O/C  Pt3 O/C  Pt4 O/C  Pt5 O/C  Pt6 O/C  Pt7 O/C Pt8 O/C ";

	for (int dps = 0; dps < NUMBEROFCOMPLEXPOINTS; dps++)
	{
		if (ComplexPointprocessTable[dps].LogicalPointnumber > 0)
		{
		printf("\n  %03d     %03d       %04d      %04d       %03d       %04d      %04d      %03d  %01d   %03d  %01d   %03d  %01d   %03d  %01d   %03d  %01d   %03d  %01d   %03d  %01d  %03d  %01d",\
				ComplexPointprocessTable[dps].LogicalPointnumber, ComplexPointprocessTable[dps].ForwardLS, \
				ComplexPointprocessTable[dps].ForwardLSTcode,  ComplexPointprocessTable[dps].ForwardprevLSTcode, ComplexPointprocessTable[dps].BackLS,\
				ComplexPointprocessTable[dps].BackLSTcode, ComplexPointprocessTable[dps].BackprevLSTcode,  \
				ComplexPointprocessTable[dps].Pointcode[1],ComplexPointprocessTable[dps].Pointpositions[1],\
				ComplexPointprocessTable[dps].Pointcode[2],ComplexPointprocessTable[dps].Pointpositions[2],\
				ComplexPointprocessTable[dps].Pointcode[3],ComplexPointprocessTable[dps].Pointpositions[3],\
				ComplexPointprocessTable[dps].Pointcode[4],ComplexPointprocessTable[dps].Pointpositions[4],\
				ComplexPointprocessTable[dps].Pointcode[5],ComplexPointprocessTable[dps].Pointpositions[5],\
				ComplexPointprocessTable[dps].Pointcode[6],ComplexPointprocessTable[dps].Pointpositions[6],\
				ComplexPointprocessTable[dps].Pointcode[7],ComplexPointprocessTable[dps].Pointpositions[7],\
				ComplexPointprocessTable[dps].Pointcode[8],ComplexPointprocessTable[dps].Pointpositions[8]);
		}
	}

	std::cout<<"\n\n";

	std::cout<<"\n Point Status Open/Close ";

	for (int dps = 0; dps < NUMBEROFCOMPLEXPOINTS; dps++)
		{
		//	if (ComplexPointprocessTable[dps].LogicalPointnumber > 0)
		//	{
				printf("\n %03d     %01d", PointStatusTable[dps].PPointnumber,PointStatusTable[dps].CurrOpenClose);

		//	}
		}


}

void LNwebLsectionsLdevices::InitPointstatus()
{
	// initializes the point and crossover tables before the configuration is loaded.
	/*

		int PPointnumber;
		int ForwardCloseLS;
		int ForwardCloseLSTcode;
		int BackCloseLS;
		int BackCloseLSTcode;
		int ForwardOpenLS;
		int ForwardOpenLSTcode;
		int BackOpenLS;
		int BackOpenLSTcode;
		int Pointcode;
		int CurrOpenClose;
		int PrevOpenClose;

		struct Pointprocessing PointprocessTable[NUMBEROFPOINTS+1];

		int Numberofpointsloaded;


	*/
	for (int Ptcnt = 0; Ptcnt <= NUMBEROFPOINTS; Ptcnt++)
	{
		PointStatusTable[Ptcnt].PPointnumber 			= Ptcnt;
		PointStatusTable[Ptcnt].CurrOpenClose			= 0;
	}

	Numberofpointsloaded 	= 0;

	/*

	struct Crossoverprocessing
	{
		int source;
		int sourcetcode;
		int sourceprevtcode;
		int target1;
		int target1tcode;
		int target2;
		int target2tcode;
		int target3;
		int target3tcode;
		int target4;
		int target4tcode;
		int target5;
		int target5tcode;
		int target6;
		int target6tcode;
	};

	struct Crossoverprocessing Crossovertable[NUMBEROFCROSSOVERSECTIONS+1];
	*/

	for (int Crossovercnt = 0; Crossovercnt<=NUMBEROFCROSSOVERSECTIONS;Crossovercnt++)
	{
		Crossovertable[Crossovercnt].source				= 0;
		Crossovertable[Crossovercnt].sourcetcode		= 0;
		Crossovertable[Crossovercnt].sourceprevtcode	= 0;
		Crossovertable[Crossovercnt].target1			= 0;
		Crossovertable[Crossovercnt].target1tcode		= 0;
		Crossovertable[Crossovercnt].target2			= 0;
		Crossovertable[Crossovercnt].target2tcode		= 0;
		Crossovertable[Crossovercnt].target3			= 0;
		Crossovertable[Crossovercnt].target3tcode		= 0;
		Crossovertable[Crossovercnt].target4			= 0;
		Crossovertable[Crossovercnt].target4tcode		= 0;
		Crossovertable[Crossovercnt].target5			= 0;
		Crossovertable[Crossovercnt].target5tcode		= 0;
		Crossovertable[Crossovercnt].target6			= 0;
		Crossovertable[Crossovercnt].target6tcode		= 0;
	}

	Crossoverrecordloaded 	= 0;

}

void LNwebLsectionsLdevices::RefreshLastupdatedetails(int rectype)
{
	char CTDatetime[DATETIMESTRING] = "Not set ";

	char* CTDatetimeptr = &CTDatetime[0];

	Clocktime CT;

	CT.Get_time(*CTDatetimeptr);

	//std::cout<<"\nUpdating the lastupdated Time stamp->"<<CTDatetime<<"\n";

	//int h1,h2,m1,m2,s1,s2,ht,mt,st,tt;

	if (rectype == PS_TIMESTAMP)
	{
		LastPSupdateseqnum++;
		LastPSupdateHH[0] = *(CTDatetimeptr + 11);
		LastPSupdateHH[1] = *(CTDatetimeptr + 12);
		LastPSupdateMN[0] = *(CTDatetimeptr + 14);
		LastPSupdateMN[1] = *(CTDatetimeptr + 15);
		LastPSupdateSS[0] = *(CTDatetimeptr + 17);
		LastPSupdateSS[1] = *(CTDatetimeptr + 18);
	}
	else
	{
		if (rectype == PD_TIMESTAMP)
			{
			LastPDupdateseqnum++;
			LastPDupdateHH[0] = *(CTDatetimeptr + 11);
			LastPDupdateHH[1] = *(CTDatetimeptr + 12);
			LastPDupdateMN[0] = *(CTDatetimeptr + 14);
			LastPDupdateMN[1] = *(CTDatetimeptr + 15);
			LastPDupdateSS[0] = *(CTDatetimeptr + 17);
			LastPDupdateSS[1] = *(CTDatetimeptr + 18);
			}
	}
}

int LNwebLsectionsLdevices::PScompare(int seqnum, int timestamp)
{
		if ((LastPSupdateseqnum != seqnum) || (LastPStimestampval != LastPStimestampval))
		{
			return 1;
		}
		else
		{
			return 0;
		}
}

int LNwebLsectionsLdevices::GetPStimestampdata(char* message)
{
	/*
	 int LastPSupdateseqnum;
	 char LastPSupdateHH[2];
	 char LastPSupdateMN[2];
	 char LastPSupdateSS[2];
	 int LastPStimestampval;
	 */

		sprintf(message,"Last Physection seq %04d at %C%C:%C%C:%C%C ",LastPSupdateseqnum,LastPSupdateHH[0],LastPSupdateHH[1],\
				LastPSupdateMN[0],LastPSupdateMN[1],LastPSupdateSS[0],LastPSupdateSS[1]);
		return LastPSupdateseqnum;
}

int LNwebLsectionsLdevices::GetPDtimestampdata(char* message)
{
		sprintf(message,"Last PDetector  seq %04d at %C%C:%C%C:%C%C ",LastPDupdateseqnum,LastPDupdateHH[0],LastPDupdateHH[1],\
					LastPDupdateMN[0],LastPDupdateMN[1],LastPDupdateSS[0],LastPDupdateSS[1]);
			return LastPDupdateseqnum;

}



int LNwebLsectionsLdevices::PDcompare(int seqnum, int timestamp)
{
		if ((LastPDupdateseqnum != seqnum) || (LastPDtimestampval != LastPStimestampval))
		{
			return 1;
		}
		else
		{
			return 0;
		}
}

void LNwebLsectionsLdevices::InitLastupdatedetails()
{
		LastPSupdateseqnum = 0;
		LastPSupdateHH[0] = '0';
		LastPSupdateHH[1] = '0';
		LastPSupdateMN[0] = '0';
		LastPSupdateMN[1] = '0';
		LastPSupdateSS[0] = '0';
		LastPSupdateSS[1] = '0';

		LastPDupdateseqnum = 0;
		LastPDupdateHH[0] = '0';
		LastPDupdateHH[1] = '0';
		LastPDupdateMN[0] = '0';
		LastPDupdateMN[1] = '0';
		LastPDupdateSS[0] = '0';
		LastPDupdateSS[1] = '0';
}

void LNwebLsectionsLdevices::Actionswitch(int ptc, int calledby, int OC)
{
	std::cout<<"\n\n3479 Actionswitch called by:"<<calledby<<" ptc:"<<ptc<<" OC:"<<OC;

	printf("\n3525 Actionswitch Pt %i OC %i code %02X %02X %02X %02X",ptc,OC,PointDCCcodetable[ptc][OC].DCCcode[0],PointDCCcodetable[ptc][OC].DCCcode[1],\
			PointDCCcodetable[ptc][OC].DCCcode[2],PointDCCcodetable[ptc][OC].DCCcode[3]);

	PointUpdatecurrentposition(ptc, OC);

	CSwitchmsgprocess(ptc,OC,calledby,PointDCCcodetable[ptc][OC].DCCcode);

}

void LNwebLsectionsLdevices::Closeallswitches()
{
	int LPcode;

	/*
		int PPointnumber;
		int ForwardCloseLS;
		int ForwardCloseLSTcode;
		int BackCloseLS;
		int BackCloseLSTcode;
		int ForwardOpenLS;
		int ForwardOpenLSTcode;
		int BackOpenLS;
		int BackOpenLSTcode;
		int Pointcode;
		int CurrOpenClose;
		int PrevOpenClose;

		PointprocessTable[NUMBEROFPOINTS+1]
	*/

	for (LPcode = 1; LPcode <= NUMBEROFPOINTS; LPcode++)
	{

	}

}

void LNwebLsectionsLdevices::PointUpdatecurrentposition(int pt, int OC)
{
	PointStatusTable[pt].CurrOpenClose = OC;		// Update the point poistion record
}

void LNwebLsectionsLdevices::GetPointDCCcodes(int pt,int OC, char* replymessage)
{
	char outputrec[256];
	
	char outputDCCcode[4];

	int isc,ic,scc,mcc; 
	
	/***************** Point DCC codes  **********************************

	struct PointDCCcodes
	{
		unsigned int Pointnumber;
		unsigned int Openclose;
		unsigned char DCCcode[4];
	};

	struct PointDCCcodes PointDCCcodetable[NUMBEROFPOINTS+1][PTOPENCLOSEFLAG]; */

	
	scc = 3;
	
	printf("\n870 GetointDCC codes pt %i OC %i ",pt ,OC);
	
	printf("\n:Pt:%03d: OC1:%i: DCC:%02X %02X %02X %02X: : OC2:%i: DCC:%02X %02X %02X %02X: ",scc,PointDCCcodetable[scc][0].Openclose,\
		PointDCCcodetable[scc][0].DCCcode[0],PointDCCcodetable[scc][0].DCCcode[1],PointDCCcodetable[scc][0].DCCcode[2],PointDCCcodetable[scc][0].DCCcode[3],\
		PointDCCcodetable[scc][1].Openclose,\
		PointDCCcodetable[scc][1].DCCcode[0],PointDCCcodetable[scc][1].DCCcode[1],PointDCCcodetable[scc][1].DCCcode[2],PointDCCcodetable[scc][1].DCCcode[3]);
		
	//return;

	mcc = 0;

	if (pt == 0)             // display the details
	{
		for (scc = 0;scc <= NUMBEROFPOINTS ;scc++)
		{
		bzero(outputrec,256);

		sprintf(outputrec,":Pt:%03d: OC1:%i: DCC:%02X %02X %02X %02X: OC2:%i: DCC:%02X %02X %02X %02X: ",scc,PointDCCcodetable[scc][0].Openclose,\
		PointDCCcodetable[scc][0].DCCcode[0],PointDCCcodetable[scc][0].DCCcode[1],PointDCCcodetable[scc][0].DCCcode[2],PointDCCcodetable[scc][0].DCCcode[3],\
		PointDCCcodetable[scc][1].Openclose,\
		PointDCCcodetable[scc][1].DCCcode[0],PointDCCcodetable[scc][1].DCCcode[1],PointDCCcodetable[scc][1].DCCcode[2],PointDCCcodetable[scc][1].DCCcode[3]);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
			{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
			}
		*(replymessage + mcc) = '\n';
		mcc++;
		}

		*(replymessage + mcc) = '\n';

		return;
	}

	if ((pt > 0) && (OC < 2))   // retrieve DCC code for point as 4 byte hex string
	{
		mcc = 0;
		bzero(outputDCCcode,4);		
		sprintf(outputDCCcode,"%02X%02X%02X%02X",PointDCCcodetable[pt][OC].DCCcode[0],PointDCCcodetable[pt][OC].DCCcode[1],PointDCCcodetable[pt][OC].DCCcode[2],PointDCCcodetable[pt][OC].DCCcode[3]);
		
		for (ic = 0;ic < 4;ic++)
			{
			*(replymessage + mcc) = PointDCCcodetable[pt][OC].DCCcode[mcc];
			mcc++;
			}	

		return;
	}

}

int LNwebLsectionsLdevices::Getpointcurrentstatus(int pointnum)
{
	return PointStatusTable[pointnum].CurrOpenClose;
}

/*
void LNwebLsectionsLdevices::Savepointconfig(int LPcode, int Direction, int FCLS, int BCLS, int FOLS, int BOLS, int PPcode)
{
	/*
	 struct Pointprocessing

	int PPointnumber;
	int ForwardCloseLS;
	int ForwardCloseLSTcode;
	int BackCloseLS;
	int BackCloseLSTcode;
	int ForwardOpenLS;
	int ForwardOpenLSTcode;
	int BackOpenLS;
	int BackOpenLSTcode;
	int Pointcode;
	int CurrOpenClose;
	int PrevOpenClose;

	struct Pointprocessing PointprocessTable[NUMBEROFPOINTS];



	 //printf("\nPoint %i Dir %i FC %i BC %i FO %i BO %i PPcode %i", LPcode,Direction,FCLS,BCLS,FOLS,BOLS,PPcode);



	 PointprocessTable[LPcode].PPointnumber = LPcode;
	 PointprocessTable[LPcode].ForwardCloseLS = FCLS;
	 PointprocessTable[LPcode].ForwardCloseLSTcode = 0;
	 PointprocessTable[LPcode].BackCloseLS = BCLS;
	 PointprocessTable[LPcode].BackOpenLSTcode = 0;
	 PointprocessTable[LPcode].ForwardOpenLS = FOLS;
	 PointprocessTable[LPcode].ForwardCloseLSTcode = 0;
	 PointprocessTable[LPcode].BackOpenLS = BOLS;
	 PointprocessTable[LPcode].BackOpenLSTcode = 0;
	 PointprocessTable[LPcode].Pointcode = PPcode;
	 PointprocessTable[LPcode].CurrOpenClose = 0;
	 PointprocessTable[LPcode].PrevOpenClose = 2;   // 0 = closed, 1 = open

	 Numberofpointsloaded++;
}
*/

void LNwebLsectionsLdevices::Savecrossoverconfig(int Crossoverrecordss, int LSsrce,int LStar1,int LStar2,int LStar3,int LStar4,int LStar5,int LStar6)
{
	Crossovertable[Crossoverrecordss].source = LSsrce;
	Crossovertable[Crossoverrecordss].target1 = LStar1;
	Crossovertable[Crossoverrecordss].target2 = LStar2;
	Crossovertable[Crossoverrecordss].target3 = LStar3;
	Crossovertable[Crossoverrecordss].target4 = LStar4;
	Crossovertable[Crossoverrecordss].target5 = LStar5;
	Crossovertable[Crossoverrecordss].target6 = LStar6;

	Crossovertable[Crossoverrecordss].sourcetcode = 0;
	Crossovertable[Crossoverrecordss].sourceprevtcode = 0;
	Crossovertable[Crossoverrecordss].target1tcode = 0;
	Crossovertable[Crossoverrecordss].target2tcode = 0;
	Crossovertable[Crossoverrecordss].target3tcode = 0;
	Crossovertable[Crossoverrecordss].target4tcode = 0;
	Crossovertable[Crossoverrecordss].target5tcode = 0;
	Crossovertable[Crossoverrecordss].target6tcode = 0;

}

void LNwebLsectionsLdevices::Displaycrossoverconfig()
{
	printf("\nDisplay Crossover table\n");
	int Crossoverrecordss;

	for (Crossoverrecordss = 1;Crossoverrecordss<=Crossoverrecordloaded;Crossoverrecordss++)
	{
	printf("\n Cnt %i LS %i T1 %i T2 %i T3 %i T4 %i T5 %i T6 %i ",\
	Crossoverrecordss,\
	Crossovertable[Crossoverrecordss].source,\
	Crossovertable[Crossoverrecordss].target1,\
	Crossovertable[Crossoverrecordss].target2,\
	Crossovertable[Crossoverrecordss].target3,\
	Crossovertable[Crossoverrecordss].target4,\
	Crossovertable[Crossoverrecordss].target5,\
	Crossovertable[Crossoverrecordss].target6);
	}
}

void LNwebLsectionsLdevices::InitDetectortriggertable()
 {

	 int Idt;

	 for (Idt = 0;Idt <= LOGICALDETECTORS;Idt++)
	 {
		 DetectorTriggerTable[Idt].Triggerdevicenumber 	= 0;
		 DetectorTriggerTable[Idt].Triggerenabled 		= 0;
		 DetectorTriggerTable[Idt].Triggerstatus 		= 0;
		 DetectorTriggerTable[Idt].TriggerPTorUR 		= 0;
		 DetectorTriggerTable[Idt].TriggeredOutputdevicenumber 	= 0;

		 DetectorTriggerTable[Idt].Countertrigenabled 	= 0;
		 DetectorTriggerTable[Idt].ActualDevicecounter 	= 0;
		 DetectorTriggerTable[Idt].TargetDevicecounter 	= 0;
		 DetectorTriggerTable[Idt].LastswitchOnOff 		= 0;

		 DetectorTriggerTable[Idt].Trainreference 		= 0;
		 DetectorTriggerTable[Idt].Locoreference		= 0;
		 DetectorTriggerTable[Idt].ActionLSreference 	= 0;
		 DetectorTriggerTable[Idt].ActionNSLSreference 	= 0;

	 }
	// printf("\nDevice Table cleared ");
 }

 void LNwebLsectionsLdevices::SaveDetectortriggerconfig(int Triggerdevice, int PToUR, int PtorURnumber)
 {
	 DetectorTriggerTable[Triggerdevice].Triggerdevicenumber = PtorURnumber;
	 DetectorTriggerTable[Triggerdevice].Triggerenabled = 0;
	 DetectorTriggerTable[Triggerdevice].Triggerstatus = 0;
	 DetectorTriggerTable[Triggerdevice].TriggerPTorUR = PToUR;
	 DetectorTriggerTable[Triggerdevice].TriggeredOutputdevicenumber = 0;

	 printf("\nDevice %i Trig config saved %i %i %i %i %i",Triggerdevice,DetectorTriggerTable[Triggerdevice].Triggerdevicenumber,\
	 DetectorTriggerTable[Triggerdevice].Triggerenabled,\
	 DetectorTriggerTable[Triggerdevice].Triggerstatus, \
	 DetectorTriggerTable[Triggerdevice].TriggerPTorUR, DetectorTriggerTable[Triggerdevice].TriggeredOutputdevicenumber);

 }

 /*
 	struct Detectortrigger
 	{
 		int Triggerdevicenumber;
 		int Triggerenabled; 			// 0 - disabled, 1 - enabled
 		int Triggerstatus;              // 0 - set , 1 - actioned
 		int TriggerPTorUR;              // 1 - uncoupler ramp. 2 - switch (point)
 		int TriggeredOutputdevicenumber;          // number of switch or uncoupler ramp
 		int ActualDevicecounter;		// Actual number of device triggers
 		int TargetDevicecounter;		// Target number of device triggers
 		int Countertrigenabled;         // Enable flag for counter based trigger
 		int LastswitchOnOff;         	// Log of last switch - toggle
 	};

 	struct Detectortrigger DetectorTriggerTable[LOGICALDETECTORS+1];


 	*/

 void LNwebLsectionsLdevices::StoreDetectortriggeractionconfig(int Triggerdevice, int Uncouplernumber, int Target, int Locoref, int Trainref, int LSreference, int NSSref)
  {
	 DetectorTriggerTable[Triggerdevice].Triggerdevicenumber = Triggerdevice;
	 DetectorTriggerTable[Triggerdevice].Triggerenabled = 1;
	 DetectorTriggerTable[Triggerdevice].Triggerstatus = 0;
	 DetectorTriggerTable[Triggerdevice].TriggerPTorUR = 1;
	 DetectorTriggerTable[Triggerdevice].TriggeredOutputdevicenumber = Uncouplernumber;
	 DetectorTriggerTable[Triggerdevice].TargetDevicecounter = Target;
	 DetectorTriggerTable[Triggerdevice].ActualDevicecounter = 0;
	 DetectorTriggerTable[Triggerdevice].LastswitchOnOff 	= 0;

	 DetectorTriggerTable[Triggerdevice].Locoreference 		= Locoref;
	 DetectorTriggerTable[Triggerdevice].Trainreference 	= Trainref;
	 DetectorTriggerTable[Triggerdevice].ActionLSreference	= LSreference;
	 DetectorTriggerTable[Triggerdevice].ActionNSLSreference = NSSref;
  }

 int LNwebLsectionsLdevices::GetDeviceLocoref(int Triggerdevice)
  {
	 return DetectorTriggerTable[Triggerdevice].Locoreference;
  }

 int LNwebLsectionsLdevices::GetDeviceTrainref(int Triggerdevice)
  {
 	 return DetectorTriggerTable[Triggerdevice].Trainreference;
  }

 void LNwebLsectionsLdevices::ResetCountertarget(int Idt)
 {
	 DetectorTriggerTable[Idt].TargetDevicecounter = 0;
	 DetectorTriggerTable[Idt].Countertrigenabled = 0;
	 DetectorTriggerTable[Idt].ActualDevicecounter = 0;
	 printf("\nReset counter target & actual for device %i ",Idt);
 }

 void LNwebLsectionsLdevices::Processdetectorcount(int ld, int occupied)
 {
 	//printf("\nDevice %i Occupied %i",ld,occupied);

 	if ((occupied > 0) && (DetectorTriggerTable[ld].LastswitchOnOff == 0))
 	{
 		DetectorTriggerTable[ld].LastswitchOnOff = 1;
 		DetectorTriggerTable[ld].ActualDevicecounter = DetectorTriggerTable[ld].ActualDevicecounter + 1;
 		//printf("\nDetector %i switched ON ",ld);
 		//printf("\n");
 		return;
 	}

 	if ((occupied == 0) && (DetectorTriggerTable[ld].LastswitchOnOff > 0))
 	{
 		DetectorTriggerTable[ld].LastswitchOnOff = 0;
 		//printf("\nDetector %i switched OFF ",ld);
 		//printf("\n");
 		return;
 	}
 }


void LNwebLsectionsLdevices::UpdateTrigger(int device,int signalmsg, int Enableflg, int Statusflg)
 {
	 int Idt;

	 //printf("\nUpdate msg %i %i %i %i ",device,signalmsg,Enableflg,Statusflg);

	 if (signalmsg == 3)  // reset all enables, status and triggers
	 {
		for (Idt = 0;Idt <= LOGICALDETECTORS;Idt++)
		{
		 DetectorTriggerTable[Idt].Triggerenabled = 0;
		 DetectorTriggerTable[Idt].Triggerstatus = 0;
		 DetectorTriggerTable[Idt].TriggeredOutputdevicenumber = 0;
		}
	 }

	 if (signalmsg == 1)  // Update details for one device
	 {
		 DetectorTriggerTable[device].Triggerenabled = Enableflg;
		 DetectorTriggerTable[device].Triggerstatus = Statusflg;
		 DetectorTriggerTable[device].TriggeredOutputdevicenumber = 0;
	 }

	 if (signalmsg == 2)  // Reset details for one device
	 {
		 DetectorTriggerTable[device].Triggerenabled = 0;
		 DetectorTriggerTable[device].Triggerstatus = 0;
		 DetectorTriggerTable[device].TriggeredOutputdevicenumber = 0;
	 }
 }


void LNwebLsectionsLdevices::SetCountertarget(int Idt, int targetvalue)
{
 	 DetectorTriggerTable[Idt].TargetDevicecounter = targetvalue;
 	 DetectorTriggerTable[Idt].Countertrigenabled = 0;
 }


void LNwebLsectionsLdevices::ResetCountertargetAll()
  {
 	 int Idt;

 	 for (Idt = 0;Idt <= LOGICALDETECTORS;Idt++)
 	 {
 		 DetectorTriggerTable[Idt].TargetDevicecounter = 0;
 		 DetectorTriggerTable[Idt].Countertrigenabled = 0;
 		 DetectorTriggerTable[Idt].ActualDevicecounter = 0;
 	 }

  }

void LNwebLsectionsLdevices::LogicalSectionTcodeRefresh()
 {
		/************* Updates the Train code after a physical section status change ***************/

		//DisplayDetectorLkUptable();

	    char PSmessage[SHORTMESSAGELENGTH ];

		int lsu = 0;

		int Occupstatus = 0;

		int Occupstatus2 = 0;

		int PSectiontype = 0;

		int Poffset = 0;

		int Poffset2 = 0;

		//std::cout<<"\nLogical section refresh....";

		for (lsu = 1; lsu < NUMBEROPERATIONALLOGSECT; lsu++)    // NUMBEROPERATIONALLOGSECT
		{
			PSectiontype = Logicalsectiondata[lsu].Logicalsectiontype;

			 Poffset = Logicalsectiondata[lsu].Physectionref;

			 Poffset2 = Logicalsectiondata[lsu].Physectionref2;

			 if ((PSectiontype == 1)|| (PSectiontype == 2))	// single physical section in one logical section
			  {
			  	if (Poffset > 0)
			  	{
			  		bzero(PSmessage,SHORTMESSAGELENGTH);

			  		Occupstatus = PSectiondatareturn(Poffset,&PSmessage[0]);

			    	//printf("\n%s",PSmessage);

			  		if ((Occupstatus > 0) & (Logicalsectiondata[lsu].LogicalsectionTcode == 0))
			  		{
			  			Logicalsectiondata[lsu].LogicalsectionTcode = 999;
			  		}

			  		if ((Occupstatus == 0) & (Logicalsectiondata[lsu].LogicalsectionTcode == 999))
			  		{
			  			Logicalsectiondata[lsu].LogicalsectionTcode = 0;
			  		}
			  	}
			  }
			 else
			 {
				 if ((PSectiontype == 3)|| (PSectiontype == 4))  // multiple physical sections in one logical section
				  			{
				  				if ((Poffset > 0) || (Poffset2 > 0))
				  				{
				  					bzero(PSmessage,SHORTMESSAGELENGTH);

				  					Occupstatus = PSectiondatareturn(Poffset,&PSmessage[0]);

				  					Occupstatus2 = PSectiondatareturn(Poffset2,&PSmessage[0]);

				  					if (((Occupstatus > 0) & (Logicalsectiondata[lsu].LogicalsectionTcode == 0)) || ((Occupstatus2 > 0) & (Logicalsectiondata[lsu].LogicalsectionTcode == 0)))
				  					{
				  					Logicalsectiondata[lsu].LogicalsectionTcode = 999;
				  					}

				  					if ((Occupstatus == 0) & (Logicalsectiondata[lsu].LogicalsectionTcode == 999) & (Occupstatus2 == 0))
				  					{
				  					Logicalsectiondata[lsu].LogicalsectionTcode = 0;
				  					}
				  				}
				  			}
				  			else
				  			{
				  				if (PSectiontype == 5)   // processing for logical detectors
				  				{
				  					if (Poffset > 0)
				  						{

				  							bzero(PSmessage,SHORTMESSAGELENGTH);

				  							Occupstatus = PSectiondatareturn(Poffset,&PSmessage[0]);

				  							Processdetectorcount(lsu,Occupstatus);			// Update device counter

				  							Logicaldetectornumerictriggersprocess();		// Check to see if it has activated a number target trigger

				  							if (Occupstatus > 0)
				  							{
				  								//printf("\n Updated LD  Poffset %i  LSU %i",Poffset, lsu);

				  								Setlogicaldetector(lsu);

				  							}

				  						}
				  				}
				  			}
			 }

			 // check for Train occupancy based on Train ID codes.

			 for (int Trss = 1; Trss < NUMBEROF_LS_TRAIN_IDS; Trss++)
			 {
				 	if ((Logicalsectiondata[lsu].LogicalsectionTrainID[Trss] > 0) && (Logicalsectiondata[lsu].LogicalsectionTcode == 0))
				 	{
				 		Logicalsectiondata[lsu].LogicalsectionTcode = 994;
				 //		Logicalsectiondata[lsu].LogicalsectionTrainID[0] = 994;
				 	}
			 }

		}

		//PointUpdateprocess();								// Execute point processing

		Complexpointprocessing();							// processing complex point clusters

		TSUpdatetrainsectionsPhysectionstatus();			// Refresh Physical sections status for Train sections

		//std::cout<<"\n3292 Point status codes for signals:"<<PointstatuscodesforSignals[1];

		RefreshTcodesandPstatusforSignals();

		LNLSLDsignalprocess->Refreshsignalvalues(TrainsectionstatusforSignals,SectionTcodesforSignals,PointstatuscodesforSignals, 0);		// execute signal refresh process

		int Psectionstatus[MAXPSECTIONS];

		int Lsectionsloglength = NUMBEROFLOGICALSECTIONS * 5;

		int Psectionloglength = MAXPSECTIONS * 4;

		char PLSectionlog[Psectionloglength + Lsectionsloglength + 64];

		char LSectionlog[10];

		int PSslogcharcounter = 0;

		int LSslogcharcounter = 0;

		sprintf(PLSectionlog,"P:%03d>",MAXPSECTIONS);

		PSslogcharcounter = 6;

		LSslogcharcounter = 6;

		char PSectiondata[2];

		char LSectiondata[5];

		char* PLSectionlogptr = &PLSectionlog[0];

		//for (int PSs = 1;PSs < 16; PSs++)
		for (int PSs = 1;PSs < MAXPSECTIONS; PSs++)
		{
			 sprintf(PSectiondata,"%01d-",PSectiondatareturn(PSs,&PSmessage[0]));
			 PLSectionlog[PSslogcharcounter] = PSectiondata[0];
			 PSslogcharcounter++;
			 PLSectionlog[PSslogcharcounter] = PSectiondata[1];
			 PSslogcharcounter++;
		}

		PLSectionlog[PSslogcharcounter] = '>';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = 'L';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = ':';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = '1';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = '2';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = '8';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = '>';
		PSslogcharcounter++;

		for (int LSs = 1;LSs < NUMBEROFLOGICALSECTIONS; LSs++)
		{
			sprintf(LSectiondata,"%04d-",Logicalsectiondata[LSs].LogicalsectionTcode);
			PLSectionlog[PSslogcharcounter] = LSectiondata[0];
			PSslogcharcounter++;
		    PLSectionlog[PSslogcharcounter] = LSectiondata[1];
		    PSslogcharcounter++;
		    PLSectionlog[PSslogcharcounter] = LSectiondata[2];
		    PSslogcharcounter++;
		    PLSectionlog[PSslogcharcounter] = LSectiondata[3];
		    PSslogcharcounter++;
		    PLSectionlog[PSslogcharcounter] = LSectiondata[4];
		   	PSslogcharcounter++;
		}
		PLSectionlog[PSslogcharcounter] = '>';
		PSslogcharcounter++;
		PLSectionlog[PSslogcharcounter] = '\0';

		//std::cout<<"\n2923 PLSection counter "<<PSslogcharcounter<<" log record ->"<< PLSectionlog;

		//std::cout<<"\n";

		LNOploggingptr->Writelatestlogfile(PLSectionlogptr);				// update latest log record

		LNOploggingptr->WriteOphistlogmessage(0, PLSectionlogptr);		// update log history file



	//UpdateSignalRelayoutputcode();			// Update signal relays

 }
 
 void LNwebLsectionsLdevices::RefreshTcodesandPstatusforSignals()
 {
	 bzero(SectionTcodesforSignals,NUMBEROFLOGICALSECTIONS);
	 
	 for (int SLS = 0; SLS < NUMBEROFLOGICALSECTIONS; SLS++)
	 {
		 SectionTcodesforSignals[SLS] = Logicalsectiondata[SLS].LogicalsectionTcode;
	 }	 

	 bzero(PointstatuscodesforSignals,NUMBEROFPOINTS+1);

	 for (int SLP = 0; SLP < NUMBEROFPOINTS + 1; SLP++)
	 {
		 PointstatuscodesforSignals[SLP] = PointStatusTable[SLP].CurrOpenClose;
	//	 std::cout<<"\n3393 Point:"<<SLP<<" OC status:"<<PointstatuscodesforSignals[SLP];
	 }

	 bzero(TrainsectionstatusforSignals,MAXTRAINSECTIONS);

	 for (int TSss = 0; TSss < MAXTRAINSECTIONS; TSss++)
	 {
		// std::cout<<"\n4022 Building Trainsection table entry:"<<TSss<<" TrainNumber:"<<TrainSectiondata[TSss].Traincode;

		TrainsectionstatusforSignals[TSss] = TrainSectiondata[TSss].Traincode;
	 }
 } 

 void LNwebLsectionsLdevices::LoadLSlogdata(char* LSlogdataptr)
 {
	 //std::cout<<"\n2850 Op LS log to store:"<<LSlogdataptr;

	 char LScode[4];

	 int LSlogdataptroffset = 0;

	 int calcLSvalue = 0;

	 int charcounter = 0;

	 for (int LSsectionlogscc = 1;LSsectionlogscc < NUMBEROFLOGICALSECTIONS; LSsectionlogscc++)
	 {
		 bzero(LScode,4);

		 calcLSvalue = 0;

		 LScode[charcounter] = *(LSlogdataptr  + LSlogdataptroffset) & 0X0F;

		 LSlogdataptroffset++;
		 charcounter++;

		 LScode[charcounter] = *(LSlogdataptr  + LSlogdataptroffset) & 0X0F;

		 LSlogdataptroffset++;
		 charcounter++;


		 LScode[charcounter] = *(LSlogdataptr  + LSlogdataptroffset) & 0X0F;

		 LSlogdataptroffset++;
		 charcounter++;


		 LScode[charcounter] = *(LSlogdataptr  + LSlogdataptroffset) & 0X0F;

		 LSlogdataptroffset++;

		 charcounter = 0;

		 calcLSvalue = (LScode[0] * 1000) + (LScode[1] * 100) + (LScode[2] * 10) + (LScode[3] * 1);

		 LSlogdataptroffset++;

		 //std::cout<<"\n2902 LS:"<<LSsectionlogscc<<" 4 char value:"<<LScode<<" calc val:"<<calcLSvalue;

		 Logicalsectiondata[LSsectionlogscc].LogicalsectionTcode = calcLSvalue;
	 }

 }

 void LNwebLsectionsLdevices::Setlogicaldetector(int ld)
 {
 	Logicaldetectors[ld].Status = 1;

 	Logicaldetectortriggersprocess();
 }

 void LNwebLsectionsLdevices::Logicaldetectornumerictriggersprocess()
  {
 	 // Check to see if Logical detector triggers are activated where there is a numeric trigger target
 	
 	 int idt = 0;


 	 for (idt = 0;idt <= LOGICALDETECTORS;idt++)
 	 {
 		//std::cout<<"\n1664 Checking logical trigger number:"<<idt<<" for device:"<< \
 				DetectorTriggerTable[idt].Triggerdevicenumber<<" Target:"<<DetectorTriggerTable[idt].TargetDevicecounter<<" Actual:"<<DetectorTriggerTable[idt].ActualDevicecounter<<" Enabled flag:"<<DetectorTriggerTable[idt].Countertrigenabled;


 		 if (DetectorTriggerTable[idt].Triggerdevicenumber > 0)
 		 {			

 			if ((DetectorTriggerTable[idt].TargetDevicecounter > 0) & (DetectorTriggerTable[idt].TargetDevicecounter == DetectorTriggerTable[idt].ActualDevicecounter) \
 			 & (DetectorTriggerTable[idt].Countertrigenabled == 0))
 			{
 				printf("\n1924 Detector %i triggered and Enabled for type %i and device %i ",idt,DetectorTriggerTable[idt].TriggerPTorUR,DetectorTriggerTable[idt].TriggeredOutputdevicenumber);

 				if (DetectorTriggerTable[idt].TriggerPTorUR == 1)
 				{ 			
					DetectorTriggerTable[idt].Countertrigenabled = 1;

					if (DetectorTriggerTable[idt].Trainreference > 0)
					{
						int Trainsubscript = CheckTrainIDinsection(DetectorTriggerTable[idt].ActionLSreference, DetectorTriggerTable[idt].Trainreference);

						SetTrainIDcoupled(DetectorTriggerTable[idt].ActionLSreference,Trainsubscript,0);		// uncouple the train
						SetTrainIDcoupled(DetectorTriggerTable[idt].ActionNSLSreference,Trainsubscript,0);
						UpdateTrainID(DetectorTriggerTable[idt].ActionNSLSreference,Trainsubscript,0,7);
						LNallTrainsinformationLSLD->UpdateCurrentLSlocation(DetectorTriggerTable[idt].Trainreference,0, DetectorTriggerTable[idt].ActionLSreference);
																												// clear from the next section
						// set the corresponding physical section to be occupied

						//Logicalsectiondata[DetectorTriggerTable[idt].ActionLSreference].LogicalsectionTcode = 994;
						//Logicalsectiondata[DetectorTriggerTable[idt].ActionLSreference].LogicalsectionTrainID[0] = 994;
						int Psection994code = Logicalsectiondata[DetectorTriggerTable[idt].ActionLSreference].Physectionref;


						std::cout<<"\n1930 Uncoupler ramp activated device train ref:"\
								<<DetectorTriggerTable[idt].Trainreference<<" in LS:"\
								<<DetectorTriggerTable[idt].ActionLSreference<<" TrIDref:"<<Trainsubscript\
								<<" NS ref:"<<DetectorTriggerTable[idt].ActionNSLSreference<<" Psection:"<<Psection994code\
								<<" LS:"<< DetectorTriggerTable[idt].ActionLSreference<<" LS Tcode:"<<Logicalsectiondata[DetectorTriggerTable[idt].ActionLSreference].LogicalsectionTcode;
					}
 				}
 			}
 		 }
 	 }
  }

  void LNwebLsectionsLdevices::Logicaldetectortriggersprocess()
  {
 	 // Check to see if Logical detector triggers are activated 	

 	 int idt = 0;

 	/*


	struct Detectortrigger
	{
		int Triggerdevicenumber;
		int Triggerenabled; 			// 0 - disabled, 1 - enabled
		int Triggerstatus;              // 0 - set , 1 - actioned
		int TriggerPTorUR;              // 1 - uncoupler ramp. 2 - switch (point)
		int TriggeredOutputdevicenumber;          // number of switch or uncoupler ramp
		int ActualDevicecounter;		// Actual number of device triggers
		int TargetDevicecounter;		// Target number of device triggers
		int Countertrigenabled;         // Enable flag for counter based trigger
		int LastswitchOnOff;         	// Log of last switch - toggle
		int Trainreference;				// Train reference for linkage to device
		int Locoreference;				// Loco reference for linkage to device
	};

	struct Detectortrigger DetectorTriggerTable[LOGICALDETECTORS+1];

 	*/

 	 for (idt = 0;idt <= LOGICALDETECTORS;idt++)
 	 {

 		 if (DetectorTriggerTable[idt].Triggerdevicenumber > 0)
 		 {

 			if ((GetLDstatus(idt) > 0) & (DetectorTriggerTable[idt].Triggerenabled > 0) & (DetectorTriggerTable[idt].Triggerstatus == 0) && (DetectorTriggerTable[idt].Countertrigenabled == 1))
 			{

 				if (DetectorTriggerTable[idt].TriggerPTorUR == 1)		//  1 = has triggered an uncoupling ramp
 				{
					Actionswitch(DetectorTriggerTable[idt].TriggeredOutputdevicenumber,8,1);

					SetUncouplerstatus(DetectorTriggerTable[idt].TriggeredOutputdevicenumber,2);

					DetectorTriggerTable[idt].Triggerstatus = 1;
 				}
 			}
 		 }

 	 }
  }


void LNwebLsectionsLdevices::SwitchpointtoClose(int LPcode)
   {
  	 PointStatusTable[LPcode].CurrOpenClose = 0;
   }

void LNwebLsectionsLdevices::SwitchpointtoOpen(int LPcode)
   {
  	 PointStatusTable[LPcode].CurrOpenClose = 1;
   }

void LNwebLsectionsLdevices::PointUpdateprocess()        
  {
	  // Switches points based on updates to Logical section Train codes

		//std::cout<<"\n3175 Perform point update process ";

	  	// Step 1 refresh Tcodes for each logical section
	/*
	  	int pointcode  = 0;  	

	  	for (pointcode = 1;pointcode <= NUMBEROFPOINTS;pointcode++)
	  	{
	  		if (PointprocessTable[pointcode].PPointnumber > 0)
	  		{
	  		PointprocessTable[pointcode].ForwardCloseLSTcode = GetLogicalSectionTcode(PointprocessTable[pointcode].ForwardCloseLS);
	  		PointprocessTable[pointcode].BackCloseLSTcode = GetLogicalSectionTcode(PointprocessTable[pointcode].BackCloseLS);
	  		PointprocessTable[pointcode].ForwardOpenLSTcode = GetLogicalSectionTcode(PointprocessTable[pointcode].ForwardOpenLS);
	  		PointprocessTable[pointcode].BackOpenLSTcode = GetLogicalSectionTcode(PointprocessTable[pointcode].BackOpenLS);

	  		Displaypointconfig(pointcode);

	  	// 1 = closed, 0 = open

	  		if ((PointprocessTable[pointcode].ForwardCloseLSTcode > 0) & (PointprocessTable[pointcode].ForwardCloseLSTcode != 999) & (PointprocessTable[pointcode].ForwardCloseLSTcode != 998))
	  			{
	  				//printf("\nTrain on point %i Forward close LS ",pointcode);
	  				SwitchpointtoClose(pointcode);
	  			}

	  			if ((PointprocessTable[pointcode].BackCloseLSTcode > 0) & (PointprocessTable[pointcode].BackCloseLSTcode != 999) & (PointprocessTable[pointcode].BackCloseLSTcode != 998))
	  			{
	  				//printf("\nTrain on point %i Back close LS ",pointcode);
	  				SwitchpointtoClose(pointcode);
	  			}

	  			if ((PointprocessTable[pointcode].ForwardOpenLSTcode > 0) & (PointprocessTable[pointcode].ForwardOpenLSTcode != 999) & (PointprocessTable[pointcode].ForwardOpenLSTcode != 998))
	  			{
	  				//printf("\nTrain on point %i Forward Open LS ",pointcode);
	  				SwitchpointtoOpen(pointcode);
	  			}

	  			if ((PointprocessTable[pointcode].BackOpenLSTcode > 0) & (PointprocessTable[pointcode].BackOpenLSTcode != 999) & (PointprocessTable[pointcode].BackOpenLSTcode != 998))
	  			{
	  				//printf("\nTrain on point %i Back Open LS ",pointcode);
	  				SwitchpointtoOpen(pointcode);
	  			}
	  		}
	  	}

	  //	LogicalDetectorupdateDetectorstatus();

	  	//Pointcurrentstatusprocess();  				// process the point triggers
		*/
	  	CrossoverUpdateprocess();					// process the crossovers


}

void LNwebLsectionsLdevices::SetLSection(int section, int Traincode)
{
	// printf("\nSet logical section %i to train code %i \n", section, Traincode);

	// Logicalsectiondata[section].LogicalsectionTcode = Traincode;

	 UpdateTcode(section,Traincode);

	// LogicalSectionTcodeRefresh();

}

void LNwebLsectionsLdevices::CrossoverUpdateprocess()
{
	// identifies if logical sections need to be blocked if they are in the crossover

	//printf("\nCrossover processing ....\n");

	int Crossoverrecordss;

	for (Crossoverrecordss = 1;Crossoverrecordss<=Crossoverrecordloaded;Crossoverrecordss++)
	{
		Crossovertable[Crossoverrecordss].sourcetcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].source);
		//Crossovertable[Crossoverrecordss].target1tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target1);
		//Crossovertable[Crossoverrecordss].target2tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target2);
		//Crossovertable[Crossoverrecordss].target3tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target3);
		//Crossovertable[Crossoverrecordss].target4tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target4);
		//Crossovertable[Crossoverrecordss].target5tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target5);
		//Crossovertable[Crossoverrecordss].target6tcode = GetLogicalSectionTcode(Crossovertable[Crossoverrecordss].target6);

		if ((Crossovertable[Crossoverrecordss].sourcetcode > 0) & (Crossovertable[Crossoverrecordss].sourcetcode != 999))
		{
			SetLSection(Crossovertable[Crossoverrecordss].target1, 999);
			SetLSection(Crossovertable[Crossoverrecordss].target2, 999);

			if (Crossovertable[Crossoverrecordss].target3 > 0)
			{
			SetLSection(Crossovertable[Crossoverrecordss].target3, 999);
			}

			if (Crossovertable[Crossoverrecordss].target4 > 0)
			{
			SetLSection(Crossovertable[Crossoverrecordss].target4, 999);
			}

			if (Crossovertable[Crossoverrecordss].target5 > 0)
			{
			SetLSection(Crossovertable[Crossoverrecordss].target5, 999);
			}

			if (Crossovertable[Crossoverrecordss].target6 > 0)
			{
			SetLSection(Crossovertable[Crossoverrecordss].target6, 999);
			}

			Crossovertable[Crossoverrecordss].sourceprevtcode = Crossovertable[Crossoverrecordss].sourcetcode;
		}
		else
		{
			if ((Crossovertable[Crossoverrecordss].sourcetcode == 0) & (Crossovertable[Crossoverrecordss].sourceprevtcode > 0) & (Crossovertable[Crossoverrecordss].sourceprevtcode != 999))
			{
				SetLSection(Crossovertable[Crossoverrecordss].target1, 0);
				SetLSection(Crossovertable[Crossoverrecordss].target2, 0);

				if (Crossovertable[Crossoverrecordss].target3 > 0)
				{
				SetLSection(Crossovertable[Crossoverrecordss].target3, 0);
				}

				if (Crossovertable[Crossoverrecordss].target4 > 0)
				{
				SetLSection(Crossovertable[Crossoverrecordss].target4, 0);
				}

				if (Crossovertable[Crossoverrecordss].target5 > 0)
				{
				SetLSection(Crossovertable[Crossoverrecordss].target5, 0);
				}

				if (Crossovertable[Crossoverrecordss].target6 > 0)
				{
				SetLSection(Crossovertable[Crossoverrecordss].target6, 0);
				}

			}
			else
			{
				Crossovertable[Crossoverrecordss].sourceprevtcode = Crossovertable[Crossoverrecordss].sourcetcode;
			}

		}
	}

}

int LNwebLsectionsLdevices::GetLDstatus(int ld)
{
	int Poffset, LDstatus;

	char PSmessage[SHORTMESSAGELENGTH ];

	Poffset = Logicaldetectors[ld].DetectortriggerPhysectionref;

	if (Poffset > 0)
	{
		LDstatus = PSectiondatareturn(Poffset,&PSmessage[0]);
	}
	else
	{
		LDstatus = 0;
	}


	return LDstatus;
}

void LNwebLsectionsLdevices::Logicaldetectorarraytablesinitialize()
{
	/*
	struct Logicaldetectorarray
	{
		int Logicaldetector;
		int DPhysectionref;			// Physical section location of the device
		int LocoNetPSectiontrigger;	// Loconet physical section that triggers the device
		int Status; 				// 0 = not set, 1 = set
		int Detectortype;			// 01 - reed switch, 02 RFid reader 03 infrared detector
		int DetectortriggerPhysectionref;
		int Instancecountofthistype;	// instance count of a detector type
		int Triggercount; 				// count of how many times triggered since last reset
		char RFidcode[RFIDCHARCOUNT];	// RFidcode received by detector
		int Statustotriggertimer;       // status to trigger time
		int Statustriggertimestamp;     // time when detector went to triggered status
		int Durationsincestatuschange;  // time period since status
	};

	struct Logicaldetectorarray Logicaldetectors[LOGICALDETECTORS + 1];

	*/

	for (int ldc = 0; ldc <=LOGICALDETECTORS; ldc++)
	{
		Logicaldetectors[ldc].Logicaldetector 	= 0;
		Logicaldetectors[ldc].DPhysectionref	= 0;
		Logicaldetectors[ldc].LocoNetPSectiontrigger = 0;
		Logicaldetectors[ldc].Status				= 0;
		Logicaldetectors[ldc].Detectortype			= 0;
		Logicaldetectors[ldc].DetectortriggerPhysectionref	= 0;
		Logicaldetectors[ldc].Instancecountofthistype		= 0;
		Logicaldetectors[ldc].Triggercount					= 0;

		for (int ldcRF = 0; ldcRF < RFIDCHARCOUNT; ldcRF++)
		{
			Logicaldetectors[ldc].RFidcode[ldcRF] = ' ';
		}
		Logicaldetectors[ldc].Statustotriggertimer			= 0;
		Logicaldetectors[ldc].Statustriggertimestamp		= 0;
		Logicaldetectors[ldc].Durationsincestatuschange		= 0;
	}
}

void LNwebLsectionsLdevices::LogicalDetectorupdateDetectorstatus()
{

	char PSmessage[SHORTMESSAGELENGTH ];

	int dsu = 0;

	int Occupstatus = 0;

	int Poffset = 0;

	//printf("\n1847 Number of detectors loaded %i \n",NumberofLogicaldetectorsloaded);

	if (NumberofLogicaldetectorsloaded > 0)
	{

		for (dsu = 1; dsu < LOGICALDETECTORS; dsu++)
		{
			Poffset = Logicaldetectors[dsu].DetectortriggerPhysectionref;

			//printf("\n4094.... dsu %i ref %i", dsu,Poffset);

			if (Poffset > 0)
			{
				Occupstatus = PSectiondatareturn(Poffset,&PSmessage[0]);

				//printf("\n 4100 Update LD  Poffset %i  DSU %i  Occupstatus %i ",Poffset, dsu,  Occupstatus );

				Processdetectorcount(dsu,Occupstatus);			// Update device counter

				Logicaldetectornumerictriggersprocess();		// Check to see if it has activated a number target trigger

				if (Occupstatus > 0)
				{
					Setlogicaldetector(dsu);
				}
			}
		}
	}
}

void LNwebLsectionsLdevices::InitStopflagtable()
 {

	/*struct Stopflagarray
	{
    int  Type;
    int	 Enabled;
    int  Status;
    int  Devicenumber;
    char Previoussection;
    char Currentsection;
    int  LocotoStop;
    char Locostopcode[8];
	};
	*/
	int sfc;

	for (sfc = 0;sfc < (MAXNUMBEROFSTOPFLAGS+1);sfc++)
	{
		Stopflagtable[sfc].Type		 		= 0;
		Stopflagtable[sfc].Enabled	 		= 0;
		Stopflagtable[sfc].Status			= 0;
		Stopflagtable[sfc].Devicenumber 	= 0;
		Stopflagtable[sfc].Previoussection 	= 0;
		Stopflagtable[sfc].Currentsection 	= 0;
		Stopflagtable[sfc].LocotoStop	 	= 0;
		Stopflagtable[sfc].LocostopTcode	= 0;

	}
}

void LNwebLsectionsLdevices::DisplayStopflagdata(char* replymessage)
{
	/* Clear the display array line  */

	/*
	  struct Stopflagarray
	{
    int  Type;
    int	 Enabled;
    int  Status;
    int  Devicenumber;
    char Previoussection;
    char Currentsection;
    int  LocotoStop;
    char Locostopcode[8];
	};
	 */
	char outputrec[256];

	int isc,ic,mcc;

	mcc = 0;

    int sccl = 0;

	for (sccl = 0;sccl <= MAXNUMBEROFSTOPFLAGS;sccl++)
	{
		bzero(outputrec,256);

		sprintf(outputrec,"%01d-%01d-%01d-%02d-%03d-%03d-%02d-%04d+",Stopflagtable[sccl].Type,Stopflagtable[sccl].Enabled,\
				Stopflagtable[sccl].Status,Stopflagtable[sccl].Devicenumber,Stopflagtable[sccl].Previoussection,\
				Stopflagtable[sccl].Currentsection,Stopflagtable[sccl].LocotoStop,Stopflagtable[sccl].LocostopTcode);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
				{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
				}
	}



	*(replymessage + mcc) = '\n';

	mcc++;

	*(replymessage + mcc) = '\n';

	return;
}

void LNwebLsectionsLdevices::DisplayDetectorTablestatus(char* replymessage)
{
		char outputrec[512];

		int isc,ic,mcc;

		mcc = 0;

	    int sccl = 0;

		//int Psec = 0;

		//int nextposition;

		for (sccl = 0;sccl <= LOGICALDETECTORS;sccl++)
		{
			bzero(outputrec,512);

			sprintf(outputrec,"\nDevice:%02d Trig device:%02d Triggerenabled:%01d Triggerstatus:%01d TriggerPTorUR:%01d Triggered-OutputdeviceNumber:%02d Actualcount:%04d Targetcount:%04d Countenabled:%01d+",sccl,DetectorTriggerTable[sccl].Triggerdevicenumber,DetectorTriggerTable[sccl].Triggerenabled,\
					DetectorTriggerTable[sccl].Triggerstatus,DetectorTriggerTable[sccl].TriggerPTorUR,DetectorTriggerTable[sccl].TriggeredOutputdevicenumber,
					DetectorTriggerTable[sccl].ActualDevicecounter,DetectorTriggerTable[sccl].TargetDevicecounter,DetectorTriggerTable[sccl].Countertrigenabled);

			isc =  std::strlen(outputrec);

			if (DetectorTriggerTable[sccl].Triggerdevicenumber > 0)
			{
				for (ic = 0;ic <isc;ic++)
					{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
					}
			}
		}
		*(replymessage + mcc) = '\n';

		mcc++;

		*(replymessage + mcc) = '\n';

		return;


}

void LNwebLsectionsLdevices::DetectorTableUpdate(char* replymessage)
{
	//int scu = 0;

	/****************** Detector trigger processing **********************

	struct Detectortrigger
	{
	int Triggerdevicenumber;
	int Triggerenabled; 			// 0 - disabled, 1 - enabled
	int Triggerstatus;              // 0 - set , 1 - actioned
	int TriggerPTorUR;              // 1 - uncoupler ramp. 2 - switch (point)
	int TriggeredOutputdevicenumber;           // number of switch or uncoupler ramp
	};

	extern struct Detectortrigger DetectorTriggerTable[LOGICALDETECTORS]; */


    char outputrec[256];

	int isc,ic,mcc;

	mcc = 0;

    int sccl = 0;

	//int Psec = 0;

	//int nextposition;

	for (sccl = 0;sccl <= LOGICALDETECTORS;sccl++)
	{
		bzero(outputrec,256);

		sprintf(outputrec,"%04d-%01d-%01d-%01d-%01d-%04d-%04d-%01d+",DetectorTriggerTable[sccl].Triggerdevicenumber,DetectorTriggerTable[sccl].Triggerenabled,\
				DetectorTriggerTable[sccl].Triggerstatus,DetectorTriggerTable[sccl].TriggerPTorUR,DetectorTriggerTable[sccl].TriggeredOutputdevicenumber,
				DetectorTriggerTable[sccl].ActualDevicecounter,DetectorTriggerTable[sccl].TargetDevicecounter,DetectorTriggerTable[sccl].Countertrigenabled);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
				{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
				}
	}
	*(replymessage + mcc) = '\n';

	mcc++;

	*(replymessage + mcc) = '\n';

	return;

}


void LNwebLsectionsLdevices::BrowserStopflagUpdate(int code, int type, int stopdevice,int stopPS, int stopCS, int stopslot, int stopTcode, int stopflg)
 {

	//std::cout<<"\nStop flag processing";

	if (stopflg == 3)
	{
		//std::cout<<"\nReset stop flags command received";

		InitStopflagtable();
	}

    if (stopflg ==1)
    {
    	int sfc = code;

    	Stopflagtable[sfc].Type		 		= type;
    	Stopflagtable[sfc].Enabled	 		= 1;
    	Stopflagtable[sfc].Status			= 0;
    	Stopflagtable[sfc].Devicenumber 	= stopdevice;
    	Stopflagtable[sfc].Previoussection 	= stopPS;
    	Stopflagtable[sfc].Currentsection 	= stopCS;
    	Stopflagtable[sfc].LocotoStop	 	= stopslot;
    	Stopflagtable[sfc].LocostopTcode	= stopTcode;
    }

 }

void LNwebLsectionsLdevices::StopFlagReviewprocess()
{
	// process any changes in SECTION or DEVICE status that may activate a stop flag process

	/*
	struct Stopflagarray
	{
    int  Type;
    int	 Enabled;
    int  Status;
    int  Devicenumber;
    char Previoussection;
    char Currentsection;
    int  LocotoStop;
    int  LocostopTcode;
	};

	extern struct Stopflagarray Stopflagtable[MAXNUMBEROFSTOPFLAGS+1];
	*/

	//printf ("\nProcessing stop flags\n");

	int sfcounter, Sdevice,Lssc, SPs, SCs, SPspvalRC, SCspvalRC, LocoTcode;

	for (sfcounter = 1; sfcounter < (MAXNUMBEROFSTOPFLAGS + 1); sfcounter++)
	{
		if (Stopflagtable[sfcounter].Enabled > 0)		// check to see if flag is enabled for processing
		{
			//printf ("\nProcessing stop flag %i\n", sfcounter);

			if (Stopflagtable[sfcounter].Status == 0) 	// check to see if flag has already been processed
			{
				if (Stopflagtable[sfcounter].Type == 1)	// check to see if the stop flag is a Device type
				{
					// check to see if Device is trigger , if so trigger the stop process

					Sdevice = Stopflagtable[sfcounter].Devicenumber;

					if (Logicaldetectors[Sdevice].Status > 0) // test to see if logical device is triggered
					{
						Stopflagtable[sfcounter].Status = 1;

						Lssc = 	Stopflagtable[sfcounter].LocotoStop;

						printf("\nStop flag triggered for Device %i on Loco slot %i on Tcode %i ",Sdevice,Lssc,Stopflagtable[sfcounter].LocostopTcode);

						// CSpeedcmdprocess(SLCslot, SLCslotspeed, SLCslotdirection);

						CSpeedcmdprocess(53,Lssc,0,0);  // Set speed to zero for the specified slot - called in LNcommandprocess routine
					}
				}
				else
				{
					SPs = Stopflagtable[sfcounter].Previoussection;		// extract the previous section code

					SCs = Stopflagtable[sfcounter].Currentsection;		// extract the current section code

					LocoTcode = Stopflagtable[sfcounter].LocostopTcode; // extract the loco T code

					// extract the physical values for the logical sections

					// int LogicaltoPhysicalstatuscheck(int LScode)

					// Test type 1 = Tcode matched and no physical occupancy

					//      type 2 = Tcode matched and physical occupany confirmed

					// return code if conditions matched

					SPspvalRC = LogicaltoPhysicalstatuscheck(SPs,LocoTcode,1);

					//printf("\nPS check %03d TC %02 ", SPs, SPspvalRC);

					SCspvalRC = LogicaltoPhysicalstatuscheck(SCs,LocoTcode,2);

					//printf(" --- CS check %03d TC %02 ", SCs, SCspvalRC);

					//printf("\nSection check PS %03d CS %03d Tcode %04d SPPS %02d SPCS %02d ",SPs, SCs, LocoTcode, SPspvalRC, SCspvalRC);



					if ((SPspvalRC == 0) && (SCspvalRC == 0))			// check to see if loco is clear of previous section and in current section only
					{
						Stopflagtable[sfcounter].Status = 1;

						Lssc = Stopflagtable[sfcounter].LocotoStop;

						printf("\nStop flag triggered for Device %02d Sections PS %i CS %i on Loco slot %i on Tcode %i ",Stopflagtable[sfcounter].Type, SPs,SCs,Lssc,Stopflagtable[sfcounter].LocostopTcode);

						CSpeedcmdprocess(52,Lssc,0,0);  // Set speed to zero for the specified slot - called in LNcommandprocess routine
					}
				}
			}
		}
	}

}

int LNwebLsectionsLdevices::LogicaltoPhysicalstatuscheck(int LScode, int LTcode, int testtype)
{

	// Routine to return physical section code for a specified logical section
	// Logical section is mapped back to the physical section and then the physical section status is returned
	// Routine is designed to handle logical sections with multiple physical codes
	//
	//
	// Test type 1 = Tcode matched and no physical occupancy

	//      type 2 = Tcode matched and physical occupany confirmed

	// return code 0 if conditions are matched

    /*
		struct LogicalsectionArray
		{
		int Physectionref;
		int LogicalsectionTcode;
		int Logicalsectiondirection;
		int Logicalsectiontype;
		int Physectionref2;
		};

		extern struct LogicalsectionArray Logicalsectiondata[NUMBEROFLOGICALSECTIONS];

      */

	if (LTcode > 0)				// check to see if LocoTcode is specified and if so then run the check
	{
		if (GetLSTcode(LScode) != LTcode)
		{
			return 3;		// no match on loco code
		}
	}

	int lsu = LScode;

	int Occupstatus = 0;

	int Occupstatus2 = 0;

	int PSectiontype = 0;

	char Messageback[8];

	int Poffset = 0;

	int Poffset2 = 0;

	PSectiontype = Logicalsectiondata[lsu].Logicalsectiontype;

	Poffset = Logicalsectiondata[lsu].Physectionref;

	Poffset2 = Logicalsectiondata[lsu].Physectionref2;

	if ((PSectiontype == 1)|| (PSectiontype == 2))	// single physical section in one logical section
		{
		  if (Poffset > 0)
		  {
			//Occupstatus = Physicalsection[Poffset].Occupied;   //PSectiondatareturn(int PS, char* returnmessage)

			Occupstatus = PSectiondatareturn(Poffset,&Messageback[0]);
		  }
	    }

	 else
	    {

		if ((PSectiontype == 3)|| (PSectiontype == 4))  // multiple physical sections in one logical section
			{
				if ((Poffset > 0) || (Poffset2 > 0))
				{
				//	Occupstatus = Physicalsection[Poffset].Occupied;
				//	Occupstatus2 = Physicalsection[Poffset2].Occupied;

					Occupstatus = PSectiondatareturn(Poffset,&Messageback[0]);
					Occupstatus2 = PSectiondatareturn(Poffset2,&Messageback[0]);
				}
			}
		}


	//
	// Test type 1 = Tcode matched and no physical occupancy

	//      type 2 = Tcode matched and physical occupany confirmed


	if ((testtype == 1) && (Occupstatus == 0) && (Occupstatus2 == 0))
	{
		return 0;
	}

	if (testtype == 2)
	{
		if ((Occupstatus == 0) || (Occupstatus2 == 0))
		{
			return 0;
		}
	}

	return 2;	// no match on occupancy code
}

void LNwebLsectionsLdevices::UpdateIRdetectors(int* sensorstateptr)
{
	printf("\n\n3097 - S-%01d-%01d-%01d-%01d-%01d-%01d-%01d-%01d-%01d-%01d-%01d-%01d\n",\
	*sensorstateptr,*(sensorstateptr+1),*(sensorstateptr+2),*(sensorstateptr+3),\
	*(sensorstateptr+4),*(sensorstateptr+5),*(sensorstateptr+6),*(sensorstateptr+7),\
      *(sensorstateptr+8),*(sensorstateptr+9),*(sensorstateptr+10),*(sensorstateptr+11));	
      
    int detectorcountersubscript = 0;
    
    for (int ldc = 0; ldc < LOGICALDETECTORS; ldc++)
	{
		if ((Logicaldetectors[ldc].Detectortype == 7) || (Logicaldetectors[ldc].Detectortype == 8)) 
		{
			std::cout<<"\n3108 IFRsensor matched type "<< Logicaldetectors[ldc].Detectortype\
			<<" at instance "<<Logicaldetectors[ldc].Instancecountofthistype<<" on detector counter "\
			<<ldc<<" subscript = "<<detectorcountersubscript<<" value : "<<*(sensorstateptr + detectorcountersubscript);
			
			Logicaldetectors[ldc].Status = *(sensorstateptr + detectorcountersubscript);
			Logicaldetectors[ldc].Triggercount++;
			detectorcountersubscript++;
		}	
		std::cout<<"\n";	
	}
	
}

void LNwebLsectionsLdevices::UpdateRFid_detectors(int RFid_device, char* RFidcode)
{
	std::cout<<"\n3106 RFid detector data feed from device "<<RFid_device<<" "<<RFidcode<<"\n";

	if (RFid_device == 0)
	{
		RFid_device++;			// special workaround for device 0 records
	}

    for (int ldc = 0; ldc < LOGICALDETECTORS; ldc++)
	{
	 if ((Logicaldetectors[ldc].Detectortype == 9) && (Logicaldetectors[ldc].Instancecountofthistype == RFid_device))
	 {
		std::cout<<"\nRFid device matched \n";
		for (int ldcrfs = 0; ldcrfs < RFIDCHARCOUNT; ldcrfs++)
		{
			Logicaldetectors[ldc].RFidcode[ldcrfs] = *(RFidcode + ldcrfs);
		}
		
		Logicaldetectors[ldc].Triggercount++;
	 }
	}

	
}




