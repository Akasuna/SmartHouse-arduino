#include <SoftwareSerial.h>

SoftwareSerial Xbee(0, 1);
char inbytes[10];
int inbyte = 0;
int count = 0;
int redPin = 8;
int yellowPin = 9;
int greenPin = 10;
int whitePin = 11;
int stovePin = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Xbee.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(stovePin, INPUT);
}

void loop() {
  if (Xbee.available()) {
    //Läser in 10 skickade char i en array
    inbyte = Xbee.read();
    inbytes[count] = inbyte;
    count++;
    if (count == 10) {
      count = 0;
      checkFirst5Byte();
    }
  }
}
void checkFirst5Byte() {
  if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read Attic temp
  } else if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '1' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set Attic temp
  } else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // REad room temp
  } else if (inbytes[0] == '1' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read outdoor temp
  } else if (inbytes[0] == '1' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power consumption
  } else if (inbytes[0] == '1' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read fire alarm
  } else if (inbytes[0] == '1' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read burgler alarm
  } else if (inbytes[0] == '1' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read wate leakage
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
    indorrLightStatus
  } else if (inbytes[0] == '2' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set indoorlight on off
    indoorLightsOnOff();
  } else if (inbytes[0] == '2' && inbytes[1] == '7' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set outdoorlight on off
    outdoorLightsOnOff();
  } else {
    for (int i = 0; i < 10; i++) {
      Serial.print(inbytes[i]);
    }
    Serial.println("detta skickade du ");

  }
}
void stove() {
  if (digitalRead(stovePin) == HIGH) {
    Serial.println("18000TRUE0");
  } else {
    Serial.println("18000FALSE");
  }
}
void indoorLightStatus()
if(digitalRead(redPin) = HIGH){
  Serial.println("25000TRUE0")
}else{
  Serial.println("25000FALSE")
}
void indoorLightsOnOff() {
  if (inbytes[5] == 'T' && inbytes[6] == 'R' && inbytes[7] == 'U' && inbytes[8] == 'E') {

    if (digitalRead(redPin) == HIGH) {
      Serial.println("The indoor lights was already on");
    } else {
      digitalWrite(redPin, HIGH);
      Serial.println("The indoor lights is on");
    }

  }
  //put lights off
  else if (inbytes[5] == 'F' && inbytes[6] == 'A' && inbytes[7] == 'L' && inbytes[8] == 'S' && inbytes[9] == 'E') {

    if (digitalRead(redPin) == LOW) {
      Serial.println("the lights was already off");
    } else {
      digitalWrite(redPin, LOW);
      Serial.println("The lights is off");
    }


  } else {
    Serial.println("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}

void outdoorLightsOnOff() {
  if (inbytes[5] == 'T' && inbytes[6] == 'R' && inbytes[7] == 'U' && inbytes[8] == 'E') {

    if (digitalRead(yellowPin) == HIGH) {
      Serial.println("The indoor lights was already on");
    } else {
      digitalWrite(yellowPin, HIGH);
      Serial.println("The indoor lights is on");
    }

  }
  //put lights off
  else if (inbytes[5] == 'F' && inbytes[6] == 'A' && inbytes[7] == 'L' && inbytes[8] == 'S' && inbytes[9] == 'E') {

    if (digitalRead(yellowPin) == LOW) {
      Serial.println("the lights was already off");
    } else {
      digitalWrite(yellowPin, LOW);
      Serial.println("The lights is off");
    }


  } else {
    Serial.println("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}





