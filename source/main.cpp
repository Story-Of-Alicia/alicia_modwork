/**
* Created by rgnt.xyz
*
*/


#include <cstdio>
#include "file/pak.hpp"


int main() {
//    printf("Specify path to res.pak >");
//    char path[512] = {0};
//    scanf("%s", path);

    PakFile pak(R"(D:\games\Alicia\res.pak)");
    pak.LoadFromDisk();

    return 0;
}


