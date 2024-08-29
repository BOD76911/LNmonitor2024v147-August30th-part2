/*
 * Stringprocessing.cpp
 *
 *  Created on: Jul 28, 2018
 *      Author: BrianRoot
 */

#include "Stringprocessing.h"
#include <string>
#include "Bufferprocessing.h"
#include <stdio.h>


Stringprocessing::Stringprocessing() {
	// TODO Auto-generated constructor stub

}

Stringprocessing::~Stringprocessing() {
	// TODO Auto-generated destructor stub
}

int Stringprocessing::Getsearchstringlength()
{
	return inboundtextlength;
}

int Stringprocessing::Getsearchtermlength()
{
	return searchtermlength;
}

int Stringprocessing::Getfullmatchpos()
{
	return fullmatchpos;
}

void Stringprocessing::Searchtext(char& message, char* searchvalue, char& stringtosearch, int displayflag, char endchar, int stringlength)
{
	char* msgoutptr = &message;						// Setup the reference pointer to external output buffer address

	char* stringtosearchptr = &stringtosearch;		// Setup pointer to buffer of the string to be searched

	char* ccheck = searchvalue;

	// searchvalue is a pointer

	if (displayflag > 0)
	{
		std::cout << "\n\nSearch value to match " << searchvalue << "\nString to search " << stringtosearchptr;
	}

	int sc, ff,ff1,ff2;

	searchtermlength = 0;										// length of string to be matched

	inboundtextlength = 0;										// length of string to be searched for a match

	// determine the length of the string to be searched - ssl

	for (ff = 0;ff < stringlength;ff++)
	{
		//std::cout << "\n"<< *(stringtosearchptr +ff);

		if ((*(stringtosearchptr +ff) == '\n') || (*(stringtosearchptr +ff) == '\0') || (*(stringtosearchptr +ff) == 0X0D)                 )
		{
			if (displayflag > 0)
				{
					std::cout << "\nEnd of record marker found at char " << ff<< "\n";
				}
			break;
		}

		inboundtextlength = ff;
	}

	// determine the length of the text to be matched - ccl

	for (ff = 0;ff < stringlength;ff++)
		{
			//std::cout << "\n"<< *(searchvalue +ff);

			searchtermlength = ff;

			if ((*(searchvalue + ff)  == '\0') || ( *(searchvalue + ff)  == '\n') || ( *(searchvalue + ff)  == 0X0D))
			{
				break;
			}
		}


	if (displayflag == 2)
	{
		std::cout << "\n\nInput stringlength " << inboundtextlength  << " Search term length " << searchtermlength;
	}

		// Start search

		// Match on first character of the search term

	for (ff = 0; ff <= inboundtextlength; ff++)
	{

		if (displayflag == 2)
		{
		printf ("\nCC %c SC %c ",*ccheck,*(stringtosearchptr + ff));
		}

		if (*ccheck == *(stringtosearchptr + ff))
		{
			if (displayflag == 2)
			{
			std::cout <<"\nMatched 1st char at position " << ff;
			}

			for (ff1 = 0; ff1 < searchtermlength;ff1++)
			{
				if (*(ccheck + ff1) != *(stringtosearchptr + ff + ff1))
				{
					if (displayflag == 2)
					{
						std::cout <<"\nNo match at pos " << ff;
					}
					break;
				}
				else
				{
					if (displayflag == 2)
					{
					std::cout <<"\nchar match at pos ff1 " << ff;
					}
				}
			}

			if (ff1 == searchtermlength)									// full match found so extract the full string
			{
				if (displayflag == 2)
					{
					std::cout <<"\n\nFull char match at pos ff " << ff;
					}

				fullmatchpos = ff;

				for (ff2 = 0;ff2 < stringlength;ff2++)              // 64
				{
					if (*(stringtosearchptr + fullmatchpos + ff2) != endchar)
					{
						*(msgoutptr +ff2) = *(stringtosearchptr + fullmatchpos + ff2);
					}
					else
					{
						return;
					}
				}

				std::cout << "\n\nString process line 155 - Did not find the end character of the search string  ";
                std::cout << "\nSearching string->"<<stringtosearch<<"<----\n";
			}

		}

	}

   	//char* searchvalueptr = &searchvalue;			// Setup the search pointer to the search string

	//char* targetsearchptr = &stringtosearch;		// Setup the pointer to the string to be searched

}
