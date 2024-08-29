//============================================================================
// Name        : LNwebmonNGmain.cpp
// Author      : B ODonnell
// Version     :  3.0   September 2021
// Copyright   : Your copyright notice
// Description : Loconet web monitor in C++, Ansi-style
//============================================================================
// Update October 2019 - to handle block occupancy tracking using either Loconet 
// -------------------   messages or Track circuit detectors (TCD). 
//
//  The track circuit detectors (TCD) are picked via the raspberry Pi parallel port.
//
//  The section details, to if the occupancy messages are LN messages or TCDs are set in 
//  config file that is loaded by the routine 'LNSectionFileload' in the C program
//  LNserial.c
//
//
//	July 2021 - Updated to include RFid sensor processing (was lost when LNwebmon2019NG was lost in June 2021
//				Updated to include Graphical display configuration processing - initial version installed July 2021
//				To be updated to include Route processing functionality from LNRteproc
//
//  August 2021 - Updated to include Route processing functionality
//
//	September 2021 - Route processing functionality included with simulator file creation. Can build and execute routes on the simulator. Route generation
//					 not yet including all stop and loop control functionality.
//
// -------------------------------
// Version 3.0 - major new version
// -------------------------------
//
//  Sept26th 2021 - Now includes route processing functionality, using either CLI or file based inputs.
//					File inputs come from a .dat file that includes Route info ,Journey (Loco + Route) info and Scheduler info.
//					Includes initial version of Scheduler process - tested for simple point to point test track
//					Includes updated simulator which reads loco speed commands as it is processing
//
// Version 3.2  - expanded commands list to include console commands for TT
//
// -----------
// Version 4.0 - April 2022
// -----------
//
// Version 4.0  - incldes makefile parameter for PI or Ubuntu version. Implements new file name structure to include
//                layout name in file name. Layout name is entered as parameter when LNNwebmonNG program is run. 
//
// Version 4.1 - Updated to include Raspberry Pi control of turntable with direct sensor and motor interface.
// Version 4.2 - Updated to enable use of PWM on PI for multiple turntable speeds
// Version 4.5 - for processing infra red interfaces
//
//
// Version 5.0 - incorporates RFid processing and Infra-red detctor processing. The RFid detection used connected receivers
//               and the Infra-red detectors using interrupt based inputs on the GPIO pins which are connected via opto-isolators to the IT hardware
//               The initialization and processing code for the inputs for both of these are located in the module LNserial.c as they use the wiringPi
//               library
// Version 5.1   includes RFid detector processing, Infra-red detector and turntable processing - not yet tested RFid USB detectors 
//-------------
// Version 5.2   September 2nd 2022 - RFid integration and tested OK with Infra-red detectorprocessing and Turntable 
//
// Version 5.3   October 31st - includes Signal processing functionality using new class
//
// Version 6.0   March 4th 2023 - installation of logging functionality
//
// Version 6.2   March 24th - introduction of new config file for commands processing. Start of refactoring of console processing to use switch command
//
// Version 7.0   April 5th - introduction of operational logging for Psection and LSsection status - first release. No read back yet.
//
// Version 8.0   April 30th includes operationl logging read-back. Activation of scheduler steps - tested with simulation and live layout.
//				            includes scheduler step command functionality.
//
// Version 9.0   May 30th - includes rewrite of Route search functionality. Includes new point processing for handling multiple point clusters. Tested with three
//                          schedules running concurrently.
//
//  Version 10.1  June 21st - New version of section crossing code
//
//  Version 10.2   July 20th - start of colour light signal commissioning 
//
//  Version 10.4   October 30th - fixed bug in simulator to clear previous LS associated physical section
//
//  Version 10.5   December 8th - new version of Webmessage reply module - cleaned up code to enable updating and readied for use with GUI HTML screens
//
//  Version 10.7   December 27th - includes GUI point switching, points integrated with signals
//
//  Version 10.8   December 27th - Includes GUI tables display, Journey button, Simulator tested for process control integration
//
//  Version 10.9   January 1st 2024 - All file references updated to 2024
//
//  Version 11.0   January 2nd 2024 - first version of TrainSections - tables, CLI and GUI. No control logic yet
//
//  Version 11.1   January 30th - Trainsections logic for loco movements, GUI message lines
//
//	Version 11.2   February 8 - Train section logic drives graphics OK. Need to fix signals
//
// 	Version 11.3   February 14th - Train section logic drives graphics OK. signals OK
//
//	Version 11.4   February 21st - New version of GUI display configuration load program
//
//  Version 11.5   March 11th 2024 - new version of GUI display config load program. Dynamically builds records from base section definitions - Not fully completed.
//
//  Version 11.6   March 18th 2024 - dynamic GUI record build done
//
//  Version 11.7   March 28th 2024 - dynamic GUI record build done extended to handle multiple devices in same section
//
//  Version 12.0   March 31st 2024 - Start of introduction of Railcar groups and Trains as logical entities
//
//				  a) smallest vehicle is a railcar (maybe freight or passenger railcars)
//                b) a Railcar group consists of one or more railcars
//                c) a Railcar group with a locomotive attached is a Train
//
//				Train logic and Control logic
//              -----------------------------
//
//				Loco master information is stored in LNlocoAlldata in LocoAlldatatables
//
//				Loco dynamic data when loco is running on a route is held in table Locodatarecord->Locoinformation[Lococountmax];
//
//				Loco control logic operates at the LS (logical section) level unless the journey specifies that Trainsection level logic is to be used.
//
//              A Train section comprises of one or more logical sections.
//
//				Train information is held in the table TrainSectiondata[]. Train information is also stored in the table Logicalsectiondata[].
//
//         		When a journey is initiated the Train information from the journey record is stored in the Train ID field in the logical section table where
//				the locomotive is located. It is also stored in the TrainSectiondata table.
//
//
//  Version 13.0  July 7th 2024 - i2c processing functionality added to LNserial.c for new type of track circuits 
//								- Raw data only displayed 
//
//  Version 14.0   August 5th 2024 - start of work to include external speed control for PWM
//                                - Ver 14.0 - working on Pi UART interface. Uses interface call originally 
//                                             built for turntable interface.
//                                           - Receives message from external microcontroller with analog
//                                           - speed and direction
//  
// Version 14.1   August 10th 2024 - progressing work to include external speed control for PWM based DC control system
//
// Version 14.2   August 10th 2024
//
// Version 14.3   August 22nd - new version for UART output signal 
//
// Version 14.4   August 23rd - new version for UART output signal switching mode
//
// Version 14.5   August 23rd - version with built in timing delay for UART output signal switching mode
//
// Version 14.6   August 24th - Extended I2C connection to multiple MCP23017 I/O buffers 
//
// Version 14.7   August 27th - Start on Track circuit and optical detectors config loading 
//
//
//---------------------------------------------------------------------------------------------------------------
//
#define PROGRAMVERSION "\n\nLNmonitor2024 - 2024 Loconet interface program NG Ver 14.7 August 27th 2024 \n"
//
#define PIPROGRAMVERSION "\n\nLNmonitor2024 - 2024 Loconet interface program for Raspberry PI - NG Ver 14.7  August 27th 2024 \n"
//
/* ********************************************************************************* */
/* Section Pointer architecture                                                      */
/* ============================                                                      */
/* Raw data from LN messages processed by Serialcomms routine in LNserial.c          */
/*                                                                                   */
/* Raw data is stored in array RWPsectionserialdata[]                                */  
/*                                                                                   */
/*                                                                                   */
/* There is a configuration flag in the LNsection.dat file that controls the config  */
/*                                                                                   */
/*      Value == 1 means the section current occupancy status is a direct hardware   */ 
/*      update message type via the Raspberry Pi I/O parallel interface              */                             
/*                                                                                   */  
/*                                                                                   */ 
/*       Value == 2 means it is updated via a Loconet message                        */
/*                                                                                   */
/* Results are stored by using pointer 'Psectionsptr' passed to LNserial.C           */ 
/*                                                                                   */
/* Pointer Psectionptr points to RWPsectionserialdata[]                              */
/*                                                                                   */
/* PSectiondataconfigptr pointers to the physical section data source config array   */
/*                                                                                   */
/* Pointer RWPsectionsptr points to Psectionserialdata[] - this is the MASTER array  */
/* for the physical section data                                                     */   
/*                                                                                   */
/* The routine RWtoPSectionTimerUpdate() transfers the data to the array             */ 
/* LNsectionserialdata[]                                                             */
/*                                                                                   */
/*      Hierarchy of arrays                                                          */
/*      ===================                                                          */
/*       Top level array     Psectionserialdata[]    MASTER ARRAY                    */
/*                                 |                                                 */
/*                                 |  C**                                            */
/*                                 |                                                 */
/*       LNPsectionserialdata[]----------TCDsectionserialdata[]                      */
/*                 |                              |                                  */
/*                 | A**                          | B**                              */
/*       RWPsectionserialdata[]          TCDinputbuffers[]                           */
/*                                                                                   */
/*       A** - transfer by program RWtoPSectionTimerUpdate()                         */
/*       B** - transfer by program TCDblockprocess()                                 */
/*       C** - transfer by Sectiondataupdate()                                       */
/*                                                                                   */
/* ********************************************************************************* */
/*                                                                                   */
/* 	June 2020 - Added additional serial port processing to drive external device     */
/*              via USB port                                                                                       */
/*                                                                                                                 */
/*                                                      	                                 	                   */
/*  March 2023 - Re-factored to consolidate config into three files only                                           */
/* *************************************************************************************************************** */

#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <thread>
#include <pthread.h>
#include <chrono>
#include <cstring>
#include <assert.h>

#include "LNwebconfig.h"
#include "LNwebclocktime.h"
#include "LNwebmonNGfileload.h"
#include "LNwebserver.h"
#include "LNwebLsectionsLdevices.h"
#include "LNwebmonSimulator.h"
#include "LNlocoAlldata.h"
#include "Clocktime.h"
#include "LNweblocorouteprocess.h"
#include "LNdisplayconfig.h"
#include "LNRFidsensorprocessing.h"
#include "LNRouteprocess.h"
#include "LNRteDevicereferencedata.h"
#include "LNSimulatorAllRoutes.h"
#include "LNSchedulerAlljourneys.h"
#include "LNsignalprocessing.h"
#include <string>
#include <string.h>

#include "LNConsoleprocess.h"
//#include "LNLocoRoutemovementMaster.h"
#include "LNLocoRoutemovementMasterNG.h"
#include "LNRteLogicalsectionData.h"
#include "LNRteLogicalsectionGroup.h"
#include "LNSchedulerExecutor.h"
#include "LNloadRteTrainSchedfile.h"
#include "LNallTrains.h"
#include "LNRouteSchedMaster.h"
#include "LNRteRoutemaster.h"
#include "LNSimRouteMaster.h"
#include "LNturntableprocess.h"
#include "LNDatetime.h"
#include "LNlogging.h"
#include "LNoperationlogging.h"
#include "LNconfigloader.h"
#include "LNcommands.h"

/*****************************************************************
 * Time stamp records
 *
 *     1                     Physical section updates
 *     2                     Physical detector update
 *     3                     Slot data update
 *     4                     Point status
 *
 ****************************************************************/

struct MsgTimestamp
{
	int Seqnumber;
	unsigned char HH[2];    // 00
	unsigned char MN[2];    // 00
	unsigned char SS[2];    // 00
	unsigned char MLS[3];   // 000
	int Timestampvalue;
};

unsigned char TCDbuffers[MAXTCDBUFFERS];

struct DCCSlotArray
    {
	int 	Lococode;
	int 	ActSpeed;
	int     Actualdirection;
    };

struct DCCSlotArray DCCSlotInformation[NUMBEROFDCCSLOTS];

struct GUIservermessagesMain
{
	char GUIservermessagelineMain[GUIMESSAGELINELENGTH];
};

struct GUIservermessagesMain GUImessagetextlineMain[MAXGUIMESSAGELINES];

