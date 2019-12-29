#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_STDIO
#include <brotli/decode.h>
//#include <brotli/encode.h>
#include "miniz.c"
#include "Arduino.h"
#include "FS.h"
#ifdef ESP32
  #include "SPIFFS.h"
#endif
#define DEFAULT_LGWIN 22
#define BROTLI_BUFFER 4000;
size_t fileSize;

File file;

 bool decompressFileBrotli(String fileName, bool outputBytes=false) {
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

 bool decompressFileZlib(String fileName, bool outputBytes=false) {
  if (!SPIFFS.exists(fileName)) {
    Serial.println(fileName+ " not found. Did you execute: pio run -t uploadfs");
    return false;
  }
  file = SPIFFS.open(fileName, "r"); 
  fileSize = file.size();
  char *inBuffer = new char[fileSize];
  file.readBytes(inBuffer, fileSize);
  file.close();

  int decompressTime = micros();
  int bufferSize = BROTLI_BUFFER;
  uint8_t *buffer = new uint8_t[bufferSize];
  uLong output_length;

    int cmp_status = uncompress(
				buffer, 
				&output_length, 
				(const unsigned char*)inBuffer, 
				fileSize);
  
  delete(inBuffer);
  delete(buffer);
	// status:
	// { MZ_OK = 0, MZ_STREAM_END = 1, MZ_NEED_DICT = 2, MZ_ERRNO = -1, MZ_STREAM_ERROR = -2, MZ_DATA_ERROR = -3, 
  //   MZ_MEM_ERROR = -4, MZ_BUF_ERROR = -5, MZ_VERSION_ERROR = -6, MZ_PARAM_ERROR = -10000 };

  Serial.printf("Decompression status %d took %d micros. %d bytes after decompression\n", cmp_status, micros() - decompressTime,output_length);

  if (cmp_status == 0) {
    Serial.printf("%.*s", output_length, buffer);
  }
  return cmp_status;
}


void setup() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");

// Decompress example with other files (Outputs a small CSV)
decompressFileZlib("/sensors.z");
//decompressFileZlib("/main.z");
}
 
void loop() {
  delay(100);
}