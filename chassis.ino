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
//#define D_EBUG
PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
int _speed = 0;
//motor wires
#define L_in1 5
#define L_in2 6
#define R_in1 9
#define R_in2 10
const int speedPinL=7,speedPinR=8;
void motorControl(int, int);
void setup(){
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
  pinMode(L_in1, OUTPUT);
  pinMode(L_in2, OUTPUT);
  pinMode(R_in1, OUTPUT);
  pinMode(R_in2, OUTPUT);
  pinMode(speedPinL,OUTPUT);
  pinMode(speedPinR,OUTPUT);
}

void loop(){
  if(error==1)
    return;
  if(type==2)
    return;
  else{
    int LY=ps2x.Analog(PSS_LY);
    int LX=ps2x.Analog(PSS_LX);
    motorControl(LX,LY);  
  }
}

//chassis motor control functions
void turnLeft(){     
  digitalWrite(L_in1,HIGH);
  digitalWrite(L_in2, LOW);
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2,HIGH);
//  delay(20);
}
void turnRight(){
   digitalWrite(L_in1, LOW);
   digitalWrite(L_in2, HIGH);
   digitalWrite(R_in1, HIGH);
   digitalWrite(R_in2, LOW);
//   delay(20);     
}
 void forward(){
  digitalWrite(L_in1, HIGH);
  digitalWrite(L_in2, LOW);
  digitalWrite(R_in1, HIGH);
  digitalWrite(R_in2, LOW);  
//  delay(20);  
}
void _stop(){
  digitalWrite(L_in1, LOW);
  digitalWrite(L_in2, LOW);
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2, LOW);
//  delay(20);
}
void back(){
  digitalWrite(L_in1, LOW);
  digitalWrite(L_in2, HIGH);
  digitalWrite(R_in1, LOW);
  digitalWrite(R_in2, HIGH);
//  delay(20);
}
void motorControl(int lx,int ly){
  if(lx<128){
    _speed=2*(127-lx);
    analogWrite(speedPinL,_speed);
    analogWrite(speedPinR,_speed);
    turnLeft();
  } else if(lx>128){
    _speed=2*(lx-127);
    analogWrite(speedPinL,_speed);
    analogWrite(speedPinR,_speed);
    turnRight();
  }
  if(ly<128){
    _speed=2*(127-ly);
    analogWrite(speedPinL,_speed);
    analogWrite(speedPinR,_speed);
    forward();
  } else if(ly>128){
    _speed=2*(ly-127);
    analogWrite(speedPinL,_speed);
    analogWrite(speedPinR,_speed);
    back();
  }
  if(ly>=128 && ly<=128 && lx>=128 && lx<=128){
    _stop();
  }
}
//chassis motor control functions end
