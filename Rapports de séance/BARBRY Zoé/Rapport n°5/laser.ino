//Sorties Laser


const int Recepteur1 = A2;

const int Recepteur2 = A3;

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
  if ((valPhot1 > 900)&&(valPhot2 > 900)&&(valPhot3 > 900)&&(valPhot4 > 900)){
    int val=1;
    Serial.println(val);
  }
  else{
    int val=0;
    Serial.println(val);
  }
  
  

}
