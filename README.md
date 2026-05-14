# rootkit-detector

A Linux process-hiding rootkit detector written in C. Detects hidden processes by cross-referencing three independent kernel interfaces and flagging any discrepancies.

---

## How It Works

Rootkits commonly hide processes by hooking the Linux VFS layer, causing `/proc` to omit certain entries. This tool defeats that technique by querying the kernel through multiple independent paths and comparing the results. A process that appears in one source but not another is flagged as suspicious.

---

## Requirements

- Linux (x86-64)
- GCC
- Make
- Root privileges (recommended for accurate results)

No external dependencies. Uses only the C standard library and Linux kernel headers.

---

## Build

```bash
git clone https://github.com/yourusername/rootkit-detector.git
cd rootkit-detector
make
```

To clean the build:

```bash
make clean
```

---

## Usage

Run as root for accurate results. Without root, `kill(pid, 0)` will return `EPERM` for many system processes, which may affect the kill scan count.

```bash
sudo ./rootkit-detector
```

---

## Limitations

- Kernel-level rootkits that patch the VFS `getdents` handler directly will fool both `proc_scan` and `dirent_scan`. A Netlink-based scanner (planned) would address this.
- Small timing races between scans (~1-5 PIDs) are normal on busy systems and do not indicate a rootkit.
- PID namespace isolation (containers) may cause minor discrepancies between scanners.

---
