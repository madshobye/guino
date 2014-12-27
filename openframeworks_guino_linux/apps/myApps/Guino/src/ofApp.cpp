/*

 First version attack
 - matrix
 - joystick
 - midi
 - osc

 - refresh graph
 - setmin fixed graph?
 - multiclick

 */


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    reFresh = false;
	ofEnableSmoothing();
	ofBackground(0);
	serialConnected = false;

	setGUI1();


    gui1->setDrawBack(true);

//
    ofBackground(red, green, blue);
ET.begin((uint8_t*)&guino_data, sizeof(guino_data),&serial);

}



//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::update()
{
    if(reFresh)
    {
      //  ddl->clearToggles();

         ddl->clearEmbeddedWidgets();
        vector<string> names;
        vector<ofSerialDeviceInfo> serialDevices = serial.getDeviceList();
        string deviceLine;
        for(int i=0; i<serialDevices.size();i++){

            ddl->addToggle(serialDevices[0].getDeviceName().c_str());

        }
        reFresh = false;
    }
    ofBackground(125, 255, 255);
    if(serialConnected)
    {

    while(serial.available()> 3){

        if(ET.receiveData())
        {
            if(gui_column.size() == 0)
            {
                addColumn();
            }
            ofxUICanvas *gui = gui_column[gui_column.size() -1];

            float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
            float length = 255-xInit;
            float dim = 16;

            switch (guino_data.cmd)
            {
                case guino_addSlider:
                    ofxUISlider *slider;
                    slider = new ofxUISlider("", 0.0, 255.0, red, length-xInit, dim);

                    guino_items.push_back(slider);
                    gui->addWidgetDown(slider);
                    slider->setID(guino_data.item);

                    slider->setValue(guino_data.value);

                    break;
                case guino_setMax:
                    if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_SLIDER_H)
                    {
                        ((ofxUISlider *)guino_items[guino_data.item])->setMax(guino_data.value);


                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_MOVINGGRAPH)
                    {
                        ((ofxUIMovingGraph *)guino_items[guino_data.item])->setMax(guino_data.value);

                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_WAVEFORM)
                    {
                        ((ofxUIWaveform *)guino_items[guino_data.item])->setMax(guino_data.value);

                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER)
                    {
                        ((ofxUIRotarySlider *)guino_items[guino_data.item])->setMax(guino_data.value);

                    }


                    break;
                case guino_setMin:
                    if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_SLIDER_H)
                    {
                        ((ofxUISlider *)guino_items[guino_data.item])->setMin(guino_data.value);


                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_MOVINGGRAPH)
                    {
                        ((ofxUIMovingGraph *)guino_items[guino_data.item])->setMin(guino_data.value);

                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_WAVEFORM)
                    {
                        ((ofxUIWaveform *)guino_items[guino_data.item])->setMin(guino_data.value);

                    }
                    else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER)
                    {
                        ((ofxUIRotarySlider *)guino_items[guino_data.item])->setMin(guino_data.value);

                    }

                    break;
                case guino_iamhere:
                    guinoInit();

                    break;
                case guino_setValue:
                    if(guino_items.size() > guino_data.item)
                    {
                        if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_SLIDER_H)
                        {
                            ((ofxUISlider *)guino_items[guino_data.item])->setValue(guino_data.value);
                        }
                        else if (guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_MOVINGGRAPH)
                        {

                            ((ofxUIMovingGraph *)guino_items[guino_data.item])->addPoint(guino_data.value);
                        }
                        else if (guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_LABELTOGGLE)
                        {
                            ((ofxUILabelToggle *)guino_items[guino_data.item])->setValue(!guino_data.value==0);

                        }
                        else if (guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_WAVEFORM)
                        {

                            //((ofxUIWaveform *)guino_items[guino_data.item])->addPoint(guino_data.value);
                            //(ofxUIWaveform *)guino_items[guino_data.item]

                        }
                        else if (guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER)
                        {

                            ((ofxUIRotarySlider *)guino_items[guino_data.item])->setValue(guino_data.value);
                        }


                    }

                    break;
                case guino_addChar:
                    if(guino_items.size() > guino_data.item)
                    {  // crappy hack to compensate for gui
                        string _name =  guino_items[guino_data.item]->getName()+ ofToString((char)guino_data.value);
                        ((ofxUIWidgetWithLabel *)guino_items[guino_data.item])->setName(_name );
                        if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_SLIDER_H ||
                           guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER
                           )


                        ((ofxUIWidgetWithLabel *)guino_items[guino_data.item])->getLabelWidget()->setLabel(_name);

                        else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_LABEL)
                                {
                                     ((ofxUILabel *)guino_items[guino_data.item])->setLabel(_name);
                                }
                        else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_LABELBUTTON)
                        {
                            ((ofxUILabelButton *) guino_items[guino_data.item])->setLabelText(_name);
                        }
                        else if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_LABELTOGGLE)
                        {
                            ((ofxUILabelToggle *) guino_items[guino_data.item])->setLabelText(_name);
                        }




                    }
                    break;
                case guino_addToggle:
                {
                    ofxUIToggle * toggle =  gui->addLabelToggle( "", false, length-xInit,25);

                    guino_items.push_back(toggle);
                    gui->addWidget(toggle);
                    toggle->setID(guino_data.item);

                    toggle->setValue(guino_data.value);

                }
                    break;

                case guino_addButton:
                {
                    ofxUILabelButton * button = new ofxUILabelButton("", false, length-xInit,25);
                    guino_items.push_back(button);
                    gui->addWidgetDown(button);
                    button->setID(guino_data.item);

                    button->setValue(guino_data.value);
                }

                    break;

                case guino_addMovingGraph:

                {
                    vector<float> buffer;
                    for(int i = 0; i < 256; i++)
                    {
                        buffer.push_back(0.0);
                    }

                    ofxUIMovingGraph * mg = new ofxUIMovingGraph((length-xInit) * ((float)guino_data.value)/10.0f, 120 * ((float)guino_data.value)/10.0f, buffer, 256, 0, 1000, "MOVING GRAPH");
                    gui->addWidgetDown(mg);
                    guino_items.push_back(mg);
                    mg->setID(guino_data.item);
                  //  mg->addPoint(guino_data.value);
                }

                    break;
                case guino_addLabel:
                    {
                    ofxUILabel* label = new ofxUILabel("", guino_data.value);


                    guino_items.push_back(label);
                    gui->addWidgetDown(label);
                    label->setID(guino_data.item);

                    }
                    break;
                case guino_addSpacer:
                {
                    ofxUISpacer * spacer =  gui->addSpacer(length-xInit, guino_data.value);
                    guino_items.push_back(spacer);
                    spacer->setID(guino_data.item);
                }
                    break;
                case guino_addColumn:
                {
                    addColumn();
                }
                    break;
                case guino_addWaveform:
                {
                    buffer = new float[2000];
                    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }

                    ofxUIWaveform * wave = new ofxUIWaveform((length-xInit) * ((float)guino_data.value)/10.0f, 120 * ((float)guino_data.value)/10.0f, buffer, 256, 0.0, 1.0, "WAVEFORM");
                  	gui->addWidget(wave);


                    gui->addWidgetDown(wave);
                    guino_items.push_back(wave);
                    wave->setID(guino_data.item);
                }


                    break;

                case guino_setFixedGraphBuffer:
                {
                    cout << "lksjdf";
                    if (guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_WAVEFORM)
                    {

                        ((ofxUIWaveform *)guino_items[guino_data.item])->setBufferSize(guino_data.value);
                    }
                }
                    break;
                case guino_clearLabel:
                {
                     ((ofxUIWidgetWithLabel *)guino_items[guino_data.item])->setName("");
                    if(guino_items[guino_data.item]->getKind() == OFX_UI_WIDGET_LABEL)
                    {
                        ((ofxUILabel *)guino_items[guino_data.item])->setLabel("");
                    }
                }
                    break;
                case guino_addRotarySlider:
                {
                    ofxUIRotarySlider * rotary = new ofxUIRotarySlider(dim*4, 0, 100, 50, "");

                    if(guino_items[guino_items.size()-1]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER)
                    {
                        gui->addWidgetRight(rotary);
                    }
                    else
                    {
                        gui->addWidgetDown(rotary);
                    }
                    rotary->setValue(guino_data.value);

                    guino_items.push_back(rotary);
                    rotary->setID(guino_data.item);
                }
                    break;
                case guino_xypad:
                   /* gui1->addWidgetDown(new ofxUI2DPad(length-xInit,80, ofPoint((length-xInit)*.5,80*.5), "PAD"));

                    ofxUIRotarySlider * rotary = new ofxUIRotarySlider(dim*4, 0, 100, 50, "");

                    if(guino_items[guino_items.size()-1]->getKind() == OFX_UI_WIDGET_ROTARYSLIDER)
                    {
                        gui->addWidgetRight(rotary);
                    }
                    else
                    {
                        gui->addWidgetDown(rotary);
                    }

                    guino_items.push_back(rotary);
                    rotary->setID(guino_data.item);*/
                    break;
                case guino_setColor:

                {
                    if(guino_data.item == 0)
                    {
                        red = guino_data.value;
                    }
                    else if(guino_data.item == 1)
                    {
                        green = guino_data.value;
                    }
                    else if(guino_data.item == 2)
                    {
                        blue = guino_data.value;
                    }
                }
                    break;
            }
            guino_data.cmd = guino_executed;
            // add slider here

        }
    }
    }

}
void ofApp::addColumn()
{
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    float dim = 16;
    gui_column.push_back(new ofxUICanvas(length*(gui_column.size()+1)+xInit+2, 0, length+xInit, ofGetHeight()));

    gui_column[gui_column.size()-1]->setDrawBack(false);
    ofAddListener(gui_column[gui_column.size()-1]->newGUIEvent,this,&ofApp::guiEvent);
}
void ofApp::guinoClear()
{

    // THIS has memory leak potential
    if(gui_column.size() !=0)
    {

        guino_items.clear();
        for(int i = 0; i < gui_column.size();i++)
        {
        ofRemoveListener(gui_column[i]->newGUIEvent, this,&ofApp::guiEvent);
        gui_column[i]->clearEmbeddedWidgets();

        delete gui_column[i];

        }
        gui_column.clear();

    }
}

