
# Alicia PAK format

> AliciaOnline version: `v1.318`. 

Reverse engineered with [ProcMon](https://docs.microsoft.com/en-us/sysinternals/downloads/procmon), [HxD](https://mh-nexus.de/en/hxd/), [and my bare hands](https://github.com/rgnter/alicia_modwork/tree/master/source).

[@HSReina](https://github.com/hsreina) already made tool for unpacking(available on their website), but it isn't open-source. 

This PAK uses little endian byte sequence. 

#### PAK Header
Header of PAK file with size of 44 bytes.
###### Header format
| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `int`      | Header magic  | Always `{0x50 0x41 0x4B 0x53}` (ANSI "PAKS") |
| `int`      | Unknown       | |
| `int`      | Unknown       | |
| `int`      | Unknown       | |
| `int`      | Length        | File length in bytes |
| `int`      | Unknown       | |
| `int`      | Unknown magic | Always `{0x4E 0x50 0x48 0x53}` (ANSI "NPHS") |
| `int`      | Unknown       | Seems to always be 0x0 |


#### PAK Assets
##### Entries
Assets start at sector: `0x7D000` prefixed with ansi magic `FILSFILZ` followed by unknown 4 bytes that are skipped. Each entry has size of 620 bytes. First 108 bytes are allocated for entry info, and other 512 bytes are allocated for path.
 
###### Entry format
| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `int`      | Asset prefix            | Always 0x0 | 
| `int`      | Asset magic             | |
| `int`      | Data offset             | Offset of data from beginning of PAK file. Only present if file is packed |
| `int`      | Data length             | Data length in bytes. Only present if file is packed |
| `int`      | Uncompressed length0    | Uncompressed length of data |
| `int`      | Is compressed           | Int that is used as boolean to indicate if data are compressed |
| `int`      | Uncompressed length1    | -- |
| `int`      | Unknown0                | Always 0x0 |
| `int`      | Uncompressed length2    | -- |
| `int`      | Unknown1                | |
| `int`      | Unknown2                | |
| `int`      | Unknown3                | |
| `int`      | Unknown4                | |
| `int`      | Unknown5                | |
| `int`      | Is Deleted              | Marks asset as deleted, has info only about offset and length|
| `int`      | Entry offset            | Offset of this entry from beginning of PAK file|
| `int`      | Is packed               | Int that is used as boolean to indicate if file is embeded in pak |
| `long`     | Unknown type            | Always ANSI "FIS\0"|
| `long`     | Unknown value           | |
| `int`      | CRC Result              | CRC checksum of Data |
| `int`      | Unknown9                | |
| `int`      | CRC Identification      | Always ANSI "CRC2"|
| `int`      | Unknown 7               | |
| `long`     | Unknown 8               | |
| `wstr`     | Path (512bytes)         | |

##### Data
Data start at sector: `0xF00000`. 

###### Data format

| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `void`     | Asset-specific data     | Seems to contain some specific information for asset |
| `void`     | Variable asset data     | Actual data | 

### Data compression
Data compression is done with [zlib](https://zlib.net/).
