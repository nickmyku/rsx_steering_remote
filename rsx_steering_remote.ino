//code which allows steering wheel mounted cruise control remote to
//control custom stereo audio play back, also retains orginal cruis control
//functionality. toggle switch has been added to choose between cruise 
//controls and stereo controls
//
//Written by: Nicholas Mykulowycz
//Created on: June 4, 2014

//character definitions
const char NEXT    = 215;  //right key
const char PLAY    = ' ';  //space key
const char VOL_UP  = 218;  //up key
const char VOL_DN  = 217;  //down key
const char ESCAPE  = 177;  //escape key

//pin definitions
#define CRUISE_6 8  //input pin to cruise control module on pin 6
#define CRUISE_7 16  //input pin to cruise control module on pin 7
#define A_DATA A3  //analog input pin

//globals
int val = 0;  //value of analog pin 
const int ANALOG_ERROR =  15;  //range to adjust for noise in analog signal
const int IDLE_DELAY   = 100;  //delay time for loop execution
const int WRITE_DELAY  = 150;  //time that output pin states are held for cruise control outputs
const int KEY_DELAY    = 175;
const int HOLD_TIME    = 600;  //time which needs to pass before button held event is registered
const int RESUME_V     = 208;  //3.85V
const int CANCEL_V     = 300;  //3.00V
const int SET_V        = 385;  //2.08V
const int UP_V         =  40;  //1.46V
const int NEXT_V       =  78;  //0.78V
const int DN_V         = 146;  //0.40V
const int ESC_V        =  57;  //0.57V

void setup()  {
  //define input output pins
  pinMode(CRUISE_6, OUTPUT);
  pinMode(CRUISE_7, OUTPUT);
  pinMode(A_DATA, INPUT);
  
  //set default pin states
  digitalWrite(CRUISE_6, HIGH);
  digitalWrite(CRUISE_7, HIGH);
  
  //initialize control over the keyboard:
  Keyboard.begin();
  
}

void loop()  {
  boolean temp = false;
  //read the analog pin
  val = getVoltage();
  //if analog value is less then 5V then a button has been pressed
  if(!inRange(val, 500))  {
    //resume button pressed
    if(inRange(val, RESUME_V)){
      resumeButton();
    }
    //cancel button pressed
    if(inRange(val, CANCEL_V)){
      cancelButton();
    }
    //set button pressed
    if(inRange(val, SET_V)){
      setButton();
    }
    //escape buttons pressed, combination of down and next key
    //if(inRange(val, ESC_V)){
    //  escapeKey();
    //}
    //up button pressed
    if(inRange(val, UP_V)){
      upKey();
    }
    //down button pressed
    if(inRange(val, DN_V)){
      downKey();
    }
    //next button pressed
     if(inRange(val, NEXT_V)){
      nextKey();
    } 
  }
  else {
    delay(IDLE_DELAY);
  }
  
}

//function to check whether analog value is within analog error range
boolean inRange(int actual_val, int target_val)  {
  
  if((actual_val < (target_val+ANALOG_ERROR)) && (actual_val > (target_val-ANALOG_ERROR)))  {
    return true;
  }
  else  {
    return false;
  }
}

//function to emulate resume button presse 
void resumeButton()  {
  //set output values
  digitalWrite(CRUISE_6, HIGH);
  digitalWrite(CRUISE_7, LOW);
  
  //delay until button released
  while(inRange(val, RESUME_V)){
      val = getVoltage();
      delay(WRITE_DELAY);
  }
  
  //set outputs back to default
  digitalWrite(CRUISE_6, HIGH);
  digitalWrite(CRUISE_7, HIGH);
}

//function to emulate cancel button press
void cancelButton()  {
  //set output values
  digitalWrite(CRUISE_6, LOW);
  digitalWrite(CRUISE_7, LOW);
  
  //delay until button released
  while(inRange(val, CANCEL_V)){
      val = getVoltage();
      delay(WRITE_DELAY);
  }
  
  //set outputs back to default
  digitalWrite(CRUISE_6, HIGH);
  digitalWrite(CRUISE_7, HIGH);
}

//function to emulate set button press
void setButton()  {
  //set output values
  digitalWrite(CRUISE_6, LOW);
  digitalWrite(CRUISE_7, HIGH);
  
  //delay until button released
  while(inRange(val, SET_V)){
      val = getVoltage();
      delay(WRITE_DELAY);
  }
  
  //set outputs back to default
  digitalWrite(CRUISE_6, HIGH);
  digitalWrite(CRUISE_7, HIGH);
}

//function to send escape/exit command to computer
void escapeKey()  {
  int curr_time = 0;
  int refresh_delay = 50;  //pause 50ms before checking analog value again
  
  //check to see how long button is held
  while(inRange(val, ESC_V) && curr_time<HOLD_TIME){
    val = getVoltage();
    delay(refresh_delay);
    curr_time += refresh_delay;
  }
  //if button was held send escape command
  if(curr_time>=HOLD_TIME)  {
    Keyboard.print(ESCAPE);
    delay(KEY_DELAY*5);
  }
}

//function to send volume up command to computer
void upKey()  {
  Keyboard.print(VOL_UP);
  delay(KEY_DELAY);
}

//function to send volume down command to computer
void downKey()  {
  Keyboard.print(VOL_DN);
  delay(KEY_DELAY);
}

//function to send next command to computer, or play command if button was held
void nextKey()  {
  int curr_time = 0;
  int refresh_delay = 50;  //pause 50ms before checking analog value again
  
  //check to see how long button is held
  while(inRange(val, NEXT_V) && curr_time<HOLD_TIME){
    val = getVoltage();
    delay(refresh_delay);
    curr_time += refresh_delay;
  }
  
  //if its below the button hold threshold then go to next song
  if(curr_time < HOLD_TIME){
    Keyboard.print(NEXT);
    while(inRange(val, NEXT_V)){
      val = getVoltage();
      delay(150);
    }
  }
  //if it is greater then hold threashold play/pause song
  else  {
    Keyboard.print(PLAY);
    //send command then pause to prevent infinite loop of play pause
    while(inRange(val, NEXT_V)){
      val = getVoltage();
      delay(refresh_delay);
    }
  }
}

int getVoltage()  {
  int value = 0;
  int value_0 = 0;
  int value_1 = 0;
  int value_2 = 0;
  //read analog pin
  value_0 = analogRead(A_DATA);
  delay(10);
  value_1 = analogRead(A_DATA);
  delay(10);
  value_2 = analogRead(A_DATA);
  //remap output from 0 to 500 (0 to 5.00V)
  value_0 = map(value_0, 0, 1023, 0, 500);
  value_1 = map(value_1, 0, 1023, 0, 500);
  value_2 = map(value_2, 0, 1023, 0, 500);
  //averagw values
  value = value_0 + value_1 + value_2;
  value /= 3;
  //output remapped value
  return value;
}
