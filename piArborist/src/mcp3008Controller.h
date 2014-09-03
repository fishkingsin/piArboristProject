//
//  mcp3008Controller.h
//  piArborist
//
//  Created by James Kong on 02/9/14.
//  Copyright (c) 2014 fishkingsin. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "wiringPiSPI.h"


class mcp3008Controller: public ofThread
{
public:
	mcp3008Controller();
	bool setup();
	void threadedFunction();
	

	
	int readAnalogDigitalConvertor();
	int potentiometerInput;
	
	
	int lastChannel0Value;
	int channel0Value;
	int changeAmount;
	bool didPotChange;
	
	bool doToleranceCheck;
	int tolerance;
	
	bool isReady;
	
};