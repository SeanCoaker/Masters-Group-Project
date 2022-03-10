#include <esp_now.h>
#include <WiFi.h>
#include <TinyPICO.h>

// Creates an instance of the TinyPICO class for LED flash functionality.
TinyPICO tp = TinyPICO();

// A variable to store the received message in.
String receivedMessage;

// Callback function that will be executed when data is received
void OnDataReceived(const uint8_t * mac, const uint8_t *data, int len) {
  memcpy(&receivedMessage, data, sizeof(receivedMessage));
  Serial.println(receivedMessage);
  
  flash();
}
 
void setup() {
  // Initialise Serial Monitor
  Serial.begin(115200);

  // Set the colour of the LED to green
  tp.DotStar_SetPixelColor(0,128,0);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Initialise ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP-NOW");
    return;
  }
  
  // Registers a callback function to use when receive callback is called.
  esp_now_register_recv_cb(OnDataReceived);
}
 
void loop() {

}

/*
  A function that causes the LED of the TinyPICO to blink.
*/
void flash() {
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
    delay(500);
    tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();
}