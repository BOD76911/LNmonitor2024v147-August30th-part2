 /*************************************************
 * 
 *  LNserial.c  
 * 
 * 	June 2019 
 * 
 * 	Updated - May 2022 for new TT controller
 * 
 * 
 * 	Updated - June 2022 for bluetooth link to TT - based on socket processing  
 * 
 *      Updated June 2023 for new DCC code matching process for physical sections.
 * 
 * 
 *      Updated August 2024 - I2C interface to DC/DCC track circuits
 * 
 *                          - Start of PICAXE based controller
 * 
 ************************************************/
/*
 *  Code to handle the serial port interface with the loconet
 * 
 *  Code cleaned up and extended to process UART interface
 * 
 * 
 * 
 */ 
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
/*-----------------------------------------------------------------------------------*/
/*                                                                                   */
/*       February 2022 - Updated for USB0 input processing with interrupts           */
/*                                                                                   */
/*       April 2022 - updated to include test for wiringPI library                                                                           */
/*                                                                                   */
/*       May 2022  - updated for new turntable control using PWM on PI               */
/*                                                                                   */
/* ********************************************************************************* */
 
#include "LNserial.h"
#include <termios.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/ioctl.h>


#ifdef PIRun
#include <wiringPi.h> //      Include WiringPi library
#endif

#define SLEEPMILLISECONDS 		1

#define NUMBEROFDCCSLOTS 		20

#define DCCMSGOUTPUT     		4

#define PS_TIMESTAMP            1
#define PD_TIMESTAMP            2
#define SLOT_TIMESTAMP          3
#define PTS_TIMESTAMP           4

#define UPLOADBUFFCHARS		32

#define ACM0BUFFERSIZE		255
#define UARTBUFFERSIZE		255

#define BCMconnections		  4

#define SIGNALCHARMAX		512

int Motorspeed;

const int TRSDinput = 22; // Active-low button - Broadcom pin 12, P1 pin 32

const int TF1input = 18; // Active-low button - Broadcom pin 20, P1 pin 38
const int TF2input = 4; // Active-low button - Broadcom pin 21, P1 pin 40

const int IN4outpin = 19; //23;  //  pi pin 29
const int IN3outpin = 5; //25;  //  pi pin 31 
const int MTRpwmpin = 13; //24; //  pi pin 33 

const int TF1Rpin = 27;
const int TF2Rpin = 17;


char Sectionvalue = 0;

char* c_time_string_str;

time_t current_time;

char Tbuff[20];

unsigned char signalbuffer[SIGNALCHARMAX];

char Outmsg[80];


int sensorstate[NUMBREOFIFRSENSORS];

int Signalcharcount = 0;

int ACM0_Signalcharcount = 0;

unsigned char ACM0Rxbuffer[255];              	//buffer for where data is put when it is read in from the Loconet receiver

unsigned char UARTRxbuffer[255];           	//buffer for where data is put when it is read in from the USB0 serial port 

unsigned char  Databuffer[TCDBUFFERCOUNT];

void Ctriggerupdate(int ivalue);
void TCDdatabufferupdate(int buffer, int bit, int section, int type);

int LNmessageproc(int res);

struct PointDCCcodes
{
	int Pointnumber;
	int Openclose;
	unsigned char DCCcode[4];
	int  PtDCCcodehashval;		
};

struct PointDCCcodes PointDCCcodetable[NUMBEROFPOINTS][PTOPENCLOSEFLAG];

struct Pointstatusdata
{
	int Lastupdate;  // 0 = no updates ,1 = initialized, 2 = opened, 3 = closed
};

struct Pointstatusdata Pointswitchstatus[NUMBEROFPOINTS];

struct SlotArray
    {
	int 	Lococode;
	int 	Throttlestatus;
	char 	Throttlemsg[8];
	int 	ActSpeed;
	int 	Targetspeed;
	int     Actualdirection;
	int     Targetdirection;
	char 	Direction[4];
	char 	CV17;
	char 	CV18;
	char 	Updateflag;
	char 	Reversepolarity;  // 0 = No. 1 = Yes
    };

struct SlotArray SlotInformation[NUMBEROFDCCSLOTS];

struct SlotArray* ptr = &SlotInformation;

char DCCmsgwithchecksum[DCCMSGOUTPUT];

int DCCmsglength;
/*********************************************************************/
/*    Physical section hex code look-up table                        */
/*********************************************************************/

struct SDetail Physicalsection[PSECTIONS];

struct TCDdataconfig TCDdatabufferconfig[TCDBUFFERCOUNT];

struct TCDdatavalues TCDdatabuffervalues[TCDBUFFERCOUNT];

int Sectionconfigflag[PSECTIONS];

void DisplayPSectionconfigflags();

void Psectionconfiginit();

char DCCmsgtext[DCCMSGOUTPUT];

/*                                               */
/* I2C interface details                         */
/*                                               */

#define MCP23017_ADDRESS_1   0x20                   // MCP23017 - 1

#define MCP23017_ADDRESS_2   0x21                   // MCP23017 - 2

int fdBCM_1;

int fdBCM_2;

int BCMopen_1;

int BCMopen_2;

uint8_t GPIOAinputs;
        
uint8_t GPIOBinputs;    

uint8_t GPIOCinputs;
        
uint8_t GPIODinputs;   

uint8_t BCMGPIOdata[BCMconnections];

/* Serial port components for Loconet interface  */

#define BAUDRATE B4800

#define LOCONETDEVICE "/dev/ttyACM0"    // interface for Loconet interface device

#define _POSIX_SOURCE 1         //POSIX compliant source

int wait_flag=TRUE;             // TRUE while no signal received
int wait_flag_ACM0 = TRUE;

int ACM0_PiD = 0;
int USB0_wait_flag=TRUE;	// TRUE while no USB0 signal received

char devicename[80];
long Baud_Rate = 38400;         // default Baud Rate (110 through 38400)

long ACM0baud = B38400;
long ACM0databits = CS8;
long ACM0stopbits = 0;
long ACM0parityon = 0;
long ACM0parity = 0;
long ACM0format = 5;

long BAUD;                      // derived baud rate from command line
long DATABITS;
long STOPBITS;
long PARITYON;
long PARITY;
int Data_Bits = 8;              // Number of data bits
int Stop_Bits = 1;              // Number of stop bits
int Parity = 0;                 // Parity as follows:
                  // 00 = NONE, 01 = Odd, 02 = Even, 03 = Mark, 04 = Space
int Format = 4;

int Serialportstatus;

int Loggingflag = 0;

int SerialCharsread;

/*            USB0 serial port parameters        */

//char *portname = "/dev/ttyUSB0";

char *portname = "/dev/ttyS0";

int fd_UART;
int wlen;

int UART_PiD = 0;

int slotcounter;

unsigned char FourbyteDCC[4];

char Messageprocess[20];

char DCCcodeshow[64];

int fd_serialport;

int ACM0_fd_serialport;

int fd_serialport1;

int fd_serialport1_found;

int ACM0serialportopened = 0;

int USB0_serialport;

int USB0serialportopened = 0;

int ACM0Rxbytes, UARTRxbytes;

volatile int STOP=FALSE;

unsigned int * Psectionptr;
unsigned int * TCDPsectionptr;
unsigned int * TEPsectionptr;
unsigned int * TXPsectionptr;
unsigned int * TCPsectionptr;
unsigned int * PDetectorptr;
unsigned int * Slotcodeptr;
unsigned int * Slotdirectionptr;
unsigned int * Slotspeedptr;
unsigned int * Pointstatusflagptr;

unsigned int * Psectionentryptr;
unsigned int * Psectionexitptr;
unsigned int * USBbytecntrptr;
unsigned char * USBbytebufferptr;

struct MsgTimestamp* MSGptr;

char Allslotsloaded;

/*****************************************************************
 * 
 *  Bluetooth connections 
 * 
 * 
 ****************************************************************/
 
 struct sockaddr_rc addr = { 0 };
 int BTstatus, BTendofread, BTmsgoutcc,BTCONNECTED,BTsignals,BTsignalsready;
 char BTbuf[1024] = { 0 };
 char BTmsgout[1024] = { 0 };
 int BTbytes_read, BTtotal_bytes_read, BTloopcount;
 //char BTdest[18] = "98:D3:31:F6:3F:BE";
 
 //char BTdest[18] = "98:D3:31:F6:98:1C";
 
 char BTdest[18] = "98:D3:71:F6:9E:EA";
 
/********************* Infra Red connections fields **************/


void handle1(void);
/*
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
void handle12(void); */

/*****************************************************************
 * Interrupt code                                                *
 * **************************************************************/
 
//#define PIN1 5  // use BCM pin coding - actual Pi pin 29, wiringPI pin 21
//#define PIN2 3  // use BCM pin coding - actual Pi pin 5, wiringPI pin 9
//#define PIN3 4  // use BCM pin coding - actual Pi pin 7, wiringPI pin 7
//#define PIN4 2  // use BCM pin coding - actual Pi pin 11, wiringPI pin 0

// How much time a change must be since the last in order to count as a change

#define IGNORE_CHANGE_BELOW_USEC 75000

#define IGNORE_CHANGE_BELOW_USEC1 75000
#define IGNORE_CHANGE_BELOW_USEC2 50000
#define IGNORE_CHANGE_BELOW_USEC3 45000
#define IGNORE_CHANGE_BELOW_USEC4 75000

#define IGNORE_CHANGE_BELOW_USEC5 75000
#define IGNORE_CHANGE_BELOW_USEC6 75000
#define IGNORE_CHANGE_BELOW_USEC7 75000
#define IGNORE_CHANGE_BELOW_USEC8 75000

#define IGNORE_CHANGE_BELOW_USEC9 75000
#define IGNORE_CHANGE_BELOW_USEC10 75000
#define IGNORE_CHANGE_BELOW_USEC11 75000
#define IGNORE_CHANGE_BELOW_USEC12 75000


// Current state of the pin
static volatile int state; 
static volatile int P1state;
static volatile int P2state;
static volatile int P3state;
static volatile int P4state;

static volatile int P5state;
static volatile int P6state;
static volatile int P7state;
static volatile int P8state;

static volatile int P9state;
static volatile int P10state;
static volatile int P11state;
static volatile int P12state; 

// Time of last change
//struct timeval last_change;
struct timeval Int1_last_change;
struct timeval Int2_last_change;
struct timeval Int3_last_change;
struct timeval Int4_last_change;

struct timeval Int5_last_change;
struct timeval Int6_last_change;
struct timeval Int7_last_change;
struct timeval Int8_last_change;

struct timeval Int9_last_change;
struct timeval Int10_last_change;
struct timeval Int11_last_change;
struct timeval Int12_last_change;


int Int1counter;
int Int2counter;
int Int3counter;
int Int4counter;

int Int5counter;
int Int6counter;
int Int7counter;
int Int8counter;

int Int9counter;
int Int10counter;
int Int11counter;
int Int12counter;

int Ctrigger;

int messageflag;

/*****************************************************************
 * Time stamp records
 *
 *     1                     Physical section updates
 *     2                     Physical detector update
 *     3                     Slot data update
 *     4                     Point status
 *
 ****************************************************************/

struct MsgTimestamp MTSc[NUMBEROFTIMESTAMPS];

int DisplayTCDdatabufferconfig(char* messagereply);

void DisplayTCDdatabuffervalues();

void ProcessTCDreadings();

char slotcode[3];

/*
 
  C++ calling routine reference
  ----------------------------- 
  
  void Serialcommsthread(unsigned int * Psectionsptr,unsigned int PDetectorptr, unsigned int * DCCslotcodeptr, \
		unsigned int * DCCslotdirectionptr,unsigned int * DCCslotspeedcodeptr);
  
 */

//int BTinitialize()
//{
//	return 0;
//}

int BTsignal_linkinitialize()
{
    BTtotal_bytes_read = 0;
    BTloopcount = 0;
    BTendofread = 0;
    BTmsgoutcc = 0;
    BTCONNECTED = 0;

	// allocate a socket
	
    BTsignals = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( BTdest, &addr.rc_bdaddr );

    // connect to server
    BTstatus = connect(BTsignals, (struct sockaddr *)&addr, sizeof(addr));
	
    if (BTstatus == 0)
    {
	    printf("\n500 Bluetooth Signal interface connection OK BTsignals = %i \n",BTsignals);  
	    BTCONNECTED = 1;	  
    }
    else
    {
	   printf("\n505 Bluetooth Signal interface connection failed \n"); 
    }
  
  
  
  BTsignalsready = 1;
  
  BTtestmessage(7);
  
  return BTCONNECTED;
}

void CloseBTsignalsconnection()
{
   close(BTsignals);
   
   printf("\n521 BTsignals connection closed ");
   printf("\n");
}

int GetBTconnectionstatus()
{
	return BTCONNECTED;	
}

void BTsignalmessagebackup(unsigned char Sigmsgout[7])
{
    printf("\nLNserial 514 - Call to send BT signal message %s",Sigmsgout);    
    
    int BTconnection = 0;
    
    while (BTconnection == 0)
    {
	BTsignals = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// set the connection parameters (who to connect to)
    
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( BTdest, &addr.rc_bdaddr );

	// connect to server
	BTstatus = connect(BTsignals, (struct sockaddr *)&addr, sizeof(addr));   
   
	if( BTstatus < 0 ) 
	{	
		//printf("\n547 BT signals connection status error %i waiting 250ms ...",BTstatus);
		BTconnection = 0;		 
		usleep(250);		
	}  
	else
	{
		BTconnection = 1;
		printf("\n549 BTsignals connected OK");
	} 	
   }
	
	if(( BTstatus == 0 ) && (BTsignalsready == 1))    // send message
	{        
		BTstatus = write(BTsignals, Sigmsgout,7);
		BTsignalsready = 0;	
	}

	
    
    
    BTtotal_bytes_read 	= 0;
    BTendofread 	= 0;
    BTloopcount		= 0;
    BTmsgoutcc		= 0;
    
    
    // read data echoed back 
    
    while ((BTendofread < 1) && (BTsignalsready < 1))
    {
		BTbytes_read = read(BTsignals, BTbuf, sizeof(BTbuf));
    
		if( BTbytes_read > 0 ) 
		{			
			BTtotal_bytes_read = BTtotal_bytes_read + BTbytes_read;
			
			for (int cc = 0; cc < BTbytes_read;cc++)
				{
					BTmsgout[BTmsgoutcc] = BTbuf[cc];
					if ((BTbuf[cc] == '\n') || (BTbuf[cc] == 'X'))
					{
						BTendofread = 1;
						BTsignalsready = 1;						
					}
					BTmsgoutcc++;
				}
			BTloopcount = BTloopcount + 1;
		}
    }	
	//printf("\nhex ->%02X %02X %02X %02X %02X %02X", BTbuf[0],BTbuf[1],BTbuf[2],BTbuf[3],BTbuf[4],BTbuf[5]);
	
	printf("\n582 Signal msg received buffer ->%s",BTmsgout);	
	printf("\n");	
    	close (BTsignals);
}

