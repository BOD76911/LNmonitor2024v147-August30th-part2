/*
 * LNturntableprocess.cpp
 *
 *  Created on: 12 Jun 2022
 *      Author: brian
 */

#include "LNturntableprocess.h"
#include <string>
#include <string.h>
#include <exception>
#include <cstring>
#include <iostream>			// include standard library of io routines


LNturntableprocess::LNturntableprocess() {
	// TODO Auto-generated constructor stub

//	std::cout<<"\n13 - Setting up turntable processing \n";
}

LNturntableprocess::~LNturntableprocess() {
	// TODO Auto-generated destructor stub
}

void LNturntableprocess::Movetoexitlocation(int exitnumber)
{
	std::cout<<"\n28 - TT processing... move to exit:"<<exitnumber;
	
	Resetallexits();
	
	TTpLastcommandsent = 51;
	
	TTpcommandinprogress = 52;
	
	TTpselectedexit = exitnumber;	
	
}

void LNturntableprocess::Initializations()
{
	
	TTpswitch1AutoMan = 0;
	TTpswitch1AutoMan_prev = 0;

    TTpswitch2ForwardBack = 0;
	TTpswitch2ForwardBack_prev = 0;
	
	TTpswitch3RunStop = 0;
	TTpswitch3RunStop_prev = 0;
	
	TTpswitch4FastSlow = 0;
	TTpswitch4FastSlow_prev = 0;
	
	TTpIRff1 = 0;
	TTpIRff1_prev = 0;

	TTpIRff2 = 0;
	TTpIRff2_prev = 0;
	
	TTpReedEndstop = 0;
	TTpReedEndstop_prev = 0;

	TTpmovecounter = 0; 
	TTpmovecountertarget = 0;
	
	TTpmotorrun = 0;
	TTpMovedirection = 0;	
	
	TTpcommandinprogress = 0;
	
    TTpselectedexit = 0;
	
	ResetTF1Rpin();
	
	ResetTF2Rpin();
	
	ResetReedswitchpin();
	
	Motorslowspeed = MOTORSLOW;
	
	Motorfastspeed = MOTORFAST;
	
	//std::cout<<"\nMotor set speeds FAST:"<<Motorfastspeed<<" SLOW:"<<Motorslowspeed<<"\n\n";
	
	// initialize stop position counter 
	
	for (int spc = 0;spc < SPC_MAX; spc++)
	{
		CounterSTOPposition[spc] = 0;		
	}
	
	CounterSTOPposition[2] = 10;
	CounterSTOPposition[3] = 16;
	CounterSTOPposition[4] = 24;
}

void LNturntableprocess::GetFFstatus()
{
	unsigned char TTCmdtextoutput[7];

	TTCmdtextoutput[0] = 'C';
		
	TTCmdtextoutput[1] = '0';
	TTCmdtextoutput[2] = '0';
	TTCmdtextoutput[3] = '0';
	TTCmdtextoutput[4] = '0';
	TTCmdtextoutput[5] = '0';
	TTCmdtextoutput[6] = '0';

	unsigned char* TTCmdtextptr = &TTCmdtextoutput[0];
					 	 
	std::cout<<"\nController C - Get FF status message sent:"<<TTCmdtextptr;
					 	 
	TTpLastcommandsent = 53;

	TTcommandsend(7,TTCmdtextptr);	
}

void LNturntableprocess::Resetallexits()
{
	unsigned char TTCmdtextoutput[7];
	
	unsigned char *TTCmdtextptr = new unsigned char[8];
	
	*TTCmdtextptr = 'A';
	*(TTCmdtextptr + 1) = '0';
	*(TTCmdtextptr + 2) = '0';
	*(TTCmdtextptr + 3) = '0';
	*(TTCmdtextptr + 4) = '0';
	*(TTCmdtextptr + 5) = '0';
	*(TTCmdtextptr + 6) = '0';
	*(TTCmdtextptr + 7) = '\0';	
					 	 
	std::cout<<"\n119 Controller A - message sent:"<<TTCmdtextptr;
					 	 
	TTpLastcommandsent = 49;
	
	TTpReedEndstop_prev = 1;			// Enable motor run TFFR flag

	TTcommandsend(7,TTCmdtextptr);	
}

