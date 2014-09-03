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
	ofSetLogLevel(OF_LOG_NOTICE);
    currentSoundSpeed = 1.0;
	isReady = controller.setup();
	bool didLoadSound = soundPlayer.loadSound("sounds/Heartbeat-SoundBible.com-1259974459.wav");
	soundPlayer.setLoop(true);
	soundPlayer.play(); 
	ofLogVerbose() << "didLoadSound: " << didLoadSound;
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
	ofLogNotice() << info.str();
	
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

