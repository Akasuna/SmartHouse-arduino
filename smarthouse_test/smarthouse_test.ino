#include <SoftwareSerial.h>

SoftwareSerial Xbee(0, 1);

char inbytes[6];
int inbyte = 0;
int count = 0;
boolean fireCount = false;
int fire = 2;
int alarm = 3;
int water = 4;
int inTemp = A1;
int atticTemp = A2;
int ldr = A3;
int outTemp = 9;
int stove = 5;
int window = 6;
int powerConsumption = A0;
int powerOutage = 7;
int alarmSiren = 12;
int fan = 10;
boolean burglerAlarmOn = false;
boolean burglerCount = false;

int multiplexB5 = 13;
int multiplexB4 = 12;
int multiplexB3 = 11;
int multiplexB0 = 8;




void setup() {
  // put your setup code here, to run once:
  // INPUTS
  pinMode(stove, INPUT);
  pinMode(alarm,INPUT);
  pinMode(fire,INPUT);
  //OUTPUTS
  pinMode(alarmSiren,OUTPUT);
  //OUTPUTS TO MULTIPLEX
  pinMode(multiplexB4, OUTPUT);
  pinMode(multiplexB5, OUTPUT);
  pinMode(multiplexB3, OUTPUT);
  pinMode(multiplexB0, OUTPUT);
  
  //Communication
  Serial.begin(9600);
  Xbee.begin(9600);
  // variabler
  fireCount = false;
  burglerCount = false; 
  burglerAlarmOn = true;

}

void loop() {
  // put your main code here, to run repeatedly:
  //stoveStatus();
  readXbee();
  fireAlarm();
  burglerAlarm();
  
  
}
void readXbee() {
  if (Xbee.available()) {
    //Läser in 10 skickade char i en array
    inbyte = Xbee.read();
    inbytes[count] = inbyte;
    count++;
    if (count == 6) {
      count = 0;
      checkFirst5Byte();
    }
  }
}
void fireAlarm() {
  
  delay(25);
  if (digitalRead(fire) == HIGH && fireCount == false) {
      Serial.print("350001");
      multiplex(HIGH,LOW,LOW,LOW);
      fireCount  = true;
  } 
 
  else if(digitalRead(fire) == LOW && fireCount == true){
      Serial.print("350000");
      multiplex(LOW,LOW,LOW,LOW);
      fireCount  = false;
  }
}
void readFireAlarm(){
  if (digitalRead(fire) == HIGH) {
    Serial.print("150001");

  } else {
    Serial.print("150000");
  }
}
void burglerAlarm(){
  if(burglerAlarmOn == true && digitalRead(alarm) == LOW && burglerCount == false){
      Serial.print("360001");
      burglerCount = true;
      multiplex(LOW,LOW,HIGH,HIGH);
  } else if(burglerAlarmOn == true && digitalRead(alarm) == HIGH && burglerCount == true){
    Serial.print("360000");
    multiplex(HIGH,LOW,HIGH,HIGH);
    burglerCount = false;
  }

}
void readBurglerAlarm(){
  if (digitalRead(fire) == HIGH) {
    Serial.print("160001");

  } else {
    Serial.print("160000");
  }
}
void indoorLightsOnOff() {
  if (inbytes[5] == '1') {
    multiplex(LOW,LOW,HIGH,LOW);
  }
  //put lights off
  else if (inbytes[5] == '0') {
    multiplex(HIGH,LOW,HIGH,LOW);
  } else {
    Serial.print("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}
void multiplex(int b4,int b5,int b3,int b0){
  digitalWrite(multiplexB4, b4);
    digitalWrite(multiplexB5, b5);
    digitalWrite(multiplexB3, b3);
    digitalWrite(multiplexB0, b0);
}
void checkFirst5Byte() {
  if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read Attic temp
    Serial.print("11"+ readTemp(atticTemp));
  } else if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '1' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set Attic temp
  } else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // REad room temp
    readTemp(inTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read outdoor temp
    readTemp(outTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power consumption
  } else if (inbytes[0] == '1' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read fire Alarm
    readFireAlarm();
  } else if (inbytes[0] == '1' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read burgler alarm
    readBurglerAlarm();
  } else if (inbytes[0] == '1' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read water leakage
  } else if (inbytes[0] == '1' && inbytes[1] == '8' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    stoveStatus();
  } else if (inbytes[0] == '1' && inbytes[1] == '9' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read windows status
  } else if (inbytes[0] == '2' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power outage status
  } else if (inbytes[0] == '2' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read attic fan
  } else if (inbytes[0] == '2' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read timer 1
  } else if (inbytes[0] == '2' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read timer 2
  } else if (inbytes[0] == '2' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read light status
    //indoorLightStatus();
  } else if (inbytes[0] == '2' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set indoorlight on off
    indoorLightsOnOff();
  } else if (inbytes[0] == '2' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set outdoorlight on off
   outdoorLightsOnOff();
  } else {
    for (int i = 0; i < 6; i++) {
      Serial.print(inbytes[i]);
    }
    Serial.print("detta skickade du ");

  }
}

void stoveStatus() {
  if (digitalRead(stove) == HIGH) {
    Serial.print("180001");

  } else {
    Serial.print("180000");
  }
}
String readTemp(int pin){// can be used for in room and attic
  
  String temp =String(analogRead(pin));
  String tmp ="";
  if(temp.length() == 1){
    tmp = "000"+temp;
  }else if(temp.length() == 2){
    tmp = "00" +temp;
  }else if(temp.length() == 3){
    tmp = "0" +temp;
  }else if(temp.length() == 4){
    tmp = temp;
  } else {
    tmp = "xxxx";
  }
  
  return tmp;
  
  
}
void outdoorLightsOnOff() {
if (inbytes[5] == '1') {
    multiplex(LOW,HIGH,HIGH,HIGH);
  }
  //put lights off
  else if (inbytes[5] == '0') {
    multiplex(HIGH,HIGH,HIGH,HIGH);
  } else {
    Serial.print("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}



