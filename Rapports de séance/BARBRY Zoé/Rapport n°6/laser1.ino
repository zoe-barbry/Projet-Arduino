//Sorties Laser


const int Recepteur1 = A2;

const int Recepteur2 = A1;

const int Recepteur3 = A4;

const int Recepteur4 = A5;


void setup() {
  
  //Initialisation de la communication série 
  //    avec l'ordinateur.
  Serial.begin(9600);
  
}

void loop() {
  
  
  //Création d'une variable booléenne
  //   pour y stocker l'état du bouton
  //   qui est lu sur la broche 3.
  int valPhot1=analogRead(Recepteur1);
  int valPhot2=analogRead(Recepteur2);
  int valPhot3=analogRead(Recepteur3);
  int valPhot4=analogRead(Recepteur4);
  
  //Envoi de la valeur de la variable etatBouton
  //   sur le terminal de l'ordinateur.
  //Serial.println(valPhot);
  if ((((valPhot1 > 800) && (valPhot2 > 800)) && (valPhot3 > 800)) && (valPhot4 > 800)){
    int val=1;
    Serial.println(val);
    
  }
  else{
    int val=0;
    Serial.println(val);
  }
  /*Serial.println(analogRead(A4));
  Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  Serial.println(analogRead(A5));*/
  
  

}