void LNturntableprocess::TTcommandmsg(int Selectedcontroller)
{
	char chardatareply[64];

	bzero(chardatareply,64);

	char* chardatareplyptr = &chardatareply[0];
	
	std::cout<<"\n40 Controller :"<<Selectedcontroller<<"\n";

	std::sprintf(chardatareply,"%d",Selectedcontroller);
	
	unsigned char TTCmdtextoutput[7];

	TTCmdtextoutput[0] = 'B';
					 					 	 
	if (Selectedcontroller > 9)
	{
		TTCmdtextoutput[1] = chardatareply[0];
		TTCmdtextoutput[2] = chardatareply[1];
	}
	else
	{
		TTCmdtextoutput[1] = '0';
		TTCmdtextoutput[2] = chardatareply[0];
	}
					 					 	 
					 				
	TTCmdtextoutput[3] = '0';
	TTCmdtextoutput[4] = '0';
	TTCmdtextoutput[5] = '0';
	TTCmdtextoutput[6] = '0';

	unsigned char* TTCmdtextptr = &TTCmdtextoutput[0];
					 				 
	std::cout<<"\nController B - message sent:"<<TTCmdtextptr;
							
	TTpLastcommandsent = 52;

	TTcommandsend(7,TTCmdtextptr);		
}

