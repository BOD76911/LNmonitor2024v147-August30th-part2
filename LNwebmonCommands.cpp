/*
 * LNwebmonCommands.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: BrianRoot
 */

#include "LNwebmonCommands.h"
#include <string>
#include <string.h>
#include <cstring>
#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <stdio.h>
#include <iomanip>

LNwebmonCommands::LNwebmonCommands() {
	// TODO Auto-generated constructor stub

}

LNwebmonCommands::~LNwebmonCommands() {
	// TODO Auto-generated destructor stub
}

void LNwebmonCommands::Cmdtablebuild(char *src, int cc)
{
	int cm = 0;

		while(cm < Commandlengthdefault)
		{
			Commandlist[cc][cm] = '\0';
			cm++;
		}

		cm = 0;

		while((cm < Commandlengthdefault) && (*(src+cm) != '\0'))
		{

		Commandlist[cc][cm] = *(src+cm);

		cm++;
		}
}

void LNwebmonCommands::StorecommandsNG(int menunum, int cmdnum, char *cmdtext, char *cmddescription)
{
	/*
	int  MenuCommandfunctioncalled[CMENUS][NUMBEROFCOMMANDS];
	char MenuCommanddescription[CMENUS][NUMBEROFCOMMANDS][COMMANDDESCRIPTIONLEN];
	char MenuCommandtext [CMENUS][NUMBEROFCOMMANDS][COMMANDTEXTLENGTH];
	*/


	MenuCommandfunctioncalled[menunum][cmdnum] = cmdnum;

	MenuCommandmenunumber[menunum][cmdnum] = menunum;

	sprintf(MenuCommandtext[menunum][cmdnum],"%s",cmdtext);

	sprintf(MenuCommanddescription[menunum][cmdnum],"%s",cmddescription);

//	std::cout << "\nStored command " << Commandfunctioncalled[cmdnum] << " Text " << Commandtext[cmdnum] << "  Desc--> " << Commanddescription[cmdnum];

}



void LNwebmonCommands::Storecommands(int cmdnum, char *cmdtext, char *cmddescription)
{
	Commandfunctioncalled[cmdnum] = cmdnum;

	sprintf(Commandtext[cmdnum],"%s",cmdtext);

	sprintf(Commanddescription[cmdnum],"%s",cmddescription);

	//std::cout << "\nStored command " << Commandfunctioncalled[cmdnum] << " Text " << Commandtext[cmdnum] << "  Desc--> " << Commanddescription[cmdnum];

}

void LNwebmonCommands::Storecommandcount(int count)
{
	Loadedcommands = count;
}


int LNwebmonCommands::Commandprocess( char cmdmsg[MAXINPUTPARAMCNT])
{
	int RC = 0;

	Commandparse(cmdmsg,DELIMITER);

	/* --------------------- search for command match ----------------*/

	int cc = 0;

	//cout <<"\nMsgline1 " << Commandmsgline1;

	while (cc < MAXNUMBEROFCOMMANDS)
	{
		// std::cout << "\nline " << Commandmsgline1 << " Text-> " << Commandtext[cc];

		if (std::strcmp(Commandmsgline1,Commandtext[cc]) == 0)
		{
			RC = cc;
		//	std::cout << "\n\nCommand RC = " << cc << "\n" ;			
			return RC;
		}
	cc++;
	}

	std::cout << "\n\nCommand process line 337  - CMD:" << cmdmsg << ": not Recognized..." ;

	RC = 98;

	return RC;
}

int LNwebmonCommands::Commandparse(char* cmdmsg, char delim)
{
	int i,j,c;

	for (i = 0;i < CMDPARSELINES;i++)
	{
		for (j = 0;j < CMDPARSECHARS;j++)
		{
			Commandmsgbuff[i][j] = '\0';
		}
	}

	line 		= 0;
	linechar 	= 0;
	k 			= 0;

	while (k < CMDBUFFLEN)
	{
		if (*cmdmsg == '\0')
		{
			k = CMDBUFFLEN;

			for (c = 0;c<CMDPARSECHARS;c++)
			{
				Commandmsgline1[c] = Commandmsgbuff[0][c];
				Commandmsgline2[c] = Commandmsgbuff[1][c];
				Commandmsgline3[c] = Commandmsgbuff[2][c];
				Commandmsgline4[c] = Commandmsgbuff[3][c];
			}
			return 0;
		}

		if (*cmdmsg == delim)
		{
			line++;
			linechar = 0;

			k++;
			cmdmsg++;
		}
		else
		{
			Commandmsgbuff[line][linechar] = *cmdmsg;
			linechar++;
			cmdmsg++;
			k++;
		}
	}
	return 0;
}

void LNwebmonCommands::Displayallcommands(int menucode)
{
	std::cout << "\n\nDisplay all loaded commands";
	std::cout << "\n---------------------------\n";

	int cc;

	for (cc = 1; cc <= Loadedcommands; cc++)
	{
		std::cout << "\n" << std::setiosflags(std::ios::right) << std::setw(3)<< std::setfill('0') << Commandfunctioncalled[cc];
		std::cout << "  ";
		std::cout << std::setiosflags(std::ios::left) << std::setw(6)<< std::setfill(' ') << Commandtext[cc];
		std::cout << "  " << Commanddescription[cc];
	}
}

void LNwebmonCommands::DisplayallcommandsNG(int menucode)
{
	//int  MenuCommandfunctioncalled[CMENUS][NUMBEROFCOMMANDS];
	//char MenuCommanddescription[CMENUS][NUMBEROFCOMMANDS][COMMANDDESCRIPTIONLEN];
	//char MenuCommandtext [CMENUS][NUMBEROFCOMMANDS][COMMANDTEXTLENGTH];

	std::cout << "\n\nDisplay commands for Sub-menu "<<menucode;
	std::cout << "\n-------------------------------\n";

	int cc;

	int mn;

	for (cc = 1; cc <= MAXNUMBEROFCOMMANDS; cc++)
	//for (cc = 1; cc <= Loadedcommands; cc++)
	{
		if ((MenuCommandmenunumber[menucode][cc] == menucode) && (MenuCommandfunctioncalled[menucode][cc] != 0))
		{
			std::cout << "\n" << std::setiosflags(std::ios::right) << std::setw(3)<< std::setfill('0') << MenuCommandfunctioncalled[menucode][cc];
			std::cout << "  ";
			std::cout << std::setiosflags(std::ios::left) << std::setw(6)<< std::setfill(' ') << MenuCommandtext[menucode][cc];
			std::cout << "  " << MenuCommanddescription[menucode][cc];
		}
		else
		{
			std::cout <<"";

		}

	}
}









