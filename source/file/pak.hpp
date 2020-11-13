//
// Created by maros on 10/24/2020.
//

#ifndef ALICIACPAK_PAK_HPP
#define ALICIACPAK_PAK_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <csignal>
#include <unordered_map>
#include <string>

#include "buffer/buffer.hpp"

/**
 * PAK Asset
 */
struct PakAsset {
    uint32_t prefix{}; // Always 0x0
    uint32_t magic{};

    uint32_t dataOffset{};
    uint32_t dataLength{};

    uint32_t uncompressedLength0{};
    uint32_t isCompressed{};
    uint32_t uncompressedLength1{};
    uint32_t unknown0{}; // Always 0x0
    uint32_t uncompressedLength2{};

    uint32_t unknown1{};
    uint32_t unknown2{};
    uint32_t unknown3{};
    uint32_t unknown4{};
    uint32_t unknown5{};
    uint32_t unknown6{};
    uint32_t entryOffset{};
    uint32_t isPacked{};

    uint64_t unknownType{};
    uint64_t unknownValue{};

    uint32_t unknown8{};
    uint32_t unknown9{};

    uint32_t crcId{};
    uint32_t crcValue{};
    uint64_t unknown10{};

    std::wstring path;
    uint8_t *data{};
};

/**
 * Represents content of PAK
 *
 * too slow ;-;
 */
class PakContent {
private:
    std::unordered_map<std::wstring, PakAsset> assets;

public:
    /**
     * @return Asset count
     */
    uint64_t GetAssetCount() const {
        return assets.size();
    }

public:
    /**
     * Overload array operator
     * @param path Path of Asset
     * @return Asset
     */
    PakAsset operator[](const std::wstring& path) const {
        return this->assets.at(path);
    }

    /**
     * Overload plus operator
     * @param asset Asset to add
     * @return this
     */
    PakContent operator+=(PakAsset& asset) {
        this->assets[asset.path] = asset;
        return *this;
    }
};

/**
 * PAK File
 */
class PakFile {
private:
    FILE* fileHandle;
    bool isOk = false;

public:
    const static uint64_t ENTRY_SECTOR = 0x7D000;
    const static uint64_t DATA_SECTOR  = 0xF00000;

public:
    /**
     * Default constructor
     * @param path Path to PAK file
     */
    explicit PakFile(const char* path);

    /**
     * Default deconstructor
     */
    ~PakFile();

public:
    /**
     * Loads PAK from disk
     */
    void LoadFromDisk();

    /**
     * Destroys all data
     */
    void Destroy();

};


#endif //ALICIACPAK_PAK_HPP
