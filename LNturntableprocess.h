/*
 * LNturntableprocess.h
 *
 *  Created on: 12 Jun 2022
 *      Author: brian
 */

#ifndef LNTURNTABLEPROCESS_H_
#define LNTURNTABLEPROCESS_H_

#define MOTORSLOW     551
#define MOTORFAST    1024
#define SPC_MAX      12
#include "LNwebconfig.h"
#include "Bufferprocessing.h"

class LNturntableprocess {
public:
	LNturntableprocess();
	virtual ~LNturntableprocess();
	
	void Movetoexitlocation(int exitnumber);	
	void TTcommandmsg(int Selectedcontroller);
	void Resetallexits();
	void GetFFstatus();
	void TTUartmsgprocess(char * msgarrayptr);
	
	void TTMotorRun(int direction,int FastSlow);
	void Initializations();
	void TTcontrolswitchprocessing(unsigned char * TCDdataptr);
	void Checkcounter();
	
private:

int Currentlocation;

int TTpLastcommandsent;

int TTpswitch1AutoMan;
int TTpswitch1AutoMan_prev;

int TTpswitch2ForwardBack;
int TTpswitch2ForwardBack_prev;

int TTpswitch3RunStop;
int TTpswitch3RunStop_prev;

int TTpswitch4FastSlow;
int TTpswitch4FastSlow_prev;

int TTpIRff1;
int TTpIRff1_prev;

int TTpIRff2;
int TTpIRff2_prev;

int TTpReedEndstop;
int TTpReedEndstop_prev;

int TTpmovecounter; 
int TTpmovecountertarget;

int TTpmotorrun;
int TTpMovedirection;

int TTpcommandinprogress;
	
int TTpselectedexit;

int NextFFtotrigger;

int Motorslowspeed;
int Motorfastspeed;

int CounterSTOPposition[12];

};

#endif /* LNTURNTABLEPROCESS_H_ */
