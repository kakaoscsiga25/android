
#include <SPI.h>


//#define dc   A0                // Define pins for TFT display.           
//#define cs   A1                // 
//#define rst  A2  

//#include <Adafruit_GFX.h>      // Core graphics library

// include Adafruit library OR QDTech library depending on the display's controller chip.

//  #include <Adafruit_ST7735.h>            // Hardware-specific library
//  Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

//  #include <Adafruit_QDTech.h>              // Hardware-specific library
//  Adafruit_QDTech tft = Adafruit_QDTech(cs, dc, rst);  


// https://github.com/zigwart/Adafruit_QDTech


const int SINE = 0x2000;                    // Define AD9833's waveform register value.
const int SQUARE = 0x2020;                  // When we update the frequency, we need to
const int TRIANGLE = 0x2002;                // define the waveform when we end writing.    


const float refFreq = 25000000.0;           // On-board crystal reference frequency

const int FSYNC = 9;                       // Standard SPI pins for the AD9833 waveform generator.
const int CLK = 13;                         // CLK and DATA pins are shared with the TFT display.
const int DATA = 11;
const int DISABLED_PIN = 10;

unsigned long freq = 1e6;               // Set initial frequency.

//SPISettings SPIsettings(500000, MSBFIRST, SPI_MODE2);


void setup() {
  pinMode(FSYNC, OUTPUT);
   
  delay(500);
//  Serial.begin(9600);
//  Serial.print("init");
  
   // Can't set SPI MODE here because the display and the AD9833 use different MODES.
  SPI.begin();
  delay(50); 
  
    
  AD9833reset();                                   // Reset AD9833 module after power-up.
  delay(50);
  AD9833setFrequency(freq, SINE);                  // Set the frequency and Sine Wave output

//  Serial.print("init end");
}


void loop() {
//  delay(1000);

//  AD9833setFrequency(freq, SINE);
}

// AD9833 documentation advises a 'Reset' on first applying power.
void AD9833reset() {
  WriteRegister(0x100);   // Write '1' to AD9833 Control register bit D8.
  delay(10);
}

// Set the frequency and waveform registers in the AD9833.
void AD9833setFrequency(long frequency, int Waveform) {
  
  long FreqWord = (frequency * pow(2, 28)) / refFreq;

  int MSB = (int)((FreqWord & 0xFFFC000) >> 14);    //Only lower 14 bits are used for data
  int LSB = (int)(FreqWord & 0x3FFF);
  
  //Set control bits 15 ande 14 to 0 and 1, respectively, for frequency register 0
  LSB |= 0x4000;
  MSB |= 0x4000; 
  
  WriteRegister(0x2100);   
  WriteRegister(LSB);                  // Write lower 16 bits to AD9833 registers
  WriteRegister(MSB);                  // Write upper 16 bits to AD9833 registers.
  WriteRegister(0xC000);               // Phase register
  WriteRegister(Waveform);             // Exit & Reset to SINE, SQUARE or TRIANGLE

}

void WriteRegister(int dat) { 

//  SPI.beginTransaction(SPIsettings);
  
//  Serial.println("write");
  
  // Display and AD9833 use different SPI MODES so it has to be set for the AD9833 here.
  SPI.setDataMode(SPI_MODE2);       

  digitalWrite(DISABLED_PIN, HIGH);
  digitalWrite(FSYNC, LOW);           // Set FSYNC low before writing to AD9833 registers
  delayMicroseconds(10);              // Give AD9833 time to get ready to receive data.
  
  SPI.transfer(highByte(dat));        // Each AD9833 register is 32 bits wide and each 16
  SPI.transfer(lowByte(dat));         // bits has to be transferred as 2 x 8-bit bytes.

  digitalWrite(FSYNC, HIGH);          //Write done. Set FSYNC high

//  SPI.endTransaction();
}


// Interrupt service routine for the 'frequency' rotary encoder.

 