void BTsignalmessage(unsigned char Sigmsgout[7])
{
    //printf("\nLNserial 514 - Call to send BT signal  BTsignal value %i message %s",BTsignals,Sigmsgout);    
    
    int BTconnection = 0;   
    
    /* 
    
    while (BTconnection == 0)
    {
	BTsignals = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// set the connection parameters (who to connect to)
    
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( BTdest, &addr.rc_bdaddr );

	// connect to server
	BTstatus = connect(BTsignals, (struct sockaddr *)&addr, sizeof(addr));   
   
	if( BTstatus < 0 ) 
	{	
		//printf("\n547 BT signals connection status error %i waiting 250ms ...",BTstatus);
		BTconnection = 0;		 
		usleep(250);		
	}  
	else
	{
		BTconnection = 1;
		printf("\n549 BTsignals connected OK");
	} 	
   }
   
	
	if(( BTstatus == 0 ) && (BTsignalsready == 1))    // send message
	{        
		BTstatus = write(BTsignals, Sigmsgout,7);
		BTsignalsready = 0;	
	}

	  */
	  
    if ((BTCONNECTED > 0) && (BTsignals > 0))     // connection already in place so send message 
    {
	BTstatus = write(BTsignals, Sigmsgout,7);	
	//printf("\n645 Direct BTmessage sent - status value %i",BTstatus);
	BTsignalsready = 0;	    
    }
    else
    {
	//printf("\n649 No transmissions as No BT signals connection ....");
    }
    
      
    
    BTtotal_bytes_read 	= 0;
    BTendofread 	= 0;
    BTloopcount		= 0;
    BTmsgoutcc		= 0;
    
    
    // read data echoed back 
    
    while ((BTendofread < 1) && (BTsignalsready < 1))
    {
		BTbytes_read = read(BTsignals, BTbuf, sizeof(BTbuf));
    
		if( BTbytes_read > 0 ) 
		{			
			BTtotal_bytes_read = BTtotal_bytes_read + BTbytes_read;
			
			for (int cc = 0; cc < BTbytes_read;cc++)
				{
					BTmsgout[BTmsgoutcc] = BTbuf[cc];
					if ((BTbuf[cc] == '\n') || (BTbuf[cc] == 'X'))
					{
						BTendofread = 1;
						BTsignalsready = 1;						
					}
					BTmsgoutcc++;
				}
			BTloopcount = BTloopcount + 1;
		}
    }	
	//printf("\nhex ->%02X %02X %02X %02X %02X %02X", BTbuf[0],BTbuf[1],BTbuf[2],BTbuf[3],BTbuf[4],BTbuf[5]);
	
	//printf("\n582 Signal msg received buffer ->%s",BTmsgout);	
	//printf("\n");	
    	//close (BTsignals);
}

void BTtestmessage(int TTcode)
{
    printf("\n401 Sending BT message %02i ",TTcode);
    
    char msgxmit[5];
    
    char Dataxmit[2];   
    
    sprintf(Dataxmit,"%02i",TTcode);
    
    printf("\nLNserial 522 Dataout %s",Dataxmit);
    
    msgxmit[0] = 'D';
    msgxmit[1] = Dataxmit[1];
    msgxmit[2] = '0';
    msgxmit[3] = '1';
    msgxmit[4] = '\n';
    
   // msgxmit[0] = 'A';
   // msgxmit[1] = 'B';
   // msgxmit[2] = 'C';
   // msgxmit[3] = 'D';
   // msgxmit[4] = '\n';
    
	// send a message
	
    BTsignals = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( BTdest, &addr.rc_bdaddr );

    // connect to server
    BTstatus = connect(BTsignals, (struct sockaddr *)&addr, sizeof(addr));
	
   // if (BTstatus == 0)
   // {
   
   printf("\nLNserial 546 - BT status = %i",BTstatus);
   //close (BTs);
   //return;
	
    if( BTstatus == 0 ) 
    {
        BTstatus = write(BTsignals, msgxmit, 5);
	printf("\nLNserial 553 BT sent %s",msgxmit);
	printf("\nBTstatus = %i",BTstatus);
        // BTstatus = write(BTs, "1234\n", 5);
    }

    if( BTstatus < 0 ) 
    {
	 printf("\n771 BT write error ");   
	 return;
    }   
    
    BTtotal_bytes_read 	= 0;
    BTendofread 	= 0;
    BTloopcount		= 0;
    BTmsgoutcc		= 0;
    
    
    // read data echoed back 
    
    while (BTendofread < 1)
    {
		BTbytes_read = read(BTsignals, BTbuf, sizeof(BTbuf));
    
		if( BTbytes_read > 0 ) 
		{			
			BTtotal_bytes_read = BTtotal_bytes_read + BTbytes_read;
			
			for (int cc = 0; cc < BTbytes_read;cc++)
				{
					BTmsgout[BTmsgoutcc] = BTbuf[cc];
					if ((BTbuf[cc] == '\n') || (BTbuf[cc] == '\0'))
					{
						BTendofread = 1;						
					}
					BTmsgoutcc++;
				}
			BTloopcount = BTloopcount + 1;
		}
    }
	
	printf("\nEOM - total bytes read %i\n",BTtotal_bytes_read);
	printf("\nreceived [%s]\n", BTbuf);
	printf("\nhex ->%02X %02X %02X %02X %02X %02X", BTbuf[0],BTbuf[1],BTbuf[2],BTbuf[3],BTbuf[4],BTbuf[5]);
	printf("\nloop count : %i",BTloopcount);
	printf("\nMsg received char count = %i",BTmsgoutcc);
	printf("\nMsg received buffer ->%s",BTmsgout);	
	close (BTsignals);
}

void InitializeTimestamp()
{
	int ts;
	
	for (ts = 0; ts < NUMBEROFTIMESTAMPS; ts++)
	{
		MTSc[ts].HH[0] = '0';
		MTSc[ts].HH[1] = '0';
		
		MTSc[ts].MN[0] = '0';
		MTSc[ts].MN[1] = '0';
		
		MTSc[ts].SS[0] = '0';
		MTSc[ts].SS[1] = '0';
		
		MTSc[ts].MLS[0] = '0';
		MTSc[ts].MLS[1] = '0';
		MTSc[ts].MLS[2] = '0';   
		  
		MTSc[ts].Seqnumber = 0;		
	}
	
}

void TCDdatainitialize()
{
#ifdef PIRun   // test to see if compiling for running on the PI
   
   wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers    
   
   RFidpinconfig();	// configure inputs for RF id detectors
   
   
    
   pinMode(TRSDinput, INPUT);   // 
  
   pinMode(TF1input, INPUT);    // 
   pinMode(TF2input, INPUT);    //  
   
   pinMode(IN4outpin, OUTPUT);    // Set Section5 pin as INPUT
   pinMode(IN3outpin, OUTPUT);    // Set Section6 pin as INPUT
   //pinMode(MTRpwmpin, OUTPUT);    // Set Motor control pin as OUTPUT mode   
   pinMode(MTRpwmpin, PWM_OUTPUT);    // Set Motor control pin as PWM mode   
   
   //pinMode(S1switchpin, INPUT);    // Set Switch 1 as input    
   //pinMode(S2switchpin, INPUT);    // Set Switch 2 as input    
   //pinMode(S3switchpin, INPUT);    // Set Switch 3 as input    
   //pinMode(S4switchpin, INPUT);    // Set Switch 4 as input    
     
   //pinMode(TTReedreset, OUTPUT);
   
   pinMode(TF1Rpin, 	OUTPUT);
   pinMode(TF2Rpin, 	OUTPUT);
   
   digitalWrite(IN4outpin, LOW);
   digitalWrite(IN3outpin, LOW);
   //digitalWrite(MTRpwmpin, LOW);
   pwmWrite(MTRpwmpin, 0);
   
   //digitalWrite(TTReedreset, LOW);
   
   digitalWrite(TF1Rpin,     LOW);
   digitalWrite(TF2Rpin,     LOW);
    
   pullUpDnControl(TRSDinput, PUD_UP); // Enable pull-up resistor on TFFR flag input
  
   pullUpDnControl(TF1input, PUD_UP); // Enable pull-up resistor 
   pullUpDnControl(TF2input, PUD_UP); // Enable pull-up resistor 
   
   //pullUpDnControl(S1switchpin, PUD_UP); // Enable pull-up resistor on switch 1
   //pullUpDnControl(S2switchpin, PUD_UP); // Enable pull-up resistor on switch 2
   //pullUpDnControl(S3switchpin, PUD_UP); // Enable pull-up resistor on switch 3
   //pullUpDnControl(S4switchpin, PUD_UP); // Enable pull-up resistor on switch 4     
    
   L293IN4low();
   L293IN3low();
   L293pwmlow();
   

    
   ResetTTendstopreedswitch();
   ResetTF1R();
   ResetTF2R();
   L293pwmlow();
   L293IN4low();
   L293IN3low();     
   
   //L293IN4high();
   //L293IN3high();
   //L293pwmhigh(880);
   
   printf ("\n\nLNreader GPIO initialization completed ....Feb 2022 \n");
#else
   printf("\n\nLNreader GPIO initialization SKIPPED as not on a PI \n");
#endif	
}

void ResetTTendstopreedswitch()
{
	#ifdef PIRun   // test to see if compiling for running on the PI   
	//digitalWrite(TTReedreset, HIGH);
	//printf("\nReset reed switch FF\n");
	//digitalWrite(TTReedreset, LOW);	
	#endif
}

void ResetTF1R()
{
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(TF1Rpin, HIGH);
	digitalWrite(TF1Rpin, LOW);
	#endif	
}

void ResetTF2R()
{
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(TF2Rpin, HIGH);
	digitalWrite(TF2Rpin, LOW);	
	#endif
}

void L293pwmhigh(int speed)
{
	#ifdef PIRun   // test to see if compiling for running on the PI 
	printf("\n457 - serial.c  - Speed set to:%i",speed); 	
	pwmWrite(MTRpwmpin, speed);
	#endif
}

void L293pwmlow()
{
	#ifdef PIRun   // test to see if compiling for running on the PI 	
	pwmWrite(MTRpwmpin, 0);
	#endif
}

void L293IN4high()
{	
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(IN4outpin, HIGH);
	#endif
}

void L293IN4low()
{	
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(IN4outpin, LOW);
	#endif
}

void L293IN3high()
{
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(IN3outpin, HIGH);
	#endif
}

void L293IN3low()
{
	#ifdef PIRun   // test to see if compiling for running on the PI   
	digitalWrite(IN3outpin, LOW);
	#endif
}

void TTMotorRun(int direction,int speed)
{
	if ((direction == 0) && (speed > 0))
	{
		//digitalWrite(TTdirection, LOW);
	}
	else
	{
		//digitalWrite(TTdirection, HIGH);
	}	
	
	// set motor speed 
		
	if (speed == 1)
	{
		//digitalWrite(TTStopGo, HIGH);
		//digitalWrite(TTFastSlow, LOW);
	}
	else 
	{
		if (speed == 2)
		{
		//	digitalWrite(TTStopGo, HIGH);
		//	digitalWrite(TTFastSlow, HIGH);
		}
		else
		{	// Motor stop 		
		//	digitalWrite(TTStopGo, LOW);
		//	digitalWrite(TTdirection, LOW);	
		}
	}
	
	
}

void GetBCMrawdata(unsigned char* BCMdataptr)
{
	for (int bcc = 0;bcc < BCMconnections; bcc++)
	{
		*(BCMdataptr + bcc) = BCMGPIOdata[bcc];
	}
}
/*
void TCDdatarequest(unsigned char* TCDdataptr, unsigned int* InPsectionptr)
{	
	#ifdef PIRun			// test to see if running on a PI
	
	TCDPsectionptr = InPsectionptr;
	
	unsigned char Sectionvalue = 0;	
	
	if (digitalRead(TRSDinput))
		{
			Sectionvalue = Sectionvalue + 1;
		}	
		
	if (digitalRead(TF1input))
		{
			Sectionvalue = Sectionvalue + 2;
		}
		
	if (digitalRead(TF2input))
		{
			Sectionvalue = Sectionvalue + 4;
		}
		
	//if (digitalRead(S1switchpin))
	//	{
	//		Sectionvalue = Sectionvalue + 0X08;		
	//	}	
		
	//if (digitalRead(S2switchpin))
	//	{
	//		Sectionvalue = Sectionvalue + 16;
	//	}	
		
	//if (digitalRead(S3switchpin))
	//	{
	//		Sectionvalue = Sectionvalue + 32;
	//	}	
		
	//if (digitalRead(S4switchpin))
	//	{
	//		Sectionvalue = Sectionvalue + 64;
	//	}		
	
	Databuffer[1] = 0XAB;
	Databuffer[2] = Sectionvalue;
	
	//printf("\nInputs value -> %02X ",Sectionvalue);
	
	*(TCDdataptr + 1) = Databuffer[1];
	*(TCDdataptr + 2) = Databuffer[2];	
	
	ProcessTCDreadings();
	#endif
	
	
	return;	
}
*/
void TCDbuffersclear()
{
	int tcd = 0;
	
	int tcdbitcnt = 0;
	
	for (tcd = 0; tcd < TCDBUFFERCOUNT;  tcd++)
	{
		for (tcdbitcnt = 0;tcdbitcnt<TCDBUFFERBITCOUNT;tcdbitcnt++)
		{
			TCDdatabufferconfig[tcd].DataBufferbit[tcdbitcnt] = 0;	
			
			TCDdatabufferconfig[tcd].Databit_type[tcdbitcnt] = 0;	
			
			TCDdatabuffervalues[tcd].bitvalues[tcdbitcnt] = 0;
		}		
	}	
}

void DisplayTCDdatabuffervalues()
{
	int tcd = 0;
	
	//int tcdbitcnt = 0;
	
	printf("\n\nTCD buffer values details");
	
	for (tcd = 0; tcd < TCDBUFFERCOUNT;  tcd++)
	{			
		printf("\nBuffer %02d bitv0-%02d bitv1-%02d bitv2-%02d bitv3-%02d bitv4-%02d bitv5-%02d bitv6-%02d bitv7-%02d", tcd,TCDdatabuffervalues[tcd].bitvalues[0],\
		TCDdatabuffervalues[tcd].bitvalues[1],TCDdatabuffervalues[tcd].bitvalues[2],TCDdatabuffervalues[tcd].bitvalues[3],\
		TCDdatabuffervalues[tcd].bitvalues[4],TCDdatabuffervalues[tcd].bitvalues[5],TCDdatabuffervalues[tcd].bitvalues[6],\
		TCDdatabuffervalues[tcd].bitvalues[7]);
	}
}

