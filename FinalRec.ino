#include <RH_ASK.h>
#include <SPI.h>


 //-----------Wheel Pins-------------//
 const int rightFwdPin = 7;   
 const int rightBwdPin = 4;
 const int rightSpdPin = 6;
 
 const int leftFwdPin = 9;
 const int leftBwdPin = 8;
 const int leftSpdPin = 5;

 

 int rightWay, leftWay;  // direction for each wheel 1 for foward -1 for backward & 0 for no motion

 //----------controller------------//
 RH_ASK driver;
 const int codePin = 11;
                                     
 //---------UltraSonic Pins--------//

 boolean safety;
 const int pad = 5; // buffer for safety mode
 const int safePin = 13;
 
 const int pingFrontPin = A1; 
 const int echoFrontPin = A2;
 
 const int pingBackPin = A4;
 const int echoBackPin = A3;

 //----------Encoders-------------//


 int RSPD1 = 120;        
 int LSPD1 = 120;
 int RSPD2 = 120;
 int LSPD2 = 120; 


 int gain = 25;
 int delCntr=0; 
 volatile long cntrL, cntrR;
 volatile long LIntTime, RIntTime;
 
 
 //---------Sound & lights-------//
    boolean horn;
    boolean dark;
    
    const int lights = 12;
    const int sound = 10;
    
 
 //--------AutoPilot---------------//

    boolean autoPilot;
     

 //--------Code-------------------//
 


void setup() 
{
  pinMode(leftFwdPin,OUTPUT);
  pinMode(leftBwdPin,OUTPUT);
  pinMode(leftSpdPin,OUTPUT);
  
  pinMode(rightFwdPin,OUTPUT);
  pinMode(rightBwdPin,OUTPUT);
  pinMode(rightSpdPin,OUTPUT);

  

  pinMode(echoFrontPin, INPUT);
  pinMode(pingFrontPin, OUTPUT);
  pinMode(echoBackPin, INPUT);
  pinMode(pingBackPin, OUTPUT);
  
  driver.init();
  
  Serial.begin(9600);

  cntrR = 0;
  cntrL = 0;
  LIntTime = 0;
  RIntTime = 0;
}

void loop() {
  uint8_t buf[12];
  if (driver.recv(buf, 12)) {
    
    String x = ((String)(char*)buf);
    Serial.println(x);
    speedChange();
    x.trim();

    //------Right Wheel-----//
    
    if(x == "Right Foward") {
      rightWay = 1;
    } 
    if (x == "Right Back") {
      
      rightWay = -1;
    } 
    if (x == "Right Stop") {
       rightWay = 0;
    } 

    //------Left Wheel-----//
    
    if (x == "Left Foward") {
      leftWay = 1;
    }
    if (x == "Left Back") {
      leftWay = -1;
    } 
    if (x == "Left Stop") {
      leftWay = 0;
    } 

    //------Buttons-------//
    
    if (x == "Blue"){
      safety = !safety;
      digitalWrite(safePin, safety);
    } 
    
    if (x == "Green") {
      horn = !horn;
      if (horn) {
        tone(sound, 800);
      } else {
        noTone(sound);
      }
    }

    if (x == "Yellow") { // autopilot tells the robot to go foward if there is no object and then turn left if there is
      autoPilot = !autoPilot;
    }

    //------Yellow------//
    
    if (x == "Light High") { // if the controller is in light room  headlights are off 
      digitalWrite(lights, LOW);
    }
    if(x == "Light Low") { // if the controller is in dark room headlights are on
      digitalWrite(lights, HIGH);
    }
  } 
  
  if (!autoPilot) {
     speedChange();
     if (rightWay == 1 && leftWay == 1) { // foward movement
     
        if (scanF() < pad && safety) {
         // object less than 5 inches stops wheels from going foward but allows other movement
           leftStop();
           rightStop();
        } else {
           rightFoward();
           leftFoward();
        }  
      } else if (rightWay == -1 && leftWay == -1) { // foward movement
         if (scanB() < pad && safety) {
         // object less than 5 inches stops wheels from going backwards but allows other movement
           leftStop();
           rightStop();
         } else {
           rightBackward();
           leftBackward();
         }
       }  else if (rightWay == 0 && leftWay == 1) {  
           rightStop();
           leftFoward();
       } else if (rightWay == 0 && leftWay == -1) { 
           leftBackward();
           rightStop();
    
       } else if(rightWay == -1 && leftWay == 1) {
           rightBackward();
           leftFoward();
       } else if (rightWay == -1 && leftWay == 0){
           rightBackward();
           leftStop();
       } else if (rightWay == 1 && leftWay == 0) {
           rightFoward();
           leftStop(); 
       } else if (rightWay == 1 && leftWay == -1) {
           rightFoward();
           leftBackward();
       } else {
           rightStop();
           leftStop();
       }
  } else {
     if(scanF() < 5) {
        rightFoward();
        leftBackward();
     } else {
        rightFoward();
        leftFoward();
     }
  }



 
  

  
}

void rightFoward() {
    digitalWrite(rightFwdPin, HIGH);
    digitalWrite(rightBwdPin, LOW);
    analogWrite(rightSpdPin, RSPD2);
    
}

void leftFoward() {
    digitalWrite(leftFwdPin, HIGH);
    digitalWrite(leftBwdPin, LOW);
    analogWrite(leftSpdPin, LSPD2);
    
  
}

void rightBackward() {
    digitalWrite(rightFwdPin, LOW);
    digitalWrite(rightBwdPin, HIGH);
    analogWrite(rightSpdPin, RSPD2);
    
}

void leftBackward() {
    digitalWrite(leftFwdPin, LOW);
    digitalWrite(leftBwdPin, HIGH);
    analogWrite(leftSpdPin, LSPD2);

}

void leftStop() {
  digitalWrite(leftFwdPin, LOW);
  digitalWrite(leftBwdPin, LOW);
  digitalWrite(leftSpdPin, 0);

}


void rightStop() {
  digitalWrite(rightFwdPin, LOW);
  digitalWrite(rightBwdPin, LOW);
  digitalWrite(rightSpdPin, 0);

}

void speedChange() { // updates the speed based on the encoder values
  long tmpLcntr, tmpRcntr;


  tmpLcntr = cntrL;
  tmpRcntr = cntrR;
  
  if(tmpLcntr < tmpRcntr) { // left needs more power
    RSPD2 = RSPD1 - gain;
    LSPD2 = LSPD1 + gain;
  } else if(tmpLcntr > tmpRcntr) {// right needs more power
    RSPD2 = RSPD1 + gain;
    LSPD2 = LSPD1 - gain;
  } else { // equal speed for both (unicorn scenario) 
    LSPD2 = LSPD2;
    RSPD2 = RSPD2; 

  }

  
}


void leftWhlCnt() { // counts the left wheels speed
  long intTime = micros();
  if(intTime > LIntTime + 1000L)
  {
    LIntTime = intTime;
    cntrL++;
  }

}

void rightWhlCnt() { // counts the right wheels speed
  long intTime = micros();
  if(intTime > RIntTime + 1000L)
  {
    RIntTime = intTime;
    cntrR++;
  }
}


long scanF() { // scans for objects in the front of the car
  digitalWrite(pingFrontPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingFrontPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingFrontPin, LOW);
  long duration = pulseIn(echoFrontPin, HIGH);

  return duration / 74 / 2;
}

long scanB() { // scans for object in the back of the car
  digitalWrite(pingBackPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingBackPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingBackPin, LOW);
  long duration = pulseIn(echoBackPin, HIGH);
  return duration / 74 / 2;
}
