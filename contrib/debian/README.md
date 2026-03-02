
Debian
====================
This directory contains files used to package mbcd/mbc-qt
for Debian-based Linux systems. If you compile mbcd/mbc-qt yourself, there are some useful files here.

## mbc: URI support ##


mbc-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install mbc-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your mbcqt binary to `/usr/bin`
and the `../../share/pixmaps/mbc128.png` to `/usr/share/pixmaps`

mbc-qt.protocol (KDE)