int DisplayTCDdatabufferconfig(char* messagereply)
{
	int tcd = 0;
	
	char outputrec[DISPLAYOUTPUTMSGBUFFERSIZE];

	int isc,ic,scc,mcc;

	mcc = 0;

	bzero(outputrec,DISPLAYOUTPUTMSGBUFFERSIZE);
	
	//printf("\n\nTCD buffer config details");
	
	for (tcd = 0; tcd < TCDBUFFERCOUNT;  tcd++)
	{	
		sprintf(outputrec,"\nBuffer %02d bit0-%02d-type-%01d bit1-%02d-type-%01d  bit2-%02d-type-%01d  bit3-%02d-type-%01d  bit4-%02d-type-%01d  bit5-%02d-type-%01d bit6-%02d-type-%01d bit7-%02d-type-%01d", tcd,\
		TCDdatabufferconfig[tcd].DataBufferbit[0],TCDdatabufferconfig[tcd].Databit_type[0],\
		TCDdatabufferconfig[tcd].DataBufferbit[1],TCDdatabufferconfig[tcd].Databit_type[1],\ 
		TCDdatabufferconfig[tcd].DataBufferbit[2],TCDdatabufferconfig[tcd].Databit_type[2],\
		TCDdatabufferconfig[tcd].DataBufferbit[3],TCDdatabufferconfig[tcd].Databit_type[3],\
		TCDdatabufferconfig[tcd].DataBufferbit[4],TCDdatabufferconfig[tcd].Databit_type[4],\
		TCDdatabufferconfig[tcd].DataBufferbit[5],TCDdatabufferconfig[tcd].Databit_type[5],\
		TCDdatabufferconfig[tcd].DataBufferbit[6],TCDdatabufferconfig[tcd].Databit_type[6],\
		TCDdatabufferconfig[tcd].DataBufferbit[7],TCDdatabufferconfig[tcd].Databit_type[7]);
		
		isc =  strlen(outputrec);		

		for (ic = 0;ic <isc;ic++)
		{
			*(messagereply + mcc) = outputrec[ic];
			mcc++;
		}
	}
}

void TCDdatabufferupdate(int buffer, int bit, int section, int type)
{
	// stores config details
	
	TCDdatabufferconfig[buffer].DataBufferbit[bit] = section;
	TCDdatabufferconfig[buffer].Databit_type[bit] = type;	
}

void ProcessTCDreadings()
{
	/* process data read from I2C interface and update PSection and Optical detector status */
	
	int tcd = 0;
	
	int TCDsection;	
	
	char Datavalueread;
	
	int dataresult; 
	
	char Maskingbitpattern = 0X01;
	
	char bitshiftcounter;
	
	int tcdbitcnt = 0;
	
	for (tcd = 0; tcd < TCDBUFFERCOUNT;  tcd++)  // loop through each buffer as configured 
	{
		//srcedata = 0x01;
		
		for (tcdbitcnt = 0;tcdbitcnt<TCDBUFFERBITCOUNT;tcdbitcnt++)  // for each bit in the 8 bit buffer 
		{				
			if (TCDdatabufferconfig[tcd].DataBufferbit[tcdbitcnt] > 0)   // if the bit is configured , read data value from BCMGPIO array 
			{
				//printf("\n1194 Buffer:%i Config buffer bit:%i value:%i",tcd,tcdbitcnt,TCDdatabufferconfig[tcd].DataBufferbit[tcdbitcnt]);
				
				Maskingbitpattern = 0X01;			     // generate bit masking pattern need to extract the data 
				
				for (int bitval = 0;bitval < tcdbitcnt ; bitval++)
				{
					Maskingbitpattern = Maskingbitpattern<<1;
				}
				
				Datavalueread = BCMGPIOdata[tcd] &  Maskingbitpattern; 
				
				if (Datavalueread > 0)
				{
					dataresult = 0;					
				}
				else
				{
					dataresult = 1;
				}
				
				TCDsection = TCDdatabufferconfig[tcd].DataBufferbit[tcdbitcnt];
				
				Physicalsection[TCDsection].Occupied = dataresult;
				
				*(Psectionptr+TCDsection) = dataresult;	
				
				Ctriggerlogicalupdate(1);  		
				
				  // 
				//printf("\n1204 Buffer Number:%02X ",tcd);
				//printf("\n1205 Masking pattern for data extract:%02X",Maskingbitpattern);
				//printf("\n1208 BCMGPIOdata for buffer %i -> %02X",tcd,BCMGPIOdata[tcd]);
				//printf("\n1209 Bit data value:%02X",Datavalueread);
				//printf("\n1223 PSection %i Dataresult:%i",TCDsection,dataresult);
			}		
			
			
		}		
	}		
	
}
 
void PointDCCcodeupdate(int ptc, int OC, unsigned char CodeDCC[4])
{
	//printf("\n\n1167 Serial.c storing DCC code for ptc:%02d OC:%01d\n",ptc,OC);

	PointDCCcodetable[ptc][OC].Pointnumber 	= ptc;
	PointDCCcodetable[ptc][OC].Openclose 	= OC;
	PointDCCcodetable[ptc][OC].DCCcode[0] = CodeDCC[0];
	PointDCCcodetable[ptc][OC].DCCcode[1] = CodeDCC[1];
	PointDCCcodetable[ptc][OC].DCCcode[2] = CodeDCC[2];
	PointDCCcodetable[ptc][OC].DCCcode[3] = CodeDCC[3];
	
	PointDCCcodetable[ptc][OC].PtDCCcodehashval = (CodeDCC[3] & 0xFF) + ((CodeDCC[2] & 0xFF) * 2) + ((CodeDCC[1] & 0xFF) * 4) + ((CodeDCC[0] & 0xFF) * 8);
	
	Pointswitchstatusupd(ptc,1);			
}

void Switchcmdprocess(int pt,int OC,unsigned char CodeDCC[4])
{
	//printf("\nPoint %i OC %i switching message sent %02X %02X %02X %02X\n",pt, OC, CodeDCC[0],CodeDCC[1],CodeDCC[2],CodeDCC[3]);


	if (Serialportstatus > 0)
	{
		write (ACM0_fd_serialport, CodeDCC, 4);
	//	printf("\nPoint switching message sent %02X %02X %02X %02X\n",CodeDCC[0],CodeDCC[1],CodeDCC[2],CodeDCC[3]);
	}
	else
	{
		printf("\nSerial port closed for point:%02d  OCflag: %01d switching message\n",pt,OC);
		printf("\nPoint switching message sent %02X %02X %02X %02X\n",CodeDCC[0],CodeDCC[1],CodeDCC[2],CodeDCC[3]);
	}		 
}

unsigned char GetDCCcodechar(int ptc, int OC, int charnum)
{
	return PointDCCcodetable[ptc][OC].DCCcode[charnum];
}

void Pointswitchstatusupd(int pt, int status)
{
	Pointswitchstatus[pt].Lastupdate = status;
	
	current_time = time(NULL);
    
	c_time_string_str = ctime(&current_time);  				
				
	//SetTimestamp(PTS_TIMESTAMP, status, ctime(&current_time));
}

void Pointswitchstatusinit()
{
	// Pointswitchstatus[NUMBEROFPOINTS];
	
	int scp;
	
	for (scp = 0;scp < NUMBEROFPOINTS;scp++)
	{
		Pointswitchstatus[scp].Lastupdate = 1;
	}
}



void DisplaySCPointDCC(int ptcc)
{
	int OC;
	int ptc;	
	
	printf("\nC code display of DCC codes\n\n");
	
	for (ptc = 0;ptc < NUMBEROFPOINTS;ptc++)
	{
		for (OC = 0; OC < PTOPENCLOSEFLAG;OC++)
		{	
			sprintf(DCCcodeshow,"Ptc:%03d:OC: %i: DCC: %02X %02X %02X %02X",PointDCCcodetable[ptc][OC].Pointnumber, PointDCCcodetable[ptc][OC].Openclose, PointDCCcodetable[ptc][OC].DCCcode[0],\
			PointDCCcodetable[ptc][OC].DCCcode[1], PointDCCcodetable[ptc][OC].DCCcode[2], PointDCCcodetable[ptc][OC].DCCcode[3]); 
	
			//if (PointDCCcodetable[ptc][OC].Pointnumber > 0)
			//{
			printf("\nCode->%s",DCCcodeshow);
			//}
		}
	}
}

int GetACM0openstatus()
{
    return ACM0serialportopened;
}

int GetUSB0openstatus()
{
    return USB0serialportopened;
}

int GetBCMGPIOreads(char* BCMdata)
{
	for (int cc = 0; cc < BCMconnections;cc++)
	{
		*(BCMdata + cc) = BCMGPIOdata[cc];
	}
}

int OpenI2Cinterfaces()
{
    BCMopen_1 = 0;
    
    BCMopen_2 = 0;
    
    int I2Crc_1;
    
    int I2Crc_2;

    // BCMGPIOdata[BCMconnections];  - initialize the BCM interface buffer

    for (int BCMcount = 0; BCMcount < BCMconnections; BCMcount++)
    {
    	BCMGPIOdata[BCMcount] = 0xFF;
    }

//#ifdef PIRun

// Open MCP23017_1 connection 

    fdBCM_1 = wiringPiI2CSetup(MCP23017_ADDRESS_1); 
    
    printf("\n\n1318 fdBCM_1 value %04X",fdBCM_1);         
    
    I2Crc_1 = wiringPiI2CWriteReg8(fdBCM_1,0x05,0X00);   // set IOCON register as BANK = 0    
    
    wiringPiI2CWriteReg8(fdBCM_1,0x00,0XFF);   // set IODIRA = 0xFF - all GPIOA pins as inputs
    
    wiringPiI2CWriteReg8(fdBCM_1,0x01,0XFF);   // set IODIRB = 0xFF - all GPIOB pins as inputs
    
    wiringPiI2CWriteReg8(fdBCM_1,0x0C,0XFF);   // set GPPUA  = 0xFF - set all GPIOA inputs with pull up resistor 
    
    wiringPiI2CWriteReg8(fdBCM_1,0x0D,0XFF);   // set GPPUB  = 0xFF - set all GPIOB inputs with pull up resistor     
    
    // read GPIOA values    
        
    GPIOAinputs = wiringPiI2CReadReg8(fdBCM_1,0x12);           
        
    // read GPIOB value         

    GPIOBinputs = wiringPiI2CReadReg8(fdBCM_1,0x13);  	
    
    printf("\n\n1294 GPIOA:%02X GPIOB:%02X  Rc:%02X\n\r",GPIOAinputs,GPIOBinputs,I2Crc_1);	
    
    if (I2Crc_1 == 0)
    {
	BCMopen_1 = 1;  
	printf("\n\r1280 BCM_1 connection OPEN\r\n");
    }
    else
    {	
	BCMopen_1 = 0;  
	printf("\n\r1280 BCM_1 connection CLOSED\r\n");
    }
    
    // Open MCP23017_2 connection 

    fdBCM_2 = wiringPiI2CSetup(MCP23017_ADDRESS_2); 
    
    printf("\n\n1324 fdBCM_2 value %04X",fdBCM_2);  
    
    I2Crc_2 = wiringPiI2CWriteReg8(fdBCM_2,0x05,0X00);   // set IOCON register as BANK = 0    
    
    wiringPiI2CWriteReg8(fdBCM_2,0x00,0XFF);   // set IODIRA = 0xFF - all GPIOA pins as inputs
    
    wiringPiI2CWriteReg8(fdBCM_2,0x01,0XFF);   // set IODIRB = 0xFF - all GPIOB pins as inputs
    
    wiringPiI2CWriteReg8(fdBCM_2,0x0C,0XFF);   // set GPPUA  = 0xFF - set all GPIOA inputs with pull up resistor 
    
    wiringPiI2CWriteReg8(fdBCM_2,0x0D,0XFF);   // set GPPUB  = 0xFF - set all GPIOB inputs with pull up resistor     
    
    // read GPIOA values    
        
    GPIOCinputs = wiringPiI2CReadReg8(fdBCM_2,0x12);           
        
    // read GPIOB value         

    GPIODinputs = wiringPiI2CReadReg8(fdBCM_2,0x13);  	
    
    printf("\n\n1387 GPIOC:%02X GPIOD:%02X  Rc:%02X\n\r",GPIOCinputs,GPIODinputs,I2Crc_2);	
    
    if (I2Crc_2 == 0)
    {
	BCMopen_2 = 1;  
	printf("\n\r1280 BCM_2 connection OPEN\r\n");
    }
    else
    {	
	BCMopen_2 = 0;  
	printf("\n\r1280 BCM_2 connection CLOSED\r\n");
    }
    
//#endif

    printf("\n\r1343 BCMopen_1 flag = %02X",BCMopen_1);
    
    printf("\n\r1343 BCMopen_2 flag = %02X",BCMopen_2);
    
    return 0;
}


int SerialUSB0comms(unsigned int* InPsectionptr, unsigned int* InPDetectorptr, unsigned int* InSlotcodeptr, unsigned int* InSlotdirectionptr, unsigned int* InSlotspeedptr, unsigned int* InPointstatusflagptr,\
 struct MsgTimestamp* InMSGptr,unsigned int * InPsectionsEptr, unsigned int * InPsectionsXptr, unsigned int * InUSBbytecntptr, unsigned char * InUSBbytebufferptr, \
 unsigned int* RWTEPsectionsptr, unsigned int* RWTXPsectionsptr, unsigned int* RWTCPsectionsptr)
 {
	printf ("\nSerialUSB0comms port starting 934...version -  March 8th 2022\n");  
	
	/*
	 ******************************************************************************************************** 
	       NOTE - If connection does not work check USB is recognized by PI - may need a reboot  - Feb 2022 
	 ******************************************************************************************************** 
	 * 
	 */
	 
	int RC = 0;
		
	fd_UART = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK); //O_RDWR | O_NOCTTY | O_NONBLOCK
	
    if (fd_UART < 0) {
        printf("LNserial 657 Error opening %s: %s\n", portname, strerror(errno));
        return 2;
    }
    
    printf("\nLNserial 661 %s Serial port opened successfully - Date and time stamp->%s \n",portname, c_time_string_str);   
    
    USB0serialportopened = 1;		// set flag to show ACM0 port is opened  
    
    
    //BTinitialize();			// Open Bluetooth connection
    
    
    
    UART_PiD = (int)getpid();
    
    printf("\nLNserial 667 -> PID for UART interface is %i ",UART_PiD);
    
    /*baudrate 4800, 8 bits, no parity, 1 stop bit */
   // set_interface_attribs(fd, B4800);
    
    int speed = B4800;
    
    struct termios tty;
    
    struct sigaction UARTsaio;  	// structure for signal handling
    
    // fetch existing attributes for serial port in fd_UART

    if (tcgetattr(fd_UART, &tty) < 0) {
        printf("603 Error from tcgetattr for UART: \n");
        return -1;
    }
    
    UARTsaio.sa_handler = signal_handler_IO;
    sigemptyset(&UARTsaio.sa_mask);   //saio.sa_mask = 0;
    UARTsaio.sa_flags = 0;
    UARTsaio.sa_restorer = NULL;
    sigaction(SIGIO,&UARTsaio,NULL);
    
    fcntl(fd_UART, F_SETOWN, getpid());
      
      // Make the file descriptor asynchronous (the manual page says only
      // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
      
    fcntl(fd_UART, F_SETFL, FASYNC);
    
    /*
	No parity (8N1):
	options.c_cflag &= ~PARENB
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
    */
    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);
    
    // cflag - settings for character processing 
    
    // iflag - setting for input processing 
    
    // oflag - setting for output processing 
    
    /*
	 set new port settings for canonical input processing 
      
      newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
      newtio.c_iflag = IGNPAR;
      newtio.c_oflag = 0;
      newtio.c_lflag = 0;       //ICANON;
      newtio.c_cc[VMIN]=1;
      newtio.c_cc[VTIME]=0;
      tcflush(fd_serialport, TCIFLUSH);
      tcsetattr(fd_serialport,TCSANOW,&newtio);




     */

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    //tty.c_lflag = 0; 
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 0;		// number of bytes required for a read 
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd_UART, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr on UART: %s\n", strerror(errno));
        return -1;
    }
      
    tcdrain(fd_UART);    /* flush clear the UART */
    
    //char PiUARTmsgout[10] = {'\n','U','A','R','T',' ','O','K','\n','\r'};   // sending opening message to Turntable to trigger response
    
    //PiUARTWritemessage(10, PiUARTmsgout); 
	
    
    return RC;
 }
	
