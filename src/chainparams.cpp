// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "random.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "uint256.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>

using namespace std;

void MineGenesis(CBlock genesis, uint256 bnProofOfWorkLimit){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = bnProofOfWorkLimit;
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
        ++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
    newhash = genesis.GetHash();
    if(newhash < besthash){
        besthash=newhash;
        printf("New best: %s\n", newhash.GetHex().c_str());
    }
    }
    printf("Genesis Hash: %s\n", genesis.GetHash().ToString().c_str());
    printf("Genesis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
    printf("Genesis nTime: %u\n", genesis.nTime);
    printf("Genesis nBits: %08x\n", genesis.nBits);
    printf("Genesis Nonce: %u\n\n\n", genesis.nNonce);
}

/**
 * Main network
 */
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    std::vector<CTxIn> vin;
    vin.resize(1);
    vin[0].scriptSig = CScript() << 1473949500 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    std::vector<CTxOut> vout;
    vout.resize(1);
    vout[0].scriptPubKey = genesisOutputScript;
    vout[0].nValue = 0;
    CMutableTransaction txNew(1, 1473949300, vin, vout, 0);

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    return genesis;
}

static CBlock CreateTestNetGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    std::vector<CTxIn> vin;
    vin.resize(1);
    vin[0].scriptSig = CScript() << 1473949300 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    std::vector<CTxOut> vout;
    vout.resize(1);
    vout[0].scriptPubKey = genesisOutputScript;
    vout[0].nValue = 0;
    CMutableTransaction txNew(1, 1478106900, vin, vout, 0);

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    return genesis;
}

