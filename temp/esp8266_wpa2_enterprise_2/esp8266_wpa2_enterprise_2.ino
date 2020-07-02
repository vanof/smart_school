// eduroam WPA2 Enterprise, TTLS, no certificate, MSCHAPv2(no EAP)
#include <ESP8266WiFi.h>
#define EAP_IDENTITY "jonny" // ID eg: "jonny" not jonny@univ.xx
#define EAP_PASSWORD "passwd"

extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

String line;
const char* ssid = "eduroam";

void setup() {
Serial.begin(115200);
delay(10);
Serial.println();
Serial.println(ssid);
WiFi.disconnect(true);
WiFi.mode(WIFI_STA);
esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
esp_wifi_sta_wpa2_ent_enable(&config);

Serial.println("MAC address: ");
Serial.println(WiFi.macAddress());
WiFi.begin(ssid);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

}

void loop() {

}
