#include "ofApp.h"
int width = 0;
int height = 0;

float lopass(float input, float cutoff , vector<int>&outputs) {
    float lo_pass_output= outputs[0]+ (cutoff*(input-outputs[0]));
    outputs[0]= lo_pass_output;
    return(lo_pass_output);
}

float hipass(float input, float cutoff, vector<int>&outputs) {
    float hi_pass_output=input-(outputs[0] + cutoff*(input-outputs[0]));
    outputs[0]=hi_pass_output;
    return(hi_pass_output);
}

float average(vector<int>&inputs , vector<int>&outputs )
{
    if(outputs.size()>width)
    {
//        outputs.clear();
        outputs.erase(outputs.begin());
    }
    float average = 0;
    for(int i = 0  ; i  < inputs.size() ; i++)
    {
        average += inputs[i];
    }
    average/=inputs.size();
//    float x= outputs.size();
    float y= ofMap(  average,0,1024,height-10,10);
    outputs.push_back(y);
    return average;
}
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    //we run at 60 fps!
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    ofAddListener(ofxBonjour::Events().onServicesDiscovered, this, &ofApp::discoveredServices);
    ofAddListener(ofxBonjour::Events().onServiceDiscovered, this, &ofApp::gotServiceData );
    bonjourClient.discover("_ecs._tcp.");
    port = 11999;
    //create the socket and bind to port 11999
//    udpConnection.Create();
//    udpConnection.Bind(5005);
//    udpConnection.SetNonBlocking(true);
    //are we connected to the server - if this fails we
	//will check every few seconds to see if the server exists
    weConnected = false;
