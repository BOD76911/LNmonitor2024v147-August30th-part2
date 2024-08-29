/*
 * LNweblocorouteprocess.h

 *
 *  Created on: Jul 25, 2019
 *      Author: BrianRoot
 */

#ifndef LNWEBLOCOROUTEPROCESS_H_
#define LNWEBLOCOROUTEPROCESS_H_

#include "LNwebconfig.h"
#include "LNwebLsectionsLdevices.h"
#include "LNallTrains.h"
#include "LNlocoAlldata.h"
#include "LNlogging.h"

/*
   Route device commands
   
   0  = Null - no action 
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
  20  = SLCT   - set loop counter target value  
  21  = BLFS(x,y)   - branch if loop stop flag is set    
  22  = STCC(d,v)   - stop if count on device d = value v
  24  = EUD(a,b,c) - Enable uncoupler a with device b on target c
  25  = STDC(x) - Stop when device triggered command and couple the train
  26  = BLCN(x,y)   - branch if loop counter actual <> target   
*/ 

//LNlocospdtbl* LRSPDtableparam;
//LNwebLsectionsLdevices* LSLD;

class LNweblocorouteprocess
{
public:
	LNweblocorouteprocess(LNwebLsectionsLdevices* LRLSLDparam, LNlocoAlldata* LNlocodatatables, LNallTrains* LNallTrainsinformationparam);

	virtual ~LNweblocorouteprocess();

	void LocodatarecordsInit(int loco);

	int Locodatarecordsdisplay(int loco, char* replymessage);
	
	int Locodatarecordsdisplayonconsole(int loco, char* replymessage);

	void DisplayLocoInformation(int location , int loco);

	void RoutedatarecordsInit(int Rte);

	void GetLRprocessmonmsg(char* replymessage);

	void GetLRlocoprocessmonmsg(int loco, char* replymessage);
	
	int GetJourneystatus(char* replymessage);

	void LNPRoverallmessage(char* message);
	
	int GetLoopcountertarget(int Locoindx);
	
	int GetLocoplanspeed(int Locoindx);

	int GetDCCLocospeed(int Locoindx);

	int GetLocoEnabled(int Locoindx);

	int GetLoopcounteractual(int Locoindx);
	
	void SetLoopcountertarget(int Locoindx, int value);
	
	void SetLoopcounteractual(int Locoindx, int value);
	
	void SetLoopstopflag(int Locoindx);
	
	void ResetLoopstopflag(int Locoindx);

	int Journeycompletedprocessing(int loco, int call_location);

	void LNprmessage(int loco, char* message);

	int GetSimulatornumber(int Locoindx);

	int Routedatarecordsdisplay(int Rte, int Step, int Substep, char* replymessage);

	int CheckIfrouteloaded(int route);

	void SaveLocojourneydata(int Locoindx, int Rtenum,int Stepnum, \
			int Substepnum,int PS, int CS,int NS, int speed, \
			int direction, int Signalhold,int Enabled, \
			int Planspeed,int Cmdinprogress,int Journey,\
			int Journeystatus, int Stopdevice, int Traincode, int Slotcode, int Simcode,int Tnumber,int Reverselocodirection, int Sectionlogicflag);

	void SetEnableLocoflag(int Locoindx, int flag);

	void SaveRouteline(int Rte, int Rtestep, int Rtesubstep,\
			int RtePS,int RteCS, int RteNS, int Rtecmd, \
			int Rtecmdparam1,int Rtecmdparam2, int Rtecmdparam3,\
			int Rtecmdparam4,char *cmd, int dispflag);

	void ClearRouteLS(int loco, int StopLS);