static CBlock CreateTestNetGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "AP November 2nd 2016: Telescope reveals amazing pillars in star-breeding ground";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateTestNetGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times Sept 15th 2016: Pardon Edward Snowden";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 26;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 26;
        consensus.nEnforceBlockUpgradeMajority = 750;
        consensus.nRejectBlockOutdatedMajority = 950;
        consensus.nToCheckBlockUpgradeMajority = 1000;
        consensus.nMinerThreads = 0;
        consensus.nTargetSpacingMax = 1 * 64;     // 64 second max spacing target
        consensus.nPoWTargetSpacing = 1 * 60;     // 60 seconds PoW Target
        consensus.fAllowMinDifficultyBlocks = false;
        consensus.nPoSTargetSpacing = 1 * 64;     // 64 seconds PoS Target
        consensus.nStakeMinAge = 1 * 60 * 60;     // 1 hour minimum stake age
        consensus.nStakeMaxAge = std::numeric_limits<int64_t>::max(); // Unlimited stake age
        consensus.nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        consensus.nLastPOWBlock = 800000;          // Proof of Work finishes on block 800000 to secure network

        genesis = CreateGenesisBlock(1473949500, 37239843, consensus.bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vSeeds.push_back(CDNSSeedData("88.131.213.108:16662", "88.131.213.108:16662"));
        vSeeds.push_back(CDNSSeedData("88.131.213.108:16663", "88.131.213.108:16663"));
        vSeeds.push_back(CDNSSeedData("88.131.213.108:16664", "88.131.213.108:16664"));
        
        // Altmarkets PUBKEY_ADDRESS addresses start with 'A'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        // Altmarkets SCRIPT_ADDRESS addresses start with 'S or T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,64);
        // Altmarkets SECRET_KEY start with '2'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,170);
        // Altmarkets BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        // Altmarkets BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        // Altmarkets BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0x10;
        pchMessageStart[2] = 0x02;
        pchMessageStart[3] = 0x20;
        vAlertPubKey = ParseHex("0485b5fbf78c8a58403721d3678a14dfac93833642d8ee53df5d8c2b168b1fd7878e05a796fbe2327394debaaa16085f13ba5db368720c34f8dcdbe70b1eb3ca38");
        nDefaultPort = 16662;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            {{ 0, uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7")},
            { 500, uint256("0x000000007bf1e762992a4d6369d9689ae501000ffec12c25c25eccbf53ded4d0")},
            { 1000, uint256("0x00000000b9a708200eec1c595aff10844cbe2f4d6cad046d8446c616ddbce87e")},
            { 4000, uint256("0x00000020b1d99f01d3debd634fbb053dc520b6e5772294cb9a0ac8cd339899e7")},
            { 10000, uint256("0x0000001fc4553f7eff02dc3c99ba97f3ff21020879b39fd2fb7a3dca854ab73e")},
            { 40000, uint256("0xce181929ebf618abb50857f0c0277e828bca58fcb0c10827c7ad367343844b23")},
            { 120000, uint256("0x4f2782c313464af8ab525bde4bd2fb8e37b9ad23627734e4f01a96e733f9f525")}},
            1567321179, // * UNIX timestamp of last checkpoint block
            192873,          // * total number of transactions between genesis and last checkpoint
						  //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 26;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 26;
        consensus.nEnforceBlockUpgradeMajority = 51;
        consensus.nRejectBlockOutdatedMajority = 75;
        consensus.nToCheckBlockUpgradeMajority = 100;
        consensus.nMinerThreads = 0;
        consensus.fAllowMinDifficultyBlocks = false;
        consensus.nStakeMinAge = 30 * 60;         // 30 minute minimum stake age
        consensus.nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        consensus.nLastPOWBlock = 100000;         // Proof of Work finishes on block 300000
        bool startNewChain = false;

        genesis = CreateTestNetGenesisBlock(1478107000, 82131309, consensus.bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));
        if(startNewChain == true) { MineGenesis(genesis, consensus.bnProofOfWorkLimit); }
        consensus.hashGenesisBlock = genesis.GetHash();

  //      assert(consensus.hashGenesisBlock == uint256("0x00000009cfa952b4f748c8cf7cdf975f89bce0a26a6a7e7c8cb33968bc20848e"));
  //      assert(genesis.hashMerkleRoot == uint256("0x2daaebb4a85d64d0a5266ca58656c8db03f95a7c9c863eb34872b48c3c6f3dea"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Altmarkets PUBKEY_ADDRESS addresses start with 'N'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,53);
        // Altmarkets PUBKEY_ADDRESS addresses start with 'M'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,50);
        // Altmarkets SECRET_KEY start with 'X or 7'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x11, 0x35, 0xAA, 0xEE};
        base58Prefixes[EXT_SECRET_KEY] = {0x35, 0x11, 0xDD, 0xFF};
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        pchMessageStart[0] = 0x02;
        pchMessageStart[1] = 0x20;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0x30;
        vAlertPubKey = ParseHex("");
        nDefaultPort = 16664;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        
        checkpointData = (CCheckpointData) {
            {{ 0, uint256("0x00000009cfa952b4f748c8cf7cdf975f89bce0a26a6a7e7c8cb33968bc20848e")}},
            1478107000, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
						  //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nEnforceBlockUpgradeMajority = 750;
        consensus.nRejectBlockOutdatedMajority = 950;
        consensus.nToCheckBlockUpgradeMajority = 1000;
        consensus.nMinerThreads = 1;
        consensus.bnProofOfWorkLimit = ~uint256(0) >> 22;
        consensus.bnProofOfStakeLimit = ~uint256(0) >> 22;
        consensus.fAllowMinDifficultyBlocks = true;
        consensus.nLastPOWBlock = 100;    // Proof of Work finishes on block 100
        
        genesis = CreateGenesisBlock(1473949500, 1427578, 0x1e00ffff, 1, (0 * COIN));
        
        consensus.hashGenesisBlock = genesis.GetHash();

 //       assert(consensus.hashGenesisBlock == uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f"));
 //       assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        nExtCoinType = 1;

        pchMessageStart[0] = 0x40;
        pchMessageStart[1] = 0x04;
        pchMessageStart[2] = 0x50;
        pchMessageStart[3] = 0x05;
        nDefaultPort = 16666;
        nMaxTipAge = 24 * 60 * 60;

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        
        checkpointData = (CCheckpointData) {
            {{ 0, uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f")}},
            1473949500, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
						  //   (the tx=... number in the SetBestChain debug.log lines)
            2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
        return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
        return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