void LNturntableprocess::TTcontrolswitchprocessing(unsigned char * TCDdataptr)
{	
 
	char TTdataread = *(TCDdataptr + 2);
	
	char testval; 
	
	testval = TTdataread & 0X08;	
	
	if (testval > 0)
	{
		TTpswitch1AutoMan = 1;
	}
	else
	{
		TTpswitch1AutoMan = 0;
	}
	
	if ((TTpswitch1AutoMan != TTpswitch1AutoMan_prev) && (TTpswitch1AutoMan_prev == 0))
	{
		std::cout<<"\n\nTTswitch1AutoMan changed to AUTO";
		TTpswitch1AutoMan_prev = TTpswitch1AutoMan;
		std::cout<<"\n";
	}
	else
	{
		if ((TTpswitch1AutoMan != TTpswitch1AutoMan_prev) && (TTpswitch1AutoMan_prev == 1))
		{
			std::cout<<"\n\nTTswitch1AutoMan changed to MANUAL";
			TTpswitch1AutoMan_prev = TTpswitch1AutoMan;
			std::cout<<"\n";
		}
		else
		{
			if (TTpswitch1AutoMan_prev == 2)
			{
				std::cout<<"\n\nTTswitch1AutoMan initialized";
				TTpswitch1AutoMan_prev = TTpswitch1AutoMan;
				std::cout<<"\n";				
			}		
		}
	}
	
	testval = TTdataread & 0X10;	
	
	if (testval > 0)
	{	
		TTpswitch2ForwardBack = 1; 
	}
	else
	{
		TTpswitch2ForwardBack = 0; 
	}
	
	if ((TTpswitch2ForwardBack != TTpswitch2ForwardBack_prev) && (TTpswitch2ForwardBack_prev == 0))
	{
		std::cout<<"\n\nTTswitch2AutoMan changed to FORWARD";
		TTpswitch2ForwardBack_prev = TTpswitch2ForwardBack;
		std::cout<<"\n";
	}
	
	else
	{
		if ((TTpswitch2ForwardBack != TTpswitch2ForwardBack_prev) && (TTpswitch2ForwardBack_prev == 1))
		{
			std::cout<<"\n\nTTswitchForwardBack changed to BACKWARDS";
			TTpswitch2ForwardBack_prev = TTpswitch2ForwardBack;
			std::cout<<"\n";
		}
	
		else
		{
			if (TTpswitch2ForwardBack_prev == 2)
			{
				std::cout<<"\n\nTTswitch2ForwardBack initialized";
				TTpswitch2ForwardBack_prev = TTpswitch2ForwardBack;
				std::cout<<"\n";				
			}		
		}
	}
	
	
	testval = TTdataread & 0X20;
	
	if (testval > 0)
	{
		TTpswitch3RunStop = 1; 
	}
	else
	{
		TTpswitch3RunStop = 0; 
	}
	
	if ((TTpswitch3RunStop != TTpswitch3RunStop_prev) && (TTpswitch3RunStop_prev == 0))
	{
		std::cout<<"\n\nTTswitch3RunStop to RUN";
		TTpswitch3RunStop_prev = TTpswitch3RunStop;
		std::cout<<"\n";
	}
	else
	{
		if ((TTpswitch3RunStop != TTpswitch3RunStop_prev) && (TTpswitch3RunStop_prev == 1))
		{
			std::cout<<"\n\nTTswitch3RunStop to STOP";
			TTpswitch3RunStop_prev = TTpswitch3RunStop;
			std::cout<<"\n";
		}
		else
		{
			if (TTpswitch3RunStop_prev == 2)
			{
				std::cout<<"\n\nTTswitch3RunStop initialized";
				TTpswitch3RunStop_prev = TTpswitch3RunStop;	
				std::cout<<"\n";			
			}		
		}
	}
	
	testval = TTdataread & 0X40;
	
	if (testval > 0)
	{
		TTpswitch4FastSlow = 1; 
	}
	else
	{
		TTpswitch4FastSlow = 0; 
	}
	
	if ((TTpswitch4FastSlow != TTpswitch4FastSlow_prev) && (TTpswitch4FastSlow_prev == 0))
	{
		std::cout<<"\n\nTTswitch4FastSlow to FAST";
		TTpswitch4FastSlow_prev = TTpswitch4FastSlow;
		std::cout<<"\n";
	}
	else
	{
		if ((TTpswitch4FastSlow != TTpswitch4FastSlow_prev) && (TTpswitch4FastSlow_prev == 1))
		{
			std::cout<<"\n\nTTswitch4FastSlow to SLOW";
			TTpswitch4FastSlow_prev = TTpswitch4FastSlow;
			std::cout<<"\n";
		}
		else
		{
			if (TTpswitch4FastSlow_prev == 2)
			{
				std::cout<<"\n\nTTswitch4FastSlow initialized";
				TTpswitch4FastSlow_prev = TTpswitch4FastSlow;	
				std::cout<<"\n";			
			}		
		}
	}	
	
	// check flip flop 1 
	
	testval = TTdataread & 0X02;
	
	if (testval > 0) 
	{
		TTpIRff1 = 0; 
	}
	else
	{
		if (TTpIRff1_prev == 0)
		{
			TTpIRff1 = 1; 
		}
	}
	
	if (TTpIRff1 != TTpIRff1_prev) 
	{
	//	std::cout<<"\nIR ff1 changed to "<<TTpIRff1;
	//	std::cout<<"\n";
		TTpIRff1_prev = TTpIRff1;
	}	
	
	// check flip flop 2 		
	
	testval = TTdataread & 0X04;
	
	if (testval > 0)
	{
		TTpIRff2 = 0; 
	}
	else
	{
		if (TTpIRff2_prev == 0)
		{
			TTpIRff2 = 1;  // FF2 now triggered so reset FF1 
			ResetTF1Rpin();
			TTpIRff1_prev == 0;
			TTpIRff1 = 0; 
		}
	}
	
	if (TTpIRff2 != TTpIRff2_prev) 
	{
	//	std::cout<<"\nIR ff2 changed to "<<TTpIRff2;
	//	std::cout<<"\n";
		TTpIRff2_prev = TTpIRff2;
	}	
	
	// check reset flip flop 	

	testval = TTdataread & 0X01;
	
	Checkcounter();
	
	
	
	if (testval > 0)
	{		
		TTpReedEndstop = 0; 
	}
	else
	{
		TTpReedEndstop = 1; 
	//	std::cout<<"\n391 Reed end stop changed to "<<TTpReedEndstop;
	}
	
	if (TTpReedEndstop  != TTpReedEndstop_prev)
	{
		std::cout<<"\n396 Motor stop triggered - changed to "<<TTpReedEndstop;
		TTpReedEndstop_prev = TTpReedEndstop;
		TTMotorRun(0,0);
		
		if (TTpselectedexit == 1) 			// if arrived at exit 1, then reset the counter 
		{
			std::cout<<"\n412 Counter reset to zero \n\n";
			TTpmovecounter = 0;
		}
	}	
}

