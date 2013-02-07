#include "testApp.h"

//--------------------------------------------------------------


#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
    //put your variable definitions here for the data you want to receive
    //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    int16_t size;
    int16_t x;
    int16_t y;
};

int size = 0;
int xx = 0;
int yy = 0;
int light = 0;

//give a name to the group of data



void loop(){
    
}

void testApp::setup(){
	ofSetVerticalSync(false);
	
	bSendSerialMessage = false;
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 64);
	

	serial.setup("/dev/tty.usbmodem411", 19200); //linux example
	  ET.begin((uint8_t*)&mydata, sizeof(mydata),&serial);
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
}

//--------------------------------------------------------------
void testApp::update(){
	while(serial.available()){
        if(ET.receiveData())
        {
        size = mydata.size;
        xx = (int)mydata.x;
        yy = (int)mydata.y;
        }
        
    }
    mydata.x = (int16_t)mouseX;
    mydata.y = (int16_t)mouseY;
    mydata.size = light;
    light++;
    light = light % 250;
    ET.sendData();

  
  //  cout << sizeof(mydata);
    
}

//--------------------------------------------------------------
void testApp::draw(	){
	
    ofBackground(0);
    ofSetColor(0,0,255);
    ofFill();
        
    ofCircle(xx,yy,size);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bSendSerialMessage = true;
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

