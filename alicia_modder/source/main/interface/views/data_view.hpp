//
// Created by maros on 12/09/2021.
//

#ifndef ALICIA_MODDER_DATA_VIEW_HPP
#define ALICIA_MODDER_DATA_VIEW_HPP

#include <cstdint>


namespace mod {
    class DataView {
        enum class ViewType {
            BINARY,
            ASCII,

            MIXED,

            TEXTURE_PNG,
            TEXTURE_DDS
        };

    public:
        uint32_t align = 30;
        ViewType type;

    public:
        void render_data_view(uint8_t *data, size_t data_length);
    };
}



#endif //ALICIA_MODDER_DATA_VIEW_HPP
