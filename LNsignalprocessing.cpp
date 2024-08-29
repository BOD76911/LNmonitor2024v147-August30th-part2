
/*
 * LNsignalprocessing.cpp
 *
 *  Created on: 31 Oct 2022
 *      Author: brian
 */

#include "LNsignalprocessing.h"

#include <iostream>			// include standard library of io routines

#include <string>
#include <string.h>
#include <exception>
#include <cstring>
#include <iostream>			// include standard library of io routines
#include <unistd.h>


LNsignalprocessing::LNsignalprocessing() {
	// TODO Auto-generated constructor stub

}

LNsignalprocessing::~LNsignalprocessing() {
	// TODO Auto-generated destructor stub
}

void LNsignalprocessing::Storesignalconfigdata(char* lineinputbufferptr, int displayflag)
{
	//std::cout<<"\n31 Signal config record->"<<lineinputbufferptr;

	int value = 0;

	int signalnumber = 0;

	int* valueptr = &value;

	Bufferprocessing* FLPCNF = new Bufferprocessing();

	FLPCNF->Fileparse(lineinputbufferptr, ';', 0);

	if (FLPCNF->ParamdataInteger(2, valueptr) == 0)
	{
		signalnumber = value;
	}

	//std::cout<<"\n\n48 Loading Signal number:"<<signalnumber<<" buffer:"<<lineinputbufferptr<<"\n\n";

	Signalprocessdata[signalnumber].Signalnumber = signalnumber;

	if (FLPCNF->ParamdataInteger(3, valueptr) == 0)
	{
		Signalprocessdata[signalnumber].Signaltype = value;
	}

	// split field 4 to extract PS (physical sections) fields - delimited by + character

	//if (FLPCNF->ParamdataInteger(4, valueptr) == 0)
	//{
	//	std::cout<<"\n60 Signal Psection:"<<value;
	//	Signalprocessdata[signalnumber].Signalphysicalsection[0] = value;
	//}

	Bufferprocessing* BPFP = new Bufferprocessing();

	int PSfc = BPFP->Fileparse(FLPCNF->Paramdata(4), '+', 0);

	//std::cout<<"\n70 PSfc:"<<PSfc<<"\n";

	// extract LS reference

	for (int PScc = 0;PScc<PSfc;PScc++)
	{
		value = 0;
		BPFP->ParamdataInteger(PScc,valueptr);
		Signalprocessdata[signalnumber].Signalphysicalsection[PScc] = value;
	}

		delete BPFP;


	// split field 5 to extract LS (logical sections) fields - delimited by + character

		Bufferprocessing* BPFX = new Bufferprocessing();

		int LSfc = BPFX->Fileparse(FLPCNF->Paramdata(5), '+', 0);

		//std::cout<<"\n90 LSfc:"<<LSfc<<"\n";

		// extract LS reference

		for (int LScc = 0;LScc<LSfc;LScc++)
		{
			value = 0;
			BPFX->ParamdataInteger(LScc,valueptr);
			Signalprocessdata[signalnumber].SignalLS[LScc] = value;
		}

		delete BPFX;

	if (FLPCNF->ParamdataInteger(6, valueptr) == 0)
	{
		Signalprocessdata[signalnumber].AmberlookaheadSignal = value;
	}

	if (FLPCNF->ParamdataInteger(7, valueptr) == 0)
	{
		Signalprocessdata[signalnumber].Hardwarereferencenumber = value;
	}

	if (FLPCNF->ParamdataInteger(8, valueptr) == 0)
	{
		Signalprocessdata[signalnumber].Currentvalue = value;
	}

	if (FLPCNF->ParamdataInteger(9, valueptr) == 0)
	{
		Signalprocessdata[signalnumber].Displayconfigsectionreference = value;
	}

	// Process link point configuration

	// split field 10 to extract LS (logical sections) fields - delimited by + character

	Bufferprocessing* BPFW = new Bufferprocessing();

	Bufferprocessing* BPFZ = new Bufferprocessing();

	LSfc = BPFW->Fileparse(FLPCNF->Paramdata(10), '+', 0);

	for (int lppc = 0;lppc < MAXNUMBEROFSIGNALTOPOINTLINKS;lppc++)
	{

	//std::cout<<"\n131 LSfc:"<<LSfc<<"\n";

	//std::cout<<"\nParamdata["<<lppc<<"]:"<<BPFW->Paramdata(lppc);

	LSfc = BPFZ->Fileparse(BPFW->Paramdata(lppc), ':', 0);

	value = 0;

	BPFZ->ParamdataInteger(0,valueptr);
	Signalprocessdata[signalnumber].Linkedpoints[lppc] = value;

	value = 0;
	BPFZ->ParamdataInteger(1,valueptr);
	Signalprocessdata[signalnumber].Linkedpointsflag[lppc] = value;
	}

	delete BPFW;

	delete BPFZ;

	//std::cout<<"\n\nSignal processing 48 -> loading Signal number:"<<signalnumber<<" GUI ref:"<<Signalprocessdata[signalnumber].Displayconfigsectionreference;

	Numberofsignalrecordsstored++;

	delete FLPCNF;

}