int Serialcomms(unsigned int* InPsectionptr, unsigned int* InPDetectorptr, unsigned int* InSlotcodeptr, unsigned int* InSlotdirectionptr, unsigned int* InSlotspeedptr, unsigned int* InPointstatusflagptr,\
 struct MsgTimestamp* InMSGptr,unsigned int * InPsectionsEptr, unsigned int * InPsectionsXptr, unsigned int * InUSBbytecntptr, unsigned char * InUSBbytebufferptr, \
 unsigned int* RWTEPsectionsptr, unsigned int* RWTXPsectionsptr, unsigned int* RWTCPsectionsptr, char* PsectionDCCfilenameptr)
{
	
   TEPsectionptr = RWTEPsectionsptr;
   TXPsectionptr = RWTXPsectionsptr;
   TCPsectionptr = RWTCPsectionsptr;
   
   Psectionptr = InPsectionptr;
   PDetectorptr = InPDetectorptr;
   Slotcodeptr = InSlotcodeptr;
   Slotdirectionptr = InSlotdirectionptr;
   Slotspeedptr = InSlotspeedptr;
   Pointstatusflagptr = InPointstatusflagptr;
   MSGptr = InMSGptr;
   Psectionentryptr = InPsectionsEptr;
   Psectionexitptr 	= InPsectionsXptr;
   USBbytecntrptr 	= InUSBbytecntptr;
   USBbytebufferptr = InUSBbytebufferptr;
   
   printf ("\n\n LNserial 777 Serialcomms starting ..Version for UART and ACM0 on --> March 8th 2022 \n");     

   InitializeTimestamp();    
   
   time_t now = time(NULL);
   
   strftime(Tbuff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));     
   
   bzero(signalbuffer,SIGNALCHARMAX);		// clear inbound signal character buffer;
   
   Signalcharcount = 0;
   
   ACM0_Signalcharcount = 0;

    /* Obtain current time.  */
    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }    
    
    /* Convert to local time format  */
    /*
    #define PS_TIMESTAMP            1
	#define PD_TIMESTAMP            2
	#define SLOT_TIMESTAMP          3
	#define PTS_TIMESTAMP           4
                                      */
                                      
   current_time = time(NULL);
    
   c_time_string_str = ctime(&current_time);   
    
   SetTimestamp(PS_TIMESTAMP, 1, ctime(&current_time));  
    
   //SetTimestamp(PTS_TIMESTAMP, 1, ctime(&current_time));    
   
   GetDatetimestamp();      
  
   Allslotsloaded = 0;
   
   InitializeslotArray();     
   
   int res, error;   
   
   int UARTchars; 
   
   int ACM0chars;
   
   // Setup structure for termios configuration
  
   struct termios oldtio, newtio;       	//place for old and new port settings for serial port
   
   struct termios ACM0_oldtio, ACM0_newtio;       	//place for old and new port settings for serial port
   //struct termios; //oldkey, newkey;       	//place tor old and new port settings for keyboard teletype
   
   // Setup structure for signal processing configuration
   
   struct sigaction saio;               	//definition of signal action  
   
   struct sigaction ACM0_saio;               	//definition of signal action  
   
   unsigned char Uploadbuff[UPLOADBUFFCHARS];			// chars uploaded to the C++ program   
   
   error=0;
   
   //BTinitialize();						// Open Bluetooth connection
   
   CheckforRFidUSBports(); 					// check for RFid USB receivers connected 
   
   OpenI2Cinterfaces();
   
   if (error==0)  
   {         
   
      BAUD 		= ACM0baud;
      DATABITS 		= ACM0databits;
      STOPBITS 		= ACM0stopbits;
      PARITYON 		= ACM0parityon;
      PARITY 		= ACM0parity;
      Format 		= ACM0format;   
      
      //open the device(com port) to be non-blocking (read will return immediately)
      
      ACM0_fd_serialport = open(LOCONETDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);     
      
      if (ACM0_fd_serialport < 0)
      {
	 printf("\nLNserial 875 Device ACM0 not found \n");
         perror(devicename);
        // return 1;
        // exit(-1);
      }      
      else
      {
		printf("\nLNserial 881 ACM0 Serial port opened successfully - Date time stamp->%s \n", c_time_string_str);    
      
		Serialportstatus = 1;
	  }
      
      SerialCharsread = 0;

      //install the serial handler before making the device asynchronous
      
      ACM0_PiD = (int)getpid();
      
      printf("\n824 -> PID for ACMO %i ",ACM0_PiD);      
      
      ACM0serialportopened = 1;		// set flag to show ACM0 port is opened 
      
      ACM0_saio.sa_handler = signal_handler_IO_ACM0;
      sigemptyset(&ACM0_saio.sa_mask);   //saio.sa_mask = 0;
      ACM0_saio.sa_flags = 0;
      ACM0_saio.sa_restorer = NULL;
      sigaction(SIGIO,&ACM0_saio,NULL);

      // allow the process to receive SIGIO      
     
      fcntl(ACM0_fd_serialport, F_SETOWN, getpid());
      
      // Make the file descriptor asynchronous (the manual page says only
      // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
      
      fcntl(ACM0_fd_serialport, F_SETFL, FASYNC);
      
      tcgetattr(ACM0_fd_serialport,&ACM0_oldtio); // save current port settings 
      
      // set new port settings for canonical input processing 
      
      ACM0_newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
      ACM0_newtio.c_iflag = IGNPAR;
      ACM0_newtio.c_oflag = 0;
      ACM0_newtio.c_lflag = 0;       //ICANON;
      ACM0_newtio.c_cc[VMIN]=0;
      ACM0_newtio.c_cc[VTIME]=1;
      tcflush(ACM0_fd_serialport, TCIFLUSH);
      tcsetattr(ACM0_fd_serialport,TCSANOW,&ACM0_newtio);
      
      sleep(2);  
      
      slotcounter = 1;
      
      UploadDCCslots();
      
      int* sensorstateptr = &sensorstate[0];
      
      while (1)
      {	
	 usleep(1000);				// sleep for a millisecond to manage load on CPU
	 
	 
        if (Ctrigger > 0)			//  check to see if any infra-red sensors have changed state 
        {	
          //  printf("\nC-> 1: %02d 2: %02d 3: %02d 4: %02d\n",Int1counter,Int2counter,Int3counter,Int4counter);
          
            /* Obtain current time stamp   */
            //current_timenow = time(NULL);
            //c_timenow_string = ctime(&current_timenow);
            //bzero(Timestamp,9);
    
           /*   extract(char *src,char *det,int pos,int len)               */
    
            //extract(c_timenow_string,Timestamp,12,8);           
            
            // get current state of each sensor
            
        	Initsensorstate();
            
        	Getsensorstate();
	    
        	IRdetectordataupdate(sensorstateptr);
            		
            Ctrigger = 0;
        }	

     // check to see if any BCM based track circuit data to be loaded

     if (BCMopen_1 > 0)
     {
//#ifdef PIRun
    	 // read GPIOB values

    	 GPIOAinputs = wiringPiI2CReadReg8(fdBCM_1,0x12);

    	 BCMGPIOdata[0] = GPIOAinputs;

    	 // read GPIOB value

    	 GPIOBinputs = wiringPiI2CReadReg8(fdBCM_1,0x13);

    	 BCMGPIOdata[1] = GPIOBinputs;
    	 
//#endif
     }
     else
     {
    	// BCM not open so set read values to 0xFF e.g circuit occupied

    	 GPIOAinputs = 0xFF;

    	 GPIOBinputs = 0xFF;

    	 BCMGPIOdata[0] = 0xF0;

    	 BCMGPIOdata[1] = 0xF1;    	 
     }
     
      if (BCMopen_2 > 0)
     {
//#ifdef PIRun
    	 // read GPIOC values

    	 GPIOCinputs = wiringPiI2CReadReg8(fdBCM_2,0x12);

    	 BCMGPIOdata[2] = GPIOCinputs;

    	 // read GPIOB value

    	 GPIODinputs = wiringPiI2CReadReg8(fdBCM_2,0x13);
	 
    	 BCMGPIOdata[3] = GPIODinputs;
	 
	 ProcessTCDreadings();
    	 
//#endif
     }
     else
     {
    	// BCM not open so set read values to 0xFF e.g circuit occupied

    	 GPIOCinputs = 0xFF;

    	 GPIODinputs = 0xFF;    	 

    	 BCMGPIOdata[2] = 0xF2;

    	 BCMGPIOdata[3] = 0xF3;
     }
		  	
	 if ((wait_flag==FALSE) ||  (wait_flag_ACM0==FALSE))						//if input is available
         {
	   ACM0chars = 0;
	   
	   UARTchars = 0;
	   
	   ACM0Rxbytes = 0;
	   
	   UARTRxbytes = 0;
	   
	   // Use system command to check how many characters in input buffers for ACM0 and UART
	   
	   ioctl(ACM0_fd_serialport,FIONREAD, &ACM0Rxbytes);
	   
	   ioctl(fd_UART,FIONREAD, &UARTRxbytes);	   
	   
	   // check which USB ports have characters waiting to be processed 
            
           for (int pc = 0; pc < Numberofportsfound; pc++)
		{            
			Port_bytecount[pc] = 0;            
			ioctl(fd_serialport_ID[pc],FIONREAD,&Port_bytecount[pc]);
		}  	  
	  
	// check to see if any of the USB ports have characters to process	  
	   
	   int nUSBchars,bc; 
	   
	   for (Portsubscript = 0; Portsubscript < USBPORTCOUNT; Portsubscript++)
		{
		    if (Port_bytecount[Portsubscript] > 0)
			{
			nUSBchars = read(fd_serialport_ID[Portsubscript],Uartportbuffer[Portsubscript],24);  
           
				bc = 0;	
			
				if (nUSBchars < 0) 
				{
					printf("\nERROR reading from serial port "); 
				}
				else
				{						
					
					while(bc <= nUSBchars)  /* read all the data in the buffer */
					{					
					
						if (Uartportbuffer[Portsubscript][bc] == 0x02)
						{               
							uart_somflag[Portsubscript]  = 1;
    
							uart_eomflag[Portsubscript] =  0;
    
							uart_charcount[Portsubscript] = 0;                
                
							bzero(USBSerialCharinputbuffer[Portsubscript],SERIALCHARBUFSIZE);
							
						}						
					
						if (Uartportbuffer[Portsubscript][bc] == 0x03)     // test for end of message character 
						{
            
							uart_eomflag[Portsubscript] = 1;
							
							
                
							USBSerialCharinputbuffer[Portsubscript][uart_charcount[Portsubscript]] = 0x00;
						}
					
						if ((Uartportbuffer[Portsubscript][bc] != 0x02) && (Uartportbuffer[Portsubscript][bc] != 0x03) && (Uartportbuffer[Portsubscript][bc] != 0x00) && (Uartportbuffer[Portsubscript][bc] != 0x0D) && (Uartportbuffer[Portsubscript][bc] != 0x0A))
						{									
							USBSerialCharinputbuffer[Portsubscript][uart_charcount[Portsubscript]] = Uartportbuffer[Portsubscript][bc];                       
                
							uart_charcount[Portsubscript]++;					
						}					
						++bc;						
					}
				} 
				 
				bzero(Uartportbuffer[Portsubscript],USBSERIALBUFFREADCHARS);     /* all characters read so clear the serial input read buffer */   
				
				 // test to see if there is a complete message to send   
				    
				 if ((uart_somflag[Portsubscript] > 0) & (uart_eomflag[Portsubscript] > 0))
				 {	                  
					//Detectornumber = Portsubscript + 1;
					
					//printf("\nRF-id at device %i with %i chars:",Portsubscript,uart_charcount[Portsubscript]);
					
					//char RFidcode[RFIDCHARCOUNT];
					
					//for (int ccc = 0;ccc < uart_charcount[Portsubscript];ccc++)
					//{
					//	printf("%c",USBSerialCharinputbuffer[Portsubscript][ccc]);					
					//}					
					
					RFidmessageprocess(Portsubscript, USBSerialCharinputbuffer[Portsubscript]);
					
					//printf("\n");						
                  
					// tidy-up 		
            
					uart_somflag[Portsubscript] = 0;
					uart_eomflag[Portsubscript] = 0;            
					uart_charcount[Portsubscript] = 0;
					
					bzero(USBSerialCharinputbuffer[Portsubscript],SERIALCHARBUFSIZE);                
				}  
				 
			}   
		}	
		
	//   printf("\n 1591");
		 
	   bzero(ACM0Rxbuffer,ACM0BUFFERSIZE);    // clear read buffer before reading data 
	   
	   bzero(UARTRxbuffer,UARTBUFFERSIZE);    // clear read buffer before reading data           
	   
           SerialCharsread = SerialCharsread + ACM0chars;               
           
           if (ACM0Rxbytes > 0)   // ACMOchars - loop processing to read characters
             {
		//printf("\n 1597->Read ACM0: %i  bytes\n", ACM0Rxbytes);  
		
		ACM0chars = read(ACM0_fd_serialport,ACM0Rxbuffer,ACM0BUFFERSIZE);   			 
			   
		//*USBbytecntrptr = *USBbytecntrptr + ACM0chars;
		 	    
		 bzero(Uploadbuff,UPLOADBUFFCHARS);
		 	   
		 int isc;		
			   
		 for (isc=0; isc<ACM0chars; isc++)  	//for all chars in string
                 {     
			Uploadbuff[isc] = ACM0Rxbuffer[isc];
                
			Bufferstore(&SerialCharinputbuffer[0],SERIALCHARBUFSIZE, ACM0Rxbuffer[isc]);
			
			SDSHexstring(2, &SerialCharinputbuffer[0],'H',SERIALCHARBUFSIZE); 
		 } 			
		       		       
		 CDataupdate(Uploadbuff,ACM0chars); 		
		 
		 LNmessageproc(ACM0chars);	 // process messages in inbound buffer		
		 
		 bzero(signalbuffer,SIGNALCHARMAX);		// clear inbound signal character buffer;
   
		 Signalcharcount = 0;				 
		 
		 ACM0_Signalcharcount = 0;	   
			   
		 bzero(ACM0Rxbuffer,ACM0BUFFERSIZE);
		      
		 wait_flag = TRUE;
		 
		 wait_flag_ACM0 = TRUE;
	    }  // end of res loop processing
	    
	    
	   if (UARTRxbytes > 0)
	   {
		UARTchars = read(fd_UART,UARTRxbuffer,UARTBUFFERSIZE); 
		//printf("\n\n ====> 978->Read: %i UART chars\n", UARTchars); 
		
		for (int cc = 0; cc < UARTchars;cc++)
		{
			//printf("\nUART char number: %i char:%C hex: %02X",cc,UARTRxbuffer[cc],UARTRxbuffer[cc]);
			//printf("%C",UARTRxbuffer[cc]);
			//write (fd_UART, 'A', 1);
		}  
		
		//PiUARTWritemessage(UARTchars,UARTRxbuffer);		
		
		UARTdataupload(UARTRxbuffer,UARTchars);
		
		wait_flag = TRUE;
		Signalcharcount = 0;
	   } 	    
		   
	 }   // end of wait flag testing
	 
      }	// end of while loop 
  }
   
  return 0;   
   
}

