#include <iostream>
#include <vector>
#include <string>

#include "src/chainparams.h"
#include "src/primitives/block.h"
#include "src/primitives/transaction.h"
#include "src/script/script.h"
#include "src/script/script.h" // CScriptNum is defined here in this fork
#include "src/utilstrencodings.h"
#include "src/uint256.h"

// Compact target (nBits) to uint256, adapted from Bitcoin Core
static uint256 CompactToUint256(uint32_t nCompact) {
    int nSize = nCompact >> 24;
    uint32_t nWord = nCompact & 0x007fffff;
    uint256 ret;
    if (nSize <= 3) {
        nWord >>= 8 * (3 - nSize);
        ret = uint256(nWord);
    } else {
        ret = uint256(nWord);
        ret <<= 8 * (nSize - 3);
    }
    return ret;
}

static bool CheckTarget(const uint256& hash, uint32_t nBits) {
    uint256 target = CompactToUint256(nBits);
    return hash <= target;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pszTimestamp> [nTime] [nBits]\n";
        return 1;
    }
    std::string pszTimestamp = argv[1];
    // Allow spaces by joining remaining args
    for (int i = 2; i < argc; ++i) {
        pszTimestamp += " ";
        pszTimestamp += argv[i];
    }

    // Parameters copied from chainparams.cpp mainnet block (can be overridden by argv)
    const std::string pubkey_hex = "04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f";
    uint32_t nTime = 1566556888;
    uint32_t nBits = 504365040;

    if (argc >= 3) nTime = (uint32_t)std::stoul(argv[2]);
    if (argc >= 4) nBits = (uint32_t)std::stoul(argv[3]);

    CBlock genesis;
    genesis.nVersion = 1;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.hashPrevBlock.SetNull();

    CMutableTransaction txNew;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
        << std::vector<unsigned char>((const unsigned char*)pszTimestamp.data(), (const unsigned char*)pszTimestamp.data() + pszTimestamp.size());
    txNew.vout[0].nValue = 0 * COIN;
    txNew.vout[0].scriptPubKey = CScript() << ParseHex(pubkey_hex) << OP_CHECKSIG;

    genesis.vtx.push_back(txNew);
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    std::cout << "Timestamp: " << pszTimestamp << "\n";
    std::cout << "MerkleRoot: " << genesis.hashMerkleRoot.ToString() << "\n";
    std::cout << "nTime: " << nTime << "\n";
    std::cout << "nBits: " << nBits << "\n";
    std::cout << "Searching for nonce...\n";

    // brute nonce
    for (uint32_t nonce = 0; nonce < 0xFFFFFFFF; ++nonce) {
        genesis.nNonce = nonce;
        uint256 h = genesis.GetHash();
        if (CheckTarget(h, nBits)) {
            std::cout << "Found nonce: " << nonce << "\n";
            std::cout << "GenesisHash: " << h.ToString() << "\n";
            return 0;
        }
        if ((nonce % 1000000) == 0) {
            // progress
            // std::cerr << "nonce="<<nonce<<" hash="<<h.ToString()<<"\n";
        }
    }

    std::cerr << "No nonce found (unexpected).\n";
    return 2;
}
