/*
 * WebBuildHTMLpage.cpp
 *
 *  Created on: Aug 6, 2018
 *      Author: BrianRoot
 */

#include "WebBuildHTMLpage.h"

WebBuildHTMLpage::WebBuildHTMLpage() {
	// TODO Auto-generated constructor stub

}

WebBuildHTMLpage::~WebBuildHTMLpage() {
	// TODO Auto-generated destructor stub
}

int WebBuildHTMLpage::GetFinalmessagelength()
{
	return Finalmessagelength;
}

int WebBuildHTMLpage::Pageprocess(int& conn, char& message, char* filename, int displayflag)
{
	char* msgptr = &message;						// Setup the reference pointer to external output buffer

		Bufferprocessing* BP = new Bufferprocessing();							// Create Buffer processing object

		BP->Fileparse(filename,'/',0);					// Parse the message

		std::string FileDir = WEBDirectory;

		std::string WEBfile = FileDir + BP->Paramdata(1);

		//std::cout << "\nline 36 Concatonated filename is "<< WEBfile;

		if (displayflag > 0)
		{
			std::cout << "\nWebBuildHTMLpage Param 3 :--" << *BP->Paramdata(1)<< "--";  // Display file name
		}

		Bufferprocessing* BPft = new Bufferprocessing();

		BPft->Fileparse(BP->Paramdata(1),'.',0);

		File_errorcode = 0;

		File_typecode = 0;

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"html",0) == 0)
		{
				Hlmsgtype = HlmsgHTMLtype;
		}

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"css",0) == 0)
		{
				Hlmsgtype = HlmsgCSStype;
		}

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"js",0) == 0)
		{
				Hlmsgtype = HlmsgJStype;
		}

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"jpg",0) == 0)
		{
				Hlmsgtype = HlmsgJPGtype;
		}


		if (displayflag > 0)
		{
			std::cout <<"\nWebBuildHTMLpage File type is--" << BPft->Paramdata(1) << "--";
		}

		//HlmsgHTMLtype = "text/html\n\r\n";					// Type for HTML file

		//if (BPft.CharArraytoStringcompare(BPft.Paramdata(1),"html",0) == 0)
		//{
		//	Hlmsgtype = HlmsgHTMLtype;
		//}


		Finalmessagelength = 0;							// Reset final message length

		//std::ifstream is (BP.Paramdata(1), std::ifstream::binary);  // Set up link to filename passed in

		std::ifstream is (WEBfile, std::ifstream::binary);  // Set up link to filename passed in


			if ((is)&& (File_typecode == 0))
				{													// get length of file:
				    is.seekg (0, is.end);
				    int length = is.tellg();
				    is.seekg (0, is.beg);

				    if (length > WEBSERVERBUFFERSIZE)
				    {
				    	File_errorcode = 2;						// file is too  large for server buffer
				    	std::cout<<"\nLine 101 WebBuildHTML - file too large for server buffer\n\n";
				    }
				    else
				    {

				    	File_toolarge = length;
				    	File_errorcode = 0;   					// file is OK to process
				    }
				}
			else
				{

					if (File_typecode != 4)						// specified file is not found
					{
						File_errorcode = 1;                         // file not found
					}

				}

			if (displayflag > 0)
			{
				std::cout <<"\n\nWebBuildHTMLpage File errorcode " << File_errorcode << " typecode:" << File_typecode <<"--";
			}

			if ((is) && (File_errorcode == 0))					// Retrieve the file details
				{													// get length of file:

				    is.seekg (0, is.end);
				    int length = is.tellg();
				    is.seekg (0, is.beg);

				    Hlmsglenstr = std::to_string(length);					// Store the length of the file as a string

				    if (displayflag == 2)
				    {
				    	std::cout << "\n\nLength of " << Hlmsglenstr;
				    }

				    Line = Hline1 + Hline2 + Hline3 + Hlmsglenstr + Hline4 + Hlmsgtype; // Start to build output line as a string

				    Hlinelen = Line.length();

				    if (displayflag > 0)
				    	    {
				    		std::cout << "\n\nHeader line length " << Hlinelen << " text is ->" << Line;
				    	    }

				    char * buffer = new char [length];							// create a buffer to hold the entire file

				    if (displayflag == 2)
				    	    {
				    		std::cout << "\n\nReading " << length << " characters from file... ";
				    	    }

				    is.read (buffer,length);									// read data as a block:

				    int msgcc;													// counter for messages in the output buffer

				    char* bufferptr  = &buffer[0];								// Create a pointer to the buffer holding the file

				    msgcc = 0;

				    if (is)
				    {
				    	// move header to output buffer

				    	std::strcpy(Linechars, Line.c_str()); 					// copy Line string to a char array

				    	if (displayflag == 2)
				    	{
				    		std::cout << "\nLinechars array " << Linechars;
				    	}

				    	char* Linecharsptr = &Linechars[0];				// Setup pointer to local output buffer

				    	for (int cc = 0;cc <= Hlinelen;cc++)
				    		{
				    		 *(msgptr + msgcc) = *(Linecharsptr + cc);				// copy the Line with the header data to the Output buffer
				    		  msgcc++;
				    		}

				    	if (displayflag > 0)
				    		{
				    		std::cout << "\n\nFile contents after header  \n" << msgptr;
				    		}

				    	msgcc--;													// move the counter back one place

				    	// move contents of the file to output buffer

				    	for (int cc = 0;cc <= length;cc++)
				    		{
				    		*(msgptr + msgcc) = *(bufferptr + cc);				// copy the buffer with the file data to the Output buffer
				    		  msgcc++;
				    		}

				    		*(msgptr + msgcc) = '\0';

				    		Finalmessagelength = msgcc;
				    }

				    else
				    {
				      std::cout << "error: only " << is.gcount() << " could be read";
				    }

				    is.close();													// close the file read

				    // ...buffer contains the entire file...

				    if (displayflag > 0)
				    {
				    	std::cout << "\n\nWebBuild Final Output File contents are #167 : \n" << msgptr;
				    }

				    delete[] buffer;

				  }

			delete BP;

			delete BPft;

	return Finalmessagelength;

}
