extern "C" {
#include "brotli/decode.h"
}
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

void setup() {
  Serial.begin(115200);
   
  long time = millis();
  uint8_t compressed[] = {27, 175, 4,248, 141, 148, 110, 222, 68, 85, 134, 214, 32, 33, 108, 111, 106, 22, 199, 106, 129, 12, 168, 102, 47, 4};
 
  uint8_t buffer [2000];
 
  size_t output_length = sizeof(buffer);
 
  BrotliDecoderDecompress(
    sizeof(compressed),
    (const uint8_t *)compressed,
    &output_length,
    buffer);
 
  Serial.printf("%.*s\n", output_length, buffer);
  long timespent = millis()-time;
  Serial.printf("Decompression took %d milliseconds.", timespent);



  String fileName = "/144.txt";

  Serial.println("Reading file '"+fileName+"' on SPIFFS");
  char *readBuffer = new char[2000];

 if (!SPIFFS.begin()) {
   Serial.println("Could not mount file system");
 }
  if (SPIFFS.exists(fileName)) {
    File file = SPIFFS.open(fileName, "r");
    int fileSize = file.size(); 
    
    file.readBytes(readBuffer, fileSize);
    Serial.printf("%d bytes read into readBuffer", fileSize);

    file.close();
  } else {
    Serial.println("Could not read "+fileName+" from SPIFFS");
    return;
  }
  
  

}
 
void loop() {
  delay(199);
}