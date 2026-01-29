/*
  netplan_sos.ino
  ESP32 static‑IP Netplan helper with SOS heartbeat

  Features:
    • SOS LED heartbeat (Morse code)
    • Serial command interface for editing network fields
    • Stored configuration fields: iface, ip, cidr, gateway, dns1, dns2
    • Autosave: all changes made with `set` are immediately written to NVS (verbose confirmation)
    • Netplan YAML generation for Ubuntu 24.04 LTS
    • BOOT button: short press toggles SOS, long press prints YAML
*/

#include <Preferences.h>
Preferences prefs;

// -----------------------------
// Configuration fields
// -----------------------------
String iface   = "eth0";
String ipAddr  = "192.168.1.100";
String cidr    = "24";
String gateway = "192.168.1.1";
String dns1    = "1.1.1.1";
String dns2    = "8.8.8.8";

// -----------------------------
// SOS LED pattern
// -----------------------------
const int LED_PIN = 2;
bool sosEnabled = true;

unsigned long sosTimer = 0;
int sosStep = 0;

// -----------------------------
// BOOT button
// -----------------------------
const int BOOT_PIN = 0;
unsigned long buttonDownTime = 0;
bool buttonPressed = false;

// -----------------------------
// Load config from NVS
// -----------------------------
void loadConfig() {
  prefs.begin("netplan", true);
  iface   = prefs.getString("iface", iface);
  ipAddr  = prefs.getString("ip", ipAddr);
  cidr    = prefs.getString("cidr", cidr);
  gateway = prefs.getString("gw", gateway);
  dns1    = prefs.getString("dns1", dns1);
  dns2    = prefs.getString("dns2", dns2);
  prefs.end();
}

// -----------------------------
// Save config to NVS
// -----------------------------
void saveConfig() {
  prefs.begin("netplan", false);
  prefs.putString("iface", iface);
  prefs.putString("ip", ipAddr);
  prefs.putString("cidr", cidr);
  prefs.putString("gw", gateway);
  prefs.putString("dns1", dns1);
  prefs.putString("dns2", dns2);
  prefs.end();
  Serial.println("Saved to NVS.");
}

// -----------------------------
// Welcome menu
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

// -----------------------------
// Print current config
// -----------------------------
void showConfig() {
  Serial.println("\nCurrent configuration:");
  Serial.println("  iface:   " + iface);
  Serial.println("  ip:      " + ipAddr);
  Serial.println("  cidr:    " + cidr);
  Serial.println("  gateway: " + gateway);
  Serial.println("  dns1:    " + dns1);
  Serial.println("  dns2:    " + dns2);
  Serial.println();
}

// -----------------------------
// Generate Netplan YAML
// -----------------------------
void printNetplan() {
  Serial.println("\n--- Netplan YAML ---");
  Serial.println("network:");
  Serial.println("  version: 2");
  Serial.println("  ethernets:");
  Serial.println("    " + iface + ":");
  Serial.println("      dhcp4: false");
  Serial.println("      addresses: [" + ipAddr + "/" + cidr + "]");
  Serial.println("      gateway4: " + gateway);
  Serial.println("      nameservers:");
  Serial.println("        addresses: [" + dns1 + ", " + dns2 + "]");
  Serial.println("---------------------\n");
}

// -----------------------------
// SOS blink engine (original switch version, fixed)
// -----------------------------
void runSOS() {
  if (!sosEnabled) return;

  unsigned long now = millis();

  // Correct timing guard
  if (now < sosTimer) return;

  const int shortOn = 200;
  const int shortOff = 200;
  const int longOn = 600;
  const int longOff = 200;
  const int pause = 800;

  switch (sosStep) {
    case 0: case 2: case 4:
      digitalWrite(LED_PIN, HIGH);
      sosTimer = now + shortOn;
      sosStep++;
      break;

    case 1: case 3: case 5:
      digitalWrite(LED_PIN, LOW);
      sosTimer = now + shortOff;
      sosStep++;
      break;

    case 6: case 8: case 10:
      digitalWrite(LED_PIN, HIGH);
      sosTimer = now + longOn;
      sosStep++;
      break;

    case 7: case 9: case 11:
      digitalWrite(LED_PIN, LOW);
      sosTimer = now + longOff;
      sosStep++;
      break;

    case 12: case 14: case 16:
      digitalWrite(LED_PIN, HIGH);
      sosTimer = now + shortOn;
      sosStep++;
      break;

    case 13: case 15: case 17:
      digitalWrite(LED_PIN, LOW);
      sosTimer = now + shortOff;
      sosStep++;
      break;

    case 18:
      digitalWrite(LED_PIN, LOW);
      sosTimer = now + pause;
      sosStep = 0;
      break;
  }
}

// -----------------------------
// Serial command handler
// -----------------------------
void handleCommand(String input) {
  input.trim();
  if (input.length() == 0) return;

  int spaceIndex = input.indexOf(' ');
  String cmd = (spaceIndex == -1) ? input : input.substring(0, spaceIndex);
  String args = (spaceIndex == -1) ? "" : input.substring(spaceIndex + 1);

  if (cmd == "show") {
    showConfig();
  }

  else if (cmd == "set") {
    int space2 = args.indexOf(' ');
    if (space2 == -1) {
      Serial.println("Usage: set <field> <value>");
      return;
    }

    String field = args.substring(0, space2);
    String value = args.substring(space2 + 1);

    if (field == "iface") iface = value;
    else if (field == "ip") ipAddr = value;
    else if (field == "cidr") cidr = value;
    else if (field == "gw") gateway = value;
    else if (field == "dns1") dns1 = value;
    else if (field == "dns2") dns2 = value;
    else {
      Serial.println("Unknown field.");
      return;
    }

    Serial.println("Updated.");
    saveConfig();
  }

  else if (cmd == "netplan") {
    printNetplan();
  }

  else if (cmd == "sos") {
    if (args == "on") {
      sosEnabled = true;
      Serial.println("SOS enabled.");
    } else if (args == "off") {
      sosEnabled = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("SOS disabled.");
    } else {
      Serial.println("Usage: sos on/off");
    }
  }

  else if (cmd == "help") {
    printMenu();
  }

  else {
    Serial.println("Unknown command.");
  }
}

// -----------------------------
// Setup
// -----------------------------
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BOOT_PIN, INPUT_PULLUP);

  loadConfig();

  Serial.println("\nNetplan SOS Helper Ready.");
  printMenu();
}

// -----------------------------
// Loop
// -----------------------------
void loop() {
  // Serial input
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    handleCommand(input);
  }

  // Button handling
  int state = digitalRead(BOOT_PIN);

  if (state == LOW && !buttonPressed) {
    buttonPressed = true;
    buttonDownTime = millis();
  }

  if (state == HIGH && buttonPressed) {
    unsigned long duration = millis() - buttonDownTime;
    buttonPressed = false;

    if (duration > 800) {
      printNetplan();
    } else {
      sosEnabled = !sosEnabled;
      if (!sosEnabled) digitalWrite(LED_PIN, LOW);
    }
  }

  // SOS engine
  runSOS();
}
