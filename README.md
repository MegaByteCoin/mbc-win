# mbc-win

MegaByteCoin (MBC) Windows wallet source for building **Windows binaries** on **Ubuntu 20.04** through the `depends` system with **MinGW-w64 cross-compilation**.

This build produces the standard Windows wallet files:
- `mbc-qt.exe`
- `mbcd.exe`
- `mbc-cli.exe`
- `mbc-tx.exe`

> This repository is for **Windows builds**. For Linux builds, use a separate working copy such as `mbc-ubuntu`.

---

## Project layout

- `depends/` — dependency build system for cross-compilation
- `src/` — daemon, CLI and TX sources
- `src/qt/` — Qt GUI wallet source
- `contrib/` — helper scripts and additional build tools

---

## Ubuntu 20.04 packages

Install the required packages first:

```bash
apt-get update
apt-get install -y software-properties-common
add-apt-repository -y universe
apt-get update
apt-get install -y build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 curl git zip unzip nsis g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64 mingw-w64-x86-64-dev
```

Optional check:

```bash
makensis -VERSION
```

---

## Build Windows wallet through `depends`

Run all commands from the project root:

```bash
cd /root/mbc-win
```

### 1. Build `depends`

```bash
make -C depends HOST=x86_64-w64-mingw32 -j2
```

### 2. Prepare autotools

```bash
./autogen.sh
```

### 3. Configure for Win64

```bash
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --host=x86_64-w64-mingw32 --prefix=$PWD/depends/x86_64-w64-mingw32
```

### 4. Build wallet binaries

```bash
make -j2
```

Expected output files:

```bash
ls -lh /root/mbc-win/src/mbcd.exe /root/mbc-win/src/mbc-cli.exe /root/mbc-win/src/mbc-tx.exe /root/mbc-win/src/qt/mbc-qt.exe
```

---

## Rebuild after editing `chainparams.cpp`

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

## Mainnet `mbc.conf` example

For a normal user wallet, create or edit the configuration file like this:

```ini
rpcuser=mbc_rpc_user
rpcpassword=change_this_to_a_long_random_password
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

Current main network reference:
- main server: `31.131.21.71`
- explorer: `https://explorer.megabytecoin.pp.ua/`
- active mainnet node ports on `31.131.21.71`: `18777`, `18779`, `18781`

---

## Testnet note

A separate Windows test wallet executable is **not** required.
The same wallet binary can work on testnet using a separate config with `testnet=1`.

Example testnet config:

```ini
testnet=1
rpcuser=mbc_test_user
rpcpassword=change_this_to_a_long_random_password
listen=1
daemon=1
server=1
txindex=1
port=28777
rpcport=28778
addnode=31.131.21.71:28777
```

---

## Useful output check

After a successful build, verify the files:

```bash
ls -lh /root/mbc-win/src/mbcd.exe /root/mbc-win/src/mbc-cli.exe /root/mbc-win/src/mbc-tx.exe /root/mbc-win/src/qt/mbc-qt.exe
```

---

## Optional NSIS installer

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
