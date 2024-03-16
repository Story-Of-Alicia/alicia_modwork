# Network
Reverse engineering Alicia network.

## Alicia protocol
Alicia protocol is based on the TCP/IP protocol. 

### Server Types
| Server | Desc |
| ------ | ---- |
| Lobby  | Synchronous |
| Relay  | Asynchronous, (ranch, ...) |
| Race   | Asynchronous | 
| Guild Chat | |

### Message codec
| Type | Name | Note |
| ------ | ---- | ---- |
| uint32 | Message magic | Encoded message information |
| uint8[x] | Message data | Actual message data |

[Example of how to encode and decode message information](https://gist.github.com/rgnter/05acd36492a28ea2bf133f3d85ad16c3)
### Messages
#### CmdCLLogin
Serverbound
| Type | Name | Note |
| ------ | ---- | ---- |
| uint16_t | | |
| uint16_t | | |
| string[52] | | |
| uint32_t | Game ID | |
| string[260] | | |

#### CmdCLLogin OK
Clientbound
| Type | Name | Note |
| ------ | ---- | ---- |

#### CmdCLLogin CANCEL
Clientbound
| Type | Name | Note |
| ------ | ---- | ---- |
| uint8_t | Enum response code | |

If responde code value is outside the enum range, default login fail message is shown to the user. 

| Response code ordinal | Name | Note |
| ------ | ---- | ---- |
| 1 | CR_INVALID_USER | |
| 2 | CR_DUPLICATED | |
| 3 | CR_INVALID_VERSION | |
| 4 | CR_INVALID_EQUIPMENTS | |
| 5 | CR_INVALID_LOGIN_ID | |
| 6 | CR_DISCONNECT_YOURSELF | |
