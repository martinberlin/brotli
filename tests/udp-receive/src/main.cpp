/**
 * Example of UDP receive plus Brotli decompress
 * Jst as a proof-of-concept, do not use without proper testing
 * To send a brotli compressed file demo use:
  
   cat ../144.txt.br |nc -w1 -u ESP_IP_ADDRESS 1234

 */
#include "WiFi.h"
#include "AsyncUDP.h"
#include "brotli/decode.h"
const char * ssid = "KabelBox-A210";
const char * password = "";

AsyncUDP udp;
TaskHandle_t brotliTask;

boolean debugMode = true;
uint8_t * compressed;
// Brotli decompression buffer
size_t bufferLength = 2000;
BrotliDecoderResult brotli;
// Temporary variables
size_t receivedLength;
bool uncompress = false;
/**
 * Generic message printer. Modify this if you want to send this messages elsewhere (Display)
 */
void printMessage(String message, bool newline = true)
{
  if (debugMode) {
    if (newline) {
      Serial.println(message);
    } else {
      Serial.print(message);
    }
   }
}

void brTask( void * pvParameters ){
    uint8_t * brOutBuffer = (uint8_t*)malloc(bufferLength);  
      
      brotli = BrotliDecoderDecompress(
        receivedLength,
        (const uint8_t *)compressed,
        &bufferLength,
        brOutBuffer);
        //free(compressed);

        printMessage("Unbrotli result: "+String(brotli)); // Should return 1 on correct decompression
        
        if (brotli == 0) {
          printMessage("Decompresion failed");
        }
        printMessage("Uncompressing:");
        Serial.printf("%.*s\n", bufferLength, brOutBuffer);

        free(brOutBuffer);
        // https://www.freertos.org/implementing-a-FreeRTOS-task.html
        // If it is necessary for a task to exit then have the task call vTaskDelete( NULL )
        vTaskDelete( NULL );
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("Firmware online");

    if(udp.listen(1234)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());

        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());

            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
        // Save compressed in memory instead of simply: uint8_t compressed[compressedBytes.size()];
      receivedLength = packet.length();

      compressed  = (uint8_t*)malloc(receivedLength);
      for ( int i = 0; i < packet.length(); i++ ) {
          uint8_t conv = (int) packet.data()[i];
          compressed[i] = conv;
          //Serial.print(conv);Serial.print(",");
      }
        uncompress = true;

          xTaskCreatePinnedToCore(
                    brTask,   /* Task function. */
                    "uncompress",     /* name of task. */
                    20000,       /* Stack size of task */
                    compressed,  /* parameter of the task */
                    9,           /* priority of the task */
                    &brotliTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 1 */

        //reply to the client
            packet.printf("Received %u bytes", packet.length());
        }); 
    }
}

void loop()
{
    delay(2);
}

