#include <WizMote.h>

uint8_t WizMoteClass::broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

WizMoteClass::WizMoteClass() {}

void WizMoteClass::begin() {

    // Prevent calling this method a second time
    if (initialized) {
        return;
    }

    // Set the voltage regulator pin as an output pin
    pinMode(VOLTAGE_REGULATOR_PIN, OUTPUT);

    // Enable the voltage regulator, so the remote stays awake
    digitalWrite(VOLTAGE_REGULATOR_PIN, HIGH);

    // Initialize I2C Bus
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize EEPROM
    EEPROM.begin(EEPROM_SIZE);

    // Set initialization status
    initialized = true;
}

void WizMoteClass::initializeEspNow(int channel) {

    // Set Wi-Fi Channel
    if (wifi_set_channel(channel) != true) {
        printException("setting Wi-Fi channel failed");
    }

    // Set device as a Wi-Fi Station
    if (WiFi.mode(WIFI_STA) != true) {
        printException("setting Wi-Fi mode failed");
    }

    // Immediately disconnect from any networks
    if (WiFi.disconnect() != true) {
        printException("disconnecting Wi-Fi failed");
    }

    // Initialize ESP-NOW
    if (esp_now_init() != OK) {
        printException("initializing ESP-NOW failed");    
    }

    // Set this device's role to CONTROLLER
    if (esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER) != OK) {
        printException("setting ESP-NOW role failed");
    };

    // Register the broadcast address as a peer with role SLAVE
    if (esp_now_add_peer(WizMoteClass::broadcastAddress, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0)  != OK) {
        printException("adding ESP-NOW peer failed");
    }
}

uint8_t WizMoteClass::readButtonPress() {

    // Select input port 0
    Wire.beginTransmission(PCA6416A_I2C_ADDR);
    Wire.write(PCA6416A_INPORT0);
    Wire.endTransmission();

    // Request one byte and read input port 0 value
    Wire.requestFrom(PCA6416A_I2C_ADDR, 1);
    return ~Wire.read();
}

uint32_t WizMoteClass::nextSequenceNumber() {

    // Read sequence number from EEPROM
    EEPROM.get(EEPROM_SEQUENCE_OFFSET, sequenceNumber);

    // Increment sequence number
    sequenceNumber++;

    // Write back sequence number into EEPROM
    EEPROM.put(EEPROM_SEQUENCE_OFFSET, sequenceNumber);
    EEPROM.commit();

    return sequenceNumber;
}

void WizMoteClass::broadcast(uint8_t *data, size_t data_size) {
    if (esp_now_send(WizMoteClass::broadcastAddress, data, data_size) != OK) {
        printException("sending ESP-NOW message failed");
    }
}

void WizMoteClass::powerOff() {

    // Disable the voltage regulator, so the remote turns off
    digitalWrite(VOLTAGE_REGULATOR_PIN, LOW);
}

void WizMoteClass::registerSendCallback(esp_now_send_cb_t cb) {
    if (esp_now_register_send_cb(cb) != OK) {
        printException("registering ESP-NOW send callback failed");
    }
}

void WizMoteClass::printException(const char* message) {
    Serial.println();
    Serial.println();
    Serial.println("========================");
    Serial.println("  An unexpected error occured.");
    Serial.printf("  message: %s\n", message);
    Serial.println("========================");
    Serial.println();
    Serial.println("System will restart in 5 seconds...");

    delay(5000);
    system_restart();
}