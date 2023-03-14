# How to launch the game w/o the real launcher
## Login
Grab yourself login token from [AliciaOnline endpoint](https://github.com/rgnter/alicia_modwork/blob/master/theory/networking/network.md).
## Launcher
[Map a shared memory](https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory) file named `927628CA6D76A6E9162C56D4E3E6D6E3` and write the following buffer to it: 
```
\n\t\t|GameId=Alicia|MemberNo=<id>|LoginID=<name>|AuthKey=<key>|InstallUrl=http://127.0.0.1:8512/Client/Alicia_mini_setup_04.exe|ServerType=0|ServerInfo=:|Age=16|Sex=2|Birthday=|WardNo=0|CityCode=00|ZipCode=|PCBangNo=0
```
Example cpp code:
```cpp
void write_webinfo()
{
  auto file_handle = CreateFile(WEBINFO_ID,
                                  GENERIC_READ | GENERIC_WRITE,
                                  0,
                                  NULL,
                                  OPEN_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL,
                                  NULL);

    WriteFile(file_handle, WEBINFO, strlen(WEBINFO), NULL, NULL);

    DWORD error;
    auto shared_memory_handle = CreateFileMappingA(file_handle,
                                                   NULL, PAGE_READWRITE,
                                                   0, 0,
                                                   WEBINFO_ID);
    error = GetLastError();
    if(error) {
        printf("Couldn't create shared memory file(%ld).", error);
        return;
    }
    printf("Logged in!");
    CloseHandle(file_handle);
  // UnmapViewOfFile(shared_memory_handle);
}
```

## Game
Run alicia with the following command `Alicia.exe -GameID 4`, the GameID parameter here is specifying the launch info grabbed from the `libconfig.dat` configuration. 
