# MegaByte Coin for Windows

<p align="center">
  <img src=".github/assets/walletFrame_bg.png" alt="MegaByte Coin Windows Build" width="100%">
</p>

<p align="center">
  <a href="https://github.com/MegaByteCoin/mbc-win/releases/tag/v1.0.0"><img src="https://img.shields.io/badge/platform-Windows%20Wallet%20%2F%20Win64-0f172a?style=for-the-badge&logo=windows" alt="Windows"></a>
  <img src="https://img.shields.io/badge/algorithm-Quark-2563eb?style=for-the-badge" alt="Quark">
  <img src="https://img.shields.io/badge/ticker-MBC-0ea5e9?style=for-the-badge" alt="MBC">
  <img src="https://img.shields.io/badge/staking-Yes-16a34a?style=for-the-badge" alt="Staking">
  <img src="https://img.shields.io/badge/masternodes-Yes-7c3aed?style=for-the-badge" alt="Masternodes">
  <img src="https://img.shields.io/badge/max_supply-1%2C000%2C000%20MBC-f59e0b?style=for-the-badge" alt="Supply">
</p>

<p align="center">
  <a href="https://megabytecoin.pp.ua"><img src="https://img.shields.io/badge/Website-megabytecoin.pp.ua-111827?style=for-the-badge"></a>
  <a href="https://explorer.megabytecoin.pp.ua"><img src="https://img.shields.io/badge/Explorer-Online-111827?style=for-the-badge"></a>
  <a href="https://github.com/MegaByteCoin/mbc-win/releases/tag/v1.0.0"><img src="https://img.shields.io/badge/Windows%20Wallet-v1.0.0-111827?style=for-the-badge"></a>
  <a href="https://ex.mbc.pp.ua"><img src="https://img.shields.io/badge/Exchange-Test%20Mode-111827?style=for-the-badge"></a>
</p>

---

## Overview

**MegaByte Coin (MBC)** is a Quark-based cryptocurrency with support for **staking** and **masternodes**.

This repository contains the **Windows wallet source tree and cross-build environment** for compiling the MegaByte Coin Windows daemon, CLI tools, TX tool, and Qt wallet on **Ubuntu 20.04** through the `depends/` system.

This README is focused on a **Windows / Win64 build workflow** that produces the standard wallet binaries:

```bash
mbcd.exe
mbc-cli.exe
mbc-tx.exe
mbc-qt.exe
```

---

## Network Parameters

| Parameter | Value |
|---|---|
| Coin | MegaByte Coin |
| Ticker | MBC |
| Algorithm | Quark |
| Staking | Yes |
| Masternodes | Yes |
| Masternode Collateral | 1000 MBC |
| Max Supply | 1,000,000 MBC |
| Mainnet Port | 18777 |
| Mainnet RPC Port | 18778 |
| Block Time | 1 minute |
| Last POW Block | 1000 |
| Genesis Launch | 28.02.2026 01:00 UTC |

---

## Current Mainnet Nodes

The current mainnet layout used in this guide is:

- `31.131.21.71:18777`
- `31.131.21.71:18779`
- `31.131.21.71:18781`
- `41.138.197.2:18777`
- `41.138.197.3:18777`
- `41.138.197.4:18777`
- `41.138.197.5:18777`
- `41.138.197.6:18777`
- `41.138.197.7:18777`
- `41.138.197.8:18777`

Explorer:

- `https://explorer.megabytecoin.pp.ua`
- reserve direct URL: `http://31.131.21.71:3001/`

---

## Official Links

- **Website:** https://megabytecoin.pp.ua
- **Exchange:** https://ex.mbc.pp.ua
- **Explorer:** https://explorer.megabytecoin.pp.ua
- **Bitcointalk:** https://megabytecoin.pp.ua/bitcointalk.php
- **GitHub:** https://github.com/MegaByteCoin
- **Discord:** https://megabytecoin.pp.ua/discord.php
- **YouTube:** https://megabytecoin.pp.ua/youtube.php
- **News Feed:** https://megabytecoin.pp.ua/feed
- **Ubuntu/Linux release:** https://github.com/MegaByteCoin/mbc-ubuntu/releases/tag/v1.0.0
- **Windows release:** https://github.com/MegaByteCoin/mbc-win/releases/tag/v1.0.0

---

## Repository Layout