int RWPsectionflag;

int RWPprocesslock;

int RWPcounter;

int RWPlockcounter;

int configreloadflag;

int STPDprocessflag, STPBprocessflag, STPMprocessflag, STCCprocessflag, STPXprocessflag;

struct MsgTimestamp MTS[NUMBEROFTIMESTAMPS];


// calls to &  from LNserial module

extern "C" {
int Serialcomms(unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, struct MsgTimestamp*,unsigned int*, \
		unsigned int*, unsigned int*, unsigned char*, unsigned int*, unsigned int*, unsigned int*, char*);
		
int SerialUSB0comms(unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, struct MsgTimestamp*,unsigned int*, \
		unsigned int*, unsigned int*, unsigned char*, unsigned int*, unsigned int*, unsigned int*);		

void Speedcmdprocess(int, int, int);
void Switchcmdprocess(int, int, unsigned char[4]);
void Allslotsstop();

int Writemessage(unsigned char[4]);
int PiUARTWritemessage(int mlen, unsigned char msgout[8]);
void linebufferprocessingNG(unsigned char*);
void PointDCCcodeupdate(int,int,unsigned char[16]);
void DisplaySCPointDCC(int);
void InitializeslotArray();
void Ctriggerlogicalupdate(int value);
void IRdetectordataupdate(int* sensorstateptr);
void RFidmessageprocess(int RFid_device, char RFidcode[RFIDCHARCOUNT]);
void CDataupdate(unsigned char[CPROGUPLOADBYTECNT], int);
void UARTdataupload(unsigned char InPSbytes[CPROGUPLOADBYTECNT], int Charcount);
int UploadDCCslots();
void TCDdatainitialize();
int DisplayTCDdatabufferconfig(char*);
void GetBCMrawdata(unsigned char*);
void TCDbuffersclear();
void TCDdatarequest(unsigned char*, unsigned int*);
void ResetTTendstopreedswitch();
void TTMotorRun(int,int);
void ResetTF1R();
void ResetTF2R();
int GetPSectionconfigflag(int);
void L293pwmhigh(int);
void L293pwmlow();
void L293IN4high();
void L293IN4low();
void L293IN3high();
void L293IN3low();
void BTtestmessage(int);
int BTsignal_linkinitialize();
void CloseBTsignalsconnection();
int GetBTconnectionstatus();
void BTsignalmessage(unsigned char[7]);
int DCCslotdatafetch(int);
int DCCslotloadedstatus();
void DCCslotdatatransfer(int);
}

std::string Layoutfilename;

char * PRHeapResbuff;

unsigned char * TCDdataptr;

char PResults[(DISPLAYBUFFERLENGTH * 2) + 1];

// Create pointers to objects 

LNDatetime* LNDatetime;

LNlogging* LNlogptr;

LNoperationlogging* LNOplogptr;

LNoperationlogging* LNOplatestlogptr;

LNcommands* LNcommandsptr;

LNconfigloader* LNconfigload;

LNwebLsectionsLdevices* LNLSLD;

LNlocoAlldata* LNlocodatatables;

LNweblocorouteprocess* LNLRprocess;

LNwebmonSimulator* LNSIM;

LNdisplayconfig* LNDC;			// Graphical display configuration object

LNRFidsensorprocessing* LNRFIDSP;

LNRouteprocess* LNRteproc;

LNRteLogicalsectionData* LNRteLSi;

LNRteLogicalsectionGroup* LNRteLSG;

LNRteDevicereferencedata* LNRteDRfd;

//LNLocoRoutemovementMaster* LNRtimeallrecs;

LNLocoRoutemovementMasterNG* LNRtimeallrecsNG;

LNSimulatorAllRoutes* LNSimAllRtes;

LNSimRouteMaster* LNSimRteMaster;

LNSchedulerAlljourneys* LNSchedAlljourneys;

LNSchedulerExecutor* LNSchedExecutor;

LNloadRteTrainSchedfile* LNRteTrainScheduledata;

LNallTrains* LNallTrainsinformation;

LNRouteSchedMaster* LNRteSchedMast;

LNturntableprocess* LNttprocess;

LNsignalprocessing* LNsigprocess;

//LNDatetime* LNLogdatetime;

// LNSimulatorAllRoutes* LNSimAllRtes, LNSchedulerAlljourneys* LNSchedAlljourneys;

int CurrentUSBbufferbytecount;

unsigned int * 	USBbytesreadcntrptr;
unsigned int  	USBbytesreadcnt;

unsigned char * USBbytestreamdata;

int 			UARTmessagesent;
unsigned char   UARTRxdatabuffer[UARTDATABUFFERSIZE];
int             UARTbytesreceived;
int             UARTEoMflag;
int             UARTSoMflag;
//int 			TTLastcommandsent;
int             TTLastcommandacknowledged;

/*********** strings for data and config filenames ****************/
 

std::string SectionMappingnamestr;
std::string PointsDCCdatafilenametr;
std::string PointsConfigfilenamestr;
std::string Locospeedtablefilenamestr;
//std::string Staticsimfilenamestr;
std::string Displayconfigfilenamestr;
std::string RFidConfigfilenamestr;
std::string RouteTrainSchedfilenamestr;
std::string Traininitialdatafilenamestr;
std::string PSectionDCCcodesfilenamestr;

std::string LNconfigfilenamestr;

char* PsectionDCCfilenameptr;

/************************* logging control flags ********************************/

int Oploghistoryflag = 0;

/************* physical section data *******************************************/

unsigned int * Psectionsptr;

unsigned int * RWPsectionsptr;
unsigned int * LNPsectionsptr;				// pointer for LN messages serial pointer 
unsigned int * TCDPsectionsptr;			// pointer for TCD messages serial pointer 
unsigned int * PSectiondataconfigptr;       // pointer to the array that holds the section config 

unsigned int * RWTEPsectionsptr;
unsigned int * RWTXPsectionsptr;
unsigned int * RWTCPsectionsptr;

unsigned int * 	Psectionserialdata;    //	Pointer to physical section data on the HEAP memory

unsigned int *     Psectiondataconfig;  

unsigned int *     PsectionTCDbufferdata;  

unsigned int * 	RWPsectionserialdata;	//  Pointer to the physical section data on the HEAP memory 
                                            //  where there is a delay in processing.
unsigned char * BCMconnectordata;		// Pointer to array on HEAP memory for Track circuits using I2C based BCM connectors

unsigned int * 	RWPTEsectionserialdata;

unsigned int * 	RWPTXsectionserialdata;

unsigned int * 	RWPTCsectionserialdata;

unsigned int   	PSlastupdate_seq;

unsigned int * 	Psectionentrycntrptr;
unsigned int 	Psectionentrycnt;

unsigned int * 	Psectionexitcntrptr;
unsigned int 	Psectionexitcntr;

/************* physical detector data *******************************************/

unsigned int * Physicaldetectordata;
unsigned int   Physicalupdate_seq;

/*********** point status pointer ***********************/

unsigned int * Pointstatusflag;
unsigned int   Pointstatusupdate_seq;

/************** slot details **************************************************/

unsigned int * DCCslotcode;
unsigned int * DCCslotdirection;
unsigned int * DCCslotspeedcode;
unsigned int   DCClastupdate_seq;

/**************** procedures **************************************************/

void CloseactiveBTsignalsconnection()
{
	CloseBTsignalsconnection();	
}

void ClearGUItextlineMain(int linenumber)
{
	for (int lcc = 0;lcc < GUIMESSAGELINELENGTH;lcc++)
	{
		GUImessagetextlineMain[linenumber].GUIservermessagelineMain[lcc] = '\0';
	}
}

void ClearAllGUItextlinesMain()
{
	for (int lc = 0;lc < MAXGUIMESSAGELINES;lc++)
	{
		ClearGUItextlineMain(lc);
	}
}

void GUIlinetransferMain(int linefrom, int lineto)
{
	//printf("\n\nlinefrom %i lineto %i",linefrom,lineto);

	//printf("\n\nPre-move from line %i :%s",linefrom,GUImessagetextlineMain[linefrom].GUIservermessagelineMain);
	//printf("\nPre-move to line %i :%s",lineto,GUImessagetextlineMain[lineto].GUIservermessagelineMain);

	for (int lcc = 0;lcc < GUIMESSAGELINELENGTH;lcc++)
	{
		if ((GUImessagetextlineMain[linefrom].GUIservermessagelineMain[lcc] != '\0') && (GUImessagetextlineMain[linefrom].GUIservermessagelineMain[lcc] != '\n'))
		{
		//printf("\n\nmoving char in hex %02X %c",GUImessagetextlineMain[linefrom].GUIservermessagelineMain[lcc],GUImessagetextlineMain[linefrom].GUIservermessagelineMain[lcc]);
		GUImessagetextlineMain[lineto].GUIservermessagelineMain[lcc] = GUImessagetextlineMain[linefrom].GUIservermessagelineMain[lcc];
		}
		else
		{
			GUImessagetextlineMain[lineto].GUIservermessagelineMain[lcc] = '\n';
			break;
		}
	}

	//printf("\n\nPost-move from line %i :%s",linefrom,GUImessagetextlineMain[linefrom].GUIservermessagelineMain);
	//printf("\nPost-move to line %i :%s",lineto,GUImessagetextlineMain[lineto].GUIservermessagelineMain);
}

void GUIAllLineshiftdownMain()
{
	// moves all lines down one row in the GUI table

	int linenumber = 3;

	for (int lcm = 0;lcm < MAXGUIMESSAGELINES - 1;lcm++)
	{
		GUIlinetransferMain(linenumber-1,linenumber);
		linenumber--;
	}
}

void InsertGUItextlineMain(char* GUImessage)
{
	GUIAllLineshiftdownMain();

	time_t rawtime;

	struct tm *info;
	char buffer[80];

	time( &rawtime );

	info = localtime( &rawtime );

	strftime(buffer,80,"%X", info);

	//printf("GUI Formatted date & time : %s\n", buffer );

	int mcc = 0;

	int GUIcc = 0;

	for (int gubf = 0;gubf < GUIMESSAGELINELENGTH;gubf++)      // clear the GUI message line
	{
	//	GUImessagetextlineMain[0].GUIservermessagelineMain[gubf] = ' ';
	}

	//printf("\n539 GUImessageline-> %01d %s",1,GUImessagetextlineMain[1].GUIservermessagelineMain);

	for (int tlcts = 0;tlcts < 8;tlcts++)						// move in the time stamp
	{
		GUImessagetextlineMain[0].GUIservermessagelineMain[GUIcc] = buffer[tlcts];
		GUIcc++;
	}

	GUIcc++;

	for (int tlc = 0;tlc < GUIMESSAGELINELENGTH;tlc++)
		{
			if ((*(GUImessage + mcc) != '\n')&& (*(GUImessage + mcc) != '\0'))
			{
				GUImessagetextlineMain[0].GUIservermessagelineMain[GUIcc] = *(GUImessage + mcc);
				mcc++;
				GUIcc++;
			}
			else
			{
				GUImessagetextlineMain[0].GUIservermessagelineMain[GUIcc] = '\n';
				break;
			}
		}

		//printf("\n564 GUImessageline->%01d %s",1,GUImessagetextlineMain[1].GUIservermessagelineMain);

	    //printf("\n566 GUIcc %i  mcc %i", GUIcc,mcc);

		return;
}

void GUIAllLinesdisplayMain()
{
	for (int gcf = 0;gcf < MAXGUIMESSAGELINES;gcf++)
	{
	printf("\nGUImessageline %01d %s",gcf,GUImessagetextlineMain[gcf].GUIservermessagelineMain);
	//std::cout<<"\nGUIline0:"<<GUImessagetextline[0].GUIservermessageline;
	}
}

void GUIAllLinesinitMain()
{
	for (int gcf = 0;gcf < MAXGUIMESSAGELINES; gcf++)
	{
		sprintf(GUImessagetextlineMain[gcf].GUIservermessagelineMain,"GUI text line Main %01d\n",gcf);
	}
}

