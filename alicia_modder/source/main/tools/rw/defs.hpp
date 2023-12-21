//
// Created by maros on 16/09/2021.
//

#ifndef ALICIA_MODDER_DEFS_HPP
#define ALICIA_MODDER_DEFS_HPP

#include <cstdint>

namespace rw_stream
{

enum RwSectionID
{
  rwSECTION_STRUCT = 0x01,
  rwSECTION_CLUMP = 0x10,
};

#pragma pack(push, 1)
struct RwSection
{
  uint32_t section_id;
  uint32_t section_length;
  uint16_t section_unknown;
  uint16_t section_version;
};
#pragma pack(pop)


}// namespace rw_stream


#endif//ALICIA_MODDER_DEFS_HPP
