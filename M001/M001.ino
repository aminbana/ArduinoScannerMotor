//  rotate&360@


#include <math.h>
#include "defines.h"

long Z = 0;
long Z1 = 0;
long Z2 = 0;
unsigned long Stime;
unsigned long Ctime;
String desired = "";
String Sdesired = "";
String dir = "";
String sp = "";
String e = "";
String OP = "";
int d = 0;
int Sd = 0;
int n = 0;
int s = 0;
int i = 0;
int a = 0;
int f = 0;
int F = 0;
int last_t = 0;

int currentSpeed = -1;

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH); //We need to set it HIGH immediately on boot
  
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(100);
  //Encoder Int
  REG_PMC_PCER0 = (1 << 27); // activate clock for TC0

  REG_PMC_PCER0 = (1 << 27) | (1 << 28); // activate clock for TC1

  // select XC0 as clock source and set capture mode

  REG_TC0_CMR0 = 5;

  // activate quadrature encoder and position measure mode, no filters

  REG_TC0_BMR = (1 << 8) | (1 << 9) | (1 << 12);

  // enable the clock (CLKEN=1) and reset the counter (SWTRG=1)

  REG_TC0_CCR0 = 5;

  REG_TC0_CCR1 = 5;

  //Z1 = REG_TC0_CV0;
  Serial.println(REG_TC0_CV0);

  pinMode(pin_svon, OUTPUT);  digitalWrite(pin_svon,1);
  pinMode(pin_spd1, OUTPUT);  digitalWrite(pin_spd1,1);
  pinMode(pin_spd2, OUTPUT);  digitalWrite(pin_spd2,1);
  pinMode(pin_spd3, OUTPUT);  digitalWrite(pin_spd3,1);
  pinMode(pin_cwlim, OUTPUT); digitalWrite(pin_cwlim,1);
  pinMode(pin_ccwlim, OUTPUT);digitalWrite(pin_ccwlim,1);
  pinMode(pin_stop, OUTPUT);  digitalWrite(pin_stop,1);
  pinMode(pin_mode, OUTPUT);  digitalWrite(pin_mode,1);

  pinMode(pin_z, INPUT);

  Serial.println("Ready!");
  
}

void stopMotor (){
  digitalWrite(pin_svon,1);
}

void startMotor (){
  digitalWrite(pin_svon,0);
}

void homeMotor (){
  while (!isHome()){
  startMotor();
  }
  stopMotor();
}

boolean isHome (){
  return (digitalRead(pin_z) == 1);
}

void angleCalib (){
  while (!isHome()) ;
  Z = REG_TC0_CV0;
  Serial.print("start: ");
  Serial.println (Z);
  delay(1000);
  while (!isHome()) {}
  Z1 = REG_TC0_CV0;
  Serial.print("end: ");
  Serial.println (Z1);
  Z2 = Z1 - Z;
  Serial.print("diff: ");
  Serial.println (Z2);
  Serial.println ();
  
}

