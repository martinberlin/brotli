extern "C" {
#include <brotli/decode.h>
#include <brotli/encode.h>
}
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

#define DEFAULT_LGWIN 19
#define BROTLI_BUFFER 4000;
size_t fileSize;

File file;

bool decompressFile(String fileName, bool outputBytes=false) {
  if (!SPIFFS.exists(fileName)) {
    Serial.println(fileName+ " not found. Did you execute: pio run -t uploadfs");
    return false;
  }
  bool brotliStatus;
  int readFsTime = micros();
  file = SPIFFS.open(fileName, "r"); 
  fileSize = file.size();
  char *inBuffer = new char[fileSize];
  file.readBytes(inBuffer, fileSize);
  file.close();
  Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
  Serial.printf("%d bytes read into inBuffer from: ", fileSize);
  Serial.print(fileName+ "\n");

  int decompressTime = micros();
  // Uncomment for easy test withouth SPIFFS
  //uint8_t inBuffer[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
  
  uint8_t buffer[2000];
  size_t output_length = sizeof(buffer);

  brotliStatus = BrotliDecoderDecompress(
    fileSize,
    (const uint8_t *)inBuffer,
    &output_length,
    buffer);
  
  delete(inBuffer);

  int timespent = micros()-decompressTime;

  Serial.printf("Decompression took %d micros. File read took: %d micros\n", timespent, decompressTime-readFsTime);
  Serial.printf("%d bytes after decompression. Estimated pixels RGBw: %d\n", output_length, (output_length-5)/4);
  if (outputBytes) {
    for ( int i = 0; i < output_length; i++ ) {
      uint8_t conv = (int) buffer[i];
      Serial.print(conv);Serial.print(",");
    }
    Serial.printf("%.*s\n", output_length, buffer);
  }
  return brotliStatus;
}

bool compressFile(String fileName, bool outputBytes=false) {
  bool brotliStatus = false;
  // Failed compression (Still need to FIX)
  if (!SPIFFS.exists(fileName)) {
    Serial.println(fileName+ " not found. Did you execute: pio run -t uploadfs");
    return false;
  }
  Serial.println("Reading file '"+fileName+"' from FS");

  file = SPIFFS.open(fileName, "r"); 
  fileSize = file.size();
  char *inBuffer = new char[fileSize];
  file.readBytes(inBuffer, fileSize);
  file.close();
  Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
  Serial.printf("%d bytes read into inBuffer from: ", fileSize);
  Serial.print(fileName+ "\n");

// 1 less to 9 max. compression 
  int quality = 9; 
  
  uint8_t outBuffer[14000];
  size_t encodedSize = sizeof(outBuffer);
  int lgwin = DEFAULT_LGWIN;


Serial.println("Calling BrotliEncoderCompress");
Serial.printf("Compression params:\n%d quality\n%d lgwin", quality, lgwin);
delay(10);
// ***ERROR*** A stack overflow in task loopTask has been detected.
// abort() was called at PC 0x4008b980 on core 1
   brotliStatus = BrotliEncoderCompress(
    quality,  
    lgwin, 
    BrotliEncoderMode::BROTLI_MODE_GENERIC,
    fileSize, 
    (const uint8_t *)inBuffer,     
    &encodedSize,
    outBuffer);  

  delete(inBuffer);

  Serial.printf("%d bytes after compression\n", encodedSize);

  if (outputBytes) {
    for ( int i = 0; i < encodedSize; i++ ) {
      uint8_t conv = (int) outBuffer[i];
      Serial.print(conv);Serial.print(",");
    }
    Serial.printf("%.*s\n", encodedSize, outBuffer);
  } 

  return brotliStatus;
}

void setup() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

  decompressFile("/144-rgbw-2-on.bin.br", true);
  
  decompressFile("/144-rgbw-all-on.bin.br");
  decompressFile("/288-rgbw-2-on.bin.br");
  decompressFile("/288-rgbw-all-on.bin.br");
  decompressFile("/432-rgbw-2-on.bin.br");
  decompressFile("/432-rgbw-all-on.bin.br");
  decompressFile("/864-rgbw-2-on.bin.br");
  decompressFile("/864-rgbw-all-on.bin.br");
  decompressFile("/864-rgbw-all-random.bin.br"); 
  
  // This one still does not work:
  //compressFile("/144-rgbw-2-on.bin", false);
}
 
void loop() {
  delay(1);
}