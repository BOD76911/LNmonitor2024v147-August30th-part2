/*
 * LNRteLogicalsectionGroup.h
 *
 *  Created on: Aug 18, 2021
 *      Author: BrianRoot
 */

#ifndef LNRTELOGICALSECTIONGROUP_H_
#define LNRTELOGICALSECTIONGROUP_H_

#include "LNRteLogicalsectionData.h"

class LNRteLogicalsectionGroup
{

// stores the logical section details loaded from the logical section configuration file

public:

		LNRteLogicalsectionGroup();

		~LNRteLogicalsectionGroup();

		void DisplayLScount();

		int DisplayLSstorecounter();

		//int ExportLSVectorData();

		int ExportLSVectorData(char *Weboutput);

		LNRteLogicalsectionData GetLSreference(int section);

		int GetnumberofLSelements();

		void InitializetheGroup();

		int LoadLogicalSectiondataGroup(LNwebLsectionsLdevices* LNLSLD,int Numberofsections);

		void StoreLSonVector(LNRteLogicalsectionData LSv);  // store logical section details on the LogicalsectionGroup vector

		void DisplayAllLSdata(char *replymessage);	// Displays all the logical section data stored in the logical section group.

private:

		unsigned int LSstorecount;

		std::vector <LNRteLogicalsectionData> LSGroup;


};

#endif /* LNRTELOGICALSECTIONGROUP_H_ */
