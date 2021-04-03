// DHT22
char dbgTH = 1; // Normalement 0, mettre a 1 pour avoir une trace du programme dans le Serial Monitor

void sondeTH() {
  if (dbg || dbgTH) {Serial.println ("Boucle sondeTH");
  delay(ddbg);}
  // DHT22
  //  // Wait a few seconds between measurements.
  //  delay(2000);
  //  // Reading temperature or humidity takes about 250 milliseconds!
  //  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //  float h = dht.readHumidity();
  //  // Read temperature as Celsius
  //  float t = dht.readTemperature();
  //
  //  // Check if any reads failed and exit early (to try again).
  //  if (isnan(h) || isnan(t)) {
  //    Serial.println("Failed to read from DHT sensor!");
  //     return ;
  //  }
  //
  //  Serial.print("Humidity: ");
  //  Serial.print(h);
  //  Serial.print(" %\t");
  //  Serial.print("Temperature: ");
  //  Serial.print(t);
  //  Serial.println(" *C ");
  //
  //  String resultatTH = "Humidity: " + String(h, 2) + "\tTemperature: " +  String(t, 2);
  //
  //  // POUR DHT22: resultatTH.toCharArray(charBuf, 40);
  //  // Serial.println(resultatTH);

  // DHT11
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));

  String resultatTH = "Humidity: " + String(h, 2) + "\tTemperature: " +  String(t, 2);
  resultatTH.toCharArray(resultDHT, 40);
  
  // DECISION RELAY !!!! =================================
  Serial.println("Declencheur: 22");  // DS  AJOUTER VARIABLE
  Serial.print("TCEL: ->");
  Serial.print(t);
  Serial.println("<-");
  if (t < 22) {
    Serial.println("<<<<<<<<<<<<<<<<  ON CHAUFFE !!!!");
    digitalWrite(monRelais1, HIGH); 
  } else {
    Serial.println(">>>>>>>>>>>>>>>>  ON FERME !!!!");
    digitalWrite(monRelais1, LOW); 
  }
  
if (dbg || dbgTH) {Serial.println ("FIN Boucle sondeTH");
delay(ddbg);}  
}
