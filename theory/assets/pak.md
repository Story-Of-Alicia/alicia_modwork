
# Alicia PAK format
> AliciaOnline version: `v1.318`. 

Reverse engineered with [ProcMon](https://docs.microsoft.com/en-us/sysuint32ernals/downloads/procmon), [HxD](https://mh-nexus.de/en/hxd/), [and my bare hands](https://github.com/rgnter/alicia_modwork/tree/master/source).

## Trivia
PAKS is divided into three sections: File Header, Content Header, Asset Sector, Data Sector. <br>
PAKS codec uses little-endian byte sequence. <br>
[@HSReina](https://github.com/hsreina) made tool for unpacking(available on their website), but it isn't open-source and doesn't say anything about codec of PAKS. 


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
| `uint32`      | Unknown 1            ||
| `uint32`      | Header sign          | Always `{0x4E 0x50 0x48 0x53}` (ASCII "NPHS") |

## Content Header 
This header contains information about assets. 
Is located at `0x7D000`. 

`Total size: 12 bytes`
| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `uint32`   | Magic                   | Always ANSI "FILS"|
| `uint32`   | Magic                   | Always ANSI "FILZ"|
| `uint32`   | Asset count             | Total asset count |




## Asset Header
Contains tightly packed array of Assets (of which size is described by Content Header). 
`Total size: 108 bytes (+ path 620 bytes)`
| Field type    | Field name                   | Notes      |
| ----------    | -----------------------      | -------    |
| `uint32`      | Asset prefix              | Always 0x0                        | 
| `uint32`      | Asset magic               | Always a value between 0x30 and 0x40 (??) |
| `uint32`      | Embedded data offset      | Offset of embedded data.  |
| `uint32`      | Embedded data length      | Length of embedded data.  |
| `uint32`      | Data uncompressed length  | Uncompressed length of data |
| `uint32`      | Are data compressed?      | Whether this data are compressed or not. |
| `uint32`      | Data uncompressed length  | -- |
| `uint32`      | Unknown0                  | Always 0x0 |
| `uint32`      | Data uncompressed length  | -- |
| `uint32`      | Unknown1                  |    |
| `uint32`      | Unknown2                  |    |
| `uint32`      | Unknown3                  |    |
| `uint32`      | Unknown4                  |    |
| `uint32`      | Unknown5                  |    |
| `uint32`      | Are data deleted          | Whether this data are deleted or not. |
| `uint32`      | Asset offset              | Contains information about offset of this header. |
| `uint32`      | Is embedded               | Whether this data are embedded or not. |
| `uint64`      | Unknown type              | Always ANSI "FIS\0" |
| `uint64`      | Unknown value             | |
| `uint32`      | CRC Result                | CRC32 checksum of data |
| `uint32`      | Unknown9                  | |
| `uint32`      | CRC Identification        | Always ANSI "CRC2\0"|
| `uint32`      | Unknown 7                 | |
| `uint64`      | Unknown 8                 | |
| `wstr`        | Path (512bytes total)     | |
     
     
### Data Sector
Contains asset embedded data. 
Is located at `0xF00000`
| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `void`     | Variable asset data     | Actual data | 

### Data compression
Data compression is done with [zlib](https://zlib.net/).
