# Network
Reverse engineering Alicia network.

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
