# SOS Static IP Helper – Ubuntu 24.04 LTS

A tiny ESP32‑based serial utility that helps you generate clean, indentation‑safe **netplan** configuration snippets for Ubuntu 24.04 LTS.  
It runs a visible **SOS heartbeat** on the onboard LED so you always know the device is powered, and exposes a simple **serial control panel** for editing network parameters and printing a ready‑to‑paste YAML block.

This project exists because sometimes you’re standing in front of a machine that needs a static IP *right now*, and fighting YAML spacing or remembering interface names is the last thing you want to deal with.

---

## ✨ Features

- SOS LED heartbeat (non‑blocking, toggleable via `sos on/off`)
- **BOOT button actions:**
  - **Short press:** toggle SOS heartbeat  
  - **Long press:** print the full netplan YAML instantly  
- Serial command console
- Editable fields:
  - Interface name
  - IP address
  - CIDR
  - Gateway
  - DNS1 / DNS2
- Generates a valid **Ubuntu 24.04 LTS netplan** YAML snippet
- Built‑in Linux reminders for finding the correct interface
- No Wi‑Fi, no BLE, no brownouts — works on even the weakest ESP32 boards

---

## 🧩 Supported Boards

This project is designed for **classic ESP32 boards based on the ESP32‑WROOM or ESP32‑WROVER modules**, including:

- ESP32 DevKitC (WROOM‑32)  
- ESP32 DOIT / generic ESP32‑WROOM dev boards  
- ESP32‑WROVER dev boards (USB‑serial type)

These boards use a **USB‑to‑serial chip** (CH340, CP2102, FTDI, etc.) and **do not rely on native USB**, which makes them ideal for a stable, serial‑only utility like this.

### Not supported

The following ESP32 variants have **native USB** and behave differently:

- ESP32‑S2  
- ESP32‑S3  
- ESP32‑C3  

They *can* run the code with minor pin changes, but the README’s behaviour assumptions (USB‑serial, no HID, no USB enumeration) do **not** apply to them.

---

## 🖥️ Serial Control Panel

Open a serial monitor at **115200 baud** and you’ll see:

```
================================================
   SOS Static IP Helper – Ubuntu 24.04 LTS
================================================
Commands:
  show                - show current config
  set iface <name>    - set interface name
  set ip <addr>       - set IP address
  set cidr <n>        - set CIDR (e.g. 24)
  set gw <addr>       - set gateway
  set dns1 <addr>     - set primary DNS
  set dns2 <addr>     - set secondary DNS
  netplan             - print netplan YAML
  sos on/off          - enable or disable SOS heartbeat
  help                - show this menu

Ubuntu 24.04 LTS tip:
  To find your interface name, run:
    ip link show
  or:
    ip a
```

---

## 🔘 BOOT Button Controls

The ESP32’s **BOOT button (GPIO0)** is fully supported:

- **Short press:**  
  Toggle the SOS heartbeat on/off  
  (useful when the blinking gets annoying)

- **Long press:**  
  Instantly print the full netplan YAML  
  (perfect when standing at a server with a terminal open)

These actions do **not** interfere with flashing or normal operation.

---

## 🧩 Example Usage

### Set your values:

```
set iface enp3s0
set ip 192.168.10.42
set cidr 24
set gw 192.168.10.1
set dns1 9.9.9.9
set dns2 1.1.1.1
```

### Print the netplan:

```
netplan
```

### Output:

```yaml
# Ubuntu 24.04 LTS netplan template
# Find your interface name with: ip link show

network:
  version: 2
  renderer: networkd
  ethernets:
    enp3s0:
      dhcp4: no
      addresses:
        - 192.168.10.42/24
      gateway4: 192.168.10.1
      nameservers:
        addresses: [9.9.9.9, 1.1.1.1]
```

Copy/paste into:

```
/etc/netplan/00-installer-config.yaml
```

Apply with:

```
sudo netplan apply
```

---

## 🔧 Hardware Requirements

- ESP32‑WROOM or ESP32‑WROVER dev board  
- One LED on GPIO 2 (built‑in on most boards)  
- BOOT button (GPIO0) for quick actions  
- USB cable for serial communication  

No Wi‑Fi or BLE is used — this avoids brownouts on low‑quality boards (of which I have 2).

---

## 🚀 Why This Exists

Ubuntu’s YAML indentation is famously picky, and interface names change between machines.  
This little tool gives you a pocket‑sized helper that:

- blinks to show it’s alive  
- stores your network settings  
- prints a perfect netplan block every time  
- responds to a physical button for quick actions  

It’s ideal for homelabs, field work, or quick server bring‑ups.

---

## 📄 License

MIT License — do whatever you like with it.

---

## Disclaimer

This project is provided as-is, with no guarantees or warranties of any kind.  
It was created as a small personal utility and shared in case others find it useful.

- The tool generates netplan YAML, but you are responsible for verifying and applying it correctly on your own systems.
- Hardware behaviour varies between ESP32 boards, especially low-cost clones. Only classic ESP32‑WROOM/WROVER USB‑serial boards are supported.
- No Wi‑Fi or BLE functionality is used or tested.
- Use this project at your own discretion. The author is not responsible for any configuration issues, network outages, or unexpected behaviour.

If it works for you, great. If not, feel free to fork it, improve it, or adapt it to your needs.
