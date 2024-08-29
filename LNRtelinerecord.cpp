/*
 * LNRtelinerecord.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: BrianRoot
 */

#include <iostream>
#include <cstring>

#include "LNRtelinerecord.h"

#include "LNwebconfig.h"

/*
   Route device commands
   
   0  =  Null - no action 
   1  = MOVE - move to next section
   2  = STPD(x) - stop at device x
   3  = SLS(y) - set loco speed to be y 
   4  = STPC   - stop if previous section is clear 
   5  = STOP 
   6  = RLD(z) - reset logical device z
   7  = RLA    - reset all logical devices
   8  = ELT    - Enable logical device trigger 
   9  = RLT    - reset  logical device trigger 
  10  = DLD    - disable device scanner
  11  = ETCS   - enable train car scanning 
  12  = DTCS   - disable train car scanning 
  13  = SDC(a,b) - set device a counter to b 
  14  = RDC(a)   - reset device a counter to zero  
  15  = STPB     - stop at device or previous section clear 
  16  = STPM     - stop at device or move to next section
  17  = RRSC(x,y)   - reset set route step counter to step x, substep y  
  18  = ILCC   		- increment the actual loop counter 
  19  = BLCE(x,y)   - branch if loop counter actual = target   
  20  = SLCT(x)   - set loop counter target value  
  21  = BLFS(x,y)   - branch if loop stop flag is set    
  22  = STCC(d,v)   - stop if count on device d = value v
  24  = EUD(a,b,c) - Enable uncoupler a with device b on target c
  25  = STDC(x) - Stop when device triggered command and couple the train
  26  = BLCN(x,y)   - branch if loop counter actual <> target   
*/ 

LNRtelinerecord::LNRtelinerecord() {
	// TODO Auto-generated constructor stub


}

LNRtelinerecord::~LNRtelinerecord() {
	// TODO Auto-generated destructor stub
}

std::vector<int> LNRtelinerecord::Rtelinedata()
{
	std::vector<int> linedetails;

	linedetails.push_back(Routenumber);
	linedetails.push_back(Routestep);
	linedetails.push_back(Routesubstep);
	linedetails.push_back(RoutePS);
	linedetails.push_back(RouteCS);
	linedetails.push_back(RouteNS);
	linedetails.push_back(RouteCmd);
	linedetails.push_back(RouteCmdparam1);
	linedetails.push_back(RouteCmdparam2);
	linedetails.push_back(RouteCmdparam3);
	linedetails.push_back(RouteCmdparam4);

	return linedetails;
}