int LNmessageproc(int res)
{
	int RC = 0;	
	
	int Msg4cnt  		= 0;
	int Msg14cnt		= 0;
	
	int Msg4processed 	= 0;
	int Msg14processed 	= 0;
	
	int Msgtype 		= 0;
	
	if (res % 4 == 0)
	{
		Msg4cnt 	= res / 4;		// test to see if 4 byte messages 
		Msgtype 	= 4;
	}
	else
	{
		if (res % 14 == 0)
		{
			Msg14cnt 	= res / 4;	// test to see if 14 byte messages 
			Msgtype 	= 14;
		}
		else
		{
			return 8;		// unrecognzed character count so return error code 
		}
	}
	
	if (Msgtype == 4)
	{
		int bc;
		int cd;
		
		while (Msg4processed < Msg4cnt)
		{
			bc = 4 * Msg4processed;
			cd = 0;				// subscript for messages to be processed 
			
			 for (bc = (4 * Msg4processed) ;bc < ((4 * Msg4processed) + 4);bc++)
				 {
					FourbyteDCC[cd] = ACM0Rxbuffer[bc]; 
					cd++;					
				 }			   
			   
			 if (FourbyteDCC[0] == 0xA0)  /* process DCC speed message */
				 {							 
				 	LNSpeedmessagesprocess(&FourbyteDCC[0]);							
				 }	
				 
			 if (FourbyteDCC[0] == 0xB0)  /* process DCC point switching message */
				 {

            		 	LNpointmessageprocess(&FourbyteDCC[0]);							
				 }					
				 
			 if (FourbyteDCC[0] == 0xA1)  /* process DCC direction message */
				 {							 
				 	LNSdirectionmessagesprocess(&FourbyteDCC[0]);								
				 }		
				 
			 if (FourbyteDCC[0] == 0xB2)  /* process DCC section code  */
				 {				
					LNSectionmessageprocess(&FourbyteDCC[0]);	// process the DCC code to set physical section indicator
					
					Loggingflag = 1;
					
					//Physectionlogsave();						/* Write physical section log file  */
					
					//LogicalSectionTcodeRefresh();				/* Check the logical section Tcode  */
					
					//LNLogicalsectionlogsave();    			/* Write logical section log file   */	
					
				 }					 
				      // ++Blockmsgtriggercnt
			
			Msg4processed++;
		}
		
		//printf("\n %i of the 4 byte messages processed ",Msg4processed);
	}
	
	if (Msgtype == 14)
	{			
		//printf("\nSlot data received %02X %02X \n", buf[4],buf[9]);
		//printf("\nLoco number %i \n", DCCLococodecalc(buf[4],buf[9]));	
				 
		DCCslotmessagesprocess(ACM0Rxbuffer);	
				 
		if (Allslotsloaded == 0)
		{
			 UploadDCCslots();
		}	 
		
		//printf("\n %i of the 14 byte messages processed ",Msg14processed);
	}
	
	return RC;
}

int Writemessage(unsigned char msgout[4])
{
	int rc = write (ACM0_fd_serialport, msgout, 4); 	
	
	return rc;
}

int PiUARTWritemessage(int mlen, unsigned char msgout[16])
{	
	int rc = write (fd_UART, msgout, mlen); 	
	return rc;
}

void signal_handler_IO (int status)
{  
  // Signal handler is shared across UART and ACM0 inputs 
   
   Signalcharcount++;	 
   
  // printf("\nRegular signal received - count %i",Signalcharcount);     
   
   wait_flag = FALSE;
}

void signal_handler_IO_ACM0 (int status)
{  
  // Signal handler is shared across UART and ACM0 inputs 
   
   ACM0_Signalcharcount++;	
   
  // printf("\nACM0 signal received - count %i",ACM0_Signalcharcount);   
   
   wait_flag_ACM0 = FALSE;
}

int str_split(unsigned char* a_str, const char a_delim, unsigned char* ls_str)
{
 
    unsigned char* tmp       = a_str;
 
    int charcount = 0;
    int delimcount = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
          //  count++;
            *ls_str = charcount;
            delimcount++;
            ls_str++;
         //   last_comma = tmp;
        }
        tmp++;
        charcount++;
    }  
    
    return delimcount;       
}

void Clearlinesplitbuffer()
{
	// char Linesplit[LINESPLITELEMENTS][LINESPLITELEMENTSLENGTH];
	
	int Lse = 0;
	int LseL = 0;	
	
	for (Lse = 0;Lse <= LINESPLITELEMENTS; Lse++)
	{
		for (LseL = 0; LseL <= LINESPLITELEMENTSLENGTH; LseL++)
		{
			Linesplit[Lse][LseL] = 0x0;				
		}		
	}	
}

int Buffercopy(unsigned char *src, unsigned char *dest, int charcount)
{
	int sc = 0;
	
	while((sc < charcount) && (*src != 0x0))
	{
		*dest = *src;
		src++;
		dest++;
		sc++;
	}	
	return sc;	/* return the number of bytes moved */
}

