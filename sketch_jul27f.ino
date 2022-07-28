#include<MsTimer2.h>
String readString;

int Ena = 6; int IN1 = 7; int IN2 = 8; 
int CW= 21; int CCW =20;
int cw =0; int ccw=0;
int ang_cw =0; int ang_ccw=0;
int ref = 1000;
int pos;
int value = 250;


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT); pinMode(IN2 ,OUTPUT); pinMode(Ena, OUTPUT);
  pinMode(CW,INPUT); pinMode(CCW, INPUT);
  attachInterrupt(2, check_A, CHANGE);
  attachInterrupt(3, check_B, CHANGE);
  MsTimer2::set(50,timerISR);
  MsTimer2::start();
  MsTimer2::set(50,calculate_vel);
  MsTimer2::start();
  Serial.begin(9600);
}


// A핀의 변화가 있을때 checking
void check_A() {
  cw = digitalRead(CW);
  ccw = digitalRead(CCW);
  if(cw != ccw) ang_cw++;
  if(cw == ccw) ang_ccw++;
}
//B친의 변화가 있을떄 checking
void check_B() {
  cw=digitalRead(CW);
  ccw=digitalRead(CCW);
  if(cw != ccw) ang_ccw++;
  if(cw == ccw) ang_cw++;
}


//모터 속도 계산.
double vel, vel1, vel2; 
double current_pos, pre_pos, elapsed_pos;
double elapsedTime, currentTime, previousTime; 
  
void calculate_vel() {
  current_pos = pos;
  elapsed_pos = current_pos-pre_pos;
  pre_pos = pos; // pos(지나가는 슬릿 개수)변화량
 
  currentTime = millis();
  elapsedTime = currentTime-previousTime;
  previousTime = currentTime; // 시간변화량
  vel=500*elapsed_pos/elapsedTime; // vel =((60*1000*elapsed_pos)/(elapsedTime*30*4));
  
}

// 속도값 입력하면 바뀜

void timerISR() {
 while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  // allow buffer to fill with next character
    }
  if (readString.length() >0) {
    value = readString.toInt();
    Serial.println(value);
    readString=""; //empty for next input
  } 
}




void loop() {
  // put your main code here, to run repeatedly:
  pos = -ang_ccw + ang_cw;
  if (value>0) {
     digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);
     analogWrite(Ena,value);
  }
  else if(value == 0) {
    analogWrite(Ena,0);
  }
  else { 
    digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);
    analogWrite(Ena,-value);
  }


  //Serial.println(pos);
  //Serial.print(',');
  //Serial.println(value);
  //Serial.print(',');
  Serial.println(vel);
  //Serial.print(',');
  //Serial.println(elapsed_pos);
  //Serial.print(',');
  //Serial.println(elapsedTime);
  //Serial.print(',');
  //Serial.println(previousTime);
  //Serial.print(',');
  //Serial.println(currentTime);
}
