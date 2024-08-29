/*
 * LNwebmonNGfileload.cpp
 *
 *  Created on: Jun 15, 2019
 *      Author: BrianRoot
 */

#include "LNwebmonNGfileload.h"
#include "Bufferprocessing.h"
#include <sstream>
#include <string.h>
#include <strings.h>
#include <iostream>			// include standard library of io routines
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>     /* atoi */


LNwebmonNGfileload::LNwebmonNGfileload() {
	// TODO Auto-generated constructor stub

	//std::cout <<"\nCreating file load object ";
}

LNwebmonNGfileload::~LNwebmonNGfileload() {
	// TODO Auto-generated destructor stub

	//std::cout <<"\nDeleting file load object ";
}

char* LNwebmonNGfileload::GetInputbufferptr()
{
	return &lineinputbuf[0];
}

int LNwebmonNGfileload::Fileread(char *filename,int Displayrecflag) // Read a text file
{
	std::string line;

	FILE *lnLNptr_file;

	int sc1, sc2, sc3, recnum;

	char *Linput_PTR = &lineinputbuf[0];

	lnLNptr_file =fopen(filename,"r");

	    		if (!lnLNptr_file)
	    		{
					std::cout << "\n\nFile " << filename << " not found \n" ;
					return 8;
				}
				else
				{
					//cout << "\nFile " << filename << " is OK \n";
				}

	    		Datalinesread = 0;

	    		TotalDatarecordsloaded = 0;

	    		recnum = 0;

	    		Commentlinesread = 0;

	    		HeaderRecord_OK = false;

	    		while (fgets(lineinputbuf,LINEINPUTBUFSIZE, lnLNptr_file)!= NULL)
	    		        	{


	    						if ((*Linput_PTR == 'X') || (*Linput_PTR == 'x'))
	    						{
	    							fclose(lnLNptr_file);

	    							TotalDatarecordsloaded = recnum;

	    							if (Displayrecflag > 0)
	    							{

	    								std::cout <<"\n " << std::setfill('0') << std::setw(4) << Commentlinesread << " Comment lines read ";

	    								std::cout <<"\n " << std::setfill('0') << std::setw(4) << Datalinesread << " Data lines read ";

	    								//TotalDatarecordsloaded = recnum;

	    								std::cout <<"\n " << std::setfill('0') << std::setw(4) << TotalDatarecordsloaded << " Data records loaded  ";

	    								std::cout <<"\n\n";
	    							}

	    							/*************** loaded data check ********************************/

	    							//LSi.DisplayLSdata();

	    							if (Displayrecflag > 0)
	    							{
	    								DisplayFieldMetadata();
	    							}

	    							return 0;
	    						}

	    						if ((*Linput_PTR == ' ' ) || (*Linput_PTR == '/') || (*Linput_PTR == '\n') || (*Linput_PTR == '\r') )
	    						{
	    							Commentlinesread++;

	    							//cout << "\nComment Line read :" << lineinputbuf ;
	    						}
	    						else
	    						{

	    							if (Displayrecflag > 0)
	    							{
	    								std::cout << "\nData line read :" << lineinputbuf;
	    							}

	    							Bufferprocessing* FBP = new Bufferprocessing();

	    							FBP->Fileparse(lineinputbuf,';',0);

	    							/*
	    							  Header format

										0	00	type - header
										1	00 fields on the header
										2	00 field 1 - start position
										3	00 field 1 - length
									 	4  00 field 1 - type     1 - numeric - store in integer , 2 character

										5	00 field 2 - start position
										6	00 field 2 - length
									 	7  00 field 2 - type     1 - numeric - store in integer , 2 character

										8	00 field 3 - start position
										9	00 field 3 - length
									     10 00 field 3 - type     1 - numeric - store in integer , 2 character

										11	00 field 4 - start position
										12	00 field 4 - length
									 	13 00 field 4 - type     1 - numeric - store in integer , 2 character

											## 			end of file indicator

									  1  2   3  4  5   6  7  8   9 10 11 12 13
										000;05;04;003;01;08;005;02;14;001;1; 16;30;02
	    							*/

	    							Rectype = atoi(FBP->Paramdata(0));

	    							if (Rectype == 0)		// File header record
	    							{
	    								FieldsOnRecord = atoi(FBP->Paramdata(1));	// file header record -  read field count

	    								if (Displayrecflag > 0)
	    								{

	    									std::cout << "\n" << FieldsOnRecord << " fields on the header record \n";
	    								}

	    								//int sc1, sc2, sc3;

	    								sc1 = 1;

	    								sc2 = 2;

	    								if (Displayrecflag > 0)
	    								{
	    									std::cout << "\nProcessing header record " << lineinputbuf;
	    								}

	    								for (FoRsc = 0;FoRsc < FieldsOnRecord; FoRsc++)
	    								{
	    					/*
	    					int Fieldstart[MAXINPUTPARAMCNT];

							int Fieldlen[MAXINPUTPARAMCNT];

							int Fieldtype[MAXINPUTPARAMCNT];
	    					*/
	    									Fieldstart[sc1] = atoi(FBP->Paramdata(sc2));
	    									Fieldlen[sc1] = atoi(FBP->Paramdata(sc2+1));
	    									Fieldtype[sc1] = atoi(FBP->Paramdata(sc2+2));


	    									sc1 = sc1 + 1;
	    									sc2 = sc2 + 3;
	    								}

	    								HeaderRecord_OK = true;
	    							}

	    							if ((Rectype == 1) && (HeaderRecord_OK))		// File Data record
	    							{
	    								Datalinesread++;

	    								recnum++;

	    								sc1 = 1;

	    								sc2 = 1;

	    								if (Displayrecflag > 0)
	    								{
	    									std::cout << "\nProcessing Data record " << lineinputbuf;
	    								}

	    								/*
	    									int Fieldnumericvalue[MAXINPUTPARAMCNT];

											char Fieldtextvalue[MAXINPUTPARAMCNT][MAXINPUTPARAMLEN];
	    								*/

	    								// Process each field on the record using the Meta data

	    								for (FoRsc = 0;FoRsc < FieldsOnRecord; FoRsc++)
	    								{

	    									//std::cout << "\n222 Field " << FoRsc << " Fieldtype: " << Fieldtype[sc1];

	    										if (Fieldtype[sc1] == 1)      // test for numeric field
	    										{

	    											//std::cout <<"\n227 Numeric field value to convert at sc2 " << FBP->Paramdata(sc2);
	    											Fieldnumericvalue[recnum][sc1] = atoi(FBP->Paramdata(sc2));
	    											Fieldtextvalue[recnum][sc1][0] = '\0';

	    											//cout << "\nNumeric field : " << FBP.Paramdata(sc2);

	    										}

	    										if (Fieldtype[sc1] == 2)      // test for text field
	    										{
	    											Fieldnumericvalue[recnum][sc1] = 0;

	    											int endchar;

	    											endchar = Fieldlen[sc1];

	    											//std::cout << "\n243 Text field --> " << FBP->Paramdata(sc2);

	    											//char buffer[20];
	    											 // std::string str ("Test string...");
	    											 // std::size_t length = str.copy(buffer,6,5);

	    											  sprintf(Fieldtextvalue[recnum][sc1],"%s",FBP->Paramdata(sc2));

	    											  for (int ftv = 1; ftv < MAXINPUTPARAMLEN;ftv++)
	    											  {
	    												  if ((Fieldtextvalue[recnum][sc1][ftv]== ' ') && (endchar == 5)) // check for command text
	    												  {
	    													  Fieldtextvalue[recnum][sc1][ftv] = '\0';
	    													  break;
	    												  }

	    											  }


	    											 // buffer[length]='\0';

	    											 // cout << "\nBuffer val --> " << buffer;
	    										}

	    										sc1 = sc1 + 1;
	    										sc2 = sc2 + 1;
	    								}

	    								if (Displayrecflag > 0)
	    								{
	    									DisplayFieldsOnRecord(recnum);
	    								}
	    							}

	    							if ((Rectype == 1) && (!HeaderRecord_OK))		// File Data record
	    							{
	    								std::cout << "\nNo META data header for file to be loaded ";
	    								delete FBP;

	    								return 98;
	    							}
	    						 delete FBP;
	    						}
	    		        	}
	 return 0;
}

