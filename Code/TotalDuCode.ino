//pH-metre
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
unsigned long int avgValue;    //Store the average value of the sensor feedback
#include <SoftwareSerial.h> 
const int tx = 2;
const int rx = 13;
SoftwareSerial MySerial(rx,tx);


//Entrées PR
const int Recepteur1 = A0;
const int Recepteur2 = A1;
const int Recepteur3 = A2;
const int Recepteur4 = A3;

//Sorties lasers
const int L1=6;
const int L2=7;
const int L3=8;
const int L4=9;
bool OUI = true;
const int bouton=12;
int val=1;
int etat=0;
int ancienval=1;

//Alarme 
#define NOTE_A4  440   //grave
#define NOTE_C6  1047 //aigu 
const int buzzer = 10;

//aled

String inputString = "";
String resultFromArd[3];

struct mesureToSend{
  float pH;
  float uDist;
  float temp;
  String alarme;
  };
struct mesureToSend dataToSend;


//Thermomètre
#include <OneWire.h>
OneWire  ds(11);  // on pin 11 (a 4.7K resistor is necessary)

//pH-mètre
const int pH1 = A5;

//mesure hauteur de l'eau
const int trig = 4;
const int echo = 3;
long int tmps = 0;
float dist = 0;


void setup() {
  MySerial.begin(9600); 
  Serial.begin(9600);
  //Bouton 
  pinMode(bouton,INPUT);

  //mesure hauteur de l'eau
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);

  //Laser
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
 
  
}
void loop() {

  bouton1();
  collect_data();
  sendAllData();
  laserino();
  
}

//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

void collect_data(){
  
  dataToSend.pH = pH();
  dataToSend.temp = temp();
  dataToSend.uDist = uDist();
  dataToSend.alarme = alarme();
}
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

float pH(){
  int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(pH1);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=(3.5*phValue+Offset);                      //convert the millivolt into pH value
  delay(800);
  return phValue;
}
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
float temp(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
   
  }
  celsius = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius ");
  return celsius;
}
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF

float uDist(){
  digitalWrite (trig, HIGH);
  delayMicroseconds(10);
  digitalWrite (trig, LOW);
  long int tmps = pulseIn(echo, HIGH);
  float dist = tmps*0.017;
  float hauteur = 8-dist;
  Serial.print ("la distance est de ");
  Serial.println (dist);
  delay(60);
  return hauteur;
}
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
void sendAllData(){
  MySerial.print(dataToSend.temp);MySerial.print('$');MySerial.print(0);Serial.print("---SENT---> ");Serial.println(dataToSend.temp);
  MySerial.print(dataToSend.uDist);MySerial.print('$');MySerial.print(1);Serial.print("---SENT---> ");Serial.println(dataToSend.uDist);
  MySerial.print(dataToSend.pH);MySerial.print('$');MySerial.print(2);Serial.print("---SENT---> ");Serial.println(dataToSend.pH);
  MySerial.print(dataToSend.alarme);MySerial.print('$');MySerial.print(3);Serial.print("---SENT---> ");Serial.println(dataToSend.alarme);
  Serial.println("###############");
}
//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
void bouton1(){
  val=digitalRead(bouton);
  if ((val==LOW) && (ancienval==HIGH)){
    etat=1-etat;
  }
  ancienval=val;
  if (etat==HIGH){
    OUI=false;
    
  }
  else{
    OUI=true;
    
  }
  Serial.println(OUI);
}
String alarme(){
  if (OUI==true){
    return "on";
  }
  else{
    return "off";
  }
}
void laserino(){
  int valPhot1=analogRead(Recepteur1);
  int valPhot2=analogRead(Recepteur2);
  int valPhot3=analogRead(Recepteur3);
  int valPhot4=analogRead(Recepteur4);

 Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  Serial.println(analogRead(A2));
  Serial.println(analogRead(A3));

  if(OUI == true){   
    digitalWrite(L1, HIGH);
    digitalWrite(L2, HIGH);
    digitalWrite(L3, HIGH);
    digitalWrite(L4, HIGH);
    if ((((valPhot1 > 800) && (valPhot2 > 800)) && (valPhot3 > 800)) && (valPhot4 > 800)){
      int val=1;
      Serial.println(val);

    }
    else{
      int val=0;
      Serial.println(val);
      tone (buzzer, NOTE_A4, 1000);
    }
   }
  
  
    if(OUI == false){
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
      digitalWrite(L3, LOW);
      digitalWrite(L4, LOW);
      
    
  }
}
  