int HEXBuffercopy(unsigned char *src, unsigned char *dest, int charcount)
{
	int sc = 0;
	while(sc < charcount)
	{
		*dest = *src;
		src++;
		dest++;
		sc++;
	}	
	return sc;	/* return the number of bytes moved */
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

unsigned char AtoH(unsigned char bytein)
{
	int result = 0x0;	
	
	unsigned char *byte = &bytein;
	
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

void Bufferstore(unsigned char *s_ptr, unsigned char maxcnt, unsigned char value)
{
	unsigned char *max_ptr;
	max_ptr = s_ptr + maxcnt - 1;
	unsigned char *r_ptr;
	r_ptr = s_ptr + 1;	
	
	//printf("\nValue %x\n", value);
	//printf("\nSize of stored %d\n",sizeof(value));
	
	while (r_ptr < max_ptr)
	{	
		*s_ptr = *r_ptr; 
		s_ptr++;
		r_ptr++;		
	}
	*s_ptr = *r_ptr; 	
	*r_ptr = value;		 
}

void printbufferinhex(unsigned char *src, int cc)
{
	printf("\nBuf char in hex");
	int cm = 0;
	while(cm < cc)
	{
	printf("  %02X",src[cm]);
	cm++;
	}	
}

int SDSHexstring(int line, unsigned char *strmsg, unsigned char format, unsigned char NumberofBytes)	/*	Stores a string in the specified line and format     */
	{																		/* format values  :  A = ASCII format                    */                 
																			/*                   H = Hex displayed in ASCII          */		
																			/* 	Specify number of bytes to convert                   */
																			/* inserts ASCII spaces between converted bytes          */
		if (NumberofBytes < 1)	
		{
			return 1;
		}															
		unsigned char Bytesconverted = 1;	
																	
		unsigned char *det = &DA[line][0];
		unsigned char c;
		
		if (format == 'A')
		{
		
			while(*strmsg)
			{
			*det=*strmsg;
			strmsg++;
			det++;			
			}	
		}
		else
		{	
			while(Bytesconverted <= NumberofBytes)
			{
			c = (*strmsg & 0xF0)>>4;
			*det= HtoA(c); 
 			det++;
            c = *strmsg & 0x0F;
			*det= HtoA(c); 	
			det++;
			*det= 0x20;
			strmsg++;
			Bytesconverted++;
			det++;			
			}			
		}		
		
	*det='\0';	
	return 0;	
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

int LNStringtoInteger(unsigned char *stringin)
{
	
	unsigned char digits[6];
	bzero(digits,6);	
	
	unsigned char *digit_ptr = &digits[5];
	
	int result = 0;
	int len = strlen(stringin);
	unsigned char *strin_ptr = &stringin[len-1]; 
	
	int sl = 0;
	for (sl = 0;sl < len;sl++)
	{
		if (*strin_ptr > 0)
		{
			*digit_ptr = (*strin_ptr - 0x30);			
		}
		digit_ptr--;
		strin_ptr--;		
	}
	
	result = digits[5] + (10 * digits[4]) + (100 * digits[3]) + (1000 * digits[2]) + (10000 * digits[1]) + (100000 * digits[0]);
	
	//printf("\n1342 String %i chars \n", len);
	//printbufferinhex(digits,6);	
	return result;
}

void Physicalsectiondatadisplay(int psection)
{
	/*
	  struct SDetail 
    {    
    int  Occupied;
    char Entrycode[12];
    char EntryHexcode[4];     // Hex code when train enters 
    int  Entrycodehashval; 
    char Exitcode[12];
    char ExitHexcode[4];      // Hex code when train exits 
    int  Exitcodehashval;    
    };

    extern struct SDetail Physicalsection[PSECTIONS];
	  
	 */
	printf("\nPsection %02d Entry hex Entry code %s Entry hex Exit code %s ",psection, \
	Physicalsection[psection].Entrycode,\
	Physicalsection[psection].Exitcode);
	
}

void LNSpeedmessagesprocess(unsigned char* FourbyteDCC)					// processing inbound speed message for a slot 
 {
	 unsigned char *Data_ptr;
	 int slot = 0;	
	 
	 // Slotspeedptr
	 
	 Data_ptr = FourbyteDCC; 	 
	 
	 slot   = *(Data_ptr +1) & 0xFF;	
	 
	 unsigned char hexValue[2];
	 
	 //printf("\nSpeed message slot %02d speed %02X", slot, (*(Data_ptr +2) & 0xFF));
	 
	 hexValue[0] = (*(Data_ptr +2) & 0xF0)/16;
	 
	 hexValue[1] = *(Data_ptr +2) & 0x0F;
	 
	 //printf("\nHex 1 %02X Hex 2 %02X ", hexValue[0], hexValue[1]);
	 
	 int c1 =  hexValue[0];
	 int c2 =  hexValue[1];
	 int speedval = (c1 * 16) + c2;     
     
     //printf("  integer value is: %d\n",speedval);	 
	 
	 //SlotInformation[slot].ActSpeed = *(Data_ptr +2) & 0xFF;
	 
	 *(Slotspeedptr+slot) = speedval;	
	 
	// SetTimestamp(SLOT_TIMESTAMP, 1, ctime(&current_time));   	 
 }
 
 void LNSdirectionmessagesprocess(unsigned char* FourbyteDCC)
 {
	 unsigned char *Data_ptr;
	 
	 int slot = 0;	
	 
	 int directionflag = 0;	 
	 
	 Data_ptr = FourbyteDCC; 	 
	 
	 directionflag = *(Data_ptr +2) & 0x20;
	 
	 slot   = *(Data_ptr +1) & 0xFF;
	 
	 //printf("\nSlot direction message slot %02d direction code %02X", slot, (*(Data_ptr +2) & 0x20));
	 
	 if (directionflag > 0)
	 {
		 *(Slotdirectionptr+slot) = 1;
	 }
	 else
	 {
		 *(Slotdirectionptr+slot) = 0;
	 }	
	 
	// SetTimestamp(SLOT_TIMESTAMP, 1, ctime(&current_time));   
 }
 
 void LNpointmessageprocess(unsigned char* FourbyteDCC)
 {
	 unsigned char *Data_ptr;
	 //int sci = 0;
	 //int pointnmatch = 0;
	 
	 int LNpointcodehashval = 0; 
	 
	 Data_ptr = FourbyteDCC;
	 
	 /* routine to process Loconet serial data stream for switching commands  
	 struct PointDCCcodes
	{
		int Pointnumber;
		int Openclose;
		unsigned char DCCcode[4];
		int  PtDCCcodehashval;		
	};

	struct PointDCCcodes PointDCCcodetable[NUMBEROFPOINTS][PTOPENCLOSEFLAG];

	struct Pointstatusdata
	{
		int Lastupdate;  // 0 = no updates ,1 = initialized, 2 = opened, 3 = closed
	};

	struct Pointstatusdata Pointswitchstatus[NUMBEROFPOINTS]; 
	 
	 */	 
	 
	 LNpointcodehashval = ((*Data_ptr & 0xFF)  * 8)+ ((*(Data_ptr +1) & 0xFF) * 4) + ((*(Data_ptr +2) & 0xFF) * 2) + ((*(Data_ptr +3) & 0xFF) * 1);	
	 
	 int pmm,OCF; 
	 
	 for (pmm = 0;pmm < NUMBEROFPOINTS;pmm++)
	 {
		for (OCF = 0;OCF < PTOPENCLOSEFLAG;OCF++)
		{				
		 //printf("\nLook-up Table hashval Pt %i OC %i Hashval %i ", pmm,OCF,PointDCCcodetable[pmm][OCF].PtDCCcodehashval);
		 
		 if (LNpointcodehashval == PointDCCcodetable[pmm][OCF].PtDCCcodehashval)
		 {
			printf("\nPoint switch code matched for %i OC %i ",pmm,OCF);
			
			if (OCF == 0)
			{
				Pointswitchstatusupd(pmm,2);
				*(Pointstatusflagptr + pmm) = 2;
			}
			
			if (OCF == 1)
			{
				Pointswitchstatusupd(pmm,3);
				*(Pointstatusflagptr + pmm) = 3;
			}			
		 }
		 
		}
	 }
	 
	 //printf("\nPoint switching message sent \n");
 }
 
 int LNsectionEntryHexmsgcheck(unsigned char msghex[4],int tec)
 {
	 if ((msghex[0] == Physicalsection[tec].EntryHexcode[0]) && (msghex[1] == Physicalsection[tec].EntryHexcode[1])\
				 && (msghex[2] == Physicalsection[tec].EntryHexcode[2]) && (msghex[3] == Physicalsection[tec].EntryHexcode[3]))
	 {
		 return 8;
	 }
	 else
	 {
		 return 0;
	 }
 }

 int LNsectionExitHexmsgcheck(unsigned char msghex[4], int tec)
 {
	 if ((msghex[0] == Physicalsection[tec].ExitHexcode[0]) && (msghex[1] == Physicalsection[tec].ExitHexcode[1])\
				 && (msghex[2] == Physicalsection[tec].ExitHexcode[2]) && (msghex[3] == Physicalsection[tec].ExitHexcode[3]))
	 {
		 return 8;
	 }
	 else
	 {
		 return 0;
	 }
 }

 void LNSectionmessageprocess(unsigned char* FourbyteDCC)
 {
	 unsigned char *Data_ptr;
	 int sci = 0;
	 int sectionmatch = 0;
	 unsigned char sectionmarchdirection;
	 int LNsectioncodehashval = 0; 
	 int NewLNsectioncodehashval = 0;
	 
	 unsigned char LNHexbitcode[4];
	// unsigned int LNshiftedHexbitcode[4];

	 Data_ptr = FourbyteDCC;
	 
	 /* routine to process Loconet serial data stream for section data */
	 //printf("\n2189 Section msg %02X %02X  %02X  %02X\n", *Data_ptr,*(Data_ptr +1),*(Data_ptr +2), *(Data_ptr +3));
	 
	 LNHexbitcode[0] = *Data_ptr & 0x00FF;
	 LNHexbitcode[1] = *(Data_ptr + 1) & 0x00FF;
	 LNHexbitcode[2] = *(Data_ptr + 2) & 0x00FF;
	 LNHexbitcode[3] = *(Data_ptr + 3) & 0x00FF;

	 	
	 	/* search for section hash code match  */
	 	
	 	for (sci = 1;sci <=LIVESECTIONS;sci++)
	 	{
			if  (LNsectionEntryHexmsgcheck(LNHexbitcode,sci) > 0)                                               //  (Physicalsection[sci].Entrycodehashval == NewLNsectioncodehashval)
			{
				sectionmatch = sci;
				sectionmarchdirection = 'E';
				
				
				Physicalsection[sci].Occupied = 1;
				
				*(Psectionptr+sci) = 1;				// set LN section as occupied 
				
				*Psectionentryptr = *Psectionentryptr + 1;
				
				*(TCPsectionptr + sci) = Physicalsection[sci].ETimedelay;	// set entry timer target			
				
				current_time = time(NULL);
    
				c_time_string_str = ctime(&current_time); 				
				
			//	SetTimestamp(PS_TIMESTAMP, 1, ctime(&current_time)); 
				
				Ctriggerlogicalupdate(1);  				
				break;			
			}
			
			if  (LNsectionExitHexmsgcheck(LNHexbitcode,sci) > 0)           //(Physicalsection[sci].Exitcodehashval == NewLNsectioncodehashval)
			{
				sectionmatch = sci;
				sectionmarchdirection = 'X';	
				
				Physicalsection[sci].Occupied = 0;
				*(Psectionptr+sci) = 0;			// set LN section as clear 
				
				*Psectionexitptr = *Psectionexitptr + 1;				
				
				*(TCPsectionptr + sci) = Physicalsection[sci].XTimedelay;  // set exit timer target 
				
				current_time = time(NULL);
    
				c_time_string_str = ctime(&current_time);  				
				
			//	SetTimestamp(PS_TIMESTAMP, 1, ctime(&current_time));
				 
				Ctriggerlogicalupdate(1); 
				break;			
			}			
		}
		
		if (sectionmatch == 0)
		{
			//printf("\n No section match found %x %x %x %x \n",);
			GetDatetimestamp();
			printf("\n%s No match found for Section msg %02X %02X  %02X  %02X\n",c_time_string_str, *Data_ptr,*(Data_ptr +1),*(Data_ptr +2), *(Data_ptr +3));
			
		}
		else
		{
		//	printf ("\n Matched section %i direction %c \n",sectionmatch,sectionmarchdirection);
		}	 
		
 }
 
void DisplayPSectionconfigflags()
{
	int sc;
	
	printf("\nPSections config display ");
	printf("\n========================\n ");
	for (sc = 0;sc < PSECTIONS;sc++)
	{
		printf("\nSection %02d Config flag %i ",sc,Sectionconfigflag[sc]);
	}	
	
}

int GetPSectionconfigflag(int Psection)
{
	return Sectionconfigflag[Psection];
}
 
void Psectionconfiginit()
{
	int sc;
	
	for (sc = 0;sc < PSECTIONS;sc++)
	{
		Sectionconfigflag[sc] = 0;
	}	
}

int Checkforhashvalduplicate(int newhashval,int sectionnumber)
{
	// checks to see if there is a duplicate of a hashvalue already in the table

	//printf("\n2302 Checking for table hashval %08d for Section %02d\n",newhashval,sectionnumber);

	for (int tc = 1;tc < PSECTIONS;tc++)
	{
		if (Physicalsection[tc].Entrycodehashval > 0)
		{
			if (newhashval == Physicalsection[tc].Entrycodehashval)
			{
				printf("\n2308 Section ENTRY  = Duplicate hash value found at table position %02d and section number %02d\n", tc,sectionnumber);
				return 8;
			}
		}

		if (Physicalsection[tc].Exitcodehashval > 0)
		{
			if (newhashval == Physicalsection[tc].Exitcodehashval)
			{
				printf("\n2308 Section EXIT  = Duplicate hash value found at table position %02d and section number %02d\n", tc,sectionnumber);
				return 8;
			}
		}
	}

	return 0;
}

void linebufferprocessingNG(unsigned char* lineinputbufptr)
{
	// input line format -> 095;048;E;B2 04 70 39;001;1;00;0;
	// 095 = record number
	// 048 = physical section number
	// E = entry / exit flag
	// B2 04 70 39 = loconet hex code
	// 001 = time delay before clearing
	// 1 = section data type   1 = DCC record, 2 = DC track circuit, 3 = Optical detector
	// 00 = TCD - direct input buffer
	// 0 = TCD direct input bit


	int LNsectionnumber;
	unsigned char LNdirectionflag;
	unsigned char LNHexcode[4];
	unsigned int LNHexbitcode[4];
	unsigned int LNshiftedHexbitcode[4];

	unsigned int NewLNHexcodehash = 0;

	int LNTimedelay;

	unsigned int LNSectiondatatype;
	unsigned int LNSectiondataTCDbuff;
	unsigned int LNSectiondataTCDbuffbit;

	int tdc = 0;

	int  LNHexcodehash = 0;

	 unsigned char lineinputbuf[LINEINPUTBUFSIZE ];
	 char Slineinputbuf[LINEINPUTBUFSIZE];
	 unsigned char Linesplitdata[SPLITFIELDSIZE];

	 int lsc = 0;
	 int hclsc = 0;

	 int lsm = 0;	

	 Clearlinesplitbuffer();

	 // find the delimiters to split the line

	 lsc = (str_split(lineinputbufptr,';', Linesplitdata));	

	// split the line using the information about where the delimiters are located

 	for (lsm = 0;lsm < lsc;lsm++)
	{
		 Buffercopy((lineinputbufptr+Linesplitdata[lsm]+1),&Linesplit[lsm][0], (Linesplitdata[lsm+1] - Linesplitdata[lsm] - 1));
    }

	  	LNsectionnumber = LNStringtoInteger(Linesplit[0]);	  

	 	LNdirectionflag = Linesplit[1][0];	

	 	LNTimedelay = LNStringtoInteger(Linesplit[3]);

	 	LNSectiondatatype =  LNStringtoInteger(Linesplit[4]);

	 	LNSectiondataTCDbuff =  LNStringtoInteger(Linesplit[5]);

	 	LNSectiondataTCDbuffbit =  LNStringtoInteger(Linesplit[6]);

	 	// split the hex character string using the ' ' (space) delimiter and generate the hex string hash code

	 	hclsc = (str_split(&Linesplit[2][0],' ', Linesplitdata));

	 	if (hclsc > 0)    /* format is xx xx xx  */
	 	{
	 		bzero(LNHexcode,4);

	 		HEXBuffercopy(&Linesplit[2][0],&HEXLinesplit[0][0],2);
	 		HEXBuffercopy(&Linesplit[2][3],&HEXLinesplit[1][0],2);
	 		HEXBuffercopy(&Linesplit[2][6],&HEXLinesplit[2][0],2);
	 		HEXBuffercopy(&Linesplit[2][9],&HEXLinesplit[3][0],2);

	 		LNHexcode[0] = StringtoH(&HEXLinesplit[0][0]); 	/* Converts string character char to hex byte */
	 		LNHexcode[1] = StringtoH(&HEXLinesplit[1][0]); 	/* Converts string character char to hex byte */
	 		LNHexcode[2] = StringtoH(&HEXLinesplit[2][0]); 	/* Converts string character char to hex byte */
	 		LNHexcode[3] = StringtoH(&HEXLinesplit[3][0]); 	/* Converts string character char to hex byte */

	 		LNHexbitcode[0] = LNHexcode[0] & 0x00FF;
	 		LNHexbitcode[1] = LNHexcode[1] & 0x00FF;
	 		LNHexbitcode[2] = LNHexcode[2] & 0x00FF;
	 		LNHexbitcode[3] = LNHexcode[3] & 0x00FF;

	 		LNshiftedHexbitcode[0] = LNHexbitcode[0] << 12;
	 		LNshiftedHexbitcode[1] = LNHexbitcode[1] << 8;
	 		LNshiftedHexbitcode[2] = LNHexbitcode[2] << 4;

	 		NewLNHexcodehash = LNshiftedHexbitcode[0] + LNshiftedHexbitcode[1] + LNshiftedHexbitcode[2] + LNHexbitcode[3];	 		
			
		    /****************************************************************************/
		    /*   Save the hash value of the hex look-up code in the section code table  */
		    /*   and the hex code details                                               */
		    /****************************************************************************/


	 		if ((LNdirectionflag == 'I') && ((LNSectiondatatype == 2) || (LNSectiondatatype == 3)))// || (LNSectiondatatype == 3)))  // storing Track circuit buffer data
	 		{
	 			//printf("\n2832 Storing TCD info Section %i Type %i TCDbuff %i TCD buffbit %i", LNsectionnumber,LNSectiondatatype,\
	 										LNSectiondataTCDbuff,LNSectiondataTCDbuffbit);

	 			// store the config in the table

	 			TCDdatabufferupdate(LNSectiondataTCDbuff, LNSectiondataTCDbuffbit,LNsectionnumber,LNSectiondatatype);

	 			Physicalsection[LNsectionnumber].Entrycodehashval = NewLNHexcodehash;

	 			HEXBuffercopy(&Linesplit[2][0],&Physicalsection[LNsectionnumber].Entrycode[0],11);

	 			HEXBuffercopy(&LNHexcode[0],&Physicalsection[LNsectionnumber].EntryHexcode[0],4);

	 			Physicalsection[LNsectionnumber].ETimedelay = LNTimedelay;

	 			Sectionconfigflag[LNsectionnumber] = 1;

	 		}

	 		if ((LNdirectionflag == 'E') && (LNSectiondatatype == 1))  // storing loconet codes
	 		{

	 	//	printf("\n2402 Storing E section  %i hash val %i Timer %i\n",  LNsectionnumber,LNHexcodehash,LNTimedelay);

	 	//	printf("\n2434 New Hash value ==> E section  %i hash val %i Timer %i\n",  LNsectionnumber,NewLNHexcodehash,LNTimedelay);

	 	//	Checkforhashvalduplicate(NewLNHexcodehash,LNsectionnumber);

	 		Physicalsection	[LNsectionnumber].Entrycodehashval = NewLNHexcodehash;

	 		HEXBuffercopy(&Linesplit[2][0],&Physicalsection[LNsectionnumber].Entrycode[0],11);

	 		HEXBuffercopy(&LNHexcode[0],&Physicalsection[LNsectionnumber].EntryHexcode[0],4);

	 		Physicalsection[LNsectionnumber].ETimedelay = LNTimedelay;	 		

	 		Sectionconfigflag[LNsectionnumber] = 2;

	 	//	printf("\n2673 - Storing E section  %i hash val %i Timer %i\n",  LNsectionnumber,LNHexcodehash,Physicalsection[LNsectionnumber].ETimedelay);
	 		}

	 		if ((LNdirectionflag == 'X') && (LNSectiondatatype == 1))
	 		{
	 			//Checkforhashvalduplicate(NewLNHexcodehash,LNsectionnumber);

	 			Physicalsection[LNsectionnumber].Exitcodehashval = NewLNHexcodehash;

	 			HEXBuffercopy(&Linesplit[2][0],&Physicalsection[LNsectionnumber].Exitcode[0],11);

	 			HEXBuffercopy(&LNHexcode[0],&Physicalsection[LNsectionnumber].ExitHexcode[0],4);

	 			Physicalsection[LNsectionnumber].XTimedelay = LNTimedelay;	 			

	 			Sectionconfigflag[LNsectionnumber] = 2;
	 		}
	 		
	 }
}


void InitializeslotArray()
{
	int ts = 0;
	
	for (ts = 0;ts < NUMBEROFDCCSLOTS; ts++)
	{
		SlotInformation[ts].Lococode = 0;
		SlotInformation[ts].ActSpeed = 0;
		SlotInformation[ts].Targetspeed = 0;
		SlotInformation[ts].Throttlestatus = 0;	
		
		SlotInformation[ts].Actualdirection = 0;
		SlotInformation[ts].Targetdirection = 0;
		
		sprintf(SlotInformation[ts].Throttlemsg, "     X");
		sprintf(SlotInformation[ts].Direction,"   X");
		SlotInformation[ts].CV17 = 0x00;
		SlotInformation[ts].CV18 = 0x00;
		SlotInformation[ts].Updateflag = 0;
		SlotInformation[ts].Reversepolarity = 0;		
	}	
	
//	printf("\nDCC Slot arrray cleared \n");
}

/**********************************************************************************
  * 
  *	  Slot message format 
  *     
  *   		Format E7  Count  Slot#  Stat1   Adr  SPD  DIRF  TRK  SS2  ADR2  SND  ID1  ID2 
  * 
  *         Byte 	     0      1      2      3      4    5     6    7    8     9    10   11   12
  *                E7   0E     xx     xx     xx     xx    xx   xx   xx    xx    xx   xx   xx
  * 
  * 
  * 
  **********************************************************************************/
  /*
    struct SlotArray
    {
	int 	Lococode;
	int 	Throttlestatus;
	char 	Throttlemsg[8];
	int 	ActSpeed;
	int 	Targetspeed;
	int     Actualdirection;
	int     Targetdirection;
	char 	Direction[4];
	char 	CV17;
	char 	CV18;
	char 	Updateflag;
	char 	Reversepolarity;  // 0 = No. 1 = Yes
    };

    extern struct SlotArray SlotInformation[NUMBEROFDCCSLOTS];
   */

void DCCslotmessagesprocess(char *DCCdata)
{
	char *DCCdata_PTR = DCCdata;
	
	char Slotsubscriptchar = *(DCCdata_PTR + 2);
	
	int Slotsubscript = Slotsubscriptchar;
	
	char status  = *(DCCdata_PTR + 3);
	
	char Directionbyte  = *(DCCdata_PTR + 6) & 0x20;
	
	SlotInformation[Slotsubscript].CV17  = *(DCCdata_PTR + 4);
	
	SlotInformation[Slotsubscript].CV18  = *(DCCdata_PTR + 9);
	
	SlotInformation[Slotsubscript].Lococode = DCCLococodecalc(*(DCCdata_PTR + 4),*(DCCdata_PTR + 9));
	
	SlotInformation[Slotsubscript].ActSpeed = *(DCCdata_PTR + 5);	
	
	SlotInformation[Slotsubscript].Throttlestatus = *(DCCdata_PTR + 3) & 0x10;
	
	*(Slotcodeptr + Slotsubscript) = SlotInformation[Slotsubscript].Lococode;
	
	if (status == 0x30)
	{
		sprintf(SlotInformation[Slotsubscript].Throttlemsg, "In Use");
	}
	
	if (status == 0x20)
	{
		sprintf(SlotInformation[Slotsubscript].Throttlemsg, "Idle  ");
	}
	
	if (status == 0x10)
	{
		sprintf(SlotInformation[Slotsubscript].Throttlemsg, "Common");
	}
	
	if (status == 0x00)
	{
		sprintf(SlotInformation[Slotsubscript].Throttlemsg, "Free  ");
	}
	
	if (Directionbyte == 0x00)
	{
		sprintf(SlotInformation[Slotsubscript].Direction,"Fwd ");
		*(Slotdirectionptr+ Slotsubscript) = 0;
	}
	
	if (Directionbyte != 0x00)
	{
		sprintf(SlotInformation[Slotsubscript].Direction,"Back");
		*(Slotdirectionptr+ Slotsubscript) = 1;
	}
	
	SlotInformation[Slotsubscript].Updateflag = 1;	
	
	//SetTimestamp(SLOT_TIMESTAMP, 1, ctime(&current_time));   
	
	printf("\nSlot = %i Loco %i \n",Slotsubscript,SlotInformation[Slotsubscript].Lococode);	
	
}

int DCCslotdatafetch(int DCCcode)
{
	int slotfound = 0;	

	if (Serialportstatus != 0)  // if serial port is open fetch DCC slot code , else return a dummy value
	{
		for (int slotcounter = 1; slotcounter < NUMBEROFDCCSLOTS;slotcounter++)
		{
		
		//printf("\n2563 counter %i DCC code %04d",slotcounter,SlotInformation[slotcounter].Lococode); 
			if (SlotInformation[slotcounter].Lococode == DCCcode)
			{
			slotfound = slotcounter;
			//break;
			}
		}
	}
	else
	{
		slotfound = 99;
	}

	return slotfound;
}

int DCCslotloadedstatus()
{
	return Allslotsloaded;
}

int UploadDCCslots()
{	
	 if (Serialportstatus == 0)
	 {
		 printf("\n2587 Serial port closed so no slot data available");

		 DCCslotdatatransfer(8);

		 Allslotsloaded = 1;

		 return 9;
	 }
	 	 
	 int slotvalue = 0;
	 
	 char DCCmessagearray[9];
	 
	 printf("\n ######### SLOT Polling ################# \n"); 	 
	 
	 if (SlotInformation[NUMBEROFDCCSLOTS].Updateflag == 1) 
	 {
		 printf("\nAll DCC slots loaded \n");
		 
		 Allslotsloaded = 1;		 

		 return 0;
	 }
	 
	 
	 int scchk = 1;
	 
	 slotvalue = 0;
	 
	 while (slotvalue == 0)
	 {
		 if (SlotInformation[scchk].Updateflag == 0)
		 {
			 slotvalue = scchk;
			// printf("\nSelected slot %i \n", slotvalue);
			// printf("\n\n");
		 }
		 scchk++;
	 } 	 
	 
	 if (slotvalue  == NUMBEROFDCCSLOTS)
	 {
		 Allslotsloaded = 1;
		 
		 printf("\n1358 - All DCC slots loaded \n");
		 
		 DCCslotdatatransfer(0);
		 
		 return 0;
	 }
	
	 if (SlotInformation[slotvalue].Updateflag == 0)
	 {	 		
		 sprintf(DCCmessagearray,"BB000000");		 
		 
		 sprintf(slotcode,"%02X",slotvalue);	
		 
		 DCCmessagearray[2] = slotcode[0];
		 DCCmessagearray[3] = slotcode[1];			
	     
		 AppendChecksum(DCCmessagearray,4); 
		 
		 printf("\nLoco slot  %i request poll %02X %02X %02X %02X ",slotvalue, DCCmsgwithchecksum[0],DCCmsgwithchecksum[1],DCCmsgwithchecksum[2],DCCmsgwithchecksum[3]);
		 
		 write (ACM0_fd_serialport, DCCmsgwithchecksum, 4); 
	 } 	 
	 
	 return 0;

}

int DCCLococodecalc(char Addr, char Addr2)
{
	// Routine to convert address details to a lococode
	
	int Loconumber = 0;
	
	int lowaddress = Addr;
	
	char highbyte = (Addr2 & 0x3F);

	Loconumber = highbyte;
	
	Loconumber = (Loconumber << 7);

	Loconumber = Loconumber + lowaddress;
	
	return Loconumber;
	
}

void AppendChecksum(char *Inputbuff, int length)
 {
	 /* Routine to calculate the check sum */
	 /* LNHexcode[0] = StringtoH(&HEXLinesplit[0][0]); 	 Converts string character char to hex byte */
	 
	 /* StringtoH(unsigned char *charinput)  Converts string character char to hex byte
											 e.g  "A1" to A1 as hex character
											 String will be format 4130                 */
	 
	 DCCmsglength = 0;
	 char* Input_ptr = Inputbuff;
	 unsigned char LNHexcode[4];
	 unsigned char LNtwochar[2];
	 int aci = 0;
	 char Checksum = 0x00;
	// char Onescomplement = 0;	
	
	//printf("\nAPC- Inputbuff HEX %02X %02X %02X %02X %02X %02X %02X %02X", \
	*(Inputbuff + 0), *(Inputbuff + 1),*(Inputbuff + 2), *(Inputbuff + 3),*(Inputbuff + 4), *(Inputbuff + 5),*(Inputbuff + 6), *(Inputbuff + 7));
	 
	// printbufferinhex(Inputbuff,8);
	 
	 /* Character string 1 */
	 
	 LNtwochar[0] = *Input_ptr;
	 LNtwochar[1] = *(Input_ptr + 1);	 
	 LNHexcode[0] = StringtoH(LNtwochar);
	 
	 /* Character string 2 */
	 
	 Input_ptr++;
	 Input_ptr++;
	 
	 LNtwochar[0] = *Input_ptr;
	 LNtwochar[1] = *(Input_ptr + 1);	 
	 LNHexcode[1] = StringtoH(LNtwochar);
	 
	 /* Character string 3 */
	 
	 Input_ptr++;
	 Input_ptr++;
	 
	 LNtwochar[0] = *Input_ptr;
	 LNtwochar[1] = *(Input_ptr + 1);	 
	 LNHexcode[2] = StringtoH(LNtwochar);
	 
	 /* Character string 4 */
	 
	 Input_ptr++;
	 Input_ptr++;
	 
	 LNtwochar[0] = *Input_ptr;
	 LNtwochar[1] = *(Input_ptr + 1);	 
	 LNHexcode[3] = StringtoH(LNtwochar);
	 
	// printbufferinhex(LNHexcode,4);
	 
	 Checksum = ~ (LNHexcode[0] & 0xFF);
	 
	// printf("\nResult %02X \n", Checksum);  	
	 
	 Checksum = Checksum ^ (LNHexcode[1] & 0xFF);
	 
	 Checksum = Checksum ^ (LNHexcode[2] & 0xFF);
	 
	 LNHexcode[3] = Checksum;
	 
	 //printf("\nChecksum Buffer Result %02X \n", Checksum);  
	 
	 for(aci = 0;aci < DCCMSGOUTPUT;aci++)
	 {
		DCCmsgwithchecksum[aci] = LNHexcode[aci]; 
	 } 	 
}

void Allslotsstop()
 {
	 int slotcnt = 1;
	 
	 for (slotcnt = 1;slotcnt < NUMBEROFDCCSLOTS;slotcnt++)
	 {
		 Speedcmdprocess(slotcnt, 0, 0);
		 sleep(1.0);
	 }	 
	 
	 printf("\nAll slots stopped - speed set to zero.... ");
 }

void Speedcmdprocess(int slotdigit, int speedval, int direction)
 {	 

		if (Serialportstatus == 0)
		{
			printf("\nSerial port closed  for speed %i and direction %i message on slot %i\n",speedval,direction,slotdigit);
			return;
		}

		bzero(Messageprocess,20);
	   
	   char DCCmessagearray[9];
	   
	  // printf("\nLNserial 2731 - Speedmsg In slot %i speed %i direction %i ",slotdigit,speedval,direction);
	  
	  sprintf(DCCmessagearray,"A1000000");		
		 
		 bzero(slotcode,2);		
	     
	     /*********************************/
	     sprintf(DCCmessagearray,"A1000000");		 
		 
		 sprintf(slotcode,"%02X",slotdigit);	
		 
		 DCCmessagearray[2] = slotcode[0];
		 DCCmessagearray[3] = slotcode[1];			 
		 
	     if (direction == 0)
	     {
			 sprintf(slotcode,"%02X",0x10);
		 }
		 else
		 {
			 sprintf(slotcode,"%02X",0x30);
		 }
		 
		 DCCmessagearray[4] = slotcode[0];
	     DCCmessagearray[5] = slotcode[1];	 		
	     
		 AppendChecksum(DCCmessagearray,4);      
	     
	     
	     /********************************/
	     		 
		 
		 //AppendChecksum(DCCmsgtext, 4);             
		 
		// printf("\nLNserial 2767 Calculated direction slot message");
		 
		// printbufferinhex(DCCmsgwithchecksum,4);
		 
		// write (ACM0_fd_serialport, DCCmsgwithchecksum, 4);
		 
		// printf("\n\n");	 		
		
		if (Serialportstatus > 0)
		{
		 write (ACM0_fd_serialport, DCCmsgwithchecksum, 4); 
		}
		else
		{
			printf("\nSerial port closed  for direction message \n");
		}
		
		/* Speed processing */
		
		 bzero(Messageprocess,20);
	  
	     sprintf(DCCmessagearray,"A0000000");
		 
		// printbufferinhex(DCCmsgtext,8);
		 
		 bzero(slotcode,2);
		 
		// printf("\nPolling Slot %i",slotvalue);
		 
		 sprintf(slotcode,"%02X",slotdigit);	
		 
		// printbufferinhex(slotcode,2);	
	
	     DCCmessagearray[2] = slotcode[0];
	     DCCmessagearray[3] = slotcode[1];	
	     
	     sprintf(slotcode,"%02X",speedval);
	     
	     DCCmessagearray[4] = slotcode[0];
	     DCCmessagearray[5] = slotcode[1];	
	     
	     AppendChecksum(DCCmessagearray, 4);             
		 
		 //printf("\nCalculated speed slot message");
		 
		 //printbufferinhex(DCCmsgwithchecksum,4);
		 
		 if (Serialportstatus > 0)
		{
		 write (ACM0_fd_serialport, DCCmsgwithchecksum, 4); 
		}
		else
		{
			printf("\nSerial port closed for speed message\n");
		}		 
		// printf("\n \n");			 
 }
 
 void SetTimestamp(int offset, int sa, char* c_time_string)
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
	
	* */	
	
	int h1,h2,m1,m2,s1,s2,ht,mt,st,tt;	
	
	 
		(MSGptr + offset)->HH[0] = *(c_time_string + 11);
		(MSGptr + offset)->HH[1] = *(c_time_string + 12);
   
		(MSGptr + offset)->MN[0] = *(c_time_string + 14);
		(MSGptr + offset)->MN[1] = *(c_time_string + 15);
   
		(MSGptr + offset)->SS[0] = *(c_time_string + 17);
		(MSGptr + offset)->SS[1] = *(c_time_string + 18);	
		
				
		if (sa == 0)
		{
			(MSGptr + offset)->Seqnumber = 0;   // reset sequence number
		} 
		
		if (sa == 0)
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
		
		//printf("\nVals tt %i ht %02d mt %02d st %02d", tt,ht,mt,st);		
 }
 
 void GetDatetimestamp()
 {
	current_time = time(NULL);
    
    c_time_string_str = ctime(&current_time);  
 }
 
 void RFidpinconfig()
 {
	Int1counter = 0;
	Int2counter = 0;
	Int3counter = 0;
	Int4counter = 0;
	Int5counter = 0;
	Int6counter = 0;
	Int7counter = 0;
	Int8counter = 0;
	Int9counter = 0;
	Int10counter = 0;
	Int11counter = 0;
	Int12counter = 0;
	
	Ctrigger    = 0;
	
	#ifdef PIRun
	/* Interrupt initialize  */
	
	// Set pin to output in case it's not
	pinMode(PIN1, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN2, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN3, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN4, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN5, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN6, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN7, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN8, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN9, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN10, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN11, OUTPUT);
	
	// Set pin to output in case it's not
	pinMode(PIN12, OUTPUT);
	
	
	pullUpDnControl (PIN1, PUD_UP) ;	
	pullUpDnControl (PIN2, PUD_UP) ;	
	pullUpDnControl (PIN3, PUD_UP) ;
	pullUpDnControl (PIN4, PUD_UP) ;
	
	pullUpDnControl (PIN5, PUD_UP) ;
	pullUpDnControl (PIN6, PUD_UP) ;
	
	pullUpDnControl (PIN7, PUD_UP) ;
	pullUpDnControl (PIN8, PUD_UP) ;	
	pullUpDnControl (PIN9, PUD_UP) ;
	
	pullUpDnControl (PIN10, PUD_UP) ;
	pullUpDnControl (PIN11, PUD_UP) ;
	pullUpDnControl (PIN12, PUD_UP) ;
	
	
	// Time now
	
	//gettimeofday(&last_change, NULL);
	
	gettimeofday(&Int1_last_change, NULL);
	gettimeofday(&Int2_last_change, NULL);
	gettimeofday(&Int3_last_change, NULL);
	gettimeofday(&Int4_last_change, NULL);
	
	gettimeofday(&Int5_last_change, NULL);
	gettimeofday(&Int6_last_change, NULL);
	gettimeofday(&Int7_last_change, NULL);
	gettimeofday(&Int8_last_change, NULL);
	
	gettimeofday(&Int9_last_change, NULL);
	gettimeofday(&Int10_last_change, NULL);
	gettimeofday(&Int11_last_change, NULL);
	gettimeofday(&Int12_last_change, NULL);
	
	// Bind to interrupt
	wiringPiISR(PIN1, INT_EDGE_BOTH, &handle1);
	// Bind to interrupt	
	wiringPiISR(PIN2, INT_EDGE_BOTH, &handle2);
	
	// Bind to interrupt
	wiringPiISR(PIN3, INT_EDGE_BOTH, &handle3);
	// Bind to interrupt
	wiringPiISR(PIN4, INT_EDGE_BOTH, &handle4);
	
	// Bind to interrupt
	wiringPiISR(PIN5, INT_EDGE_BOTH, &handle5);
	// Bind to interrupt
	wiringPiISR(PIN6, INT_EDGE_BOTH, &handle6);
	// Bind to interrupt	
	wiringPiISR(PIN7, INT_EDGE_BOTH, &handle7);
	// Bind to interrupt
	wiringPiISR(PIN8, INT_EDGE_BOTH, &handle8);
	
	// Bind to interrupt
	wiringPiISR(PIN9, INT_EDGE_BOTH, &handle9);
	// Bind to interrupt	
	wiringPiISR(PIN10, INT_EDGE_BOTH, &handle10);
	// Bind to interrupt
	wiringPiISR(PIN11, INT_EDGE_BOTH, &handle11);
	// Bind to interrupt
	wiringPiISR(PIN12, INT_EDGE_BOTH, &handle12);
	
	// Get initial state of pin 1
	P1state = digitalRead(PIN1);

	if (P1state) {
		printf("Started! Initial state of Pin 1 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 1 is off\n");
	}
	
	// Get initial state of pin 2
	P2state = digitalRead(PIN2);

	if (P2state) {
		printf("Started! Initial state of Pin 2 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 2 is off\n");
	}
	
	// Get initial state of pin 3
	P3state = digitalRead(PIN3);

	if (P3state) {
		printf("Started! Initial state of Pin 3 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 3 is off\n");
	}
	
	// Get initial state of pin 4
	P4state = digitalRead(PIN4);

	if (P4state) {
		printf("Started! Initial state of Pin 4 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 4 is off\n");
	}
	
	// Get initial state of pin 5
	P5state = digitalRead(PIN5);

	if (P5state) {
		printf("Started! Initial state of Pin 5 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 5 is off\n");
	}
	
	// Get initial state of pin 6
	P6state = digitalRead(PIN6);

	if (P6state) {
		printf("Started! Initial state of Pin 6 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 6 is off\n");
	}
	
	// Get initial state of pin 7
	P7state = digitalRead(PIN7);

	if (P7state) {
		printf("Started! Initial state of Pin 7 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 7 is off\n");
	}
	
	// Get initial state of pin 8
	P8state = digitalRead(PIN8);

	if (P8state) {
		printf("Started! Initial state of Pin 8 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 8 is off\n");
	}
	
	// Get initial state of pin 9
	P9state = digitalRead(PIN9);

	if (P9state) {
		printf("Started! Initial state of Pin 9 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 9 is off\n");
	}
	
	// Get initial state of pin 10
	P10state = digitalRead(PIN10);

	if (P10state) {
		printf("Started! Initial state of Pin 10 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 10 is off\n");
	}
	
	// Get initial state of pin 11
	P11state = digitalRead(PIN11);

	if (P11state) {
		printf("Started! Initial state of Pin 11 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 11 is off\n");
	}
	
	// Get initial state of pin 12
	P12state = digitalRead(PIN12);

	if (P12state) {
		printf("Started! Initial state of Pin 12 is on\n");
	}
	else {
		printf("Started! Initial state of Pin 12 is off\n");
	}
	#endif
	 
	 messageflag = 0;
 }
 
 // Handlers for interrupt processing on input pins