void LNsignalprocessing::Manualprocess(int Signalnumber,char Signalcodechar)
{
	std::cout<<"\n31 - LNsignal update process called "<<" Num:"<<Signalnumber<<" Char:"<<Signalcodechar;	
	
	int assignedvalue = 0;		
	
	if (Signalcodechar == 'G')
	{
		assignedvalue = 4;
	}
	
	if (Signalcodechar == 'Y')
	{
		assignedvalue = 3;
	}
	
	if (Signalcodechar == 'R')
	{
		assignedvalue = 1;
	}
	
	Signalprocessdata[Signalnumber].Currentvalue = assignedvalue;
	
	//SendhardwareUpdate(Signalnumber,Signalcodechar);
}

int LNsignalprocessing::Getsigcurrentvalue(int Signalnumber)
{
	//Refreshsignalvalues(0);
	
	return Signalprocessdata[Signalnumber].Currentvalue;	
}

int LNsignalprocessing::Numberofaspects(int DisplaysectionNumber)
{
	int sc;
	
	//std::cout
	
	for (sc = 1;sc <= Numberofsignalrecordsstored;sc++)
	{
		//std::cout<<"\n59 sc:"<<sc<<" SPRef:"<<Signalprocessdata[sc].Displayconfigsectionreference;
		if (Signalprocessdata[sc].Displayconfigsectionreference == DisplaysectionNumber)
		{
			//std::cout<<"\n64 Matched - returning:"<<Signalprocessdata[sc].Currentvalue;
			return Signalprocessdata[sc].Numberofcolours;
		}
		
	}
	
	//std::cout<<"\n66 Signalprocessing - Get value for display section:"<< DisplaysectionNumber<<" not matched";
	
	return 8;
}

int LNsignalprocessing::GetPsection(int Signalnumber)
{
	return Signalprocessdata[Signalnumber].Signalphysicalsection[0];
}