void LNRtelinerecord::Buildstep(int* Storeflag,int* Rtedirection, LNRteLogicalsectionGroup* LNRteLSG, int Route, int Stepnum, int Substep, std::vector<int> RteLSsteps, \
		int Stoptypeflag, int Stopflagdevicenumber, int StopdeviceLS, int Stopflagdevicetarget,int Uncouplernum, int Uncouplerdevice,\
		int UncouplerdeviceLS, int Uncouplerdevicetarget, int Traincouplerflag, int Sectionswitchoverflag, int SectionswitchoverfromLS,\
		int SectionswitchovertoLS,\
		int counterbranchcheckstep, int counterincrementstep,int PlannedTypeofloop, int Lcounter,\    
		int StartloopLS,int EndloopLS,int Branchsteptarget, int Branchsubsteptarget)
{
	std::vector<int >::iterator iSTEPLSNUM;

	// create logical section object on the stack

	LNRteLogicalsectionData LNRtelineLSDCS;

	LNRteLogicalsectionData LNRtelineLSDNS;

	int Numberofsteps = RteLSsteps.size();
	/*
	if (Substep > 0)
	{
	std::cout<<"\n\nLNRtelinerecord line 90 Stepnum:"<<Stepnum<<" Substep:"<<Substep;
	std::cout<<"\nLNRtelinerecord line 92 Counterbranchcheckstep:"<<counterbranchcheckstep<<"\n counterincrementstep:"<<counterincrementstep<<\
	"\n PlannedTypeofloop:"<<PlannedTypeofloop<<"\n Lcounter:"<<Lcounter<<"\n StartloopLS:"<<StartloopLS<<"\n EndloopLS:"<<EndloopLS<<\
	"\n Branchsteptarget:"<<Branchsteptarget<<"\nBranchsubsteptarget:"<<Branchsubsteptarget;
	}	
	*/
	//std::cout<<"\nLNRtelinerecord line 58 - number of steps in RteLSsteps vector :"<<Numberofsteps;


	//printf("\n60 Buildstep Stepnum %i, Substep %i  Numberofsteps %i, Stopflag %i Stopflagdevice %i StopdeviceLS %i",\
			Stepnum,Substep,Numberofsteps,Stoptypeflag, Stopflagdevicenumber,StopdeviceLS);

	// extract the current section

	// sett iterator to the start of the vector listing all the sections

	iSTEPLSNUM = RteLSsteps.begin();

	// add on the stepnumber offset from zero to get the current section


	int LNRteCS = *(iSTEPLSNUM + (Stepnum - 1));			//fetch current section
	int LNRteNS = *(iSTEPLSNUM + (Stepnum - 1) + 1);		// fetch next section

	int LNRtePS = 0;

	if ((Stepnum == Numberofsteps) && (PlannedTypeofloop != 2))
	{
		LNRteNS = 0;
	}
	
	if ((Stepnum == Numberofsteps) && (PlannedTypeofloop == 2))
	{
		LNRteNS = *(iSTEPLSNUM + (1 - 1) + 1);  // special processing for type 2 loop 
	}
	
	if ((Stepnum == Numberofsteps) && (PlannedTypeofloop == 1))
	{
		LNRteNS = *(iSTEPLSNUM + (1 - 1) + 1);  // special processing for type 1 loop 
	}
	
	

	if (Stepnum == 1)
	{
		LNRtePS = 0;										// first step so set PS to zero
	}
	else
	{
		LNRtePS = *(iSTEPLSNUM + (Stepnum - 1) - 1);		//fetch previous section

	}
	LNRtelineLSDCS = LNRteLSG->GetLSreference(LNRteCS);		// Extract the logical section object for the current section

	int LNRtSDCSpsection = LNRtelineLSDCS.GetLSpsection();	// Extract the physical section for the current logical section

	int LNRtSDNSpsection = LNRtelineLSDNS.GetLSpsection();	// Extract the physical section for the next logical section

	int RteLSdirection = LNRtelineLSDNS.GetLSdirection();


	// build out step details

	if (Substep == 1) // test for MOVE record
	{
		Routenumber 	= Route;
		Routestep 		= Stepnum;
		Routesubstep 	= Substep;
		RoutePS 		= LNRtePS;
		RouteCS			= LNRteCS;		// store current section
		RouteNS			= LNRteNS;		// store section
		RouteCmd		= 1;			// setup MOVE command
		RouteCmdparam1 	= 0;
		RouteCmdparam2	= 0;
		RouteCmdparam3 	= 0;
		RouteCmdparam4	= 0;

		*Rtedirection = RteLSdirection; // passback direction of travel from first LS

		*Storeflag = 1;		// set the store flag back to indicate it is a MOVE record

		if ((Stepnum == 1) && (Traincouplerflag > 0))
		{
			RouteCmdparam1 	= 5;	// set route param to indicate that the loco should be coupled to the Train when it stops at the end of the route.
		}

		if (Sectionswitchoverflag > 0) // check to see if there is an Logical section switchover flag for the change to be executed when the loco couples to it
		{
			RouteCmdparam2	= 1;
			RouteCmdparam3	= SectionswitchoverfromLS;
			RouteCmdparam4  = SectionswitchovertoLS;
		}
	}

	if ((Stepnum == Numberofsteps) && (Substep == 1)  && (PlannedTypeofloop != 2))	 
	{
		
		//std::cout<<"\nLNRtelinerecord 179 - PlannedTypeofloop:"<<PlannedTypeofloop;
		// step to build and it is not a Type 2 loop.
		// decode the stop code

		/* Stop code mapping

		 Stoptypeflag    text       RteCMD

		 	 1			STPC			4
		 	 2			STPD			2
		 	 3			STPB		   15
		 	 4			STPM		   16
		 	 5			STCC		   22
		 	 6			STPX		   23
			 7          STDC           25
		*/

		if (Stoptypeflag == 1)  // STPC
		{
			RouteCmd = 4;
		}

		if (Stoptypeflag == 2)  // STPD
		{
			RouteCmd = 2;
		}

		if (Stoptypeflag == 3)  // STPB
		{
			RouteCmd = 15;
		}

		if (Stoptypeflag == 4)  // STPM
		{
			RouteCmd = 16;
		}

		if (Stoptypeflag == 5)  // STCC
		{
			RouteCmd = 22;
		}

		if (Stoptypeflag == 6)  // STPX
		{
			RouteCmd = 23;
		}

		if (Stoptypeflag == 7)  // STDC
		{
			RouteCmd = 25;
		}

		if (Stoptypeflag == 8)  // STOP
		{
			RouteCmd = 5;
		}

		if (Stoptypeflag > 0)
		{
	//	std::cout<<"\n231 Rteline  - Building Stop command - Cmdcode:"<<RouteCmd<<" Stopflagdevicenumber:"<<Stopflagdevicenumber<<" Stopflagdevicetarget"<<Stopflagdevicetarget;

		RouteCmdparam1 	= Stopflagdevicenumber;
		RouteCmdparam2	= Stopflagdevicetarget;
		Routenumber 	= Route;
		Routestep 		= Stepnum;
		Routesubstep 	= Substep;
		RoutePS 		= LNRtePS;
		RouteCS			= LNRteCS;		// store current section
		RouteNS			= 0;		

		*Storeflag = 4;		// set the store flag back to indicate it is a STOP record
		}
	}
	
	if ((Stepnum == Numberofsteps) && (Substep == 2)  && (PlannedTypeofloop == 2))
	{
		if (Stoptypeflag == 1)  // STPC
		{
			RouteCmd = 4;
		}

		if (Stoptypeflag == 2)  // STPD
		{
			RouteCmd = 2;
		}

		if (Stoptypeflag == 3)  // STPB
		{
			RouteCmd = 15;
		}

		if (Stoptypeflag == 4)  // STPM
		{
			RouteCmd = 16;
		}

		if (Stoptypeflag == 5)  // STCC
		{
			RouteCmd = 22;
		}

		if (Stoptypeflag == 6)  // STCC
		{
			RouteCmd = 23;
		}

		if (Stoptypeflag == 7)  // STDC
		{
			RouteCmd = 25;
		}

		if (Stoptypeflag == 8)  // STOP
		{
			RouteCmd = 5;
		}

		if (Stoptypeflag > 0)
		{
	//	std::cout<<"\n231 Rteline  - Building Stop command - Cmdcode:"<<RouteCmd<<" Stopflagdevicenumber:"<<Stopflagdevicenumber<<" Stopflagdevicetarget"<<Stopflagdevicetarget;

		RouteCmdparam1 	= Stopflagdevicenumber;
		RouteCmdparam2	= Stopflagdevicetarget;
		RouteCmdparam1 	= Stopflagdevicenumber;
		RouteCmdparam2	= Stopflagdevicetarget;
		Routenumber 	= Route;
		Routestep 		= Stepnum;
		Routesubstep 	= 2;
		RoutePS 		= LNRtePS;
		RouteCS			= LNRteCS;		// store current section
		RouteNS			= 0;		

		*Storeflag = 4;		// set the store flag back to indicate it is a STOP record
		}		
	}

	if (Substep == 2)
	{
		// processing for speed records

		if((Stepnum == Numberofsteps)) // checking to see if at last step on route
		{
			if ((Stoptypeflag == 8) || (Stoptypeflag == 4))  // test for immediate stop commands
			{
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= 0;
			RouteCmd		= 3;		// setup SLS  command
			RouteCmdparam1 	= 0;    	// set speed to zero
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 2;		// set the store flag back to indicate it is an SLS record
			}
			else			// process for non-immediate stop commands
			{
				Routenumber 	= Route;
				Routestep 		= Stepnum;
				Routesubstep 	= Substep;
				RoutePS 		= LNRtePS;
				RouteCS			= LNRteCS;
				RouteNS			= LNRteNS;
				RouteCmd		= 3;		// setup SLS  command
				RouteCmdparam1 	= LNRtelineLSDCS.GetLSdataspeed();	// extract the speed setting for the section
				RouteCmdparam2	= 0;
				RouteCmdparam3 	= 0;
				RouteCmdparam4	= 0;

				*Storeflag = 2;		// set the store flag back to indicate it is an SLS record
			}
		}
		else			// not at last step so write  a regular speed record
		{
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 3;		// setup SLS  command
			RouteCmdparam1 	= LNRtelineLSDCS.GetLSdataspeed();	// extract the speed setting for the section
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 2;		// set the store flag back to indicate it is an SLS record
		}
	}

	if ((Stepnum == Numberofsteps) && (Substep == 3)  && (PlannedTypeofloop == 2))
	{
		Routenumber 	= Route;
		Routestep 		= Stepnum;
		Routesubstep 	= 3; //Substep;
		RoutePS 		= LNRtePS;
		RouteCS			= LNRteCS;
		RouteNS			= LNRteNS;
		RouteCmd		= 3;		// setup SLS  command
		RouteCmdparam1 	= LNRtelineLSDCS.GetLSdataspeed();	// extract the speed setting for the section
		RouteCmdparam2	= 0;
		RouteCmdparam3 	= 0;
		RouteCmdparam4	= 0;

		*Storeflag = 4;		// set the store flag back to indicate it is a special SLS record		
	}
	
	if ((Substep == 3) && (Stepnum == 1) && (PlannedTypeofloop > 1) && (Lcounter > 0))
	{
		// setup route counter target for a loop that has a set number of loops
		printf("\n255 Initialize loop counter for Route %i target to be %i",Route,Lcounter);
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 20;		// Command SLCT - initialize the route loop counter 
			RouteCmdparam1 	= Lcounter; 	
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
	}
	
	if ((Substep == 3) && (PlannedTypeofloop == 1) && (Stepnum == (Numberofsteps-1)))	
	{
		// this is a specific type of loop (type 1) where the end LS is the same as the start LS
		// Reset the step counter to one when the second to last step is read 
		//std::cout<<"\nLNRtelinerecord line 275  Reset the step counter to 1 as this is a type 1 loop (StartLS = FinishLS)";
		//std::cout<<"\nRoute:"<<Route<<" Step:"<<Stepnum<<" Substep:"<<Substep;
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 17;		// Reset step counter to 1 
			RouteCmdparam1 	= 0;	 	// Reset substep to 1
			RouteCmdparam2	= 1;;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
		
		
	}
	
	if ((Substep == 3) && (PlannedTypeofloop == 3) && (Stepnum == counterincrementstep))	
	{
		// checking for Type 3 loop (loop occurs within the LS steps between start and end but are not
		// the start and end LS - Counter increment
		
		//std::cout<<"\nLNRtelinerecord line 298  Increment the loop counter as this is a type 3 loop ";
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 18;		// Reset step counter to 1 
			RouteCmdparam1 	= 0;	 	// Reset substep to 1
			RouteCmdparam2	= 0;;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
		
		
	}
	
	if ((Substep == 1) && (PlannedTypeofloop == 2) && (Stepnum == counterbranchcheckstep))	
	{
		// checking for Type 3 loop (loop occurs within the LS steps between start and end and are 
		// the start and end LS
		
		//std::cout<<"\nLNRtelinerecord line 329  found line for Test for branch if the act loop count has reached the target";
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 26;					// test for branch if not equal
			RouteCmdparam1 	= Branchsteptarget;	 	// destination step if branched
			RouteCmdparam2	= 1; //Branchsubsteptarget;	// destination substep if branched
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
				
	}
	
	if ((Substep == 3) && (PlannedTypeofloop == 3) && (Stepnum == counterbranchcheckstep))	
	{
		// checking for Type 3 loop (loop occurs within the LS steps between start and end but are not
		// the start and end LS
		
		//std::cout<<"\nLNRtelinerecord line 321 Test for branch if the act loop count has NOT reached the target";
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 26;					// test for branch if not equal
			RouteCmdparam1 	= Branchsteptarget;	 	// destination step if branched
			RouteCmdparam2	= Branchsubsteptarget;	// destination substep if branched
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
				
	}
	
	

	if ((Substep == 3) && (Stopflagdevicenumber > 0))// test for supplementary records
	{
		if ((StopdeviceLS > 0) && (StopdeviceLS == LNRteNS))
		{
			printf("\nLNRtelinerecord 297 Create RLD record for STOP device %i in NS %i",Stopflagdevicenumber, StopdeviceLS);
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 6;		// setup RLD - reset logical device record
			RouteCmdparam1 	= Stopflagdevicenumber; 	// create detector device reset record
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record
		}
	}
	
	if ((Substep == 3) && (counterincrementstep == Stepnum) && (Lcounter > 0)) // test for counter increment record to create a counter \
	increment command
	{
		
			printf("\nLNRtelinerecord 228 Create Counter increment record at substep 3 in step:%i",Stepnum);
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 18;		// setup Counter increment step
			RouteCmdparam1 	= 0;       
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record		
	}
	

	if ((Substep == 3) && (Uncouplerdevice > 0))// test for supplementary record to create a Reset Logical device record
	{
		if ((UncouplerdeviceLS > 0) && (UncouplerdeviceLS == LNRteNS))
		{
			//printf("\n228 Create RLD record for device %i in NS for Uncoupler %i",Stopflagdevicenumber, StopdeviceLS);
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 6;		// setup RLD - reset logical device record
			RouteCmdparam1 	= Uncouplerdevice; 	// create detector device reset record
			RouteCmdparam2	= 0;
			RouteCmdparam3 	= 0;
			RouteCmdparam4	= 0;

			*Storeflag = 3;		// set the store flag back to indicate it is an RLD record
		}
	}

	if ((Substep == 4) && (Uncouplerdevice > 0))
		{
		//std::cout<<"\n216 found Uncoupler trigger device:"<<Uncouplerdevice<<" UncouplerdeviceLS:"<<UncouplerdeviceLS;

			if ((UncouplerdeviceLS > 0)&& (UncouplerdeviceLS == LNRteNS))
			{
			//printf("\nCreate Uncoupler trigger setup EUD record for Uncoupler %i in NS %i",Stopflagdevicenumber, StopdeviceLS);
			Routenumber 	= Route;
			Routestep 		= Stepnum;
			Routesubstep 	= Substep;
			RoutePS 		= LNRtePS;
			RouteCS			= LNRteCS;
			RouteNS			= LNRteNS;
			RouteCmd		= 24;				// setup EUD record - Uncoupler (param1) trigger by device (param2) when counter equals param3
			RouteCmdparam1 	= Uncouplernum;
			RouteCmdparam2 	= Uncouplerdevice;	// create uncoupler trigger record
			RouteCmdparam3	= Uncouplerdevicetarget;
			RouteCmdparam4	= 0;

			*Storeflag = 5;		// set the store flag back to indicate it is an RLD record
			}
		}

	//std::cout<<"\n257 End of Rteline processing \n";
}

