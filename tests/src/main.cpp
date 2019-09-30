extern "C" {
#include "brotli/decode.h"
}
#include "Arduino.h"
 
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
}
 
void loop() {
  delay(199);
}