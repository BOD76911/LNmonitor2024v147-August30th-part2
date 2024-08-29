/*
 * LNsignalprocessing.h
 *
 *  Created on: 31 Oct 2022
 *      Author: brian
 */

#ifndef LNSIGNALPROCESSING_H_
#define LNSIGNALPROCESSING_H_

#include "LNwebconfig.h"
#include "Bufferprocessing.h"
#include "LNwebmonNGfileload.h"

class LNsignalprocessing {
public:
	LNsignalprocessing();
	virtual ~LNsignalprocessing();

void Manualprocess(int Signalnumber,char Signalcodechar);

int Loadsignalconfigdata();

void Initializesignaldata();

void SendhardwareUpdate(int Signalnumber,char Signalcodechar);

int Displaysignalconfig(int Dsection, char* replymessage);

int Getsignalcurrentvalue(int DisplaysectionNumber);

int Getsigcurrentvalue(int Signalnumber);

int GetsignalnumberforGUI(int GUIobjectnumber);

int GetPsection(int DisplaysectionNumber);

int Numberofaspects(int DisplaysectionNumber);

int SignalChangedCheck();

void Storesignalconfigdata(char* lineinputbuffer, int displayflag);

void Refreshsignalvalues(int Trainsectioncodes[], int SectionTcodes[], int Pointstatus[], int displayflag);

void UpdateTrainsectionsconfig(int Trainsection, int PS1, int PS2);

private:

int Signalcolour = 1;

char Signalnumberchar[2];

int Numberofsignalrecordsstored = 0;

struct signaldata
	{
		int Signalnumber;
		int Signaltype;
		int Signalphysicalsection[MAXNUMBEROFSIGNALPSECTIONS];
		int Signalphysicalsectioncurroccupancy[MAXNUMBEROFSIGNALPSECTIONS];
		int GUIobjectreference;
		int Trainsection;
		int Trainsectionoccupancy;
		int SignalLS[MAXNUMBEROFSIGNALLSSECTIONS];
		int AmberlookaheadSignal;
		int BlockingPointnumber;
		int Hardwarereferencenumber;
		int Numberofcolours;
		int Currentvalue;
		int Previousvalue; 
		int Displayconfigsectionreference;	
		int Linkedpoints[MAXNUMBEROFSIGNALTOPOINTLINKS];
		int Linkedpointsflag[MAXNUMBEROFSIGNALTOPOINTLINKS];
	};
	
	struct signaldata  Signalprocessdata[MAXNUMBEROFSIGNALS];

};

#endif /* LNSIGNALPROCESSING_H_ */
