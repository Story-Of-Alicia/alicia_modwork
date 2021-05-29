
# Alicia PAK format

> AliciaOnline version: `v1.318`. 


Alicia PAK(*„PAKS“*) stores asset headers and asset data. PAKS has little-endian bytesequence and all text(that is loaded by game engine) has UTF-16(LE) encoding. Reverse engineered with [ProcMon](https://docs.microsoft.com/en-us/sysuint32ernals/downloads/procmon), [HxD](https://mh-nexus.de/en/hxd/), [and my bare hands](https://github.com/rgnter/alicia_modwork/tree/master/source).

[@HSReina](https://github.com/hsreina) already made tool for unpacking(available on their website), but it isn't open-source and doesn't say anything about codec of PAKS. 

#### PAKS Begin Sector `0x00000`
##### PAKS Header(*„paks header“*)
Contains information about PAKS file.
###### Header codec
> `Total size: 40 bytes`

| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `uint32`      | Header magic         | Always `{0x50 0x41 0x4B 0x53}` (ANSI "PAKS") |
| `uint32`      | Unknown 1            | |
| `uint32`      | PakLib version       | |
| `uint32`      | Locale               | |
| `uint32`      | Asset count          | |
| `uint32`      | Used assets count    | |
| `uint32`      | Deleted assets count | |
| `uint32`      | File size            | (bytes) |
| `uint32`      | Header sign          | Always `{0x4E 0x50 0x48 0x53}` (ASCII "NPHS" - NPak(Ntreev PAK Header Structure?)) |


#### PAKS Asset Header Sector `0x7D000`
##### Asset Array Header
Asset Array Header.
###### Asset Array Header codec
> `Total size: 12 bytes`

| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `uint64`   | AHA magic               | Always ANSI "FILSFILZ" | 
| `uint32`   | Asset count             | Total asset count |


##### PAKS Asset Header(*„asset header“*)
Asset header contains information about **asset** and it's *data*. If it's data is encoded in PAK, we reffer to it's data as „asset data“. Otherwise, we reffer to it as „data“ or „extern data“.


###### Asset Header codec
> `Total size: 620 bytes`

| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `uint32`      | Asset prefix            | Always 0x0 | 
| `uint32`      | Asset magic             | values between 0x30 and 0x40 (??) |
| `uint32`      | Data offset             | Offset of data from beginning of PAK file. Only present if file is packed |
| `uint32`      | Data length             | Data length in bytes. Only present if file is packed |
| `uint32`      | Uncompressed length0    | Uncompressed length of data |
| `uint32`      | Is compressed           | uint32 that is used as boolean to indicate if data are compressed |
| `uint32`      | Uncompressed length1    | -- |
| `uint32`      | Unknown0                | Always 0x0 |
| `uint32`      | Uncompressed length2    | -- |
| `uint32`      | Unknown1                | |
| `uint32`      | Unknown2                | |
| `uint32`      | Unknown3                | |
| `uint32`      | Unknown4                | |
| `uint32`      | Unknown5                | |
| `uint32`      | Is Deleted              | Marks asset as deleted, has info only about offset and length|
| `uint32`      | Entry offset            | Offset of this entry from beginning of PAK file|
| `uint32`      | Is packed               | uint32 that is used as boolean to indicate if file is embeded in pak |
| `uint64`      | Unknown type            | Always ANSI "FIS\0"|
| `uint64`      | Unknown value           | |
| `uint32`      | CRC Result              | CRC checksum of Data |
| `uint32`      | Unknown9                | |
| `uint32`      | CRC Identification      | Always ANSI "CRC2"|
| `uint32`      | Unknown 7               | |
| `uint64`      | Unknown 8               | |
| `wstr`        | Path (512bytes total)   | |

#### PAKS Asset Data Sector `0xF00000`
##### PAKS Asset Data(*„asset data“*)
Nothing special about this part.
###### Asset Data codec

| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `void`     | Variable asset data     | Actual data | 

### Data compression
Data compression is done with [zlib](https://zlib.net/).
