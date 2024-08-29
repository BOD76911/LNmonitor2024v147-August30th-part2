/*
 * LNwebconfig.h
 *
 *  Created on: Jun 15th  2019
 *  Author: BrianRoot
 *
 *      Source of config details for LNweb2019NG project
 *
 *
 *   Code clean-up  June 2023
 */
 
#include <string>
#include <string.h>

#define GUIMESSAGELINELENGTH		    64
#define MAXGUIMESSAGELINES				4

#define MAXTRAINSECTIONS				25
#define MAXPSinTS						4
#define MAXLSinTS						12
#define	MAXGUIrefTS						4
#define MAXNSinTS						4
#define MAXGUICURVEANGLERECORDS			8

#define MAXLOGICALSECTIONS		 		128
#define MAXLOGICALDETECTORS		 		8
#define MAXPWMPHYSECTIONS               32
#define PWMPHYSECTIONSMAXLS				12
#define MAXCOUNTMANCONTROLLERS			5

#define MAXTCDBUFFERS					8  // track circuit buffers 

// parameters for command processing

#define MAXNUMBEROFCOMMANDS 			256
#define Commandlengthdefault 			4

#define 	COMMANDDESCRIPTIONLEN		50
#define 	COMMANDTEXTLENGTH			 5
#define     MAXGUIOBJECTS               128
#define     MAXGUILAYERS				10
#define     MAXGUIREFSFORLS             8
#define     MAXGUITEXTLENGTH            26
#define     MAXNUMBEROFLSNEXTSECTIONS   12

#define		CMENUS						4

#define     NUMBEROF_LS_TRAIN_IDS		4
#define     NUMBEROFLSCODESINSECTION 	8

// Train settings

#define 	MAXIMUMOFCARSONTRAIN		16

#define		MAXIMUMOFTRAINS				64

// RFid processing definitions

#define RFIDCHARCOUNT 				13
#define VEHICLEDESCRIPTIONLEN  		22
#define VEHICLEMAXCOUNT  			32

// Route, Journeys and Schedules settings

#define    	MAXNUMBEROFROUTELINESINROUTE   2048
#define     MAXNUMRTESTORED				64
#define     MAXNUMRTESTEPS				96
#define     MAXNUMRTESUBSTEPS			6

#define     MAXNUMBERLOCOROUTES     	64

#define 	RTEDISPSHORTDESCLENGTH 		12
#define 	RTEDISPLONGDESCLENGTH		128

#define 	MAXNUMBEROFSCHEDULESRECS    32
#define 	MAXNUMBEROFSCHEDULESTEPS    32


// Simulator settings - linked to number of routes saved

#define     MAXNUMSIMROUTES				64
#define 	MAXNUMSIMROUTESTEPS			96
#define 	SIMROUTEDESCRIPTIONLENGTH 	64

#define 	LRMESSAGELENGTH			120

#define 	MAXINPUTPARAMLEN		8192 //1024 //512 //256 //195 // 128
#define		MAXINPUTPARAMCNT		256
#define     MAXRECSINFILE			256

#define 	MAXWEBINPUTPARAMLEN		8096
#define		MAXWEBINPUTPARAMCNT  	128

#define     CPROGUPLOADBYTECNT		32

#define 	PARAMSUBELEMENTS     	25
#define 	PARAMSUBELEMENTSLENGTH  20

#define     SHORTMESSAGELENGTH      200

#define     OUTBOUNDCMDLENGTH       32
#define     OUTBOUNDCMDMSGLENGTH    64

#define     TCPOUTBOUNDREQLEN       32
#define     OUTPUTLINELENGTH        80
#define     OUTMSGBUFFERSIZE		4096

#define     LNSIMRTEDESCRIPTIONLEN  	20

#define     LNSCHEDULEDESCRIPTIONLEN	30

#define     WEBINPUTBUFFERSIZE  	160000//135000//65536// all three updated July 2021 to handle larger script files
#define 	OUTPUTMSGBUFFERSIZE		160000//135000//65536//49182 //131072 //65536 // 49152
#define 	WEBSERVERBUFFERSIZE     160000//135000//65536//49182 // 31072 // 49152

#define 	LINEINPUTBUFSIZE 		135000//65536 //49152

#define     UARTDATABUFFERSIZE		255

#define     SPEEDINDEXVALUES		10

#define		MAXTIMERLOOPS			8

#define     LOCOINDEXSPEEDELEMENTS	10

#define 	NUMBEROFDCCSLOTS 		20

#define 	NUMBEROFPOINTS 			64

#define		NUMBEROFCOMPLEXPOINTS	64		// increased - Sept 4th 2023

