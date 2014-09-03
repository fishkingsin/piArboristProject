//
//  mcp3008Controller.cpp
//  piArborist
//
//  Created by James Kong on 02/9/14.
//  Copyright (c) 2014 fishkingsin. All rights reserved.
//

#include "mcp3008Controller.h"

mcp3008Controller::mcp3008Controller()
{
	isReady = false;
    didPotChange = false;
	changeAmount = 0;
	lastChannel0Value = 0;
	channel0Value = 0;
    //10k trim pot connected to adc #0
    potentiometerInput = 0;
}


bool mcp3008Controller::setup()
{
	int status = wiringPiSPISetup(0, 1000000);
	if (status != -1)
	{
		ofLogVerbose() << "wiringPiSetup PASS";
		isReady = true;
	}else 
	{
		ofLogError() << "wiringPiSetup FAIL status: " << status;
	}
    
	if (isReady) 
	{
		startThread(false);
	}
	return isReady;
}


void mcp3008Controller::threadedFunction()
{
	while (isThreadRunning()) 
	{
		didPotChange = false;
		
		channel0Value = readAnalogDigitalConvertor();
		changeAmount = abs(channel0Value - lastChannel0Value);
        
		if(changeAmount!=0)
        {
            didPotChange = true;
        }
        
		lastChannel0Value = channel0Value;
        sleep(10);
	}
}

int  mcp3008Controller::readAnalogDigitalConvertor()
{
	uint8_t buffer[3];
    
	buffer[0] = 1;
	buffer[1] = (8+potentiometerInput)<<4;
	buffer[2] = 0;
    
	wiringPiSPIDataRW(0, buffer, 3);
    
	return ((buffer[1]&3) << 8) + buffer[2];
}
