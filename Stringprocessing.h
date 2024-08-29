/*
 * Stringprocessing.h
 *
 *  Created on: Jul 28, 2018
 *      Author: BrianRoot
 *
 *      Routine for search for a specified string
 */

#ifndef STRINGPROCESSING_H_
#define STRINGPROCESSING_H_

#include <string>
#include <iostream>

class Stringprocessing {
public:
	Stringprocessing();
	virtual ~Stringprocessing();

	void Searchtext(char& message, char* searchvalue, char& stringtosearch, int displayflag, char endchar, int stringlength);

	int Getsearchstringlength();

	int Getsearchtermlength();

	int Getsearchstringposition();

	int Getfullmatchpos();

private:

	int inboundtextlength;

	int searchtermlength;

	int seachstringposition;

	int searchstringlength;

	int firstcharmatchpos;

	int fullmatchpos;

};

#endif /* STRINGPROCESSING_H_ */