#define 	POINTSINCLUSTER			16

#define     MAXNUMBEROFSIGNALS      32

#define     MAXNUMBEROFSIGNALPSECTIONS   4

#define     MAXNUMBEROFSIGNALLSSECTIONS  8

#define     MAXNUMBEROFSIGNALTOPOINTLINKS  4

#define 	NUMBEROFSIMEXECUTORS    6

#define     MAXNUMBEROFLOCOMOTIVES  16

#define     MAXNUMBEROFLOCOSPEEDVALUES 10

#define     LOCODESCRIPTIONLENGTH   32

#define 	MAXNUMBEROFSCHEDULES	12

#define 	NUMBEROFCROSSOVERSECTIONS 	64

#define 	PTOPENCLOSEFLAG     	2

#define     DISPLAYBUFFERLENGTH     32768 // 16384 // 256

#define      HEAPDISPLAYBUFFERLENGTH  81920 //  65536//16384 //16384  June 4th

#define     OUTPUTRECSIZE           		128

#define 	PHYSICALDETECTORS       		12

#define     BCMTRACKCIRCUITCONNECTORS	  	4

#define 	NUMBEROFLOGICALSECTIONS 280 //384 //256

#define 	NUMBEROPERATIONALLOGSECT 280 //384 //256

#define     NUMBEROFSIGNALRELAYS    8

#define 	LOGICALDETECTORS      	30

#define     NUMBEROFUNCOUPLERS      12

#define 	MAXDEVICELSREF			12

#define 	MAXDEVICECOUNT 			48

#define 	MAXNUMBEROFSTOPFLAGS	8

#define     LSPERPS                 8

#define     MAXNUMOFTHREADS         8

#define     MAXPSECTIONS            91

#define     USBBYTEMAX				756

#define 	DELIMITER	            '-'

#define     DATETIMESTRING          30

#define 	NUMBEROFTIMESTAMPS		8

#define 	PS_TIMESTAMP           	1

#define 	PD_TIMESTAMP          	2

#define 	SLOT_TIMESTAMP         	3

#define 	PTS_TIMESTAMP          	4

//#define     WEBDirectory 					"/home/brian/LNweb2021/LNweb2021/"

#ifdef PIRun
#define     WEBDirectory 					"/home/pi/LNmonitor2024/ConfigandDatafiles/"
#else
#define     WEBDirectory 					"/home/brian/LNmonitor2024/ConfigandDatafiles/"
#endif

// Logging details

#define  PILogfiledirectory "/home/pi/LNmonitor2024/Logfiles/"

#define  Logfiledirectory   "/home/brian/LNmonitor2024/Logfiles/"

#define     DATETIMEFILENAMELENGTH		80

#define		NUMBEROFLOGLEVELS            3

#define		LOGRECORDLENGTH			   160

#define 	LOCOLOGLENGTH 				30

#define     TCPMSGLEN					80

#define     DATETIMERECORDLENGTH        80

#define     COMMANDFILENAME                 "LNWebcommands.dat"

#define     SECTIONMAPPINGFILENAME          "LNLogicalsection.dat"

#define     POINTSDCCDATAFILENAME           "LNpointsDCCcodes.dat"

#define     POINTSCONFIGFILENAME            "LNpointsignalconfig.dat"

#define     LOCOSPEEDTABLEFILENAME          "LNlocospeedtable.dat"

//#define     STATICSIMFILENAME            	"LNstaticsimulator.dat"

#define     DISPLAYCONFIGFILENAME           "LNDisplayarrayconfig.dat"

#define     RFIDCONFIGFILENAME				"LNRFidconfig.dat"

#define     RTETRAINSCHEDFILENAME			"LNRteTrainSched.dat"

#define     TRAININITIALDATAFILENAME        "LNInitialTrains.dat"

#define		PSECTIONSDCCFILENAME            "LNDCCsections.dat"

#define     ALLCONFIGFILENAME               "AllConfigfile.dat"

/*********** strings for data and config filenames ****************/
 

extern std::string SectionMappingnamestr;
extern std::string PointsDCCdatafilenametr;
extern std::string PointsConfigfilenamestr;
extern std::string Locospeedtablefilenamestr;
//extern std::string Staticsimfilenamestr;
extern std::string Displayconfigfilenamestr;
extern std::string RFidConfigfilenamestr;
extern std::string RouteTrainSchedfilenamestr;
extern std::string Traininitialdatafilenamestr;
extern std::string PSectionDCCcodesfilenamestr;

extern char* PsectionDCCfilenameptr;

/***************** Static Simulator processing ************************************************/

#define  FIELDCOUNT  				8
#define  MAXSIMROUTESLINES			64
#define  MAXSIMROUTESTEPS	    	15

