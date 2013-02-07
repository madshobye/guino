/*
  GUINO DASHBOARD TEMPLATE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 Connect potmeter to analog in, ground and plus 5v.
 Connect diode to pin 11 and ground.
 
 # This is your main template to edit.
 */

// Dummy example variables

int amplitude = 20;
int width = 0;
int graphValue = 0;
int potMeter = 0;
float angle = 0;

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  // Start the guino dashboard interface.
  // The number is your personal key for saving data. This should be unique for each sketch
  // This key should also be changed if you change the gui structure. Hence the saved data vill not match.
  gBegin(34236); 


}


void loop(){
  // **** Main update call for the guino
  guino_update();
  
  // Radians 0 to 6.28
  angle = angle  +(1.0f/(float)width);
  if(angle > 6.28)
  {
    angle = 0;
  }
  graphValue = round((sin(angle)+1) * (float)amplitude);
  
 
  // Read analog
  potMeter = analogRead(0);
  // Attach a led to pin 11 to control it.
  analogWrite(11,graphValue);

  // Send the value to the gui.
  gUpdateValue(&potMeter);
  gUpdateValue(&graphValue);
 // gUpdateValue(&angle);
  
  


}

// This is where you setup your interface 
void gInit()
{
  gAddLabel("SLIDERS",1);
  gAddSpacer(1); 

  
  gAddSlider(3,127,"AMPLITUDE", &amplitude);


  gAddColumn();

  gAddLabel("GRAPHS",1);
  gAddSpacer(1);

  // Last parameter in moving graph defines the size 10 = normal
  gAddMovingGraph("SINUS",0,255, &graphValue, 10);


  gAddMovingGraph("POTMETER",0,1023, &potMeter, 10);
  
  gAddSlider(0,1023,"POTMETER",&potMeter);

}

// Method called everytime a button has been pressed in the interface.
void gButtonPressed(int id)
{
 
}

void gItemUpdated(int id)
{

}








