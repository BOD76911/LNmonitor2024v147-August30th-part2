/*************************************************************************************
 * 
 *  LNwebmon.h header file 
 * 
 * 	July 2017 
 * 
 *  Ver 4.0 December 2018 - includes stop flag processing 
 *
 *  Ver 4.9 June 2019 - includes simulator file processes 
 *
 *  Ver 5.0 - Updated August 2022 to include processing for Infra-red processing and USB based
 *            RFid processing
 * 
 * 
 * 
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define  PAGESTORESIZE				2400 
#define  LINEINPUTBUFSIZE 			256

#define  SPLITFIELDSIZE  			16 
#define  LINESPLITELEMENTS 			 8 
#define  LINESPLITELEMENTSLENGTH		16

#define PSECTIONS       			91 //65  June 4th
#define LIVESECTIONS     			90 // 64 June 4th
#define LOGICALDETECTORS            		 8

#define TCDBUFFERCOUNT				 4
#define TCDBUFFERBITCOUNT            		 8

#define HEXLINESPLITELEMENTS 		 	4 
#define HEXLINESPLITELEMENTSLENGTH	 	2

#define NUMBEROFTIMESTAMPS			8

#define NUMBEROFPOINTS 				16
#define NUMBEROFCROSSOVERSECTIONS 		64
#define PTOPENCLOSEFLAG     		 	2

#define SERIALCHARBUFSIZE  			48

#define DISPLAYOUTPUTMSGBUFFERSIZE		160000

#define MAXDALINES				16
#define MAXDAITEMS  				940				/* length of each line in the display table                 */
  
#define FALSE 0
#define TRUE 1

#define NUMBREOFIFRSENSORS  12

#define     CONFIGFILEDirectory 	"/home/brian/LNmonitor2024/ConfigandDatafiles/"
#define     DCCSECTIONCODES             "LNsection.dat"

typedef int Cbool;

unsigned char DA[MAXDALINES][MAXDAITEMS];

unsigned char *DAptr[MAXDALINES][MAXDAITEMS];

/***************** Definitions for Infra-red detectors and USB based RFid detection ***********/ 
// August 26th 2022

extern int Ctrigger;

extern int messageflag;

extern int Int1counter;
extern int Int2counter;
extern int Int3counter;
extern int Int4counter;

extern int Int5counter;
extern int Int6counter;
extern int Int7counter;
extern int Int8counter;

extern int Int9counter;
extern int Int10counter;
extern int Int11counter;
extern int Int12counter;

 
#define PIN1 23  // use BCM pin coding - actual Pi pin 29, 

#define PIN2 1 //8  // use BCM pin coding - actual Pi pin 3, 


#define PIN3 8 //7  // use BCM pin coding - actual Pi pin 5, 
#define PIN4 7 //8 //1  // use BCM pin coding - actual Pi pin 7, 

#define PIN5 21 //12  // use BCM pin coding - actual Pi pin 31, 
#define PIN6 24 //16  // use BCM pin coding - actual Pi pin 33, 

#define PIN7 6 //25 //26  // use BCM pin coding - actual Pi pin 35, 
#define PIN8 25 //6 //20  // use BCM pin coding - actual Pi pin 37, 

#define PIN9 12 //21  // use BCM pin coding - actual Pi pin 26

#define PIN10 16 // 24  // use BCM pin coding - actual Pi pin 36, 
#define PIN11 26 //25  // use BCM pin coding - actual Pi pin 38, 
#define PIN12 20 //6   // use BCM pin coding - actual Pi pin 40


extern int sensorstate[NUMBREOFIFRSENSORS];

void handle1(void);
void handle2(void);
void handle3(void);
void handle4(void);

void handle5(void);
void handle6(void);
void handle7(void);
void handle8(void);

void handle9(void);
void handle10(void);
void handle11(void);
void handle12(void);

int Serialcomms();

void RFidpinconfig();

void Initsensorstate();

void Getsensorstate();

/***************** USB RFid receivers ******************/

#define USBPORTCOUNT    			8       // maximum number of USB receiver connections supported 

#define USBSERIALBUFFREADCHARS 			64

void CheckforRFidUSBports();

char RFiddevicename[13];

int fd_serialport_check[USBPORTCOUNT];

int fd_serialport_ID[USBPORTCOUNT];

int Port_bytecount[USBPORTCOUNT];

int uart_somflag[USBPORTCOUNT];
    
int uart_eomflag[USBPORTCOUNT];
    
int uart_charcount[USBPORTCOUNT];

unsigned char USBSerialCharinputbuffer[USBPORTCOUNT][SERIALCHARBUFSIZE];

unsigned char Uartportbuffer[USBPORTCOUNT][USBSERIALBUFFREADCHARS];       //buffer for where data is put when it is read in  

int Portsubscript; 

int Numberofportsfound;

/*********** Serial port processing ********************/

extern int fd_serialport;

void signal_handler_IO (int status);         //definition of signal handler for AMO interface

