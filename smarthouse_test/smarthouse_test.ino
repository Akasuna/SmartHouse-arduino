#include <SoftwareSerial.h>

SoftwareSerial Xbee(0, 1);


// Digital pins

int fire = 2;
int alarm = 3;
int water = 4;
int stove = 5;
int window = 6;
int powerOutage = 7;
int multiplexB0 = 8;
int outTemp = 9;
int fan = 10;
int multiplexB3 = 11;
int multiplexB4 = 12;
int multiplexB5 = 13;

// analog pins
int powerConsumption = A0;
int inTemp = A1;
int atticTemp = A2;
int ldr = A3;

//variables
char inbytes[6];
int inbyte = 0;
int count = 0;
boolean fireCount = false;
boolean waterCount = false;
boolean burglerAlarmOn = false;
boolean burglerCount = false;
boolean indoorOn = false;
boolean outdoorOn = false;
double setInTemp = 20.00;
double setAtticTemp = 20.00;





void setup() {
  // put your setup code here, to run once:
  // INPUTS
  pinMode(stove, INPUT);
  pinMode(alarm, INPUT);
  pinMode(fire, INPUT);
  //OUTPUTS
  pinMode(alarmSiren, OUTPUT);
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
  waterCount = false;

  //attic element off
  multiplexB4  = HIGH;
  multiplexB5 = HIGH;
  multiplexB3 = LOW;
  multiplexB0 = LOW;

}

void loop() {
  // put your main code here, to run repeatedly:

  readXbee();
  fireAlarm();
  burglerAlarm();
  waterAlarm();


}
//-----------------------------------------------------------------------------Command Methodes-------------------------------------------------------------------

// read from Xbee and out it in array inbytes
void readXbee() {
  if (Xbee.available()) {
    //Läser in 10 skickade char i en array
    inbyte = Xbee.read();
    inbytes[count] = inbyte;
    count++;
    if (count == SizeOf(inbytes)) {
      count = 0;
      checkFirst5Byte();
    }
  }
}
//read the first 5 bytes in array inbytes
void checkFirst5Byte() {
  //---------------------------------------------------------Read Commands------------------------------------------------------------------------------
  if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read Attic temp
    Serial.print("11000" + readTemp(atticTemp));
  }
  else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // REad room temp
    Serial.print("12000" + readTemp(inTemp));
  }
  else if (inbytes[0] == '1' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read outdoor temp
    Serial.print("13000" + readTemp(outTemp));
  }
  else if (inbytes[0] == '1' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power consumption
    readPowerCon();
  }
  else if (inbytes[0] == '1' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read fire Alarm
    readFireAlarm();
  }
  else if (inbytes[0] == '1' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read burgler alarm
    readBurglerAlarm();
  }
  else if (inbytes[0] == '1' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read water leakage ta bort men säg till carl först
  }
  else if (inbytes[0] == '1' && inbytes[1] == '8' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    stoveStatus();
  } 
  else if (inbytes[0] == '1' && inbytes[1] == '9' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read windows status
    windowStatus();
  } 
  else if (inbytes[0] == '2' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power outage status skall vara ett alarm
  }
  else if (inbytes[0] == '2' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read indoorlight status
    indoorLightStatus();
  }
  else if (inbytes[0] == '2' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read attic fan
  }
  //---------------------------------------------------------Set Command--------------------------------------------------------------------------------

  else if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '1' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set Attic temp
    setTemp();
  }
  else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '2' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set room temp
  }
  else if (inbytes[0] == '2' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set indoorlight on off
    indoorLightsOnOff();
  } 
  else if (inbytes[0] == '2' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set outdoorlight on off
    outdoorLightsOnOff();
  } 
  else {
    for (int i = 0; i < 6; i++) {
      Serial.print(inbytes[i]);
    }
    Serial.print("detta skickade du ");

  }
}
void multiplex(int b4, int b5, int b3, int b0) {
  digitalWrite(multiplexB4, b4);
  digitalWrite(multiplexB5, b5);
  digitalWrite(multiplexB3, b3);
  digitalWrite(multiplexB0, b0);
}

