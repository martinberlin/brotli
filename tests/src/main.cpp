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

bool decompressFile(String fileName, bool outputBytes=false) {
  if (!SPIFFS.exists(fileName)) {
    Serial.println(fileName+ " not found. Did you execute: pio run -t uploadfs");
    return false;
  }
  bool brotliStatus = true;
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
}
 
void loop() {
  delay(1);
}

void compressFile(String fileName) {
  // Failed compression (Still need to FIX)

  Serial.println("Reading file '"+fileName+"' on SPIFFS");
  char *inBuffer = new char[2000];


 if (SPIFFS.exists(fileName)) {
    file = SPIFFS.open(fileName, "r"); 
    fileSize = file.size();
    file.readBytes(inBuffer, fileSize);
    file.close();
    Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    Serial.printf("%d bytes read into inBuffer from: ", fileSize);
    Serial.print(fileName+ "\n");
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