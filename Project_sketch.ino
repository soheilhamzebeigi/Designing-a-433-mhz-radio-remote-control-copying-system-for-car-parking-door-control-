/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h> 
#include <AceButton.h>
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <FastLED.h>
#include <Preferences.h>



// RF Code
RCSwitch receiverRF = RCSwitch();
RCSwitch transmiterRF = RCSwitch();



// ACE Button Code
using namespace ace_button;
const int BUTTON_ONE_PIN = 32;
const int BUTTON_TWO_PIN = 35;
// ---------------------------

// RF Pins
const int RF_PIN_RECEIVE = 4;
const int RF_PIN_TRANSMIT = 2;
// ---------------------------

const int LED_PIN = 2;
const int LED_ON = HIGH;
const int LED_OFF = LOW;





/************************* WiFi Access Point *********************************/

#define WLAN_SSID "esp32"
#define WLAN_PASS "12345678"
// #define WLAN_SSID "AndroidAPE2DB"
// #define WLAN_PASS "ioeo1875"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 8883
#define AIO_USERNAME "wotox"
#define AIO_KEY "aio_OOSE07Gwa9258cJ5jT1uU7mTf8uj"

/************ Global State (you don't need to change this!) ******************/

WiFiClientSecure client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// io.adafruit.com root CA
const char* adafruitio_root_ca =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
  "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
  "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
  "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
  "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
  "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
  "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
  "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
  "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
  "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
  "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
  "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
  "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
  "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
  "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
  "-----END CERTIFICATE-----\n";







/****************************** Feeds ***************************************/


// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish parkingStateTopic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/parking-state");



// ----------------------------- Subscribes------------------------
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe parkingOpenSub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/parking-open");
Adafruit_MQTT_Subscribe parkingCloseSub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/parking-close");





// ------------------------------ Pereference --------------------------
Preferences preferences;
// ---------------------------------------------------------------------



// ------------------------------ Buttons Codes -----------------------------------------
AceButton buttonOne(BUTTON_ONE_PIN);
AceButton buttonTwo(BUTTON_TWO_PIN);
// --------------------------------------------------------------------------------------------


// ------------------------------ Radio FR Codes For Buttons ----------------------------------
int firstButtonCode = 0;
int secondButtonCode = 0;
// --------------------------------------------------------------------------------------------


// ---------------------------------- Recording Siginal Global & Buttons States ---------------
bool firstButtonRecordingState = false;
bool secondButtonRecordingState = false;
bool globalButtonRecordingState = false;
// --------------------------------------------------------------------------------------------



void setup() {
  Serial.begin(115200);
  preferences.begin("rf-code", false);


  pinMode(LED_PIN, OUTPUT);


  firstButtonCode = preferences.getInt("firstButtonCode", 8895272);
  secondButtonCode = preferences.getInt("secondButtonCode", 8895265);

  preferences.end();

  Serial.println(firstButtonCode);
  Serial.println(secondButtonCode);

  // RF Code
  receiverRF.enableReceive(RF_PIN_RECEIVE);
  transmiterRF.enableTransmit(RF_PIN_TRANSMIT);


  initWifi();
  initMQTT();
  initButtons();
}


void loop() {
  buttonOne.check();
  buttonTwo.check();
  MQTT_connect();
  recordSiginal();
  handleGlobalStateLed();
  handleSubscribers();
}