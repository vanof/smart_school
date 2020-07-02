#include <Ethernet.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

String apiKeyValue = "tPmAT5Ab3j7F9";
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
const int oneWireBus = 2;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
EthernetClient client;
char server[] = "smart.1561.moscow";

void setup() {
  Serial.begin(9600);
  sensors.begin();

  if(Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1);
  }
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  delay(2000);
}

void loop() {  
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  
  String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(sensors.getTempCByIndex(0))
                           + "&value2=" + String(20) + "&value3=" + String(30) + "";
                           
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

  
  while(client.connected()){
    if(client.available()){
      char c = client.read();
      Serial.print(c);  
    }
  }
  delay(180000);
}
