#include <SoftwareSerial.h>

SoftwareSerial Xbee(0, 1);
char inbytes[6];
int inbyte = 0;
int count = 0;
int redPin = 8;
int yellowPin = 9;
int greenPin = 10;
int whitePin = 11;
int stovePin = 2;
int atticTemp = A0;
int roomTemp = A0;
int outDoorTemp = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Xbee.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(stovePin, INPUT);
  pinMode(atticTemp, INPUT);
  pinMode(roomTemp, INPUT);
  pinMode(outDoorTemp, INPUT);
}

void loop() {
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
void checkFirst5Byte() {
  if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read Attic temp
    readTemp(atticTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '1' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set Attic temp
  } else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // REad room temp
    readTemp(roomTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read outdoor temp
    readTemp(outDoorTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power consumption
  } else if (inbytes[0] == '1' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read fire alarm
  } else if (inbytes[0] == '1' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read burgler alarm
  } else if (inbytes[0] == '1' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read water leakage
  } else if (inbytes[0] == '1' && inbytes[1] == '8' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    stove();
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
    indoorLightStatus();
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
    Serial.println("detta skickade du ");

  }
}
void readTemp(int pin){// can be used for in room and attic
  double Temp = (5.0 * analogRead(pin) * 100.0) / 1024;
  Serial.println(Temp);
}
void stove() {
  if (digitalRead(stovePin) == HIGH) {
    Serial.println("180001");
  } else {
    Serial.println("180000");
  }
}
void indoorLightStatus() {
  if (digitalRead(redPin) == HIGH) {
    Serial.print("250001");
  } else {
    Serial.print("250000");
  }
}
void indoorLightsOnOff() {
  if (inbytes[5] == '1') {
    
    if (digitalRead(redPin) == HIGH) {
      Serial.print("260001");
    } else {
      digitalWrite(redPin, HIGH);
      Serial.print("260001");
    }

  }
  //put lights off
  else if (inbytes[5] == '0') {

    if (digitalRead(redPin) == LOW) {
      Serial.print("260000");
    } else {
      digitalWrite(redPin, LOW);
      Serial.print("260000");
    }


  } else {
    Serial.println("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}

void outdoorLightsOnOff() {
  if (inbytes[5] == '1') {

    if (digitalRead(yellowPin) == HIGH) {
      Serial.println("270001");
    } else {
      digitalWrite(yellowPin, HIGH);
      Serial.println("270001");
    }

  }
  //put lights off
  else if (inbytes[5] == '0') {

    if (digitalRead(yellowPin) == LOW) {
      Serial.println("270000");
    } else {
      digitalWrite(yellowPin, LOW);
      Serial.println("270000");
    }


  } else {
    Serial.println("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}





