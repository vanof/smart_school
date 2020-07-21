#include <Ethernet.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//для калибровки с использованием датчика ds18b20 
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 2;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temperature_calibration = 0.0;

//уникальный ключ для записи в базу данных + разный мак-адрес
String apiKeyValue = "tPmAT5Ab3j7F9";
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient client;
char server[] = "smart.1561.moscow";

#define SENSOR_AMOUNT 3
#define MAX_SENSOR_AMOUNT 4 

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
  {DHTPIN4, DHTTYPE}
};

float humidity[MAX_SENSOR_AMOUNT];
float temperature[MAX_SENSOR_AMOUNT];

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

void setup() {
  Serial.begin(9600);

  //ds18b20
  sensors.begin();

  //инициализация датчиков
  for (auto& sensor : dht) {
    sensor.begin();
  }
  
  for (int i = 0; i < MAX_SENSOR_AMOUNT; i++) {
    temperature[i] = 0.0;
    humidity[i] = 0.0;
  }
  
  for (int i = 0; i < SENSOR_AMOUNT; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }
  
  for (int i = 0; i < SENSOR_AMOUNT; i++) {
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

  /*
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
   httpRequest();  //prototype of void loop
  }
  */

  //опрос ds18b20
  sensors.requestTemperatures(); 
  temperature_calibration = sensors.getTempCByIndex(0);

  for (int i = 0; i < SENSOR_AMOUNT; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }
  
  String httpRequestData = "api_key=" + apiKeyValue
                           + "&value1=" + String(temperature[0])
                           + "&value2=" + String(humidity[0])
                           + "&value3=" + String(temperature[1]) 
                           + "&value4=" + String(humidity[1])                                          
                           + "&value5=" + String(temperature[2]) 
                           + "&value6=" + String(humidity[2])
                           + "&value7=" + String(temperature[3])
                           + "&value8=" + String(humidity[3]) 
                           + "&calibration=" + String(temperature_calibration)
                           + "";
  Serial.println(httpRequestData); 
  
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
      Serial.println("Connected to server");
      client.println("POST /post-data.php HTTP/1.1");
      client.println("Host: smart.1561.moscow");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);
      client.println();
      lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
  
  delay(300000);  //5 минут
  //delay(900000);  //15 минут частота опроса
}
