# Network
Reverse engineering Alicia network. Done with [Procmon](https://docs.microsoft.com/en-us/sysinternals/downloads/procmon) and [Wireshark](https://www.wireshark.org).

## Alicia protocol
TCP/IP
### Server Types
| Server | Desc |
| ------ | ---- |
| Relay  | ranch, ... |
| Lobby  | |
| Race   | | 
| Guild Chat | |
### Network messages
#### Codec
| Type | Name | Note |
| ------ | ---- | ---- |
| uint32 | Message magic | Encoded message information |
| uint8[x] | Message data | Actual message data |

[Example of how to encode and decode message information](https://gist.github.com/rgnter/05acd36492a28ea2bf133f3d85ad16c3)


## Alicia Online protocol
### Login endpoint (Alicia Online middle man)
// todo
Request:
```
15:46:19.911 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint - /login.php
15:46:19.911 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint - Request: 
15:46:19.911 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Method: POST
15:46:19.911 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   URI: /login.php
15:46:19.911 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Headers:
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -       Referer: https://aliciagame.com
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -       Host: login.aliciagame.com
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -       User-agent: Mozilla/5.0 (Windows NT 5.1; rv:19.0) Gecko/20100101 Firefox/19.0
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -       Content-type: application/x-www-form-urlencoded
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -       Content-length: 83
15:46:19.912 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Body: login=regent&password=<password>&token=<token>
```
Token is not required.

Response:
```
15:50:05.455 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint - /login.php
15:50:05.625 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint - Response: 
15:50:05.625 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Headers: 
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Server: Microsoft-IIS/8.5
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Content-Length: 294
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Date: Thu, 18 Nov 2021 14:50:08 GMT
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   X-Powered-By: PHP/7.0.0
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint -   Content-Type: text/html; charset=UTF-8
15:50:08.490 [HTTP-Dispatcher] INFO  xyz.rgnt.endpoint.AliciaLoginEndpoint - Body: 		|GameId=Alicia|MemberNo=830451|LoginID=regent|AuthKey=<authKey>|InstallUrl=http://install.aliciagame.com/Client/Alicia_mini_setup_04.exe|ServerType=0|ServerInfo=:|Age=16|Sex=2|Birthday=|WardNo=0|CityCode=00|ZipCode=|PCBangNo=0|CloseTime=
```

### Loopback endpoints
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
