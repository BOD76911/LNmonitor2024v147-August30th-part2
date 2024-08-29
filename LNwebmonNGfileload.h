/*
 * LNwebmonNGfileload.h
 *
 *  Created on: Jun 15, 2019
 *      Author: BrianRoot
 */

#ifndef LNWEBMONNGFILELOAD_H_
#define LNWEBMONNGFILELOAD_H_

#define SPLITFIELDSIZE     8

#include "LNwebconfig.h"
#include <vector>

class LNwebmonNGfileload {
public:
	LNwebmonNGfileload();
	virtual ~LNwebmonNGfileload();

	int Fileread(char *filename,int Displayrecflag); // Read a text file

	int GetNumberofDatarecordsloaded();

	char* GetInputbufferptr();

	int GetRecordsFieldValue(int recnum, int fieldnum);

	char* GetRecordFieldtext(int recnum, int fieldnum);

	void DisplayFieldMetadata();

	void DisplayFieldsOnRecord(int reccnt);

private:

	int Datalinesread;

	int TotalDatarecordsloaded;

	int Commentlinesread;

	char lineinputbuf[LINEINPUTBUFSIZE ];

	char Linesplitdata[SPLITFIELDSIZE];

	int Rectype;

	// Record Meta data

	int FieldsOnRecord;

	int Fieldstart[MAXINPUTPARAMCNT];

	int Fieldlen[MAXINPUTPARAMCNT];

	int Fieldtype[MAXINPUTPARAMCNT];

	// data records value

	int Fieldnumericvalue[MAXRECSINFILE][MAXINPUTPARAMCNT];

	char Fieldtextvalue[MAXRECSINFILE][MAXINPUTPARAMCNT][MAXINPUTPARAMLEN];

	int FoRsc;

	bool HeaderRecord_OK;
};

#endif /* LNWEBMONNGFILELOAD_H_ */
