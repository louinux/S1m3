
#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
//int keyIndex = 0;                // your network key Index number (needed only for WEP)

// Reseau
char ssid[] = "Loft";         // my network SSID (name)  // ################## mettre ds un autre fichier
char pass[] = "1063Becker";       // my network key
long rssi;

char dbg = 1; // Normalement 0, mettre a 1 pour avoir une trace du programme dans le Serial Monitor
int ddbg = 3000; // debug delay
String maVersion = "202104021932"; // date '+%Y%m%d%H%M'

int status = WL_IDLE_STATUS;                     // the Wifi radio's status

unsigned long time1;
unsigned long time2 = 0;

const char *statuses[] = {"WL_IDLE_STATUS", "WL_NO_SSID_AVAIL", "WL_SCAN_COMPLETED", "WL_CONNECTED", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_DISCONNECTED"};

WiFiServer server(80);  // port pour http
WiFiClient client = server.available(); // Returns the number of bytes available for reading (that is, the amount of data that has been written to the client by the server it is connected to).

// Bloc DHT22
//#include "DHT.h"
//#define DHTPIN 4        // what pin we're connected to
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//DHT dht(DHTPIN, DHTTYPE);
//char charBuf[40];
// Fin DHT22


// Bloc DHT11
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11 Dummy variable for sensor read
DHT dht(DHTPIN, DHTTYPE);
// Fin DHT11

char resultDHT[40];

//  RELAY
int monRelais1 = 13;

void setup()
{

  Serial.begin(9600);  // vitesse du transfert du port serie Initialize serial and wait for port to open:
  dht.begin();  // sondeTH
  if (dbg) {
    delay(5000);
    Serial.println ("\n\n------------ S1m3 mode DEBUG ------------\n\n");
    Serial.print ("\n\n VERSION: ");
    Serial.println (maVersion);
    Serial.println ("\n\nBoucle setup");
  }
  // freeRam ();   // A ajouter ds debug pour test memoire
  //  pinMode(9, OUTPUT);   // set the LED pin mode  ######################################### a  faire
  //  digitalWrite(9, LOW); //LED OFF to show disconnected
  if (dbg) while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  while (!ScanSSIDs()) WiFiConnect();
  server.begin(); // A la bonne place ?????
  if (dbg) {
    Serial.println ("FIN Boucle setup");
    delay(ddbg);
  }
  if (dbg) {
    Serial.println ("#####     Fin Setup     #####");
    delay(ddbg);
  }
  Serial.println (maVersion);
  pinMode(monRelais1, OUTPUT);      // Initialize the Atmel GPIO pin as an output
}

void loop()
{
  if (dbg) {
    Serial.println ("\nBoucle loop");
    delay(ddbg);
  }

  // #####################################################################################################################
  // #####################################################################################################################

  if (dbg) Serial.println ("Bloc OPERATION");
  // bloc operations a faire avant l affichage web et meme si web plate
  // sondeTH();
  // DHT22            client.println(charBuf);
  operations();
  if (dbg) Serial.println ("FIN Bloc OPERATION");

  // Operations onglet
  // sondeTH();
  // millis: Returns the number of milliseconds passed since the Arduino board began running the current program.
  // This number will overflow (go back to zero), after approximately 50 days.
  time1 = millis();
  if ((time1 - time2) > 3000) //every 3s
  {
    if (dbg) Serial.println ("\tBoucle time 3s");
    time2 = time1;
    if (dbg) Serial.println ("\tCall TestWiFiConnection");
    TestWiFiConnection(); // test connection, and reconnect if necessary
    if (dbg) Serial.println ("\tFIN Call TestWiFiConnection");
    // Gets the signal strength of the connection to the router
    rssi = WiFi.RSSI();
    if (dbg) {//Serial.print("\tRSSI:");
      //      Serial.println(rssi);
      // print the SSID of the network you're attached to:
      Serial.print("\tSSID: ");
      Serial.println(WiFi.SSID());
      // print your board's IP address:
      IPAddress ip = WiFi.localIP();
      Serial.print("\tIP Address: ");
      Serial.println(ip);
      // print the received signal strength:
      long rssi = WiFi.RSSI();
      Serial.print("\tsignal strength (RSSI):");
      Serial.print(rssi);
      Serial.println(" dBm");
      Serial.println ("\tFIN Boucle time 3s");
    }
  }
  // listen for incoming clients
  if (dbg) {
    Serial.println ("\n\nLoop listen for incoming clients");
  }

  WiFiClient client = server.available();
  if (client) {
    if (dbg) {
      Serial.println ("client VRAI");
      delay(ddbg);
    }
    printWEB(); // requete pour la fonction printWEB
    //---    monDHT11();
  }
  if (dbg) {
    Serial.println ("\nFIN Boucle loop");
    delay(ddbg);
  }
}

void TestWiFiConnection()
// test if always connected
{
  if (dbg) Serial.println ("\t\tBoucle TestWiFiConnection: test if always connected");
  int StatusWiFi = WiFi.status();
  if (dbg) {
    Serial.print("\t\t\tStatusWiFi: ");
    Serial.println (statuses[StatusWiFi]);
  }
  if (StatusWiFi == WL_CONNECTION_LOST || StatusWiFi == WL_DISCONNECTED || StatusWiFi == WL_SCAN_COMPLETED) { //if no connection
    digitalWrite(9, LOW); //LED OFF to show disconnected ################################# A CORRIGER
    if (dbg) {
      Serial.print("\t\t\tStatusWiFiIF: ");
      Serial.println (statuses[StatusWiFi]);
    }
    if (ScanSSIDs()) WiFiConnect(); //if my SSID is present, connect
  }
  if (dbg) {
    Serial.println ("\t\tFIN Boucle TestWiFiConnection");
    delay(ddbg);
  }
}

void WiFiConnect()
//connect to my SSID
{
  if (dbg) {
    Serial.println ("\t\t\t\tBoucle WiFiConnect");
    delay(ddbg);
  }
  status = WL_IDLE_STATUS;
  int StatusWiFi = WiFi.status();
  while (status != WL_CONNECTED)
  {
    if (dbg) {
      Serial.println ("\t\t\t\t\tWiFi.begin(ssid, pass); \n\t\t\t\t\tInitializes the WiFiNINA library's network settings and provides the current status.");
      delay(ddbg);
    }
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) {
      digitalWrite(9, HIGH); //LED ON to show connected Voir code Antoine LED GREEN
      if (dbg) {
        StatusWiFi = WiFi.status();
        Serial.print("\t\t\t\t\tStatusWifi WL_CONNECTED?: ");
        Serial.println (statuses[StatusWiFi]);
      }
      //Serial.println ("\t\t\t\t\tWL_CONNECTED ");
      delay(ddbg);
    }
    else {
      Serial.println ("\t\t\t\t\tNOT_CONNECTED ");
      delay(ddbg);
    }
  }
  if (dbg) {
    Serial.println ("\t\t\t\tFIN Boucle WiFiConnect");
    delay(ddbg);
  }
}

