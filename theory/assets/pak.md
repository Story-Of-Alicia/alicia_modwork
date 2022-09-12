
# Alicia PAK format
> AliciaOnline version: `v1.318`. 

Reverse engineered with [ProcMon](https://docs.microsoft.com/en-us/sysuint32ernals/downloads/procmon), [HxD](https://mh-nexus.de/en/hxd/), [and my bare hands](https://github.com/rgnter/alicia_modwork/tree/master/source).<br>
With help of [presentation made by somebody from Ntreev team](https://prezi.com/a8bmssbbb4ay/v20/), I found some field names. If presentation is not available anymore, contact me for a copy.

## Trivia
PAKS is divided into three sections: File Header, Content Header, Asset Sector, Data Sector. <br>
PAKS codec uses little-endian byte sequence. <br>
[@HSReina](https://github.com/hsreina) made tool for unpacking(available on their website), but it isn't open-source and doesn't say anything about codec of PAKS. 

## Devs
You can use [C++ NPAK synchronous API](https://github.com/rgnter/alicia_modwork/blob/master/alicia_modder/source/main/tools/assets/assets.hpp) to write and read PAKs. It works, but is far from perfect.

## File Header
This header contains information about PAK itself. 
Is located at `0x00000`. 

`Total size: 40 bytes`
| Field type    | Field name              | Notes   |
| ------------- | ----------------------- | ------- |
| `uint32`      | Header magic         | Always `{0x50 0x41 0x4B 0x53}` (ANSI "PAKS") |
| `uint32`      | Unknown 0            | |
| `uint32`      | PakLib version       | |
| `uint32`      | Locale               | |
| `uint32`      | Asset count          | |
| `uint32`      | Used assets count    | |
| `uint32`      | Deleted assets count | |
| `uint32`      | File size            | (bytes) |
| `uint32`      | Team version          ||
| `uint32`      | Header sign          | Always `{0x4E 0x50 0x48 0x53}` (ASCII "NPHS") |

## Content Header 
This header contains information about assets. 
Is located at `0x7D000`. 

`Total size: 12 bytes`
| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `uint32`   | Magic                   | Always ASCII "FILS"|
| `uint32`   | Magic                   | Always ASCII "FILZ"|
| `uint32`   | Asset count             | Total asset count |



## Asset Sector
### Asset Header
Contains tightly packed array of Assets (of which size is described by Content Header). 
`Total size: 108 bytes (with path total is 620 bytes)`
| Field type    | Field name                   | Notes      |
| ----------    | -----------------------      | -------    |
| `uint32`      | Asset prefix              | Always 0x0                        | 
| `uint32`      | Asset magic               | Always a value between 0x30 and 0x40 (??) |
| `uint32`      | Embedded data offset      | Offset of embedded data.  |
| `uint32`      | Embedded data length      | Length of embedded data.  |
| `uint32`      | Data decompressed length  | Uncompressed length of data |
| `uint32`      | Are data compressed?      | Whether this data are compressed or not. |
| `uint32`      | Data decompressed length  | -- |
| `uint32`      | Unknown0                  |    |
| `uint32`      | Data decompressed length  | -- |
| `uint32`      | Unknown1                  |    |
| `uint32`      | Unknown2                  |    |
| `uint32`      | Unknown3                  |    |
| `uint32`      | Unknown4                  |    |
| `uint32`      | Unknown5                  |    |
| `uint32`      | Are data deleted          | Whether this data are deleted or not. |
| `uint32`      | Asset offset              | Contains information about offset of this header. |
| `uint32`      | Is embedded               | Whether this data are embedded or not. |
| `uint64`      | Unknown type              | Always ASCII "FIS\0" |
| `uint64`      | Unknown value             | |
| `uint32`      | Decompressed CRC          | *crc note |
| `uint32`      | Embedded CRC              | *crc note |
| `uint32`      | CRC Identification        | Always ASCII "CRC2\0"|
| `uint32`      | Decompressed Checksum                 | |
| `uint32`      | Embedded Checksum                  | |
| `uint32`      | Unknown 6             | |
| `wstr`        | Path (512bytes total)     | |


*crc note: Their CRC algorithm is not standard, they use **signed** integer to store the CRC value. See [implementation here](https://github.com/rgnter/alicia_modwork/blob/4fc8a6c69755a843920cd86a68fdf30c22c7506f/alicia_modder/source/main/tools/assets/assets.cpp#L20).
     
## Data Sector
### Data header
Contains magic.
Is located at `{asset count} * sizeof(asset_header)`
| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `uint32`   | Magic                   | ASCII "FILE" | 

### Data
Contains asset embedded data. 
Is located at `0xF00000`
| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `void`     | Variable asset data     | Actual data | 

### Data compression
Data compression is done with [zlib](https://zlib.net/).
