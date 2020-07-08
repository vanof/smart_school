#include <Ethernet.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//ds18b20 lib
#include <OneWire.h>
#include <DallasTemperature.h>

String apiKeyValue = "tPmAT5Ab3j7F9";
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

#define DHTPIN1 7
#define DHTPIN2 8
#define DHTPIN3 9
#define DHTPIN4 10

DHT dht[] = {
  {DHTPIN1, DHTTYPE},
  {DHTPIN2, DHTTYPE},
  {DHTPIN3, DHTTYPE},
  {DHTPIN4, DHTTYPE},
};

float humidity[4];
float temperature[4];
float temperature_calibration = 0.0;

//ds18b20 для калибровки
const int oneWireBus = 8;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

EthernetClient client;
char server[] = "smart.1561.moscow";

void setup() {
  Serial.begin(9600);
  sensors.begin();
  for (auto& sensor : dht) {
    sensor.begin();
  }
   
  //ds18b20 read temperature
  sensors.requestTemperatures(); 
  temperature_calibration = sensors.getTempCByIndex(0);
  Serial.print(F("temperature_calibration:")); Serial.print(temperature_calibration); Serial.println("ºC");
  
  for (int i = 0; i < 4; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }

  for (int i = 0; i < 4; i++) {
    Serial.print(F("Temperature "));
    Serial.print(i);
    Serial.println(temperature[i]);
    Serial.print(F("Humidity "));
    Serial.print(i);
    Serial.println(humidity[i]);
  }

    //start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1);
  }
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());  
  delay(1000);  
}

void loop() {  
  
  for (int i = 0; i < 4; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }

   String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperature[0])
                           + "&value2=" + String(humidity[0]) + "&value3=" + String(temperature[1]) + "";

  Serial.println(httpRequestData);
  if (client.connect(server,80)){
      Serial.println("Connected to server");
      client.println("POST /post-data.php HTTP/1.1");
      client.println("Host: smart.1561.moscow");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);
      client.println();
  }else{
      Serial.println("Connection to server failed");
  }

  delay(10000);
}
