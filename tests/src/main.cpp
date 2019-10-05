extern "C" {
#include <brotli/decode.h>
#include <brotli/encode.h>
}
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

#define DEFAULT_LGWIN 19
#define BROTLI_BUFFER 10000;
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
  
  //Serial.printf("%d bytes read into inBuffer from: ", fileSize);
  //Serial.print(fileName+ "\n");

  int decompressTime = micros();
  // Uncomment for easy test withouth SPIFFS
  //uint8_t inBuffer[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
  
  uint8_t *buffer = new uint8_t[30000];
  size_t output_length = 30000;

  brotliStatus = BrotliDecoderDecompress(
    fileSize,
    (const uint8_t *)inBuffer,
    &output_length,
    buffer);
  
  delete(inBuffer);
  delete(buffer);

  int timespent = micros()-decompressTime;

  //Serial.printf("Decompression took %d micros. File read took: %d micros\n", timespent, decompressTime-readFsTime);
  //Serial.printf("%d bytes after decompression. Estimated pixels RGBw: %d\n", output_length, (output_length-5)/4);
  int pixelslen = (output_length-5)/4;

  Serial.print(fileName+";"+String(pixelslen)+";"+String(fileSize)+";");
  Serial.print(String(output_length)+";");
  Serial.print(String(timespent)+";");
  Serial.print(String((pixelslen/300)*9)+";");
  Serial.println();

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
  int quality = 1; 
  int bufferSize = BROTLI_BUFFER;
  uint8_t *buffer = new uint8_t[bufferSize];
  size_t encodedSize = bufferSize;
  int lgwin = 2;


Serial.println("Calling BrotliEncoderCompress");
Serial.printf("Compression params:\n%d quality\n%d lgwin", quality, lgwin);

// ***ERROR*** lgwinabort() increasing quality >1
int decompressTime = micros();

   brotliStatus = BrotliEncoderCompress(
    quality,  
    lgwin, 
    BrotliEncoderMode::BROTLI_MODE_GENERIC,
    fileSize, 
    (const uint8_t *)inBuffer,     
    &encodedSize,
    buffer);  

  int timespent = micros()-decompressTime;

  delete(inBuffer);
  delete(buffer);
  Serial.println();
  Serial.printf("%d microseconds spend compressing\n", timespent);
  Serial.printf("%d bytes after compression\n", encodedSize);
/* 
  if (outputBytes) {
    for ( int i = 0; i < encodedSize; i++ ) {
      uint8_t conv = (int) buffer[i];
      Serial.print(conv);Serial.print(",");
    }
    Serial.printf("%.*s\n", encodedSize, buffer);
  }  */

  return brotliStatus;
}

void setup() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
Serial.println("Filename;Pixels;Compressed size(byte);Decompressed size;Decomp. micros;Neopixel process millis;");

/*   decompressFile("/500-1.bin.br");
  decompressFile("/500-r.bin.br");
  decompressFile("/1000-1.bin.br");
  decompressFile("/1000-r.bin.br");
  decompressFile("/2000-1.bin.br");
  decompressFile("/4000-1.bin.br");
  decompressFile("/6000-1.bin.br"); */
  // This one still does not work:
  compressFile("/144.txt");
}
 
void loop() {
  delay(100);
}