int LNsignalprocessing::Displaysignalconfig(int Signalnumber, char* replymessage)
{
	char outputrec[HEAPDISPLAYBUFFERLENGTH];

	int sc,ssc,ic,isc,mcc;
	
	mcc = 0;
	
	if (Signalnumber == 0)
	{
	
		for (int nsl = 1; nsl < MAXNUMBEROFSIGNALS; nsl++)
		{
			if (Signalprocessdata[nsl].Signalphysicalsection[0] > 0)
			{
				bzero(outputrec,HEAPDISPLAYBUFFERLENGTH);
				sprintf(outputrec,"\nSignal:%03d Current Value %01d Display Loc:%03d Trainsection:%03d TrainsectionOccup:%03d PS1:%03d-%01d PS2:%03d-%01d PS3:%03d-%01d PS4:%03d-%01d LS1:%03d LS2:%03d LS3:%03d LS4:%03d LS5:%03d LS6:%03d LS7:%03d LS8:%03d Ambersig:%03d\nBlockpoint:%03d HardwareRef %03d DisplayConfref %03d Linkpt1 %03d flag:%01d Linkpt2 %03d flag:%01d Linkpt3 %03d flag:%01d Linkpt4 %03d flag:%01d ",
						nsl,Signalprocessdata[nsl].Currentvalue,Signalprocessdata[nsl].Displayconfigsectionreference,Signalprocessdata[nsl].Trainsection,Signalprocessdata[nsl].Trainsectionoccupancy,
						Signalprocessdata[nsl].Signalphysicalsection[0],Signalprocessdata[nsl].Signalphysicalsectioncurroccupancy[0],Signalprocessdata[nsl].Signalphysicalsection[1],Signalprocessdata[nsl].Signalphysicalsectioncurroccupancy[1],\
						Signalprocessdata[nsl].Signalphysicalsection[2],Signalprocessdata[nsl].Signalphysicalsectioncurroccupancy[2],Signalprocessdata[nsl].Signalphysicalsection[3],Signalprocessdata[nsl].Signalphysicalsectioncurroccupancy[3],Signalprocessdata[nsl].SignalLS[0],\
						Signalprocessdata[nsl].SignalLS[1],Signalprocessdata[nsl].SignalLS[2],\
						Signalprocessdata[nsl].SignalLS[3],Signalprocessdata[nsl].SignalLS[4],Signalprocessdata[nsl].SignalLS[5], Signalprocessdata[nsl].SignalLS[6],Signalprocessdata[nsl].SignalLS[7], Signalprocessdata[nsl].AmberlookaheadSignal,\
						Signalprocessdata[nsl].BlockingPointnumber,Signalprocessdata[nsl].Hardwarereferencenumber,Signalprocessdata[nsl].Displayconfigsectionreference, Signalprocessdata[nsl].Linkedpoints[0],Signalprocessdata[nsl].Linkedpointsflag[0],\
						Signalprocessdata[nsl].Linkedpoints[1],Signalprocessdata[nsl].Linkedpointsflag[1],Signalprocessdata[nsl].Linkedpoints[2],Signalprocessdata[nsl].Linkedpointsflag[2],\
						Signalprocessdata[nsl].Linkedpoints[3],Signalprocessdata[nsl].Linkedpointsflag[3]);
					
				isc =  std::strlen(outputrec);
				for (ic = 0;ic <isc;ic++)
				{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
				}
			}
		}
	}
	else
	{
		bzero(outputrec,HEAPDISPLAYBUFFERLENGTH);
		sprintf(outputrec,"\nSignal:%03d Current Value %01d Display Loc:%03d Trainsection:%03d TrainsectionOccup:%03d Psection:%03d LS1:%03d LS2:%03d LS3:%03d LS4:%03d Ambersig:%03d Blockpoint:%03d HardwareRef %03d DisplayConfref %03d Linkpt1 %03d flag:%01d Linkpt2 %03d flag:%01d Linkpt3 %03d flag:%01d Linkpt4 %03d flag:%01d",
		Signalnumber,Signalprocessdata[Signalnumber].Currentvalue,Signalprocessdata[Signalnumber].Displayconfigsectionreference,Signalprocessdata[Signalnumber].Trainsection,Signalprocessdata[Signalnumber].Trainsectionoccupancy,
		Signalprocessdata[Signalnumber].Signalphysicalsection[0],Signalprocessdata[Signalnumber].SignalLS[0],\
		Signalprocessdata[Signalnumber].SignalLS[1],Signalprocessdata[Signalnumber].SignalLS[2],\
		Signalprocessdata[Signalnumber].SignalLS[3],Signalprocessdata[Signalnumber].AmberlookaheadSignal,\
		Signalprocessdata[Signalnumber].BlockingPointnumber,Signalprocessdata[Signalnumber].Hardwarereferencenumber,Signalprocessdata[Signalnumber].Displayconfigsectionreference,\
		Signalprocessdata[Signalnumber].Linkedpoints[1],Signalprocessdata[Signalnumber].Linkedpointsflag[1],Signalprocessdata[Signalnumber].Linkedpoints[2],Signalprocessdata[Signalnumber].Linkedpointsflag[2],\
		Signalprocessdata[Signalnumber].Linkedpoints[3],Signalprocessdata[Signalnumber].Linkedpointsflag[3]);

		isc =  std::strlen(outputrec);
		for (ic = 0;ic <isc;ic++)
		{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
		}
	}
		
	return 0;
}

