/*
 * Webmessagereply.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: BrianRoot
 */

#include "Webmessagereply.h"
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "Bufferprocessing.h"
#include "Clocktime.h"
#include "LNwebmonNGfileload.h"
#include "LNwebconfig.h"
#include "WebBuildHTMLpage.h"
#include "LNRteCMDlookuptable.h"

Webmessagereply::Webmessagereply() {
	// TODO Auto-generated constructor stub

}

Webmessagereply::~Webmessagereply() {
	// TODO Auto-generated destructor stub
}

int Webmessagereply::GetFinalmessagelength()
{
	return Finalmessagelength;
}

void Webmessagereply::Processmessage(int& conn, char& message, char* filename, int displayflag, LNwebLsectionsLdevices* LNLSLD,MsgTimestamp* MSGptr, \
		LNwebmonSimulator* LNSIM,LNweblocorouteprocess* LNLRprocess,LNdisplayconfig* LNDC, LNRFidsensorprocessing* LNRFIDSP,\
		LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,LNRteDevicereferencedata* LNRteDRfd)
{
		char* msgptr = &message;						// Setup the reference pointer to external output buffer

		int Routerecordcount = 0;

		int Totalrecordcount = 0;

		int Putcommandflag   = 0;

		Bufferprocessing* BP = new Bufferprocessing();	// Create Buffer processing object

		Bufferprocessing* BPft = new Bufferprocessing();

		Totalrecordcount = BP->Fileparse(filename,'W',0);					// Parse the message

		BPft->Fileparse(BP->Paramdata(0),'/',0);

		//std::cout<<"\nWMR 50 PUT message part 0:"<<BPft->Paramdata(0)<<" part 1:"<<BPft->Paramdata(1)<<" part 2:"<<BPft->Paramdata(2);
		//printf("\nline 51\n\n  ");

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"SCHEDCMD-",0) == 0)
		{
					/*************** Start of Scheduler command processing processing *******************/

			Bufferprocessing* BPftr = new Bufferprocessing();

			BPftr->Fileparse(BPft->Paramdata(1),'-',0);

			std::string Schedbuffercommandstr = BPftr->Paramdata(1);

			int Schbuffercmd = std::stoi(Schedbuffercommandstr);

			if (Schbuffercmd == 0)
			{
				LNSchedExecutor->DisableSchedulerexecution(0);
			}

			if (Schbuffercmd == 1)
			{
				LNSchedExecutor->EnableSchedulerexecution(0);
			}

			if (Schbuffercmd == 2)
			{
				LNSchedExecutor->Resettickcounter();
			}
		}
		
		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"CMDMODE-",0) == 0)
		{
									/*************** Start of PWM section command processing processing *******************/

				//printf("\n88 Webmessagereply - CMDMODE command received ");
				//printf("\n");

				Bufferprocessing* BPftr = new Bufferprocessing();		// parse the input command

				BPftr->Fileparse(BPft->Paramdata(1),'-',0);

				int PWMmode;

				std::string PWMcmdmode = BPftr->Paramdata(1);

				PWMmode = std::stoi(PWMcmdmode);

				LNLSLD->SwitchPWMsectionsmode(PWMmode);

				delete BPftr;
		}
		

		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"CMDDCM-",0) == 0)
		{
							/*************** Start of PWM section command processing processing *******************/

			//printf("\n88 Webmessagereply - PWM command received ");
			//printf("\n");

			Bufferprocessing* BPftr = new Bufferprocessing();		// parse the input command

			BPftr->Fileparse(BPft->Paramdata(1),'-',0);

			int Command;

			int Section;

			int Speed;

			int Direction;

			std::string PWMcmdparts = BPftr->Paramdata(1);

			Command = std::stoi(PWMcmdparts);

			if (Command == 3)
			{
				//printf("\n109 - clear all PWM sections ");
				//printf("\n");

				LNLSLD->InitializePWMsections();
			}
			else
			{
				if (Command == 1)
				{
					PWMcmdparts = BPftr->Paramdata(2);

					Section = std::stoi(PWMcmdparts);

					PWMcmdparts = BPftr->Paramdata(3);

					Speed = std::stoi(PWMcmdparts);

					PWMcmdparts = BPftr->Paramdata(4);

					Direction = std::stoi(PWMcmdparts);

					//printf("\n128 PWM command Section:%03d Speed: %03d Direction %01d",Section,Speed,Direction);

					LNLSLD->SetPWMsectiondirectionandspeed(Section, Speed, Direction);
				}
				else
				{
					std::cout<<"\nPWM Command not recgnized";
					std::cout<<BPft->Paramdata(1);
					std::cout<<"\n";
				}
			}


			delete BPftr;
		}



		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"PICMD-",0) == 0)
		{
			/*************** Start of physical input buffer process processing *******************/

			//std::cout<<"\n\nPI command : "<<BPft->Paramdata(1)<<"\n";

			Bufferprocessing* BPftr = new Bufferprocessing();

			BPftr->Fileparse(BPft->Paramdata(1),'-',0);

			std::string PIbuffercommandstr = BPftr->Paramdata(2);

			int PIbuffercmd = std::stoi(PIbuffercommandstr);

			Putcommandflag = 1;

			std::cout<<"\n\nPI command : "<<PIbuffercommandstr<<"\n";

			if (PIbuffercmd == 1)
			{
				Sectioncounterset(0,0);
			}

			if (PIbuffercmd == 2)
			{
				USBbytecounterset(0);

				USBbytesclear();
			}

			delete BPftr;
		}



		if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"LRD-",0) == 0)
		{
		/*************** Start of LRD- processing ****** Loco and Route details download ************************************/

		int Loconum, Slotcode,DCCcode,Dirt,Rte,Rtestep,Rtesubstep,PS,CS,NS,CmD,CmDp1,CmDp2,CmDp3,CmDp4,Simcode,Enable,Journeycode,Traincode;

		//std::cout << "\nwebmessagereply 90 :" << filename<< "--\n";  // Display file name

		BPft->Fileparse(BP->Paramdata(1),'+',0);		// extract details. loco - Locoslotcode - Rtedirection - LocoDCCdatacode Route number

		//Routerecordcount = BPft2->Fileparse(BP->Paramdata(3),'+',1);		// extract route record details

		std::string Loconumberstr = BPft->Paramdata(1);
		std::string Locoslotcodestr = BPft->Paramdata(2);
		std::string LocoRtedirectionstr = BPft->Paramdata(3);
		std::string LocoDCCstr = BPft->Paramdata(4);
		std::string LocoRtestr = BPft->Paramdata(5);
		std::string LocoEnablestr = BPft->Paramdata(6);
		std::string LSimcodestr = BPft->Paramdata(7);
		std::string LocoJourneycode = BPft->Paramdata(8);
		std::string LocoTraincode = BPft->Paramdata(9);

		std::string Rstepstr;
		std::string Rsubstepstr;
		std::string PSstr;
		std::string CSstr;
		std::string NSstr;
		std::string Cmdstr;
		std::string Cmdparam1str;
		std::string Cmdparam2str;
		std::string Cmdparam3str;
		std::string Cmdparam4str;


		// SaveLocojourneydata(int Locoindx, int Rtenum,int Stepnum, \
		int Substepnum,int PS, int CS,int NS, int speed, \
		int direction, int Signalhold,int Enabled, \
		int Planspeed,int Cmdinprogress,int Journey,\
		int Journeystatus, int Stopdevice, int Traincode,int Slotcode);

		Loconum = std::stoi(Loconumberstr);
		Rte 	= std::stoi(LocoRtestr);
		DCCcode = std::stoi(LocoDCCstr);
		Slotcode = std::stoi(Locoslotcodestr);
		Simcode  = std::stoi(LSimcodestr);
		Enable   = std::stoi(LocoEnablestr);
		Journeycode = std::stoi(LocoJourneycode);
		Traincode = std::stoi(LocoTraincode);

		LNLRprocess->SaveLocojourneydata(Loconum,Rte,0,0,0,0,0,0,0,0,Enable,0,0,Journeycode,0,0,DCCcode,Slotcode,Simcode,Traincode,0,0);

		std::cout<<"\nDownloaded details -> Loco:"<<Loconumberstr<<" Rte:"<< LocoRtestr<<\
				" Slot:"<<Locoslotcodestr<<" Rtedir:"<<LocoRtedirectionstr<<" DCC:"<<LocoDCCstr<<" Enableflg:"<<Enable<<\
				" Simcode:"<<Simcode<<" Journeycode:"<<Journeycode<<" Traincode:"<<Traincode;

		//PRHeapResbuff = new char[HEAPDISPLAYBUFFERLENGTH];

		//bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

		//LNLRprocess->Locodatarecordsdisplay(0,PRHeapResbuff);

		//std::cout<<"\nLoco details stored -->"<<PRHeapResbuff;

		// Now extract the route lines

		Routerecordcount = Totalrecordcount - 3;

		char RteCmdmessage[15] = "LRD CmD" ;

		std::cout<<"\n"<< Routerecordcount <<" route records to process";

		int rrtpc = 0;

		//BPft->Fileparse(BP->Paramdata(3),'+',1);


		for (rrtpc = 3;rrtpc < (Routerecordcount + 3);rrtpc++)
		{

			BPft->Fileparse(BP->Paramdata(rrtpc),'+',0);

			Rstepstr 		= BPft->Paramdata(1);
			Rsubstepstr 	= BPft->Paramdata(2);
			PSstr 			= BPft->Paramdata(3);
			CSstr 			= BPft->Paramdata(4);
			NSstr 			= BPft->Paramdata(5);
			Cmdstr 			= BPft->Paramdata(6);
			Cmdparam1str 	= BPft->Paramdata(7);
			Cmdparam2str 	= BPft->Paramdata(8);
			Cmdparam3str 	= BPft->Paramdata(9);
			Cmdparam4str 	= BPft->Paramdata(10);

			Rtestep 		= std::stoi(Rstepstr);
			Rtesubstep		= std::stoi(Rsubstepstr);
			PS				= std::stoi(PSstr);
			CS				= std::stoi(CSstr);
			NS				= std::stoi(NSstr);
			CmD				= std::stoi(Cmdstr);
			CmDp1			= std::stoi(Cmdparam1str);
			CmDp2			= std::stoi(Cmdparam2str);
			CmDp3			= std::stoi(Cmdparam3str);
			CmDp4			= std::stoi(Cmdparam4str);

			//std::cout<<"\nln 182 Rte record "<< Rte <<"-"<<Rtestep<<"-"<<Rtesubstep<<"-"<<PS<<"-"<<CS<<"-"<<NS<<"-Cmd-"<<CmD<<"-"<<CmDp1<<"-"<<CmDp2<<"-"<<CmDp3<<"-"<<CmDp4<<"-";

			/*
			 SaveRouteline(int Rte, int Rtestep, int Rtesubstep,int RtePS,int RteCS, int RteNS, int Rtecmd, int Rtecmdparam1,int Rtecmdparam2, int Rtecmdparam3,\
			int Rtecmdparam4,char *cmd, int dispflag)

			 */

			LNLRprocess->SaveRouteline(Rte,Rtestep,Rtesubstep,PS,CS,NS,CmD,CmDp1,CmDp2,CmDp3,CmDp4,&RteCmdmessage[0],0);			

			Putcommandflag = 2;
		}
		
		if (Enable == 1)
		{
			std::cout<<"\nLoco run enabled ";
			LNLRprocess->LNsectionCrossingprocessing(Loconum,1);
		}
		else
		{
			std::cout<<"\nLoco run DISABLED ";
		}

		/*************** end of LRD- processing ******************************************/
	}


	if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"LRR-",0) == 0)
	{
		/*************** Start of  LRR- processing **** loco reset **************************************/

		int Loconum;		

		BPft->Fileparse(BP->Paramdata(1),'+',0);		// extract details, loco number 		

		std::string Loconumberstr = BPft->Paramdata(1);
		
		Loconum = std::stoi(Loconumberstr);
		
		std::cout<<"\nReset command for loco "<< Loconum <<"\n";
		
		LNLRprocess->LocodatarecordsInit(Loconum);
		
	}

	if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"RFM-",0) == 0)
		{
			/*************** Start of  RFM - RFid message processing for input of car location **************************************/

			printf("\nWMR 239 RFid message %s\n",filename);

			// RFM-%02d-%03d-%s-%s-.dat\n",Detectornumber,Messagecounter,Timestamp,SerialCharinputbuffer);

			LNRFIDSP->Processcarmessage(filename, LNRteDRfd);
		}




		File_errorcode = 0;

		File_typecode = 0;

		std::string Missingmsgback = "NULL";

		Hlmsgtype = HlmsgTXTtype;

		File_errorcode = 4;

		File_typecode = 4;

		char Linechars[WEBSERVERBUFFERSIZE];			// Create local output character buffer

		std::string Line;								// Create string to hold the header details

		Finalmessagelength = 0;							// Reset final message length

		char CTDatetime[DATETIMESTRING] = "Not set ";

		char* CTDatetimeptr = &CTDatetime[0];

		Clocktime CT;

		CT.Get_time(*CTDatetimeptr);

		//std::cout<<"\n\nPUT message processing completed  ";

		if (Putcommandflag == 1)
		{
			Filemissingmsgpt2 = std::string(CTDatetime) + "; Buffer reset command completed";
		}

		if (Putcommandflag == 2)
		{
			Filemissingmsgpt2 = std::string(CTDatetime) + "; Loco Route download completed";
		}

		if (Putcommandflag == 3)
		{
			Filemissingmsgpt2 = std::string(CTDatetime) + "; TT command processed ";
		}

		Missingmsgback = Filemissingmsgpt2;

		Hlinemsglen = Missingmsgback.length();

		Hlmsglenstr = std::to_string(Hlinemsglen);

		Line = Hline1 + Hline2 + Hline3 + Hlmsglenstr + Hline4 + Hlmsgtype; // Start to build output line as a string

		Hlinelen = Line.length();

		int msgcc;														  // counter for messages in the output buffer

		msgcc = 0;

		// move header to output buffer

	    std::strcpy(Linechars, Line.c_str()); 							// copy Line string to a char array

		if (displayflag == 2)
			{
				std::cout << "\nLinechars array " << Linechars;
			}

				char* Linecharsptr = &Linechars[0];							// Setup pointer to local output buffer

				for (int cc = 0;cc <= Hlinelen;cc++)
					{
					  *(msgptr + msgcc) = *(Linecharsptr + cc);				// copy the Line with the header data to the Output buffer
					  msgcc++;
					}

					if (displayflag > 0)
					  {
					  	std::cout << "\n\nFile contents after header #355 \n" << msgptr;
					  }

					  msgcc--;													// move the counter back one place

					 // move contents of the missing message string to output buffer

					 std::strcpy(Linechars, Missingmsgback.c_str()); 				// copy Line string to a char array


					 for (int cc = 0;cc <= Hlinemsglen;cc++)
					  {
						 *(msgptr + msgcc) = *(Linecharsptr + cc);				// copy the buffer with the file data to the Output buffer

					  	msgcc++;
					   }

					 	 *(msgptr + msgcc) = '\0';

					  	Finalmessagelength = msgcc;
				//}

		delete BP;

		delete BPft;

		return;

}

