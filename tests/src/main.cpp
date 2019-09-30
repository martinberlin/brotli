extern "C" {
#include <brotli/decode.h>
#include <brotli/encode.h>
}
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

#define DEFAULT_LGWIN 24
#define COMPRESSION_BUFFER 2000;
int fileSize;

void setup() {
  Serial.begin(115200);
   
  int time = millis();
  uint8_t compressed[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
 
  uint8_t buffer [2000];
  //uint8_t buffer  = new uint8_t[2000]; // For some reason is not the same
  size_t output_length = sizeof(buffer);
 
  BrotliDecoderDecompress(
    sizeof(compressed),
    (const uint8_t *)compressed,
    &output_length,
    buffer);
 
  Serial.printf("%.*s\n", output_length, buffer);
  delete buffer;

  int timespent = millis()-time;
  Serial.printf("Decompression took %d milliseconds.", timespent);

  String fileName = "/144.txt";

  Serial.println("Reading file '"+fileName+"' on SPIFFS");
  char *inBuffer = new char[2000];

 if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }
  if (SPIFFS.exists(fileName)) {
    File file = SPIFFS.open(fileName, "r"); 
    fileSize = file.size();
    file.readBytes(inBuffer, fileSize);
    file.close();
    Serial.printf("%d bytes read into inBuffer", fileSize);
  } else {
    Serial.println("Could not read "+fileName+" from SPIFFS");
    return;
  }
  
  // Print out inBuffer to serial
    for ( int i = 0; i < fileSize; i++ ) {
        uint8_t conv = (int) inBuffer[i];
        Serial.print(conv);Serial.print(",");
    }

  int quality = 1; /* 1 less to 9 max. compression */
  size_t encodedSize;

  uint8_t outBuffer [1000];
  int lgwin = 19;

Serial.printf("%d bytes input_size", fileSize);
bool returnValue = false;

//***ERROR*** A stack overflow in task loopTask has been detected.
//abort() was called at PC 0x4008b980 on core 1

 returnValue = BrotliEncoderCompress(
      quality,  
      lgwin, 
      BrotliEncoderMode::BROTLI_MODE_GENERIC,
      fileSize, 
      (const uint8_t *)inBuffer,     
      &encodedSize,
      outBuffer);  
  delete inBuffer;

  Serial.println(returnValue);

}
 
void loop() {
  delay(1);
}