void LNsignalprocessing::Initializesignaldata()
{
	/*
	int Signalnumber;
		int Signaltype;
		int Signalphysicalsection;
		int SignalLS1;
		int SignalLS2;
		int SignalLS3;
		int SignalLS4;
		int SignalLS5;
		int SignalLS6;
		int Hardwarereferencenumber;
		int Numberofcolours;
		int Displayconfigsectionreference;	
	};
	
	struct signaldata  Signalprocessdata[MAXNUMBEROFSIGNALS];
	*/
	
	int spc;
	
	for (spc = 0; spc < MAXNUMBEROFSIGNALS; spc++)
	{
		Signalprocessdata[spc].Signaltype 				= 0;
		for (int psd = 0;psd < MAXNUMBEROFSIGNALPSECTIONS;psd++)
		{
			Signalprocessdata[spc].Signalphysicalsection[psd] = 0;
			Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psd] = 0;
		}

		Signalprocessdata[spc].GUIobjectreference		= 0;

		for (int sls = 0;sls < MAXNUMBEROFSIGNALLSSECTIONS;sls++)
		{
			Signalprocessdata[spc].SignalLS[sls] = 0;
		}
		Signalprocessdata[spc].AmberlookaheadSignal		= 0;
		Signalprocessdata[spc].BlockingPointnumber		= 0;
		Signalprocessdata[spc].Hardwarereferencenumber	= 0;
		Signalprocessdata[spc].Numberofcolours			= 0;
		Signalprocessdata[spc].Displayconfigsectionreference = 0;
		Signalprocessdata[spc].Currentvalue				= 0;
		Signalprocessdata[spc].Previousvalue			= 0;
		Signalprocessdata[spc].Trainsection				= 0;
		Signalprocessdata[spc].Trainsectionoccupancy	= 0;

		for (int slp = 0;slp < MAXNUMBEROFSIGNALTOPOINTLINKS;slp++)
		{
			Signalprocessdata[spc].Linkedpoints[slp] = 0;
			Signalprocessdata[spc].Linkedpointsflag[slp] = 0;
		}
	}
	
	Numberofsignalrecordsstored = 0;	
}

void LNsignalprocessing::UpdateTrainsectionsconfig(int Trainsection, int PS1, int PS2)
{
	// Run once signal configuration is loaded to determine Trainsection application for signal

	//std::cout<<"\n342 Trainsection:"<<Trainsection<<" Physical section1:"<<PS1<<" Physical section2:"<<PS2;

	for (int sgc = 0; sgc < MAXNUMBEROFSIGNALS;sgc++)
	{
		for (int sgcpc = 0; sgcpc < MAXNUMBEROFSIGNALPSECTIONS;sgcpc++)
		{
			if ((Signalprocessdata[sgc].Signalphysicalsection[sgcpc] == PS1) && (Signalprocessdata[sgc].Trainsection == 0))
			{
				Signalprocessdata[sgc].Trainsection = Trainsection;
				return;
			}
		}
	}

	std::cout<<"\n356 ERROR Trainsection:"<<Trainsection<<" and Physical section"<<PS1<<" NOT MATCHED to signal code";
}

