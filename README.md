# 🔐 Buffer Overflow Demonstration with Protection

This project demonstrates a **classic buffer overflow vulnerability** and how protection mechanisms can prevent exploitation. It contains:

* `overflow_no_protection`: A vulnerable binary without any stack protections.
* `overflow_with_protection`: A version with mitigations like stack canaries, NX bit, and compile-time hardening.

## 🧪 Purpose

To **visually observe** how unsafe memory usage can crash a program (segfault), and how common protection techniques defend against it — across compilers and platforms using CI.

## 📁 Project Structure

```
.
├── CMakeLists.txt
├── main.cpp
├── .github/
│   └── workflows/
│       └── cmake-multiplatform.yml
└── README.md
```

## 🔧 Building

This project uses **CMake**. You can build it manually like so:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Or use the GitHub Actions workflow, which builds and runs the project on:

* **Ubuntu** (GCC, Clang)
* **Windows** (MSVC)

## 🚀 Running the Programs

```bash
# Without protection (should crash)
./overflow_no_protection

# With protection (should detect and prevent overflow)
./overflow_with_protection
```

You will see output like:

```
Buffer address: 0x7ffcb5e45c80
Buffer content: AAAAAAAAAAAAA...
Returned safely from vulnerable_function
```

Or:

```
Segmentation fault (core dumped)
```

## 🤖 GitHub Actions Workflow

The included CI workflow:

* Builds both binaries on Linux and Windows
* Runs them even if they crash (exit code ≠ 0)
* Captures and prints exit codes to distinguish crashes

### Key Features

* Uses matrix strategy to test on:

  * Ubuntu + GCC
  * Ubuntu + Clang
  * Windows + MSVC
* Uses `|| true` to **continue execution even after segfault**
* Detects and prints crash codes (e.g. `exit 139` = segmentation fault)

## 📜 Example CI Output

```
Buffer address: 000000F3BACFF8E0
Buffer content: AAAAAAAAAAAAAA...
Returned safely from vulnerable_function
💥 Segmentation fault (exit code 139)
```

## 🔒 Protection Techniques Used

* Stack canaries (`-fstack-protector`)
* Non-executable stack (`-z noexecstack`)
* Position-independent executable (`-fPIE -pie`)
* PR\_SET\_DUMPABLE to disable core dumps (Linux only)
