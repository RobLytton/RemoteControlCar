#include <RH_ASK.h>
#include <SPI.h>
int x1 = A3;
int y1 = A5;
int s1 = 7;
int x2 = A2;
int y2 = A4;
int s2 = 2;
int b1 = 4;
int b2 = 6;
int b3 = 8;
int b4 = 10;
int l = A0;

RH_ASK driver;

boolean pressed[4] = {0,0,0,0};
boolean action[4] = {0,0,0,0};
void setup() {
    pinMode(x1, INPUT);
    pinMode(y1, INPUT);
    pinMode(s1, INPUT);
    pinMode(x2, INPUT);
    pinMode(y2, INPUT);
    pinMode(s2, INPUT);
    pinMode(b1, INPUT);
    pinMode(b2, INPUT);
    pinMode(b3, INPUT);
    pinMode(b4, INPUT);
    Serial.begin(9600);

    driver.init();

    
}

void loop() {
  const char *meg1 = "Green       "; // button1
  const char *meg2 = "Blue        "; // button2
  const char *meg3 = "Yellow      "; // button3
  const char *meg4 = "Red         "; // button4
  const char *meg5 = "Right Foward"; // right stick foward
  const char *meg6 = "Left Foward "; // left stick foward
  const char *meg7 = "Right Back  "; // right stick backward
  const char *meg8 = "Left Back   "; // left stick backward
  const char *meg9 = "Left Stop   ";
  const char *meg10 ="Right Stop  ";
  const char *meg11 ="Right Left  "; // right stick left
  const char *meg12 ="Left Left   "; // left stick left
  const char *meg13 ="Right Right "; // right stick right
  const char *meg14 ="Left Right  "; // left stick right 
 
  const char *meg15 ="Light High  "; // light high
  const char *meg16 ="Light Low   "; // light low
  
  if(analogRead(y1) > 700) { // left stick is moved back moving left wheels back
    Serial.println("left back");
    driver.send((uint8_t *)meg8, strlen(meg8));
    driver.waitPacketSent();
  }if(analogRead(y1) < 400) { // left stick is moved foward moving right wheels foward
    Serial.println("left foward");
    driver.send((uint8_t *)meg6, strlen(meg6));
    driver.waitPacketSent();
  } else if (analogRead(y1) >= 400 && analogRead(y1) <= 700) {
    Serial.println("left stop");
    driver.send((uint8_t *)meg9, strlen(meg9));
    driver.waitPacketSent();
  }
  
  
 
  
  
  if(analogRead(y2) > 700) { // right stick is moved back moving left wheels back
    Serial.println("right back");
    driver.send((uint8_t *)meg7, strlen(meg7));
    driver.waitPacketSent();
  } else if(analogRead(y2) < 400) { //  right stick is moved foward moving left wheels foward 
    Serial.println("right foward");
    driver.send((uint8_t *)meg5, strlen(meg5));
    driver.waitPacketSent();
  } else if (analogRead(y2) >= 400 && analogRead(y2) <= 700){ // right stick stopped
    Serial.println("right stop");
    driver.send((uint8_t *)meg10, strlen(meg10));
    driver.waitPacketSent();
  }

  if(analogRead(x1) > 700) { // right stick right
    Serial.println("left left");
    driver.send((uint8_t *)meg12, strlen(meg12));
    driver.waitPacketSent();
  }
  if(analogRead(x2) > 700) { // right stick left
    Serial.println("right left");
    driver.send((uint8_t *)meg11, strlen(meg11));
    driver.waitPacketSent();
  }

  if(analogRead(x1) < 450) { // left stick right
    Serial.println("left right");
    driver.send((uint8_t *)meg14, strlen(meg14));
    driver.waitPacketSent();
  }
  if(analogRead(x2) < 450) { //  right stick left
    Serial.println("right right");
    driver.send((uint8_t *)meg13, strlen(meg13));
    driver.waitPacketSent();
  }

  
  
  
  
  
  if(digitalRead(b1) == HIGH && !pressed[0]) {
    driver.send((uint8_t *)meg1, strlen(meg1));
    driver.waitPacketSent();
    Serial.println("button 1 pressed");
    pressed[0] = true;
    delay(50);
  }
  if (digitalRead(b1) == LOW) {
    pressed[0] = false;
  }

  if(digitalRead(b2) == HIGH && !pressed[1]) {
    Serial.println("button 2 pressed");
    driver.send((uint8_t*)meg2, strlen(meg2));
    driver.waitPacketSent();
    pressed[1] = true;
    delay(50);
  }
  if (digitalRead(b2) == LOW) {
    pressed[1] = false;
  }

  if(digitalRead(b3) == HIGH && !pressed[2]) {
    Serial.println("button 3 pressed");
    driver.send((uint8_t*)meg3, strlen(meg3));
    driver.waitPacketSent();
    pressed[2] = true;
    delay(50);
  }
  if (digitalRead(b3) == LOW) {
    pressed[2] = false;
  }

  if(digitalRead(b4) == HIGH && !pressed[3]) {
    Serial.println("button 4 pressed");
    driver.send((uint8_t*)meg4, strlen(meg4));
    driver.waitPacketSent();
    pressed[3] = true;
    delay(50);
  }
  if (digitalRead(b4) == LOW) {
    pressed[3] = false;
  }

 

  if (analogRead(l) > 250) {
    Serial.println(analogRead(l));
    driver.send((uint8_t*)meg15, strlen(meg15));
  } else {
    Serial.println(analogRead(l));
    driver.send((uint8_t*)meg16, strlen(meg16));
  }

  

  

}
