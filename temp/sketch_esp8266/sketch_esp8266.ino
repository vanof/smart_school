/**
This sensor uses I2C bus to communicate, specials pins are required to interface
  SDA                    SCL
GPIO4/D2               GPIO5/D1
*/


#include <ESP8266HTTPClient.h>

#include <ESP8266WiFi.h>
#include <HTU21D.h>

#define BLYNK_PRINT Serial

HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);

char ssid[] = "ITzone"; // Название WiFi сети
char pass[] = "15611561"; // Пароль к WiFi сети

void setup () { 
  Serial.begin(115200);
  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.println("Connecting.."); 
  } 

  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  while (myHTU21D.begin() != true)
  {
    Serial.println(F("HTU21D, SHT21 sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("HTU21D, SHT21 sensor is active"));
  Serial.print(F("Humidity............: ")); Serial.print(myHTU21D.readHumidity());            Serial.println(F(" +-2%"));
  Serial.print(F("Compensated Humidity: ")); Serial.print(myHTU21D.readCompensatedHumidity()); Serial.println(F(" +-2%"));
  Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature()); Serial.println(F(" +-0.3C"));
}


void loop() { 
  sendTemps(); 
  delay(60000);    //Send a request every 30 seconds 
}

void sendTemps()
{
  float temp1 = myHTU21D.readTemperature(); 
  float temp2 = 0; 
  float temp3 = 0; 
  float temp4 = 0; 
  char str[200];
  sprintf (str, "Temperatures sends: t1:%.2f, t2:%.2f, t3:%.2f, t4:%.2f", temp1, temp2, temp3, temp4);
  Serial.println(str);
  if (WiFi.status() == WL_CONNECTED) {
    char url[200];
    
    sprintf (url, "http://smart.1561.moscow/?t1=%.2f&t2=%.2f&t3=%.2f&t4=%.2f", temp1, temp2, temp3, temp4);
    Serial.println(url);

    HTTPClient http; 
    http.begin(url);
    int httpCode = http.GET();  //Send the request 
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Sent to server, response: " + payload);
    } 
    else {
      Serial.println("http status: " + httpCode);  
    }
    http.end();   //Close connection 
  }
  else {
    Serial.print("not connected"); 
  }
}
