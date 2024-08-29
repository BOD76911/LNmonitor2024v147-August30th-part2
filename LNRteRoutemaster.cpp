/*
 * LNRteRoutemaster.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: BrianRoot
 */

#include "LNRteRoutemaster.h"
#include "LNRteCMDlookuptable.h"

LNRteRoutemaster::LNRteRoutemaster() {
	// TODO Auto-generated constructor stub

}

LNRteRoutemaster::~LNRteRoutemaster() {
	// TODO Auto-generated destructor stub
}

void LNRteRoutemaster::StoreRouteline(LNRtelinerecord LNRteline, LNRouteSchedMaster* LNRteSchedMast)
{
	unsigned int Routenumber;
	unsigned int Routestep;
	unsigned int Routesubstep;
	unsigned int RoutePS; 			
	unsigned int RouteCS; 
	unsigned int RouteNS; 
	unsigned int RouteCmD;

	std::vector<int> Rtelinedetail = LNRteline.Rtelinedata();

	std::vector<int >::iterator iRteSTEPLS;

	iRteSTEPLS = Rtelinedetail.begin();

	Routenumber 	= *(iRteSTEPLS + 0);
	Routestep 		= *(iRteSTEPLS + 1);
	Routesubstep 	= *(iRteSTEPLS + 2);
	RoutePS			= *(iRteSTEPLS + 3);
	RouteCS			= *(iRteSTEPLS + 4);
	RouteNS			= *(iRteSTEPLS + 5);
	RouteCmD		= *(iRteSTEPLS + 6);
	
//	std::cout<<"\n\nLNRteRoutemaster line 36 storing record for Route:"<<Routenumber<<\
	"\nStep:"<<Routestep<<" Substep:"<<Routesubstep<<" PS:"<<RoutePS<<" CS:"<<RouteCS<<" NS:"<<RouteNS<<" CmD:"<<RouteCmD;

	LNRteSchedMast->StoreRoutelinedetailsarray(Routenumber,Routestep,Routesubstep,*(iRteSTEPLS + 3),*(iRteSTEPLS + 4),\
			*(iRteSTEPLS + 5),*(iRteSTEPLS + 6),*(iRteSTEPLS + 7),*(iRteSTEPLS + 8),*(iRteSTEPLS + 9),*(iRteSTEPLS + 10));

	//Routestepcounter++;
}

void LNRteRoutemaster::StoreLocoRoutedetails(int Rtenum, LNweblocorouteprocess* LNLRprocess,LNRouteSchedMaster* LNRteSchedMast)
{
	LNRtelinerecord* Rteline = new LNRtelinerecord();

	std::vector<int> routelinedetails;

	char cmdtext[] = "autoRte";

	char* cmdtextptr = &cmdtext[0];

	std::vector<int> Rtelinedetails;

	std::vector<int>::iterator iroutelinedetails; //= routelinedetails.begin();

	int Rte,Rtestep,Rtesubstep,RtePS,RteCS,RteNS,Rtecmd,Rtecp1,Rtecp2,Rtecp3,Rtecp4,Rtestepcntr, Rtesubstepcntr;

	for (Rtestepcntr = 1; Rtestepcntr < MAXNUMRTESTEPS; Rtestepcntr++)
	{
		for (Rtesubstepcntr = 1; Rtesubstepcntr < MAXNUMRTESUBSTEPS; Rtesubstepcntr++)
		{
			if (LNRteSchedMast->GetCSvalue(Rtenum,Rtestepcntr,Rtesubstepcntr) > 0)
			{
				Rtelinedetails = LNRteSchedMast->Routestepdata(Rtenum,Rtestepcntr,Rtesubstepcntr);
				iroutelinedetails = Rtelinedetails.begin();

				Rte = *(iroutelinedetails);
				Rtestep 	= *(iroutelinedetails + 1);
				Rtesubstep 	= *(iroutelinedetails + 2);
				RtePS	 	= *(iroutelinedetails + 3);
				RteCS	 	= *(iroutelinedetails + 4);
				RteNS	 	= *(iroutelinedetails + 5);
				Rtecmd		= *(iroutelinedetails + 6);
				Rtecp1	 	= *(iroutelinedetails + 7);
				Rtecp2	 	= *(iroutelinedetails + 8);
				Rtecp3	 	= *(iroutelinedetails + 9);
				Rtecp4	 	= *(iroutelinedetails + 10);

				LNLRprocess->SaveRouteline(Rte,Rtestep,Rtesubstep,RtePS,RteCS,RteNS,Rtecmd,Rtecp1,Rtecp2,Rtecp3,Rtecp4,&cmdtext[0],0);
			}
		}
	}

	return;
}

