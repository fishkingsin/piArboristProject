//  piArborist
//
//  Created by James Kong on 02/9/14.
//  Copyright (c) 2014 fishkingsin. All rights reserved.
//
#pragma once

#include "ofMain.h"
#include "mcp3008Controller.h"
#include "ofxNetwork.h"

#include "ofxAvahiClientService.h"
class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
	
	mcp3008Controller controller;
	ofSoundPlayer  soundPlayer;
	bool isReady;
	float currentSoundSpeed;

	ofxTCPServer TCP;

	ofxAvahiClientService service;
};

