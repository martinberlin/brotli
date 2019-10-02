## Testing 

Only tested in ESP32 boards. 
Just use Plataformio or Arduino and main.cpp as sketch implementation example. To run it, flash the FS data folder (Spiffs) with Platformio or Arduino:

    pio run --target uploadfs

and then the program itself:

    pio run --target upload

More examples only per request using the Git issues. Use this basis to add your own compressed files in the /data folder and update the references to them in the setup() part of the Firmware.

### Numbers speak

This statistics can be run following the previous testing section. Brotli is very fast in the ESP32 decompressing small files. This ones where created to test the compression of the [UDPX transmission protocol](https://github.com/martinberlin/udpx) to send RGB(w) Neopixels over WiFi in the ESP32.

```
14 bytes read into inBuffer from: /144-rgbw-2-on.bin.br
Decompression took 1066 micros. File read took: 1218 micros
437 bytes after decompression. Estimated pixels RGBw: 108
Brotli decompressed bytes:
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,P
- - - - - - - - - - - - - - - - - - - - - - - - - -
21 bytes read into inBuffer from: /144-rgbw-all-on.bin.br
Decompression took 706 micros. File read took: 5614 micros
437 bytes after decompression. Estimated pixels RGBw: 108
- - - - - - - - - - - - - - - - - - - - - - - - - -
28 bytes read into inBuffer from: /288-rgbw-2-on.bin.br
Decompression took 593 micros. File read took: 1002 micros
869 bytes after decompression. Estimated pixels RGBw: 216
- - - - - - - - - - - - - - - - - - - - - - - - - -
30 bytes read into inBuffer from: /288-rgbw-all-on.bin.br
Decompression took 607 micros. File read took: 8035 micros
869 bytes after decompression. Estimated pixels RGBw: 216
- - - - - - - - - - - - - - - - - - - - - - - - - -
16 bytes read into inBuffer from: /432-rgbw-2-on.bin.br
Decompression took 542 micros. File read took: 1010 micros
1301 bytes after decompression. Estimated pixels RGBw: 324
- - - - - - - - - - - - - - - - - - - - - - - - - -
40 bytes read into inBuffer from: /432-rgbw-all-on.bin.br
Decompression took 896 micros. File read took: 7977 micros
1301 bytes after decompression. Estimated pixels RGBw: 324
- - - - - - - - - - - - - - - - - - - - - - - - - -
32 bytes read into inBuffer from: /864-rgbw-2-on.bin.br
Decompression took 761 micros. File read took: 8302 micros
2000 bytes after decompression. Estimated pixels RGBw: 498
- - - - - - - - - - - - - - - - - - - - - - - - - -
34 bytes read into inBuffer from: /864-rgbw-all-on.bin.br
Decompression took 701 micros. File read took: 994 micros
2000 bytes after decompression. Estimated pixels RGBw: 498
- - - - - - - - - - - - - - - - - - - - - - - - - -
2602 bytes read into inBuffer from: /864-rgbw-all-random.bin.br
Decompression took 284 micros. File read took: 8678 micros
2000 bytes after decompression. Estimated pixels RGBw: 498
```

### Credits

1.- Original library
https://github.com/google/brotli

2.- Article in techtutorials (compressed example copied and adapted from there)
https://techtutorialsx.com/2018/12/05/esp32-brotli-decompression

Library forked for Espressif boards by Martin [Fasani](https://fasani.de)