char ScanSSIDs()
//scan SSIDs, and if my SSID is present return 1
{
  if (dbg) {
    Serial.println ("\t\t\tBoucle ScanSSIDs, and if my SSID is present return 1");
    delay(ddbg);
  }
  char score = 0;
  if (dbg) {
    Serial.println ("\t\t\t\tWiFi.scanNetworks();");
  }
  int numSsid = WiFi.scanNetworks();
  if (dbg) {
    Serial.print ("\t\t\t\tnumSsid: ");
    Serial.println (numSsid);
    delay(ddbg);
  }
  if (dbg) {
    Serial.println ("\t\t\t\tTest: if (numSsid == -1) return (0);");
  }
  if (numSsid == -1) {
    return (0);//error
    if (dbg)Serial.println ("return (0); ERROR");
  }
  for (int thisNet = 0; thisNet < numSsid; thisNet++) if (strcmp(WiFi.SSID(thisNet), ssid) == 0) score = 1; //if one is = to my SSID
  if (dbg) {
    Serial.print ("\t\t\t\tscore: ");
    Serial.println (score);
    Serial.println ("\t\t\tFIN Boucle ScanSSIDs ");
    delay(ddbg);
  }
  return (score);
}

// Creation de la page web qui sera affichee
void printWEB() {
  if (dbg) {
    Serial.println ("Boucle printWEB");
  }
  Serial.println(client);
  WiFiClient client = server.available();
  Serial.println(client);

  if (client) {                             // if you get a client,
    Serial.println("new client!!!!!!!!!!!!!!!!");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            //client.print("<H1>Preuve de concept <FONT color=green size=+8> S1m<SUP>3</SUP></FONT></H1>");
            client.print("<H1> BETA Preuve de concept BETA <FONT color=green size=+8> S1m<SUP>3</SUP></FONT></H1>");
            
            //client.print("<p>");
            // DHT
            client.print(resultDHT);

            
            client.print("<br>MonRelais1: ");
            client.print(digitalRead(monRelais1));
            client.print("<br><br>");
            
            // Signal WiFi
            client.print("Signal strength (RSSI):");
            client.print(rssi);
            client.println(" dBm");
            // VERSION
            client.print("<br>");
            client.print(maVersion);


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H")) {
          //---          digitalWrite(ledPin, HIGH);
        }
        if (currentLine.endsWith("GET /L")) {
          //---          digitalWrite(ledPin, LOW);
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  if (dbg) {
    Serial.println ("FIN Boucle printWEB");
    delay(ddbg);
  }
}
