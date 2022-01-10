// Thermomètre 
#include <OneWire.h>
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)


//pH-mètre 
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
unsigned long int avgValue;    //Store the average value of the sensor feedback


//distance par ultrason
const int trig = 4;
const int echo = 3;
long int tmps = 0;
float dist = 0;


void setup() {
  // thermomètre 
  Serial.begin(9600);

  //pH-mètre 
  pinMode(13,OUTPUT);  

  //distance par ultrason 
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);

}

void loop() {
  //thermomètre 
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);      
  
  delay(1000);    
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);       
  for ( i = 0; i < 9; i++) {         
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  
    else if (cfg == 0x20) raw = raw & ~3; 
    else if (cfg == 0x40) raw = raw & ~1; 
  }
  celsius = (float)raw / 16.0;
  Serial.print("Temperature = ");
  Serial.print(celsius);
  Serial.println(" Celsius");


  //pH-mètre 
  int buf[10];                // for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        
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
  for(int i=2;i<8;i++)                      
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6;
  phValue=3.5*phValue+Offset;                     
  //Serial.println(analogRead(A0));
  Serial.print("pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 


  //distance par ultrason 
  digitalWrite (trig, HIGH);
  delayMicroseconds(10);
  digitalWrite (trig, LOW);
  long int tmps = pulseIn(echo, HIGH);
  float dist = tmps*0.017;
  Serial.print ("Distance: ");
  Serial.println (dist);
  delay(60);

}
