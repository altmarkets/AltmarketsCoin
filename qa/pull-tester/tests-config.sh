#!/bin/bash
# Copyright (c) 2013-2014 The Bitcoin Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

BUILDDIR="/home/repo/wallet/Altmarkets-SEQ"
EXEEXT=""

# These will turn into comments if they were disabled when configuring.
ENABLE_WALLET=1
ENABLE_UTILS=1
ENABLE_AltmarketsD=1

REAL_AltmarketsD="$BUILDDIR/src/Altmarketsd${EXEEXT}"
REAL_AltmarketsCLI="$BUILDDIR/src/Altmarkets-cli${EXEEXT}"

