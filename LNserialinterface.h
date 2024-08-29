/*
 * LNserialinterface.h
 *
 *  Created on: Jun 23, 2019
 *      Author: BrianRoot
 */

#ifndef LNSERIALINTERFACE_H_
#define LNSERIALINTERFACE_H_

#include "LNwebconfig.h"

class LNserialinterface {
public:
	LNserialinterface();
	virtual ~LNserialinterface();

private:
	unsigned char  Physicalsection[MAXPSECTIONS];
};

#endif /* LNSERIALINTERFACE_H_ */