void LNturntableprocess::TTUartmsgprocess(char * msgarrayptr)
{
	
	std::cout<<"\n121 - TT controller message received "<<msgarrayptr<<"\n";
	
	Bufferprocessing* uartmsgprocessptr = new Bufferprocessing();	
	
	uartmsgprocessptr->Fileparse(msgarrayptr,';',0);	
	
	if ((std::stoi(uartmsgprocessptr->Paramdata(3)) == TTpLastcommandsent) && (TTpcommandinprogress > 0))
	{
		std::cout<<"\n416 last command matched ...........\n";
		std::cout<<"\n\n";
		TTcommandmsg(TTpselectedexit);		
		std::cout<<"\n418 command sent for exit:"<<TTpselectedexit;
		
		// start TT motor 
		
		if (TTpselectedexit == 1)  // if moving to exit 1, run motor backwards
		{
			TTMotorRun(0,1); 
			std::cout<<"\n429 Starting TT motor to run backwards \n";
		}
		else
		{
			TTMotorRun(1,1); 	// if moving to exit other than 1, run motor forwards
			std::cout<<"\n434 Starting TT motor to run forwards \n";
			TTpmovecountertarget = CounterSTOPposition[TTpselectedexit];
			std::cout<<"\n461Move counter target set to "<<TTpmovecountertarget;
		}
		
		TTpcommandinprogress = 0;		
		
		std::cout<<"\n";
	}	
	
}

void LNturntableprocess::Checkcounter()
{
	if ((TTpIRff1 ==1) && (TTpIRff2 == 1) && (TTpmotorrun > 0))
	{
		// both latches tripped so increment counter and reset both latches
		
		if (TTpMovedirection == 1)
		{
			TTpmovecounter++;
			std::cout<<"\nCounter incremented to "<<TTpmovecounter;
		//	std::cout<<"\n467 Motor stop trigger "<<TTpReedEndstop<<" prev:"<<TTpReedEndstop_prev;
		}
		
		ResetTF1Rpin();
		ResetTF2Rpin();		
		TTpIRff2_prev == 0;
		TTpIRff2 = 0; 
		
		
		
		if ((TTpmovecountertarget > 0) && (TTpmovecountertarget - TTpmovecounter < 5) && (TTpMovedirection == 1))
		{
				TTMotorRunforward(Motorslowspeed);
			
				std::cout<<"\n480 TT motor switched to slow speed value :"<<Motorslowspeed<<" \n";
		}			
	}	
}

void LNturntableprocess::TTMotorRun(int direction, int FastSlow)
{
	 if (FastSlow == 0)
		{
			TTMotorStop();
			TTpmotorrun = 0;						 
			std::cout<<"\n\n401 TT motor stopped";
			std::cout<<"\n";
			return;
		}	
			
	 if (direction == 1)
	 {
		if (FastSlow == 1)
		{
			TTMotorRunforward(Motorslowspeed);
			TTpmotorrun = 1;
			std::cout<<"\n\n412 Run TT motor forwards SLOWLY";			
			std::cout<<"\n";			
		}
		if (FastSlow == 2)
		{
			TTMotorRunforward(Motorfastspeed);
			TTpmotorrun = 1;
			std::cout<<"\n\n419 Run TT motor forwards FAST";
			std::cout<<"\n";				
		}	
		TTpMovedirection = 1;	
	 }
	 else
	 {
		//  ResetReedswitchpin();
		//  TTReedEndstop = 0;
		//  TTReedEndstop_prev = 0;
		//  TTmotorrun = 1;		 					 				 
					 
		 if (FastSlow == 1)
		{
			TTMotorRunbackward(580);
			TTpmotorrun = 1;
			std::cout<<"\n\n434 Run TT motor backwards SLOWLY";
			std::cout<<"\n";			
		}
		if (FastSlow == 2)
		{
			TTMotorRunbackward(Motorfastspeed);
			TTpmotorrun = 1;
			std::cout<<"\n\n441 Run TT motor backwards FAST";
			std::cout<<"\n";				
		}	
		TTpMovedirection = 0;	 
	 }
	 
}
