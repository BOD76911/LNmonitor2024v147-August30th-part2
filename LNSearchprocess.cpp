/*
 * LNSearchprocess.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#include "LNSearchprocess.h"

LNSearchprocess::LNSearchprocess()
{

	//std::cout<<"\nCreating the search process object";

}

LNSearchprocess::~LNSearchprocess()
{



}

void LNSearchprocess::Displayvector(std::vector<int> &srcevect)
{
		// prints values in integer vector

		//printf("\n\n Values stored in vector..");
		for (std::vector<int>::iterator RBit = srcevect.begin(); RBit != srcevect.end(); ++RBit)
		std::cout << ' ' << *RBit;
		std::cout << '\n';
}


int LNSearchprocess::SearchRoutestepsvector(int srchLS, std::vector<int> &storedstepsvect)
{
	// search existing route steps vector to see if logical section has already been stored

	// returns -1 if sectin not found, else returns the step in the loop - NOTE: Starts from zero

	int Stepcounter = 0;

	for (std::vector<int>::iterator SrchRBit = storedstepsvect.begin(); SrchRBit != storedstepsvect.end(); ++SrchRBit)
	{
		//std::cout << ' ' << *SrchRBit;
		//std::cout << '\n';

		if (srchLS == *SrchRBit)
		{
			//std::cout<<"\nDuplicate LS "<< srchLS <<" found ";
			return Stepcounter;		// return the step number
			break;
		}

		Stepcounter++;
	}

	return -1;
}

void LNSearchprocess::DisplayRteSearchTable(int tableinstance)
{
	int row = 1;
	int Col = 0;

		printf("\n\nDisplaying Search Table %02d with %02d Steps stored\n=================================================",tableinstance,RSTB_table_columnindex[tableinstance]);
		printf("\n\n         001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 021 022 023 024 025 026 027 028 029 030 031 032");
		printf("\n         --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---");

		if (RteSearchTable[tableinstance][1][1].LSnumber == 0)
		{
			printf("\n\nNo data to display as table is not initialized");
		}
		else
		{
			DisplayRtesearchtableline(tableinstance,row,2,"LS Type:");
			DisplayRtesearchtableline(tableinstance,row,1,"LS data:");
			DisplayRtesearchtableline(tableinstance,row + 1,1,"LS Br01:");
			DisplayRtesearchtableline(tableinstance,row + 2,1,"LS Br02:");
			DisplayRtesearchtableline(tableinstance,row + 3,1,"LS Br03:");
			DisplayRtesearchtableline(tableinstance,row + 4,1,"LS Br04:");
			DisplayRtesearchtableline(tableinstance,row + 5,1,"LS Br05:");
			DisplayRtesearchtableline(tableinstance,row + 6,1,"LS Br06:");
			DisplayRtesearchtableline(tableinstance,row + 7,1,"LS Br07:");
			DisplayRtesearchtableline(tableinstance,row + 8,1,"LS Br08:");
			DisplayRtesearchtableline(tableinstance,row + 9,1,"LS Br09:");
			DisplayRtesearchtableline(tableinstance,row + 10,1,"LS Br10:");
			DisplayRtesearchtableline(tableinstance,row + 11,1,"LS Br11:");
		}
}

void LNSearchprocess::DisplayRtesearchtableline(int tableinstance, int row, int rowelement, std::string rowname)
{
	char * cstr = new char [rowname.length()+1];

	std::strcpy (cstr, rowname.c_str());

	switch(rowelement)
	{
	case 1:
	{

	printf("\n%s %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d",cstr, \
					RteSearchTable[tableinstance][1][row].LSnumber,RteSearchTable[tableinstance][2][row].LSnumber,RteSearchTable[tableinstance][3][row].LSnumber,\
					RteSearchTable[tableinstance][4][row].LSnumber,RteSearchTable[tableinstance][5][row].LSnumber,RteSearchTable[tableinstance][6][row].LSnumber,\
					RteSearchTable[tableinstance][7][row].LSnumber,RteSearchTable[tableinstance][8][row].LSnumber,RteSearchTable[tableinstance][9][row].LSnumber,\
					RteSearchTable[tableinstance][10][row].LSnumber,RteSearchTable[tableinstance][11][row].LSnumber,RteSearchTable[tableinstance][12][row].LSnumber,\
					RteSearchTable[tableinstance][13][row].LSnumber,RteSearchTable[tableinstance][14][row].LSnumber,RteSearchTable[tableinstance][15][row].LSnumber,\
					RteSearchTable[tableinstance][16][row].LSnumber,RteSearchTable[tableinstance][17][row].LSnumber,RteSearchTable[tableinstance][18][row].LSnumber,\
					RteSearchTable[tableinstance][19][row].LSnumber,RteSearchTable[tableinstance][20][row].LSnumber,RteSearchTable[tableinstance][21][row].LSnumber,\
					RteSearchTable[tableinstance][22][row].LSnumber,RteSearchTable[tableinstance][23][row].LSnumber,RteSearchTable[tableinstance][24][row].LSnumber,\
					RteSearchTable[tableinstance][25][row].LSnumber,RteSearchTable[tableinstance][26][row].LSnumber,RteSearchTable[tableinstance][27][row].LSnumber,\
					RteSearchTable[tableinstance][28][row].LSnumber,RteSearchTable[tableinstance][29][row].LSnumber,RteSearchTable[tableinstance][30][row].LSnumber,\
					RteSearchTable[tableinstance][31][row].LSnumber,RteSearchTable[tableinstance][32][row].LSnumber);
	}
	break;

	case 2:
	{

	printf("\n%s %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d",cstr, \
						RteSearchTable[tableinstance][1][row].LStype,RteSearchTable[tableinstance][2][row].LStype,RteSearchTable[tableinstance][3][row].LStype,\
						RteSearchTable[tableinstance][4][row].LStype,RteSearchTable[tableinstance][5][row].LStype,RteSearchTable[tableinstance][6][row].LStype,\
						RteSearchTable[tableinstance][7][row].LStype,RteSearchTable[tableinstance][8][row].LStype,RteSearchTable[tableinstance][9][row].LStype,\
						RteSearchTable[tableinstance][10][row].LStype,RteSearchTable[tableinstance][11][row].LStype,RteSearchTable[tableinstance][12][row].LStype,\
						RteSearchTable[tableinstance][13][row].LStype,RteSearchTable[tableinstance][14][row].LStype,RteSearchTable[tableinstance][15][row].LStype,\
						RteSearchTable[tableinstance][16][row].LStype,RteSearchTable[tableinstance][17][row].LStype,RteSearchTable[tableinstance][18][row].LStype,\
						RteSearchTable[tableinstance][19][row].LStype,RteSearchTable[tableinstance][20][row].LStype,RteSearchTable[tableinstance][21][row].LStype,\
						RteSearchTable[tableinstance][22][row].LStype,RteSearchTable[tableinstance][23][row].LStype,RteSearchTable[tableinstance][24][row].LStype,\
						RteSearchTable[tableinstance][25][row].LStype,RteSearchTable[tableinstance][26][row].LStype,RteSearchTable[tableinstance][27][row].LStype,\
						RteSearchTable[tableinstance][28][row].LStype,RteSearchTable[tableinstance][29][row].LStype,RteSearchTable[tableinstance][30][row].LStype,\
						RteSearchTable[tableinstance][31][row].LStype,RteSearchTable[tableinstance][32][row].LStype);
	}
	break;

	case 3:
	{
		printf("\n%s %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d",cstr, \
				RteSearchTable[tableinstance][1][row].Branchtablenumber,RteSearchTable[tableinstance][2][row].Branchtablenumber,RteSearchTable[tableinstance][3][row].Branchtablenumber,\
				RteSearchTable[tableinstance][4][row].Branchtablenumber,RteSearchTable[tableinstance][5][row].Branchtablenumber,RteSearchTable[tableinstance][6][row].Branchtablenumber,\
				RteSearchTable[tableinstance][7][row].Branchtablenumber,RteSearchTable[tableinstance][8][row].Branchtablenumber,RteSearchTable[tableinstance][9][row].Branchtablenumber,\
				RteSearchTable[tableinstance][10][row].Branchtablenumber,RteSearchTable[tableinstance][11][row].Branchtablenumber,RteSearchTable[tableinstance][12][row].Branchtablenumber,\
				RteSearchTable[tableinstance][13][row].Branchtablenumber,RteSearchTable[tableinstance][14][row].Branchtablenumber,RteSearchTable[tableinstance][15][row].Branchtablenumber,\
				RteSearchTable[tableinstance][16][row].Branchtablenumber,RteSearchTable[tableinstance][17][row].Branchtablenumber,RteSearchTable[tableinstance][18][row].Branchtablenumber,\
				RteSearchTable[tableinstance][19][row].Branchtablenumber,RteSearchTable[tableinstance][20][row].Branchtablenumber,RteSearchTable[tableinstance][21][row].Branchtablenumber,\
				RteSearchTable[tableinstance][22][row].Branchtablenumber,RteSearchTable[tableinstance][23][row].Branchtablenumber,RteSearchTable[tableinstance][24][row].Branchtablenumber,\
				RteSearchTable[tableinstance][25][row].Branchtablenumber,RteSearchTable[tableinstance][26][row].Branchtablenumber,RteSearchTable[tableinstance][27][row].Branchtablenumber,\
				RteSearchTable[tableinstance][28][row].Branchtablenumber,RteSearchTable[tableinstance][29][row].Branchtablenumber,RteSearchTable[tableinstance][30][row].Branchtablenumber,\
				RteSearchTable[tableinstance][31][row].Branchtablenumber,RteSearchTable[tableinstance][32][row].Branchtablenumber);

	}
	break;

	case 4:
	{
		printf("\n%s %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d %03d",cstr, \
					RteSearchTable[tableinstance][1][row].Branchtablestatus,RteSearchTable[tableinstance][2][row].Branchtablestatus,RteSearchTable[tableinstance][3][row].Branchtablestatus,\
					RteSearchTable[tableinstance][4][row].Branchtablestatus,RteSearchTable[tableinstance][5][row].Branchtablestatus,RteSearchTable[tableinstance][6][row].Branchtablestatus,\
					RteSearchTable[tableinstance][7][row].Branchtablestatus,RteSearchTable[tableinstance][8][row].Branchtablestatus,RteSearchTable[tableinstance][9][row].Branchtablestatus,\
					RteSearchTable[tableinstance][10][row].Branchtablestatus,RteSearchTable[tableinstance][11][row].Branchtablestatus,RteSearchTable[tableinstance][12][row].Branchtablestatus,\
					RteSearchTable[tableinstance][13][row].Branchtablestatus,RteSearchTable[tableinstance][14][row].Branchtablestatus,RteSearchTable[tableinstance][15][row].Branchtablestatus,\
					RteSearchTable[tableinstance][16][row].Branchtablestatus,RteSearchTable[tableinstance][17][row].Branchtablestatus,RteSearchTable[tableinstance][18][row].Branchtablestatus,\
					RteSearchTable[tableinstance][19][row].Branchtablestatus,RteSearchTable[tableinstance][20][row].Branchtablestatus,RteSearchTable[tableinstance][21][row].Branchtablestatus,\
					RteSearchTable[tableinstance][22][row].Branchtablestatus,RteSearchTable[tableinstance][23][row].Branchtablestatus,RteSearchTable[tableinstance][24][row].Branchtablestatus,\
					RteSearchTable[tableinstance][25][row].Branchtablestatus,RteSearchTable[tableinstance][26][row].Branchtablestatus,RteSearchTable[tableinstance][27][row].Branchtablestatus,\
					RteSearchTable[tableinstance][28][row].Branchtablestatus,RteSearchTable[tableinstance][29][row].Branchtablestatus,RteSearchTable[tableinstance][30][row].Branchtablestatus,\
					RteSearchTable[tableinstance][31][row].Branchtablestatus,RteSearchTable[tableinstance][32][row].Branchtablestatus);

	}
	break;

	}

}


void LNSearchprocess::InitializeRteSearchTable(int tableinstance)
{
	// if tableinstance = 0 then initialize all tables else initailize the specified table

	// RteSearchTable[MAXROUTESEARCTABLEINSTANCES][MAXROUTESEARCHTABLECOLUMNS][MAXROUTESEARCHTABLEROWS][MAXROUTESEARCHTABLEITEMS]


	if (tableinstance == 0)
	{
	//	std::cout<<"\n\n226 Initialize all Route Search tables";

		for (int STinstances = 0;STinstances < MAXROUTESEARCTABLEINSTANCES + 1; STinstances++)
		{
			for (int STcolumns = 0; STcolumns < MAXROUTESEARCHTABLECOLUMNS + 1; STcolumns++)
			{
				for (int STrows = 0; STrows < MAXROUTESEARCHTABLEROWS + 1; STrows++)
				{
					RteSearchTable[STinstances][STcolumns][STrows].LSnumber = 0;
					RteSearchTable[STinstances][STcolumns][STrows].Branchtablestatus = 0;
					RteSearchTable[STinstances][STcolumns][STrows].LSnumber = 0;
					RteSearchTable[STinstances][STcolumns][STrows].LStype = 0;
				}
			}

			RteSearchTableStepsstored[STinstances] = 0;
		}
	}
	else
	{
		for (int STcolumns = 0; STcolumns < MAXROUTESEARCHTABLECOLUMNS; STcolumns++)
		{
			for (int STrows = 0; STrows < MAXROUTESEARCHTABLEROWS; STrows++)
			{
				RteSearchTable[tableinstance][STcolumns][STrows].LSnumber = 0;
				RteSearchTable[tableinstance][STcolumns][STrows].Branchtablestatus = 0;
				RteSearchTable[tableinstance][STcolumns][STrows].LSnumber = 0;
				RteSearchTable[tableinstance][STcolumns][STrows].LStype = 0;
			}
		}

		RteSearchTableStepsstored[tableinstance] = 0;
	}

	Rsrchtablesbuilt = 0;

	Branchtablesbuilt = 1;
}

std::vector<int> LNSearchprocess::Runsearch(LNRteLogicalsectionGroup* LNRteLSG,int SrchRte, int RunStartLS,int RunMidLS, int RunEndLS, int RunMaxsteps, int diagnosticsflag)
{
	//std::cout<<"\n\n356 LNSearchprocess -  Starting to execute the Runsearch process to build Routes ............";

	Rtesrchsteps.clear();

	RteSearchTablebuild(LNRteLSG,RunStartLS, RunMidLS, RunEndLS, RunMaxsteps,0);

	return Rtesrchsteps;

}
// ================ Vector copy routine =================================================================
std::vector<int> LNSearchprocess::Vectcopy(std::vector<int> &srcevect, std::vector<int> &destvect)
{
	 int i;

	 for (i=0; i<srcevect.size(); i++)
        destvect.push_back(srcevect[i]);

     return  destvect;

}

int LNSearchprocess::SingleTablebuild(LNRteLogicalsectionGroup* LNRteLSG, int Tableinprocess, int StartLS, int MidLS, int EndLS, int Maxsteps, int diagnosticsflag)
{
	// build details for table number as passed as a parameter
	// Routine will populate the main LS row and identify branch tables required and create the initial stubs for these
	//

	int currentLS = 0;

	int currentNS = 0;

	int MidLSfound = 0;

	int LStocopy;

	if (diagnosticsflag > 0)
	{
		std::cout<<"\n586 ========== Building Single Table details for Table "<<Tableinprocess<<" ====================";
	}

	LNRteLogicalsectionData LSisrch;

	// Fetch and process next section details


	for (int stepcounter = 0; stepcounter < Maxsteps; stepcounter++)
	{
		if (RSTB_table_columnindex[Tableinprocess] > MAXROUTESEARCHTABLECOLUMNS)
		{
			std::cout<<"\n\n274 LNSearchprocess Route Build Error for Table "<<Tableinprocess<<" for Route StartLS:"<<StartLS<<" EndLS:"<<EndLS;
			std::cout<<"\n\n274 LNSearchprocess Route Build Error - column index:"<<RSTB_table_columnindex[Tableinprocess]<<" greater than:"<<MAXROUTESEARCHTABLECOLUMNS;
			std::cout<<"\nProgram aborted ";
			std::cout<<"\n\n\n";
			exit(0);
		}

		currentLS = RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LSnumber;

		if ((MidLS > 0) && (MidLS == currentLS))
		{
			MidLSfound = MidLS;
		//	std::cout<<"\n288 MidLS found "<<MidLS;
		}

		if (currentLS == 0)
		{
		//	std::cout<<"\n609 currentLS is zero in Table "<<Tableinprocess;
			return 0;
		}

		if ((currentLS == EndLS) && (RSTB_table_columnindex[Tableinprocess] > 1) && (MidLS == 0))
		{
		//	std::cout<<"\n\n298 EndLS detected as currentLS so stop search";
			return 8;
		}

		if ((currentLS == EndLS) && (RSTB_table_columnindex[Tableinprocess] > 1) && (MidLSfound > 0))
		{
		//	std::cout<<"\n\n304 MidLS found and EndLS detected as currentLS so stop search";
			return 12;
		}

		LSisrch = LNRteLSG->GetLSreference(currentLS);			// get vector of section details for Logical section

		if (diagnosticsflag > 0)
		{
			std::cout<<"\n606 Stepcounter = "<<stepcounter<<" Current LS:"<<currentLS;
		}

		NS = LSisrch.GetNextsteps();

	//	std::cout<<"\n304 NS members = "<<NS.size();

		if (diagnosticsflag > 0)
		{
			//	std::cout<<"\n 613 Single Table -> NS:"<<NS.at(0)<<" NS-b1:"<<NS.at(1)<<" NS-b2:"<<NS.at(2)<<" NS-b3:"<<NS.at(3);
		}

		currentNS = NS.at(0);

		if (currentNS > 0)
		{
			RSTB_table_columnindex[Tableinprocess]++;				// if NS is > 0 increment the column index and store the LS found
			RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LSnumber = currentNS;
			RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LStype++;

			for (int bp = 1;bp < MAXNUMBEROFNS;bp++)				// now process any branches and store them
			{
				//std::cout<<"\n622 NSB bp:"<<bp<<" = "<<NS.at(bp);

				if (NS.at(bp) > 0)									// check to see if there is a branch to process
				{													// Store branch details in Table and then create a branch table
					RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess] + bp].LSnumber = NS.at(bp);												// store the value on the next row at the same column
					RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LStype++;
					Branchtablesrequired++;

					// populate the Row 1 of the new branch table up to the current column

					RSTB_table_rowindex[Branchtablesrequired] = 1;

					for (int tc = 1; tc < RSTB_table_columnindex[Tableinprocess];tc++)
					{
						RSTB_table_columnindex[Branchtablesrequired] = tc;

						LStocopy = RteSearchTable[Tableinprocess][tc][RSTB_table_rowindex[Tableinprocess]].LSnumber;

						RteSearchTable[Branchtablesrequired][RSTB_table_columnindex[Branchtablesrequired]][RSTB_table_rowindex[Branchtablesrequired]].LSnumber = LStocopy;
						RteSearchTable[Branchtablesrequired][RSTB_table_columnindex[Branchtablesrequired]][RSTB_table_rowindex[Branchtablesrequired]].LStype++;
					}

					// move the branch LS into row 1 of the new branch table at the next column.

					RSTB_table_columnindex[Branchtablesrequired]++;

					RteSearchTable[Branchtablesrequired][RSTB_table_columnindex[Branchtablesrequired]][RSTB_table_rowindex[Branchtablesrequired]].LSnumber = NS.at(bp);
					RteSearchTable[Branchtablesrequired][RSTB_table_columnindex[Branchtablesrequired]][RSTB_table_rowindex[Branchtablesrequired]].LStype++;
				}
			}
		}
		else
		{
		//	std::cout<<"\n632 next NS is zero for step:"<<stepcounter;
			break;
		}
	}

	if (diagnosticsflag > 0)
	{
		std::cout<<"\n637 All NS processed ";
	}

	return 0;
}

void LNSearchprocess::RteSearchTablebuild(LNRteLogicalsectionGroup* LNRteLSG, int StartLS,int MidLS, int EndLS, int Maxsteps, int diagnosticsflag)
{
	// initialize Table 1 for start of search.

	if (diagnosticsflag > 0)
	{
		std::cout<<"\n\n ========= TABLE BASED ROUTE BUILDER ========================";
		std::cout<<"\n\n575 Starting Table based Route search";
		std::cout<<"\nStart LS:"<<StartLS<<" End LS:"<<EndLS<<" Max number of search steps:"<<Maxsteps;
	}

	Tableinprocess = 1;

	int Searchcompleted = 0;

	int TPCfound 	= 0;

	InitializeRteSearchTable(0);			// clear all route search tables

	ClearRtesearchtableindices();			// clear search tables indices

	RteSearchTableStepsstored[Tableinprocess]++;			// increment step counter for table 1

	RSTB_table_rowindex[Tableinprocess] = 1;

	RSTB_table_columnindex[Tableinprocess] = RteSearchTableStepsstored[1];

	RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LSnumber = StartLS;

	RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].LStype	= 1;

	RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].Branchtablenumber = 0;

	RteSearchTable[Tableinprocess][RSTB_table_columnindex[Tableinprocess]][RSTB_table_rowindex[Tableinprocess]].Branchtablestatus = 0;

	//std::cout<<"\n\n====> 706 Process Table 1";

	int SingletablebuildRC = 0;

	SingletablebuildRC = SingleTablebuild(LNRteLSG, Tableinprocess, StartLS, MidLS, EndLS, Maxsteps, diagnosticsflag);

	if (SingletablebuildRC == 8)
	{
	//	std::cout<<"\n421 Search completed -  EndLS found in table 1 - no MidLS specified";

		Searchcompleted = 1;

		TPCfound = 1;
	}


	if (SingletablebuildRC == 12)
	{
	//	std::cout<<"\n429 Search completed - MidLS and EndLS found in table 1";

		Searchcompleted = 2;

		TPCfound = 1;
	}

	if (Searchcompleted == 0)  // Table 1 search not completed so process the next search tables
	{
		for (int tpc = 2; tpc < Maxsteps; tpc++)
		{
		//std::cout<<"\n\n====> 712 Processing Table "<<tpc;

			SingletablebuildRC = SingleTablebuild(LNRteLSG, tpc, StartLS, MidLS, EndLS, Maxsteps, diagnosticsflag);

			if ((SingletablebuildRC == 8) && (MidLS == 0))
			{
				TPCfound = tpc;
				break;
			}

			if ((SingletablebuildRC == 12) && (MidLS > 0))
			{
				TPCfound = tpc;
				break;
			}
		}
	}

	//std::cout<<"\n455 SingletablebuildRC =>"<<SingletablebuildRC<<" tpc "<<TPCfound;

	if (MidLS == 0)
	{
		if (CheckforENDls(EndLS, Maxsteps) > 0)		// search all Route tables built to check if EndLS is reached
		{
			//std::cout<<"\n460 Search Result - EndLS found in Table "<<EndLStable<<" at column "<<EndLStablecolumn;

		// EndLS found so populate the vector list of the search steps;

			for (int sscc = 1; sscc <= EndLStablecolumn; sscc++)
			{
		//	std::cout<<"\n475 sscc:"<<sscc<<" LS = "<<RteSearchTable[EndLStable][sscc][1].LSnumber;

			Rtesrchsteps.push_back(RteSearchTable[EndLStable][sscc][1].LSnumber);
			}
		}
		else
		{
			std::cout<<"\n\n433 EndLS "<<EndLS<<" not found in search";
		}
	}

	if (MidLS > 0)
	{
		std::cout<<"\n\n479 Checking route for midpoint LS:"<<MidLS<<" and Endpoint LS:"<<EndLS;
		std::cout<<"\n\n";

		if (CheckforMIDandENDls(EndLS,MidLS, Maxsteps) > 0)		// search all Route tables built to check if EndLS is reached
		{
				//	std::cout<<"\n493 Search Result - MidLS and EndLS found in Table "<<EndLStable<<" at column "<<EndLStablecolumn;

				// EndLS found so populate the vector list of the search steps;

				//	printf("\n\n");

					for (int sscc = 1; sscc <= EndLStablecolumn; sscc++)
					{
				//	std::cout<<"\n499 sscc:"<<sscc<<" LS = "<<RteSearchTable[EndLStable][sscc][1].LSnumber;

					Rtesrchsteps.push_back(RteSearchTable[EndLStable][sscc][1].LSnumber);
					}
		}
		else
		{
					std::cout<<"\n\n457 EndLS "<<EndLS<<" not found in search";
		}
	}




	if (diagnosticsflag > 0)
	{
		std::cout<<"\n\n515 Number of Branch tables to be displayed is :"<<Branchtablesrequired<<"\n";
		for (int dpc = 1; dpc <= Branchtablesrequired; dpc++)
		{
			DisplayRteSearchTable(dpc);
		}
	}
}

int LNSearchprocess::CheckforENDls(int EndLS, int Maxsteps)
{
	int ENDlsfound = 0;

	EndLStable = 0;

	EndLStablecolumn = 0;

	for (int tsEnd = 1; tsEnd < Maxsteps; tsEnd++)
	{
		for (int tsELS = 2; tsELS <= Maxsteps; tsELS++)			 		// start at column 2 in case this is a loop route where StartLS = EndLS
		{
			if (RteSearchTable[tsEnd][tsELS][1].LSnumber == EndLS)
			{
			//	std::cout<<"\n759 EndLS found ";
				EndLStable = tsEnd;
				EndLStablecolumn = tsELS;
				break;
			}
		}
	}

	return EndLStable;
}

int LNSearchprocess::CheckforMIDandENDls(int EndLS,int MidLS, int Maxsteps)
{
	int ENDlsfound = 0;

		EndLStable = 0;

		int Midpointfound = 0;

		int Endpointfound = 0;

		EndLStablecolumn = 0;

		for (int tsEnd = 1; tsEnd < Maxsteps; tsEnd++)
		{
			//printf("\n\n Table %03d \n",tsEnd);

			ENDlsfound = 0;

			Midpointfound = 0;

			Endpointfound = 0;

			for (int tsELS = 2; tsELS <= Maxsteps; tsELS++)			 		// start at column 2 in case this is a loop route where StartLS = EndLS
			{
				//printf("\n565 tsEnd %03d tsELS %03d LS entry %03d ",tsEnd,tsELS,RteSearchTable[tsEnd][tsELS][1].LSnumber);

				if (RteSearchTable[tsEnd][tsELS][1].LSnumber == MidLS)
				{
					Midpointfound = tsELS;
				//	std::cout<<"\n\n516 Midpoint LS "<<MidLS<<" found at Table entry "<<Midpointfound<<"\n";
				}

				if (RteSearchTable[tsEnd][tsELS][1].LSnumber == EndLS)
				{
					Endpointfound = tsELS;
				//	std::cout<<"\n\n759 EndLS found at tsELS:"<<Endpointfound<<"\n";
					//EndLStable = tsEnd;
					//EndLStablecolumn = tsELS;
					//return EndLStable;
				}

				if ((Endpointfound > 0) && (Midpointfound > 0) && (Endpointfound > Midpointfound))
				{
				//	printf("\n\n581 ==>  Midpoint %03d and Endpoint %03d found in correct sequence\n\n",Midpointfound,Endpointfound);
					EndLStable = tsEnd;
					EndLStablecolumn = tsELS;
					return EndLStable;
				}
			}
		}
		return 0;
}

void LNSearchprocess::ClearRtesearchtableindices()
{
	for (int tcs = 0; tcs < MAXROUTESEARCTABLEINSTANCES + 1;tcs++)
	{
		RSTB_table_columnindex[tcs] 	= 0;
		RSTB_table_rowindex[tcs]		= 0;
		RSTB_Branchcounter[tcs]			= 0;
		RSTB_Branchtablesrequired[tcs]	= 0;
		RSTB_LStype[tcs]				= 0;
		RSTB_SectionInprocess[tcs]		= 0;
	}
}