int GUIAllmsglinesgetMain(char* replymessage)
{
	char outputrec[HEAPDISPLAYBUFFERLENGTH];

	int ic,isc,mcc;

	mcc = 0;

	bzero(outputrec,HEAPDISPLAYBUFFERLENGTH);

	for (int GUIlc = 0; GUIlc < MAXGUIMESSAGELINES;GUIlc++)
	{
		sprintf(outputrec,">%s",GUImessagetextlineMain[GUIlc].GUIservermessagelineMain);

		isc =  std::strlen(outputrec);

		for (ic = 0;ic <isc;ic++)
		{
			*(replymessage + mcc) = outputrec[ic];
			mcc++;
		}
	}

	return mcc;
}


void Linksequencereset()
{

	DCClastupdate_seq 		= 0;
	PSlastupdate_seq 		= 0;
	DCClastupdate_seq 		= 0;
	Pointstatusupdate_seq 	= 0;
}

void CSwitchmsgprocess(int pt, int OC, int calledby, unsigned char DCCcode[4])
{


	Switchcmdprocess(pt,OC,DCCcode);
}

void USB0Serialmsgprocess(int mlen,unsigned char msgcode[16])
{	
	PiUARTWritemessage(mlen,msgcode);
}

void ResetTF1Rpin()
{
	ResetTF1R();
}

void ResetTF2Rpin()
{
	ResetTF2R();
}

void ResetReedswitchpin()
{
	ResetTTendstopreedswitch();
}

void L293pwmhighpin()
{
	L293pwmhigh(1024);	
	std::cout<<"\n388 L293high called with speed 1024";
}

void L293pwmlowpin()
{
	L293pwmlow();
}

void L293IN4highpin()
{
	L293IN4high();
}

void L293IN4lowpin()
{
	L293IN4low();
}

void L293IN3highpin()
{
	L293IN3high();
}

void L293IN3lowpin()
{
	L293IN3low();
}

int RetrieveBTconnectionstatus()
{
	return GetBTconnectionstatus();
}

void BTtestmessagesend(int TTcode)
{
	std::cout<<"\n421 TT code "<<TTcode<<"\n";
	BTtestmessage(TTcode);
}

void TTMotorRunforward(int Mspeed)
{
	L293IN3highpin();
	L293IN4lowpin();
	L293pwmhigh(Mspeed);
	std::cout<<"\n430 L293high called with speed "<<Mspeed;
	
	
}

void TTMotorRunbackward(int Mspeed)
{
	ResetReedswitchpin();		// reset the end stop switch for the motor running backwards
	L293IN3lowpin();
	L293IN4highpin();
	L293pwmhigh(Mspeed);	
	std::cout<<"\n442 L293high called with speed "<<Mspeed;
	
	
}

void TTMotorStop()
{
	L293pwmhigh(0);
	L293IN3lowpin();
	L293IN4lowpin();
	L293pwmlow();
	
}

void LNPsectionconfigload(unsigned char* lineinputdataptr)
{
  //	std::cout<<"\nPSection configloader New buffer->"<<lineinputdataptr;
	linebufferprocessingNG(lineinputdataptr);

}

void DisplayTCDconfig(char* returnmessage)
{
	DisplayTCDdatabufferconfig(returnmessage);
}

void RFidmessageprocess(int RFid_device, char RFidcode[RFIDCHARCOUNT])
{
	LNLSLD->UpdateRFid_detectors(RFid_device, RFidcode);	
}

void IRdetectordataupdate(int* sensorstateptr)
{	      
     LNLSLD->UpdateIRdetectors(sensorstateptr);
}

int DCCslotdatafetchcall(int DCCcode)
{
	int DCCslotfetch = DCCslotdatafetch(DCCcode);


	if (DCCslotfetch == 0)
	{
		std::cout<<"\n540 Slot not found for DCC code "<<DCCcode<<" \n";
		return 0;
	}
	else
	{
		return DCCslotfetch;
	}
}

void DCCslotdatareload()
{
	DCCslotdatatransfer(0);
}

void DCCslotdatatransfer(int slotloadRC)
{
	int locotableDCC = 0;

	int locospeedtableindex = 0;

	int locoDCCslotnumber = 0;

	if (slotloadRC == 0)
	{
		std::cout<<"\nDCC slot polling completed so update LocoAllData tables\n";

		for (int scss = 1;scss < MAXNUMBEROFLOCOMOTIVES;scss++)
		{
			locotableDCC = LNlocodatatables->GetlocoDCCcode(scss);

			if (locotableDCC > 0)
			{
				locoDCCslotnumber = DCCslotdatafetch(locotableDCC);

				if (locoDCCslotnumber > 0)
				{
					LNlocodatatables->PutlocoDCCslotnumber(locoDCCslotnumber, scss);  // Update loco Alldata tables

					locospeedtableindex = LNlocodatatables->Getlocospeedtablecode(locotableDCC);

					LNRtimeallrecsNG->StoreDCCslotcode(locotableDCC, locoDCCslotnumber,locospeedtableindex); // Update Loco journey records

					// Update speed table index


				}
				else
				{
					std::cout<<"\n589 Slot not found for DCC code "<<locotableDCC<<"\n\n";
				}
			}
			else
			{

			}
		}
	}
	else
	{
		std::cout<<"\n\n601 No slot data available so store dummy (99) values\n";

		for (int scss = 1;scss < MAXNUMBEROFLOCOMOTIVES;scss++)
		{
			locotableDCC = LNlocodatatables->GetlocoDCCcode(scss);

			if (locotableDCC > 0)
			{
				locoDCCslotnumber = 99;

				LNlocodatatables->PutlocoDCCslotnumber(locoDCCslotnumber, scss);  // Update loco Alldata tables

				locospeedtableindex = LNlocodatatables->Getlocospeedtablecode(locotableDCC);

				LNRtimeallrecsNG->StoreDCCslotcode(locotableDCC, locoDCCslotnumber,locospeedtableindex); // Update Loco journey records
			}
		}

	}
}


void Ctriggerlogicalupdate(int value)
{
    // LNLSLD->CcallLogicalsectionupdate();
    
    //std::cout<<"\n904 Ctriggerlogicalupdate called ";

	 RWPprocesslock = 1;				// set the process lock to avoid contention

	 RWtoPSectionUpdate();				// manage move of raw physical data to managed physical data

     LNLSLD->LogicalSectionTcodeRefresh();

     LNLSLD->LogicalDetectorupdateDetectorstatus();

   // LNLSLD->RefreshLastupdatedetails(1);
   
     //std::cout<<"\n\n\n607 Section data update trigger of loco refresh ";

     LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover    

     RWPprocesslock = 0;				// Release the process lock

}

void UARTdataupload(unsigned char InPSbytes[CPROGUPLOADBYTECNT], int Charcount)
{
	//std::cout<<"\n902 Pi UART msg:"<<InPSbytes<<"\n";
	
	//return;
	 		 
    int cc = 0;
    
    for (cc = 0; cc < Charcount; cc++)
    {
		if (InPSbytes[cc] == '#') 
		{
			UARTSoMflag = 1;
			UARTEoMflag = 0;
			UARTbytesreceived = 0;
		}
		
		UARTRxdatabuffer[UARTbytesreceived] = InPSbytes[cc];			
		UARTbytesreceived++;	
		
		if ((InPSbytes[cc] == '\n') && (UARTSoMflag == 1))
		{
			//std::cout<<"\n922 - UART msg recd:"<<UARTRxdatabuffer<<"\n";
			UARTEoMflag = 1;
			UARTSoMflag = 0;
			
			//printf("\nUARTbuff:%02X %02X %02X %02X %02X %02X %02X %02X %02X ",UARTRxdatabuffer[0],UARTRxdatabuffer[1],UARTRxdatabuffer[2],UARTRxdatabuffer[3],UARTRxdatabuffer[4],\
			UARTRxdatabuffer[5],UARTRxdatabuffer[6],UARTRxdatabuffer[7],UARTRxdatabuffer[8],UARTRxdatabuffer[9]);
			
			// parse the message 
			
			unsigned char * UARTRxdatabufferptr = &UARTRxdatabuffer[0];
			
			int charcounter = sizeof(UARTRxdatabuffer);
			
			//std::cout<<"\nSize of UART message:"<<charcounter<<"\n";			
			
			char   msgarray[UARTDATABUFFERSIZE];
			
			char * msgarrayptr = &msgarray[0];			
			
			char direction = UARTRxdatabuffer[6];
			char speed = UARTRxdatabuffer[4];
			char section = UARTRxdatabuffer[2];

			LNLSLD->UpdatePWMcontroller(1, speed, direction, 0);		// Store controller 1 value in Manual controller array			
			
			if (LNLSLD->GetPWMsectionmode() > 0)
			{
			//	PiUartPWMSerialTransmission(0,speed,direction,0);		// if PWMmode is manual transmit results to PWM sections
				LNLSLD->UpdateAllPWMsections(speed, direction);
			}					
		}	
	}
	
	
	if (UARTbytesreceived == (UARTDATABUFFERSIZE-1))
	{
		std::cout<<"\n960 UART buffer overrun:"<<UARTRxdatabuffer;
		std::cout<<"\n961 UART buffer cleared:";
		bzero(UARTRxdatabuffer,UARTDATABUFFERSIZE);
		UARTbytesreceived = 0;
	}	
}

void PiUartPWMSerialTransmission(int section, int speed, int direction, int mode)
{
	char PWMsection = section & 0xFF;
	char PWMspeed = speed & 0xFF;
	char PWMdirection = direction & 0xFF;
	char PWMmode = mode & 0xFF;

	char PiUARToutserialmessage[UARTDATABUFFERSIZE];
	bzero(PiUARToutserialmessage,UARTDATABUFFERSIZE);

	sprintf(PiUARToutserialmessage,"#;%c;%c;%c;%c",PWMsection,PWMspeed,PWMdirection,PWMmode);

	//printf("\n1020 UART output section:%04X speed:%04X direction:%02X %02X",PWMsection,PWMspeed,PWMdirection,PWMmode);

	unsigned char PiUARTserialtransmission[UARTDATABUFFERSIZE];

	for (int ccm = 0; ccm < 9;ccm++)
	{
		PiUARTserialtransmission[ccm] = static_cast<unsigned char>(PiUARToutserialmessage[ccm]);
	}

	PiUARTWritemessage(9,PiUARTserialtransmission);
}

void CDataupdate(unsigned char InPSbytes[CPROGUPLOADBYTECNT], int Charcount)
{

     int cc = 0;
     int scc = 0;

     //USBbytesclear();

   //  std::cout<<"\nUploaded char count->"<<Charcount<<" Current USB charcount:"<<CurrentUSBbufferbytecount;

   //  for (cc = 0; cc < CPROGUPLOADBYTECNT; cc++)
  //   {
   // 	 USBbytestreamdata[cc] =  InPSbytes[cc];
   // 	 CurrentUSBbufferbytecount++;
   //  }



     if ((CurrentUSBbufferbytecount + Charcount) < USBBYTEMAX)
     {
     	 scc = CurrentUSBbufferbytecount;

     	 for (cc = 0; cc < Charcount; cc++)
     	 {
    	 	 USBbytestreamdata[scc] =  InPSbytes[cc];
    	 	 CurrentUSBbufferbytecount++;
    	 	 scc++;
     	 }
     }
     else
     {
     	 // buffer is full so need to create space by moving all characters up

    	 //  std::cout<<"\nBuffer overrun->"<<Charcount<<" Current USB charcount:"<<CurrentUSBbufferbytecount;

     	  int srcsub = Charcount;
     	  int destsub = 0;

     	  while (srcsub < USBBYTEMAX)
     	  {
     	  	  USBbytestreamdata[destsub] = USBbytestreamdata[srcsub];
     	  	  srcsub++;
     	  	  destsub++;
     	  }

     	  CurrentUSBbufferbytecount = CurrentUSBbufferbytecount - Charcount;

     	 //std::cout<<"\nNew dest ->"<<destsub<<" Updated USB charcount:"<<CurrentUSBbufferbytecount;

     	 // now install data received

     	  scc = CurrentUSBbufferbytecount;

     	  for (cc = 0; cc < Charcount; cc++)
     	 	 {
    	 	 	 USBbytestreamdata[scc] = InPSbytes[cc];
    	 	 	 CurrentUSBbufferbytecount++;
    	 	 	 scc++;
     	 	 }
     }

}