void handle1(void) {
	struct timeval now;
	unsigned long diff1;
	
	//printf("\nInt 1 on pin 1 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff1 = (now.tv_sec * 1000000 + now.tv_usec) - (Int1_last_change.tv_sec * 1000000 + Int1_last_change.tv_usec);

	// Filter jitter
	if (diff1 > IGNORE_CHANGE_BELOW_USEC1) {
		if (P1state) {
			
			Int1counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int1 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int1 Rising\n");
			}		
		}

		P1state = !P1state;
	}

	Int1_last_change = now;
}


void handle2(void) {
	struct timeval now;
	unsigned long diff2;
	
	//printf("\nInt 2 on pin 3 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff2 = (now.tv_sec * 1000000 + now.tv_usec) - (Int2_last_change.tv_sec * 1000000 + Int2_last_change.tv_usec);

	// Filter jitter
	if (diff2 > IGNORE_CHANGE_BELOW_USEC2) {
		if (P2state) 
		{
			
			Int2counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int2 Falling\n");
			}
		}
		else 
		{			
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int2 Rising\n");
			}
		}

		P2state = !P2state;
	}

	Int2_last_change = now;
}

void handle3(void) {
	struct timeval now;
	unsigned long diff3;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff3 = (now.tv_sec * 1000000 + now.tv_usec) - (Int3_last_change.tv_sec * 1000000 + Int3_last_change.tv_usec);

	// Filter jitter
	if (diff3 > IGNORE_CHANGE_BELOW_USEC3) {
		if (P3state) 
		{
			
			Int3counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int3 Falling\n");
			}
		}
		else
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int3 Rising\n");
			}
		}

		P3state = !P3state;
	}

	Int3_last_change = now;
}

