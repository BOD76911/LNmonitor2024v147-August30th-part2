/*
 * LNwebLsectionsLdevices.h
 *
 *  Created on: Jul 3, 2019
 *      Author: BrianRoot
 */
/**************************************************************************************
 *
 *  Class for managing logical sections,logical detector devices, Points, Crossovers
 *  and uncoupling ramps
 *
 *  Updated July 2021 - to handle route processing calls
 *
 *************************************************************************************/

#ifndef LNWEBLSECTIONSLDEVICES_H_
#define LNWEBLSECTIONSLDEVICES_H_

#include "LNwebconfig.h"
#include "LNallTrains.h"
#include "LNsignalprocessing.h"
#include "Bufferprocessing.h"
#include "LNoperationlogging.h"
#include <chrono>
#include <thread>

#include <vector>


class LNwebLsectionsLdevices {
public:
	LNwebLsectionsLdevices();

	virtual ~LNwebLsectionsLdevices();

	int LNLogicalsectionmapfileload();

	void InitOploggingptr(LNoperationlogging* LNOplatestlogptr);

	void LoadLSlogdata(char* LSlogdataptr);

	void InitLogicalsectionmap();

	int LNlogicalsectionconfigload(char* lineinputbufferptr, int displayflag);

	int LNlogicaldeviceconfigload(char* lineinputbufferptr, int displayflag);

	int GetNumberofLSloaded();

	int GetMaxsectionnumberloaded();

	int GetNumberofDetectorsloaded();

	int GetNumberofUncouplersloaded();

	int DisplayLsections(int sc, char* replymessage);

	std::vector<int> GetLSdataforRte(int section);  // create integer vector with Logical section data details

	std::vector<int> GetPStoLSmapping(int Physection);

	std::vector<int> GetDetectordata(int detector);

	std::vector<int> GetUncouplerdata(int uncoupler);

	int CreateLSLDextract(char* replymessage);

	int DisplayLdevices(int sc, char* replymessage);

	int GetLSTcode(int sc);

	int GetLDstatus(int ld);

	int GetLS_PS1section(int ls);

	int GetPSforGUIref(int GUIref);

	int GetLS_PS2section(int ls);

	int GetLDevicestatus(int dc);
	
	int GetLDevicecountervalue(int dc);

	void UpdateTcode(int sc, int Tcode);

	void UpdateTrainID(int sc, int TrID, int Trainnumber, int actionflag);

	//int GetTrainID(int sc);

	void UpdateDcode(int sc, int Dcode);

	void ClearTcodes();

	void ClearDcodes();

	// detector updates

	void Logicaldetectorarraytablesinitialize();

	void UpdateLdevicestatus(int ld, int status);

	void UpdateIRdetectors(int* sensorstateptr);

	void UpdateRFid_detectors(int RFid_device, char* RFidcode);

	void RFidinit(int devicenum, int trigstatus);

	int DisplayTcodes(int dflg,char* replymessage);

	int DisplayDcodes(int dflg,char* replymessage);

	int DisplayDcodesNG(int dflg,char* replymessage);

	int DisplayLcodes(int dflg,char* replymessage);

	int DisplayPhysicaltoLSmapping(char* replymessage);

	int DisplayPhysicaltoLSmappingDetail(int Psection, char* replymessage);

	int GetmaxPSnumber();

	int TrainIDLSswitchover(int FromLS, int ToLS);

	char Datadisplaybuffer[DISPLAYBUFFERLENGTH];

	int LogicaltoPhysicalstatuscheck(int LScode, int LTcode, int testtype);

	int GetLSlistforPsection(int* LSlistptr ,int Psection);

	/***************************** point processing methods ***************************************/

	void InitPointDCCcodes();

	void InitPointstatus();

	void InitcomplexPointconfig();

	void DisplaycomplexPointconfig();

	void Complexpointprocessing();

	void Loadcomplexpointlogicalconfig(char* lineinputbuffer,int displayflag);

	void LoadpointDCCconfig(char* lineinputbuffer,int displayflag);

	void Loadpointlogicalconfig(char* lineinputbufferptr, int displayflag);

	void StoreDCCpointcodes(int pt,int Dir, unsigned char[4]);

	void GetPointDCCcodes(int pt,int OC, char* replymessage);

	void Actionswitch(int ptc, int calledby,int OC);

	void Displaysinglepointconfig(int point);

	void Displaycrossoverconfig();

	void DisplayUncouplerdetails(char* replymessage);

	int GetUncouplerstatus(int uncouplernumber);

