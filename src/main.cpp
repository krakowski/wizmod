#include <Arduino.h>
#include <WizMote.h>

#include <configuration.h>

typedef struct __attribute__ ((packed)) {
  uint32_t sequence;
  uint8_t button;
} broadcast_message;

broadcast_message broadcast_data;

WizMoteClass WizMote;

void on_data_sent(uint8_t *mac_addr, uint8_t sendStatus) {

  // Turn the WizMote off once the packet has been sent
  WizMote.powerOff();
}

void setup() {

  // Initialize Serial communication
  Serial.begin(BAUD_RATE);

  // Initialize the WizMote
  WizMote.begin();

  // Read button press as soon as possible
  broadcast_data.button = WizMote.readButtonPress();

  // Set sequence number
  broadcast_data.sequence = WizMote.nextSequenceNumber();

  // Initialize ESP-NOW
  WizMote.initializeEspNow(WIFI_CHANNEL);

  // Register send callback
  WizMote.registerSendCallback(on_data_sent);

  // Broadcast message to ESP-NOW receivers
  WizMote.broadcast((uint8_t *) &broadcast_data, sizeof(broadcast_message));
}

void loop() {}