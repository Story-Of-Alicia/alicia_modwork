/**
* Created by rgnt.xyz
*
*/

#include <windows.h>
#include <cstdio>
#include "pak/pak.hpp"


int main() {
    PakFile pak(R"(D:\games\Alicia\res.pak)");
    pak.LoadFromDisk();

    return 0;
}


