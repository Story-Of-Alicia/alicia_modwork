# Alicia PAK format

Alicia online version: `v1.318`. Reminder that this is just a guess-work. 

This pak format uses little endian byte sequence with fixed size variables(fields).



#### Header
##### format
 `int` (*4 bytes*) - **Name**: "Header magic"  **Note**: Always {0x50 0x41 0x4B 0x53} (ANSI "PAKS")

 `int` (*4 bytes*) - **Name**: "Unknown"

 `int` (*4 bytes*) - **Name**: "Unknown"

 `int` (*4 bytes*) - **Name**: "Unknown"

 `int` (*4 bytes*) - **Name**: "File byte length"

 `int` (*4 bytes*) - **Name**: "Unknown"

 `int` (*4 bytes*) - **Name**: "Unknown magic"  **Note**: Always {0x4E 0x50 0x48 0x53} (ANSI "NPHS")

 
 #### Entry
 Entries start at sector: `0x7D000` prefixed with magic "`FILSFILZ`". Each entry has size of 620 bytes. First 108 bytes are allocated for entry info, and other 512 bytes are allocated for path.
 
 ##### format

`int` &nbsp; (*4 bytes*) - **Name**: "Entry magic" 

`int`  &nbsp; (*4 bytes*) - **Name**: "Relative Resource Offset" **Note**: Is relative position in PAK. If this value is 0, it indicates that file is not encoded in PAK.

`int` &nbsp; (*4 bytes*) - **Name**: "Resource data byte length"       **Note**: Has non-zero value only when resource is encoded in PAK

`long` (*8 bytes*) - **Name**: "Resource data file byte length"  **Note**: If resource is encoded in pak, this is byte length

`long` (*8 bytes*) - **Name**: "Resource data file byte length"  **Note**: If resource is encoded in pak, this is byte length

`int` &nbsp; (*4 bytes*) - **Name**: "Unknown"                         **Note**: Seems to be always "resource data file byte length", but type is int

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`long` (*8 bytes*) - **Name**: "Unknown type"                   **Note**: Always {0x46 0x49 0x53 0x00 0x00 0x00 0x00 0x00} (ANSI "FIS")

`int` &nbsp; (*4 bytes*) - **Name**: "Unknown type value"    

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - ??

`int` &nbsp; (*4 bytes*) - **Name**: "CRC identifier"                 **Note**: Always {0x43 0x52 0x43 0x32} (ANSI "CRC2")

`int` &nbsp; (*4 bytes*) - **Name**: "CRC algorithm result?"

`long` (*8 bytes*) - **Name**: "Unknown"

`int` &nbsp; (*4 bytes*) - ??

`wstr` &nbsp; (max 516 bytes) - **Name**: "Path to file"


#### Resource
Resources start at sector: `0xF00000`

##### format
`void` (*x bytes*) - **Name**: "Variable data prefix"    **Note**: Seems to contain some specific information for resources. (this field is optional)

`void` (*x bytes*) - **Name**: "Variable resource data"  **Note**: Actual data of resource
# Raw:

```txt
Alicia online version: v1.318
This pak format uses little endian byte sequence with fixed size variables(fields).

PAK HEADER FORMAT:
 int    (4 bytes) - Name: "Header magic"  Note: Always {0x50 0x41 0x4B 0x53} (ANSI "PAKS")
 int    (4 bytes) - Name: "Unknown"
 int    (4 bytes) - Name: "Unknown"
 int    (4 bytes) - Name: "Unknown"
 int    (4 bytes) - Name: "File byte length"
 int    (4 bytes) - Name: "Unknown"
 int    (4 bytes) - Name: "Unknown magic" Note: Always {0x4E 0x50 0x48 0x53} (ANSI "NPHS")


PAK ENTRY FORMAT (104+516 bytes): 
 int  (4 bytes) - Name: "Entry magic"                     Note: Seems to indicate position of entry on stack. Seems to start on 0x28?
 int  (4 bytes) - Name: "Relative Resource Offset"        Note: Is relative position in PAK. If this value is 0, it indicates that file is not encoded in PAK.
 int  (4 bytes) - Name: "Resource data byte length"       Note: Has non-zero value only when resource is encoded in PAK
 long (8 bytes) - Name: "Resource data file byte length"  Note: If resource is encoded in pak, this is byte length
 long (8 bytes) - Name: "Resource data file byte length"  Note: If resource is encoded in pak, this is byte length
 int  (4 bytes) - Name: "Unknown"                         Note: Seems to be always "resource data file byte length", but type is int
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 long (8 bytes) - Name: "Unknown type"                   Note: Always {0x46 0x49 0x53 0x00 0x00 0x00 0x00 0x00} (ANSI "FIS")
 int  (4 bytes) - Name: "Unknown type value"    
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - ??
 int  (4 bytes) - Name: "CRC identifier"                 Note: Always {0x43 0x52 0x43 0x32} (ANSI "CRC2")
 int  (4 bytes) - Name: "CRC algorithm result?"
 long (8 bytes) - Name: "Unknown"
 int  (4 bytes) - ??
  
 wstr (max 516 bytes) - Name: "Path to file"



PAK RESOURCE FORMAT:
 void (x bytes) - Name: "Variable data prefix"    Note: Seems to contain some specific information for resources. (this field is optional)
 void (x bytes) - Name: "Variable resource data"  Note: Actual data of resource

 ```