- `depends/` — dependency build system for Win64 cross-compilation
- `src/` — daemon, CLI, transaction tool, and core sources
- `src/qt/` — Qt wallet sources
- `contrib/` — helper scripts and extra tools
- `.github/assets/` — README assets such as banner images

---

## Build Windows Wallet on Ubuntu 20.04 through `depends`

### 1. Install build dependencies

```bash
sudo apt-get update
sudo apt-get install -y software-properties-common
sudo add-apt-repository -y universe
sudo apt-get update
sudo apt-get install -y build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 curl git zip unzip nsis
sudo apt-get install -y g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64 mingw-w64-x86-64-dev
```

Optional check:

```bash
makensis -VERSION
```

### 2. Build `depends` for Win64

```bash
cd /root/mbc-win/depends
make HOST=x86_64-w64-mingw32 -j2
```

### 3. Generate `configure`

```bash
cd /root/mbc-win
./autogen.sh
```

### 4. Configure the project for Windows

```bash
cd /root/mbc-win
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --host=x86_64-w64-mingw32 --prefix=$PWD/depends/x86_64-w64-mingw32
```

### 5. Build

```bash
cd /root/mbc-win
make -j2
```

---

## Expected Build Output

After a successful build, the Windows binaries should be available here:

```bash
/root/mbc-win/src/mbcd.exe
/root/mbc-win/src/mbc-cli.exe
/root/mbc-win/src/mbc-tx.exe
/root/mbc-win/src/qt/mbc-qt.exe
```

Check them:

```bash
ls -lh /root/mbc-win/src/mbcd.exe /root/mbc-win/src/mbc-cli.exe /root/mbc-win/src/mbc-tx.exe /root/mbc-win/src/qt/mbc-qt.exe
```

---

## Rebuild after Editing `chainparams.cpp`

When you change only `src/chainparams.cpp`, you do **not** need to rebuild `depends`.
You only need to rebuild the Windows project itself:

```bash
cd /root/mbc-win
make clean
./autogen.sh
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --host=x86_64-w64-mingw32 --prefix=$PWD/depends/x86_64-w64-mingw32
make -j2
```

---

## Example `mbc.conf` for a Regular Mainnet Wallet

This example is for a **normal mainnet Windows wallet / daemon**.

File name on Windows:

```bash
mbc.conf
```

Typical data directory on Windows:

```bash
%APPDATA%\MegaByteCoin\mbc.conf
```

Example config:

```ini
rpcuser=mbcuser
rpcpassword=CHANGE_THIS_TO_A_LONG_RANDOM_PASSWORD

listen=1
daemon=1
server=1
txindex=1

port=18777
rpcport=18778

maxconnections=128
upnp=0

addnode=31.131.21.71:18777
addnode=31.131.21.71:18779
addnode=31.131.21.71:18781
addnode=41.138.197.2:18777
addnode=41.138.197.3:18777
addnode=41.138.197.4:18777
addnode=41.138.197.5:18777
addnode=41.138.197.6:18777
addnode=41.138.197.7:18777
addnode=41.138.197.8:18777
```

Notes:

- `rpcuser` and `rpcpassword` should be changed before first launch.
- Do not keep outdated nodes from `31.131.27.102`.
- This config is intended for a regular mainnet wallet user.
- The same `mbc-qt.exe` can be used for both mainnet and testnet through different configs.

---

## Example Testnet `mbc.conf`

A separate Windows test wallet executable is **not** required.
The same wallet binary works on testnet with a separate config containing `testnet=1`.

Example testnet config:

```ini
testnet=1
rpcuser=mbctest
rpcpassword=CHANGE_THIS_TO_A_LONG_RANDOM_PASSWORD
listen=1
daemon=1
server=1
txindex=1
port=28777
rpcport=28778
addnode=31.131.21.71:28777
```

---

## Optional NSIS Installer

If the repository contains an `.nsi` installer file, build it like this:

```bash
find contrib -maxdepth 5 -type f -iname "*.nsi" -print
```

```bash
makensis /path/to/installer.nsi
```

---

## Notes

- Keep Linux and Windows builds in separate folders: `mbc-ubuntu` and `mbc-win`
- Do not mix build outputs between the two trees
- Do not commit build results like `*.exe`, `*.o`, `*.a`, `*.dll`
- For Windows rebuilds after source edits, reuse the already built `depends` tree unless dependencies changed

---

## License

See `COPYING` and the licenses of bundled third-party dependencies where applicable.
