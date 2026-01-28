## What's Next?

This project is “feature complete” for what it was meant to be:  
a tiny, reliable, serial‑driven netplan generator that keeps old ESP32 boards out of landfill.

But if anyone wants to take it further, here are some ideas that could be fun, useful, or completely unnecessary:

### 🧩 Practical Extensions
- **EEPROM/Flash persistence**  
  Save interface/IP/DNS settings so they survive reboots.

- **Multiple profiles**  
  Switch between “home lab”, “office”, “field kit”, etc.

- **IPv6 support**  
  Because someone, somewhere, will eventually ask for it.

- **Custom templates**  
  Let users upload or select different netplan layouts.

### 🔧 Hardware‑Level Ideas
- **OLED or LCD display**  
  Show the current config without needing a serial monitor.

- **Rotary encoder or buttons**  
  Navigate a tiny menu system on the device itself.

- **Battery‑powered field mode**  
  Turn it into a pocket‑sized sysadmin gadget.

### 🌐 Network‑Adjacent Madness
- **DHCP sniffing mode**  
  Listen for DHCP offers and auto‑suggest network ranges.

- **Link‑local detection**  
  Warn you if you’re about to paste a config that won’t route.

- **Serial‑to‑SSH bridge**  
  Use the ESP32 as a dumb terminal for emergency access.

### 🤪 Completely Unnecessary but Entertaining
- **Morse code output of the IP address**  
  Because the LED is already blinking — why not go full ham radio.

- **“YAML linting mode”**  
  The ESP32 judges your indentation and shames you accordingly.

- **Netplan haiku generator**  
  Turn your network config into poetry for no reason at all.

- **Self‑destruct mode**  
  Wipes the config and blinks in dramatic fashion.  
  (No actual destruction. Probably.)

### 🧨 And finally… the ridiculous‑but‑possible dream feature
- **Ethernet PHY + LLDP port discovery**  
  Add a LAN8720 or similar RMII PHY, plug the ESP32 directly into a switch, listen for LLDP frames, and automatically capture:
  - switch name  
  - port ID  
  - VLAN info  
  - management IP  
  - system description  
  Store it in memory and print it on demand.  
  Completely unnecessary. Absolutely brilliant. A pocket‑sized field tool that tells you *exactly* which switch port you’re on.  
  Will it ever be implemented? Probably not.  
  Could it be? Oh yes.
