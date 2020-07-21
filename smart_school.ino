// библиотеки для Ethernet
// 10 пин используется для CS по умолчанию
// You can use Ethernet.init(pin) to configure the CS pin
//Ethernet.init(10);  // Most Arduino shields
#include <SPI.h>
#include <Ethernet.h>

// библиотеки для основных датчиков
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// библиотеки для калибровки с использованием датчика ds18b20 
#include <OneWire.h>
#include <DallasTemperature.h>

// конфигурация датчика ds18b20 
const int oneWireBus = 2;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float temperature_calibration = 0.0;

// количество датчиков
#define SENSOR_AMOUNT 4
#define MAX_SENSOR_AMOUNT 4 

// тип датчика
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// подключение
#define DHTPIN1 7
#define DHTPIN2 8
#define DHTPIN3 9
//dont use 10 pin! 
#define DHTPIN4 11

DHT dht[] = {
  {DHTPIN1, DHTTYPE},
  {DHTPIN2, DHTTYPE},
  {DHTPIN3, DHTTYPE},
  {DHTPIN4, DHTTYPE}
};

// хранение полученных значений
float humidity[MAX_SENSOR_AMOUNT];
float temperature[MAX_SENSOR_AMOUNT];

// конфигурация интернет соединения
// мак адрес для плат должен быть разным!
// assign a MAC address for the ethernet controller.
// fill in your address here:
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
//byte mac[] = {0x00,0x01,0x02,0x03,0x04,0x05};
//byte mac[] = {0x06,0x07,0x02,0x03,0x04,0x05};
byte mac[] = {0x02,0x04,0x02,0x03,0x04,0x05};

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;
char server[] = "smart.1561.moscow";
//IPAddress server(64,131,82,241);

// уникальный ключ для записи в базу данных
//String apiKeyValue = "tPmAT5Ab3j7F9";  //тестовый ключ
//String apiKeyValue = "D1tPmAT5Ab3j7";  //первый этаж
//String apiKeyValue = "D2tPmAT5Ab3j7";  //второй этаж
String apiKeyValue = "D3tPmAT5Ab3j7";  //третий этаж

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 60*1000;  // delay between updates, in milliseconds

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
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
    Serial.print(F("Temperature "));
    Serial.print(i);
    Serial.println(temperature[i]);
    Serial.print(F("Humidity "));
    Serial.print(i);
    Serial.println(humidity[i]);
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.print("DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

  //рандомная пауза, иначе контроллеры забрасывают друг-друга пакетами
  randomSeed(analogRead(0));
  int randDelay = random(1000, 2500);
  Serial.println(randDelay);
  delay(randDelay);
  Serial.println("Run!");
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

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

  httpRequest(httpRequestData);

  //delay(30000);
  //delay(60000);     //1 минута
  //delay(300000);  //5 минут
  delay(900000);  //15 минут частота опроса
}

// this method makes a HTTP connection to the server:
void httpRequest(String httpRequestData) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
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
    /*
    // send the HTTP GET request:
    client.println("GET /latest.txt HTTP/1.1");
    client.println("Host: smart.1561.moscow");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    */
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
