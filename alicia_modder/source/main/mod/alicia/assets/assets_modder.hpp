//
// Created by maros on 16/09/2021.
//

#ifndef ALICIA_MODDER_ASSETS_MODDER_HPP
#define ALICIA_MODDER_ASSETS_MODDER_HPP

#include "main/tools/assets/assets.hpp"
#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

namespace mod {
    class AssetsModder {
    public:
        assets::PakContents pakContents;
        assets::InfoContents infoContents;

    public:
        std::string pak_path;
        std::string info_path;

    private:
        volatile bool working = false;
        std::thread worker;
        spdlog::stopwatch workerElapsed;
        std::string workerDesc;

    public:
        void renderInterface();

    private:
        template<typename Func>
        bool submitWork(const Func& function,
                        const std::string& desc);
    };
}

#endif //ALICIA_MODDER_ASSETS_MODDER_HPP
