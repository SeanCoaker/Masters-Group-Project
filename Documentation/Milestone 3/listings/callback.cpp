void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
    memcpy(&message, data, sizeof(message));
    Serial.println(message);
    comms.setReminder(true);
}