void signal_handler_IO_ACM0 (int status);

void USB0_signal_handler_IO (int status);    //definition of signal handler for USB0 interface

int Serialcomms();

int SerialUSB0comms();

void error(char *msg);

void cerror(FILE *stream, char *cause, char *errno, 
	    char *shortmsg, char *longmsg);

int LNSectionFileload(char *filename);

void linebufferprocessingNG(unsigned char* lineinputbufptr);

int str_split(unsigned char* a_str, const char a_delim, unsigned char* ls_str);

int Buffercopy(unsigned char *src, unsigned char *dest, int charcount);

int HEXBuffercopy(unsigned char *src, unsigned char *dest, int charcount);

void printbufferinhex(unsigned char *src, int cc);

void Bufferstore(unsigned char *s_ptr, unsigned char maxcnt, unsigned char value);

int SDSHexstring(int line, unsigned char *strmsg, unsigned char format, unsigned char NumberofBytes);

unsigned char AtoH(unsigned char bytein);

unsigned char StringtoH(unsigned char *charinput);

void Physicalsectiondatadisplay(int psection); 

int LNStringtoInteger(unsigned char *stringin);

int PSectionrecordcount;

int PSectionrecordvalidlines;

char HtoA(unsigned char c);

unsigned char Linesplit[LINESPLITELEMENTS+1][LINESPLITELEMENTSLENGTH+1];  

unsigned char HEXLinesplit[HEXLINESPLITELEMENTS][HEXLINESPLITELEMENTSLENGTH];

unsigned char SerialCharinputbuffer[SERIALCHARBUFSIZE];

/************** message processing routines ******************************/

void LNSpeedmessagesprocess(unsigned char* FourbyteDCC);

void LNSdirectionmessagesprocess(unsigned char* FourbyteDCC);

void LNSectionmessageprocess(unsigned char* FourbyteDCC);

int Blockmsgtriggercnt;

void DCCslotmessagesprocess(char *DCCdata);

int UploadDCCslots();

void TCDbuffersclear();

void InitializeslotArray();

int DCCLococodecalc(char Addr, char Addr2);

void AppendChecksum(char *Inputbuff, int length);

void Allslotsstop();

void Speedcmdprocess(int slotdigit, int speedval, int direction);

void PointDCCcodeupdate(int ptc, int OC, unsigned char CodeDCC[4]);

void Switchcmdprocess(int pt,int OC,unsigned char CodeDCC[4]);

void LNpointmessageprocess(unsigned char* FourbyteDCC);

unsigned char GetDCCcodechar(int ptc, int OC, int charnum);

void DisplaySCPointDCC(int ptcc);

void Pointswitchstatusupd(int pt, int status);

void SetTimestamp(int offset, int sa, char* c_time_string);

void GetDatetimestamp();

//int BTinitialize();

int GetBTconnectionstatus();

void BTtestmessage();

/*************************************************************************/

//extern int * Psectionptr;

int getNum(char ch);

unsigned int hex2int(unsigned char hex[]);

/*************************************************************************/
struct MsgTimestamp
{
	int Seqnumber;
	unsigned char HH[2];    // 00
	unsigned char MN[2];    // 00
	unsigned char SS[2];    // 00
	unsigned char MLS[3];   // 000
	int Timestampvalue;
};

extern struct MsgTimestamp MTSc[NUMBEROFTIMESTAMPS];

struct TCDdataconfig
{
	unsigned int Databit_type[TCDBUFFERBITCOUNT]; 
	unsigned int DataBufferbit[TCDBUFFERBITCOUNT];	
};

extern struct TCDdataconfig TCDdatabufferconfig[TCDBUFFERCOUNT];

struct TCDdatavalues
{
	unsigned int bitvalues[TCDBUFFERBITCOUNT];	
};

extern struct TCDdatavalues TCDdatabuffervalues[TCDBUFFERCOUNT];

/************** processing codes for inbound LocoNet DCC data ********************/ 

struct SDetail 
{    
    unsigned int  Occupied;
    unsigned int  ETimedelay;
    unsigned char Entrycode[12];
    unsigned char EntryHexcode[4];     // Hex code when train enters 
    unsigned int  Entrycodehashval; 
    
    
    unsigned char Exitcode[12];
    unsigned char ExitHexcode[4];      // Hex code when train exits 
    unsigned int  Exitcodehashval;  
    unsigned int  XTimedelay; 
};

extern struct SDetail Physicalsection[PSECTIONS];

struct DDetail 
{    
    unsigned int  Occupied;
    unsigned char Entrycode[12];
    unsigned char EntryHexcode[4];     // Hex code when train enters 
    unsigned int  Entrycodehashval; 
    unsigned char Exitcode[12];
    unsigned char ExitHexcode[4];      // Hex code when train exits 
    unsigned int  Exitcodehashval;    
};

extern struct DDetail Physicaldetectors[LOGICALDETECTORS];
