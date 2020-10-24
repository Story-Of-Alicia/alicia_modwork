
# Alicia PAK format

AliciaOnline version: `v1.318`. 

This PAK uses little endian byte sequence. 

#### PAK Header
Header of PAK file with size of 44 bytes.
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
Entries start at sector: `0x7D000` prefixed with ansi magic `FILSFILZ` followed by unknown 4 bytes that are skipped. Each entry has size of 620 bytes. First 108 bytes are allocated for entry info, and other 512 bytes are allocated for path.
 
###### Entry format
| Field type | Field name              | Notes      |
| ---------- | ----------------------- | -------    |
| `int`      | Entry prefix            | Always 0x0 | 
| `int`      | Entry magic             | |
| `int`      | Data offset             | Offset of data from begining of PAK. If this value is zero, it means that file is not located in PAK but file-system |
| `int`      | Data length             | Data length in bytes. Only present if previous field value is  non-zero |
| `long`     | Resource data file byte length"  **Note**: If resource is encoded in pak, this is byte length
> 
> `long` (*8 bytes*) - **Name**: "Resource data file byte length"  **Note**: If resource is encoded in pak, this is byte length
> 
> `int` &nbsp; (*4 bytes*) - **Name**: "Unknown"                         **Note**: Seems to be always "resource data file byte length", but type is int
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `long` (*8 bytes*) - **Name**: "Unknown type"                   **Note**: Always {0x46 0x49 0x53 0x00 0x00 0x00 0x00 0x00} (ANSI "FIS")
> 
> `int` &nbsp; (*4 bytes*) - **Name**: "Unknown type value"    
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `int` &nbsp; (*4 bytes*) - **Name**: "CRC identifier"                 **Note**: Always {0x43 0x52 0x43 0x32} (ANSI "CRC2")
> 
> `int` &nbsp; (*4 bytes*) - **Name**: "CRC algorithm result?"
> 
> `long` (*8 bytes*) - **Name**: "Unknown"
> 
> `int` &nbsp; (*4 bytes*) - ??
> 
> `wstr` &nbsp; (max 516 bytes) - **Name**: "Path to file"


##### Data
Data start at sector: `0xF00000`.

###### Data format

| Field type | Field name              | Notes   |
| ---------- | ----------------------- | ------- |
| `void`     | Variable data prefix    | Seems to contain some specific information for data. |
| `void`     | Variable resource data  | Actual data | 