	void ClearRoute_AllLS(int loco, int StopLS);
/************************ Section crossing processing ********************************************/

int LNsectionCrossingprocessing(int loco,int action);

void LNsectionCrossingprocessingNG_Datarefresh(int loco);

int LNsectionCrossingprocessingNG_Newjourney(int loco);

int LNsectionCrossingprocessingNG_Crossoverprocess(int loco);

int LNsectionCrossingprocessingNG_CheckPSclearance();

int LNsectionCrossingprocessingNG_CheckSTOP_BC_flags(int loco);

int LNsectionCrossingprocessingNG_Check_NS_STOP_flags(int loco);

void LNsectionCrossingprocessing_displaySTOPflags(int loco);

int FollowOnSubsteprocessing(int loco);

int StopFlagprocessing();

int LNlocosectionrefresh();

void LNSTPDprocessing();

int Lococommandexecute(int loco, int cmd, char* message);

void Enabletrainscancmd(int Locoindx, int Traincode, int Lococode);

void Disabletrainscancmd(int Locoindx, int Traincode, int Lococode);

void Passloggingpointer(LNlogging* LNlogging);

void Setschedulerrunstatus(int loco, int value);

int GetJourneystatusvalue(int Loco);

int Getschedulerrunstatus(int loco);

private:

LNlogging*                      LNloggingptr;

LNwebLsectionsLdevices* 		LSLD;

LNlocoAlldata* 					LCSPDtable;

LNallTrains*                    LNallTrainsinformation;

#define 	Routecount			50
#define     Routelinesmax		50
#define 	Routeflagscnt		16
#define 	Routemaxsubsteps	 8
#define 	Routelinefields     	15

#define 	Lococountmax		20 // 15
#define 	Locodescriptionlen	20

#define     COMMMANDCHARLENGTH  15

int Rte, P1y, P2y, typey,Tcode_NG, Plannedspeed,Rtestep, Rtesubstep, Stepcmd, Stepcmdp1, Stepcmdp2,Direction,ConnectedTraincode, Trainsection, Trainsectionstatus;

int Py1oc, Py2oc, PSoc, CSoc, NSoc, LStype;

char lprmessage[LRMESSAGELENGTH];

int FollowonprocessingRC;

int         TrainsectionactualPScount;

int 		Crossing_inprogress_flag_NG;

char 		PSmessage_NG[SHORTMESSAGELENGTH];

char 		lprmessage_NG[LRMESSAGELENGTH];

int 		NSS_LS_NG;

int 		PS_LS_NG;
int         PS_LS_type_NG;
int 		CS_LS_NG;
int         CS_LS_type_NG;
int 		NS_LS_NG;
int         NS_LS_type_NG;

int 		PS_ps1_val_NG;
int 		PS_ps1_data_NG;

int 		PS_ps2_val_NG;
int  		PS_ps2_data_NG;

int 		CS_ps1_val_NG;
int 		CS_ps1_data_NG;

int 		CS_ps2_val_NG;
int 		CS_ps2_data_NG;

int 		NS_ps1_val_NG;
int 		NS_ps1_data_NG;

int 		NS_ps2_val_NG;
int 		NS_ps2_data_NG;

int         PS_Tcode_NG;
int 		CS_Tcode_NG;
int         NS_Tcode_NG;

int         PSTrainsection_PSstatus[MAXPSinTS];
int 		CSTrainsection_PSstatus[MAXPSinTS];
int 		NSTrainsection_PSstatus[MAXPSinTS];

int 		PSTrainsectionprevious;

/*
int 		PS_ps1_val;
int 		PS_ps1_data;

int 		PS_ps2_val;
int  		PS_ps2_data;

int 		CS_ps1_val;
int 		CS_ps1_data;

int 		CS_ps2_val;
int 		CS_ps2_data;

int 		NS_ps1_val;
int 		NS_ps1_data;

int 		NS_ps2_val;
int 		NS_ps2_data;

*/

unsigned char LRprocessmonitormessage[LRMESSAGELENGTH];

struct Locodatarecord
		{
		unsigned int Locoindex;
		unsigned int LocoDCCcode;
		unsigned int Route;
		unsigned int Rtestep;
		unsigned int Rtesubstep;
		unsigned int Enabled;
		unsigned int Journey;
		unsigned int Journeystatus;
		unsigned int Schedulerunstatus;
		unsigned int Signalhold;
		unsigned int Systemhold;
		unsigned int Slotnumber;
		unsigned int Locodirectionreversed;
		unsigned int PS;
		unsigned int CS;
		unsigned int NS;

		unsigned int PS_previous;
		unsigned int CS_previous;
		unsigned int NS_previous;

		unsigned int PSTrainsection;
		unsigned int PSTrainsection_previous;
		unsigned int PSsectionPS[MAXPSinTS];
		unsigned int PSsectionPSstatus[MAXPSinTS];

		unsigned int CSTrainsection;
		unsigned int CSTrainsection_previous;
		unsigned int CSsectionPS[MAXPSinTS];
		unsigned int CSsectionPSstatus[MAXPSinTS];

		unsigned int NSTrainsection;
		unsigned int NSTrainsection_previous;
		unsigned int NSsectionPS[MAXPSinTS];
		unsigned int NSsectionPSstatus[MAXPSinTS];

		unsigned int NSTrainsectionoccupancystatus;

		unsigned int Trainsectioncrossingtype;
		unsigned int Direction;
		unsigned int Speed;
		unsigned int Planspeed;
		unsigned int ActDCCvalue;
		unsigned int Cmdinprogress;
		unsigned int Loopcountertarget;
		unsigned int Loopcounteractual;
		unsigned int Loopstopflag;
		unsigned int Cmdparam1;
		unsigned int Cmdparam2;
		unsigned int Cmdparam3;
		unsigned int Cmdparam4;
		unsigned int Locomissingflag;
		unsigned int Locomissingcounter;
		unsigned int Stopdetector;
		unsigned int LastknownLS;
		unsigned int Traincode;		
		unsigned int ConnectedTraincode;
		unsigned int STPCflag;
		unsigned int STPDflag;
		unsigned int STPBflag;
		unsigned int STPMflag;
		unsigned int STCCflag;
		unsigned int STPXflag;
		unsigned int STDCflag;
		unsigned int STOPflag;
		unsigned int NextsectionSTOPflag;
		unsigned int TrainLSswitchoverflag;
		unsigned int TrainswitchoverfromLS;
		unsigned int TrainswitchovertoLS;
		unsigned int TrainNumber;
		unsigned int Simulatornumber;
		unsigned int Sectionlogicflag;
		unsigned int Rteflagsdvce[Routeflagscnt];
		unsigned int Rteflagsdvcecntplan[Routeflagscnt];
		unsigned char description[Locodescriptionlen];
		unsigned int filler1;
		unsigned int Routesectionhistory[Routelinesmax];
		unsigned char LRlocoprocmessage[LRMESSAGELENGTH];
	};

struct Locodatarecord Locoinformation[Lococountmax];

struct Routerecord
{
	unsigned int Route;
	unsigned int Routestep;
	unsigned int Routesubstep;
	unsigned int RoutePS;
	unsigned int RouteCS;
	unsigned int RouteNS;
	unsigned int RouteCmd;
	unsigned int RouteCmdparam1;
	unsigned int RouteCmdparam2;
	unsigned int RouteCmdparam3;
	unsigned int RouteCmdparam4;
	unsigned char Rtecommand[COMMMANDCHARLENGTH];
};

struct Routerecord RouteData[Routecount][Routelinesmax][Routemaxsubsteps];

};

#endif /* LNWEBLOCOROUTEPROCESS_H_ */