void ofApp::guinoInit()
{
     float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    float dim = 16;

    guinoClear();
    addColumn();




    guino_data.cmd = guino_init;

    ET.sendData();
    cout << "sending init";
}


//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(red, green, blue, 255);



    ofSetRectMode(OF_RECTMODE_CENTER);
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();

if(serialConnected)
{
    if(e.widget->getID() >=0)
    {
        if(kind == OFX_UI_WIDGET_SLIDER_H)
        {
            guino_data.value = (int16_t)((ofxUISlider *)guino_items[e.widget->getID()])->getScaledValue();
            guino_data.item = e.widget->getID();
            guino_data.cmd = guino_setValue;
            ET.sendData();
        }
        else if(kind == OFX_UI_WIDGET_LABELBUTTON)
        {
            guino_data.item = e.widget->getID();
            guino_data.cmd = guino_buttonPressed;

            ET.sendData();

        }
        else if(kind == OFX_UI_WIDGET_LABELTOGGLE)
        {
            guino_data.item = e.widget->getID();
            guino_data.cmd = guino_setValue;
            guino_data.value = (int16_t)((ofxUIToggle *)guino_items[e.widget->getID()])->getValue();
            ET.sendData();

        }
        else if( kind  == OFX_UI_WIDGET_ROTARYSLIDER)
        {
            guino_data.value = (int16_t)((ofxUIRotarySlider *)guino_items[e.widget->getID()])->getScaledValue();
            guino_data.item = e.widget->getID();
            guino_data.cmd = guino_setValue;
            ET.sendData();

        }


    }
}
    if(name == "CONNECT" || name == "DISCONNECT")
    {
        connect();
    }
	else if (name=="REFRESH PORT LIST")
    {
        cout << "Rerfresh";


        reFresh = true;

    }
    else if (name=="SAVE TO BOARD" && serialConnected)
    {
        cout << "hep";
        guino_data.item = -1;
        guino_data.cmd = guino_saveToBoard;
        guino_data.value = -1;
        ET.sendData();


    }



}
//--------------------------------------------------------------
void ofApp::exit()
{
	delete gui1;

}

