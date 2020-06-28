/**
 * https://github.com/enjoyneering/HTU21D
 * https://github.com/enjoyneering/AHT10
 * https://github.com/miguel5612/MQSensorsLib
Sensors uses I2C bus to communicate, specials pins are required to interface
 SDA                    SCL
GPIO4/D2               GPIO5/D1
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Wire.h>
#include <HTU21D.h>
#include <AHT10.h>

float valueTH = 0;

HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);
AHT10 myAHT10(AHT10_ADDRESS_0X38);

// Replace with your network credentials
char ssid[] = "KOK"; // Название WiFi сети
char pass[] = "nevermore32"; // Пароль к WiFi сети

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://smart.1561.moscow/post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  while (myHTU21D.begin() != true)
  {
    Serial.println(F("HTU21D sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("HTU21D sensor is active"));
  Serial.print(F("Humidity............: ")); Serial.print(myHTU21D.readHumidity());            Serial.println(F(" +-2%"));
  Serial.print(F("Compensated Humidity: ")); Serial.print(myHTU21D.readCompensatedHumidity()); Serial.println(F(" +-2%"));
  Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature()); Serial.println(F(" +-0.3C"));

  while (myAHT10.begin(SDA, SCL) != true)
  {
    Serial.println(F("AHT10 not connected or fail to load calibration coefficient"));
    delay(5000);
  }
  Serial.println(F("AHT10 sensor is active"));
  Serial.print(F("Temperature.........: ")); Serial.print(myAHT10.readTemperature(AHT10_FORCE_READ_DATA)); Serial.println(F(" +-0.3C"));
  Serial.print(F("Humidity............: ")); Serial.print(myAHT10.readHumidity(AHT10_USE_READ_DATA));Serial.println(F(" +-2%"));
}

void loop() {

  Serial.println(F("HTU21D sensor is active"));
  Serial.print(F("Humidity............: ")); Serial.print(myHTU21D.readHumidity());            Serial.println(F(" +-2%"));
  Serial.print(F("Compensated Humidity: ")); Serial.print(myHTU21D.readCompensatedHumidity()); Serial.println(F(" +-2%"));
  Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature()); Serial.println(F(" +-0.3C"));
  delay(5000);

 
  Serial.println(F("AHT10 sensor is active"));
  Serial.print(F("Temperature.........: ")); Serial.print(myAHT10.readTemperature(AHT10_FORCE_READ_DATA)); Serial.println(F(" +-0.3C"));
  Serial.print(F("Humidity............: ")); Serial.print(myAHT10.readHumidity(AHT10_USE_READ_DATA));Serial.println(F(" +-2%"));
  delay(60000);
  
  /**
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(myHTU21D.readTemperature())
                           + "&value2=" + String(myHTU21D.readCompensatedHumidity()) + "&value3=" + String(myHTU21D.readHumidity()/100.0F) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(600000);  
  */
}
