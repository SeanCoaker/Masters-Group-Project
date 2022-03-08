#include <esp_now.h>
#include <WiFi.h>
#include <TinyPICO.h>

TinyPICO tp = TinyPICO();

String message;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
  memcpy(&message, data, sizeof(message));
  Serial.println(message);
  
  flash();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  tp.DotStar_SetPixelColor(0,128,0);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}

void flash() {
    tp.DotStar_SetBrightness(128);
    tp.DotStar_Show();
    delay(500);
    tp.DotStar_SetBrightness(0);
    tp.DotStar_Show();
}