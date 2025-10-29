# I2PChat

A peer-to-peer instant messaging application that operates over the I2P (Invisible Internet Project) network. I2PChat uses the SAM (Simple Anonymous Messaging) bridge to provide anonymous, encrypted chat with file transfer capabilities.

## About This Fork

This is a personal fork of the original [I2PChat project](https://github.com/I2PPlus/i2pchat) (last updated 5 years ago). This fork has been modernized and enhanced with the following improvements:

### Key Enhancements

- **macOS ARM64 Support**: Native Apple Silicon (M1/M2/M3/M4) compatibility
- **C++20 Migration**: Updated from C++11 to C++20 for modern language features
- **Qt 6 Support**: Migrated from Qt 5 to Qt 6.4+ for better performance and compatibility
- **SOCKS5 Proxy Support**: Added SOCKS5 proxy functionality for flexible network routing
- **UI Improvements**: Various user interface tweaks and enhancements
- **Universal Binary**: Support for both ARM64 and x86_64 architectures on macOS

For detailed information about SOCKS5 support, see:
- [SOCKS5_PROXY_SUPPORT.md](SOCKS5_PROXY_SUPPORT.md) - Configuration and usage
- [SOCKS5_EXPLAINED.md](SOCKS5_EXPLAINED.md) - Technical details
- [SOCKS5_TESTING_GUIDE.md](SOCKS5_TESTING_GUIDE.md) - Testing procedures

### Original Project

- **Original Repository**: [https://github.com/I2PPlus/i2pchat](https://github.com/I2PPlus/i2pchat)
- **Original Last Update**: June 2021 (5 years ago)
- **License**: See [LICENSE](LICENSE) file

## Features

- Anonymous peer-to-peer messaging over I2P network
- End-to-end encrypted communications
- File transfer support
- Contact management
- User profiles with avatars and status
- Audio notifications
- Multi-platform support (Linux, macOS, Windows)

## Requirements

### System Requirements

- **Qt 6.4 or later** (C++20 support required)
- **C++20 compatible compiler**:
  - Clang 11+
  - GCC 10+
  - MSVC 2019 16.10+
- **I2P Router** with SAM bridge enabled:
  - Java I2P (recommended)
  - i2pd (C++ implementation)

### Platform-Specific Requirements

#### macOS
- **macOS 13.0 or later** (required for Qt 6 on Apple Silicon)
- **Xcode 15+** with Command Line Tools
- **Homebrew** package manager

#### Linux
- Qt 6 development packages
- Standard build tools (make, gcc/clang)

#### Windows
- Visual Studio 2019 or later
- Qt 6 for Windows

## Installation

### macOS (Apple Silicon / Intel)

#### Quick Setup (Automated)

Run the automated setup script:

```bash
./setup_macos.sh
```

This script will:
1. Check for Homebrew installation
2. Install Qt 6 (or Qt 5 if you prefer)
3. Optionally install i2pd with SAM bridge enabled
4. Create environment configuration script

#### Manual Setup

1. **Install Homebrew** (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Install Qt 6**:
   ```bash
   brew install qt@6
   ```

3. **Install I2P Router** (choose one):

   **Option A: i2pd (C++ implementation)**
   ```bash
   brew install i2pd
   ```

   Configure SAM bridge:
   - On Apple Silicon: `/opt/homebrew/etc/i2pd/i2pd.conf`
   - On Intel: `/usr/local/etc/i2pd/i2pd.conf`

   Add to configuration file:
   ```ini
   [sam]
   enabled = true
   address = 127.0.0.1
   port = 7656
   ```

   Start i2pd:
   ```bash
   brew services start i2pd
   ```

   **Option B: Java I2P**
   - Download from [https://geti2p.net/en/download](https://geti2p.net/en/download)
   - Install and enable SAM bridge at: [http://127.0.0.1:7657/configclients](http://127.0.0.1:7657/configclients)

4. **Set up Qt environment**:

   For Apple Silicon:
   ```bash
   export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
   export LDFLAGS="-L/opt/homebrew/opt/qt@6/lib"
   export CPPFLAGS="-I/opt/homebrew/opt/qt@6/include"
   export PKG_CONFIG_PATH="/opt/homebrew/opt/qt@6/lib/pkgconfig"
   ```

   For Intel:
   ```bash
   export PATH="/usr/local/opt/qt@6/bin:$PATH"
   export LDFLAGS="-L/usr/local/opt/qt@6/lib"
   export CPPFLAGS="-I/usr/local/opt/qt@6/include"
   export PKG_CONFIG_PATH="/usr/local/opt/qt@6/lib/pkgconfig"
   ```

5. **Build I2PChat**:

   **Important**: If you need to rebuild from scratch, clean the previous build first:
   ```bash
   make clean
   rm -rf I2PChat.app
   rm -rf temp/
   rm -f Makefile .qmake.stash
   ```

   Then build:
   ```bash
   qmake I2PChat.pro "CONFIG += release"
   make -j$(sysctl -n hw.ncpu)
   ```

6. **Run the application**:
   ```bash
   ./I2PChat.app/Contents/MacOS/I2PChat
   # or
   open I2PChat.app
   ```

### Linux

#### Ubuntu/Debian

1. **Install dependencies**:
   ```bash
   sudo apt-get update
   sudo apt-get install qt6-base-dev build-essential
   ```

2. **Build**:

   To clean previous build (if needed):
   ```bash
   make clean
   rm -rf temp/
   rm -f Makefile .qmake.stash I2PChat
   ```

   Then build:
   ```bash
   qmake I2PChat.pro "CONFIG += release"
   make -j$(nproc)
   ```

3. **Run**:
   ```bash
   ./I2PChat
   ```

#### Fedora

1. **Install dependencies**:
   ```bash
   sudo dnf install qt6-qtbase-devel make gcc-c++
   ```

2. **Build**:

   To clean previous build (if needed):
   ```bash
   make clean
   rm -rf temp/
   rm -f Makefile .qmake.stash I2PChat
   ```

   Then build:
   ```bash
   qmake-qt6 I2PChat.pro "CONFIG += release"
   make -j$(nproc)
   ```

3. **Run**:
   ```bash
   ./I2PChat
   ```

### Windows

1. **Install Qt 6** from [https://www.qt.io/download](https://www.qt.io/download)
2. **Install Visual Studio 2019 or later**
3. **Open Qt Creator** and load `I2PChat.pro`
4. **Build** the project in Release mode
5. Alternatively, use the provided batch file:
   ```cmd
   Windows_build.bat
   ```

## Build Configuration

### Cleaning Previous Build

**Important**: When switching between build configurations or encountering build issues, clean the previous build first:

**macOS**:
```bash
make clean
rm -rf I2PChat.app
rm -rf temp/
rm -f Makefile .qmake.stash
```

**Linux**:
```bash
make clean
rm -rf temp/
rm -f Makefile .qmake.stash I2PChat
```

**Windows**:
```cmd
nmake clean
del /Q Makefile .qmake.stash
rmdir /S /Q temp
del /Q I2PChat.exe
```

### Build Options

- **Release build** (optimized):
  ```bash
  qmake I2PChat.pro "CONFIG += release"
  make
  ```

- **Debug build** (with debug symbols):
  ```bash
  qmake I2PChat.pro "CONFIG += debug"
  make
  ```

### Build Outputs

- **Executable**:
  - Linux: `I2PChat`
  - macOS: `I2PChat.app`
  - Windows: `I2PChat.exe`
- **Temporary build files**: `temp/obj/`, `temp/qrc/`, `temp/moc/`
- **UI compiled files**: `src/gui/`

### Multi-threaded Compilation

For faster compilation, use multiple CPU cores:

- **macOS**: `make -j$(sysctl -n hw.ncpu)`
- **Linux**: `make -j$(nproc)`
- **Manual**: `make -j8` (adjust number based on your CPU)

## Configuration

On first run, I2PChat will create configuration directories:

- **Linux**: `~/.i2pchat/`
- **macOS**: `~/Library/Application Support/I2PChat/`
- **Windows**: `%APPDATA%\Roaming\I2PChat\`

### SAM Bridge Settings

Default SAM bridge connection:
- **Host**: 127.0.0.1
- **Port**: 7656

You can configure these in the application settings.

## Troubleshooting

### macOS Issues

**Problem**: "qmake: command not found"
- **Solution**: Make sure Qt is in your PATH. Run the environment setup:
  ```bash
  source env_setup.sh
  ```

**Problem**: Build fails with architecture errors
- **Solution**: The project automatically detects your architecture. Ensure you have Xcode 15+ installed for Apple Silicon support.

**Problem**: Cannot connect to I2P
- **Solution**:
  1. Verify I2P router is running
  2. Check SAM bridge is enabled (port 7656)
  3. For i2pd: `brew services list` to check status

### Linux Issues

**Problem**: Qt 6 not found
- **Solution**: Install Qt 6 development packages for your distribution

**Problem**: On Fedora, qmake not working
- **Solution**: Use `qmake-qt6` instead of `qmake`

## Version Information

### This Fork
- **Fork Version**: 0.30.0+ (based on v0.2.37)
- **Protocol Version**: 0.6 (compatible with original)
- **Required C++ Standard**: C++20
- **Required Qt Version**: 6.4+
- **Git Commit**: 7f1bde3 - "feat: bump to cpp 20 and socks5 function and UI tweaks"

### Original Project
- **Original Version**: v0.2.37 and earlier
- **C++ Standard**: C++11
- **Qt Version**: 5.15

**Note**: This fork is NOT backward compatible with older systems due to C++20 and Qt 6 requirements. For older systems, use the original project v0.2.37 or earlier.

## Architecture Support

- **macOS**: Universal binary (x86_64 + arm64)
  - Native Apple Silicon (M1/M2/M3/M4) support
  - Intel Mac support
- **Linux**: x86_64, ARM64, i386
- **Windows**: x86_64, i386

## Security Features

- **Anonymous messaging** via I2P network
- **End-to-end encryption** using I2P's encryption layer
- **Signature types** supported:
  - EdDSA_SHA512_Ed25519 (recommended)
  - ECDSA_SHA256_P256
  - ECDSA_SHA384_P384
  - ECDSA_SHA512_P521
- **Encryption types**:
  - ECIES (recommended)
  - ElGamal

**Note**: DSA_SHA1 has been removed for security reasons as of v0.2.31.

## Documentation

- **CLAUDE.md**: Development guide and architecture overview
- **HOW_TO_RUN.md**: Runtime instructions and configuration
- **SOCKS5_PROXY_SUPPORT.md**: SOCKS5 proxy configuration
- **SOCKS5_TESTING_GUIDE.md**: Testing SOCKS5 functionality
- **COMPATIBILITY_2025.md**: Platform compatibility information
- **SPEC.md**: Technical specifications

## Contributing

Contributions are welcome! Please ensure:
- Code compiles with C++20 standard
- Qt 6.4+ compatibility
- Follows existing code style
- Tests pass before submitting PR

## License

See [LICENSE](LICENSE) file for details.

## Links

### This Fork
- **Original I2PChat Repository**: [https://github.com/I2PPlus/i2pchat](https://github.com/I2PPlus/i2pchat)

### Related Projects
- **I2P Project**: [https://geti2p.net/](https://geti2p.net/)
- **i2pd**: [https://i2pd.website/](https://i2pd.website/)
- **Qt Framework**: [https://www.qt.io/](https://www.qt.io/)

## Acknowledgments

This project is a personal fork based on the excellent work of the original I2PChat developers. Special thanks to:
- The original I2PChat development team
- The I2P Project community
- Qt framework contributors

All modifications in this fork (macOS ARM64 support, SOCKS5 proxy, C++20 migration, UI improvements) are provided as-is for personal use and educational purposes.

## Support

This is a personal fork maintained for individual use. For the original project, please refer to the [upstream repository](https://github.com/I2PPlus/i2pchat).
