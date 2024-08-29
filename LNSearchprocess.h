/*
 * LNSearchprocess.h
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#ifndef LNSEARCHPROCESS_H_
#define LNSEARCHPROCESS_H_

#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <vector>

#include <list>
#include <ctime>
#include <time.h>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <strings.h>
#include <iostream>			// include standard library of io routines
#include <cstdio>
#include <cstring>

#include "LNwebLsectionsLdevices.h"
#include "LNRteDevicereferencedata.h"
#include "LNRteLogicalsectionData.h"
#include "LNRteLogicalsectionGroup.h"
#include "LNSearchresult.h"
#include "LNRouteprocess.h"


#define MAXBRANCHSEARCH 10

class LNSearchprocess
{
public:
	// Class for objects created for a specific search

	LNSearchprocess();

	virtual ~LNSearchprocess();

	std::vector<int> Runsearch(LNRteLogicalsectionGroup* LNRteLSG, int SrchRte, int RunStartLS,int RunMidLS, int RunEndLS, int RunMaxsteps, int diagnosticsflag);

	int SearchRoutestepsvector(int srchLS, std::vector<int> &storedstepsvect);

	std::vector<int> Vectcopy(std::vector<int> &srcevect, std::vector<int> &destvect);

	void Displayvector(std::vector<int> &srcevect);

	void InitializeRteSearchTable(int tableinstance);

	void DisplayRteSearchTable(int tableinstance);

	void RteSearchTablebuild(LNRteLogicalsectionGroup* LNRteLSG, int StartLS,int MidLS, int EndLS, int Maxsteps, int diagnosticsflag);

	int SingleTablebuild(LNRteLogicalsectionGroup* LNRteLSG, int Tablenumber, int StartLS,int MidLS, int EndLS, int Maxsteps, int diagnosticsflag);

	int CheckforENDls(int EndLS, int Maxsteps);

	int CheckforMIDandENDls(int EndLS,int MidLS, int Maxsteps);

	void DisplayRtesearchtableline(int tableinstance, int row, int rowelement, std::string rowname);

	void ClearRtesearchtableindices();

private:

	#define MAXROUTESEARCHTABLECOLUMNS	64
	#define MAXROUTESEARCHTABLEROWS     12
	#define MAXROUTESEARCTABLEINSTANCES 256

	#define MAXNUMBEROFNS				12

	int RTI; // = MAXROUTESEARCTABLEINSTANCES; 	// Number of Route search tables that can be built for search process (Table zero is not used)

	int RTC; // = MAXROUTESEARCHTABLECOLUMNS;	// Number of columns to store search step logical sections. Column 0 is the Serach start LS number
											// Number of search steps must be less than this number

	int RTR; // = MAXROUTESEARCHTABLEROWS;		// Number of branch options for any given section.
											// This is the same as maximum number of Next Section branches in the logical section configuration

	int RTD; // = MAXROUTESEARCHTABLEITEMS;		// Elements for each search section: 0 = LS number,
													// 1 = Type of section (0 -> straight through, if greater than zero, number = number of branches for the section
	int Rsrchtablesbuilt;

	int Branchtablesbuilt;

	int BR1LS;
	int BR2LS;
	int BR3LS;
	int BR4LS;

	int Tableinprocess;

	int RSTB_table_columnindex[MAXROUTESEARCTABLEINSTANCES + 1];

	int RSTB_table_rowindex[MAXROUTESEARCTABLEINSTANCES + 1];

	int RSTB_Branchcounter[MAXROUTESEARCTABLEINSTANCES + 1];

	int RSTB_Branchtablesrequired[MAXROUTESEARCTABLEINSTANCES + 1];

	int RSTB_LStype[MAXROUTESEARCTABLEINSTANCES + 1];

	int RSTB_SectionInprocess[MAXROUTESEARCTABLEINSTANCES + 1];

	int Branchcounter = 0;

	int Branchtablesrequired = 1;

	int EndLStable;

	int MidEndLStable;

	int EndLStablecolumn;

	int MidEndLStablecolumn;

	int RteSearchTableStepsstored[MAXROUTESEARCTABLEINSTANCES + 1];

	struct RsLSdata
	{
		int LSnumber;
		int LStype;
		int Branchtablenumber;
		int Branchtablestatus;
	};

	struct RsLSdata RteSearchTable[MAXROUTESEARCTABLEINSTANCES + 1][MAXROUTESEARCHTABLECOLUMNS + 1][MAXROUTESEARCHTABLEROWS + 1];

	int Branchprocessedflag[MAXBRANCHSEARCH];

	int BranchprocessedLSvalue[MAXBRANCHSEARCH];

	unsigned int FirstStartLS;

	unsigned int EndLSfound;
	unsigned int SrchRoute;
	unsigned int StartlocLS;
	unsigned int EndlocLS;
	unsigned int MaxLSearchsteps;
	unsigned int Searchstepcount;
	unsigned int Searchstepsexecuted;
	unsigned int Branchesfound;
	unsigned int Brancheschecked;
	unsigned int Startbranchstep;
	unsigned int StartbranchLS;

	std::vector<int> Ssteps;

	std::vector<std::vector<int> > Routesteps;

	std::vector<LNSearchresult> BranchRoutesteps;

	std::vector<int> Srchresults;

	std::vector<int> NSlist;

	std::vector<int> NS;

	std::vector<int> Branches;

	std::vector<int> Branchestart;

	std::vector<int> Rtesrchsteps;


};

#endif /* LNSEARCHPROCESS_H_ */