int LNsignalprocessing::GetsignalnumberforGUI(int GUIobjectnumber)
{
	int Signalnumber = 0;

	int spc;

	//std::cout<<"\n257 Getsignal GUI object number:"<<GUIobjectnumber;

	for (spc = 1; spc < MAXNUMBEROFSIGNALS; spc++)
	{
		//std::cout<<"\n261 Sig:"<<spc<<" phy section:"<<Signalprocessdata[spc].Signalphysicalsection<<" configsectionreference:"<<Signalprocessdata[spc].Displayconfigsectionreference<<" ";

		if ((Signalprocessdata[spc].Signalphysicalsection[0] > 0) && (Signalprocessdata[spc].Displayconfigsectionreference == GUIobjectnumber))
		{
			Signalnumber = spc;
			//std::cout<<"\nMatched: "<<spc;
			break;
		}
	}

	return Signalnumber;
}

int LNsignalprocessing::SignalChangedCheck()
{
	int SigchangedprocRC = 0;
	
	char Newdisplayvaluechar;
	
	for (int Nsigs = 1; Nsigs < MAXNUMBEROFSIGNALS; Nsigs++)
	{
		if (Signalprocessdata[Nsigs].Signalnumber > 0)
		{
			//std::cout<<"\n257 Checking signal "<<Nsigs<<" status ";
			
			if (Signalprocessdata[Nsigs].Currentvalue != Signalprocessdata[Nsigs].Previousvalue)
			{
				Signalprocessdata[Nsigs].Previousvalue = Signalprocessdata[Nsigs].Currentvalue;
				
				if (Signalprocessdata[Nsigs].Currentvalue == 4)
				{
					Newdisplayvaluechar = 'G';
				}
	
				if (Signalprocessdata[Nsigs].Currentvalue == 3)
				{
					Newdisplayvaluechar = 'Y';
				}
				
				if (Signalprocessdata[Nsigs].Currentvalue == 1)
				{
					Newdisplayvaluechar = 'R';
				}		
			
				//std::cout<<"\n282 Signal "<<Nsigs<<" value has been changed to "<< Signalprocessdata[Nsigs].Currentvalue<<" display char:"<<Newdisplayvaluechar;
				
				SendhardwareUpdate(Nsigs,Newdisplayvaluechar);
			}			
		}		
	}
	
	return SigchangedprocRC;	
}

