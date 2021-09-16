//
// Created by maros on 16/09/2021.
//

#ifndef ALICIA_MODDER_FILESYSTEM_HPP
#define ALICIA_MODDER_FILESYSTEM_HPP

#include <string>

namespace filesystem {
    class helper {
    public:
        static std::string UserSelectFile(const char* title,
                                          const char* filter,
                                          int flags = 0);

        static std::string UserSelectFolder(
                const char* title,
                int flags = 0);

        static void UserMessage(
                const char* title,
                const char* desc
                );
    };
}


#endif //ALICIA_MODDER_FILESYSTEM_HPP
