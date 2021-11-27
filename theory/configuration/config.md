# Shared Configuration
Alicia uses registry as it's shared configuration provider.

## Launcher configuration
#### `HKCU\Software\AliciaOnline`
Contains information about:
| Field name | Field type    | Note |
| :--       |:--:           | :-- |
| Language   | `REG_SZ`      | Selected language |
| Name       | `REG_SZ`      | Selected profile name |
| Remember   | `REG_DWORD`   | Whether should launcher remember profiles or not |
| Update     | `REG_DWORD`   | Whether should launcher update or not |

#### `HKCU\Software\AliciaOnline\Login`
Contains information about:
| Field name | Field type      | Note |
| :--       |:--:             | :-- |
| [profile name]  | `REG_SZ`   | Probably contains profile password, unknown 216bit hash |

#### `HKLM\SOFTWARE\WOW6432Node\Ntreev\Alicia\`
| Field name | Field type    | Note |
| :--       |:--:           | :-- |
| Download resource   | `REG_DWORD`      ||
| Install_Dir       | `REG_SZ`      | Alicia Game location |
| Patcher   | `REG_SZ`   | Launcher module name (`Launcher.exe`) |
| Version     | `REG_SZ`   | In my case `008`|

## Game configuration

#### `HKCU\Software\Ntreev\Alicia\Option`
Contains information about:
| Field name | Field type      | Note |
| :--       |:--:             | :-- |
| Bloom      | `REG_SZ`      | |
| CamType    | `REG_SZ`      |  1-3, 1 being lowest|
| LensFlare  | `REG_SZ`      | |
| MotionBlur | `REG_SZ`      | |
| Movie      | `REG_SZ`      | |
| PostWebImage | `REG_SZ`      | |
| Quality      | `REG_SZ`      | |
| VolBGM      | `REG_SZ`      | |
| VolSFX      | `REG_SZ`      | |


#### `HKCU\Software\Ntreev\Alicia\Window`
Contains information about:
| Field name | Field type      | Note |
| :--       |:--:             | :-- |
| Height      | `REG_SZ`      | |
| screenResolutionID    | `REG_SZ`      ||
| screenWindowType  | `REG_SZ`      | |
| Width  | `REG_SZ`      | |

#### `HKCU\Software\Ntreev\Alicia\ShopList`
Contains information about:
| Field name | Field type      | Note |
| :--       |:--:             | :-- |
| XMLData_GoodsShop_4_version      | `REG_SZ`      | Contains `2013-10-01 16:02:35` |

# Game Configuration
## Client configuration
Contains information about translations, items, abilities, servers(game id info)

## Script configuration
Scripts can modify game behaviour by modifying variables created by client.
### Variable types
| Var name | desc |
| -------- | ---- |
| server var | |
| archive var | | 
| var | |

### Modifying
#### Scripts
Scripts can modify variables by calling `util::SetVar[Value|Float|String...]`. 

#### Console commands   
The variables can by also modified with console commands.
| Command name | Desc |
| ------------ | ----- | 
| set          | Sets variable |
| seta         | sets archive variable |
| seta         | sets server variable |

Usage: `<command name> <variable name> <variable value>`.

### Variable listing
All variables
#### Common Variables (`com_`)

#### Debug Variables (`dbg_`)

#### Camera Variables (`cam_`)

#### Command Variables  (`cmd_`)

#### PHY Variables (`phy_`)

#### Render Variables (`r_`)

#### Sound Variables (`snd_`)

#### System Variables (`sys_`)

#### Interface Variables (`ui_`)

#### Uncategorized
tempuser
item
maxAlp
maxDis
minAlp
minDis
