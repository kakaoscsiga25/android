#include <SPI.h>

// Arduino pro mini: 11(MOSI) 12(MISO) 13(SCK)

// set up the speed, data order and data mode
SPISettings SPIsettings(2000000, MSBFIRST, SPI_MODE1);

// Mode     ClockPolarity   ClockPhase    Output Edge
// SPI_MODE0      0             0           Falling
// SPI_MODE1      0             1           Rising
// SPI_MODE2      1             0           Falling
// SPI_MODE3      1             1           Rising

const int CS_PIN = 10;

void setup() {
  pinMode(CS_PIN, OUTPUT); // chip select as output pin
  // initialize SPI:
  SPI.begin();
}

uint8_t stat, val1, val2, result;

void loop() {
  SPI.beginTransaction(SPIsettings);
  digitalWrite(CS_PIN, LOW); // enable the bus

  //SPI.transfer(<data>);     // write
  // param = SPI.transfor(0); // read

  digitalWrite(CS_PIN, LOW); // disable the bus
  SPI.endTransaction();
}
