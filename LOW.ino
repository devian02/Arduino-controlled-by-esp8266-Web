#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3); //Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.
#define DEBUG true

int indietro = 11;      //pin 6 of arduino to pin 7 of l293d
int avanti = 12;      //pin 7 of arduino to pin 2 of l293d
int sinistra  = 10;     //pin 5 of arduino to pin 10 of l293d
int destra = 9;      //pin 4 of arduino to pin 15 of l293d

int rosso = 6;
int beep = 8;

/* const int triggerPort1 = 8;
const int echoPort1 = 7;
const int triggerPort2 = 1;
const int echoPort2 = 0; */

void setup()
  {
    pinMode(13, OUTPUT);
    pinMode(rosso,OUTPUT);
    pinMode(beep, OUTPUT);
    pinMode(avanti, OUTPUT);
    pinMode(indietro, OUTPUT);
    pinMode(sinistra, OUTPUT);
    pinMode(destra, OUTPUT);
    /* pinMode(triggerPort1, OUTPUT);
    pinMode(echoPort1, INPUT);
    pinMode(triggerPort2, OUTPUT);
    pinMode(echoPort2, INPUT); */
  
    /* Centra le ruote all'avvio */
    digitalWrite(destra, HIGH);
    digitalWrite(sinistra, LOW);
    delay(500);
    digitalWrite(destra, LOW);
    digitalWrite(sinistra,HIGH);
    delay(133);
    digitalWrite(destra, LOW);
    digitalWrite(sinistra,LOW);
    digitalWrite(13,HIGH);
    digitalWrite(beep,LOW);
    Serial.begin(9600);
    esp8266.begin(9600); //Baud rate for communicating with ESP8266. Your's might be different.
    esp8266Serial("AT+RST\r\n", 2000, DEBUG); // Reset the ESP8266
    esp8266Serial("AT+CWMODE=3\r\n", 1000, DEBUG); //Set station mode Operation
    esp8266Serial("AT+CIFSR\r\n", 1000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 
    esp8266Serial("AT+CIPMUX=1\r\n", 1000, DEBUG);
    esp8266Serial("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
  }

void loop()
  {
    /* porta bassa l'uscita del trigger
    digitalWrite( triggerPort1, LOW );
    //invia un impulso di 10microsec su trigger
    digitalWrite( triggerPort1, HIGH );
    delayMicroseconds( 10 );
    digitalWrite( triggerPort1, LOW );
   
    long durata = pulseIn( echoPort1, HIGH );
   
    long distanza = 0.034 * durata / 2;
  
    if (distanza < 20){
      digitalWrite(indietro,LOW);
    }
  
    //porta bassa l'uscita del trigger
    digitalWrite( triggerPort2, LOW );
    //invia un impulso di 10microsec su trigger
    digitalWrite( triggerPort2, HIGH );
    delayMicroseconds( 10 );
    digitalWrite( triggerPort2, LOW );
   
    long durata2 = pulseIn( echoPort2, HIGH );
   
    long distanza2 = 0.034 * durata2 / 2;
  
    if (distanza2 < 20){
      digitalWrite(avanti,LOW);
    }
    Serial.println(distanza);
    Serial.println(distanza2); */
    if (!digitalRead(indietro) && !digitalRead(avanti)){
      digitalWrite(rosso, HIGH);
    } else {
      digitalWrite(rosso, LOW);
    }
    
    if (esp8266.available())
      {
        if (esp8266.find("+IPD,"))
          {
            String msg;
            esp8266.find("?");
            msg = esp8266.readStringUntil(' ');
            String command1 = msg.substring(0, 3);
            String command2 = msg.substring(4);
                        

            if(command2 == "AV"){
              digitalWrite(avanti, HIGH);
              digitalWrite(indietro, LOW);
            } else if (command2 == "IN") {
              digitalWrite(avanti, LOW);
              digitalWrite(indietro, HIGH);
            } else if (command2 == "DX") {
              digitalWrite(destra, HIGH);
              digitalWrite(sinistra, LOW);
              delay(500);
              digitalWrite(destra, LOW);
              digitalWrite(sinistra,LOW);
            } else if (command2 == "SX") {
              digitalWrite(destra, LOW);
              digitalWrite(sinistra, HIGH);
              delay(500);
              digitalWrite(destra, LOW);
              digitalWrite(sinistra,LOW);
            } else if (command2 == "ST") {
              digitalWrite(avanti, LOW);
              digitalWrite(indietro, LOW);
            } else if (command2 == "CX") {
              digitalWrite(destra, HIGH);
              digitalWrite(sinistra, LOW);
              delay(500);
              digitalWrite(destra, LOW);
              digitalWrite(sinistra,HIGH);
              delay(133);
              digitalWrite(destra, LOW);
              digitalWrite(sinistra,LOW);
            } else if (command2 == "LH") {
              digitalWrite(13, HIGH);
            } else if (command2 == "LL") {
              digitalWrite(13, LOW);
            } else if (command2 == "BP") {
              digitalWrite(beep, HIGH);
              delay(100);
              digitalWrite(beep, LOW);
            }
          }
      }
  }
   
String esp8266Serial(String command, const int timeout, boolean debug)
  {
    String response = "";
    esp8266.print(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (esp8266.available())
          {
            char c = esp8266.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
