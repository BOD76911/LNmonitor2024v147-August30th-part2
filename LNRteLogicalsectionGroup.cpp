/*
 * LNRteLogicalsectionGroup.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#include "LNRteLogicalsectionGroup.h"
#include "LNRteLogicalsectionData.h"

LNRteLogicalsectionGroup::LNRteLogicalsectionGroup() {
	// TODO Auto-generated constructor stub

	LSstorecount = 0;

}

LNRteLogicalsectionGroup::~LNRteLogicalsectionGroup() {
	// TODO Auto-generated destructor stub
}


int LNRteLogicalsectionGroup::LoadLogicalSectiondataGroup(LNwebLsectionsLdevices* LNLSLD,int Numberofsections)
{

	//std::cout<<"\n26 LNRtelogicalsectionGroup loading for "<<Numberofsections<<" sections ";

	LNRteLogicalsectionData LSRDi;

	std::vector <int> LSsectiondata;

	int numberofLSobjects = 0;

	int sc = 0;

	for (sc = 1;sc <=Numberofsections;sc++)
	{

	// Fetch section details - these are returned in a vector

	LSsectiondata = LNLSLD->GetLSdataforRte(sc);

	LSRDi.StoreLSdata(LSsectiondata[0],LSsectiondata[2],LSsectiondata[3],LSsectiondata[5],LSsectiondata[1],LSsectiondata[4], \
			LSsectiondata[6],LSsectiondata[7],LSsectiondata[8],LSsectiondata[9],LSsectiondata[10],LSsectiondata[11],LSsectiondata[12],LSsectiondata[13],LSsectiondata[14], LSsectiondata[15],\
			LSsectiondata[16],LSsectiondata[17],LSsectiondata[18]);

	StoreLSonVector(LSRDi);

	numberofLSobjects++;
	}

	return numberofLSobjects;
}

void LNRteLogicalsectionGroup::DisplayAllLSdata(char *replymessage)
{
	char outputrec[OUTPUTMSGBUFFERSIZE];

	bzero(outputrec,OUTPUTMSGBUFFERSIZE);

	int isc,ic,scc,mcc;

	mcc = 0;

	LNRteLogicalsectionData* LSDdisplay = new LNRteLogicalsectionData();

	sprintf(outputrec,"Logical Section data for Route processing: %i sections found: ",GetnumberofLSelements());

	isc =  std::strlen(outputrec);

	for (ic = 0;ic <isc;ic++)
	{
		*(replymessage + mcc) = outputrec[ic];
		mcc++;
	}

	//return;

	if (GetnumberofLSelements() > 0)
	{

	// Print the logical section group by iterating through each member

	for (std::vector<LNRteLogicalsectionData>::iterator dds = LSGroup.begin() ; dds != LSGroup.end(); ++dds)
		{
			*LSDdisplay = *dds;

			bzero(outputrec,OUTPUTMSGBUFFERSIZE);

			LSDdisplay->DisplayLSdata(outputrec);

			isc =  std::strlen(outputrec);

			for (ic = 0;ic <isc;ic++)
			{
				*(replymessage + mcc) = outputrec[ic];
				mcc++;
			}
		}
	}

	return;

}

int LNRteLogicalsectionGroup::ExportLSVectorData(char *Weboutput)
{
	LNRteLogicalsectionData LSDdisplay;

	char DRRline[OUTPUTLINELENGTH];

	char DRRfullrecord[OUTMSGBUFFERSIZE * 2];

	bzero(DRRfullrecord,OUTMSGBUFFERSIZE * 2);

	int cl, cm, tdr;

	tdr = 0;

	for (std::vector<LNRteLogicalsectionData>::iterator dds = LSGroup.begin() ; dds != LSGroup.end(); ++dds)
		{
			 LSDdisplay = *dds;
			 cl = LSDdisplay.BuildLSdataoutputrecord(&DRRline[0]);
			// std::cout <<  "\nRecord Build Output " << &DRRline[0] <<" - " <<  cl ;

			 for (cm = 0;cm < cl;cm++)
			 {
				DRRfullrecord[tdr] = DRRline[cm];
				tdr++;
				*Weboutput = DRRline[cm];
				Weboutput++;
			 }

			// std::cout << "\n\nFinal Build Output " << DRRfullrecord << " tdr = " << tdr;
		}

	//std::cout << "\n\nFinal Build Output " << &DRRfullrecord[0] << " tdr = " << tdr;
	return 0;
}

void LNRteLogicalsectionGroup::StoreLSonVector(LNRteLogicalsectionData LSv)
{
	//printf("\n\nStore logical section object on Logicalsection group vector \n");

	// std::vector <LogicalsectionData> LSGroup;

	LSGroup.push_back(LSv);

	LSstorecount++;

}

int LNRteLogicalsectionGroup::DisplayLSstorecounter()
{
	return LSstorecount;
}

int LNRteLogicalsectionGroup::GetnumberofLSelements()
{
	return LSGroup.size();
}

void LNRteLogicalsectionGroup::InitializetheGroup()
{

	LSGroup.clear();

	LSstorecount = 0;

	return;
}



LNRteLogicalsectionData LNRteLogicalsectionGroup::GetLSreference(int section)
{
	//printf("\n174 LNRteLogicalsectionData : Call for section details %i ",(section - 1));

	return LSGroup[section - 1];
}

