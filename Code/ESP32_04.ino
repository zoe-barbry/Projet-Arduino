#include <WiFi.h>   // Utilisation de la librairie WiFi.h
#include <WebServer.h>   // Utilisation de la librairie WebServer.h
#include <SoftwareSerial.h>

const int tx = 17;
const int rx = 16;

SoftwareSerial Arduino(rx,tx);

String inputString = "";
String result[4];

String temp = result[0];
String uDist = result[1];
String pH = result[2];
String alarme = result[3];

const char* ssid = "Androidz";  // Mettre le ssid de votre réseau Wifi
const char* password = "azer7766";   // Mettre le mot de passe de votre réseau Wifi
WebServer server(80);  // Permet l'affichage de la page d'accueil 

void handleRoot(){  // Page d'accueil La page HTML est mise dans le String page
  char page[3500];
  String temp = result[0];
  String uDist = result[1];
  String pH = result[2];
  String alarme = result[3];

  snprintf(page, 3500, 

  "<html>\
<head>\
    <title>Arduin'eau sûr</title>\
    <meta http-equiv='refresh' content='1'/>\
    <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8'/>\
    <link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>\
</head>\
<body lang='fr'>\
    <div class='w3-container w3-center '>\
        <h1>Arduin'eau sûr</h1>\
    </div>\
    <div class='w3-container w3-light-blue'>\
        <h2>Voici les informations sur votre piscine</h2>\
    </div>\
    <section>\
        <div class='w3-container w3-lightgrey'>\
            <p>Le pH est de : %s</p>\
            <p>La température de l'eau est de : %s </p>\
            <p>La hauteur de l'eau est de : %s</p>\
        </div>\
    </section>\
    <div class='w3-container w3-light-blue'>\
        <h3>Marche/Arrêt de l'alarme</h3>\
    </div>\
    <div class='w3-container'>\
        <p>Alarme : %s </p>\
    </div>\
</body>\
</html>",
pH,temp,uDist,alarme

);
    server.send(200, "text/html", page);  // Envoie de la page HTML
}

void handleNotFound(){  // Page Not found
  server.send(404, "text/plain","404: Not found");
}

void setup() {
  Serial.begin(9600);
  Arduino.begin(9600);
  delay(1000);
  Serial.println("\n");
  WiFi.begin(ssid, password);  // Initialisation du Wifi
  Serial.print("Attente de connexion ...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\n");  // Affichage "Connexion établie" et de l'adresse IP
  Serial.println("Connexion etablie !");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);  // Chargement de la page d'accueil
  server.onNotFound(handleNotFound);  // Chargement de la page "Not found"
  server.begin();  // Initialisation du serveur web
  Serial.println("Serveur web actif");
}

void loop() {
  
  readData();
  server.handleClient();  // Attente de demande du client
  // Send answer to master
  Serial.println(pH);
  Serial.println(temp);
  Serial.println(uDist);
  Serial.println(alarme);
  delay(2000);
  
}
void readData(){
  while (Arduino.available()) {
    //Serial.println("1");
    char inChar = (char)Arduino.read();
    if (inChar != '$') {
      inputString += inChar;
    }
    
    else{ // if  == $
      char indice = Arduino.read(); // on recolte l'indice de l'info
      int e = indice - '0'; //  technique illegale pour convertir un caractere en entier
      result[e] = inputString; // on ajoute la données dans le tableau resultat
      inputString = ""; // on vide la chaine
      }
    Serial.println(inputString);
   
  }
}
