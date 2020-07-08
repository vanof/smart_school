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

#define DHT_1 7
#define DHT_2 10
DHT_Unified dht_1(DHT_1, DHTTYPE);
DHT_Unified dht_2(DHT_2, DHTTYPE);

//ds18b20 для калибровки
const int oneWireBus = 8;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

EthernetClient client;

char server[] = "smart.1561.moscow";

uint32_t delayMS;

float humidity_1 = 0.0;
float humidity_2 = 0.0;
float temperature_1 = 0.0;
float temperature_2 = 0.0;
float temperature_calibration = 0.0;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  dht_1.begin();
  dht_2.begin();

/**
  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1);
  }
  */
  //   start the Ethernet connection:
  
  Serial.println("Initialize Ethernet with DHCP:");
  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1);
  }
 
  // print your local IP address:
  Serial.print("My IP address: ");
 // Serial.println(Ethernet.localIP());


  /**
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht_1.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht_1.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  */
}

void loop() {  
  //delay(delayMS);
  // Get temperature event and print its value.
  
  sensors_event_t event;
  dht_1.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    temperature_1 = event.temperature;
    Serial.print(temperature_1);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht_1.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    humidity_1 = event.relative_humidity;
    Serial.print(humidity_1);
    Serial.println(F("%"));
  }


  dht_2.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    temperature_2 = event.temperature;
    Serial.print(temperature_2);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht_2.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    humidity_2 = event.relative_humidity;
    Serial.print(humidity_2);
    Serial.println(F("%"));
  }

  //ds18b20 read temperature
  sensors.requestTemperatures(); 
  //temperature_calibration = sensors.getTempCByIndex(0);
  Serial.print(F("temperature_calibration:")); Serial.print(temperature_calibration); Serial.println("ºC");
   
  //String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperature_1)
                          + "&value2=" + String(humidity_1) + "&value3=" + String(0) + "";
                           
  
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(0)
                           + "&value2=" + String(0) + "&value3=" + String(0) + "";

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
  
  /*while(client.connected()){
    if(client.available()){
      char c = client.read();
      Serial.print(c);  
    }
 }*/
  delay(5000);
}