int LNRtelinerecord::Displaystep(LNRteCMDlookuptable* CMDtable, char *replymessage)
{
		// display step details

		char outputrec[OUTPUTRECSIZE * 4];

		char Stext[DISPSHORTDESCLENGTH];

		char Ltext[DISPLONGDESCLENGTH];

		bzero(Stext,DISPSHORTDESCLENGTH);

		bzero(Ltext,DISPLONGDESCLENGTH);

		char* Stextptr = &Stext[0];

		char* Ltextptr = &Ltext[0];

		int isc,ic,scc,mcc;

		mcc = 0;

		int DC;

		CMDtable->Getcmdtext(RouteCmd,'S',Stextptr);

		CMDtable->Getcmdtext(RouteCmd,'L',Ltextptr);

		//printf("\nRteCMD %i S %s L %s", RouteCmd, Stextptr, Ltextptr);

		bzero(outputrec,OUTPUTRECSIZE * 4);

		sprintf(outputrec,"\nRte Num: %03d Step: %02d Substep: %02d PS: %03d CS: %03d NS: %03d Cmd:%02d:%s: Params: %02d %02d %02d %02d Command:%s", \
				Routenumber, Routestep, Routesubstep, RoutePS, RouteCS, RouteNS, RouteCmd, Stext, RouteCmdparam1, RouteCmdparam2, \
				RouteCmdparam3, RouteCmdparam4, Ltext);

				isc =  std::strlen(outputrec);

					for (ic = 0;ic <isc;ic++)
					{
					*(replymessage + mcc) = outputrec[ic];
					mcc++;
					}

		return isc;
}