void Webmessagereply::Buildmessage(int& conn, char& message, char* filename, int displayflag, LNwebLsectionsLdevices* LNLSLD,MsgTimestamp* MSGptr,\
		LNwebmonSimulator* LNSIM,LNSimRouteMaster* LNSimRteMaster,LNweblocorouteprocess* LNLRprocess, LNdisplayconfig* LNDC,LNSchedulerExecutor* LNSchedExecutor,LNRouteSchedMaster* LNRteSchedMast,\
		LNSchedulerAlljourneys* LNSchedAlljourneys,LNLocoRoutemovementMasterNG* LNRtimeallrecsNG, LNlocoAlldata* LNlocodatatables)
{
	// Function process PUT commands received by LNwebserver. The routine builds the output for the return to the sending client.

	char* msgptr = &message;						// Setup the reference pointer to external output buffer

	//char* messageptr = &message;

	char Datareturnchars[WEBSERVERBUFFERSIZE];			// Create local output character buffer

	int* connptr = &conn;

	Bufferprocessing* BP = new Bufferprocessing();							// Create Buffer processing object

	BP->Fileparse(filename,'/',0);					// Parse the message

	//std::cout << "\nWMR 2631 Param line 2631 :" << BP->Paramdata(1)<< "--";  // Display file name

	Bufferprocessing* BPft = new Bufferprocessing();

	BPft->Fileparse(BP->Paramdata(1),'.',0);

	File_processingcode	= 0;

	File_typecode 		= 0;

	//----------------------------------------------------------------------------------------//
	//
	//Processing for HTML, CSS,JS and JPG files is processed in WebBuildHTMLpage class
	//
	//----------------------------------------------------------------------------------------//

	if ((BPft->CharArraytoStringcompare(BPft->Paramdata(1),"html",0) == 0) || (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"css",0) == 0) \
			|| (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"js",0) == 0) || (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"jpg",0) == 0))
	{
		WebBuildHTMLpage WBHTML;

		WBHTML.Pageprocess(*connptr, *msgptr,filename, 0);

		Finalmessagelength = WBHTML.GetFinalmessagelength();

		delete BPft;

		delete BP;

		return;
	}

	//-------------------------------------------------------------------------------------------//
	//
	// Check to see if file type is a  ' .dat ' file which may be from an AJAX call to the server.
	//
	//-------------------------------------------------------------------------------------------//


	if (BPft->CharArraytoStringcompare(BPft->Paramdata(1),"dat",0) == 0)
	{
		Hlmsgtype = HlmsgTXTtype;

		File_processingcode = 4;

		File_typecode = 4;					// means it is a data file from another program or one loaded by the scheduler
	}

	 // ------- Processing for non HTML file requests ------------------------------------------
     // Filename
	 // --------
	 //
	 // PSset = processing physical sections - done
	 // LSset = processing logical sections - done
	 // LDset = logical device processing - done
	 // LRECMD = Loco record command - done
	 // SLCset = set slot speed and direction - done
	 // DPset = Device setup processing - done
	 // DPCset  = Device counter processing
	 // SWset = switch processing - done
	 // SRQ = Stop request processing
	 // SIMCMD = Simulator command processing - done
	 // DSsreq = Set STOP processing - done
	 // displayDCFdata = display DCF data - done
	 // servertime  = Base dataset for system monitoring - done
	 // Simdatabuild = Simulator description data - done
	 // displayGUIdynamic = data to drive GUI display- dynamically updated each time it is called  - done
	 // displayGUIstatic = data to be retrieved to display GUI static data elements e.g LS configuration - done
	 // Schedcmd = command for the Scheduler that controls the Journeys
	 // SchedJnyRtedetail = command to extract Schedule, Journey and Route details
	 // GUIRtedata  = command to display Route data on Browser
	 // GUIJnydata  = command to display Journey (Loco & Route) data on Browser
	 // PTGUIclk = command to process point switching from the GUI
	 // TRNsect  = command to process Train sections
	 // RCGsect = command to process Railcar section processing

	 Bufferprocessing* BPft1 = new Bufferprocessing();

	 BPft1->Fileparse(BPft->Paramdata(0),'-',0);

	 //std::cout<<"\nWMR2697 processing non HTML request "<<BPft1->Paramdata(0)<<"  ";

	 //--------------------------------------------------------------------------------------------------------------------------------
	 //
	 //           Display displayconfigurationdata (DCF) data
	 //
	 //--------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"displayDCFdata",0) == 0)
	 {
	                          	  // return the display configuration details

		 char CTDatetime[DATETIMESTRING] = "Not set ";

	     char* CTDatetimeptr = &CTDatetime[0];

	     Clocktime CT;

	     CT.Get_time(*CTDatetimeptr);

	     Browser_response_datastream = std::string(CTDatetime) + ";+";

	     bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);								// clear buffer to store output file

	     LNDC->GetGUIdata(PRHeapResbuff,LNLSLD,0,0,0);								// fetch GUI data

	     Browser_response_datastream.append(PRHeapResbuff);

	     bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);								// clear buffer to store output file

	     Browser_response_datastream.append("#");

	     LNSchedExecutor->BrowserSchedulerExecutionresponses(PRHeapResbuff);		//  Transmits Scheduler execution status for each Executor

	     Browser_response_datastream.append(PRHeapResbuff);

	     Browser_response_datastream.append(";");

	     Browser_response_datastream.append("#");

	     bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	     LNLSLD->DisplayDcodes(0,PRHeapResbuff);									// fetch detector data

	     Browser_response_datastream.append(PRHeapResbuff);

	     Browser_response_datastream.append(";");

	     Browser_response_datastream.append("#");

	     bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	     LNLSLD->DetectorTableUpdate(PRHeapResbuff);								// fetch status of detector device triggers

	     Browser_response_datastream.append(PRHeapResbuff);

	     Browser_response_datastream.append(";");

	     Browser_response_datastream.append("END;");
	   }

	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                 Processing for Point switch commands sent from the GUI
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

		 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"PTGUIclk",0) == 0)
		  {
			  char CTDatetime[DATETIMESTRING] = "Not set ";

		      char* CTDatetimeptr = &CTDatetime[0];

		      Clocktime CT;

		      CT.Get_time(*CTDatetimeptr);

		      Browser_response_datastream = std::string(CTDatetime) + ";+";

		      bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);								// clear buffer to store output fi

		      // extract the physical section details

		      std::string PointbuttonGUIref = BPft1->Paramdata(1);

		      int PointbuttonGUIrefint = std::stoi(PointbuttonGUIref);			// extract the point GUI reference number

			 //std::cout<<"\n568 Point GUI reference:"<<PointbuttonGUIrefint;

			 int PTrefdata[2];

			 int *PTrefdataptr = &PTrefdata[0];

			 int PTrefRC = LNDC->Getpointconfigdata(PTrefdataptr, PointbuttonGUIrefint);

			 //std::cout<<"\nPt number:"<<*PTrefdataptr<<" O/C:"<<*(PTrefdataptr+1);

			 if (PTrefRC == 0)
			 {
				 LNLSLD->Actionswitch(*PTrefdataptr,11,*(PTrefdataptr +1));

				 LNLSLD->RefreshLastupdatedetails(1);

				 LNLSLD->LogicalSectionTcodeRefresh();

				 LNLSLD->LogicalDetectorupdateDetectorstatus();

				 LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover
			 }

			 Browser_response_datastream.append(";");

			 Browser_response_datastream.append("END;");
		  }


		 //---------------------------------------------------------------------------------------------------------------------------------
		 //
		 //                 Processing for Train section commands sent from the track GUI or the control GUI - for Trains or Railcar groups
		 //
		 //---------------------------------------------------------------------------------------------------------------------------------

		 		 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"TRNsect",0) == 0)
		 		  {
		 			  char CTDatetime[DATETIMESTRING] = "Not set ";

		 		      char* CTDatetimeptr = &CTDatetime[0];

		 		      Clocktime CT;

		 		      CT.Get_time(*CTDatetimeptr);

		 		      Browser_response_datastream = std::string(CTDatetime) + ";+";

		 		      bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);								// clear buffer to store output fi

		 		      // extract the train section details

		 		      std::string Trainprocessingflag = BPft1->Paramdata(1);

		 		      std::string Trainlocation = BPft1->Paramdata(2);

		 		      std::string TrainID = BPft1->Paramdata(3);

		 		     // std::cout<<"\n629 Train section flag:"<<Trainprocessingflag<<" section:"<<Trainlocation<<" ID:"<<TrainID;

		 		     // std::cout<<"\n";

		 		      int Processingflag = std::stoi(Trainprocessingflag);

		 		      int Trainsection = std::stoi(Trainlocation);

		 		      int TrainNumber = std::stoi(TrainID);

		 		      if (Processingflag == 1)  // store train code in Train section
		 		      {
		 		    	  LNLSLD->TSupdateSection(Trainsection, 0, TrainNumber,1);
		 		      }

		 		     if (Processingflag == 2)  // store train code in Train section
		 		     {
		 		     	LNLSLD->TSupdateSection(Trainsection, 0, 0,2);
		 		     }

		 		     if (Processingflag == 3)  // store train code in Train section
		 		     {
		 		    	LNLSLD->TSallsectionsclear();
		 		     }

		 		     LNLSLD->RefreshLastupdatedetails(1);

		 		     LNLSLD->LogicalSectionTcodeRefresh();

		 		     LNLSLD->LogicalDetectorupdateDetectorstatus();

		 		     LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover



		 			  Browser_response_datastream.append(";");

		 			  Browser_response_datastream.append("END;");
		 		  }

		 		 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"RCGsect",0) == 0)
		 		 {
		 			char CTDatetime[DATETIMESTRING] = "Not set ";

		 			char* CTDatetimeptr = &CTDatetime[0];

		 			Clocktime CT;

		 			CT.Get_time(*CTDatetimeptr);

		 			Browser_response_datastream = std::string(CTDatetime) + ";+";

		 			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);								// clear buffer to store output fi

		 			// extract the Railcar section details

		 			std::string Railcarprocessingflag = BPft1->Paramdata(1);

		 			std::string Railcargrouplocation = BPft1->Paramdata(2);

		 			std::string RailcargroupID = BPft1->Paramdata(3);

		 			// std::cout<<"\n629 Train section flag:"<<Trainprocessingflag<<" section:"<<Trainlocation<<" ID:"<<TrainID;

		 			// std::cout<<"\n";

		 			 int Processingflag = std::stoi(Railcarprocessingflag);

		 			 int Railcargroupsection = std::stoi(Railcargrouplocation);

		 			 int RailcargroupNumber = std::stoi(RailcargroupID);

		 			 if (Processingflag == 1)  // store Railcargroup code in Train section
		 			 {
		 				 LNLSLD->SetRailcarInsection(Railcargroupsection,RailcargroupNumber);
		 			 }

		 			if (Processingflag == 2)  // Reset Railcar group Train section
		 			{
		 				 LNLSLD->ClearRailcarGroupInsection(Railcargroupsection);
		 			}

		 			if (Processingflag == 3)  // Clear all Railcar groups in Train section
		 			{
		 				 LNLSLD->TSallRCGsectionsclear();
		 			}

		 			if (Processingflag == 4)  // Couple Railcargroups in Train section to Loco
		 			{
		 				 LNLSLD->TSallRCGlococouple(Railcargroupsection);
		 			}

		 			if (Processingflag == 5)  // Couple Railcargroups in Train section to Loco
		 			{
		 				LNLSLD->TSallRCGlocoUncouple(Railcargroupsection);
		 			}

		 			LNLSLD->RefreshLastupdatedetails(1);

		 			LNLSLD->LogicalSectionTcodeRefresh();

		 			LNLSLD->LogicalDetectorupdateDetectorstatus();

		 			LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover

		 			Browser_response_datastream.append(";");

		 			Browser_response_datastream.append("END;");
		 		 }


	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                   Processing for Journey data display on the GUI
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

		 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"JnyRUN",0) == 0)
		 {
			 std::string Jnynumberstr = BPft1->Paramdata(1);

			 int Jnynumber = std::stoi(Jnynumberstr);			// extract the point GUI reference number

			 std::cout<<"\n614 Journey run request:"<<Jnynumber;

			// Browser_response_datastream = "Jny RUN:" + Jnynumberstr + " started";

			 int LRMmatchedflagNG;

			 int LNTRexecuteNGindex;

			 LNTRexecuteNGindex = LNRtimeallrecsNG->GetRuntimerecord(LRMmatchedflagNG, Jnynumber);

			 // LNTRexecute = LNRtimeallrecs->GetRuntimerecord(LRMmatchedflag,SelectedTRM);

			 char TestOutboundmessage[OUTPUTMSGBUFFERSIZE];

			 bzero(TestOutboundmessage,OUTPUTMSGBUFFERSIZE);

			 char* TestOutboundmessageptr = &TestOutboundmessage[0];

			 if (LRMmatchedflagNG > 0)
			 {
			 	 std::cout<<"\n\nWebmessagereply 691 -> Train Route Movement:" + Jnynumberstr + " record found\n\n";

			 	 int Enable = 1;

			  	 int Locosimcheck;

			  	 if (LNRtimeallrecsNG->GetSimulationflag(LNTRexecuteNGindex) > 0)
			  	 	{
			  			PSectionset(LNRtimeallrecsNG->GetSLSpsection(LNTRexecuteNGindex),1);

			  			LNLSLD->RefreshLastupdatedetails(1);

			  			LNLSLD->LogicalSectionTcodeRefresh();

			  			LNLSLD->LogicalDetectorupdateDetectorstatus();

			  			LNLRprocess->LNlocosectionrefresh();
			  		}

			  	int slotcode = DCCslotdatafetchcall(LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex));

			  	if (slotcode == 0)
			  	{
			  		std::cout<<"\n657 - no DCC slot found for DCC code "<<LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex);
			  	}
			  	else
			  	{
			  		std::cout<<"\nWebmessagereply 662 -> Starting route for loco "<< LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex) \
			  		<<" DCC code "<<LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex) \
			  		<<" Slot number "<< slotcode<<"\n\n";
			  	}

			  	LNLRprocess->SaveLocojourneydata(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),\
			  			 												LNRtimeallrecsNG->GetRouteNum(LNTRexecuteNGindex), \
			  			 												0,0,0,0,0,0,0,0,Enable,0,0,\
			  															LNRtimeallrecsNG->GetJourneyNumber(LNTRexecuteNGindex),0,0,\
			  			 												LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex), \
			  			 												LNRtimeallrecsNG->GetSlotNumber(LNTRexecuteNGindex),\
			  			 												LNRtimeallrecsNG->GetSimulationflag(LNTRexecuteNGindex),\
			  															LNRtimeallrecsNG->GetTrainNumber(LNTRexecuteNGindex),LNRtimeallrecsNG->GetReversefacingflag(LNTRexecuteNGindex),\
																		LNRtimeallrecsNG->GetSectionlogicflag(LNTRexecuteNGindex));

			  	if (Enable == 1)
			  	{
			  		//std::cout<<"\n\n766 Console command to start Loco run enabled ";

			  		LNLRprocess->Setschedulerrunstatus(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),0);

			  		LNLRprocess->LNsectionCrossingprocessing(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),1);

			  		Locosimcheck = LNLRprocess->GetSimulatornumber(LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex));

			  		//std::cout<<"\n687 Webmessagereply - Loco run enabled - checking if simulator journey ";

			  		if (Locosimcheck > 0) // check to see if simulator should be started in run mode
			  			{
			  			 	std::cout<<"\n746 Webmessage Journey to be executed on simulator:"<<Locosimcheck;
			  			 	LNSIM->InitMultiSimrun(Locosimcheck,LNRtimeallrecsNG->GetRouteNum(LNTRexecuteNGindex),1,LNRtimeallrecsNG->GetLoconumberforRun(LNTRexecuteNGindex),LNRtimeallrecsNG->GetTrainNumber(LNTRexecuteNGindex), LNRtimeallrecsNG->GetDCCcodeNumber(LNTRexecuteNGindex), LNLSLD,LNLRprocess,LNSimRteMaster);
			  			}


			  		Browser_response_datastream = " Journey started";

			  		//char GUImsgstring[GUIMESSAGELINELENGTH];

			  		//bzero(GUImsgstring,GUIMESSAGELINELENGTH);

			  		//char* GUImsgstringptr = &GUImsgstring[0];

			  		//sprintf(GUImsgstring,"A-Journey %02d started ",LNRtimeallrecsNG->GetJourneyNumber(LNTRexecuteNGindex));

			  		//InsertGUItextlineMain(GUImsgstringptr);
			  	}
			  	else
			  	{
			  		std::cout<<"\nLoco run DISABLED ";
			  	}

			  }


		 }





	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                   Processing for Journey data display on the GUI
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"GUIJnydata",0) == 0)
	 {
		 // Build response data stream

		char CTDatetime[DATETIMESTRING] = "Not set ";

		char* CTDatetimeptr = &CTDatetime[0];

		Clocktime CT;

		CT.Get_time(*CTDatetimeptr);

		Browser_response_datastream = std::string(CTDatetime) + ";";

		bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

		LNRtimeallrecsNG->DisplayAllmovementMasters(PRHeapResbuff,1);

		Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

		//Browser_response_datastream.append(";");				// Add the delimiter.

		//Browser_response_datastream.append("END;");
	 }

	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                   Processing for Routedata display on the GUI
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"GUIRtedata",0) == 0)
	 {
		 // Build response data stream

		char CTDatetime[DATETIMESTRING] = "Not set ";

		char* CTDatetimeptr = &CTDatetime[0];

		Clocktime CT;

		CT.Get_time(*CTDatetimeptr);

		Browser_response_datastream = std::string(CTDatetime) + ";";

		bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

		LNRteCMDlookuptable* RteCmdtable = new LNRteCMDlookuptable();

		LNRteSchedMast->DisplayRoutelinedetailsarray(RteCmdtable,PRHeapResbuff,1);		// fetch Route details

		Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

		//Browser_response_datastream.append(";");				// Add the delimiter.

		//Browser_response_datastream.append("END;");
	 }

	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                   Processing for 'servertime' data request. This returns the base operating data on section details, loco details
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"servertime",0) == 0)
	 {
		// Build response data stream

			char CTDatetime[DATETIMESTRING] = "Not set ";

			char* CTDatetimeptr = &CTDatetime[0];

			Clocktime CT;

			CT.Get_time(*CTDatetimeptr);

			Browser_response_datastream = std::string(CTDatetime) + ";";

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			PSectiondatareturn(0, PRHeapResbuff);					// (1) extract Physical section details for all sections

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			Slotdatareturn(0, PRHeapResbuff);						// (2) extract Slot data details

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->DisplayTcodes(0,PRHeapResbuff);					// (3) extract LS traincode details data details

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->DisplayDcodes(0,PRHeapResbuff);					// (4) Extract the detector status information

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->GetPStimestampdata(PRHeapResbuff);    			// (5)Get Physical section last update timestamp details

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->GetPDtimestampdata(PRHeapResbuff);    			// (6) Get detector update timestamp data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			Pointstatusdatareturn(0, PRHeapResbuff);				// (7) extract Points status details

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->DisplayStopflagdata(PRHeapResbuff);		        // (8) extract stop flag data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->DetectorTableUpdate(PRHeapResbuff);				// (9) Extract detector data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLRprocess->Locodatarecordsdisplay(0,PRHeapResbuff);   // (10) Get Loco data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNSIM->DisplayMultiSimexecutorstatus(PRHeapResbuff);    // (11) Get simulator executor status data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLRprocess->GetLRprocessmonmsg(PRHeapResbuff);			// (12) get loco route processing message

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLRprocess->GetLRlocoprocessmonmsg(0,PRHeapResbuff);	// (13) get loco route processing message

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			GetSerialInterfacecntrs(PRHeapResbuff);					// (14) get serial interface counters

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			GetTCDbufferdata(PRHeapResbuff);						// (15) Get TCD buffer data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNSchedAlljourneys->BrowserDisplaySchedMast(0, PRHeapResbuff); // (16) Get Schedule Master data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLRprocess->GetJourneystatus(PRHeapResbuff);			// (17) Get Journey data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");	     		// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->TSGUIdisplayTrainsecstions(PRHeapResbuff);		// (18) get the Trainsection data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");	     		// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			GetBCMconnectordata(PRHeapResbuff);						// (19) Get BCM connector data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->GetallPWMsectionData(PRHeapResbuff);			// (20) Get PWM section data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

			LNLSLD->GetallcontrollerData(PRHeapResbuff);			// (21) Get PWM manual controller data

			Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

			Browser_response_datastream.append(";");				// Add the delimiter.

			Browser_response_datastream.append("ENDED;");
	 }

	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                 Processing for GUI data display which is updated dynamically
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"displayGUIdynamic",0) == 0)
	 {
	      // return the display configuration details

	      char CTDatetime[DATETIMESTRING] = "Not set ";

	      char* CTDatetimeptr = &CTDatetime[0];

	      Clocktime CT;

	      CT.Get_time(*CTDatetimeptr);

	      Browser_response_datastream = std::string(CTDatetime) + ";+";

	      bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);			// clear buffer to store output file

	      LNDC->GetGUIdata(PRHeapResbuff,LNLSLD,1,0,0);			// retrieve GUI data

	      Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

	      Browser_response_datastream.append(";*");				// Add the * delimiter to split the GUI data from the Sched/Jny/Rte data

	      bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	      LNSchedAlljourneys->BrowserDisplaySchedMast(0, PRHeapResbuff); // (16) Get Schedule Master data

	      Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

	      Browser_response_datastream.append(";*");				// Add the * delimiter to split the GUI data from the Sched/Jny/Rte data

	      bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	      //LNDC->GUIAllmsglinesget(PRHeapResbuff);

	      GUIAllmsglinesgetMain(PRHeapResbuff);

	      Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

	      Browser_response_datastream.append(">;");				// Add the delimiter.

	      Browser_response_datastream.append("END;");
	 }

	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 //                 Processing for retrieving static GUI data
	 //
	 //---------------------------------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"displayGUIstatic",0) == 0)
	 {
	        // return the display static configuration details

	        char CTDatetime[DATETIMESTRING] = "Not set ";

	        char* CTDatetimeptr = &CTDatetime[0];

	        Clocktime CT;

	        CT.Get_time(*CTDatetimeptr);

	        std::string GUIstaticparam1 = BPft1->Paramdata(1);  // extract the GUI static data parameter 1

	        std::string GUIstaticparam2 = BPft1->Paramdata(2);  // extract the GUI static data parameter 2

	        Browser_response_datastream = std::string(CTDatetime) + ";+";

	        bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);			// clear buffer to store output file

	        int GUIparam1 = std::stoi(GUIstaticparam1);

	        int GUIparam2 = std::stoi(GUIstaticparam2);

	        LNDC->GetGUIdata(PRHeapResbuff,LNLSLD,2,GUIparam1,GUIparam2);

	        Browser_response_datastream.append(PRHeapResbuff);

	        Browser_response_datastream.append(";");

	        Browser_response_datastream.append("END;");
	 }


	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 // --------------- Processing for Simdatabuild command. This command fetches the Simulator description data
	 //

	 if (BPft->CharArraytoStringcompare(BPft->Paramdata(0),"Simdatabuild",0) == 0)
	 {
	     char CTDatetime[DATETIMESTRING] = "Not set ";

	     char* CTDatetimeptr = &CTDatetime[0];

	     Clocktime CT;

	     CT.Get_time(*CTDatetimeptr);

	     Browser_response_datastream = std::string(CTDatetime) + ";";

	     bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	     LNSIM->DisplaySimulatordescriptions(0,PRHeapResbuff);

	     //std::cout<<"\nWMR 2877 response->"<<PRHeapResbuff;

	     Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

	     Browser_response_datastream.append(";");				// Add the delimiter.

	     Browser_response_datastream.append("END;");

	     //std::cout<<"\nWMR 2880 response->"<<Browser_response_datastream;

	  }


	 //---------------------------------------------------------------------------------------------------------------------------------
	 //
	 // --------------- Processing for PSset command. This command updates the Physical Section (PS) details
	 //

	 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"PSset",0) == 0)
	  {
		  char CTDatetime[DATETIMESTRING] = "Not set ";

	      char* CTDatetimeptr = &CTDatetime[0];

	      Clocktime CT;

	      CT.Get_time(*CTDatetimeptr);

	      // extract the physical section details

	      std::string PSsectionstr = BPft1->Paramdata(1);

	      std::string PSactionstr = BPft1->Paramdata(2);

	      int PSsection = std::stoi(PSsectionstr);			// section number

		  int PSaction = std::stoi(PSactionstr);			// action code

		  int rc;

		  SetTimestamp(PS_TIMESTAMP, 1,MSGptr);

		  if (PSaction == 1)
		  {
				rc = PSectionset(PSsection,1);

				Browser_response_datastream = std::string(CTDatetime) + "; Physical section " + PSsectionstr + " set";
		  }

		  if (PSaction == 0)
		  {
			   rc = PSectionset(PSsection,0);

			   Browser_response_datastream = std::string(CTDatetime) + "; Physical section " + PSsectionstr + " cleared";
		  }

	      if (PSaction == 3)
	      {
	          PSectionreset();

	          InitTCDbuffers();

	          Browser_response_datastream = std::string(CTDatetime) + "; All Physical sections cleared";
	      }
	      else
	      {
	           //Filemissingmsgpt2 = std::string(CTDatetime) + "; Physical section " + PSsectionstr + " updated";
	      }

	      LNLSLD->RefreshLastupdatedetails(1);

	      LNLSLD->LogicalSectionTcodeRefresh();

	      LNLSLD->LogicalDetectorupdateDetectorstatus();

	      LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover
	  }


	 //--------------------------------------------------------------------------------------------------------
	 //
	 //             Logical section command (LSset) processing
     //
	 //--------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"LSset",0) == 0)
	 {
	      char CTDatetime[DATETIMESTRING] = "Not set ";

	      char* CTDatetimeptr = &CTDatetime[0];

	      Clocktime CT;

	      CT.Get_time(*CTDatetimeptr);

	      std::string LSsectionstr = BPft1->Paramdata(1);    	// extract LS section number

	      std::string LSlococodestr = BPft1->Paramdata(2);		// extract loco code

	      std::string LSTrIDcodestr = BPft1->Paramdata(3);		// extract train code

	      std::string LSsetactionflgstr = BPft1->Paramdata(4);  // extract the coupling flag

	      int LSsectionnum  = std::stoi(LSsectionstr);			// convert section number to integer

	      int LSlococodenum  = std::stoi(LSlococodestr);		// convert loco number to integer

	      int LSsetactionflg = std::stoi(LSsetactionflgstr);	// convert couplong flag to integer

	      int LSTrIDcodenum = std::stoi(LSTrIDcodestr);			// convert Train code to integer

	     // std::cout<<"\n1567 Section loco code action msg - section:"<<LSsectionnum<<" lococode:"<<LSlococodenum<<" TrID:"<<LSTrIDcodenum<<" flg:"<<LSsetactionflg;

	      if (LSsetactionflg == 3)
	      {
	           LNLSLD->ClearTcodes();  // clears Tcode and TrainID fields
	           Browser_response_datastream = std::string(CTDatetime) + "; All LS sections cleared ";
	      }
	      else
	      {
	    	  if (LSTrIDcodenum == 0)
	          {
	             LNLSLD->UpdateTcode(LSsectionnum,LSlococodenum);    				// UpdateTcode(int sc, int Tcode)

	             LNlocodatatables->SetlococurrentLS_DCC(LSlococodenum, LSsectionnum);		// update loco all data table

	             LNLSLD->UpdateTrainID(LSsectionnum,LSTrIDcodenum,LSlococodenum,LSsetactionflg);	// LSTrIDcodenum
	          }
	          else // if Train ID greater than zero, do not update the Tcode field but do process the flag field
	          {
	             LNLSLD->UpdateTrainID(LSsectionnum,LSTrIDcodenum,LSlococodenum,LSsetactionflg);   //
	          }

	    	  Browser_response_datastream = std::string(CTDatetime) + "; LS section " + LSsectionstr + " update completed";
	     }



	     LNLSLD->LogicalSectionTcodeRefresh();	// run Tcoderefresh process
	 }

	 //-------------------------------------------------------------------------------------------------------
	 //
	 //         Logical device processing (LDset)
	 //
	 //-------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"LDset",0) == 0)
	  {
	      char CTDatetime[DATETIMESTRING] = "Not set ";

	      char* CTDatetimeptr = &CTDatetime[0];

	      Clocktime CT;

	      CT.Get_time(*CTDatetimeptr);

	      std::string LDdevicestr = BPft1->Paramdata(1);

	      std::string LDactionflgstr = BPft1->Paramdata(2);

	      int LDdevicenum = std::stoi(LDdevicestr);

	      int LDactionflg = std::stoi(LDactionflgstr);

	      if (LDactionflg == 3)
	      {
	          LNLSLD->ClearDcodes();
	      }
	      else
	      {
	            if (LDactionflg == 0)
	            {
	               LNLSLD->UpdateDcode(LDdevicenum,0);
	            }
	            else
	            {
	               LNLSLD->UpdateDcode(LDdevicenum,1);
	            }
	      }

	      LNLSLD->RefreshLastupdatedetails(2);

	      LNLSLD->LogicalDetectorupdateDetectorstatus();

	      Browser_response_datastream = std::string(CTDatetime) + "; Logical device processing completed";
	  }

	 //--------------------------------------------------------------------------------------------------------
	 //
	 //           Slot data processing SLCset
	 //
	 //--------------------------------------------------------------------------------------------------------


	  if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"SLCset",0) == 0)
	  {
       	 char CTDatetime[DATETIMESTRING] = "Not set ";

	     char* CTDatetimeptr = &CTDatetime[0];

	     Clocktime CT;

	     CT.Get_time(*CTDatetimeptr);

	     std::string SLCslotstr = BPft1->Paramdata(1);  // extract slot number, speed,direction and flag

	     std::string SLCslotspeedstr = BPft1->Paramdata(2);

	     std::string SLCslotdirectionstr = BPft1->Paramdata(3);

	     std::string SLCslotflagstr = BPft1->Paramdata(4);

	     int SLCslot = std::stoi(SLCslotstr);			// convert slot number, speed and direction to integers

	     int SLCslotspeed = std::stoi(SLCslotspeedstr);

	     int SLCslotdirection = std::stoi(SLCslotdirectionstr);

	     int SLCslotactionflag = std::stoi(SLCslotflagstr);

	     if (SLCslotactionflag == 3)
	     {
       		CAllslotsstop();

       		Browser_response_datastream = std::string(CTDatetime) + "; All DCC Slot data cleared ";
	            	 }
	            	 else
	            	 {
	            		 CSpeedcmdprocess(51,SLCslot, SLCslotspeed, SLCslotdirection);
	            		 Browser_response_datastream = std::string(CTDatetime) + "; DCC Slot " + SLCslotstr + " update completed";
	            	 }
	     }

	  //------------------------------------------------------------------------------------------------------
	  //
	  //               Loco LRE command processing
	  //
	  //------------------------------------------------------------------------------------------------------

	  /********************** LRE = processing Loco run commands *******************************************/

	  if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"LRECMD",0) == 0)
	  {
	       char CTDatetime[DATETIMESTRING] = "Not set ";

	       char* CTDatetimeptr = &CTDatetime[0];

	       char LREmessage[SHORTMESSAGELENGTH] = "Loco message not set";

	       Clocktime CT;

	       int replycode = 0;

	       CT.Get_time(*CTDatetimeptr);

	       std::string LRElocoflagstr = BPft1->Paramdata(1);

	       std::string LREcmdflagstr = BPft1->Paramdata(2);

	       int LRElocoflag = std::stoi(LRElocoflagstr);

	       int LREcmdflag = std::stoi(LREcmdflagstr);

	       bzero(LREmessage,SHORTMESSAGELENGTH);

	       replycode = LNLRprocess->Lococommandexecute(LRElocoflag,LREcmdflag,&LREmessage[0]);

	       if ((LREcmdflag == 1) && (replycode == 0))
	       {
	    	   Browser_response_datastream = std::string(CTDatetime) + "; Loco LRE RUN command executed " + LREmessage;
	       }

	       if ((LREcmdflag == 1) && (replycode > 0))
	       {
	    	   Browser_response_datastream = std::string(CTDatetime) + "; Loco LRE RUN error:=> " + LREmessage;
	       }

	       if ((LREcmdflag == 2) && (replycode == 0))
	       {
	    	   Browser_response_datastream = std::string(CTDatetime) + "; Loco LRE STOP command executed ";
	       }

	       if ((LREcmdflag == 3) && (replycode == 0))
	       {
	    	   Browser_response_datastream = std::string(CTDatetime) + "; Loco LRE STOP command executed ";
	       }
	  }



	 //-------------------------------------------------------------------------------------------------------
	 //
	 //             Point and switch processing SWset
	 //
	 //-------------------------------------------------------------------------------------------------------

	  if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"SWset",0) == 0)
	  {
	       char CTDatetime[DATETIMESTRING] = "Not set ";

	       char* CTDatetimeptr = &CTDatetime[0];

	       Clocktime CT;

	       CT.Get_time(*CTDatetimeptr);

	       Browser_response_datastream = std::string(CTDatetime) + "; Switching ";

	       std::string LDswitchnumstr = BPft1->Paramdata(1);

	       std::string LDactionflgstr = BPft1->Paramdata(2);

	       int LDswitchnum = std::stoi(LDswitchnumstr);

	       if (LDswitchnum == 0)
	       {
	       	   std::cout<<"\n1067 Webmessage received Close all switches command\n";
	       	   Browser_response_datastream = std::string(CTDatetime) + "; Close all switches command";
	           LNLSLD->Closeallswitches();
	       }
	       else
	       {
	  			if (LDactionflgstr == "true")
	            {
	  				Browser_response_datastream = std::string(CTDatetime) + "; Switch " + LDswitchnumstr + " opened";

	                LNLSLD->Actionswitch(LDswitchnum,9,1);
	            }

	            if (LDactionflgstr == "false")
	            {
	            	Browser_response_datastream = std::string(CTDatetime) + "; Switch " + LDswitchnumstr + " closed";

	                LNLSLD->Actionswitch(LDswitchnum,10,0);
	            }
	       }

	       LNLSLD->RefreshLastupdatedetails(1);

	       LNLSLD->LogicalSectionTcodeRefresh();

	       LNLSLD->LogicalDetectorupdateDetectorstatus();

	       LNLRprocess->LNlocosectionrefresh();		// refresh the section crossover
	  }

	 //-------------------------------------------------------------------------------------------------------
	 //
	 //            Schedule / Journey / Route detail
	 //
	 //-------------------------------------------------------------------------------------------------------

	  if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"SchedJnyRtedetail",0) == 0)
	 	  	 {
	 	  	       char CTDatetime[DATETIMESTRING] = "Not set ";

	 	  	       char* CTDatetimeptr = &CTDatetime[0];

	 	  	       Clocktime CT;

	 	  	       CT.Get_time(*CTDatetimeptr);

	 	  	       Browser_response_datastream = std::string(CTDatetime) + ";";

	 	  	   	   bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);

	 	  	       LNSchedAlljourneys->BrowserDisplaySchedJnyRtedetail(1,PRHeapResbuff,LNRteSchedMast,LNRtimeallrecsNG);

	 	  	       Browser_response_datastream.append(PRHeapResbuff);		// store the data in the response stream

	 	  	       Browser_response_datastream.append(";");	     		// Add the delimiter.

	 	  	       Browser_response_datastream.append("END;");

	 	  	 }


   // bzero(PRHeapResbuff,HEAPDISPLAYBUFFERLENGTH);


	 //-------------------------------------------------------------------------------------------------------
	 //
	 //            Schedcmd - scheduler execution command processing
	 //
	 //-------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"SCHEDcmd",0) == 0)
	  	 {
	  	       char CTDatetime[DATETIMESTRING] = "Not set ";

	  	       char* CTDatetimeptr = &CTDatetime[0];

	  	       Clocktime CT;

	  	       CT.Get_time(*CTDatetimeptr);

	  	       std::string Schednumber = BPft1->Paramdata(1);

	  	       std::string Schedaction = BPft1->Paramdata(2);

	  	       int Schednumint = std::stoi(Schednumber);

	  	       int Schedactionint = std::stoi(Schedaction);

	  	       if (Schedactionint == 1)
	  	       {
	  	    	   LNSchedAlljourneys->EnableScheduleExecution(Schednumint);

	  	    	   LNSchedExecutor->EnableSchedulerexecution(0);
	  	       }
	  	       else
	  	       {
	  	    	 LNSchedExecutor->DisableSchedulerexecution(0);

	  	    	 LNSchedAlljourneys->DisableScheduleExecution(Schednumint);
	  	       }

	  	       std::cout<<"\n1162 Scheduler command: Schedule:"<<Schednumint<<" Action:"<<Schedactionint;

	  	       Browser_response_datastream = std::string(CTDatetime) + "; Schedule " + Schednumber + " processing completed";
	  	 }

	 //-------------------------------------------------------------------------------------------------------
	 //
	 //            DPset processing - device processing setup
	 //
	 //-------------------------------------------------------------------------------------------------------

	 if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"DPset",0) == 0)
	 {
	       char CTDatetime[DATETIMESTRING] = "Not set ";

	       char* CTDatetimeptr = &CTDatetime[0];

	       Clocktime CT;

	       CT.Get_time(*CTDatetimeptr);

	       std::string DPcodestr = BPft1->Paramdata(1);

	       std::string DPenablestr = BPft1->Paramdata(2);

	       std::string DPstatusstr = BPft1->Paramdata(3);

	       std::string DPflagstr = BPft1->Paramdata(4);

	       int DPcodenum = std::stoi(DPcodestr);

	       int DPenable = std::stoi(DPenablestr);

	       int DPstatus = std::stoi(DPstatusstr);

	       int DPflag = std::stoi(DPflagstr);

	       if (DPflag == 1)
	       {
	           	LNLSLD->UpdateTrigger(DPcodenum,1,DPenable,DPstatus);
	       }

	       if (DPflag == 0)
	       {
	            LNLSLD->UpdateTrigger(DPcodenum,2,DPenable,DPstatus);
	       }

	       if (DPflag == 3)
	       {
	          // Reset all device details

	          // signalmsg == 3 reset all enables, status and triggers

	         	LNLSLD->UpdateTrigger(DPcodenum,3,DPenable,DPstatus);

	       }

	       Browser_response_datastream = std::string(CTDatetime) + "; Device " + DPcodestr + " setup processing completed";
	 }


	 //--------------------------------------------------------------------------------------------------------
	 //
	 //            Device counter processing setup (DPCset)
	 //
	 //--------------------------------------------------------------------------------------------------------

    if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"DPCset",0) == 0)
       {
          char CTDatetime[DATETIMESTRING] = "Not set ";

          char* CTDatetimeptr = &CTDatetime[0];

          Clocktime CT;

          CT.Get_time(*CTDatetimeptr);

          std::string DPCcodestr = BPft1->Paramdata(1);

          std::string DPCcntargetstr = BPft1->Paramdata(2);

          std::string DPCentrigstr = BPft1->Paramdata(3);

          std::string DPCflagstr = BPft1->Paramdata(4);

          int DPcodenum = std::stoi(DPCcodestr);

          int DPCcntarget = std::stoi(DPCcntargetstr);

          int DPCflag = std::stoi(DPCflagstr);

          if (DPCflag == 1)
          {
              // Update device counter details
              LNLSLD->SetCountertarget(DPcodenum,DPCcntarget);
           }

          if (DPCflag == 0)
          {
               // Reset device counter
           	 LNLSLD->ResetCountertarget(DPcodenum);
          }

          if (DPCflag == 3)
          {
            // Reset ALL device counters
           	 LNLSLD->ResetCountertargetAll();
          }

          Browser_response_datastream = std::string(CTDatetime) + "; Device " + DPCcodestr + "counter setup processing completed";
       }

    //---------------------------------------------------------------------------------------------------------
    //
    //      Browser Device STOP command processing
    //
    //---------------------------------------------------------------------------------------------------------

    if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"DSsreq",0) == 0)
    {
       char CTDatetime[DATETIMESTRING] = "Not set ";

       char* CTDatetimeptr = &CTDatetime[0];

       Clocktime CT;

       CT.Get_time(*CTDatetimeptr);

       std::string DSstopstr = BPft1->Paramdata(1);

       std::string DSstoptypestr = BPft1->Paramdata(2);

       std::string DSstopdevicestr = BPft1->Paramdata(3);

       std::string DSstopPSstr = BPft1->Paramdata(4);

       std::string DSstopCSstr = BPft1->Paramdata(5);

       std::string DSstopslotstr = BPft1->Paramdata(6);

       std::string DSstopTcodestr = BPft1->Paramdata(7);

       std::string DSstopflagstr = BPft1->Paramdata(8);

            //  std::cout<<"\n\nLocal stop:"<< DSstopstr << " type:"<<DSstoptypestr<<" device:"<<" PS:"<<DSstopPSstr\
                           		  <<" CS:"<<DSstopCSstr<<" slot:"<<DSstopslotstr<<" Tcode:"<<DSstopTcodestr<<" flag:"<<DSstopflagstr;

       int DPcodenum = std::stoi(DSstopstr);

       int DSstoptype = std::stoi(DSstoptypestr);

       int DSstopdevice = std::stoi(DSstopdevicestr);

       int DSstopPS = std::stoi(DSstopPSstr);

       int DSstopCS = std::stoi(DSstopCSstr);

       int DSstopslot = std::stoi(DSstopslotstr);

       int DSstopTcode = std::stoi(DSstopTcodestr);

       int DSstopflag = std::stoi(DSstopflagstr);

       // BrowserStopflagUpdate(int code, int type, int stoptype,int stopPS, int stopCS, int stopslot, int stopTcode, int stopflg)

       LNLSLD->BrowserStopflagUpdate(DPcodenum,DSstoptype,DSstopdevice,DSstopPS,DSstopCS,DSstopslot,DSstopTcode,DSstopflag);

       Browser_response_datastream = std::string(CTDatetime) + "; Local STOP command setup processing completed";
    }

    //---------------------------------------------------------------------------------------------------------
    //
    //           Simulator command processing
    //
    //---------------------------------------------------------------------------------------------------------


    //  SIMCMD-001-000-2-000-000-000-.dat  --> SIMCMD-Simnum-Rtenum-flag-mode-000-000-

     if (BPft->CharArraytoStringcompare(BPft1->Paramdata(0),"SIMCMD",0) == 0)
     {
            char CTDatetime[DATETIMESTRING] = "Not set ";

            char* CTDatetimeptr = &CTDatetime[0];

            Clocktime CT;

            CT.Get_time(*CTDatetimeptr);

            std::string SimNumberstr = BPft1->Paramdata(1);

            std::string SimRtecodestr = BPft1->Paramdata(2);

            std::string Simflagstr = BPft1->Paramdata(3);

            std::string Simrunmodestr = BPft1->Paramdata(4);

            int Simflag = std::stoi(Simflagstr);

            int SimNumber = std::stoi(SimNumberstr);

            int SimRtecode = std::stoi(SimRtecodestr);

            int SimRunmode = std::stoi(Simrunmodestr);

            if (Simflag == 1)  // initiate scheduler run
            {
                 //  LNSIM->InitMultiSimrun(SimNumber,SimRtecode,SimRunmode,0,0,0,LNLSLD,LNLRprocess);

            	Browser_response_datastream = std::string(CTDatetime) + "; Simulator run started";
            }

            if (Simflag == 2) // stop  the simulator
            {
                LNSIM->StopMultiSimrun(SimNumber);
                Browser_response_datastream = std::string(CTDatetime) + "; Simulator run stopped";
            }

            if (Simflag == 3)
            {
                LNSIM->ResetMultiSimrun(SimNumber);
                Browser_response_datastream = std::string(CTDatetime) + "; Simulator Reset ";
            }

            if (Simflag == 4)
            {
               LNSIM->StepMultiSimrun(SimNumber);
               Browser_response_datastream = std::string(CTDatetime) + "; Simulator step execute ";
            }
     }


	 //-------------------------------------------------------------------------------------------------------//
	 //
	 // Build the complete data stream to return to the server.
	 //
	 //-------------------------------------------------------------------------------------------------------//




	 // Step 1 - get the length of the data response string and convert it to a character string

	 Hlinemsglen = Browser_response_datastream.length();

	 Hlmsglenstr = std::to_string(Hlinemsglen);

	 // Start to build output line as a string

	 // Build header details

	 std::string Msgbacklinehdr = Browser_response_part1 + Browser_response_part2 + Browser_response_part3 + Hlmsglenstr + Browser_response_part4 + Hlmsgtype;

	 Hlinelen = Msgbacklinehdr.length();

	 int msgcc;			  // counter for messages in the output buffer

	 msgcc = 0;

	 // move header to output buffer

	 std::strcpy(Datareturnchars, Msgbacklinehdr.c_str()); 							// copy Line string to a char array

	 char* Datareturncharsptr = &Datareturnchars[0];							// Setup pointer to local output buffer

	 for (int cc = 0;cc <= Hlinelen;cc++)
	 {
		*(msgptr + msgcc) = *(Datareturncharsptr + cc);				// copy the Line with the header data to the Output buffer
				    		msgcc++;
	 }

	 msgcc--;	//  move the counter back one place

	 // move contents of the missing message string to output buffer

	 std::strcpy(Datareturnchars, Browser_response_datastream.c_str()); // copy Line string to a char array

	 for (int cc = 0;cc <= Hlinemsglen;cc++)
	 {
		  *(msgptr + msgcc) = *(Datareturncharsptr + cc);				// populate the return buffer with the file data to the Output buffer

		   msgcc++;
	 }

	 *(msgptr + msgcc) = '\0';

	// std::cout << "\n\n2874 File returned \n" << msgptr;

	 Finalmessagelength = msgcc;

	 delete BP;

	 delete BPft;

	 delete BPft1;
}