void ofApp::connect()
{


    if(toggleConnect->getValue())
    {
        string selectedText = "";
        vector<ofxUIWidget *> &selected = ddl->getSelected();

        cout << selected.size()<< endl;
        for(int i = 0; i < selected.size(); i++)
        {
            cout << "SELECTED: " << selected[i]->getName() << endl;
            selectedText = selected[i]->getName();
        }
        cout << selectedText << endl;
        if(selectedText != "")
        {
            serialConnected = serial.setup(selectedText, 115200);

        }

    }
    else
    {
        serialConnected = false;
        serial.close();
        guinoClear();


    }
    toggleConnect->setValue(serialConnected);
    if(serialConnected)
    {
        toggleConnect->setLabelText("DISCONNECT");

    }
    else {
        toggleConnect->setLabelText("CONNECT");
    }
    if(serialConnected)
    {
        guinoInit();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
   /* if(gui2->hasKeyboardFocus())
    {
        return;
    }*/
	switch (key)
	{
		case 's':
		{
			/*string hr;
			ofImage img;
			img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
			if(ofGetHours() < 12)
			{
				hr = " AM";
			}
			else
			{
				hr = " PM";
			}
			img.saveImage("snapshots/OFXUI "+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+" at "+ofToString(ofGetHours(),2)+"."+ofToString(ofGetMinutes(),2)+"."+ofToString(ofGetSeconds(),2) +hr +".png");*/
            guino_data.item = -1;
            guino_data.cmd = guino_saveToBoard;
            guino_data.value = -1;
            ET.sendData();
		}
			break;

		case 'f':
			ofToggleFullscreen();
			break;

		case 'c':
            toggleConnect->toggleValue();
            connect();
			break;
        case 't':
            gui1->toggleVisible();
        {
            float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
            float length = 255-xInit;

            if(gui1->isVisible())
            {
                for(int i = 0; i < gui_column.size();i++)
                {
                    gui_column[i]->getRect()->x = gui_column[i]->getRect()->x +(length+xInit);
                }


            }
            else {
                for(int i = 0; i < gui_column.size();i++)
                {
                    gui_column[i]->getRect()->x = gui_column[i]->getRect()->x -(length+xInit);
                }
            }
        }
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{


}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}



void ofApp::setGUI1()
{
	red = 233; blue = 52; green = 27;

	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	hideGUI = false;





	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
	gui1->addWidgetDown(new ofxUILabel("GUINO:ArduinoDashboard", OFX_UI_FONT_LARGE));
	;

    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("1. SELECT SERIAL PORT.", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("2. PRESS CONNECT.", OFX_UI_FONT_SMALL));


    vector<string> names;
    vector<ofSerialDeviceInfo> serialDevices = serial.getDeviceList();
    string deviceLine;
    for(int i=0; i<serialDevices.size();i++){

        deviceLine = serialDevices[i].getDeviceName().c_str();
        names.push_back(deviceLine);
    }

    ddl = new ofxUIDropDownList(length-xInit, "SELECT PORT",names , OFX_UI_FONT_MEDIUM);
    ddl->draw();

    ddl->setAllowMultiple(false);
    ddl->setAutoClose(true);
    ddl->setDrawBack(true);
    ddl->setColorBack( ofColor(0,0,0,255));
    ddl->setColorFill(ofColor(0,0,0));
    ddl->setDrawBack(true);

    gui1->addWidgetDown(ddl);
     toggleConnect =  gui1->addLabelToggle( "CONNECT", false, length-xInit);
    //toggleConnect->solid = false;
     toggleConnect->setID(-1);
    //ofxUILabelButton *a = new ofxUILabelButton("REFRESH PORT LIST", false, length-xInit);
    //gui1->addWidgetDown(a);


    gui1->addSpacer(length-xInit, 2);
    ofxUILabelButton *b = new ofxUILabelButton("SAVE TO BOARD", false, length-xInit);
    gui1->addWidgetDown(b);
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("(F)ULL SCREEN", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("(C)ONNECT", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("(S)AVE TO BOARD", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("(T)OGGLE VISIBILITY", OFX_UI_FONT_SMALL));



    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("LINUX PORT: @MENTARUS", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("BY: MADS HOBYE", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("WITH: INSTRUCTABLES & MEDEA", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("WWW.HOBYE.DK", OFX_UI_FONT_SMALL));
    gui1->addWidgetDown(new ofxUILabel("VERSION: 1.0", OFX_UI_FONT_SMALL));

	ofAddListener(gui1->newGUIEvent,this,&ofApp::guiEvent);




  /*	gui1->addSlider("BLUE", 0.0, 255.0, blue, length-xInit,dim);

  gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("V SLIDERS", OFX_UI_FONT_MEDIUM));
	gui1->addSlider("0", 0.0, 255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui1->addSlider("1", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("2", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("3", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("4", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("5", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("6", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("7", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("8", 0.0, 255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    gui1->addSpacer(length-xInit, 2);
	gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim);
	gui1->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);

    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM));
	gui1->addButton("DRAW GRID", false, dim, dim);
	gui1->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM));
	gui1->addToggle( "D_GRID", false, dim, dim);

    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("RANGE SLIDER", OFX_UI_FONT_MEDIUM));
	gui1->addRangeSlider("RSLIDER", 0.0, 255.0, 50.0, 100.0, length-xInit,dim);

    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM));
	gui1->add2DPad("PAD", ofPoint(0,length-xInit), ofPoint(0,120), ofPoint((length-xInit)*.5,120*.5), length-xInit,120);
*/
}
/*
void ofApp::setGUI2()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;

    img = new ofImage();
    img->loadImage("nerd_me.png");
    buffer = new float[256];
    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }

    gui2 = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight());
	gui2->addWidgetDown(new ofxUILabel("PANEL 2: ADVANCED", OFX_UI_FONT_LARGE));

    gui2->addSpacer(length-xInit, 2);
	gui2->addWidgetDown(new ofxUILabel("TEXT INPUT", OFX_UI_FONT_MEDIUM));
	gui2->setWidgetFontSize(OFX_UI_FONT_LARGE);
	gui2->addTextInput("TEXT INPUT", "Input Text", length-xInit);
    gui2->addWidgetDown(new ofxUILabel("AUTO CLEAR DISABLED", OFX_UI_FONT_MEDIUM));
    gui2->addTextInput("TEXT INPUT2", "Input Text", length-xInit)->setAutoClear(false);
	gui2->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("WAVEFORM DISPLAY", OFX_UI_FONT_MEDIUM));
	gui2->addWidgetDown(new ofxUIWaveform(length-xInit, 64, buffer, 256, 0.0, 1.0, "WAVEFORM"));
    gui2->addWidgetDown(new ofxUILabel("SPECTRUM DISPLAY", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUISpectrum(length-xInit, 64, buffer, 256, 0.0, 1.0, "SPECTRUM"));

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("IMAGE DISPLAY", OFX_UI_FONT_MEDIUM));
	gui2->addWidgetDown(new ofxUIImage(img->width*.5, img->height*.5, img, "IMAGE CAPTION"));

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("FPS LABEL", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM));

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("NUMBER DIALER", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUINumberDialer(-10000, 10000, 5000, 1, "NUMBER DIALER", OFX_UI_FONT_MEDIUM));

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("LABEL BUTTON", OFX_UI_FONT_MEDIUM));
    gui2->addLabelButton("LABEL BUTTON", false, length-xInit);
    gui2->addLabelButton("AUTOSIZE BUTTON", false);
    gui2->addLabelButton("MEDIUM BUTTON", false);

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("LABEL TOGGLES", OFX_UI_FONT_MEDIUM));
    gui2->addLabelToggle("LABEL TOGGLE", false, length-xInit);
    gui2->addLabelToggle("AUTOSIZE TOGGLE", false);
    gui2->addLabelToggle("MEDIUM TOGGLE", false);

	ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
}

void ofApp::setGUI3()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui3 = new ofxUICanvas(length*2+xInit*2+4, 0, length+xInit, ofGetHeight());
    gui3->addWidgetDown(new ofxUILabel("PANEL 3: ADVANCED", OFX_UI_FONT_LARGE));

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("MATRIX", OFX_UI_FONT_MEDIUM));
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim, dim, 4, 4, "MATRIX1"));
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*2.0, dim*2.0, 3, 6, "MATRIX2"));
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*3.0, dim*3.0, 1, 4, "MATRIX3"));

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("IMAGE BUTTON", OFX_UI_FONT_MEDIUM));
    gui3->addWidgetDown(new ofxUIImageButton(dim*4.0, dim*4.0, false, "GUI/images/App.png","IMAGEBTN"));
    gui3->addWidgetDown(new ofxUILabel("IMAGE TOGGLE", OFX_UI_FONT_MEDIUM));
    gui3->addWidgetDown(new ofxUIImageToggle(dim*4.0, dim*4.0, false, "GUI/images/Preview.png","IMAGETGL"));

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("ROTARY SLIDER", OFX_UI_FONT_MEDIUM));
    gui3->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 100, 50, "R2SLIDER"));
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("DROP DOWN", OFX_UI_FONT_MEDIUM));

    vector<string> items;
    items.push_back("FIRST ITEM");
    items.push_back("SECOND ITEM");
    items.push_back("THIRD ITEM WHATS UP DOG");
    items.push_back("FOURTH ITEM");
    items.push_back("FIFTH ITEM");
    items.push_back("SIXTH ITEM");

    gui3->addDropDownList("DROP DOWN LIST", items, 200);

	ofAddListener(gui3->newGUIEvent,this,&ofApp::guiEvent);
}

void ofApp::setGUI4()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE));

    gui4->addSpacer(length-xInit, 2);

    gui4->addWidgetDown(new ofxUILabel("BILABEL SLIDER", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));

    gui4->addWidgetDown(new ofxUILabel("MINIMAL SLIDER", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim, 0, 100, 50.0, "MINIMAL",OFX_UI_FONT_MEDIUM));

    gui4->addSpacer(length-xInit, 2);

    gui4->addWidgetDown(new ofxUILabel("CIRCLE SLIDER", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "NORTH SOUTH", OFX_UI_FONT_MEDIUM));

    gui4->addSpacer(length-xInit, 2);
    gui4->addWidgetDown(new ofxUILabel("FPS SLIDER", OFX_UI_FONT_MEDIUM));
    gui4->addFPSSlider("FPS SLIDER", length-xInit, dim);

    vector<float> buffer;
    for(int i = 0; i < 256; i++)
    {
        buffer.push_back(0.0);
    }

    gui4->addWidgetDown(new ofxUILabel("MOVING GRAPH", OFX_UI_FONT_MEDIUM));
    mg = (ofxUIMovingGraph *) gui4->addWidgetDown(new ofxUIMovingGraph(length-xInit, 120, buffer, 256, 0, 400, "MOVING GRAPH"));

    gui4->addSpacer(length-xInit, 2);
    gui4->addWidgetDown(new ofxUILabel("IMAGE SAMPLER", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIImageSampler(img->getWidth(), img->getHeight(), img, "SAMPLER"));
    gui4->addWidgetDown(new ofxUIMultiImageButton(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
    gui4->addWidgetDown(new ofxUIMultiImageToggle(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));


	ofAddListener(gui4->newGUIEvent,this,&ofApp::guiEvent);
}*/