	void SetUncouplerstatus(int uncouplernumber, int statusvalue);

	int DisplayTrainIDs(int sc, char* replymessage);

	void Loadcrossoverconfig(char* lineinputbufptr, int displayflag);

	void Savecrossoverconfig(int Crossoverrecordss, int LSsrce,int LStar1,int LStar2,int LStar3,int LStar4,int LStar5,int LStar6);

	/************************** detector and stop flag processing ********************************/

	void InitDetectortriggertable();

	void DisplayDetectorTablestatus(char* replymessage);

	void InitStopflagtable();

	void DisplayStopflagdata(char* replymessage);

	void ClearAllDetectortriggers();

	void UpdateTrigger(int device,int signalmsg, int Enableflg, int Statusflg);

	void ActionTrigger(int device);

	int GetTriggerstatus(int device);

	void ProcessDeviceTriggers();

	void Processdetectorcount(int ld, int occupied);

	void SaveDetectortriggerconfig(int Triggerdevice, int PToUR, int PtorURnumber);

	void StoreDetectortriggeractionconfig(int Triggerdevice, int Uncouplernumber, int Target, int Locoref, int Trainref, int LSreference, int NSSref);

	void ResetCountertarget(int Idt);

	void ResetCountertargetAll();

	void SetCountertarget(int Idt, int targetvalue);

	void LogicalSectionTcodeRefresh();

	int GetLogicalSectionTcode(int section);

	int CheckTrainIDinsection(int sc, int TrainIDnum);

	int GetLSTrainID(int sc, int TrainIDnum);

	int GetLSTrainIDcoupled(int sc, int TrainIDnum);

	int SetTrainIDcoupled(int sc, int TrIDsubscript, int value);

	int GetLogicalSectiondirection(int section);

	void Logicaldetectortriggersprocess();

	void Logicaldetectornumerictriggersprocess();

	void DetectorTableUpdate(char* replymessage);

	void Setlogicaldetector(int ld);

	/************************ Date & time stamp processing *******************************************/

	void InitLastupdatedetails();

	void RefreshLastupdatedetails(int rectype);

	int PScompare(int seqnum, int timestamp);

	int PDcompare(int seqnum, int timestamp);

	int GetPStimestampdata(char* message);

	int GetPDtimestampdata(char* message);

	void PointUpdateprocess();

	void PointUpdatecurrentposition(int pt, int OC);

	void Closeallswitches();

	void SwitchpointtoClose(int LPcode);

	void SwitchpointtoOpen(int LPcode);

	void Pointcurrentstatusprocess();

	void CrossoverUpdateprocess();

	void SetLSection(int section, int Traincode);

	void LogicalDetectorupdateDetectorstatus();

	void CcallLogicalsectionupdate();

	void BrowserStopflagUpdate(int code, int type, int stoptype,int stopPS, int stopCS, int stopslot, int stopTcode, int stopflg);

	void StopFlagReviewprocess();

	int GetP1code(int sc);

	int GetP2code(int sc);

	int GetPtype(int sc);
	
	void InitSignalRelayTable();
	
	void UpdateSignalRelayoutputcode();
	
	unsigned char GetCurrrelaysignalcode();
	
	unsigned char GetPrevrelaysignalcode();
	
	int Getpointcurrentstatus(int pointnum);

	void Updateprevsignalrelaycode();

	void Initialtrainpositionsetup(LNallTrains* LNallTrainsinformation);

	int GetDeviceLocoref(int Triggerdevice);

	int GetDeviceTrainref(int Triggerdevice);

	void LNallTrainslinkage(LNallTrains* LNallTrainsinformation);
	
	void LNsignalprocesslinkage(LNsignalprocessing* LNsignalprocess);

	std::vector <int>  CreateLSlist(int Psection);

	std::vector <int> GetLSdatalist(int LSsection);

	std::vector <int> CreateDetectorlist(int Psection);
	
	void RefreshTcodesandPstatusforSignals();

	int GetLogicaldevicetriggercount(int device);

	// ---------------- Train section logic - Jan 2024 - introduced with version 11.0  ----

	void TSinitTrainsections();

	int TSclidisplayTrainsections(int Trainsectiondisplay,char* replymessage);

	void StoreUncouplerdetails(char* lineinputbufferptr, int displayflag);

	void TSbuildLSsectionlist();

	int LSfetch(int* LSlist, int PS, int direction);

	void TSstoreTrainsectionconfig(char* lineinputbufferptr, int displayflag);

	int TSGUIdisplayTrainsecstions(char* replymessage);