void CSpeedcmdprocess(int callref, int SLCslot, int SLCslotspeed, int SLCslotdirection)
{
	//std::cout<<"\n775 Speed command for Slot:"<<SLCslot<<" Speed:"<<SLCslotspeed<<" direction:"<<SLCslotdirection<<" call ref "<<callref;

	if ((SLCslot != 0) && (SLCslot != 99))		// test to see if there is a valid slot code before call function
	{
		Speedcmdprocess(SLCslot,SLCslotspeed,SLCslotdirection);
	}
	else
	{
	//	std::cout<<"\n783 Speedcmd not sent as slot code "<<SLCslot<<" not valid";
	//	std::cout<<"\n";
	}
}

void CPointDCCcodeupdate(int ptc, int OC, unsigned char CodeDCC[4])
{
	PointDCCcodeupdate(ptc,OC,CodeDCC);
}

void CDisplaySCPointDCC()
{
	DisplaySCPointDCC(0);
}

void CAllslotsstop()
{
	Allslotsstop();
}

void InitTCDbuffers()
{
	int tc = 0;
	
	for (tc = 0;tc < MAXTCDBUFFERS;tc++)
	{		
		TCDbuffers[tc] = 0x0;		
	}
}

void Psectiondataconfigclear()
{
	// Reset all Psection data config values to zero
	
	int ps = 0;
	
	for (ps = 0;ps < MAXPSECTIONS;ps++)
	{
		Psectiondataconfig[ps] = 0;		
	}	
}

int PSectionset(int sc, int value)
{

	//std::cout<<"\n720 -> PSsectionset "<<sc<<" "<<value;

	if ((sc > 0) && (sc < MAXPSECTIONS) && (value < 3))
	{
		Psectionserialdata[sc] = value;
		RWPsectionserialdata[sc] = value;
		return 0;
	}
	else
	{
		return 99;
	}
}

void BCMconnectordataReset()
{
	for (int bcmc = 0; bcmc < BCMTRACKCIRCUITCONNECTORS;bcmc++)
	{
		BCMconnectordata[bcmc] = 0xFF;
	}
}

void PSectionreset()
{
	for (int sc = 0;sc < MAXPSECTIONS;sc++)
	{
		Psectionserialdata[sc] = 0;
		RWPsectionserialdata[sc] = 0;

	//	std::cout<<"\nSect "<<sc<<" Timer data E:"<<RWPTEsectionserialdata[sc]<<" X:"<<RWPTXsectionserialdata[sc]<<" A:"<<RWPTCsectionserialdata[sc];
	}
}

void Sectioncounterset(unsigned int ec, unsigned int xc)
{
	*Psectionentrycntrptr = ec;

	*Psectionexitcntrptr = xc;
}

void USBbytecounterset(unsigned int bc)
{
	*USBbytesreadcntrptr = bc;

	CurrentUSBbufferbytecount = bc;
}

void USBbytesclear()
{
	for (int sc = 0;sc < USBBYTEMAX;sc++)
	{
		USBbytestreamdata[sc] = 0x00;
	}
}


void InitializeTimestamp()
{
	int ts;

	for (ts = 0; ts < NUMBEROFTIMESTAMPS; ts++)
	{
		MTS[ts].HH[0] = '1';
		MTS[ts].HH[1] = '2';

		MTS[ts].MN[0] = '3';
		MTS[ts].MN[1] = '4';

		MTS[ts].SS[0] = '5';
		MTS[ts].SS[1] = '6';

		MTS[ts].MLS[0] = '0';
		MTS[ts].MLS[1] = '0';
		MTS[ts].MLS[2] = '0';

		MTS[ts].Seqnumber = 0;
	}

}

void SetTimestamp(int offset, int sa, MsgTimestamp* MSGptr )
 {
	 /*
	 struct MsgTimestamp
	{
	int Seqnumber;
	unsigned char HH[2];    // 00
	unsigned char MN[2];    // 00
	unsigned char SS[2];    // 00
	unsigned char MLS[3];   // 000
	int Timestampvalue;
	};
	*
	* char CTDatetime[DATETIMESTRING]

	*/
	char CTDatetime[DATETIMESTRING] = "Not set ";

	char* CTDatetimeptr = &CTDatetime[0];

	Clocktime CT;

	CT.Get_time(*CTDatetimeptr);

	//std::cout<<"\nTime stamp->"<<CTDatetime<<"\n";

	int h1,h2,m1,m2,s1,s2,ht,mt,st,tt;


		(MSGptr + offset)->HH[0] = *(CTDatetimeptr + 11);
		(MSGptr + offset)->HH[1] = *(CTDatetimeptr + 12);

		(MSGptr + offset)->MN[0] = *(CTDatetimeptr + 14);
		(MSGptr + offset)->MN[1] = *(CTDatetimeptr + 15);

		(MSGptr + offset)->SS[0] = *(CTDatetimeptr + 17);
		(MSGptr + offset)->SS[1] = *(CTDatetimeptr + 18);


		if (sa == 0)
		{
			(MSGptr + offset)->Seqnumber = 0;   // reset sequence number
		}

		if (sa == 1)
		{
			(MSGptr + offset)->Seqnumber++;    // increment sequence number
		}

		h1 = (int)((MSGptr + offset)->HH[0]) - 48;
		h2 = (int)((MSGptr + offset)->HH[1]) - 48;

		ht = (h1 * 10) + h2;

		m1 = (int)((MSGptr + offset)->MN[0]) - 48;
		m2 = (int)((MSGptr + offset)->MN[1]) - 48;

		mt = (m1 * 10) + m2;

		s1 = (int)((MSGptr + offset)->SS[0]) - 48;
		s2 = (int)((MSGptr + offset)->SS[1]) - 48;

		st = (s1 * 10) + s2;

		tt = (ht * 3600) + (mt * 60) + st;

		(MSGptr + offset)->Timestampvalue = tt;

	//	printf("\nVals tt %i ht %02d mt %02d st %02d", tt,ht,mt,st);
 }

void GetBCMconnectordata(char* returnmessage)
{
	char TCmessage[SHORTMESSAGELENGTH];

	bzero(TCmessage,SHORTMESSAGELENGTH);

	// fetch BCMconnector data from LNserial

	unsigned char* BCMconnectorptr = &BCMconnectordata[0];

	GetBCMrawdata(BCMconnectorptr);

	int ic,cc;

	cc = 0;

	sprintf(TCmessage,"%02X:%02X:%02X:%02X",BCMconnectordata[0],BCMconnectordata[1],BCMconnectordata[2],BCMconnectordata[3]);

	int ms = strlen(TCmessage);

	for (ic = 0;ic < ms;ic++)
	{
		*(returnmessage + cc) = TCmessage[ic];
		cc++;
	}
		*(returnmessage + cc) = ':';
		cc++;
		*(returnmessage + cc) = '\n';

		return;
}

void GetTCDbufferdata(char* returnmessage)
{
	char TCmessage[SHORTMESSAGELENGTH];

	bzero(TCmessage,SHORTMESSAGELENGTH);

	int ic,cc;

	cc = 0;	
	
	sprintf(TCmessage,"TCD:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",TCDbuffers[0],TCDbuffers[1],TCDbuffers[2],TCDbuffers[3],TCDbuffers[4],\
		TCDbuffers[5],TCDbuffers[6],TCDbuffers[7]);

	int ms = strlen(TCmessage);

	for (ic = 0;ic < ms;ic++)
	{
		*(returnmessage + cc) = TCmessage[ic];
		cc++;
	}
		*(returnmessage + cc) = ':';
		cc++;
		*(returnmessage + cc) = '\n';
		
		return;
}


void GetSerialInterfacecntrs(char* returnmessage)
{
	char PSmessage[SHORTMESSAGELENGTH];

	bzero(PSmessage,SHORTMESSAGELENGTH);

	int ic,cc;

	cc = 0;

	/*
	unsigned int * USBbytesreadcntrptr;

    ************* physical section data ******

	unsigned int * Psectionserialdata;
	unsigned int   PSlastupdate_seq;

	unsigned int * Psectionentrycntrptr;
	unsigned int * Psectionextitcntrptr;

	*/



	sprintf(PSmessage,"USBCNTS:%05d:%05d:%05d:00000",*USBbytesreadcntrptr,*Psectionentrycntrptr,*Psectionexitcntrptr);

	int ms = strlen(PSmessage);

	for (ic = 0;ic < ms;ic++)
	{
		*(returnmessage + cc) = PSmessage[ic];
		cc++;
	}
		*(returnmessage + cc) = ':';
		cc++;

		// USBbytestreamdata[sc]

	bzero(PSmessage,SHORTMESSAGELENGTH);

	int bss;

	bss = 0;

	for (bss = 0;bss < USBBYTEMAX;bss++)
	{
		bzero(PSmessage,SHORTMESSAGELENGTH);

		sprintf(PSmessage,"%02X ",USBbytestreamdata[bss]);

		ms = strlen(PSmessage);

		for (ic = 0;ic < 3;ic++)
		{
			*(returnmessage + cc) = PSmessage[ic];
			cc++;
		}
	}
	*(returnmessage + cc) = ':';
}

int UARTRXdatamessage(int bytecount, char* returnmessage)
{
	
	int ic = 0;
	
	int cc = 0;
	
	for (ic = 0;ic <UARTbytesreceived ;ic++)
	{
		*(returnmessage + cc) = UARTRxdatabuffer[ic];
		cc++;
	}
	
	return UARTbytesreceived;	
}

int PSectiondatareturn(int PS, char* returnmessage)
{
	/*
	 * if PS = 0, return all sections
	 * else return section details
	 */

	char sectioninfo[5];

	//char aChar = '0';// + i;
	int psc = 0;
	int cc = 0;

	int ic;

	Rtnmsgbuffclear();

	if (PS == 0)
	{
		//std::cout<<"\n1108 fetching ALL Psection data ";
		//printf("\n");

		for (psc = 1; psc < MAXPSECTIONS;psc++)
		{
			sprintf(sectioninfo,"%03d-%01d",psc,Psectionserialdata[psc]);
		//	printf("\n1118 - psc: %03d info %s",psc,sectioninfo);
			for (ic = 0;ic <5;ic++)
			{
				*(returnmessage + cc) = sectioninfo[ic];
				cc++;
			}
			*(returnmessage + cc) = ' ';
			cc++;
		}

		int ccf = (MAXPSECTIONS * 6) + 1;

		*(returnmessage + ccf) = '\n';

		return 0;						// return 0 if all sections returned
	}
	else
	{
		sprintf(returnmessage,"Section %03d",PS);

		//std::cout<<"\n\n1155 PS:"<<PS<<" Data:"<<Psectionserialdata[PS]<<"\n";

		return Psectionserialdata[PS];  // return integer value
	}
}

int Pointstatusdatareturn(int Point, char* returnmessage)
{
	/*
	 * if PS = 0, return all sections
	 * else return section details
	 */

	char aChar = '0';// + i;
	int psc = 0;
	int cc = 0;

	Rtnmsgbuffclear();

	if (Point == 0)
	{
		for (psc = 1; psc <= NUMBEROFPOINTS;psc++)
		{
			*(returnmessage + cc) = aChar + Pointstatusflag[psc];
			cc++;
			*(returnmessage + cc) = ' ';
			cc++;
		}

		int ccf = (NUMBEROFPOINTS * 2) + 1;

		*(returnmessage + ccf) = '\n';

		return 0;						// return 0 if all sections returned
	}
	else
	{
		sprintf(returnmessage,"Point %02d status ",Point);

		return Pointstatusflag[Point];  // return integer value
	}
}

