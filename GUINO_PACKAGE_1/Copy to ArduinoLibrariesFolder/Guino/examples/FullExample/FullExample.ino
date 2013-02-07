/*
  GUINO DASHBOARD TEMPLATE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 # This is your main template to edit.
 */

// Dummy example variables
int size = 0;
int width = 230;
int amplitude = 20;
int ledLight = 0;
int max = 0;
int min = 0;
int pause = 0;
int buttonId = 0;
int r = 75;
int g = 75;
int b = 110;
int graphValue = 0;
int fixedGraph = 0;

int rotaryRID,rotaryGID,rotaryBID;
int flexLabelId =0;
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

  fixedGraph = random(200);
  //Pause button on gui stop the program for 2 seconds.
  if(pause)
  {
    gUpdateLabel(flexLabelId, "YOU PAUSED THE PROGRAM");
    delay(2000);
    gUpdateLabel(flexLabelId, "PROGRAM RUNNING AGAIN");
    pause = false;
    gUpdateValue(&pause);
  }

  // Update the graph value
    // Radians 0 to 6.28
  angle = angle  +(1.0f/(float)width);
  if(angle > 6.28)
  {
    angle = 0;
  }
  graphValue = round((sin(angle)+1) * (float)amplitude);
  
  
  // Attach a led to pin 11 to control it.
  analogWrite(11,graphValue);

  // Led at pin 13 blinks according to the sine wave below.
  digitalWrite(13,graphValue > 0);

  // Send the value to the gui.
  gUpdateValue(&graphValue);
  gUpdateValue(&fixedGraph);


}

// This is where you setup your interface 
void gInit()
{

  gAddLabel("SLIDERS",1);
  gAddSpacer(1);
  gAddSlider(3,200,"WIDTH",&width);
   gAddSlider(3,127,"AMPLITUDE", &amplitude);

  gAddSlider(0,255,"LED BRIGHTNESS",&ledLight); 
  // The rotary sliders 
  gAddLabel("ROTARY SLIDERS",1);
  gAddSpacer(1);

  rotaryRID = gAddRotarySlider(0,255,"R",&r);
  rotaryGID = gAddRotarySlider(0,255,"G",&g);
  rotaryBID = gAddRotarySlider(0,255,"B",&b);

  gAddLabel("BUTTONS",1);
  gAddSpacer(1);
  buttonId = gAddButton("HEIGHT TO 100"); 
  gAddToggle("PAUSE",&pause);
  gAddSpacer(1);


  gAddSpacer(1);
  flexLabelId = gAddLabel("LIVE LABEL",2);
  gAddSpacer(1);

  gAddColumn();

  gAddLabel("GRAPHS",1);
  gAddSpacer(1);

  // Last parameter in moving graph defines the size 10 = normal
  gAddMovingGraph("SINUS",0,255, &graphValue, 10);
  gAddSlider(-100,100,"VALUE",&graphValue);
  gAddFixedGraph("FIXED GRAPH",-100,100,100,&fixedGraph,10);
  // The graphs take up two columns we are going to add two
  gAddColumn();

  // Add more stuff here.
  gSetColor(r,g,b); // Set the color of the gui interface.
}

// Method called everytime a button has been pressed in the interface.
void gButtonPressed(int id)
{
  if(buttonId == id)
  {
    amplitude= 100; 
    gUpdateValue(&amplitude);
  }
}

void gItemUpdated(int id)
{
  if(rotaryRID == id || rotaryGID == id || rotaryBID == id)
  {
    gSetColor(r,g,b);
  }
}








