/*
   ============================================================
   SOS Static IP Helper – Ubuntu 24.04 LTS Edition
   ============================================================
   Features:
     • SOS heartbeat on BLUE_LED (toggleable via serial or BOOT button)
     • BOOT button:
         - Short press: toggle SOS
         - Long press: print netplan YAML
     • Serial control panel
     • Stored config fields (iface, IP, CIDR, gateway, DNS)
     • Netplan YAML generator for Ubuntu 24.04 LTS
     • Linux command reminders
     • No WiFi/BLE (avoids brownouts)
*/

#define BLUE_LED 2
#define BOOT_BUTTON 0

// -----------------------------
// Stored configuration
// -----------------------------
String iface   = "eth0";
String ipAddr  = "192.168.1.42";
String cidr    = "24";
String gateway = "192.168.1.1";
String dns1    = "8.8.8.8";
String dns2    = "1.1.1.1";

// -----------------------------
// SOS blink state
// -----------------------------
unsigned long lastBlink = 0;
int blinkStep = 0;
bool sosEnabled = true;

// -----------------------------
// Button state
// -----------------------------
bool buttonWasDown = false;
unsigned long buttonDownAt = 0;

const int SHORT_ON  = 250;
const int SHORT_OFF = 250;
const int LONG_ON   = 750;
const int LONG_OFF  = 750;

// -----------------------------
// Menu + helpers
// -----------------------------
void printMenu() {
  Serial.println();
  Serial.println("================================================");
  Serial.println("   SOS Static IP Helper – Ubuntu 24.04 LTS");
  Serial.println("================================================");
  Serial.println("Commands:");
  Serial.println("  show                - show current config");
  Serial.println("  set iface <name>    - set interface name");
  Serial.println("  set ip <addr>       - set IP address");
  Serial.println("  set cidr <n>        - set CIDR (e.g. 24)");
  Serial.println("  set gw <addr>       - set gateway");
  Serial.println("  set dns1 <addr>     - set primary DNS");
  Serial.println("  set dns2 <addr>     - set secondary DNS");
  Serial.println("  netplan             - print netplan YAML");
  Serial.println("  sos on/off          - enable or disable SOS heartbeat");
  Serial.println("  help                - show this menu");
  Serial.println();
  Serial.println("BOOT button actions:");
  Serial.println("  Short press: toggle SOS");
  Serial.println("  Long press : print netplan YAML");
  Serial.println();
  Serial.println("Ubuntu 24.04 LTS tip:");
  Serial.println("  To find your interface name, run:");
  Serial.println("    ip link show");
  Serial.println("  or:");
  Serial.println("    ip a");
  Serial.println();
}

void showConfig() {
  Serial.println();
  Serial.println("Current configuration:");
  Serial.print("  iface : "); Serial.println(iface);
  Serial.print("  ip    : "); Serial.println(ipAddr);
  Serial.print("  cidr  : "); Serial.println(cidr);
  Serial.print("  gw    : "); Serial.println(gateway);
  Serial.print("  dns1  : "); Serial.println(dns1);
  Serial.print("  dns2  : "); Serial.println(dns2);
  Serial.println();
}

void printNetplan() {
  Serial.println();
  Serial.println("# Ubuntu 24.04 LTS netplan template");
  Serial.println("# Find your interface name with: ip link show");
  Serial.println();
  Serial.println("network:");
  Serial.println("  version: 2");
  Serial.println("  renderer: networkd");
  Serial.println("  ethernets:");
  Serial.print("    "); Serial.print(iface); Serial.println(":");
  Serial.println("      dhcp4: no");
  Serial.println("      addresses:");
  Serial.print("        - "); Serial.print(ipAddr); Serial.print("/"); Serial.println(cidr);
  Serial.print("      gateway4: "); Serial.println(gateway);
  Serial.println("      nameservers:");
  Serial.print("        addresses: ["); Serial.print(dns1); Serial.print(", "); Serial.print(dns2); Serial.println("]");
  Serial.println();
}

// -----------------------------
// Tokenizer
// -----------------------------
String nextToken(String &s) {
  s.trim();
  int idx = s.indexOf(' ');
  if (idx == -1) {
    String t = s;
    s = "";
    return t;
  }
  String t = s.substring(0, idx);
  s = s.substring(idx + 1);
  s.trim();
  return t;
}