void Pointstatusflagreset()
{
	for (int dc = 0;dc < NUMBEROFPOINTS; dc++)
	{
		Pointstatusflag[dc] = 0;
	}
}

void Devicedetectorreset()
{
	for (int dc = 0;dc < PHYSICALDETECTORS; dc++)
	{
		Physicaldetectordata[dc] = 0;
	}
}

void Setdevicedata(int dd)
{
	Physicaldetectordata[dd] = 1;
}

int Devicedatareturn(int dd, char* returnmessage)
{
	char aChar = '0';// + i;
		int dsc = 0;
		int cc = 0;

		Rtnmsgbuffclear();

		if (dd == 0)
		{
			for (dd = 0; dd<= PHYSICALDETECTORS;dd++)
			{
				*(returnmessage + cc) = aChar + Physicaldetectordata[dd];
				cc++;
				*(returnmessage + cc) = ' ';
				cc++;
			}

			int ccf = (PHYSICALDETECTORS * 2) + 1;

			*(returnmessage + ccf) = '\n';

			return 0;						// return 0 if all detectors returned
		}
		else
		{
			for (dsc = 0; dsc <= PHYSICALDETECTORS;dsc++)
			{
				*(returnmessage + cc) = ' ';
				cc++;
				*(returnmessage + cc) = ' ';
				cc++;
			}

			*(returnmessage + 0) = aChar + Physicaldetectordata[dd];

			*(returnmessage + 1) = '\n';

			return Physicaldetectordata[dd];  // return integer value
		}

}

void UpdateSlotdata(int sc,int scspeed,int scdirection)
{
	DCCslotspeedcode[sc] = scspeed;
	DCCslotdirection[sc] = scdirection;
}

void Slotsreset()
{
	for (int sc = 0;sc < NUMBEROFDCCSLOTS;sc++)
	{
		DCCslotcode[sc] 		= 0;
		DCCslotdirection[sc] 	= 0;
		DCCslotspeedcode[sc]	= 0;
	}
}

int Findslotcode(unsigned int dccvalue)
{
	int NullDCCcode = 0;

	for (int sc = 0;sc < NUMBEROFDCCSLOTS;sc++)
	{
		if (dccvalue == DCCslotcode[sc])		// check to see if DCC code is already stored in one of the slots
		{
			return sc;							// if found, then return the slot number
		}
	}

	return NullDCCcode;
}

void Rtnmsgbuffclear()
{
	int cc = 0;

	int ccmax = (DISPLAYBUFFERLENGTH * 2) + 1;

	for (cc = 0;cc < ccmax; cc++)
	{
		PResults[cc] = '\0';
	}
}

void Slotdatareturn(int slot, char* returnmessage)
{
	int sc = 0;

	// format CCCC-SSS-D:

	int DDCmsglen = 14;

	Rtnmsgbuffclear();

	int msgcc = 0;

	char DDCmsg[DDCmsglen];

	if (slot == 0)
	{
		for (sc = 1;sc < NUMBEROFDCCSLOTS;sc++)
		{
			sprintf(DDCmsg,"#%02d-%04d-%03d-%01d:",sc, DCCslotcode[sc], DCCslotspeedcode[sc],DCCslotdirection[sc]);

			for (int cm = 0;cm < DDCmsglen;cm++)
			{
				*(returnmessage + msgcc) = DDCmsg[cm];
				msgcc++;
			}
		}
		*(returnmessage + msgcc) = '\n';
	}
	else
	{
		sprintf(DDCmsg,"%02d-%04d-%03d-%01d:",sc,DCCslotcode[slot], DCCslotspeedcode[slot],DCCslotdirection[slot]);

		for (int cm = 0;cm < DDCmsglen;cm++)
			{
				*(returnmessage + msgcc) = DDCmsg[cm];
						msgcc++;
			}
		*(returnmessage + msgcc) = '\n';
	}
}

void Signalcommandsend(int mlen, unsigned char* Sigcmd)
{
	unsigned char Sigmsgout[7];

	//std::cout<<":\n917 Sig command to send:"<<Sigcmd;

	int cc;

	for (cc = 0;cc < 7;cc++)
	{
		Sigmsgout[cc] = *(Sigcmd + cc);
	}	
	
	//UARTmessagesent = 1;									// set flag to indicate message sent to UART
	
	//bzero(UARTRxdatabuffer,UARTDATABUFFERSIZE);					// clear character received buffer 
	
	//UARTbytesreceived = 0;

	//std::cout<<"\n1178 Sig text msgout:"<<Sigmsgout;

	//USB0Serialmsgprocess(mlen,Sigmsgout);
	
	BTsignalmessage(Sigmsgout);
}

void TTcommandsend(int mlen, unsigned char* TTcmd)
{
	unsigned char TTmsgout[7];

	//std::cout<<":\n917 TT command to send:"<<TTcmd;

	int cc;

	for (cc = 0;cc < 7;cc++)
	{
		TTmsgout[cc] = *(TTcmd + cc);
	}
	
	//TTmsgout[5] = '\0';
	
	UARTmessagesent = 1;									// set flag to indicate message sent to UART
	
	bzero(UARTRxdatabuffer,UARTDATABUFFERSIZE);					// clear character received buffer 
	
	UARTbytesreceived = 0;

	//std::cout<<"\n926 TT text msgout:"<<TTmsgout;

	USB0Serialmsgprocess(mlen,TTmsgout);
}


void SignalRelayrefresh(LNwebLsectionsLdevices* LNLSLD)
{
	
	LNLSLD->UpdateSignalRelayoutputcode();
		 
	if (LNLSLD->GetCurrrelaysignalcode() != LNLSLD->GetPrevrelaysignalcode())
		{		
	
		 unsigned char msg[7];
		 				
		 msg[2] = 	0x01;	
		 msg[3] = 	0X01;
		 msg[4] = 	0X01;		 			
		 				
		 msg[0] = 	'C';	
		 msg[1] = 	0X00;			 				
		 msg[5] = 	LNLSLD->GetCurrrelaysignalcode();
		 msg[6] = 	0X0D;			 
		 					 				
		 int mlen = 7; 				 				
		 				
		 USB0Serialmsgprocess(mlen,msg);
		 
		 LNLSLD->Updateprevsignalrelaycode();	 
		 
		 //printf("\nSignal relay code sent %02X \n\n", LNLSLD->GetCurrrelaysignalcode());		 
		}			
}

void SerialUSB0commsthread(unsigned int * Psectionsptr,unsigned int * PDetectorptr, unsigned int * DCCslotcodeptr, \
		unsigned int * DCCslotdirectionptr,unsigned int * DCCslotspeedcodeptr, unsigned int * Pointstatusflagptr, \
		MsgTimestamp* MSGptr,unsigned int *PsectionsEptr,\
		unsigned int * PsectionsXptr, unsigned int * USBbytecntptr, unsigned char * UARTbytebufferptr, \
		unsigned int * RWTEPsectionsptr,unsigned int * RWTXPsectionsptr,unsigned int * RWTCPsectionsptr,LNsignalprocessing* LNsigprocess)
{
   	//Serialcomms(Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr, DCCslotspeedcodeptr);
   	
   	UARTmessagesent = 0;

   	SerialUSB0comms(Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr, DCCslotspeedcodeptr, Pointstatusflagptr, MSGptr, PsectionsEptr,\
   			PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr);

	//  C call - int Serialcomms(unsigned int* InPsectionptr, unsigned int* InPDetectorptr,
   	//  unsigned int* InSlotcodeptr, unsigned int* InSlotdirectionptr, unsigned int* InSlotspeedptr)

   	std::cout<<"\nReturning from C thread for USB0 \n";

	return;
}


void Serialcommsthread(LNlogging* LNlogptr, unsigned int * Psectionsptr,unsigned int * PDetectorptr, unsigned int * DCCslotcodeptr, \
		unsigned int * DCCslotdirectionptr,unsigned int * DCCslotspeedcodeptr, unsigned int * Pointstatusflagptr, \
		MsgTimestamp* MSGptr,unsigned int *PsectionsEptr,\
		unsigned int * PsectionsXptr, unsigned int * USBbytecntptr, unsigned char * UARTbytebufferptr, \
		unsigned int * RWTEPsectionsptr,unsigned int * RWTXPsectionsptr,unsigned int * RWTCPsectionsptr)
		{
   	

   	Serialcomms(Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr, DCCslotspeedcodeptr, Pointstatusflagptr, MSGptr, PsectionsEptr,\
   			PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr,PsectionDCCfilenameptr);

	//  C call - int Serialcomms(unsigned int* InPsectionptr, unsigned int* InPDetectorptr,
   	//  unsigned int* InSlotcodeptr, unsigned int* InSlotdirectionptr, unsigned int* InSlotspeedptr)

   	std::cout<<"\nReturning from C thread \n";

	return;
	}

//void TCDdataupdate() 
//{	
	//TCDdatarequest(TCDdataptr,Psectionsptr);	
	
	/* Update TT indicators  *******************/ 
	
	//LNttprocess->TTcontrolswitchprocessing(TCDdataptr);		
//}

void LNwebmonConsole(LNlogging* LNlogptr, LNConsoleprocess* CP, bool *Tflagptr, LNwebLsectionsLdevices* LNLSLD, \
		MsgTimestamp* MSGptr,LNweblocorouteprocess* LNLRprocess, \
		LNdisplayconfig* LNDC,LNRFidsensorprocessing* LNRFIDSP, LNRouteprocess* LNRteproc, \
		LNRteLogicalsectionData* LNRteLSi,LNRteLogicalsectionGroup* LNRteLSG, LNRteDevicereferencedata* LNRteDRfd, \
		LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNwebmonSimulator* LNSIM,	\
		LNSimulatorAllRoutes* LNSimAllRtes, LNSimRouteMaster* LNSimRteMaster,LNSchedulerAlljourneys* LNSchedAlljourneys,LNSchedulerExecutor* LNSchedExecutor,\
		LNloadRteTrainSchedfile* LNRteTrainScheduledata,LNallTrains* LNallTrainsinformation, LNRouteSchedMaster* LNRteSchedMast,\
		LNturntableprocess* LNttprocess, LNsignalprocessing* LNsigprocess, LNcommands* LNcommandsptr,LNlocoAlldata* LNlocodatatables)
{
	std::cout <<"\nStarting thread for the LNwebmon console....\n";

	int consolerun = 0;

	while (consolerun == 0)
	{
		consolerun = CP->Consoleprompt(LNlogptr,Tflagptr,\
				LNLRprocess,\
				LNDC,\
				LNLSLD,\
				LNRFIDSP,\
				LNRteproc,\
				LNRteLSi,\
				LNRteLSG,\
				LNRteDRfd,\
				LNRtimeallrecsNG,\
				LNSIM,\
				LNSimAllRtes,\
				LNSimRteMaster,\
				LNSchedAlljourneys,\
				LNSchedExecutor,\
				LNRteTrainScheduledata,\
				LNallTrainsinformation,\
				LNRteSchedMast,\
				LNttprocess,\
				LNsigprocess,\
				LNcommandsptr,\
				LNlocodatatables);
	}

	std::cout <<"\nExiting the thread for the LNwebmon console...\n";
}

void LNwebSimulatorexecute(LNwebmonSimulator* LNSIM, LNwebLsectionsLdevices* LNLSLD, MsgTimestamp* MSGptr,bool *Tflagptr,LNweblocorouteprocess* LNLRprocess,\
		LNRouteprocess* LNRteproc,LNSimulatorAllRoutes* LNSimAllRtes,LNSimRouteMaster* LNSimRteMaster)
{
	std::cout <<"\nStarting thread for Simulator....\n";

	while (1)
	{
		LNSIM->Simprocessing(MSGptr,LNLSLD,LNLRprocess,LNSimAllRtes,LNSimRteMaster);
	}
}

