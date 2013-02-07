/*
  GUINO DASHBOARD TEMPLATE HEADER FILE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Includable Arduino library file created by domints.
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 # This is the Guino Protocol Library should only be edited if you know what you are doing.
*/

#ifndef Guino_h
#define Guino_h

#ifndef BAUD
#define BAUD 115200
#endif

#include <EasyTransfer.h>
#include <EEPROM.h>

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
#define guino_addMovingGraph 13
#define guino_buttonPressed 14
#define guino_addChar 15
#define guino_setMin 16
#define guino_setMax 17
#define guino_setValue 20
#define guino_addLabel 12
#define guino_large 0
#define guino_medium 1
#define guino_small 2
#define guino_setColor  21

//These three functions have to be at least declared in your sketch. What you do with them is up to you, byt they have to be declared.
void gInit();
void gButtonPressed(int id);
void gItemUpdated(int id);

//This function will write a 2 char integer to the eeprom at the specified address and address + 1
//DO NOT USE IN SKETCH!!!
void EEPROMWriteInt(int p_address, int p_value);

//This function will read a 2 char integer from the eeprom at the specified address and address + 1
//DO NOT USE IN SKETCH!!!
unsigned int EEPROMReadInt(int p_address);

//This function need to be run every loop, updates all Guino objects
void guino_update();

//This function initializes EEPROM memory by checking of existence and inputting EEPROM key.
//DO NOT USE IN SKETCH!!!
void gInitEEprom();

//This function sets the background color of the Guino desktop app
//Variable names are self-explanatory.
void gSetColor(int _red, int _green, int _blue);

//By this function you can get value saved in EEPROM.
// int item_number - ID of saved data
// int *_variable - variable in which the data will be saved after read
void gGetSavedValue(int item_number, int *_variable);

//This function inits EEPROM and the rest of Guino interface.
//DO NOT USE IN SKETCH!!!
void gBegin(int _eepromKey);

//This function adds the button to the dashboard
// char * _name - name of the button displayed in the dashboard
// return int - ID of the button
// USE IN gInit() FUNCTION!!!
int gAddButton(char * _name);

//This function just adds the column to the dashboard.
// USE IN gInit() FUNCTION!!!
void gAddColumn();

//This function adds text label to Dashboard.
//char * _name - text displayed
//int _size - not sure, have to ask author, when put 1 or 2 works, but I can't see difference, when put 5 app crashes ;)
//return int - ID of the label
// USE IN gInit() FUNCTION!!!
int gAddLabel(char * _name, int _size);

//This function adds simple horizontal line to Dashboard.
//int _size - not sure, have to ask author, when put 1 or 2 works, but I can't see difference, when put 5 app crashes ;)
//return int - ID of the label
// USE IN gInit() FUNCTION!!!
int gAddSpacer(int _size);

//This function adds unknown to me type of something
//char * _name - label of this thing
//int * _variable - variable storing value of this
//return int - ID of this thing
// USE IN gInit() FUNCTION!!!
int gAddToggle(char * _name, int * _variable);

//This adds some type of graph
//meaning of variables doesn't seem to mean a lot to me
//return int - ID of this graph
// USE IN gInit() FUNCTION!!!
int gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size);

//This adds some type of graph
//meaning of variables doesn't seem to mean a lot to me
//return int - ID of this graph
// USE IN gInit() FUNCTION!!!
int gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size);

//This function allows you to change the label text
//TO VERIFY BY AUTHOR: Probably can change text of other items than just label, like slider or button
//int _item - ID of label which text have to be changed
//char * _text - new label text
//return int - WRONG DEFINITION, DOESN'T SEEM TO RETURN ANYTHING AT ALL!!!
int gUpdateLabel(int _item, char * _text);

//This function adds rotary slider to dashboard
//int _min - minimum value of the slider
//int _max - maximum value of the slider
//char * _name - label of the slider
//int * _variable - variable containing value of slider
//return int - ID of this slider
// USE IN gInit() FUNCTION!!!
int gAddRotarySlider(int _min,int _max, char * _name, int * _variable);

//This function adds linear slider to dashboard
//int _min - minimum value of the slider
//int _max - maximum value of the slider
//char * _name - label of the slider
//int * _variable - variable containing value of slider
//return int - ID of this slider
// USE IN gInit() FUNCTION!!!
int gAddSlider(int _min,int _max, char * _name, int * _variable);


//Not sure what to do with those two, I'd merge them into one which would ask for item ID and value once, but I can be wrong about this what they are for
void gUpdateValue(int _item);
void gUpdateValue(int * _variable);

//This function sends a command to the dashboard, rather do not use it in sketch.
void gSendCommand(char _cmd, char _item, int _value);

#endif