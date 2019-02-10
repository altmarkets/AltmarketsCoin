
Debian
====================
This directory contains files used to package Altmarketsd/Altmarkets-qt
for Debian-based Linux systems. If you compile Altmarketsd/Altmarkets-qt yourself, there are some useful files here.

## Altmarkets: URI support ##


Altmarkets-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install Altmarkets-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your Altmarkets-qt binary to `/usr/bin`
and the `../../share/pixmaps/Altmarkets128.png` to `/usr/share/pixmaps`

Altmarkets-qt.protocol (KDE)

