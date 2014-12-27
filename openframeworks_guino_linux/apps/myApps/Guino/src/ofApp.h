#ifndef OFAPP
#define OFAPP

#include "ofMain.h"
#include "ofxUI.h"
#include <EasyTransfer.h>


class ofApp : public ofBaseApp
{
	public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void guinoInit();
    void guinoClear();
    void addColumn();
	void setGUI1();
    void connect();

    bool reFresh;
	ofSerial	serial;
    vector<ofxUICanvas *> gui_column;


	ofxUICanvas *gui1;



	bool hideGUI;

	float red, green, blue;


	void guiEvent(ofxUIEventArgs &e);

    ofxUIMovingGraph *mg;
    float *buffer;
    ofImage *img;

    // arduino



private:
    bool serialConnected;

    ofxUIDropDownList *ddl;

    string buttonState;
    string potValue;
    //create object
    EasyTransfer ET;

    struct RECEIVE_DATA_STRUCTURE{
        //put your variable definitions here for the data you want to receive
        //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
        unsigned char cmd;
        unsigned char item;
        int16_t value;

    };
    ofxUILabelToggle  * toggleConnect;

    // COMMAND STRUCTURE
    #define guino_executed -1
    #define guino_init 0
    #define guino_addSlider 1
    #define guino_addButton 2
    #define guino_iamhere 3
    #define guino_addToggle 4
#define guino_addRotarySlider 5
    #define guino_saveToBoard 6
    #define guino_setFixedGraphBuffer 8
#define guino_clearLabel 7
    #define guino_addWaveform 9
    #define guino_addColumn 10
    #define guino_addSpacer 11
#define guino_xypad 18



#define guino_addLabel 12
#define guino_addMovingGraph 13
#define guino_buttonPressed 14


    #define guino_addChar 15
    #define guino_setMin 16
    #define guino_setMax 17
    #define guino_setValue 20
    #define guino_setColor  21



    vector<ofxUIWidget *> guino_items;

    // END COMMAND STRCUTURE

    RECEIVE_DATA_STRUCTURE guino_data;

    int size;
    int xx;
    int yy;
    int light;



};

#endif
