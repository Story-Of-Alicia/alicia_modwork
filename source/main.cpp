/**
* Created by rgnt.xyz
*
*/


#include <cstdio>
#include "file/pak.hpp"


int main() {
    PakFile pak(R"(D:\games\Alicia\res.pak)");
    pak.LoadFromDisk();

    return 0;
}


