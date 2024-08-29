/*
 * LNwebmonCommands.h
 *
 *  Created on: Jun 16, 2019
 *      Author: BrianRoot
 */

#ifndef LNWEBMONCOMMANDS_H_
#define LNWEBMONCOMMANDS_H_

#include "LNwebconfig.h"


#define CMDPARSELINES 8
#define CMDBUFFLEN    32
#define CMDPARSECHARS 8

#include <vector>

class LNwebmonCommands {

	std::vector<void(*)()> Commandlistvector;

public:
	LNwebmonCommands();

	virtual ~LNwebmonCommands();

	void Cmdtablebuild(char *src, int cc);

	void Exitcommand();

	int Findcommand(char *cp);  		// Command number lookup - finds the command number text string entered

	int Commandprocess( char cmdmsg[MAXINPUTPARAMCNT]);

	int Commandparse(char* cmdmsg, char delim);

	void Storecommands(int cmdnum, char *cmdtext, char *cmddescription);

	void StorecommandsNG(int menunum, int cmdnum, char *cmdtext, char *cmddescription);

	void Storecommandcount(int count);

	void Displayallcommands(int menucode);

	void DisplayallcommandsNG(int menucode);

	bool global_runflag;

private:

	char Commandlist[MAXNUMBEROFCOMMANDS][Commandlengthdefault+1];

	int Loadedcommands;

	char Commandmsgbuff[CMDPARSELINES][CMDPARSECHARS];
	char WebOutmsg[OUTPUTMSGBUFFERSIZE];

	int line, linechar, k, rc;

	int cmdparam;

	char Commandmsg[CMDBUFFLEN];

	char Commandmsgline1[CMDPARSECHARS];
	char Commandmsgline2[CMDPARSECHARS];
	char Commandmsgline3[CMDPARSECHARS];
	char Commandmsgline4[CMDPARSECHARS];

	int  Commandfunctioncalled[MAXNUMBEROFCOMMANDS];
	char Commanddescription[MAXNUMBEROFCOMMANDS][COMMANDDESCRIPTIONLEN];
	char Commandtext [MAXNUMBEROFCOMMANDS][COMMANDTEXTLENGTH];


	int  MenuCommandmenunumber[CMENUS][MAXNUMBEROFCOMMANDS];
	int  MenuCommandfunctioncalled[CMENUS][MAXNUMBEROFCOMMANDS];
	char MenuCommanddescription[CMENUS][MAXNUMBEROFCOMMANDS][COMMANDDESCRIPTIONLEN];
	char MenuCommandtext [CMENUS][MAXNUMBEROFCOMMANDS][COMMANDTEXTLENGTH];

};

#endif /* LNWEBMONCOMMANDS_H_ */
