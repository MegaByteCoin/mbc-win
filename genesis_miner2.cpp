#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#include "src/primitives/transaction.h"
#include "src/script/script.h"
#include "src/serialize.h"
#include "src/hash.h"
#include "src/uint256.h"

// Provide HexDigit for uint256.cpp without pulling full utilstrencodings.cpp
signed char HexDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Minimal hex decode (no utilstrencodings dependency)
static std::vector<unsigned char> HexToBytes(const std::string& hex) {
    auto hexval = [](char c)->int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    std::vector<unsigned char> out;
    out.reserve(hex.size()/2);
    int hi = -1;
    for (char c : hex) {
        int v = hexval(c);
        if (v < 0) continue;
        if (hi < 0) hi = v;
        else {
            out.push_back((unsigned char)((hi<<4) | v));
            hi = -1;
        }
    }
    return out;
}

static void AppendLE32(std::vector<unsigned char>& v, uint32_t x) {
    v.push_back((unsigned char)(x & 0xFF));
    v.push_back((unsigned char)((x >> 8) & 0xFF));
    v.push_back((unsigned char)((x >> 16) & 0xFF));
    v.push_back((unsigned char)((x >> 24) & 0xFF));
}

static void AppendLE32S(std::vector<unsigned char>& v, int32_t x) {
    AppendLE32(v, (uint32_t)x);
}

// Compact target (nBits) to uint256 (same as previous tool)
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

static uint256 ComputeHeaderHashQuark(int32_t nVersion, const uint256& prev, const uint256& merkle, uint32_t nTime, uint32_t nBits, uint32_t nNonce) {
    std::vector<unsigned char> d;
    d.reserve(4 + 32 + 32 + 4 + 4 + 4);
    AppendLE32S(d, nVersion);
    d.insert(d.end(), prev.begin(), prev.end());
    d.insert(d.end(), merkle.begin(), merkle.end());
    AppendLE32(d, nTime);
    AppendLE32(d, nBits);
    AppendLE32(d, nNonce);
    return HashQuark(d.begin(), d.end());
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <pszTimestamp> <nTime> <nBits>\n";
        return 1;
    }

    std::string pszTimestamp = argv[1];
    // Allow spaces by joining remaining args except last two (nTime,nBits) if user quoted wrongly.
    // Here we assume argv[1] is already quoted if it contains spaces.

    uint32_t nTime = (uint32_t)std::stoul(argv[2]);
    uint32_t nBits = (uint32_t)std::stoul(argv[3]);

    // Keep the original pubkey from chainparams.cpp mainnet genesis txout
    const std::string pubkey_hex = "04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f";

    // Build coinbase tx (same structure as chainparams.cpp)
    CMutableTransaction txNew;
    txNew.vin.resize(1);
    txNew.vout.resize(1);

    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
        << std::vector<unsigned char>((const unsigned char*)pszTimestamp.data(), (const unsigned char*)pszTimestamp.data() + pszTimestamp.size());

    txNew.vout[0].nValue = 0 * COIN;

    std::vector<unsigned char> pubkey = HexToBytes(pubkey_hex);
    txNew.vout[0].scriptPubKey = CScript() << pubkey << OP_CHECKSIG;

    // For 1-tx genesis, merkle root == tx hash
    CTransaction tx(txNew);
    uint256 merkleRoot = tx.GetHash();

    uint256 prev;
    prev.SetNull();

    std::cout << "Timestamp: " << pszTimestamp << "\n";
    std::cout << "nTime: " << nTime << "\n";
    std::cout << "nBits: " << nBits << "\n";
    std::cout << "MerkleRoot: " << merkleRoot.ToString() << "\n";
    std::cout << "Searching for nonce...\n";

    for (uint32_t nonce = 0; nonce < 0xFFFFFFFF; ++nonce) {
        uint256 h = ComputeHeaderHashQuark(1, prev, merkleRoot, nTime, nBits, nonce);
        if (CheckTarget(h, nBits)) {
            std::cout << "Found nonce: " << nonce << "\n";
            std::cout << "GenesisHash: " << h.ToString() << "\n";
            return 0;
        }
        if ((nonce % 2000000u) == 0u) {
            // progress marker
        }
    }

    std::cerr << "No nonce found (unexpected).\n";
    return 2;
}
