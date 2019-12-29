#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_STDIO
#include "miniz.c"
//#include "Arduino.h"
#include "FS.h"
#ifdef ESP32
  #include "SPIFFS.h"
#endif

#define DECOMPRESSION_BUFFER 4000;
size_t fileSize;
File file;

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
  int bufferSize = DECOMPRESSION_BUFFER;
  uint8_t *buffer = new uint8_t[bufferSize];
  uLong output_length;

    int cmp_status = uncompress(
				buffer, 
				&output_length, 
				(const unsigned char*)inBuffer, 
				fileSize);

	// status:
	// { MZ_OK = 0, MZ_STREAM_END = 1, MZ_NEED_DICT = 2, MZ_ERRNO = -1, MZ_STREAM_ERROR = -2, MZ_DATA_ERROR = -3, 
  //   MZ_MEM_ERROR = -4, MZ_BUF_ERROR = -5, MZ_VERSION_ERROR = -6, MZ_PARAM_ERROR = -10000 };

  Serial.printf("Decompression status %d took %d micros. %d bytes after decompression\n", cmp_status, micros() - decompressTime,output_length);

  if (cmp_status == 0) {
    Serial.printf("%.*s", output_length, buffer);
  }

  delete(inBuffer);
  delete(buffer);
  return cmp_status;
}


void setup() {
  Serial.begin(115200);delay(10);
  
  if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }

Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");

// Decompress example
decompressFileZlib("/readme.z");
Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - -");
decompressFileZlib("/sensors.z");
}
 
void loop() {
  delay(100);
}