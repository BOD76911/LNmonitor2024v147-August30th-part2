/*
 * WebBuildHTMLpage.h
 *
 *  Created on: Aug 6, 2018
 *      Author: BrianRoot
 */

#ifndef WEBBUILDHTMLPAGE_H_
#define WEBBUILDHTMLPAGE_H_

#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "Bufferprocessing.h"
#include "Clocktime.h"


class WebBuildHTMLpage {
public:
	WebBuildHTMLpage();
	virtual ~WebBuildHTMLpage();

	int Pageprocess(int& conn, char& message, char* filename, int displayflag);

	int GetFinalmessagelength();

private:

		std::string Hline1 = "\HTTP/1.1 200 OK\n";
		std::string Hline2 = "Server: LNTrainmanager Web Server\n";
		std::string Hline3 = "Content-length: ";
		std::string Hline4 = "\nContent-type:";

		int Hlinelen;

		char Linechars[WEBSERVERBUFFERSIZE];			// Create local output character buffer

		std::string Line;								// Create string to hold the header details

		int File_errorcode;

		int File_typecode;

		int File_toolarge;

		std::string File_toolargestr;

		int Hlinemsglen;

		int Finalmessagelength;

		std::string Hlmsglenstr;

		std::string HlmsgHTMLtype = "text/html\n\r\n";	;
		std::string HlmsgCSStype = "text/css\n\r\n";
		std::string HlmsgJStype = "text/js\n\r\n";
		std::string HlmsgJPGtype = "image/jpg\n\r\n";
		std::string HlmsgTXTtype = "text/plain\n\r\n";

		std::string Hlmsgtype;

		std::string Filemissingmsgpt1 = "File ";
		std::string Filemissingmsgpt2 = " ";
		std::string Filemissingmsgpt3 = " is not found ";

};

#endif /* WEBBUILDHTMLPAGE_H_ */
