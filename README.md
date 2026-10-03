#  morpheus-kernel

A custom 64-bit hobby kernel built from scratch in **C** and **x86_64 Assembly** to explore systems architecture, hardware management, and kernel design.

*Note: As higher-level kernel concepts are introduced, C++ will gradually be integrated into the codebase.*

---

## Developer Note

Hello! **morpheus-kernel** is an educational kernel project born out of a curiosity about how operating systems work under the hood. I'm developing this as a part of my learning journey, as I belive it's good to write some code to really understand the subject:] Feel free to look around the repository. 

I also keep a short digital log of my development process on Instagram if you are interested in following along! [LINK](https://www.instagram.com/czopson.database)

---

## Technology & Toolchain

* **Language:** C (Freestanding `-ffreestanding`), x86_64 Assembly (`NASM` / GCC Inline Assembly)
* **Boot Protocol:** [Limine Bootloader](https://github.com/limine-bootloader/limine) (64-bit Long Mode initialization, higher-half physical mapping)
* **Emulation & Debugging:** QEMU (`qemu-system-x86_64`)
* **Build System:** GNU Make

---

##  What's Implemented So Far

### Interrupt Descriptor Table (IDT) & Exception Handling
* **CPU Exception Handlers (Vectors 0–31):** Custom assembly stubs that safely preserve execution state (register pushes) and pass CPU stack frames to C handlers (e.g., `#DE` Divide-by-Zero, `#GP` General Protection Fault, `#PF` Page Fault).
* **Software Debugging Subsystem:** with the debug_put() function

###  Hardware Control & Advanced Timekeeping
* **Legacy Hardware Masking:** Explicitly disables/masks the legacy 8259 PIC to eliminate spurious hardware noise.
* **Modern APIC & x2APIC Subsystem:** Implements support for the local Advanced Programmable Interrupt Controller (LAPIC).
* **MMU Bypass via MSRs:** Leverages **x2APIC** using CPU Model-Specific Registers (`wrmsr`/`rdmsr`) to control hardware timing without requiring premature virtual memory allocation/paging setups. (will later be replaces of course)
* **Preemptive LAPIC Timer:** Calibrated local timer running in periodic mode to deliver reliable tick events and power kernel delay functions like `sleep()`.

##  Building & Running

### Prerequisites
Ensure you have the following installed on your host system:
* `gcc` / `clang` (with x86_64 target support)
* `nasm`
* `make`
* `qemu-system-x86_64`

### Quick Start
```bash
# 1. Clone the repository
git clone https://github.com/ECzopor/morpheus-kernel
cd morpheus-kernel

# 2. Build the kernel ISO
make

# 3. Launch in QEMU
make run
```
## Inspirations & Resources

* [Fire has been lit](https://pages.cs.wisc.edu/~remzi/OSTEP/)
* [OSdev wiki](https://wiki.osdev.org/Expanded_Main_Page)
* [r/osdev reddit comunity](https://www.reddit.com/r/osdev/)