#define SIMDESCRIPTIONLEN   		64 //25

//int LNRFidControllerport = 8098;

void LNwebmonConsole();

void LNwebserverexecute();

void STPDcheckloop();

extern int UARTmessagesent;

extern unsigned char UARTRxdatabuffer[UARTDATABUFFERSIZE];

extern int Oploghistoryflag;

extern int configreloadflag;

extern int RWPsectionflag;

extern int STPDprocessflag;

extern int STPXprocessflag;

extern int RWPprocesslock;

extern char * PRHeapResbuff;

extern struct MsgTimestamp *MTSptr; // = NULL;

extern char PResults[(DISPLAYBUFFERLENGTH * 2) + 1];

extern void ResetTF1Rpin();

extern void ResetTF2Rpin();

extern void ResetReedswitchpin();

extern void L293pwmhighpin();

extern void L293pwmlowpin();

extern void L293IN4highpin();

extern void L293IN4lowpin();

extern void L293IN3highpin();

extern void L293IN3lowpin();

//extern void IRdetectordataupdate(int* sensorstateptr);

extern void CloseactiveBTsignalsconnection();

extern void BTtestmessagesend(int TTcode);

extern int RetrieveBTconnectionstatus();

extern int Reloadconfiguration();

extern void DCCslotdatareload();

extern int DCCslotdatafetchcall(int DCCcode);

//extern void RFidmessageprocess(int RFid_device, char RFidcode[RFIDCHARCOUNT]);

//extern void UARTdataupload(unsigned char InPSbytes[CPROGUPLOADBYTECNT], int Charcount);

//extern void CDataupdate(unsigned char[CPROGUPLOADBYTECNT], int);

int GUIAllmsglinesgetMain(char* replymessage);

void InsertGUItextlineMain(char* GUImessage);

void TTMotorRunforward(int Mspeed);

void TTMotorRunbackward(int Mspeed);

void TTMotorStop();

char GetTTInputDatastatus();

char RefreshTTstatus();

int CheckTTcounter();

void Serialcommsthread(unsigned int * Psectionsptr,unsigned int * PDetectorptr, unsigned int * DCCslotcodeptr, \
		unsigned int * DCCslotdirectionptr,unsigned int * DCCslotspeedcodeptr, unsigned int * Pointstatusflagptr, MsgTimestamp* MSGptr);

void PSectionreset();

int UARTRXdatamessage(int bytecount, char* returnmessage);

void PiUartPWMSerialTransmission(int section, int speed, int direction, int mode);

void Psectiondataconfigclear();

void LNPsectionconfigload(unsigned char* lineinputdataptr);

void RWtoPSectionUpdate();

void RWtoPSectionTimerUpdate();

void USBbytesclear();

void InitTCDbuffers();

void GetTCDbufferdata(char* returnmessage);

void DisplayTCDconfig(char* returnmessage);

void GetBCMconnectordata(char* returnmessage);

void TCDdataupdate();

unsigned char AtoH(char bytein);

char HtoA(unsigned char c);

unsigned char StringtoH(unsigned char *charinput);

void Rtnmsgbuffclear();

int PSectionset(int sc, int value);

void SetTimestamp(int offset, int sa, MsgTimestamp* MSGptr );

int PSectiondatareturn(int PS, char* returnmessage);

void Sectioncounterset(unsigned int ec, unsigned int xc);

void USBbytecounterset(unsigned int bc);

void USBbytesclear();

void GetSerialInterfacecntrs(char* returnmessage);

void Slotdatareturn(int slot, char* returnmessage);

int Pointstatusdatareturn(int Point, char* returnmessage);

void UpdateSlotdata(int sc,int scspeed,int scdirection);

void CSpeedcmdprocess(int callref, int SLCslot, int SLCslotspeed, int SLCslotdirection);

void CSwitchmsgprocess(int pt, int OC, int calledby, unsigned char DCCcode[4]);

void USB0Serialmsgprocess(int mlen, unsigned char msgcode[16]);

void TTcommandsend(int mlen, unsigned char* TTcmd);

void Signalcommandsend(int mlen, unsigned char* Sigcmd);

void CPointDCCcodeupdate(int ptc, int OC, unsigned char CodeDCC[4]);

void CDisplaySCPointDCC();

void CAllslotsstop();

void Setdevicedata(int dd);

int Devicedatareturn(int dd, char* returnmessage);

void Slotsreset();

int Findslotcode(unsigned int dccvalue);

void Pointstatusflagreset();

void Linktimereset();

extern struct MsgTimestamp MTS[NUMBEROFTIMESTAMPS];