int LNwebmonNGfileload::GetNumberofDatarecordsloaded()
{
	return TotalDatarecordsloaded;

}

int LNwebmonNGfileload::GetRecordsFieldValue(int recnum, int fieldnum)
{
	return Fieldnumericvalue[recnum][fieldnum];   // retrieve the numeric value of the specified field
}

char* LNwebmonNGfileload::GetRecordFieldtext(int recnum, int fieldnum)
{

	char* textvalptr = &(Fieldtextvalue[recnum][fieldnum][0]);  //  point to first value in the specified text field on the record

	return textvalptr;
}


void LNwebmonNGfileload::DisplayFieldMetadata()
{
	std::cout << "\n\nMeta data loaded \n";

	for (FoRsc = 0;FoRsc <= FieldsOnRecord; FoRsc++)
	{
		std::cout << "\nField " << FoRsc << " Start: " << Fieldstart[FoRsc] << " Len: " << Fieldlen[FoRsc] << " Type: " << Fieldtype [FoRsc];
	}
}

void LNwebmonNGfileload::DisplayFieldsOnRecord(int reccnt)
{
	std::cout << "\n\nData Record field values  \n";

		for (FoRsc = 0;FoRsc <= FieldsOnRecord; FoRsc++)
		{
			std::cout << "\nField " << FoRsc << " Numeric value: " << Fieldnumericvalue[reccnt][FoRsc] << " Text value -->" << Fieldtextvalue[reccnt][FoRsc];
		}
}