void LNwebSchedulerexecutor(LNSchedulerExecutor* LNSchedExecutor,LNweblocorouteprocess* LNLRprocess,\
		LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNwebmonSimulator* LNSIM,LNSimulatorAllRoutes* LNSimAllRtes,LNSimRouteMaster* LNSimRteMaster, \
		LNSchedulerAlljourneys* LNSchedAlljourneys,LNwebLsectionsLdevices* LNLSLD, LNRouteSchedMaster* LNRteSchedMast)
{

	LNSchedAlljourneys->InitializeScheduleStart(LNRtimeallrecsNG, LNLRprocess, LNLSLD, LNRteSchedMast);

	std::cout <<"\nStarting thread for Scheduler execution....\n";

	while (1)
	{
		LNSchedExecutor->RunScheduler(LNSchedExecutor,LNLRprocess,LNRtimeallrecsNG, LNSIM,LNSimAllRtes,LNSimRteMaster,LNSchedAlljourneys,LNLSLD);
	}
}

void LNwebtimerprocessexecute(LNwebmonSimulator* LNSIM, LNwebLsectionsLdevices* LNLSLD, MsgTimestamp* MSGptr,bool *Tflagptr,LNweblocorouteprocess* LNLRprocess)
{
	std::cout <<"\nStarting thread for Timer 1 processing on activities\n";
	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	
	
	ResetTF1Rpin();

	ResetTF2Rpin();

	ResetReedswitchpin();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(25));

		//std::cout <<"\nTimer thread TICK\n";

		RWtoPSectionTimerUpdate();				// manage move of raw physical data to managed physical data

		STPDcheckloop();						// Check to see if there are any STPD commands
	}
}

void LNwebtimer2processexecute(LNwebmonSimulator* LNSIM, LNwebLsectionsLdevices* LNLSLD, MsgTimestamp* MSGptr,bool *Tflagptr,LNweblocorouteprocess* LNLRprocess, LNsignalprocessing* LNsigprocess)
{
	std::cout <<"\nStarting thread for Timer 2 processing on activities\n";
	
	BTsignal_linkinitialize();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(250));		
		
		//SignalRelayrefresh(LNLSLD);
		
		LNsigprocess->SignalChangedCheck();
		
		LNLSLD->RefreshsectionPWMcontrollers();		// Refresh PWM controllers to update speed and directions		

	//	LNLRprocess->LNlocosectionrefresh();

		//std::cout <<"\nTimer thread 2 TICK\n";
	}
	
}


void LNwebserverexecute(LNlogging* LNlogptr, LNwebmonSimulator* LNSIM,LNwebLsectionsLdevices* LNLSLD, \
		MsgTimestamp* MSGptr,bool *Tflagptr,LNweblocorouteprocess* LNLRprocess, \
		LNdisplayconfig* LNDC,LNRFidsensorprocessing* LNRFIDSP, LNRouteprocess* LNRteproc,LNSimRouteMaster* LNSimRteMaster, \
		LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,LNSchedulerAlljourneys* LNSchedAlljourneys, \
		LNRteDevicereferencedata* LNRteDRfd,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG,LNsignalprocessing* LNsigprocess,LNlocoAlldata* LNlocodatatables)
{
	printf("\nStarting thread for the LNwebserver = port 8091");

	LNwebserver* LNWS = new LNwebserver();

	int rc;

	while (1)
	{
	  rc = LNWS->WebServerrun(LNlogptr,LNLRprocess,LNSIM, 8091,LNLSLD,MSGptr,LNDC,LNRFIDSP,LNRteproc,LNSimRteMaster,LNSchedExecutor,\
			  LNRteSchedMast,LNSchedAlljourneys,LNRteDRfd,LNRtimeallrecsNG,LNlocodatatables,Tflagptr);

	  if (rc > 0)
	  {
		   std::cout<<"\n\n*******************************";
		   std::cout <<"\n\n      Web Server run error " << rc;
		   std::cout<<"\n\n*******************************";
		  break;
	  }
	}

	 delete LNWS;

	//pthread_exit(0);
}



unsigned char AtoH(char bytein)
{
	int result = 0x0;

	char *byte = &bytein;

	if (*byte > 0x39) /* test to see if it is alphanumeric */
	{
		result = (*byte - 0x41 + 0x0A);
	}
	else
	{
		result = *byte - 0x30;
	}

	return result;
}

char HtoA(unsigned char c)
{
	//char c = 0x0F;

	if (c > 0x0F)
	{
	//printf("\nHtoA source Invalid - 1\n");
	return 0;
	}

	if (c < 0x0A)
	{
	//printf("\nHtoA source OK - 1 %X \n",c);
	return c + 0x30;
	}

	if (c > 0x09)
	{
	//printf("\nHtoA source OK - 2 %X \n", c);
	return c + 0x37;
	}

	return 0;
}

unsigned char StringtoH(unsigned char *charinput)  	/* Converts string character char to hex byte */
{											/* e.g  "A1" to A1 as hex character           */
											/* String will be format 4130                 */
	unsigned char HC = 0;
	unsigned char HC1 = 0;
	unsigned char *cp = charinput;
	char cps = *cp;

	/* first character */

	HC = AtoH(cps);
	HC = HC * 16;

	cps = *(cp + 1);
	HC1 = AtoH(cps);

	return (HC + HC1);
}

void STPDcheckloop()						// check to see if there are any pending STPD commands
{
	if (STPDprocessflag > 0)
	{
		LNLRprocess->LNSTPDprocessing();   // does check processing for any stop command that is linked to a trigger device e.g reed switch.
	}

}

void RWtoPSectionTimerUpdate()				// manage move of raw physical data to managed physical data
{
	int ct;
	
	RWPcounter++;	
	
	//#ifdef PIRun 						// test to see if compiling for running on the PI  
	
	//TCDdataupdate();					// Refresh track circuit and turntable data status 
	
	//#endif
	
	RWtoPSectionUpdate();				// Update the physical data records 

	if (RWPprocesslock > 0)
	{
		RWPlockcounter++;
		
	//	std::cout<<"\nRWP UPDATE contention Count:"<<RWPcounter<<" Locks:"<<RWPlockcounter;
		
		return;
	}

	int Sectionmoved = 0;

	if (RWPsectionflag > 0)					// test to see if there are any delays to process
	{
		RWPsectionflag = 0;					// Reset the flag and process each section

	//	std::cout<<"\n--> Processing Sections with EX delays\n";

		for (ct = 0;ct < MAXPSECTIONS;ct++)
			{
				if (RWPTCsectionserialdata[ct] > 0)
					{
						RWPTCsectionserialdata[ct]--;

						if (RWPTCsectionserialdata[ct] == 0)
						{

							*(RWPsectionsptr + ct) = *(Psectionsptr + ct);
					//		std::cout<<"\n--> Timer Section "<<ct<<" now cleared:"<<RWPTCsectionserialdata[ct];
							Sectionmoved = 1;

						}
						else
						{
					//		std::cout<<"\n---> Timer Section "<<ct<<" still counting:"<<RWPTCsectionserialdata[ct];
							RWPsectionflag = 1;
						}
					}
					else
					{
						//RWPsectionflag = 1;
					}
			}
	}

	if (Sectionmoved > 0)
	{
		LNLSLD->LogicalSectionTcodeRefresh();

		LNLSLD->LogicalDetectorupdateDetectorstatus();
		   
		std::cout<<"\n1044 - Timer triggered Loco section refresh ";

		LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

	}

}

void RWtoPSectionUpdate()				// manage move of raw physical data to managed physical data
{
	int ct;

	RWPsectionflag = 0;	

	/*for (ct = 0;ct < MAXPSECTIONS;ct++)
	{
		*(RWPsectionsptr + ct) = *(Psectionsptr + ct);

		//if (RWPTCsectionserialdata[ct] > 0)
		//{
		//	std::cout<<"\nSection :"<<ct<<" Timer delay->"<<RWPTCsectionserialdata[ct];
		//	RWPTCsectionserialdata[ct]--;
		//}
	}*/

	

	for (ct = 0;ct < MAXPSECTIONS;ct++)
	{
		if (GetPSectionconfigflag(ct) == 2)    // config 2 means it is a Loconet message section
		{			
			if (RWPTCsectionserialdata[ct] > 0)
			{
		//	std::cout<<"\nFound Section EX delay:"<<ct<<" value:"<<RWPTCsectionserialdata[ct];

				RWPTCsectionserialdata[ct]--;

				if (RWPTCsectionserialdata[ct] == 0)
				{
				*(RWPsectionsptr + ct) = *(Psectionsptr + ct);
				}
				else
				{
			///	std::cout<<"\nHIGH Section EX delay:"<<ct<<" value:"<<RWPTCsectionserialdata[ct];
				RWPsectionflag = 1;
				}
			}
			else
			{			
			//	std::cout<<"\nNO Section EX delay for :"<<ct;
			}
		}
		
		if (GetPSectionconfigflag(ct) == 1)    // config 1 means it is a Loconet hardware direct update section
		{	
			
			//std::cout<<"\n2128 found direct TC update for section:"<<ct;
			
			*(RWPsectionsptr + ct) = *(Psectionsptr + ct);
		}
	}	
}

int  iret1, iret2, iret3, iret4,iret5, iret6, iret7, iret8;

int Configurationloadprocessing(LNconfigloader* LNconfigload,LNlogging* LNlogptr, LNcommands* LNcommandsptr,\
		LNwebLsectionsLdevices* LNLSLD, LNlocoAlldata* LNlocodatatables, LNloadRteTrainSchedfile* LNRteTrainScheduledata, LNallTrains* LNallTrains)
{
	LNcommandsptr->InitializeCmdtable();			// clear the command table

	int RC = 0;

	std::string ConfigurationDatasetname = WEBDirectory + LNconfigfilenamestr;

	int fln = ConfigurationDatasetname.length();

	char Filenamechar_array[fln+1];

	strcpy(Filenamechar_array, ConfigurationDatasetname.c_str());

	if (LNconfigload->Fileread(Filenamechar_array, LNlogptr, LNcommandsptr, LNLSLD, LNlocodatatables,LNRteTrainScheduledata, LNsigprocess, LNallTrains, 0) > 0)
		{
			LNlogptr->Writelogmessage(0,"LNwebmonNGmain - configuration file load error - program shutting down");
			std::cout<<"\n\nLNwebmonNGmain - configuration file load error - program shutting down"<<std::endl;
			RC = 8;
		}
		else
		{
			// processing configuration records read from file

			LNlogptr->Writelogmessage(0,"LNwebmonNGmain - configuration file loaded OK");

		}

	return RC;
}
/**************************************************************************************************/
/*                                                                                                */
/*           MAIN - start of processing                                                           */
/*                                                                                                */
/**************************************************************************************************/