void rotate (double degree){
  Serial.print ("Rotating : ");
  Serial.println(degree);
                          
  double pulses360degs [8]={ 0 //speed0
                           , 0 //speed1
                           , 1309919 //speed2
                           , 39621.0 //speed3
                           , 2323572.0 //speed4
                           , 718608 //speed5
                           , 0 //speed6
                           , 0 //speed7
                           }; 
  
  startMotor();
  Z = REG_TC0_CV0;
  Z1 = REG_TC0_CV0;
  Z2 = Z1 - Z;
  double pulses360deg =pulses360degs[currentSpeed];
  while (Z2 < (pulses360deg * degree / 360.0)){
    Z1 = REG_TC0_CV0;
    Z2 = Z1 - Z;
  }
  stopMotor();
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  d = 0;

  if (millis() - last_t > 15000){
    last_t = millis() ;
    Z = REG_TC0_CV0;
    Serial.print("Z:");
    Serial.println(Z);    
  }
  if (Serial.available()) {
    //Serial.println("ccc");
    OP = Serial.readStringUntil('&');
    if (OP == "p" ){      
      Serial.println("Calibrating...");
      Serial.println(REG_TC0_CV0);
      angleCalib();
      Serial.println("done");
    }
    if ((OP == "son")| (OP == "Son")){      
      Serial.println("son...");
      startMotor ();
    }
    if ((OP == "soff") | (OP == "Soff")){      
      Serial.println("soff...");
      stopMotor ();
    }
    /*if (OP == "spd1" ){            
      digitalWrite(pin_spd1,!digitalRead(pin_spd1)); 
      Serial.print("spd : ");
      Serial.print(!digitalRead(pin_spd3));Serial.print(!digitalRead(pin_spd2));Serial.println(!digitalRead(pin_spd1));
      currentSpeed = ?
    }
    if (OP == "spd2" ){            
      digitalWrite(pin_spd2,!digitalRead(pin_spd2)); 
      Serial.print("spd : ");
      Serial.print(!digitalRead(pin_spd3));Serial.print(!digitalRead(pin_spd2));Serial.println(!digitalRead(pin_spd1));
      currentSpeed = ?
    }
    if (OP == "spd3" ){            
      digitalWrite(pin_spd3,!digitalRead(pin_spd3)); 
      Serial.print("spd : ");
      Serial.print(!digitalRead(pin_spd3));Serial.print(!digitalRead(pin_spd2));Serial.println(!digitalRead(pin_spd1));
      currentSpeed = ?
    }*/
    if (OP == "cw" ){            
      digitalWrite(pin_cwlim,!digitalRead(pin_cwlim)); 
      Serial.print("pin_cwlim : ");
      Serial.println(digitalRead(pin_cwlim));
    }
    if (OP == "ccw" ){            
      digitalWrite(pin_ccwlim,!digitalRead(pin_ccwlim)); 
      Serial.print("pin_ccwlim : ");
      Serial.println(digitalRead(pin_ccwlim));
    }
    if ((OP == "speed0") | (OP == "Speed0")){
      setSpeed (0);
      currentSpeed = 0;
    }
    if ((OP == "speed1") | (OP == "Speed1")){
      setSpeed (1);
      currentSpeed = 1;
    }
    if ((OP == "speed2")| (OP == "Speed2")){
      setSpeed (2);
      currentSpeed = 2;
    }
    if ((OP == "speed3") | (OP == "Speed3")){
      setSpeed (3);
      currentSpeed = 3;
    }
    if ((OP == "speed4") | (OP == "Speed4")) {
      setSpeed (4);
      currentSpeed = 4;
    }
    if ((OP == "speed5") | (OP == "Speed5")){
      setSpeed (5);
      currentSpeed = 5;
    }
    if ((OP == "speed6") | (OP == "Speed6")){
      setSpeed (6);
      currentSpeed = 6;
    }
    if ((OP == "speed7") | (OP == "Speed7")){
      setSpeed (7);
      currentSpeed = 7;
    }
    
    if (OP == "mode" ){            
      digitalWrite(pin_mode,!digitalRead(pin_mode)); 
      Serial.print("pin_mode : ");
      Serial.println(digitalRead(pin_mode));
    }
    if (OP == "stop" ){            
      digitalWrite(pin_stop,0); 
      Serial.println("stop...");      
    }
    if (OP == "start" ){            
      digitalWrite(pin_stop,1); 
      Serial.println("start...");      
    }
    if ((OP == "Home") | (OP == "home")){            
      Serial.println("homing...");      
      homeMotor();
    }
    if ((OP == "Rotate") | (OP == "rotate")){            //rotate&360@
      Serial.println("rotating...");      
      int deg = Serial.readStringUntil('@').toInt();
      rotate (deg * 1.0);
      
    }

  }
}

void setSpeed (int Speed){
  Serial.print ("Setting Speed to :");
  Serial.println (Speed);
  int sp1 , sp2 , sp3;
  sp1 = (Speed & (1));
  sp2 = (Speed & (2))>>1;
  sp3 = (Speed & (4))>>2;
  digitalWrite(pin_spd1,!sp1);
  digitalWrite(pin_spd2,!sp2);
  digitalWrite(pin_spd3,!sp3);
  Serial.print(!digitalRead(pin_spd3));Serial.print(!digitalRead(pin_spd2));Serial.println(!digitalRead(pin_spd1));
}

