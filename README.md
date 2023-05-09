# Kuri
## Configuration
### Library Installation
Install the [Esp32 Servo library](https://github.com/madhephaestus/ESP32Servo)

Install [PS4-esp32 library](https://github.com/aed3/PS4-esp32) then navigate to its directory and edit the following lines in ps4_spp.c:
```c
#if  CONFIG_IDF_COMPATIBILITY  >=  IDF_COMPATIBILITY_MASTER_D9CE0BB
esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
#elif  CONFIG_IDF_COMPATIBILITY  >=  IDF_COMPATIBILITY_MASTER_21AF1D7
esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE);
#endif
```
to be:
```c
//#if  CONFIG_IDF_COMPATIBILITY  >=  IDF_COMPATIBILITY_MASTER_D9CE0BB
//esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
//#elif  CONFIG_IDF_COMPATIBILITY  >=  IDF_COMPATIBILITY_MASTER_21AF1D7
esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE);
//#endif
```

### To Install Board
Go to Preferences and in the field "Additional Boards Manager URLs:", input ``https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json``

Then in the Arduino IDE board manager, install "esp32" **version 1.0.4**

Find the MAC address of the Esp32 board using getESP32MAC.ino and mark it down

### Prepare Code
Install [SixAxisPairer](https://sixaxispairtool.software.informer.com/download/) and run it with the PS4 controller plugged in via USB.

Enter the Esp32's MAC address to the controller using SixAxisPairer

Finally, in main.ino, enter the Esp32's MAC address as the ESP32_MAC constant

## Controls
|Button|Action|
|------|------|
|Left stick Y|Head up/down|
|Left stick X|Head swivel|
|Cross|Happy eyes|
|Circle|Close eyes|
|Square|Neutral eyes|
|L1+R1|Set all to neutral|
|Right Stick|Drive|