int main(int argc, char *argv[])
{
	/**********************************************************************************************/
	/*    Persistent Objects are created on the heap  - using the 'new' command                   */
	/*                                                                                            */
	/*    Temporary Objects are created on the stack                                              */
	/**********************************************************************************************/
	
	#ifdef PIRun 	
	std::cout << PIPROGRAMVERSION <<"  \n";	
	#else
	std::cout << PROGRAMVERSION <<"  \n";	
	#endif
	
	if (argc < 2)
	{
		std::cout<<"\nERROR - no layout config filename specified - program stops \n";
		exit(1);		
	}
	
	Layoutfilename = argv[1];		
	
	configreloadflag = 0;

	/*   build config and data filenames using layoutname ***************/
	
	
	//std::cout<<"\nStartup check 1 - line 1461\n";
	
	//PointsDCCdatafilenametr 	= Layoutfilename + POINTSDCCDATAFILENAME;    // done
	PointsConfigfilenamestr 	= Layoutfilename + POINTSCONFIGFILENAME;     // done

	Displayconfigfilenamestr	= Layoutfilename + DISPLAYCONFIGFILENAME;	// done
	RFidConfigfilenamestr		= Layoutfilename + RFIDCONFIGFILENAME;		// done
	RouteTrainSchedfilenamestr	= Layoutfilename + RTETRAINSCHEDFILENAME;	// done
	Traininitialdatafilenamestr	= Layoutfilename + TRAININITIALDATAFILENAME; // done
	PSectionDCCcodesfilenamestr = WEBDirectory + Layoutfilename + PSECTIONSDCCFILENAME;	
	
	LNconfigfilenamestr 		= Layoutfilename + ALLCONFIGFILENAME;
	
	//int psfl = PSectionDCCcodesfilenamestr.length();
	
	//char PSfilename_array[psfl + 1];
	
	//strcpy(PSfilename_array,PSectionDCCcodesfilenamestr.c_str());   // setup character array for PsectionDCCcodes filename
		
	//PsectionDCCfilenameptr = &PSfilename_array[0];
	
	LNlogptr = new LNlogging();										// Create the logging object on the heap

	if (LNlogptr->Openloggingfile() > 0)
	{
			std::cout<<"\n\nLogging file open failed - program exiting"<<std::endl;
			exit;
	}

	LNlogptr->Writelogmessage(0,"LNmonitor2024 Ver 10.8 starting object build.........");

	//LNOplogptr = new LNoperationlogging();							// create object for operational logging history

	LNOplatestlogptr = new LNoperationlogging();					// create object for operational logging file - latest snapshot

	if (LNOplatestlogptr->CreateOphistloggingfile() > 0)
	{
		std::cout<<"\n\nOperational Logging file open failed - program exiting"<<std::endl;
		return 0;
	}



	LNConsoleprocess* CP = new LNConsoleprocess();            		// Create the console object on the heap

	bool *Tflagptr = new bool[MAXNUMOFTHREADS];				  		// Create array of bool objects to control the threads

	USBbytestreamdata = new unsigned char[USBBYTEMAX];
	
	Psectiondataconfig = new unsigned int[MAXPSECTIONS];           // Create physical section config storage array
	
	PsectionTCDbufferdata = new unsigned int[MAXPSECTIONS];        // Create physical section Track circuit data array

	Psectionserialdata = new unsigned int[MAXPSECTIONS];     		// Create Psection array data on the heap

	BCMconnectordata = new unsigned char[BCMTRACKCIRCUITCONNECTORS]; // Create BCM based track circuit connectors on heap

	RWPsectionserialdata = new unsigned int[MAXPSECTIONS];     		// Create RWPsection array data on the heap

	RWPTEsectionserialdata = new unsigned int[MAXPSECTIONS];     	// Create RWPsection array entry time data on the heap

	RWPTXsectionserialdata = new unsigned int[MAXPSECTIONS];     	// Create RWPsection array entry time data on the heap

	RWPTCsectionserialdata = new unsigned int[MAXPSECTIONS];     	// Create RWPsection array entry time data on the heap

	Physicaldetectordata = new unsigned int[PHYSICALDETECTORS];		// Create physical detector array on the heap

	DCCslotcode = new unsigned int[NUMBEROFDCCSLOTS];

	DCCslotdirection = new unsigned int[NUMBEROFDCCSLOTS];

	DCCslotspeedcode = new unsigned int[NUMBEROFDCCSLOTS];

	Pointstatusflag = new unsigned int[NUMBEROFPOINTS];

	/************** initialize Psectiondata, BCMconnectordata, slot data, timer reset and device detectors  ******************/

	PSectionreset();
	
	BCMconnectordataReset();

	InitTCDbuffers();
	
	TCDbuffersclear();

	Slotsreset();

	Linksequencereset();

	Devicedetectorreset();

	InitializeTimestamp();

	Psectionexitcntrptr 	= &Psectionexitcntr;

	Psectionentrycntrptr 	= &Psectionentrycnt;

	USBbytesreadcntrptr 	= &USBbytesreadcnt;
	
	//std::cout<<"\nStartup check 2 - line 1524\n";
	
	// Objects are built on the heap
	
	LNconfigload = new LNconfigloader();
	
	LNcommandsptr = new LNcommands();

	PRHeapResbuff = new char[HEAPDISPLAYBUFFERLENGTH];

	LNLSLD = new LNwebLsectionsLdevices();

	LNLSLD->InitOploggingptr(LNOplatestlogptr);

	LNRteSchedMast = new LNRouteSchedMaster();

	LNlocodatatables = new LNlocoAlldata();

	LNSIM = new LNwebmonSimulator();
	
	LNDC = new LNdisplayconfig();

	LNRFIDSP = new LNRFidsensorprocessing();				// sensor processing object

	LNRteproc = new LNRouteprocess();						// Route processing object

	LNRteLSi = new LNRteLogicalsectionData();				// Logical section data object for route processing

	LNRteLSG = new LNRteLogicalsectionGroup();				// Collection of logical section data objects for route processing

	LNRteDRfd = new LNRteDevicereferencedata();				// Class for storing detector reference data for Route processing

//	LNRtimeallrecs = new LNLocoRoutemovementMaster();		// Grouping for all Train Route run time records;

	LNRtimeallrecsNG = new LNLocoRoutemovementMasterNG();	// New object for all Train Route run time records

	LNSimAllRtes = new LNSimulatorAllRoutes();				// Simulator object for storing all simulator routes

	LNSimRteMaster = new LNSimRouteMaster();				// Object with array to store all Simulator records

	LNSchedAlljourneys = new LNSchedulerAlljourneys();		// Scheduler object for storing all Schedules

	LNSchedExecutor = new LNSchedulerExecutor();			// Scheduler executor

	LNRteTrainScheduledata = new LNloadRteTrainSchedfile(); // Route, Train journey and Schedule details file load

	LNallTrainsinformation = new LNallTrains();				// Initial load of Train data

	LNsigprocess = new LNsignalprocessing();				// Signal processing object - drives coloured light displays
	
	LNLRprocess = new LNweblocorouteprocess(LNLSLD,LNlocodatatables,LNallTrainsinformation);
	
	LNttprocess = new LNturntableprocess();
	
	LNsigprocess->Initializesignaldata();					// Initialize signal configuration array;

	/****************************************************************************************************/
	/*          Load configuration                                                                      */
	/*          ==================                                                                      */
	/* 1. Commands available on system                                                                  */
	/* 2. PSection DCC codes mapping                                                                    */
	/* 3. Logical to physical section mapping config                                                    */
	/* 4  Loco config and speed tables                                                                  */
	/* 5  Point DCC config                                                                              */
	/* 6  Point logical config                                                                          */
	/* 7  Route, Journey and Schedule config                                                            */
	/*                                                                                                  */
	/****************************************************************************************************/

	Configurationloadprocessing(LNconfigload,LNlogptr, LNcommandsptr,LNLSLD, LNlocodatatables, LNRteTrainScheduledata, LNallTrainsinformation);

	char TestOutboundmessage[OUTPUTMSGBUFFERSIZE];

	bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	char* TestOutboundmessageptr = &TestOutboundmessage[0];

	//LNlocodatatables->DisplaylocoAlldatatable(TestOutboundmessageptr);

	//LNLSLD->DisplayLsections(0,TestOutboundmessageptr);

	//std::cout<<"\nAll LS  data loaded \n----------------------------------\n"<<TestOutboundmessage;

	//std::cout<<"\n\n2257 - LNwebmonNG stopped at end of config loading \n\n";

	//exit(0);

	// *******************************************************

	if (LNRFIDSP->LNRFidconfigload(0) > 0)							// load RFid sensors config file
	{
		std::cout<<"\nRFid Config file load failed - program stopped \n";
		exit(1);
	}
	
	//std::cout<<"\n\n2248 - LNwebmonNG stopped  \n\n";

	//exit(0);


	LNSchedExecutor->InitExecutors();						// Initialize Schedule executors
	
	LNttprocess->Initializations();							// initialize TT control variables

	LNSchedExecutor->InitializeSchedulerclocktimestamp();

	LNRteDRfd->LoadDeviceReference(LNLSLD);		// Populate device reference table from Logical section data table

	//std::cout<<"\n\n1987 - LNwebmonNG stopped  \n\n";

	//exit(0);

	LNRteLSG->InitializetheGroup();					// Initialize logical section group store used in Route processing

	LNRtimeallrecsNG->InitLocoRouteMasterTable();	// Initialize the Loco Route master table	

	//bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	int LSDisp = 0;

	STPDprocessflag = 0;
	
	RWPcounter = 0;
	
	RWPlockcounter = 0;

	PSectionreset();

	Sectioncounterset(0,0);

	USBbytecounterset(0);

	USBbytesclear();

	LNLSLD->ClearTcodes();

	InitializeslotArray();
	
	LNLSLD->InitSignalRelayTable();

	LNLSLD->InitDetectortriggertable();	

	LNLSLD-> InitStopflagtable();
	
	LNLSLD->InitializePWMsections();

	LNLSLD->LNsignalprocesslinkage(LNsigprocess);

	// **************** process operating logs *****************************

	char OplogLSload[2048];

	char* OplogLSloadptr = &OplogLSload[0];

	LNOplatestlogptr->Readlatestlogfile(OplogLSloadptr);

	OplogLSloadptr = &OplogLSload[0]; // reset the pointer to the start of the data array

	//std::cout<<"\n1980 Op loco log read:"<<OplogLSloadptr;

	LNLSLD->LoadLSlogdata(OplogLSloadptr);

	//LNlocodatatables->LoadlocodataOplog(OplogLSloadptr);

	LNOplatestlogptr->ReceiveLocoAlldataref(LNlocodatatables);

	LNLSLD->RefreshLastupdatedetails(1);

	LNLSLD->LogicalSectionTcodeRefresh();

	LNLSLD->LogicalDetectorupdateDetectorstatus();

	LNLRprocess->LNlocosectionrefresh();

	LNRteproc->LNRtepassreferences(LNLSLD);

	//std::cout<<"\n\n2065 Number of sections loaded "<< LNLSLD->GetNumberofLSloaded() <<" Max LS number loaded "<< LNLSLD->GetMaxsectionnumberloaded();

	LNRteLSG->LoadLogicalSectiondataGroup(LNLSLD,LNLSLD->GetMaxsectionnumberloaded());                                           //GetNumberofLSloaded());

	if (LNRteTrainScheduledata->LoadRteTrainSchedfiles(LNLRprocess,LNRteproc,LNRteLSG,LNRteDRfd,LNSIM,LNSimAllRtes,LNSimRteMaster,\
			LNRtimeallrecsNG,LNSchedAlljourneys,LNRteSchedMast,0) > 0)		// Load Route, Train and Schedule file
	{
		std::cout<<"\n\n\nTrain Route and Scheduler data load failed- program stopped \n\n\n";
				exit(1);
	}

	//std::cout<<"\n\n2353 - LNwebmonNG stopped at end of Route building\n\n";
	
	//exit(0);

	LNDC->Resetrecordstoredcounter(0);
	
	LNDC->ClearDisplayArraydata();
	
	LNDC->SetLSLDlinkage(LNLSLD);

	LNDC->Signalprocessreference(LNsigprocess);		// pass signal process reference to config display process

	//LNDC->ClearDisplayArraydataNG();

	if(	LNDC->LNDisplayarraydatafileload(0) > 0)		// load display configuration file
	{
		std::cout<<"\nDisplay configuration data load failed- program stopped \n";
		exit(1);
	}	
	
	//std::cout<<"\n\nLNwebmonNGmain Line 2363 Execution stopping\n\n";

	//exit(0);

	//bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	//LNDC->Displaysectionarrayconfig(0, TestOutboundmessageptr);

	//std::cout<<"\n\nDisplay Config data loaded \n--------------------------\n"<<TestOutboundmessage;

	//int numberofLSperPS = 12;  // number of logical sections linked to a physical section

	//int LogicalsectionlinktoPsection[numberofLSperPS];

	//int* LogicalsectionlinktoPsectionptr = &LogicalsectionlinktoPsection[0];

	//int LSfound = LNLSLD->GetLSlistforPsection(LogicalsectionlinktoPsectionptr , 1);

	//std::cout<<"\nLS found:"<<LSfound<<" LS1:"<<LogicalsectionlinktoPsection[0]<<" LS2:"<<LogicalsectionlinktoPsection[1];


	//std::cout<<"\n\nLNwebmonNGmain Line 2179 Execution stopping";
	
	//exit(0);
	
	//if (LNallTrainsinformation->Trainfileloader(0) > 0) // load Train information file
	//{
	//	std::cout<<"\nTrain initial position data load failed- program stopped \n";
	//	exit(1);
	//}
	
	LNLSLD->Initialtrainpositionsetup(LNallTrainsinformation);

	LNLSLD->TSbuildLSsectionlist();

	LNLSLD->LNallTrainslinkage(LNallTrainsinformation);
	
	LNLSLD->UpdatesignalsTrainSections();			// Populate train section data into Signals object

	//LNDC->Signalprocessreference(LNsigprocess);		// pass signal process reference to config display process

	/**************************************************************************/
	/*                                                                        */
	/*     Create pointers for serial interface and other messages            */
	/*                                                                        */
	/**************************************************************************/

	unsigned char * USBbytestreamdataptr;

	unsigned int * PsectionsEptr;

	unsigned int * PsectionsXptr;

	unsigned int * USBbytecntptr;

	unsigned char * UARTbytebufferptr;

	unsigned int * PDetectorptr;

	unsigned int * DCCslotcodeptr;

	unsigned int * DCCslotdirectionptr;

	unsigned int * DCCslotspeedcodeptr;

	unsigned int * Pointstatusflagptr;

	/*
		struct MsgTimestamp MTS[4];

	*/

	struct MsgTimestamp *MTSptr = NULL;

	MTSptr = MTS;

	std::cout<<"\n\nNG Timestamp 1:"<<(MTSptr + PS_TIMESTAMP)->HH[0]<<(MTSptr + PS_TIMESTAMP)->HH[1]<<":"\
				<<(MTSptr + PS_TIMESTAMP)->MN[0]<<(MTSptr + PS_TIMESTAMP)->MN[1]<<":"<<(MTSptr + PS_TIMESTAMP)->SS[0]<<(MTSptr + PS_TIMESTAMP)->SS[1];

	//std::cout<<"\nStartup check 8 - line 2018\n";


	/********************************************************/
	/*                                                      */
	/*    Initialize pointers for serial interface          */
	/*                                                      */
	/********************************************************


	********************************************************/

	PsectionsEptr = Psectionentrycntrptr;

	PsectionsXptr = Psectionexitcntrptr;

	USBbytecntptr = USBbytesreadcntrptr;

	//Psectionsptr = &Psectionserialdata[0];

	//RWPsectionsptr = &RWPsectionserialdata[0];
	
	TCDdataptr = &TCDbuffers[0];

	Psectionsptr = &RWPsectionserialdata[0];

	RWPsectionsptr = &Psectionserialdata[0];

	RWTEPsectionsptr = &RWPTEsectionserialdata[0];

	RWTXPsectionsptr = &RWPTXsectionserialdata[0];

	RWTCPsectionsptr = &RWPTCsectionserialdata[0];

	USBbytestreamdataptr = &USBbytestreamdata[0];

	UARTbytebufferptr = &USBbytestreamdata[0];

	DCCslotcodeptr = &DCCslotcode[0];

	DCCslotdirectionptr = &DCCslotdirection[0];

	DCCslotspeedcodeptr = &DCCslotspeedcode[0];

	PDetectorptr = &Physicaldetectordata[0];

	Pointstatusflagptr = &Pointstatusflag[0];
	
	LNLRprocess->Passloggingpointer(LNlogptr);

	//char TestOutboundmessage[OUTPUTMSGBUFFERSIZE];

	//bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

	//char* TestOutboundmessageptr = &TestOutboundmessage[0];

	//LNsigprocess->Displaysignalconfig(1, TestOutboundmessageptr);

	//std::cout<<"\n\n2264 Sig 1 config:"<<TestOutboundmessageptr;

	GUIAllLinesinitMain();

	//GUIAllLinesdisplayMain();

	//InsertGUItextlineMain(char* GUImessage)
	/*
	char GUItestmsg[GUIMESSAGELINELENGTH];

	char* GUItestmsgptr = &GUItestmsg[0];

	bzero(GUItestmsg,GUIMESSAGELINELENGTH);

	sprintf(GUItestmsg,"New line 1\n");

	InsertGUItextlineMain(GUItestmsgptr);

	printf("\n\n");

	GUIAllLinesdisplayMain();

	sprintf(GUItestmsg,"New line 2\n");

	InsertGUItextlineMain(GUItestmsgptr);

	printf("\n\nFinal Check\n\n");

	GUIAllLinesdisplayMain();



	std::cout<<"\n2346 Stopping at line 2346 \n";
	exit(0);
	*/
	/* ******************************************************************* */
	/* Initialize GPIO on Pi                                               */
	
	//std::cout<<"\nStartup check 9 - line 2099\n";

	TCDdatainitialize();

	int tc;

	int * Thptr;

	int Thvalue;

	Thptr = &Thvalue;

	Thvalue = 8;

	for (tc = 0;tc < MAXNUMOFTHREADS;tc++)
	{
		Tflagptr[tc]	= false;
	}

	/*********************************************************************************************/
	//++ Serialcomms(Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr, DCCslotspeedcodeptr, Pointstatusflagptr, MTSptr);

	/*********************************************************************************************/

	Tflagptr[1] = true;   // for console

	Tflagptr[2] = true;   // for webserver


	/****************************************************************************************************/
	// Main Serialcomms thread - runs serial port processing of DCC signals
	/****************************************************************************************************/


	std::thread t1 {[LNlogptr,Psectionsptr,PDetectorptr,DCCslotcodeptr,DCCslotdirectionptr,DCCslotspeedcodeptr,\
					 Pointstatusflagptr, MTSptr,PsectionsEptr,PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr]\
		{Serialcommsthread(LNlogptr,Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr,DCCslotspeedcodeptr,\
				Pointstatusflagptr, MTSptr,PsectionsEptr,\
				PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr);}};

	/****************************************************************************************************/
	/*   Webserver execution thread                                                                     */
	/****************************************************************************************************/


	std::thread t2 {[LNlogptr,LNSIM,LNLSLD,MTSptr,&Tflagptr,LNLRprocess,LNDC,LNRFIDSP,\
					 LNRteproc,LNSimRteMaster,LNSchedExecutor,LNRteSchedMast,LNSchedAlljourneys,\
					 LNRteDRfd,LNRtimeallrecsNG,LNsigprocess,LNlocodatatables]\
		{LNwebserverexecute(LNlogptr,LNSIM,LNLSLD,MTSptr,Tflagptr,LNLRprocess,LNDC,LNRFIDSP,\
				LNRteproc,LNSimRteMaster,LNSchedExecutor,LNRteSchedMast,LNSchedAlljourneys,\
				LNRteDRfd,LNRtimeallrecsNG,LNsigprocess,LNlocodatatables);}};

	//std::this_thread::sleep_for (std::chrono::seconds(2));

	std::cout<<"\n\nNG Timestamp:"<<(MTSptr + PS_TIMESTAMP)->HH[0]<<(MTSptr + PS_TIMESTAMP)->HH[1]<<":"\
			<<(MTSptr + PS_TIMESTAMP)->MN[0]<<(MTSptr + PS_TIMESTAMP)->MN[1]<<":"<<(MTSptr + PS_TIMESTAMP)->SS[0]<<(MTSptr + PS_TIMESTAMP)->SS[1];

	std::cout<<"\n\nMTS-seq:"<<MTSptr->Seqnumber<<" MTS-seq1:"<<(MTSptr+1)->Seqnumber;
	
	/****************************************************************************************************/
	// Console log thread
	/****************************************************************************************************/

	std::thread t3 {[LNlogptr,CP,&Tflagptr,LNLSLD,MTSptr,LNLRprocess,LNDC,LNRFIDSP,LNRteproc,LNRteLSi,LNRteLSG,LNRteDRfd,\
					 LNRtimeallrecsNG, LNSIM,LNSimAllRtes,LNSimRteMaster,LNSchedAlljourneys,LNSchedExecutor,LNRteTrainScheduledata,\
					 LNallTrainsinformation,LNRteSchedMast,LNttprocess,LNsigprocess,LNcommandsptr, LNlocodatatables] \
		{LNwebmonConsole(LNlogptr,CP,Tflagptr,LNLSLD,MTSptr,LNLRprocess,LNDC,LNRFIDSP,LNRteproc,LNRteLSi,LNRteLSG,\
				LNRteDRfd,LNRtimeallrecsNG, LNSIM,LNSimAllRtes,LNSimRteMaster,LNSchedAlljourneys,\
				LNSchedExecutor,LNRteTrainScheduledata,LNallTrainsinformation,LNRteSchedMast,LNttprocess,\
				LNsigprocess,LNcommandsptr,LNlocodatatables);}};


	/****************************************************************************************************/
	// Simulator thread
	/****************************************************************************************************/

	std::thread t4 {[LNSIM,LNLSLD,MTSptr,&Tflagptr,LNLRprocess,LNRteproc,LNSimAllRtes,LNSimRteMaster]{LNwebSimulatorexecute(LNSIM,LNLSLD,MTSptr,Tflagptr,LNLRprocess,LNRteproc,LNSimAllRtes,LNSimRteMaster);}};

	std::thread t5 {[LNSIM,LNLSLD,MTSptr,&Tflagptr,LNLRprocess]{LNwebtimerprocessexecute(LNSIM,LNLSLD,MTSptr,Tflagptr,LNLRprocess);}};
	
	/****************************************************************************************************/
	// Serial port thread
	/****************************************************************************************************/

	std::thread t6 {[Psectionsptr,PDetectorptr,DCCslotcodeptr,DCCslotdirectionptr,DCCslotspeedcodeptr,\
					 Pointstatusflagptr, MTSptr,PsectionsEptr,PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr, LNsigprocess]\
		{SerialUSB0commsthread(Psectionsptr,PDetectorptr, DCCslotcodeptr,DCCslotdirectionptr,DCCslotspeedcodeptr,\
				Pointstatusflagptr, MTSptr,PsectionsEptr,\
				PsectionsXptr,USBbytecntptr,UARTbytebufferptr,RWTEPsectionsptr,RWTXPsectionsptr,RWTCPsectionsptr,LNsigprocess);}};

	/****************************************************************************************************/
	// Thread 7 - Timed events thread e.g Signal refresh
	/****************************************************************************************************/

	std::thread t7 {[LNSIM,LNLSLD,MTSptr,&Tflagptr,LNLRprocess]{LNwebtimer2processexecute(LNSIM,LNLSLD,MTSptr,Tflagptr,LNLRprocess, LNsigprocess);}};

	/****************************************************************************************************/
	// Scheduler thread
	/****************************************************************************************************/

	std::thread t8 {[LNSchedExecutor,LNLRprocess,LNRtimeallrecsNG,LNSIM,LNSimAllRtes,LNSimRteMaster,LNSchedAlljourneys,LNLSLD, LNRteSchedMast]\
		{LNwebSchedulerexecutor(LNSchedExecutor,LNLRprocess,LNRtimeallrecsNG, LNSIM,LNSimAllRtes,LNSimRteMaster,LNSchedAlljourneys,LNLSLD, LNRteSchedMast);}};

	std::cout<<"\nJoining console processing on thread 3....\n";

	t3.join();

	std::this_thread::sleep_for (std::chrono::seconds(3));

	std::cout << "LNmonitor2024 program ended run\n";

	return 0;
}


int Reloadconfiguration()
{
	Configurationloadprocessing(LNconfigload,LNlogptr,LNcommandsptr, LNLSLD, LNlocodatatables, LNRteTrainScheduledata, LNallTrainsinformation);

	return 0;
}