// -----------------------------
// Serial command handler
// -----------------------------
void handleSerial() {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0) return;

  String original = line;
  String cmd = nextToken(line);

  if (cmd.equalsIgnoreCase("help") || cmd.equalsIgnoreCase("menu")) {
    printMenu();
  }
  else if (cmd.equalsIgnoreCase("show")) {
    showConfig();
  }
  else if (cmd.equalsIgnoreCase("netplan")) {
    printNetplan();
  }
  else if (cmd.equalsIgnoreCase("sos")) {
    String state = nextToken(line);
    if (state.equalsIgnoreCase("on")) {
      sosEnabled = true;
      Serial.println("SOS heartbeat enabled.");
    }
    else if (state.equalsIgnoreCase("off")) {
      sosEnabled = false;
      digitalWrite(BLUE_LED, LOW);
      Serial.println("SOS heartbeat disabled for this session.");
    }
    else {
      Serial.println("Usage: sos <on|off>");
    }
  }
  else if (cmd.equalsIgnoreCase("set")) {
    String field = nextToken(line);
    String value = line;

    if (field.equalsIgnoreCase("iface")) {
      if (value.length() == 0) Serial.println("Usage: set iface <name>");
      else { iface = value; Serial.print("iface set to: "); Serial.println(iface); }
    }
    else if (field.equalsIgnoreCase("ip")) {
      if (value.length() == 0) Serial.println("Usage: set ip <addr>");
      else { ipAddr = value; Serial.print("ip set to: "); Serial.println(ipAddr); }
    }
    else if (field.equalsIgnoreCase("cidr")) {
      if (value.length() == 0) Serial.println("Usage: set cidr <n>");
      else { cidr = value; Serial.print("cidr set to: "); Serial.println(cidr); }
    }
    else if (field.equalsIgnoreCase("gw")) {
      if (value.length() == 0) Serial.println("Usage: set gw <addr>");
      else { gateway = value; Serial.print("gateway set to: "); Serial.println(gateway); }
    }
    else if (field.equalsIgnoreCase("dns1")) {
      if (value.length() == 0) Serial.println("Usage: set dns1 <addr>");
      else { dns1 = value; Serial.print("dns1 set to: "); Serial.println(dns1); }
    }
    else if (field.equalsIgnoreCase("dns2")) {
      if (value.length() == 0) Serial.println("Usage: set dns2 <addr>");
      else { dns2 = value; Serial.print("dns2 set to: "); Serial.println(dns2); }
    }
    else {
      Serial.println("Unknown field. Use: iface, ip, cidr, gw, dns1, dns2");
    }
  }
  else {
    Serial.print("Unknown command: ");
    Serial.println(original);
    Serial.println("Type 'help' for menu.");
  }
}

// -----------------------------
// BOOT button handler
// -----------------------------
void handleButton() {
  bool pressed = (digitalRead(BOOT_BUTTON) == LOW);

  if (pressed && !buttonWasDown) {
    buttonDownAt = millis();
    buttonWasDown = true;
  }

  if (!pressed && buttonWasDown) {
    unsigned long held = millis() - buttonDownAt;
    buttonWasDown = false;

    if (held < 600) {
      sosEnabled = !sosEnabled;
      Serial.println(sosEnabled ? "SOS enabled (button)" : "SOS disabled (button)");
    } else {
      Serial.println("Printing netplan (button long press)...");
      printNetplan();
    }
  }
}

// -----------------------------
// SOS blink engine
// -----------------------------
void blinkSOS() {
  unsigned long now = millis();

  switch (blinkStep) {
    case 0: case 2: case 4:
      if (now - lastBlink >= SHORT_ON) {
        digitalWrite(BLUE_LED, LOW);
        lastBlink = now;
        blinkStep++;
      } else digitalWrite(BLUE_LED, HIGH);
      break;

    case 1: case 3: case 5:
      if (now - lastBlink >= SHORT_OFF) {
        lastBlink = now;
        blinkStep++;
      }
      break;

    case 6: case 8: case 10:
      if (now - lastBlink >= LONG_ON) {
        digitalWrite(BLUE_LED, LOW);
        lastBlink = now;
        blinkStep++;
      } else digitalWrite(BLUE_LED, HIGH);
      break;

    case 7: case 9: case 11:
      if (now - lastBlink >= LONG_OFF) {
        lastBlink = now;
        blinkStep++;
      }
      break;

    case 12:
      if (now - lastBlink >= 1500) {
        blinkStep = 0;
        lastBlink = now;
      }
      break;
  }
}

// -----------------------------
// Setup + Loop
// -----------------------------
void setup() {
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
  delay(10);

  Serial.println("SOS Static IP Helper – Ubuntu 24.04 LTS");
  printMenu();
}

void loop() {
  handleSerial();
  handleButton();

  if (sosEnabled) blinkSOS();
  else digitalWrite(BLUE_LED, LOW);
}
