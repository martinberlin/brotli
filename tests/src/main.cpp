extern "C" {
#include <brotli/decode.h>
#include <brotli/encode.h>
}
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

#define DEFAULT_LGWIN 24
#define COMPRESSION_BUFFER 2000;
size_t fileSize;

void setup() {
  Serial.begin(115200);
   
  int time = millis();
  uint8_t compressed[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
 
  uint8_t buffer [2000];
 
  size_t output_length = sizeof(buffer);
 
  BrotliDecoderDecompress(
    sizeof(compressed),
    (const uint8_t *)compressed,
    &output_length,
    buffer);
 
  Serial.printf("%.*s\n", output_length, buffer);
  int timespent = millis()-time;
  Serial.printf("Decompression took %d milliseconds.", timespent);

  String fileName = "/144.txt.br";

  Serial.println("Reading file '"+fileName+"' on SPIFFS");
  

 if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }
  if (!SPIFFS.exists(fileName)) {
    Serial.println("Could not read "+fileName+" from SPIFFS");
  }


 File file = SPIFFS.open(fileName, "r"); 
    fileSize = file.size();
    char *inFileBuffer = new char[fileSize];
    file.readBytes(inFileBuffer, fileSize);
    file.close();
    Serial.printf("%d bytes read into inBuffer: ", fileSize);

    uint8_t inBuffer [fileSize];
      // Print out inBuffer to serial
    for ( int i = 0; i < fileSize; i++ ) {
        inBuffer[i] = (int) inFileBuffer[i];
        Serial.print(inBuffer[i]);Serial.print(",");
    } 

  int quality = 5; /* 1 less to 9 max. compression */
  size_t encodedSize;
  uint8_t outBuffer[900];
  int lgwin = 11;
  

Serial.println();
Serial.println("Heap: "+String(ESP.getFreeHeap())); 
//***ERROR*** A stack overflow in task loopTask has been detected.
//abort() was called at PC 0x4008b980 on core 1
  /* BrotliEncoderCompress(
      quality,  
      lgwin, 
      BrotliEncoderMode::BROTLI_MODE_FONT,
      fileSize, 
      (const uint8_t *)inFileBuffer,     
      &encodedSize,
      outBuffer);   */

//TODO: Try: 
//BrotliEncoderCompressStream()
}
 
void loop() {
  delay(100);
}