//Code du pH-mètre

const byte pHpin = 5;    // Connect the sensor's Po output to analogue pin 0.
float val = 0;
float x = 0;
float y = 0;
void setup() {
  Serial.begin(9600);
  pinMode(pHpin, INPUT);
}

void loop(){

  val = y/750;
  x = analogRead(pHpin);
  y = x*7;
  Serial.print("Lecture du PH :");
  Serial.print(x);
  Serial.print("Lecture du PH :");
  Serial.print(y);
  Serial.print("oui?");
  Serial.println(val);
  
  
  }
