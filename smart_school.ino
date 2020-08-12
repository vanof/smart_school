// 10 пин используется для CS по умолчанию
// You can use Ethernet.init(pin) to configure the CS pin
//Ethernet.init(10);  // Most Arduino shields
#include <SPI.h>
#include <Ethernet.h>

// библиотеки для основных датчиков
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/*
 * Выбор режима работы контроллера
 * DEBUD - отладка по USB
 * FIRST, SECOND, THIRD - выбор конфигурации для прошики определенного этажа
 */
//#define DEBUG
//#define FIRST
#define SECOND
//#define THIRD

// количество датчиков
#define SENSOR_AMOUNT 4

// выбор типа датчика
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22     // DHT 22 (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// подключение датчиков
// dont use 10 pin! 
#define DHTPIN1 6
#define DHTPIN2 7
#define DHTPIN3 8
#define DHTPIN4 9

DHT dht[] = {
  {DHTPIN1, DHTTYPE},
  {DHTPIN2, DHTTYPE},
  {DHTPIN3, DHTTYPE},
  {DHTPIN4, DHTTYPE}
};

// хранение полученных значений
float humidity[SENSOR_AMOUNT];
float temperature[SENSOR_AMOUNT];

// Время опроса
#define TIME 10000

// конфигурация интернет соединения
// мак адрес для плат должен быть разным!
// assign a MAC address for the ethernet controller.
// fill in your address here:
#ifdef DEBUG
  byte mac[] = {0x0E,0xAD,0xBE,0x0F,0xFE,0xED};
#endif

#ifdef FIRST
  byte mac[] = {0x00,0x01,0x02,0x03,0x04,0x05};
#endif

#ifdef SECOND
  byte mac[] = {0x06,0x07,0x02,0x03,0x04,0x05};
#endif

#ifdef THIRD
  byte mac[] = {0x02,0x04,0x02,0x03,0x04,0x05};
#endif

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;
char server[] = "smart.1561.moscow";
//IPAddress server(64,131,82,241);

String DisplayAddress(IPAddress address)
{
    return String(address[0]) + "." + 
           String(address[1]) + "." + 
           String(address[2]) + "." + 
           String(address[3]);
}

// уникальный ключ для записи в базу данных
#ifdef DEBUG
  String apiKeyValue = "tPmAT5Ab3j7F9";  //тестовый ключ
#endif

#ifdef FIRST
  String apiKeyValue = "D1tPmAT5Ab3j7";  //первый этаж
#endif

#ifdef SECOND
  String apiKeyValue = "D2tPmAT5Ab3j7";  //второй этаж
#endif

#ifdef THIRD
  String apiKeyValue = "D3tPmAT5Ab3j7";  //третий этаж
#endif

// declare reset function at address 0
void(* resetFunc) (void) = 0;
char c = '1';
bool state = true;
#define RESET_PIN 4
#define LED_PIN 5

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  //инициализация датчиков
  for (auto& sensor : dht) {
    sensor.begin();
  }
  
  for (int i = 0; i < SENSOR_AMOUNT; i++) {
    temperature[i] = 0.0;
    humidity[i] = 0.0;
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
  // give the Ethernet shield a 1-1.5 second to initialize
  //рандомная пауза, иначе контроллеры забрасывают друг-друга пакетами
  randomSeed(analogRead(0));
  int randDelay = random(1000, 1500);
  Serial.println(randDelay);
  delay(randDelay);
  Serial.println("Ethernet shield initialized");

  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);
}

void loop()
{
client.stop();
client.flush();

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
                       + "&IP=" + String(DisplayAddress(Ethernet.localIP()))
                       + "";
                       
Serial.println(httpRequestData); 

if (client.connect(server, 80))
{
  Serial.println("Get data from file"); 
  client.print( "GET /add_data.php?");
  client.print("&");
  client.println( " HTTP/1.1");
  client.print( "Host: " );
  client.println(server);
  client.println( "Connection: close" );
  client.println();
  client.println();

  Serial.println("Connected to server");
  client.println("POST /post-data.php HTTP/1.1");
  client.println("Host: smart.1561.moscow");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(httpRequestData.length());
  client.println();
  client.println(httpRequestData);
  client.println();

delay(200);

while (client.available())
  {
    c = client.read();
    if ( c=='1')
    {
      state=true;
    }
    if ( c=='0')
    {
      state=false;
    }
  }
  
  client.stop();
  client.flush();
  delay(100);
  }
  else
  {
  //reconnect
    client.stop();
    Serial.println("reconnect ");
    delay(1000);
    client.connect(server, 80);
  }

  if (state==true)
  {
    digitalWrite (LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    Serial.println("resetting...");
    // blink LED before reseting
        for( int i =0; i < 5; i++) {
          digitalWrite(LED_PIN, LOW);
          delay(100);
          digitalWrite(LED_PIN, HIGH);
          delay(100);
        }
        //digitalWrite(RESET_PIN, LOW);
        resetFunc();  //call reset
  } 

  delay(TIME); 
}