
// This is the Wi-Fi channel the remote will use to broadcast its packages.
// Set it to the channel your ESP-Home receiver uses. Typically, this will
// be the channel used by your router.
#define WIFI_CHANNEL 13

// This is the baud rate the serial interface will use for communication.
#define BAUD_RATE 115200

// This is the space in bytes that will be reserved in EEPROM for storing
// data that should be persisted.
#define EEPROM_SIZE 32

// This is the offset at which the sequence number will be stored
// inside the EEPROM.
#define EEPROM_SEQUENCE_OFFSET 0

// Under normal circumstances the pins would be SDA=4 and SCL=5, but for
// some strange reason the PCA6414A is connected the other way around.
#define SDA_PIN 5
#define SCL_PIN 4

// PCA6416A - Low-voltage translating 16-bit I2C-bus/SMBus I/O expander
//   Datasheet: https://www.nxp.com/docs/en/data-sheet/PCA6416A.pdf
#define PCA6416A_I2C_ADDR       0x20
#define PCA6416A_INPORT0        0x00

// The ESP's Pin 16 is connected to the enable pin of a voltage regulator.
// Setting this Pin to HIGH lets the remote stay awake.
#define VOLTAGE_REGULATOR_PIN 16