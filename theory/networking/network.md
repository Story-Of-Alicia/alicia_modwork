# Network
Reverse engineering Alicia network. Done with [Procmon](https://docs.microsoft.com/en-us/sysinternals/downloads/procmon) and [Wireshark](https://www.wireshark.org).

## Loopback endpoints
Communication endpoints in **AliciaLauncher** and **AliciaGame** are done with **HTTP/1.1***(over TCP/IP)*. 

### Protocol

#### Endpoints
###### AliciaLauncher
Runs on port `8512`
- `/Client`
	- `/GameInfo.dat` - LUA Script that configures launcher.
- `/Patch` 
    - `/Datas` - Contains game assets
- `/LauncherContents`
###### AliciaGame