	void TSpopulateLSdata();

	void TSupdateSection(int section, int CurrentTrainID, int NewTrainID, int CallID);

	void ProcessTrainRailcargroupmatch(int Trainsection,int fieldcheckflag);

	void TSallsectionsclear();

	int TSgetPS(int TS, int PSss);

	void UpdatesignalsTrainSections();

	void TSUpdatetrainsectionsPhysectionstatus();

	int TSgetoccupancyflag(int Trainsection);

	int TSgetPSoccupancyflag(int Trainsection, int PSsubscript);

	int TSmatchedLS(int LS);

	int TSmatchedLSindex(int LS);

	int TSgetLScount(int Trainsection);

	int TrainsectionNumberfetch(int PS);

	int TSgetTrainNumber(int Trainsection);

	int TSgetlastvalidPS(int Trainsection);

	// ---------------- Railcar processing logic - April 2024 - introduced with version 12.0  ----

	int GetRailcarInsection(int TS);

	void SetRailcarInsection(int TS, int railcargroupnumber);

	void LoadRailcargrouptoTrainsection(char* lineinputbufferptr, int displayflag);

	void LoadTrainstoTrainsections(char* lineinputbufferptr, int displayflag);

	void ClearRailcarGroupInsection(int TS);

	void TSallRCGsectionsclear();

	void TSallRCGlococouple(int Railcargroupsection);

	void TSallRCGlocoUncouple(int Railcargroupsection);

	/* PWM DC control sections functions   */

	void InitializePWMsections();

	int SetPWMsectiondirectionandspeed(int section, int speed,int direction);

	int SetPWMsectionDirections(int section, int direction);

	int GetPWMsectionData(int section, int* sectiondata);    // returns PWM specified section -  section reference, loco number, current speed and current direction

	int GetallcontrollerData(char* replymessage);

	int GetallPWMsectionData(char* replymessage);

	int FindPWMsection(int loco);

	void UpdatePWMcontroller(int controller,int speed,int direction,int mode);
	
	void RefreshsectionPWMcontrollers();
	
	void SwitchPWMsectionsmode(int mode);

	int GetPWMsectionmode();

	void UpdateAllPWMsections(int Speed, int Direction);

private:

	LNallTrains* LNallTrainsinformationLSLD = new LNallTrains();
	
	LNsignalprocessing* LNLSLDsignalprocess = new LNsignalprocessing();

	LNoperationlogging* LNOploggingptr;

	/****************************************************************************************/

	struct TSectionarray
		{
			int  TrainsectionNumber;
			int  Trainsectiontype;
			int  Traincode;
			int  Traintype;
			int  TSoccupancyflag;
			int  RCGcode;
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


	/***********************************************************************
	/
	/
	/  Record number 		  000
	/  Logical section  	  000
	/  Direction              0 = forwards  1 = backwards
	/  Physical section 1     000
	/  Type                   1 = Plain section  2 = Plain Switch  3 = Split section  4 = Split section switch  5 = device detector
	/  Physical section 2 	  000
	/  Logical partner        000
	/  Section speed            0
	/  Next section 1         000
	/  Next section 2         000
	/  Next section 3         000
	/  Next section 4         000
	/
	/ X - end of file
	*/

	struct LDMsgTimestamp
	{
		int Seqnumber;
		unsigned char HH[2];    // 00
		unsigned char MN[2];    // 00
		unsigned char SS[2];    // 00
		unsigned char MLS[3];   // 000
		int Timestampvalue;
	};

	struct LDMsgTimestamp LDMTS[NUMBEROFTIMESTAMPS];

	struct LDMsgTimestamp *LDMTSptr;

	int LNsectionnumber;
	int LNsection;
	int PHSection1;
	int PHSection2;
	int LNDirection;
	int LNType;
	int LNdirectionflag;
	int LNsectionpair;

	int LNsectionspeed;
	int LNnextsection1;
	int LNnextsection2;
	int LNnextsection3;
	int LNnextsection4;

	int LastPSupdateseqnum;
	char LastPSupdateHH[2];
	char LastPSupdateMN[2];
	char LastPSupdateSS[2];
	int LastPStimestampval;

	int LastPDupdateseqnum;
	char LastPDupdateHH[2];
	char LastPDupdateMN[2];
	char LastPDupdateSS[2];
	int LastPDtimestampval;
	
	/* ********************* Signal processing for Relay based signals ***** */
	
	unsigned char Currsignalrelaycode;
	
	unsigned char Prevsignalrelaycode;
	