void handle4(void) {
	struct timeval now;
	unsigned long diff4;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff4 = (now.tv_sec * 1000000 + now.tv_usec) - (Int4_last_change.tv_sec * 1000000 + Int4_last_change.tv_usec);

	// Filter jitter
	if (diff4 > IGNORE_CHANGE_BELOW_USEC4) {
		if (P4state) 
		{
			
			Int4counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int4 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int4 Rising\n");
			}
		}

		P4state = !P4state;
	}

	Int4_last_change = now;
}

void handle5(void) {
	struct timeval now;
	unsigned long diff5;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff5 = (now.tv_sec * 1000000 + now.tv_usec) - (Int5_last_change.tv_sec * 1000000 + Int5_last_change.tv_usec);

	// Filter jitter
	if (diff5 > IGNORE_CHANGE_BELOW_USEC5) {
		if (P5state) 
		{			
			Int5counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int5 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int5 Rising\n");
			}
		}

		P5state = !P5state;
	}

	Int5_last_change = now;
}

void handle6(void) {
	struct timeval now;
	unsigned long diff6;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff6 = (now.tv_sec * 1000000 + now.tv_usec) - (Int6_last_change.tv_sec * 1000000 + Int6_last_change.tv_usec);

	// Filter jitter
	if (diff6 > IGNORE_CHANGE_BELOW_USEC6) {
		if (P6state) 
		{			
			Int6counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int6 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int6 Rising\n");
			}
		}

		P6state = !P6state;
	}

	Int6_last_change = now;
}

void handle7(void) {
	struct timeval now;
	unsigned long diff7;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff7 = (now.tv_sec * 1000000 + now.tv_usec) - (Int7_last_change.tv_sec * 1000000 + Int7_last_change.tv_usec);

	// Filter jitter
	if (diff7 > IGNORE_CHANGE_BELOW_USEC7) {
		if (P7state) 
		{			
			Int7counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int7 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int7 Rising\n");
			}
		}

		P7state = !P7state;
	}

	Int7_last_change = now;
}

void handle8(void) {
	struct timeval now;
	unsigned long diff8;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff8 = (now.tv_sec * 1000000 + now.tv_usec) - (Int8_last_change.tv_sec * 1000000 + Int8_last_change.tv_usec);

	// Filter jitter
	if (diff8 > IGNORE_CHANGE_BELOW_USEC8) {
		if (P8state) 
		{			
			Int8counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int8 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int8 Rising\n");
			}
		}

		P8state = !P8state;
	}

	Int8_last_change = now;
}


void handle9(void) {
	struct timeval now;
	unsigned long diff9;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff9 = (now.tv_sec * 1000000 + now.tv_usec) - (Int9_last_change.tv_sec * 1000000 + Int9_last_change.tv_usec);

	// Filter jitter
	if (diff9 > IGNORE_CHANGE_BELOW_USEC9) {
		if (P9state) 
		{			
			Int9counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int9 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int9 Rising\n");
			}
		}

		P9state = !P9state;
	}

	Int9_last_change = now;
}


void handle10(void) {
	struct timeval now;
	unsigned long diff10;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff10 = (now.tv_sec * 1000000 + now.tv_usec) - (Int10_last_change.tv_sec * 1000000 + Int10_last_change.tv_usec);

	// Filter jitter
	if (diff10 > IGNORE_CHANGE_BELOW_USEC10) {
		if (P10state) 
		{			
			Int10counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int10 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int10 Rising\n");
			}
		}

		P10state = !P10state;
	}

	Int10_last_change = now;
}


void handle11(void) {
	struct timeval now;
	unsigned long diff11;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff11 = (now.tv_sec * 1000000 + now.tv_usec) - (Int11_last_change.tv_sec * 1000000 + Int11_last_change.tv_usec);

	// Filter jitter
	if (diff11 > IGNORE_CHANGE_BELOW_USEC11) {
		if (P11state) 
		{			
			Int11counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int11 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int11 Rising\n");
			}
		}

		P11state = !P11state;
	}

	Int11_last_change = now;
}


void handle12(void) {
	struct timeval now;
	unsigned long diff12;
	
	//printf("\nInt 3 on pin 4 ");

	gettimeofday(&now, NULL);

	// Time difference in usec
	diff12 = (now.tv_sec * 1000000 + now.tv_usec) - (Int12_last_change.tv_sec * 1000000 + Int12_last_change.tv_usec);

	// Filter jitter
	if (diff12 > IGNORE_CHANGE_BELOW_USEC12) {
		if (P12state) 
		{			
			Int12counter++;
			Ctrigger++;
			if(messageflag == 1)
			{
				printf("Int12 Falling\n");
			}
		}
		else 
		{
			Ctrigger++;
			
			if(messageflag == 1)
			{
				printf("Int12 Rising\n");
			}
		}

		P12state = !P12state;
	}

	Int12_last_change = now;
	
}


void Initsensorstate()
{
  int cc;
  
  // clear down current sensor state 
  
  for (cc = 0; cc < NUMBREOFIFRSENSORS;cc++)
  {
    sensorstate[cc] = 0;    
  }
  
}

void Getsensorstate()
{
#ifdef PIRun
  sensorstate[0] = digitalRead(PIN1);  
  sensorstate[1] = digitalRead(PIN2);  
  sensorstate[2] = digitalRead(PIN3);
  sensorstate[3] = digitalRead(PIN4);  
  
  sensorstate[4] = digitalRead(PIN5);
  sensorstate[5] = digitalRead(PIN6);
  
  sensorstate[6] = digitalRead(PIN7);
  sensorstate[7] = digitalRead(PIN8);   
  sensorstate[8] = digitalRead(PIN9);
  
  sensorstate[9] = digitalRead(PIN10);
  sensorstate[10] = digitalRead(PIN11);
  sensorstate[11] = digitalRead(PIN12);  
#endif
  
}

void CheckforRFidUSBports()
{
	bzero(RFiddevicename,13);
    
	sprintf(RFiddevicename,"/dev/ttyUSB*");   
	
	Numberofportsfound = 0;
    
	for (int pc = 0;pc < USBPORTCOUNT;pc++)
	{
		RFiddevicename[11] =  pc + '0';
		//printf("\n0.Checking RFid name->%s\n",RFiddevicename);
      
		fd_serialport1 = open(RFiddevicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
      
		if (fd_serialport1 < 0)
		{
			printf("\n0 Serial port %s not found  %i\n",RFiddevicename,fd_serialport1);
			//  perror(devicename);
			fd_serialport_ID[pc] = fd_serialport1;
        
			Port_bytecount[pc]   = 0;
        
			fd_serialport1_found = 0;        
		}
		else
		{
			printf("\n0 Serial port %s found OK %i \n", RFiddevicename,fd_serialport1);       
			fd_serialport_ID[pc] = fd_serialport1;
			Numberofportsfound++;
			fd_serialport1_found = 1;        
		}      
	}
    
    
	printf("\nNumber of USB ports found %i --> Port0: %i Port1: %i Port2:%i  Port3:%i\n",Numberofportsfound,fd_serialport_ID[0],fd_serialport_ID[1],fd_serialport_ID[2],fd_serialport_ID[3]);
 
 
	if (Numberofportsfound > 0)  
	{      
	
		struct sigaction saio;  	// structure for signal handling
		
		struct termios newtio;
	
		// Prepare each port and setup signal processing 
    
		for (Portsubscript = 0;   Portsubscript < Numberofportsfound; Portsubscript++)
		{   

		//install the serial handler before making the device asynchronous
      
			saio.sa_handler = signal_handler_IO;
			sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
			saio.sa_flags = 0;
			saio.sa_restorer = NULL;
			sigaction(SIGIO,&saio,NULL);

			// allow the process to receive SIGIO    
      
			fcntl(fd_serialport_ID[Portsubscript], F_SETOWN, getpid());     
      
			fcntl(fd_serialport_ID[Portsubscript], F_SETFL, FASYNC);      
      
			// set new port settings for canonical input processing 
			newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
			newtio.c_iflag = IGNPAR;
			newtio.c_oflag = 0;
			newtio.c_lflag = 0;       //ICANON;
			newtio.c_cc[VMIN]=1;
			newtio.c_cc[VTIME]=0;   
      
			tcflush(fd_serialport_ID[Portsubscript], TCIFLUSH);
      
			tcsetattr(fd_serialport_ID[Portsubscript],TCSANOW,&newtio);
		}
        // All ports setup for processing 
	}
      
      Portsubscript = 0; 
      
      sleep(2);  
      
      // initiliaze message flags       
      
      for (int pc = 0;pc <USBPORTCOUNT;pc++)
      {
        uart_somflag[pc] = 0;
    
        uart_eomflag[pc] = 0;
    
        uart_charcount[pc] = 0;        
      } 
    	
}


























