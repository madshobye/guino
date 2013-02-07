/*
  GUINO DASHBOARD TEMPLATE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 # This is the Guino Protocol Library should only be edited if you know what you are doing.
 */

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


boolean guidino_initialized = false;

//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char cmd;
  char item;
  int value;
};

// Find a way to dynamically allocate memory
int guino_maxGUIItems = 100;
int guino_item_counter = 0;
int *guino_item_values[100]; 
int gTmpInt = 0; // temporary int for items without a variable
boolean internalInit = true; // boolean to initialize before connecting to serial

// COMMAND STRUCTURE

//give a name to the group of data
SEND_DATA_STRUCTURE guino_data;
int eepromKey = 1234;
void guino_update()
{

  while(Serial.available())
  {

    if(ET.receiveData())
    {
      switch (guino_data.cmd) 
      {
      case guino_init:

        guino_item_counter = 0;
        guidino_initialized = true;
        gInit();
        break;
      case guino_setValue:
        *guino_item_values[guino_data.item] = guino_data.value;
        guino_data.cmd = guino_executed;
        gItemUpdated(guino_data.item);
        break;
      case guino_buttonPressed:
        gButtonPressed(guino_data.item);
        break;
      case guino_saveToBoard:
        {

          gInitEEprom();
          for (int i =0; i < guino_item_counter;i++)
          {
            EEPROMWriteInt(i*2+2, *guino_item_values[i]);
          }
        }
        break;
      }
    }
  }
}

void gInitEEprom()
{
  if(EEPROMReadInt(0) != eepromKey)
  {
    EEPROMWriteInt(0, eepromKey);
    for (int i =1; i <guino_maxGUIItems;i++)
    {
      EEPROMWriteInt(i*2+2, -3276);
    }
  }

}

void gSetColor(int _red, int _green, int _blue)
{
  gSendCommand(guino_setColor, 0, _red);
  gSendCommand(guino_setColor, 1, _green);
  gSendCommand(guino_setColor, 2, _blue);
}

void gGetSavedValue(int item_number, int *_variable)
{

  if(EEPROMReadInt(0) == eepromKey && internalInit)
  {

    int tmpVar =  EEPROMReadInt((item_number)*2+2);
    if(tmpVar != -3276)
      *_variable = tmpVar;
  }

}

void gBegin(int _eepromKey)
{

  // Sets all pointers to a temporary value just to make sure no random memory pointers.
  for(int i = 0; i < guino_maxGUIItems; i++)
  {
    guino_item_values[i] = &gTmpInt;
  }
  eepromKey = _eepromKey;

  gInit(); // this one needs to run twice only way to work without serial connection.
  internalInit = false;
  Serial.begin(115200);
  ET.begin(details(guino_data), &Serial);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_executed, 0, 0);
  gSendCommand(guino_iamhere, 0, 0); 

}
int gAddButton(char * _name)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gSendCommand(guino_addButton,(byte)guino_item_counter,0);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }
    guino_item_counter++;
    return guino_item_counter-1;
  }
  return -1;
}


void gAddColumn()
{

  gSendCommand(guino_addColumn,0,0);

}




int gAddLabel(char * _name, int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  { 
    gSendCommand(guino_addLabel,(byte)guino_item_counter,_size);

    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;


} 
int gAddSpacer(int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gSendCommand(guino_addSpacer,(byte)guino_item_counter,_size);

    guino_item_counter++;
    return guino_item_counter-1;
  }
  return -1;

}   



int gAddToggle(char * _name, int * _variable)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addToggle,(byte)guino_item_counter,*_variable);

    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;

    return guino_item_counter-1;


  }
  return -1;
}   

int gAddFixedGraph(char * _name,int _min,int _max,int _bufferSize, int * _variable, int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gAddLabel(_name,guino_small);
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addWaveform,(byte)guino_item_counter,_size);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    gSendCommand(guino_setFixedGraphBuffer,(byte)guino_item_counter,_bufferSize);


    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;
}

int gAddMovingGraph(char * _name,int _min,int _max, int * _variable, int _size)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    gAddLabel(_name,guino_small);
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addMovingGraph,(byte)guino_item_counter,_size);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);


    guino_item_counter++;

    return guino_item_counter-1;
  }
  return -1;


}   


int gUpdateLabel(int _item, char * _text)
{

  gSendCommand(guino_clearLabel,(byte)_item,0);
  for (int i = 0; i < strlen(_text); i++){
    gSendCommand(guino_addChar,(byte)_item,(int)_text[i]);
  }



}



int gAddRotarySlider(int _min,int _max, char * _name, int * _variable)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addRotarySlider,(byte)guino_item_counter,*_variable);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;
    gUpdateValue(_variable);
    return guino_item_counter-1;
  }
  return -1;

}

int gAddSlider(int _min,int _max, char * _name, int * _variable)
{
  if(guino_maxGUIItems > guino_item_counter)
  {
    guino_item_values[guino_item_counter] =_variable ;
    gGetSavedValue(guino_item_counter, _variable);
    gSendCommand(guino_addSlider,(byte)guino_item_counter,*_variable);
    gSendCommand(guino_setMax,(byte)guino_item_counter,_max);
    gSendCommand(guino_setMin,(byte)guino_item_counter,_min);
    for (int i = 0; i < strlen(_name); i++){
      gSendCommand(guino_addChar,(byte)guino_item_counter,(int)_name[i]);
    }

    guino_item_counter++;
    gUpdateValue(_variable);
    return guino_item_counter-1;
  }
  return -1;

}

void gUpdateValue(int _item)
{

  gSendCommand(guino_setValue,_item, *guino_item_values[_item]); 
}


void gUpdateValue(int * _variable)
{

  int current_id = -1;
  for(int i = 0; i < guino_item_counter; i++)
  {

    if(guino_item_values[i] == _variable)
    {

      current_id = i;
      gUpdateValue(current_id);
    }
  }
  // if(current_id != -1)

}



void gSendCommand(byte _cmd, byte _item, int _value)
{
  if(!internalInit && (guidino_initialized || guino_executed || _cmd == guino_iamhere)  )
  {
    guino_data.cmd = _cmd;
    guino_data.item = _item;
    guino_data.value = _value;
    ET.sendData();
  }

}