//-----------------------------------------------------------Alarm Methods----------------------------------------------------------

//sent a alarm when the house are on fire
void waterAlarm() {
  if (digitalRead(water) == HIGH && waterCount == false) {
    Serial.print("340001");
    waterCount = true;
  } else if (digitalRead(water) == LOW && waterCount == true) {
    Serial.print("340000");
    waterCount = false;
  }

}
void fireAlarm() {

  delay(25);
  if (digitalRead(fire) == HIGH && fireCount == false) {
    Serial.print("350001");
    multiplex(HIGH, LOW, LOW, LOW);
    fireCount  = true;
  }

  else if (digitalRead(fire) == LOW && fireCount == true) {
    Serial.print("350000");
    multiplex(LOW, LOW, LOW, LOW);
    fireCount  = false;
  }
}
void burglerAlarm() {
  if (burglerAlarmOn == true && digitalRead(alarm) == LOW && burglerCount == false) {
    Serial.print("360001");
    burglerCount = true;
    multiplex(LOW, LOW, HIGH, HIGH);
  } else if (burglerAlarmOn == true && digitalRead(alarm) == HIGH && burglerCount == true) {
    Serial.print("360000");
    multiplex(HIGH, LOW, HIGH, HIGH);
    burglerCount = false;
  }

}

//----------------------------------------------------Read Methods-------------------------------------------------------
void readFireAlarm() {
  if (digitalRead(fire) == HIGH) {
    Serial.print("150001");

  } else {
    Serial.print("150000");
  }
}

void readBurglerAlarm() {
  if (digitalRead(fire) == HIGH) {
    Serial.print("160001");

  } else {
    Serial.print("160000");
  }
}
void readPowerCon() {
  Serial.print(analogRead(A0));
}
void windowStatus() {
  if (digitalRead(window) == HIGH) {
    Serial.print("190001");

  } else {
    Serial.print("190000");
  }
}
void stoveStatus() {
  if (digitalRead(stove) == HIGH) {
    Serial.print("180001");

  } else {
    Serial.print("180000");
  }
}

String readTemp(int pin) { // can be used for in room and attic

  String temp = String(analogRead(pin));
  String tmp = "";
  if (temp.length() == 1) {
    tmp = "000" + temp;
  } else if (temp.length() == 2) {
    tmp = "00" + temp;
  } else if (temp.length() == 3) {
    tmp = "0" + temp;
  } else if (temp.length() == 4) {
    tmp = temp;
  } else {
    tmp = "xxxx";

  }
  return tmp;
}

void indoorLightStatus() {
  if ( == true) {
    Serial.print("250001");

  } else {
    Serial.print("250000");
  }
}

void outdoorLightStatus() {
  if (outdoorOn == true) {
    Serial.print("180001");

  } else {
    Serial.print("180000");
  }
}
//-----------------------------------------------------Set Methods-----------------------------------------------------
void indoorLightsOnOff() {
  if (inbytes[5] == '1') {
    multiplex(LOW, LOW, HIGH, LOW);
    indoorOn = true;
  }
  //put lights off
  else if (inbytes[5] == '0') {
    multiplex(HIGH, LOW, HIGH, LOW);
    indoorOn = false;
  } else {
    Serial.print("du vill tända/släcka lamporna inomhus men måste välja on or off");

  }
}

void outdoorLightsOnOff() {
  if (inbytes[5] == '1') {
    multiplex(LOW, HIGH, HIGH, HIGH);
    outdoorOn = true;
  }
  //put lights off
  else if (inbytes[5] == '0') {
    multiplex(HIGH, HIGH, HIGH, HIGH);
    outdoorOn = false;
  } else {
    Serial.print("du vill tända/släcka lamporna inomhus men måste välja on or off");

  }
}

void setTemp() {
  String tmp1 = String(inbytes[5]);
  Serial.print(tmp1);

}





