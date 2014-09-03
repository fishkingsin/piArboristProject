#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#define SAMPLE_SIZE 10
class ofApp : public ofBaseApp , ofThread{

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
    void threadedFunction();
    ofxTCPClient tcpClient;
    bool weConnected;
    float counter;
    int connectTime;
    int deltaTime;
    
    ofTrueTypeFont  mono;
    ofTrueTypeFont  monosm;
    
    vector<ofPoint> points;
    vector<ofPoint> average_points;
    vector<ofPoint> average_hi_points;
    vector<ofPoint> average_lo_points;
    vector<int> inputsample;
    vector<int> highpass_sample;
    vector<int> lowpass_sample;
    string message;
    
    
};
