# NvMemFlashDict

A simple dictionary-like class for storing and retrieving key-value pairs in non-volatile memory.

Includes support for:

- ESP8266 (tested): 
    - Makes use of the [Embedis](https://github.com/thingSoC/embedis) library.
- ESP32 (tested): 
    - Makes use of the [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences).
- Generic Arduino (untested):
    - Should work with all platforms supported by [Embedis](https://github.com/thingSoC/embedis).