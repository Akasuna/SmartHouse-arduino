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

int multiplexB5 = 13;
int multiplexB4 = 12;
int multiplexB3 = 11;
int multiplexB0 = 8;




void setup() {
  // put your setup code here, to run once:
  pinMode(stove, INPUT);
  pinMode(multiplexB4, OUTPUT);
  pinMode(multiplexB5, OUTPUT);
  pinMode(multiplexB3, OUTPUT);
  pinMode(multiplexB0, OUTPUT);
  Serial.begin(9600);
  Xbee.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
 //stoveStatus();
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
  if(digitalRead(fire) == HIGH){
    
    if(fireCount == false){
      Serial.print("150001");
      fireCount  = true;
    }
  }else{
    fireCount = false;
    }
 
}void indoorLightsOnOff() {
  if (inbytes[5] == '1') {
    
    digitalWrite(multiplexB4, LOW);
    digitalWrite(multiplexB5,LOW);
    digitalWrite(multiplexB3, HIGH);
    digitalWrite(multiplexB0, LOW);

  }
//put lights off
  else if (inbytes[5] == '0') {

   digitalWrite(multiplexB4, HIGH);
   digitalWrite(multiplexB5,LOW);
   digitalWrite(multiplexB3, HIGH);
   digitalWrite(multiplexB0, LOW);


  } else {
    Serial.println("du vill tända/släcka lamporna inomhus men måste välja true or false");

  }
}

void checkFirst5Byte() {
  if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read Attic temp
    //readTemp(atticTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '1' && inbytes[2] == '1' && inbytes[3] == '0' && inbytes[4] == '0') {
    // set Attic temp
  } else if (inbytes[0] == '1' && inbytes[1] == '2' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // REad room temp
    //readTemp(roomTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '3' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    // read outdoor temp
    //readTemp(outDoorTemp);
  } else if (inbytes[0] == '1' && inbytes[1] == '4' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read power consumption
  } else if (inbytes[0] == '1' && inbytes[1] == '5' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read fire alarm
  } else if (inbytes[0] == '1' && inbytes[1] == '6' && inbytes[2] == '0' && inbytes[3] == '0' && inbytes[4] == '0') {
    //read burgler alarm
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
    //outdoorLightsOnOff();
  } else {
    for (int i = 0; i < 6; i++) {
      Serial.print(inbytes[i]);
    }
    Serial.println("detta skickade du ");

  }
}

void stoveStatus(){
  delay(1000);
  Serial.println(digitalRead(stove));
  if(digitalRead(stove) == HIGH){
    Serial.print("180001");
    
  } else {
    Serial.print("180000");
  }
}
void outdoorlight(){
  
}

