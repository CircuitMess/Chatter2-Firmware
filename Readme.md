## Chatter 2 port to Chatter 1

This is a fork of Chatter2-Firmware to run on Chatter 1 devices that adds games and connectivity with Chatter 2 devices.

Installation:
- Download Arduino 1.x https://www.arduino.cc/en/software (Arduino 2.x does not work with the plugin below)
- Install https://github.com/me-no-dev/arduino-esp32fs-plugin to upload images from ./data folder to the device
- Add "https://raw.githubusercontent.com/CircuitMess/Arduino-Packages/master/package_circuitmess.com_esp32_index.json" to Additional board Manager URLs and pick "Chatter" as the board
- Run Tools -> ESP32 Sketch Data Upload
- Upload sketch

Troubleshooting:
- If compilation fails with `"python": executable file not found in $PATH` because you only have "python3" binary then this fixes it (adjust path accordingly):
`sed -i -e 's/=python /=python3 /g' ~/Library/Arduino15/packages/cm/hardware/esp32/*/platform.txt`

