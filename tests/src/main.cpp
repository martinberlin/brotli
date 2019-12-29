#include <brotli/decode.h>
//#include <brotli/encode.h>

#include "Arduino.h"
#include "FS.h"
#ifdef ESP32
  #include "SPIFFS.h"
#endif
#define DEFAULT_LGWIN 22
#define BROTLI_BUFFER 30000;
size_t fileSize;

File file;
 bool decompressFile(String fileName, bool outputBytes=false) {
  if (!SPIFFS.exists(fileName)) {
    Serial.println(fileName+ " not found. Did you execute: pio run -t uploadfs");
    return false;
  }
  bool brotliStatus;
  file = SPIFFS.open(fileName, "r"); 
  fileSize = file.size();
  char *inBuffer = new char[fileSize];
  file.readBytes(inBuffer, fileSize);
  file.close();

  int decompressTime = micros();
  // Uncomment for easy test withouth SPIFFS
  //uint8_t inBuffer[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
  int bufferSize = BROTLI_BUFFER;
  uint8_t *buffer = new uint8_t[bufferSize];
  size_t output_length = bufferSize;

  brotliStatus = BrotliDecoderDecompress(
    fileSize,
    (const uint8_t *)inBuffer,
    &output_length,
    buffer);
  
  delete(inBuffer);
  delete(buffer);

  int timespent = micros()-decompressTime;
  int pixelslen = (output_length-5)/4;

  //Serial.printf("Decompression took %d micros. File read took: %d micros\n", timespent, decompressTime-readFsTime);
  //Serial.printf("%d bytes after decompression. Estimated pixels RGBw: %d\n", output_length, (output_length-5)/4);
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

void setup() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
Serial.println("Filename;Pixels;Compressed size(byte);Decompressed size;Decomp. micros;Neopixel process millis;");

// Decompress example with other files (Outputs a small CSV)
   decompressFile("/500-1.bin.br");
  decompressFile("/500-r.bin.br");
  decompressFile("/1000-1.bin.br");
  decompressFile("/1000-r.bin.br");
  decompressFile("/2000-1.bin.br");
  decompressFile("/4000-1.bin.br");
  decompressFile("/6000-1.bin.br"); 
  
  // Read a file from FS
  int bufferSize = BROTLI_BUFFER;
  uint8_t *outBytes = new uint8_t[bufferSize];
  
  // Decompress the compressed bytes output
  //decompressFile("/144.txt.br", outBytes);
}
 
void loop() {
  delay(100);
}