//
// Created by maros on 16/09/2021.
//

#ifndef ALICIA_MODDER_ASSETS_HPP
#define ALICIA_MODDER_ASSETS_HPP


#include <system_error>
#include <vector>
#include <memory>
#include <string>

namespace assets {
#pragma pack(push, 1)

    static constexpr size_t PAK_ASSET_HEADER_ADDR = 0x7D000;
    static constexpr size_t PAK_DATA_SECTOR_ADDR = 0xF00000;

    struct pak_file_header {
    public:
        uint32_t header_magic{};
        uint32_t unknown0{};
        uint32_t paklib_version{};
        uint32_t locale{};

        uint32_t asset_count{};
        uint32_t used_assets_count{};
        uint32_t deleted_assets_count{};

        uint32_t file_size{};
        uint32_t unknown1{};
        uint32_t header_sign{};
    };

    struct pak_data_header {
        uint32_t magic = 0x454C4946; // ASCII: FILE
    };

    struct info_file_footer {
    public:
        uint32_t unknown0{};
        uint32_t magic = 1162627398; // ASCII: FILE
    };


    /**
     * Represents PAK content header
     */
    struct pak_content_header {
    public:
        uint32_t first_magic{};
        uint32_t second_magic{};

        uint32_t asset_count{};
    };


    /**
     * Represents DAT content header
     */
    struct dat_content_header {
    public:
        uint32_t first_magic{};

        uint32_t unknown0{};
        uint32_t unknown1{};
        uint32_t unknown2{};
        uint32_t unknown3{};

        uint32_t second_magic{};
        uint32_t asset_count{};
    };

    /**
     * Represents common asset entries
     */
    struct asset_common {
    public:
        uint32_t asset_prefix{}; // Always 0x0
        uint32_t asset_magic{};

        uint32_t embedded_data_offset{};
        uint32_t embedded_data_length{};

        uint32_t data_decompressed_length{};
        uint32_t is_data_compressed{};
        uint32_t data_decompressed_length0{};
        uint32_t unknown0{};
        uint32_t data_decompressed_length1{};

        uint32_t unknown1{};
        uint32_t unknown2{};
        uint32_t unknown3{};
        uint32_t unknown4{};
        uint32_t unknown5{};
        uint32_t is_asset_deleted{};
        uint32_t asset_offset{};
        uint32_t is_asset_embedded{};

        uint64_t unknown_type{};
        uint64_t unknown_value{};

        uint32_t crc_decompressed{};
        uint32_t crc_embedded{};
        uint32_t crc_identity{};

        uint32_t unknown7{};
        uint64_t unknown8{};
    };

    /**
     * Represents PAK Asset header
     */
    struct pak_asset_header {
    public:
        asset_common common{};

    public:
        wchar_t path[256] = {0};
    };

    /**
     * Represents DAT Asset header
     */
    struct dat_asset_header {
    public:
        asset_common common{};
    };

#pragma pack(pop)

    /**
     * Represents Pak Asset
     */
    class PakAsset {
    public:
        pak_asset_header header;

    public:
        std::shared_ptr<uint8_t> data;

    };

    /**
     * Represents Dat Asset
     */
    class DatAsset {
    public:
        uint8_t magic[3] = {0};
        uint32_t unknown0{};

    public:
        std::shared_ptr<uint8_t> data;

    public:
        dat_asset_header header;
    };


    /**
     * Stores contents of PAK
     */
    class PakContents {
    public:
        pak_file_header fileHeader{};
        pak_content_header contentHeader{};
        pak_data_header dataHeader{};

    public:
        std::vector<std::shared_ptr<PakAsset>> assets;

    public:
        inline bool hasAssets() const {
            return assets.size()>0;
        }
    };


    /**
     * Stores contents of INFO
     */
    class InfoContents {
    public:
        dat_content_header contentHeader{};
        info_file_footer   fileFooter{};

    public:
        std::vector<DatAsset> assets;
    };


    class AssetsIO {
    public:
        /**
         * Reads Pak Contents from file.
         * @param path     Path to assets file. Must exist.
         * @param contents Contents pointer.
         */
        static void read_pak_contents(const std::string &path,
                                      PakContents &contents);

        /**
         * Writes Pak Contents to file.
         * @param path     Path to assets file.
         */
        static void write_pak_contents(const std::string &path,
                                       PakContents &contents);


        /**
         * Reads Info contents from file
         * @param path     Path to info file. Must exist.
         * @param contents Contents pointer.
         * @return todo
         */
        static bool read_info_contents(const std::string &path,
                                       InfoContents &contents);

        /**
         * Writes Info contents from file.
         * @param path     Path to info file.
         * @param contents Contents pointer.
         * @return todo
         */
        static bool write_info_contents(const std::string &path,
                                        InfoContents &contents);


        /**
         * Writes Pak asset to file.
         * @param parent_dir Parent dir.
         * @param asset      Pak Asset.
         * @return todo
         */
        static bool export_pak_asset(const std::string &parent_dir,
                                     const std::shared_ptr<PakAsset>& asset);

        /**
         * Writes Dat asset to file.
         * @param parent_dir Parent dir.
         * @param asset      Dat Asset.
         * @return todo
         */
        static bool export_dat_asset(const std::string &parent_dir,
                                     const std::shared_ptr<PakAsset>& asset);
    };
}


#endif //ALICIA_MODDER_ASSETS_HPP
