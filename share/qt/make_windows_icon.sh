#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/Altmarkets.png
ICON_DST=../../src/qt/res/icons/Altmarkets.ico
convert ${ICON_SRC} -resize 16x16 Altmarkets-16.png
convert ${ICON_SRC} -resize 32x32 Altmarkets-32.png
convert ${ICON_SRC} -resize 48x48 Altmarkets-48.png
convert Altmarkets-16.png Altmarkets-32.png Altmarkets-48.png ${ICON_DST}