//	weConnected = tcpClient.setup(host, port);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	tcpClient.setMessageDelimiter("[/TCP]");
	
	connectTime = 0;
	deltaTime = 0;

    
    ofSetBackgroundAuto(true);
    ofBackground(255, 255, 255);
    startThread();
    width = ofGetWidth();
    height = ofGetHeight();
    
}
void ofApp::exit()
{
    stopThread();
}
void ofApp::threadedFunction()
{
    while(isThreadRunning())
    {
         /*char udpMessage[1000];
         udpConnection.Receive(udpMessage,1000);
         string message=udpMessage;
         if(message != ""){
             int averageSample = 0;
             if(points.size()>width)
             {

                 points.erase(points.begin());
             }
             if(inputsample.size() >=SAMPLE_SIZE)
             {
                 averageSample = average(inputsample,average_points);
                 inputsample.erase(inputsample.begin());
             }
                 
             float x,y;
             x=points.size();
             int value = ( (message [0] & 0xff) | ((message[1]  & 0xff) << 8) ) ;
             inputsample.push_back(value);
             ofLogVerbose() << value;
             y= ofMap(  value,0,1024,ofGetHeight()-10,10);
             points.push_back(ofPoint(x,y));
             
             
             if(highpass_sample.size()>SAMPLE_SIZE)
             {
                 average(highpass_sample,average_hi_points);
//                 highpass_sample.clear();
                 highpass_sample.erase(highpass_sample.begin());
             }
             if(lowpass_sample.size()>SAMPLE_SIZE)
             {
                 average(lowpass_sample,average_lo_points);
//                 lowpass_sample.clear();
                 lowpass_sample.erase(lowpass_sample.begin());
             }
             
             if(inputsample.size()>0)
             {
                 int input = inputsample[inputsample.size()-1];
                 if(highpass_sample.size()==0)
                 {
                     highpass_sample.push_back(input);
                 }
                 if(lowpass_sample.size()==0)
                 {
                     lowpass_sample.push_back(input);
                 }
//                 int input = inputsample[inputsample.size()-1];
                 
                 float new_highpass= hipass(input*1.0f, 0.55, highpass_sample);
                 highpass_sample.push_back(new_highpass);
                 float new_lowpass= lopass(input*1.0f, 0.45, lowpass_sample);
                 lowpass_sample.push_back(new_lowpass);
             }
         }*/
         sleep(2);
     }
}
//--------------------------------------------------------------
void ofApp::update(){


    if(weConnected)
    {
        if(tcpClient.send("a")){
        
            //if data has been sent lets update our text
            unsigned char buffer [6];
            if(tcpClient.receiveRawMsg((char*)buffer, 6)>0)
            {
                stringstream info;
                int lastChannel0Value = ((buffer [0] & 0xff) | ((buffer[1]  & 0xff) << 8));
                int channel0Value = ((buffer [2] & 0xff) | ((buffer[3]  & 0xff) << 8));
                int changeAmount = ((buffer [4] & 0xff) | ((buffer[5]  & 0xff) << 8));
                info << "lastChannel0Value: "		<< lastChannel0Value		<< "\n";
                info << "channel0Value: "			<< channel0Value			<< "\n";
                info << "changeAmount: "		<< changeAmount		<< "\n";
                message = info.str();
                
                int averageSample = 0;
                if(points.size()>width)
                {
                    
                    points.erase(points.begin());
                }
                if(inputsample.size() >=SAMPLE_SIZE)
                {
                    averageSample = average(inputsample,average_points);
                    inputsample.erase(inputsample.begin());
                }
                
                float x,y;
//                x=points.size();
                int value = channel0Value;
                
                inputsample.push_back(value);
                //ofLogVerbose() << value;
                y= ofMap(  value,0,1024,ofGetHeight()-10,10);
                maxValue = (int)max(y,(float)maxValue);
                minValue = (int)min(y,(float)minValue);
                points.push_back(y);
                
                if(highpass_sample.size()>SAMPLE_SIZE)
                {
                    average(highpass_sample,average_hi_points);
                    //                 highpass_sample.clear();
                    highpass_sample.erase(highpass_sample.begin());
                }
                if(lowpass_sample.size()>SAMPLE_SIZE)
                {
                    average(lowpass_sample,average_lo_points);
                    //                 lowpass_sample.clear();
                    lowpass_sample.erase(lowpass_sample.begin());
                }
                
                if(inputsample.size()>0)
                {
                    int input = inputsample[inputsample.size()-1];
                    if(highpass_sample.size()==0)
                    {
                        highpass_sample.push_back(input);
                    }
                    if(lowpass_sample.size()==0)
                    {
                        lowpass_sample.push_back(input);
                    }
                    //                 int input = inputsample[inputsample.size()-1];
                    
                    float new_highpass= hipass(input*1.0f, 0.55, highpass_sample);
                    highpass_sample.push_back(new_highpass);
                    float new_lowpass= lopass(input*1.0f, 0.45, lowpass_sample);
                    lowpass_sample.push_back(new_lowpass);
                }
                
            }
        
        }else if(!tcpClient.isConnected()){
            weConnected = false;
        }
    }
    else{
        
        //if we are not connected lets try and reconnect every 5 seconds
        deltaTime = ofGetElapsedTimeMillis() - connectTime;
        
        if( deltaTime > 5000 ){
            weConnected = tcpClient.setup(host, port);
            connectTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
    ofFill();
    ofSetHexColor(0xF0F0F0);
    ofRect(0,0,width,height);
    ofPopStyle();
    
    ofPushStyle();
    ofSetHexColor(0x101010);
    ofDrawBitmapString(message, 10, 20);
    for(unsigned int i=1;i<points.size();i++){
        ofLine(i-1,points[i-1],i,points[i]);
    }
    ofPopStyle();
    ofPushStyle();
    ofSetColor(ofColor::green);
    for(unsigned int i=1;i<average_points.size();i++){
        ofLine(i-1,average_points[i-1],i,average_points[i]);
    }
    ofPopStyle();
    
    ofPopStyle();
    ofPushStyle();
    ofSetColor(ofColor::red);
    for(unsigned int i=1;i<average_hi_points.size();i++){
        ofLine(i-1,average_hi_points[i-1],i,average_hi_points[i]);
    }
    ofPopStyle();
    
    ofPopStyle();
    ofPushStyle();
    ofSetColor(ofColor::blue);
    for(unsigned int i=1;i<average_lo_points.size();i++){
        ofLine(i-1,average_lo_points[i-1],i,average_lo_points[i]);
    }
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(ofColor::purple);
    ofLine(0, minValue, width, minValue);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(ofColor::yellow);
    ofLine(0, maxValue, width, maxValue);
    ofPopStyle();
    
    if(!weConnected)
    {
        ofDrawBitmapString("Connecting",width*0.5,height*0.5);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void ofApp::discoveredServices( vector<NSNetService*> & services ){
    for (int i=0; i<services.size(); i++){
        cout<< [services[i].description cStringUsingEncoding:NSUTF8StringEncoding] << endl;
    }
}

//--------------------------------------------------------------
void ofApp::gotServiceData( Service & service ){
    cout<< service.ipAddress << ":" << service.port << endl;
    if(service.ipAddress != "0.0.0.0")
    {
        host = service.ipAddress;
    }
}