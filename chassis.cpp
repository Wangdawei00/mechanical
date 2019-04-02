#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false
#define D_EBUG
PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
int _speed = 0;
#define NUM_OF_LEGS 4
//motor wires
//#define legs[0] 5
//#define L_in2 6
//#define R_in1 3
//#define legs[3] 9
const int legs[NUM_OF_LEGS]={5,6,3,9};
//const int speedPinL=7,speedPinR=8;
void motorControl(int, int,bool);
bool speedMode =true;
void setup(){
  Serial.begin(9600);
  delay(500);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
#ifdef D_EBUG
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  } 
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
#endif
  type = ps2x.readType();
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
  pinMode(legs[0], OUTPUT);
  pinMode(legs[1], OUTPUT);
  pinMode(legs[2], OUTPUT);
  pinMode(legs[3], OUTPUT);
//  pinMode(speedPinL,OUTPUT);
//  pinMode(speedPinR,OUTPUT);
}

void loop(){
//  Serial.print("error:");
//  Serial.println(error);
//  Serial.print("Ty:");
//  Serial.println(error);
  if(error!=0)
    return;
  if(type!=1)
    return;
  else{
    ps2x.read_gamepad(false, vibrate);
    if(ps2x.Button(PSB_R3)){
      speedMode=(!speedMode);
    }
    Serial.print("x:");
    Serial.println(ps2x.Analog(PSS_LX),DEC);
    int LY=ps2x.Analog(PSS_LY);
    int LX=ps2x.Analog(PSS_LX);
    motorControl(LX,LY,speedMode);
  }
}

//chassis motor control functions
/*void turnLeft(){     
//  analogWrite(L_in1,_speed);
//  analogWrite(L_in2, 0);
  analogWrite(R_in1, 0);
  analogWrite(legs[3],_speed);
  delay(20);
}
void turnRight(){
//   analogWrite(L_in1, 0);
//   analogWrite(L_in2, _speed);
   analogWrite(R_in1, _speed);
   analogWrite(R_in2, 0);
   delay(20);     
}
 void forward(){
//  analogWrite(L_in1, _speed);
//  analogWrite(L_in2, 0);
  analogWrite(R_in1, _speed);
  analogWrite(R_in2, 0);  
  delay(20);  
}
void _stop(){
  analogWrite(L_in1, 0);
  analogWrite(legs[1], 0);
  analogWrite(R_in1, 0);
  analogWrite(R_in2, 0);
  delay(20);
}
void back(){
//  analogWrite(L_in1, 0);
//  analogWrite(L_in2, _speed);
  analogWrite(R_in1, 0);
  analogWrite(R_in2, _speed);
  delay(20);
}*/
void forward(bool speedMode) {
  if(speedMode){
    analogWrite(legs[0], 255);
    analogWrite(legs[1], 0);
    analogWrite(legs[2], 0);
    analogWrite(legs[3], 255);
  }else{
    analogWrite(legs[0], 25);
    analogWrite(legs[1], 0);
    analogWrite(legs[2], 0);
    analogWrite(legs[3], 25);
  }
}

void backward(bool speedMode) {
  if(speedMode){
    analogWrite(legs[0], 0);
    analogWrite(legs[1], 255);
    analogWrite(legs[2], 255);
    analogWrite(legs[3], 0);
  }else{
    analogWrite(legs[0], 0);
    analogWrite(legs[1], 50);
    analogWrite(legs[2], 50);
    analogWrite(legs[3], 0);
  }
}

void turnLeft(bool speedMode) {
  if(speedMode){
  analogWrite(legs[0], 255);
  analogWrite(legs[1], 0);
  analogWrite(legs[2], 255);
  analogWrite(legs[3], 0);}
  else{
    analogWrite(legs[0], 50);
  analogWrite(legs[1], 0);
  analogWrite(legs[2], 50);
  analogWrite(legs[3], 0);
  }
}

void turnRight(bool speedMode) {
  if(speedMode){
  analogWrite(legs[0], 0);
  analogWrite(legs[1], 255);
  analogWrite(legs[2], 0);
  analogWrite(legs[3], 255);}
  else{
    analogWrite(legs[0], 0);
  analogWrite(legs[1], 50);
  analogWrite(legs[2], 0);
  analogWrite(legs[3], 50);
  }
}

void _stop() {
  for(int i=0;i<NUM_OF_LEGS;i++){
    analogWrite(legs[i],0);
  }
}
void motorControl(int lx,int ly, bool speedMode){
  if(lx<30){
//    _speed=2*(127-lx);
    turnLeft(speedMode);
  } else if(lx>250){
//    _speed=2*(lx-127);
    turnRight(speedMode);
  }
  if(ly<30){
//    _speed=2*(127-ly);
    forward(speedMode);     
  } else if(ly>250){
//    _speed=2*(ly-127);
    backward(speedMode);
  }
  if(ly>=30 && ly<=250 && lx>=30 && lx<=250){
    _stop();
  }
}
//chassis motor control functions end
