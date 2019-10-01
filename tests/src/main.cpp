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

File file;

void setup() {
  Serial.begin(115200);
   
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

    String fileName = "/144-rgbw-2-on.bin";
    file = SPIFFS.open(fileName, "r"); 
    fileSize = file.size();
    char *inBuffer = new char[fileSize];
    file.readBytes(inBuffer, fileSize);
    file.close();
    Serial.printf("%d bytes read into inBuffer", fileSize);

  int time = millis();
  //uint8_t compressed[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
 
  uint8_t buffer [2000];
 
  size_t output_length = sizeof(buffer);
 
  BrotliDecoderDecompress(
    sizeof(inBuffer),
    (const uint8_t *)inBuffer,
    &output_length,
    buffer);
 
  Serial.printf("%.*s\n", output_length, buffer);
  int timespent = millis()-time;
  Serial.printf("Decompression took %d milliseconds.", timespent);
/*
  String fileName = "/144.txt";

  Serial.println("Reading file '"+fileName+"' on SPIFFS");
  char *inBuffer = new char[2000];


 if (SPIFFS.exists(fileName)) {
    file = SPIFFS.open(fileName, "r"); 
    fileSize = file.size();
    file.readBytes(inBuffer, fileSize);
    file.close();
    Serial.printf("%d bytes read into inBuffer", fileSize);
  } else {
    Serial.println("Could not read "+fileName+" from SPIFFS");
    return;
  }

// 1 less to 9 max. compression 
  int quality = 9; 
  
  size_t encodedSize;
  uint8_t outBuffer[3000];
  int lgwin = DEFAULT_LGWIN;
  size_t input_size = sizeof(inBuffer);

Serial.printf("%d bytes input_size", input_size);
bool returnValue = false;
 */

//***ERROR*** A stack overflow in task loopTask has been detected.
//abort() was called at PC 0x4008b980 on core 1

/* returnValue = BrotliEncoderCompress(
      quality,  
      lgwin, 
      BrotliEncoderMode(BROTLI_MODE_GENERIC),
      fileSize, 
      (const uint8_t *)inBuffer,     
      &encodedSize,
      outBuffer);  */
  

}
 
void loop() {
  delay(1);
}