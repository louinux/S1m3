 // DHT22
 void sondeTH(){
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
     return ;
  }
 
//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.println(" *C ");
  
  String resultatTH = "Humidity: " + String(h, 2) + "\tTemperature: " +  String(t, 2);
  resultatTH.toCharArray(charBuf, 40);
  // Serial.println(resultatTH);
 
}