void LNsignalprocessing::Refreshsignalvalues(int Trainsectioncodes[], int SectionTcodes[], int Pointstatus[],int displayflag)
{
	// refreshes the current value of the signal based on the physical status of the sections
	
	//std::cout<<"\n405 Refresh signal values -> Signal point status point 1:"<<Pointstatus[1];

	char signalmessage[OUTPUTMSGBUFFERSIZE];

	bzero(signalmessage,OUTPUTMSGBUFFERSIZE);

	char* signalmessageptr = &signalmessage[0];
	
	int spc;
	
	int Amberlookaheadvalue, Amberlookaheadsignalnumber;	
	
	int LS1Tcode;
	
	int LS1Tcodevalue;
	
	int LS2Tcode;
	
	int Linkpointnumber = 0;

	int Linkpointnumberstatus = 0;

	int LS2Tcodevalue;
	
	int occupancyflag = 0;

	//for (int svc = 0; svc < MAXTRAINSECTIONS;svc++)
	//{
	//	std::cout<<"\n456 Trainsection:"<<svc<<" value passed:"<<Trainsectioncodes[svc];
	//}

	// Update Train section status for Signals

	for (int sdr = 1; sdr < MAXNUMBEROFSIGNALS; sdr++)
	{
		if (Signalprocessdata[sdr].Trainsection > 0)
		{
			Signalprocessdata[sdr].Trainsectionoccupancy = Trainsectioncodes[Signalprocessdata[sdr].Trainsection];
		}
	}


	for (spc = 1; spc < MAXNUMBEROFSIGNALS; spc++)
	{
		// check to see if signal should check section Tcodes as well as Psection status

		//std::cout<<"\n343 spc:"<<spc<<" LS1:"<<Signalprocessdata[spc].SignalLS1<<" PS:"<<Signalprocessdata[spc].Signalphysicalsection<<" PS1val:"<<PSectiondatareturn(Signalprocessdata[spc].Signalphysicalsection, signalmessageptr);
	
		// update Physical section status indicators

		//std::cout<<"\n439 Refresh signal values -> Signal point status point 1:"<<Pointstatus[1];

		//std::cout<<"\n464 Signal :"<<spc<<" TrainNumber:"<<Trainsectioncodes[spc];

		for (int psu = 0;psu < MAXNUMBEROFSIGNALPSECTIONS;psu++)
		{
			if (Signalprocessdata[spc].Signalphysicalsection[psu] > 0)
			{
				//std::cout<<"\n392 Checking signal:"<<spc<<" psu:"<<psu;
				//std::cout<<"\n393 Checking PS:"<<Signalprocessdata[spc].Signalphysicalsection[psu];
				//occupancyflag = (PSectiondatareturn(Signalprocessdata[spc].Signalphysicalsection[psu], signalmessageptr));

				//std::cout<<"\n394 Occupancy value:"<<occupancyflag;

				//std::cout<<"\n451 Refresh signal values -> Signal point status point 1:"<<Pointstatus[1];

				if (PSectiondatareturn(Signalprocessdata[spc].Signalphysicalsection[psu], signalmessageptr) > 0)
				{
					Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psu] = 1;
				}
				else
				{
					Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psu] = 0;
				}
			}
			else
			{
			  Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psu] = 0;
			}

			// check to see if signal is set by Trainsection occupancy

			if (Signalprocessdata[spc].Trainsectionoccupancy > 0)
			{
				Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psu] = 1;
			}
		}

		// Check if LS sections are set

		if ((Signalprocessdata[spc].SignalLS[0] == 0)\
		&& (Signalprocessdata[spc].SignalLS[1] == 0)\
		&& (Signalprocessdata[spc].SignalLS[2] == 0)\
		&& (Signalprocessdata[spc].SignalLS[3] == 0)\
		&& (Signalprocessdata[spc].SignalLS[4] == 0)\
		&& (Signalprocessdata[spc].SignalLS[5] == 0)\
		&& (Signalprocessdata[spc].SignalLS[6] == 0)\
		&& (Signalprocessdata[spc].SignalLS[7] == 0))
		{	
		
			// No LS set so checking Psection status only

			int PSstatusflag = 0;

			for (int psu = 0;psu < MAXNUMBEROFSIGNALPSECTIONS;psu++)
			{
				if (Signalprocessdata[spc].Signalphysicalsectioncurroccupancy[psu] > 0)
				{
					PSstatusflag = 1;
					break;
				}
			}

			// check link point status values

			if (Signalprocessdata[spc].Linkedpoints[0] > 0)
			{
				// checking link point status

			//	std::cout<<"\n496 Signal:"<<spc<<" Checking link point:"<<Signalprocessdata[spc].Linkedpoints[0]<<" Curr pointstatus:"<<Pointstatus[Signalprocessdata[spc].Linkedpoints[0]]<<" Flagval:"<<Signalprocessdata[spc].Linkedpointsflag[0];

				if (Signalprocessdata[spc].Linkedpointsflag[0] != Pointstatus[Signalprocessdata[spc].Linkedpoints[0]])
				{
			//		std::cout<<"\n500 Signal:"<<spc<<" blocked by point:"<<	Signalprocessdata[spc].Linkedpoints[0]<<" Flagval:"<<Signalprocessdata[spc].Linkedpointsflag[0]<<" Cstatus:"<<Pointstatus[Signalprocessdata[spc].Linkedpoints[0]];

					PSstatusflag = 1;
				}

			}

			if (PSstatusflag > 0)
			{
				Signalprocessdata[spc].Currentvalue = 4;
			}
			else
			{
				Signalprocessdata[spc].Currentvalue = 3;
				
				// check amber lookahead signal value
				
					Amberlookaheadsignalnumber = Signalprocessdata[spc].AmberlookaheadSignal;
				
					if (Amberlookaheadsignalnumber > 0)
					{
						Amberlookaheadvalue = Signalprocessdata[Amberlookaheadsignalnumber].Currentvalue;
					
						//printf("\n337 Doing Amber check for signal %i checking sig %i value is %i",spc,\
						Amberlookaheadsignalnumber,Amberlookaheadvalue);
						//printf("\n338 Amberlookaheadsignalnumber: %i  value %i",Amberlookaheadsignalnumber,Amberlookaheadvalue);
						if (Amberlookaheadvalue == 4)
						{
							Signalprocessdata[spc].Currentvalue = 2; // switch colour from green to amber
						//	printf("\n462 Signal %i set to amber",spc);
						//	printf("\n");
							//SendhardwareUpdate(spc,'Y');
						}
						else
						{
							//SendhardwareUpdate(spc,'G');
						}
					}
				}
			}
		else
		{
		// Check LS Tcodes as well as Psection value			
		
			LS1Tcode = Signalprocessdata[spc].SignalLS[0];
			
			LS2Tcode = Signalprocessdata[spc].SignalLS[1];
			
			LS1Tcodevalue = SectionTcodes[LS1Tcode];
			
			LS2Tcodevalue = SectionTcodes[LS2Tcode];	
			
			//printf("\n363 Checking Tocodes for Signal %i LS1 %i LS1Tcodevalue %i ", spc,LS1Tcode, LS1Tcodevalue);
			
			int Psectionvalue = PSectiondatareturn(Signalprocessdata[spc].Signalphysicalsection[0], signalmessageptr);
			
			//printf("\n367 Psection value %i ",Psectionvalue);
			
			if (PSectiondatareturn(Signalprocessdata[spc].Signalphysicalsection[0], signalmessageptr) > 0)
			{
				Signalprocessdata[spc].Currentvalue = 1;
			}
			else
			{	
				if ((LS1Tcodevalue > 0) && (LS1Tcodevalue != 999))  
				{
					Signalprocessdata[spc].Currentvalue = 4;  // section clear and valid Tcode in place
				}
				else
				{
					Signalprocessdata[spc].Currentvalue = 1;
				}				
			}
		
		}
		
		
	}  // end of checking loop	
	
	
	if (displayflag > 0)
	{
	std::cout<<"\n198 LNSignalprocessing current values \n0->"<<Signalprocessdata[0].Currentvalue\
	<<"\n1->"<<Signalprocessdata[1].Currentvalue\
	<<"\n2->"<<Signalprocessdata[2].Currentvalue\
	<<"\n3->"<<Signalprocessdata[3].Currentvalue\
	<<"\n4->"<<Signalprocessdata[4].Currentvalue\
	<<"\n5->"<<Signalprocessdata[5].Currentvalue\
	<<"\n6->"<<Signalprocessdata[6].Currentvalue\
	<<"\n7->"<<Signalprocessdata[7].Currentvalue\
	<<"\n8->"<<Signalprocessdata[8].Currentvalue\
	<<"\n9->"<<Signalprocessdata[9].Currentvalue\
	<<"\n10->"<<Signalprocessdata[10].Currentvalue\
	<<"\n11->"<<Signalprocessdata[11].Currentvalue\
	<<"\n12->"<<Signalprocessdata[12].Currentvalue;
	}
	
}

void LNsignalprocessing::SendhardwareUpdate(int Signalnumber,char Signalcodechar)
{
	//std::cout<<"\nLNsignal hardware refresh process called ";

	sprintf(Signalnumberchar,"%02d",Signalnumber);

	unsigned char *SigCmdtextoutputptr = new unsigned char [8];

	*SigCmdtextoutputptr = 'S';
	*(SigCmdtextoutputptr + 1) = Signalnumberchar[0];
	*(SigCmdtextoutputptr + 2) = Signalnumberchar[1];
	*(SigCmdtextoutputptr + 3) = Signalcodechar;
	*(SigCmdtextoutputptr + 4) = 'X';
	*(SigCmdtextoutputptr + 5) = '0';
	*(SigCmdtextoutputptr + 6) = '0';
	*(SigCmdtextoutputptr + 7) = '\0';

	Signalcommandsend(7,SigCmdtextoutputptr);
	
}