	struct SectionsignalArray
	{
		int 			PSectnumber; 
		unsigned char Signalstopcode;		
	};
	
	struct SectionsignalArray Sectionsignaldata[NUMBEROFSIGNALRELAYS];

	/* ********************* logical sections ***************************** */
	/* Expanded to include route processing data  - July 2021               */

	struct LogicalsectionArray
	{
		int Physectionref;
		int LogicalsectionTcode;
		int Logicalsectiondirection;
		int Logicalsectiontype;
		int Physectionref2;
		int GUIobjectreference[MAXGUIREFSFORLS];
		int Logicalsectionpair;
		int Sectionspeed;
		int Logicalentrysignalreference;
		int Nextsection[MAXNUMBEROFLSNEXTSECTIONS];
		int LogicalsectionTrainID[NUMBEROF_LS_TRAIN_IDS];
		int TrainIDcoupled[NUMBEROF_LS_TRAIN_IDS];
	};

	struct LogicalsectionArray Logicalsectiondata[NUMBEROFLOGICALSECTIONS];
	
	/* PWM section control structure             */

	int PWMcontrolmode;
	
	struct PWMphysicalsections
	{
		int SectionreferenceNumber;
		int LocoCode;
		int Currentspeedvalue;
		int Prevspeedvalue;
		int Currentdirectionvalue;
		int Prevdirectionvalue;
		int SectionTransmissionreference;
		int Linkedlogicalsections[PWMPHYSECTIONSMAXLS];
		int GUIobjectreference;		
	};

	struct PWMphysicalsections PWMsectionsarray[MAXPWMPHYSECTIONS];

	struct ManualPWMcontrollers
	{
		int Speed;
		int Direction;
		int Mode;
	};

	struct ManualPWMcontrollers Manualcontrollersarray[MAXCOUNTMANCONTROLLERS];

	int SectionTcodesforSignals[NUMBEROFLOGICALSECTIONS];

	int TrainsectionstatusforSignals[MAXTRAINSECTIONS];

	int NumberofLogicalSectionsloaded = 0;

	int Maxsectionnumberloaded = 0;

	/***************** Logical detectors **********************************/

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

	struct Logicaldetectortable
	{
		int DPhysectionref;
		int Detectorstatus;
		//int LogicalsectionTcode;
		//int Logicalsectiondirection;
		//int Logicalsectiontype;
		//int Physectionref2;
	};

	struct Logicaldetectortable LogicaldetectorLkUpdata[LOGICALDETECTORS+1];

	int NumberofLogicaldetectorsloaded = 0;
	int NumberofRFiddetectorsloaded  = 0;
	int NumberofReeddetectorsloaded = 0;
	int NumberofVerticalIFRdetectorsloaded = 0;
	int NumberofHorizontalIFRdetectorsloaded = 0;

	/***************** Point processing  **********************************/

	struct Pointprocessing
	{
		int PPointnumber;
		int CurrOpenClose;
	};

	struct Pointprocessing PointStatusTable[NUMBEROFPOINTS+1];

	int PointstatuscodesforSignals[NUMBEROFPOINTS+1];

	struct ComplexPointprocessing
		{
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
		};

	struct ComplexPointprocessing ComplexPointprocessTable[NUMBEROFCOMPLEXPOINTS+1];





	int Numberofpointsloaded;

	int Crossoverrecordloaded;

	int Devicetriggerrecordloaded;


	/***************** Crossover processing  **********************************/

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

	/***************** Point DCC codes  **********************************/

	struct PointDCCcodes
	{
		unsigned int Pointnumber;
		unsigned int Openclose;
		unsigned char DCCcode[4];
	};

	struct PointDCCcodes PointDCCcodetable[NUMBEROFPOINTS+1][PTOPENCLOSEFLAG];

	/****************** Detector trigger processing ***********************/

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
		int ActionLSreference;			// logical section where Train action is to occur
		int ActionNSLSreference;		// logical section where Train is next due to go
	};

	struct Detectortrigger DetectorTriggerTable[LOGICALDETECTORS+1];

	struct Uncouplerdata
	{
		int Uncouplernumber;
		int Psectionlocation;
		int UCtriggerstatus;
	};

	struct Uncouplerdata UncouplerReferencedata[NUMBEROFUNCOUPLERS];

	int Uncouplerloadcount = 0;

	/******************* Stop flag processing ****************************/

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

	struct Stopflagarray Stopflagtable[MAXNUMBEROFSTOPFLAGS+1];
};

#endif /* LNWEBLSECTIONSLDEVICES_H_ */
