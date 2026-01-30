Power saving - my 2 units have fried Wifi BT so...

```
#include "WiFi.h"
#include "bt.h"

void setup() {
  // 1. Force the radios off immediately
  WiFi.mode(WIFI_OFF);
  btStop(); 
  
  // 2. (Optional) Put the radio peripherals in power-down mode
  // This is the "Nuclear Option" for brownout prevention
  esp_bt_controller_disable();
  
  // Start your OLED and Serial logic here...
}
```
