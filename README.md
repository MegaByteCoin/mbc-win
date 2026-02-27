# mbc-win

MegaByteCoin (MBC) wallet sources for building **Windows** binaries (Qt GUI + daemon + CLI) on **Ubuntu 20.04** using the `depends` system (MinGW-w64 cross-compilation) and an optional **NSIS** installer.

> This repository is focused on Windows builds. For Linux builds, use a separate copy (e.g. `mbc-ubuntu`) and build `depends` without `HOST`.

---

## Layout

- `depends/` — dependency build system (MinGW-w64 toolchain, BDB, Boost, Qt, etc.)
- `src/` — daemon/cli/tx sources
- `src/qt/` — Qt GUI sources
- `contrib/` — helper scripts (may include NSIS installer files)

---

## Requirements (Ubuntu 20.04)

```bash
sudo apt update
sudo apt install -y software-properties-common
sudo add-apt-repository -y universe
sudo apt update

sudo apt install -y \
  build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 \
  g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64 \
  curl git zip unzip nsis
```

Check NSIS:
```bash
makensis -VERSION
```

---

## Build Windows binaries via `depends` (x86_64)

Run all commands from the project root (where `autogen.sh`, `configure.ac`, `depends/`, `src/` exist).

### 1) Build depends

```bash
cd /root/mbc-win
make -C depends HOST=x86_64-w64-mingw32 -j"$(nproc)"
```

### 2) Use `config.site` (recommended)

```bash
export HOST=x86_64-w64-mingw32
export CONFIG_SITE="$PWD/depends/$HOST/share/config.site"
```

Verify:
```bash
ls -la "$CONFIG_SITE"
```

### 3) Autogen + Configure

```bash
./autogen.sh
./configure --prefix="$PWD/depends/$HOST" --host="$HOST" --disable-tests --disable-bench
```

If `configure` complains about BerkeleyDB (requires 4.8), force the paths:
```bash
./configure --prefix="$PWD/depends/$HOST" --host="$HOST" --disable-tests --disable-bench \
  BDB_LIBS="-L$PWD/depends/$HOST/lib -ldb_cxx-4.8 -ldb-4.8" \
  BDB_CFLAGS="-I$PWD/depends/$HOST/include"
```

### 4) Compile

```bash
make -j"$(nproc)"
```

Expected outputs:
- `src/mbcd.exe`
- `src/mbc-cli.exe`
- `src/mbc-tx.exe`
- `src/qt/mbc-qt.exe`

Check:
```bash
ls -la src/*.exe src/qt/*.exe 2>/dev/null || true
```

---

## Build NSIS installer (optional)

Locate `.nsi` file:
```bash
find contrib -maxdepth 5 -type f -iname "*.nsi" -print
```

Build (replace with your `.nsi` path):
```bash
makensis /path/to/installer.nsi
```

---

## Publish to GitHub (quick)

```bash
cd /root/mbc-win
git init
git add .
git commit -m "Initial commit (mbc-win)"
git branch -M main
git remote add origin https://github.com/<USER>/<REPO>.git
git push -u origin main
```

---

## Notes

- To rebuild quickly:
  ```bash
  make clean || true
  rm -f config.cache
  ```
- Do not commit build outputs (`depends/*`, `*.o`, `*.exe`, `*.a`, `*.dll`). Use a proper `.gitignore`.

---

## License

See `COPYING` (if present) and third-party dependency licenses.




# mbc-win
MegaByteCoin Windows build source with depends cross-compilation support (mingw64).
