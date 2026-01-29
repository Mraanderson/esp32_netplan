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

---

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

# ESP32 + LAN8720 + OLED + PoE  
## Parts List, Wiring Diagram, and Build Notes

This describes the hardware needed to build a PoE‑powered, wired‑Ethernet ESP32 tool capable of LLDP sniffing, OLED display output, and everything else your imagination wants to bolt on.

---

## 🧩 Parts List

### Core Components
- **ESP32‑WROOM Dev Board**  
  Standard USB‑serial board.

- **LAN8720 RMII Ethernet PHY Module**  
  Common breakout board with 50MHz crystal or clock‑in pin.

- **RJ45 MagJack**  
  RJ45 connector with integrated magnetics (required for Ethernet).

### PoE Power Options (choose ONE)

#### Option A — Easiest & Safest
- **802.3af/at PoE Splitter (5V output)**  
  Provides isolated 5V from PoE.  
  Example: “Active PoE Splitter 5V 2A”.

#### Option B — Compact & Professional
- **Silvertel AG9705 / AG9712 PoE Module**  
  Fully isolated, 802.3af compliant, outputs 5V or 12V.

#### Option C — DIY‑ish
- **HLK‑PM48 PoE DC‑DC Module**  
  48V → 5V isolated converter.

> ⚠️ PoE is *never* provided by the LAN8720.  
> You must use a PoE module or splitter to safely extract power.

### Display
- **0.96" or 1.3" I²C OLED (SSD1306 or SH1106)**  
  4‑pin: VCC, GND, SDA, SCL  
  Fully 3.3V compatible.

---

## 🧩 Wiring Diagram (ESP32 ↔ LAN8720)

### RMII Ethernet Wiring

```
ESP32 Pin        →   LAN8720 Pin
-----------------------------------------
3V3              →   VCC
GND              →   GND

GPIO0 or GPIO17  →   CLKIN (depends on module)
GPIO23           →   TX_EN
GPIO19           →   TX0
GPIO22           →   TX1
GPIO25           →   RX0
GPIO26           →   RX1
GPIO27           →   CRS_DV

EN (via 10k)     →   nRST (LAN8720 reset)
```

### Notes
- Some LAN8720 boards require a **50MHz clock** on `CLKIN`.  
  ESP32 can output this on GPIO0 or GPIO17 depending on configuration.
- If your LAN8720 has its own crystal, you do **not** need the ESP32 clock output.

---

## 🧩 LAN8720 ↔ RJ45 MagJack

If your LAN8720 module does **not** include magnetics:

```
LAN8720 TX+  → RJ45 TX+
LAN8720 TX-  → RJ45 TX-
LAN8720 RX+  → RJ45 RX+
LAN8720 RX-  → RJ45 RX-
```

If using a MagJack breakout, these are labelled.

---

## 🧩 PoE Splitter → ESP32

If using a PoE splitter (recommended):

```
PoE Splitter 5V Out → ESP32 5V pin
PoE Splitter GND    → ESP32 GND
```

The splitter handles:
- 48V negotiation  
- isolation  
- DC‑DC conversion  

No high‑voltage wiring required.

---

## 🧩 OLED Display (I²C)

```
OLED VCC → 3.3V
OLED GND → GND
OLED SDA → GPIO21
OLED SCL → GPIO22 (or GPIO32 if TX1 is used)
```

If GPIO22 is used for RMII TX1, move SCL to GPIO32 or GPIO33.

---

## 🧩 Recommended GPIO Map (Final)

```
GPIO0 / GPIO17 → LAN8720 CLKIN
GPIO19        → TX0
GPIO22        → TX1  (move OLED SCL if needed)
GPIO23        → TX_EN
GPIO25        → RX0
GPIO26        → RX1
GPIO27        → CRS_DV

GPIO21        → OLED SDA
GPIO32        → OLED SCL
```

---

## 🧩 What This Build Enables

With these components wired together, you get:

- **PoE‑powered ESP32**  
- **Real wired Ethernet** via LAN8720  
- **LLDP frame capture** (switch name, port ID, VLAN, mgmt IP, etc.)  
- **OLED display output** for field‑tech mode  
- **Serial interface** for netplan generation  
- **Button‑controlled SOS + netplan print**  
- A **pocket‑sized sysadmin tool** that can identify the exact switch port it’s plugged into.

This is the kind of device network engineers would absolutely lose their minds over.

