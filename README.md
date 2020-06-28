# ESP8266 
![Screenshot](img/ESP8266_pinout.PNG)
# Installation ESP8266
## Voraussetzungen
wpa2 based on 
https://github.com/jtuttas/ESP8266-WPA2-Enterprise


examples
https://esp8266.ru/forum/threads/akvarium-na-esp8266.4637/page-2

https://github.com/espressif/ESP8266_RTOS_SDK/blob/master/examples/wifi/wpa2_enterprise/main/wpa2_enterprise_main.c
https://cxemotexnika.org/2018/11/eksperimenty-s-wifi-modulyami-na-osnove-soc-esp8266/
https://github.com/beta-tank/ESP8266-WPA2-Enterprise-PEAP-demo
https://github.com/espressif/arduino-esp32/issues/1381
## Vorbereitungen

```c++
static const char* ssid = "MMBBS-Intern";
// Username for authentification
#define EAP_USERNAME "tuttas"
#define EAP_PASSWORD "geheim!"
```
## Verkabelung
![Wireing](doc/sketch32_Steckplatine.png)