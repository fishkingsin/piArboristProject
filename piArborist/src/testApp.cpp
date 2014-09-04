//  piArborist
//
//  Created by James Kong on 02/9/14.
//  Copyright (c) 2014 fishkingsin. All rights reserved.
//
#include "testApp.h"

//Often I have to run 
// $sudo amixer cset numid=3 1
//


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_SILENT);
	service.start("piArborist", "_ecs._tcp", 1234);
    currentSoundSpeed = 1.0;
	isReady = controller.setup();
	bool didLoadSound = soundPlayer.loadSound("sounds/Heartbeat-SoundBible.com-1259974459.wav");
	soundPlayer.setLoop(true);
	soundPlayer.play(); 
	ofLogVerbose() << "didLoadSound: " << didLoadSound;

	//we run at 60 fps!
	ofSetVerticalSync(true);

	//setup the server to listen on 11999
	TCP.setup(11999);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
	TCP.setMessageDelimiter("[/TCP]");
}


//--------------------------------------------------------------
void testApp::update(){

	ofSoundUpdate();
	if (!isReady) {
		return;
	}
	currentSoundSpeed = ofMap((float)controller.channel0Value, 0.0, 1024.0, 2.0, 0.1, true);
	soundPlayer.setSpeed(currentSoundSpeed);
	stringstream info;
	info << "lastChannel0Value: "		<< controller.lastChannel0Value		<< "\n";
	info << "channel0Value: "			<< controller.channel0Value			<< "\n";
	info << "changeAmount: "		<< controller.changeAmount		<< "\n";
	info << "fps: "					<< ofToString(ofGetFrameRate())		<< "\n";
	info << "currentSoundSpeed: "	<< ofToString(currentSoundSpeed)	<< "\n";
	// ofLogNotice() << info.str();

	//for each client lets send them a message letting them know what port they are connected on
	for(unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++){

		if( !TCP.isClientConnected(i) )continue;
		//we only want to update the text we have recieved there is data
		string str = TCP.receive(i);
		//protocol
		char byteToSend [6];
		
		byteToSend[0] = controller.lastChannel0Value & 0xff;
		byteToSend[1] = (controller.lastChannel0Value  >> 8)& 0xFF;
		byteToSend[2] = controller.channel0Value & 0xff;
		byteToSend[3] = (controller.channel0Value  >> 8)& 0xFF;
		byteToSend[4] = controller.changeAmount & 0xff;
		byteToSend[5] = (controller.changeAmount   >> 8)& 0xFF;
		
		if(str=="a")
		{
			TCP.sendRawMsg(i, byteToSend , 6);
		}
	}
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	return;
	if (!isReady) {
		return;
	}
	// stringstream info;
	// info << "lastChannel0Value: "		<< controller.lastChannel0Value		<< "\n";
	// info << "channel0Value: "			<< controller.channel0Value			<< "\n";
	// info << "changeAmount: "		<< controller.changeAmount		<< "\n";
	// info << "fps: "					<< ofToString(ofGetFrameRate())		<< "\n";
	// info << "currentSoundSpeed: "	<< ofToString(currentSoundSpeed)	<< "\n";
	
	
	// int colorValue = ofMap(controller.channel0Value, 0, 1024, 0, 255, true);
	// ofBackgroundGradient(ofColor::darkOliveGreen, ofColor(colorValue, colorValue, colorValue),  OF_GRADIENT_CIRCULAR);
	// ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	
	
}

void testApp::exit()
{
	if (isReady)
	{
		isReady = false;
		controller.waitForThread(true);
		ofLogVerbose() << "finished waiting on EXIT";
	}
}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

