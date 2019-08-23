// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef Altmarkets_VERSION_H
#define Altmarkets_VERSION_H

#define VERSION_IDENTIFIER "Altmarkets"
static const int PROTOCOL_VERSION = 70200;
static const int INIT_PROTO_VERSION = 209;

// In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 60800;
static const int MIN_PEER_PROTO_VERSION = 60900;
static const int CADDR_TIME_VERSION = 31402;

// only request blocks from nodes outside this range of versions
static const int NOBLKS_VERSION_START = 32000;
static const int NOBLKS_VERSION_END = 32400;
static const int BIP0031_VERSION = 60200;

#endif // Altmarkets_VERSION_H
