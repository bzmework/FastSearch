/*
	模块：NTFS结构
	作者：denglf
	日期：2019-04-24
	说明：
	参考：
	https://sourceforge.net/projects/linux-ntfs/
	https://sourceforge.net/projects/ntfs-3g/
*/

#ifndef __NTFS_H__
#define __NTFS_H__

#include <windows.h>

/*
	Everything on an NTFS volume is a file. 
	There are two categories: Metadata and Normal. 
	The Metadata files contain information about the volume and the Normal files contain your data.
	NTFS卷上的一切都是文件。
	分成两类: 元数据文件和普通文件。
	元数据文件包含关于卷的信息，而普通文件则包含您的数据。

	Below is a table of files found on a Win2K volume (Key).
	下面是在Win2K卷(Key)上的文件表结构：
	Inode 	Filename	OS 	Description
	0 		$MFT 	  		Master File Table - An index of every file
	1 		$MFTMirr 	  	A backup copy of the first 4 records of the MFT
	2 		$LogFile 	  	Transactional logging file
	3 		$Volume 	  	Serial number, creation time, dirty flag
	4 		$AttrDef 	  	Attribute definitions
	5 		.(dot) 	  		Root directory of the disk
	6 		$Bitmap 	  	Contains volume's cluster map (in-use vs. free)
	7 		$Boot 	  		Boot record of the volume
	8 		$BadClus 	  	Lists bad clusters on the volume
	9 		$Quota 		NT 	Quota information
	9 		$Secure 	2K 	Security descriptors used by the volume
	10 		$UpCase 	  	Table of uppercase characters used for collating
	11 		$Extend 	2K 	A directory: $ObjId, $Quota, $Reparse, $UsnJrnl

	12-15 	<Unused> 	  	Marked as in use but empty
	16-23 	<Unused> 	  	Marked as unused

	Any 	$ObjId		2K 	Unique Ids given to every file
	Any 	$Quota 		2K 	Quota information
	Any 	$Reparse 	2K 	Reparse point information
	Any 	$UsnJrnl 	2K 	Journalling of Encryption

	> 24 	A_File 	  		An ordinary file
	> 24 	A_Dir 	  		An ordinary directory
	... 	... 	  		...

	Note:
	For some reason $ObjId, $Quota, $Reparse and $UsnJrnl don't have inode numbers below 24, like the rest of the Metadata files.
	Also, the sequence number for each of the system files is always equal to their mft record number and it is never modified.
	注意：
	由于某种原因，$ObjId、$Quota、$Reparse和$UsnJrnl不像其他元数据文件一样，inode编号低于24。
	此外，每个系统文件的序列号总是等于它们的mft记录号，而且它从不修改。
*/

#pragma pack(push, 1)
/* 
	7
	$Boot
	Boot record of the volume

	$Boot is the system file that allows the system to boot. 
	This metadata file points at the boot sector of the volume. 
	It contains information about the size of the volume, clusters and the MFT. 
	It is the only file that cannot be relocated.
	$Boot是允许系统启动的系统文件。
	这个元数据文件指向卷的引导扇区。
	它包含关于卷大小、簇和MFT的信息。
	它是惟一不能重新定位的文件。

*/
typedef struct
{
	UCHAR		Jump[3];				// 0x0000 	3 	Jump to the boot loader routine 跳转到引导加载程序
	UCHAR		Format[8];				// 0x0003 	8 	System Id: "NTFS    "
	USHORT		BytesPerSector;			// 0x000B 	2 	Bytes per sector 每个扇区字节数
	UCHAR		SectorsPerCluster;		// 0x000D 	1 	Sectors per cluster 每簇扇区数 
	USHORT		BootSectors;			// 引导扇区数
	UCHAR		Mbz1;					// 保留0 
	USHORT		Mbz2;					// 保留0 
	USHORT		Reserved1;				// 保留0 
	UCHAR		MediaType;				// 0x0015 	1 	Media descriptor: A media descriptor of 0xF8 means a hard disk. 介质类型: 0xF8的媒体描述符表示硬盘。
	USHORT		Mbz3;					// 总为0 
	USHORT		SectorsPerTrack;		// 0x0018 	2 	Sectors per track 每磁道扇区数
	USHORT		NumberOfHeads;			// 0x001A 	2 	Number of heads 磁头数 
	ULONG		PartitionOffset;		// 该分区的偏移（即该分区前的隐含扇区数 一般为磁道扇区数0x3f 63） 
	ULONG		Rserved2;
	ULONG		Rserved3;				// 0x0024 	4 	Usually 80 00 80 00. A value of 80 00 00 00 has been seen on a USB thumb drive which is formatted with NTFS under Windows XP. Note this is removable media and is not partitioned, the drive as a whole is NTFS formatted. 在Windows XP下使用NTFS格式化的USB u盘上显示值为8000 00 00。注意，这是可移动媒体，没有分区，驱动器作为一个整体是NTFS格式。
	ULONGLONG	TotalSectors;			// 0x0028 	8 	Number of sectors in the volume. 该分区总扇区数
	ULONGLONG	MftStartLcn;			// 0x0030 	8 	LCN of VCN 0 of the $MFT. MFT表($MFT)的起始簇号LCN
	ULONGLONG	Mft2StartLcn;			// 0x0038 	8 	LCN of VCN 0 of the $MFTMirr. MFT备份表($MFTMirr)的起始簇号LCN 			
	ULONG		ClustersPerFileRecord;  // 0x0040 	4 	Clusters per MFT Record: This can be negative, which means that the size of the MFT record is smaller than a cluster. In this case the size of the MFT record in bytes is equal to 2^(-1 * Clusters per MFT record). So for example if Clusters per MFT Record is 0xF6 (-10 in decimal), the MFT record size is 2^(-1 * -10) = 2^10 = 1024 bytes. 每个MFT记录包含几个簇：记录的字节不一定为ClustersPerFileRecord*SectorsPerCluster*BytesPerSector。这可能是负的，这意味着MFT记录的大小小于簇。在本例中，以字节为单位的MFT记录的大小等于2^(-1 *Clusters per MFT记录)。因此，例如，如果每个MFT记录的集群是0xF6(十进制为-10)，那么MFT记录大小是2^(-1 * -10)= 2^10 = 1024字节。
	ULONG		ClustersPerIndexBlock;	// 0x0044 	4 	Clusters per Index Record: This can be negative, which means that the size of the Index record is smaller than a cluster. In this case the size of the Index record in bytes is equal to 2^(-1 * Clusters per Index record). So for example if Clusters per MFT Record is 0xF6 (-10 in decimal), the MFT record size is 2^(-1 * -10) = 2^10 = 1024 bytes. 每个索引块的簇数：这可能是负的，这意味着Index记录的大小小于簇。记录的字节不一定为：ClustersPerFileRecord*SectorsPerCluster*BytesPerSector。在本例中，以字节为单位的Index记录的大小等于2^(-1 *Clusters per Index记录)。因此，例如，如果每个MFT记录的集群是0xF6(十进制为-10)，那么MFT记录大小是2^(-1 * -10)= 2^10 = 1024字节。
	ULONGLONG	VolumeSerialNumber;		// 0x0048 	8 	Volume serial number 卷序列号
	UCHAR		Code[0x1AE];
	USHORT		BootSignature;
} BOOT_BLOCK, *PBOOT_BLOCK;				// 0x0200 	  	Windows NT Loader
#pragma pack(pop)


/*
	0 	
	$MFT 	  	
	Master File Table - An index of every file

	The MFT is a set of FILE records.
	Each file of the volume is completely described by one or more of these FILE Records.
	File Records are equivalent to inodes in Unix terminology.
	The first FILE Record that describes a given file is called the Base FILE record and the others are called Extension FILE Records.
	MFT是一组文件记录。
	卷中的每个文件都由一个或多个这样的文件记录完整地描述。
	在Unix术语中，文件记录相当于inode。
	描述给定文件的第一个文件记录称为基本文件记录，其他文件记录称为扩展文件记录。

	A FILE Record is built up from a header, several variable length attributes and an end marker (simply 0xFFFFFFFF).
	文件记录由一个头、几个可变长度属性和一个结束标记(简单地说就是0xFFFFFFFF)组成。

	The file record Layout is as follows
	文件记录布局如下：

		Record Header
		Attribute
		Attribute
		...
		End Marker (0xFFFFFFFF)


	The file record structure is as follows
	文件记录结构如下：

	Offset 	Size 	OS 	Description
	0x00 	4 	  		Magic number 'FILE'
	0x04 	2 	  		Offset to the Update Sequence
	0x06 	2 	  		Size in words of Update Sequence (S)
	0x08 	8 	  		$LogFile Sequence Number (LSN)
	0x10 	2 	  		Sequence number
	0x12 	2 	  		Hard link count
	0x14 	2 	  		Offset to the first Attribute
	0x16 	2 	  		Flags
	0x18 	4 	  		Real size of the FILE record
	0x1C 	4 	  		Allocated size of the FILE record
	0x20 	8 	  		File reference to the base FILE record
	0x28 	2 	  		Next Attribute Id
	0x2A 	2 		XP 	Align to 4 byte boundary
	0x2C 	4 		XP 	Number of this MFT Record
			2 	  		Update Sequence Number (a)
			2S-2 	  	Update Sequence Array (a)

	(a) The offset to these two fields depends on your operating system. 这两个字段的偏移量取决于您的操作系统。
*/


// --- MFT structure ---
// NTFS_RECORD_HEADER
//  |
//  |-- ATTRIBUTE_HDR (of type ATTRIBUTE_TYPE)
//      |
//      |-- RESIDENT_ATTRIBUTE
//      |   |
//      |   |-- STANDARD_INFORMATION
//      |   |-- FILENAME_ATTRIBUTE
//      |   |-- AttributeIndexAllocation
//      |       |-- INDEX_RECORD
//      |           |-- RUN_LIST
//      |               |-- ATTRIBUTE_LIST
//      |                   |-- INDEX_HEADER, *PINDEX_HEADER;
//      |                   |-- INDEX_RECORD, *PINDEX_RECORD;
//      |
//      |-- NONRESIDENT_ATTRIBUTE
//          |   
//          |-- STANDARD_INFORMATION
//          |-- FILENAME_ATTRIBUTE
//          |-- INDEX_ROOT, *PINDEX_ROOT;
//              |-- INDEX_HEADER, *PINDEX_HEADER;
//                  |-- INDEX_RECORD, *PINDEX_RECORD;
//

/*
	NTFS_RECORD_HEADER
	NTFS记录头
*/
typedef struct
{
	// 0x00 	4 	  	Magic number 'FILE'
	// The type of NTFS record.When the value of Type is considered as a sequence of four one - byte characters, 
	// it normally spells an acronym for the type.Defined values include:
	// NTFS记录的类型。当Type的值被认为是由四个单字节字符组成的序列时，它通常为该类型拼写一个缩略词。定义值包括:
	// 'FILE' 'INDX' 'BAAD' 'HOLE' *CHKD'
	ULONG Type;						

	// 0x04 	2 	  	Offset to the Update Sequence
	// The offset, in bytes, from the start of the structure to the Update Sequence Array(USA).
	// 更新序列数组(USA)偏移，校验值地址：从结构开始到更新序列数组的偏移量，以字节为单位。
	USHORT UsaOffset;				
	
	// 0x06 	2 	  	Size in words of Update Sequence (S)
	// The number of values in the Update Sequence Array.
	// 更新序列数组(USA)中值的数目。1+n：1为校验值个数，n为待替换值个数。
	USHORT UsaCount;				

	// 0x08 	8 	  	$LogFile Sequence Number (LSN)
	// The Update Sequence Number(USN) of the NTFS record.
	// This is changed every time the record is modified. 
	// NTFS记录的更新序列号(USN)。
	// 每次修改记录时都会更改此值。
	USN Usn;						
} NTFS_RECORD_HEADER, * PNTFS_RECORD_HEADER;

/*
	FILE_RECORD_HEADER
	文件记录头

	An record in the MFT consists of a FILE_RECORD_HEADER followed by a sequence of attributes.
	MFT记录由FILE_RECORD_HEADER和一系列属性组成。
*/
typedef struct
{
	// An NTFS_RECORD_HEADER structure with a Type of 'FILE'
	// 带有'FILE'类型的NTFS_RECORD_HEADER结构。
	// Ntfs.Type总是'ELIF'。 
	NTFS_RECORD_HEADER Ntfs;

	// 0x10 	2 	  	Sequence number
	// Number of times this mft record has been reused.
	// N.B.The increment(skipping zero) is done when the file is deleted.
	// N.B.If this is set to zero it is left as zero.
	// 此MFT记录被重用的次数
	// 注意，增量(跳过零)是在删除文件时完成的。
	// 注意，如果设置为零，它就会被保留为零。
	USHORT SequenceNumber;			

	// 0x12 	2 	  	Hard link count
	// The number of directory links to the MFT record.
	// Number of hard links, i.e.the number of directory entries referencing this record.
	// N.B.Only used in mft base records.
	// 指向MFT记录的目录链接的数量。
	// 硬链接的数量，即引用此记录的目录条目的数量。
	// 注意, 仅用于mft基础记录。
	USHORT LinkCount;				

	// 0x14 	2 	  	Offset to the first Attribute
	// The offset, in bytes, from the start of the structure to the first attribute of the MFT entry.
	// 第一个属性的偏移：从结构开始到MFT条目的第一个属性的偏移量，单位为字节。
	USHORT AttributesOffset;		

	// 0x16 	2 	  	Flags
	// A bit array of flags specifying properties of the MFT entry. The values defined include :
	// InUse 0x0001  The MFT entry is in use
	// Directory 0x0002  The MFT entry represents a directory
	// 指定MFT条目属性的位标志数组。定义的值包括:
	// 0x0001	InUse		Record is in use MFT记录正在使用
	// 0x0002	Directory	Record is a directory	MFT记录表示一个目录	
	// 0x0004 	Don't know
	// 0x0008 	Don't know
	USHORT Flags;					

	// 0x18 	4 	  	Real size of the FILE record 
	// The number of bytes used by the MFT record.
	// The Real Size is a count of how many bytes of the Record are actually used. 
	// N.B.The Real Size will be padded to an 8 byte boundary.
	// 记录MFT记录(头和属性)的总长度(字节数)，即记录在磁盘上实际占用的字节空间。
	// 实际大小是记录实际使用的字节数。
	// 注意，实际大小将填充为8字节的边界。
	ULONG BytesInUse;				
	
	// 0x1C 	4 	  	Allocated size of the FILE record
	// The number of bytes allocated for the MFT record.
	// The Allocated Size is how much space the Record takes up on disk. 
	// This should be a multiple of the cluster size and should probably be equal to the size of an MFT File Record. 
	// 分配给MFT记录的字节数。
	// 分配的大小是记录占用磁盘上的空间。
	// 这应该是簇大小的倍数，并且可能等于MFT文件记录的大小。
	ULONG BytesAllocated;			
	
	// 0x20 	8 	  	File reference to the base FILE record
	// If the MFT record contains attributes that overflowed a base MFT record, this member contains the file reference number of the base record; otherwise, it contains zero.
	// This is zero for Base MFT Records.
	// When it is not zero it is a MFT Reference pointing to the Base MFT Record to which this Record belongs.
	// The Base Record contains the information about the Extension Record.
	// This information is stored in an ATTRIBUTE_LIST attribute.
	// 如果MFT记录包含溢出基本MFT条记录的属性，则此成员包含基本记录的文件引用号; 否则，它包含0。
	// 对于基本MFT记录，这是零。
	// 当它不为零时，它是指向该记录所属的基本MFT记录的MFT引用。
	// 基本记录包含关于扩展记录的信息。
	// 此信息存储在ATTRIBUTE_LIST属性中。
	ULARGE_INTEGER BaseFileRecord;	

	// 0x28 	2 	  	Next Attribute Id
	// The number that will be assigned to the next attribute added to the MFT record
	// 将要分配给添加到MFT记录的下一个属性的号码，即下一属性ID
	USHORT NextAttributeNumber;		

	// 0x2A 	2 		XP: Align to 4 byte boundary
	USHORT Alignment;	

	// 0x2C 	4 		XP: Number of this MFT Record
	ULONG NumbersOfMFTRecord;		
} FILE_RECORD_HEADER, *PFILE_RECORD_HEADER;

/* 
	4
	$AttrDef
	Attribute definitions

	Each MFT FILE Record is built up from Attributes. The list of possible Attributes is defined in $AttrDef. 
	每个MFT文件记录都是由属性构建的。可能的属性列表在$AttrDef中定义。

	Note: 
	$PROPERTY_SET, $SYMBOLIC_LINK and $VOLUME_VERSION existed in NTFS v1.2, but weren't used. They no longer exist in NTFS v3.0 (that used by Win2K).
	Each MFT FILE Record is built up from Attributes. The list of possible Attributes is defined in $AttrDef.
	Each MFT record has a Standard Header, followed by a list of attributes (in order of ascending Attribute Id) and an end marker. The end marker is just four bytes: 0xFFFFFFFF.
	注意：
	NTFS v1.2中存在$PROPERTY_SET、$SYMBOLIC_LINK和$VOLUME_VERSION，但没有使用它们。它们不再存在于NTFS v3.0 (Win2K使用的版本)中。
	每个MFT文件记录都是由属性构建的。可能的属性列表在$AttrDef中定义。
	每个MFT记录都有一个标准的头，然后是一个属性列表(按升序属性Id排列)和一个结束标记。结束标记只有四个字节: 0xFFFFFFFF。

	Type 	OS 	Name
	0x10 	  	$STANDARD_INFORMATION
	0x20 	  	$ATTRIBUTE_LIST
	0x30 	  	$FILE_NAME
	0x40 	NT 	$VOLUME_VERSION
	0x40 	2K 	$OBJECT_ID
	0x50 	  	$SECURITY_DESCRIPTOR
	0x60 	  	$VOLUME_NAME
	0x70 	  	$VOLUME_INFORMATION
	0x80 	  	$DATA
	0x90 	  	$INDEX_ROOT
	0xA0 	  	$INDEX_ALLOCATION
	0xB0 	  	$BITMAP
	0xC0 	NT 	$SYMBOLIC_LINK
	0xC0 	2K 	$REPARSE_POINT
	0xD0 	  	$EA_INFORMATION
	0xE0 	  	$EA
	0xF0 	NT 	$PROPERTY_SET
	0x100 	2K 	$LOGGED_UTILITY_STREAM

*/
typedef enum 
{
	//$STANDARD_INFORMATION
	//此属性为常驻属性(Resident Attributes)。存储诸如：
	//文件只读、存档等属性； 
	//时间戳，如文件创建时间, 最后一次修改时间； 
	//多少目录指向该文件(硬链接计数hard link count)。
	StandardInformation = 0x10,	

	//$ATTRIBUTE_LIST
	//由于属性值可能会增长，因此此属性可能是非常驻属性(Nonresident Attributes)。
	//当一个文件要求多个MFT文件记录时, 会有该属性。
	AttributeList		= 0x20,	

	//$FILE_NAME
	//此属性为常驻属性(Resident Attributes)。文件名属性可以有多个： 
	//1.长文件名自动为其短文件名(以便MS-DOS和16位程序访问) 
	//2.当该文件存在硬链接时 
	FileName			= 0x30, 

	//NT: $VOLUME_VERSION		
	//2K: $OBJECT_ID
	//此属性为常驻属性(Resident Attributes)。 
	//此属性代表一个文件或目录的64字节标识符，其中低16字节对于该卷来说是唯一的链接-跟踪服务将对象ID分配给外壳快捷方式和OLE链接源文件。 
	//NTFS提供了相应的API，因为文件和目录可以通过其对象ID，而不是通过其文件名打开。 
	ObjectId			= 0x40, 

	//$SECURITY_DESCRIPTOR
	//此属性仅出现于$Secure_Descriptor元数据文件中。
	//为与NTFS以前版本保持向后兼容, 所有具有相同安全描述符的文件或目录共享同样的安全描述,
	//以前版本的NTFS将私有的安全描述符信息与每个文件和目录存储在一起。
	SecurityDescripter	= 0x50,	 

	//$VOLUME_NAME
	//保存了该卷的名称等信息
	//此属性仅出现于$VolumeName元数据文件中 
	VolumeName			= 0x60, 

	//$VOLUME_INFORMATION
	//保存了该卷的版本等信息
	//此属性仅出现于$VolumeInformation元数据文件中 
	VolumeInformation	= 0x70, 

	//$DATA
	//由于属性值可能会增长，因此此属性可能是非常驻属性(Nonresident Attributes)。
	//此属性记录文件的内容，一个文件仅有一个未命名的数据属性，但可有额外多个命名数据属性， 
	//即一个文件可以有多个数据流，目录没有默认的数据属性，但可有多个可选的命名数据属性。
	Data				= 0x80, 

	//以下三个用于实现大目录的文件名分配和位图索引 
	IndexRoot			= 0x90, // $INDEX_ROOT
	IndexAllocation		= 0xA0, // $INDEX_ALLOCATION
	Bitmap				= 0xB0, // $BITMAP

	//NT: $SYMBOLIC_LINK		
	//2K: $REPARSE_POINT
	//存储了一个文件的重解析点数据，NTFS的交接(junction)和挂载点包含此属性。
	ReparsePoint		= 0xC0, 
	
	//以下两个为扩展属性，现已不再被主动使用，之所以提供是为与OS/2程序保持向后兼容 
	EAInformation		= 0xD0,	// $EA_INFORMATION
	EA					= 0xE0,	// $EA

	PropertySet			= 0xF0, // NT: $PROPERTY_SET
	LoggedUtilityStream = 0x100 // 2K: $LOGGED_UTILITY_STREAM
} ATTRIBUTE_TYPE, *PATTRIBUTE_TYPE;

/* 
	ATTRIBUTE Structure
	属性结构
*/
typedef struct 
{
	// The type of the attribute.
	// 属性的类型。
	ATTRIBUTE_TYPE AttributeType;

	// The size, in bytes, of the resident part of the attribute.
	// 本属性长度（包含属性值）: 属性的常驻部分的大小，以字节为单位。
	ULONG Length;

	// Specifies, when true, that the attribute value is nonresident.
	// 指定，为真时，属性为非常驻。
	BOOLEAN Nonresident;

	// The size, in characters, of the name (if any) of the attribute
	// 属性名的名称长度: 属性名称(如果有的话)的字符数量
	UCHAR NameLength;

	// The offset, in bytes, from the start of the structure to the attribute name.
	// The attribute name is stored as a Unicode string.
	// 属性名偏移: 从结构开始到属性名的偏移量，以字节为单位。属性名存储为Unicode字符串。
	USHORT NameOffset;

	// A bit array of flags specifying properties of the attribute.The values defined include :
	// 0x0001 = Compressed  The attribute is compressed
	// 属性标志：属性的指定属性的位标志数组。定义的值包括:
	// 0x0001 = Compressed  属性被压缩
	USHORT Flags; 

	// A numeric identifier for the instance of the attribute.
	// 属性标识：属性实例的数字标识符。
	USHORT AttributeNumber;
} ATTRIBUTE, *PATTRIBUTE;

/* 
	Resident ATTRIBUTE 
	常驻属性
*/
typedef struct 
{
	// An ATTRIBUTE structure containing members common to resident and nonresident attributes.
	// 属性类型，一个属性结构，包含常驻属性和非常驻属性共有的成员。
	ATTRIBUTE Attribute;

	// The size, in bytes, of the attribute value.
	// 属性值的大小, 以字节为单位。
	ULONG ValueLength;

	// The offset, in bytes, from the start of the structure to the attribute value
	// 属性值偏移, 从结构开始到属性值的偏移量，以字节为单位
	USHORT ValueOffset;

	// A bit array of flags specifying properties of the attribute.The values defined include:
	// 0x0001 = Indexed  The attribute is indexed
	// 索引标志, 属性的指定属性的位标志数组。定义的值包括:
	// 0x0001 = Indexed 属性被索引
	USHORT Flags; 
} RESIDENT_ATTRIBUTE, * PRESIDENT_ATTRIBUTE;

/* 
	nonresident ATTRIBUTE 
	非常驻属性
*/
typedef struct 
{
	// An ATTRIBUTE structure containing members common to resident and nonresident attributes.
	// 一个属性结构，包含常驻属性和非常驻属性共有的成员。
	ATTRIBUTE Attribute;

	// The lowest valid Virtual Cluster Number (VCN) of this portion of the attribute value.
	// Unless the attribute value is very fragmented(to the extent that an attribute list is needed to describe it), 
	// there is only one portion of the attribute value, and the value of LowVcn is zero.
	// 属性值的这一部分的最低有效虚拟簇号(VCN)。即开始虚拟簇号。
	// 除非属性值非常分散(到需要一个属性列表来描述它的程度)，否则属性值只有一部分，而且LowVcn的值为零。
	ULONGLONG LowVcn;

	// The highest valid VCN of this portion of the attribute value.
	// 属性值的这一部分的最高有效VCN。即虚拟簇数量。
	ULONGLONG HighVcn;

	// The offset, in bytes, from the start of the structure to the run array that contains the
	// mappings between VCNs and Logical Cluster Numbers(LCNs)
	// 从结构开始到包含VCNs和逻辑簇号(LCNs)之间映射的运行数组的偏移量，以字节为单位。
	USHORT RunArrayOffset;

	// The compression unit for the attribute expressed as the logarithm to the base two of
	// the number of clusters in a compression unit.If CompressionUnit is zero, the attribute is not compressed.
	// 属性的压缩单元，表示为压缩单元中簇数量的以2为底的对数。
	// 如果CompressionUnit为零，则属性未被压缩。
	UCHAR CompressionUnit;

	// 对齐或保留
	UCHAR AligmentOrReserved[5];

	// The size, in bytes, of disk space allocated to hold the attribute value.
	// 分配给保存属性值的磁盘空间的大小，以字节为单位。
	ULONGLONG AllocatedSize;

	// The size, in bytes, of the attribute value.
	// This may be larger than the AllocatedSize if the attribute value is compressed or sparse.
	// 属性值的大小, 以字节为单位。
	// 如果属性值被压缩或稀疏，则该值可能大于已分配的大小。
	ULONGLONG DataSize;

	// The size, in bytes, of the initialized portion of the attribute value.
	// 属性值的初始化部分的大小, 以字节为单位。
	ULONGLONG InitializedSize;

	// The size, in bytes, of the attribute value after compression.
	// This member is only present when the attribute is compressed.
	// 压缩后属性值的大小，以字节为单位。
	// 此成员仅在属性被压缩时才出现。
	ULONGLONG CompressedSize; 
} NONRESIDENT_ATTRIBUTE, *PNONRESIDENT_ATTRIBUTE;

/*
	VolumeName - just a Unicode String
	Data = just data
	SecurityDescriptor - rarely found
	Bitmap - array of bits, which indicate the use of entries
*/

/* 
	0x10
	$STANDARD_INFORMATION
	The standard information attribute is always resident.
	标准信息属性

	In old version of NTFS this Attribute contained little more than the DOS File Permissions and the file times.
	Windows 2000 introduced four new fields which are used to reference Quota, Security, File Size and Logging information.
	As defined in $AttrDef, this attribute has a minimum size of 48 bytes and a maximum of 72 bytes.
	在旧版本的NTFS中，这个属性只包含DOS文件权限和文件时间。
	Windows 2000引入了四个新的字段，用于引用配额、安全性、文件大小和日志信息。
	正如在$AttrDef中定义的，此属性的最小大小为48字节，最大大小为72字节。

*/
typedef struct
{
	// 0x00 	8 	  	C Time - File Creation
	// The time when the file was created in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601)
	// 以标准时间格式创建文件的时间(即自1601年1月1日以来100纳秒间隔的数量)
	FILETIME CreationTime;	

	// 0x08 	8 	  	A Time - File Altered
	// The time when the file attributes were last changed in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// 文件属性最后一次以标准时间格式更改的时间(即自1601年1月1日以来100纳秒间隔的数量)。
	FILETIME ChangeTime;					

	// 0x10 	8 	  	M Time - MFT Changed
	// The time when the file was last written in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601)
	// 文件最后一次以标准时间格式写入的时间(即自1601年1月1日以来的100纳秒间隔)
	FILETIME LastWriteTime;					

	// 0x18 	8 	  	R Time - File Read
	// The time when the file was last accessed in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601)
	// 最后一次以标准时间格式访问文件的时间(即自1601年1月1日以来的100纳秒间隔)
	FILETIME LastAccessTime;				

	// 0x20 	4 	  	DOS File Permissions
	// FILE_ATTRIBUTES_* like in windows.h
	// The attributes of the file.Defined attributes include:
	// 文件的属性。定义的属性包括:
	// 0x0001	FILE_ATTRIBUTE_READONLY			Read-Only		只读
	// 0x0002	FILE_ATTRIBUTE_HIDDEN			Hidden			隐藏
	// 0x0004 	FILE_ATTRIBUTE_SYSTEM			System			系统
	// 0x0010	FILE_ATTRIBUTE_DIRECTORY		Directory		目录
	// 0x0020	FILE_ATTRIBUTE_ARCHIVE			Archive			存档
	// 0x0040	FILE_ATTRIBUTE_DEVICE           Device			设备
	// 0x0080	FILE_ATTRIBUTE_NORMAL			Normal			正常
	// 0x0100	FILE_ATTRIBUTE_TEMPORARY		Temporary		临时
	// 0x0200	FILE_ATTRIBUTE_SPARSE_FILE		SparseFile		稀疏
	// 0x0400	FILE_ATTRIBUTE_REPARSE_POINT	ReparsePoint	解析点 
	// 0x0800	FILE_ATTRIBUTE_COMPRESSED		Compressed		压缩
	// 0x1000	FILE_ATTRIBUTE_OFFLINE			Offline			离线
	// 0x2000	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED NotContentIndexed	索引
	// 0x4000	FILE_ATTRIBUTE_ENCRYPTED		Encrypted		加密
	ULONG FileAttributes;

	// 0x24 	4 	  	Maximum Number of Versions
	// Maximum allowed versions for file. Zero means that version numbering is disabled. 
	// 文件的最大允许版本。0表示禁用版本编号。
	ULONG MaximumNumberOfVersions;			

	// 0x28 	4 	  	Version Number
	// This file's version (if any). Will be zero if Maximum Number of Versions is zero. 
	// 这个文件的版本(如果有的话)。如果最大版本数为零，则为零。
	ULONG VersionNumber;					
	
	// 0x2C 	4 	  	Class Id
	// Class Id from bidirectional Class Id index. 
	// 来自双向类Id索引的类Id。
	ULONG ClassId;							
	
	// NTFS 3.0 or higher
	// A numeric identifier of the disk quota that has been charged for the file (probably an index into the file "\$Extend\$Quota").
	// If quotas are disabled, the value of QuotaId is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, this member is only present if the file has been accessed since the upgrade.
	// 已为文件收费的磁盘配额的数字标识符(可能是文件"\$Extend\$Quota"中的索引)。
	// 如果禁用配额，QuotaId的值为零。
	// 此成员只出现在NTFS 3.0中。
	// 如果卷已从NTFS的早期版本升级到3.0版本，则只有在升级后访问了该文件时才会出现该成员。
	// Quota Id of the user owning the file. This Id is a key in the $O and $Q Indexes of the file $Quota. If zero, then quotas are disabled. 
	// 拥有文件的用户的配额Id。这个Id是文件$Quota的$O和$Q索引中的一个键。如果为零，则禁用配额。
	ULONG QuotaId; 

	// NTFS 3.0 or higher
	// A numeric identifier of the security descriptor that applies to the file(probably an index into the file "\$Secure").This member is only present in NTFS 3.0.
	// If a volume	has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// 应用于文件的安全描述符的数字标识符(可能是文件"\$Secure"中的索引)。此成员只出现在NTFS 3.0中。
	// 如果卷已从NTFS的早期版本升级到3.0版本，则只有在升级后访问了该文件时才会出现该成员。
	// This should not be confused with a Security Identifier. The Security Id is a key in the $SII Index and $SDS Data Stream in the file $Secure. 
	// 这不应该与安全标识符混淆。安全Id是$SII索引中的一个键，而$SDS数据流是文件$Secure中的一个键。
	ULONG SecurityID; 

	// NTFS 3.0 or higher
	// The size, in bytes, of the charge to the quota for the file.
	// If quotas are disabled, the value of QuotaCharge is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// 文件的配额费用的大小, 以字节为单位。
	// 如果禁用配额，QuotaCharge的值为零。
	// 此成员只出现在NTFS 3.0中。
	// 如果卷已从NTFS的早期版本升级到3.0版本，则只有在升级后访问了该文件时才会出现该成员。
	// The number of bytes this file user from the user's quota. This should be the total data size of all streams. If zero, then quotas are disabled. 
	// 此文件用户从用户配额中获得的字节数。这应该是所有流的总数据大小。如果为零，则禁用配额。
	ULONGLONG QuotaCharged; 

	// NTFS 3.0 or higher
	// The Update Sequence Number of the file.
	// If journaling is not enabled, the value of Usn is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// 文件的更新序列号。
	// 如果不启用日志记录，Usn的值为零。
	// 此成员只出现在NTFS 3.0中。
	// 如果卷已从NTFS的早期版本升级到3.0版本，则只有在升级后访问了该文件时才会出现该成员。
	// Last Update Sequence Number of the file. This is a direct index into the file $UsnJrnl. If zero, the USN Journal is disabled. 
	// 文件的最新更新序列号。这是指向文件$UsnJrnl的直接索引。如果为零，则表示USN日志处于被禁用状态。
	USN Usn; 
} STANDARD_INFORMATION, * PSTANDARD_INFORMATION;

/* 
	0x20 	  	
	$ATTRIBUTE_LIST
	is always nonresident and consists of an array of ATTRIBUTE_LIST
	属性列表
	属性列表总是非驻留的，并且由ATTRIBUTE_LIST数组组成

	The attribute list attribute is always nonresident and consists of an array of ATTRIBUTE_LIST structures.
	An attribute list attribute is only needed when the attributes of a file do not fit in a single MFT record. 
	Possible reasons for overflowing a single MFT entry include:
	1.The file has a large numbers of alternate names (hard links).
	2.The attribute value is large, and the volume is badly fragmented.
	3.The file has a complex security descriptor (does not affect NTFS 3.0).
	4.The file has many streams.
	属性列表属性总是非驻留的，它由ATTRIBUTE_LIST结构数组组成。
	只有当文件的属性不适合单个MFT记录时，才需要属性列表属性。
	溢出单一MFT记录产生ATTRIBUTE_LIST的可能原因包括:
	1.该文件有大量替代名称(硬链接)。
	2.属性值很大，并且卷被严重分割。
	3.该文件具有复杂的安全描述符(不影响NTFS 3.0)。
	4.该文件有许多流。
*/
typedef struct 
{
	// 0x00 	4 	Type
	// The type of the attribute.
	// 属性类型。
	ATTRIBUTE_TYPE Attribute;			

	// 0x04 	2 	Record length 
	// The size, in bytes, of the attribute list entry.
	// 本记录长度：属性列表项的大小，以字节为单位。
	USHORT Length;						

	// 0x06 	1 	Name length (N) 
	// The size, in characters, of the name (if any) of the attribute.
	// 属性名长度：属性名称(如果有的话)的字符数量。
	UCHAR NameLength;					

	// 0x07 	1 	Offset to Name: If the name doesn't exist, does this point at the attribute or zero? it always points to where the name would be (0x1A) 0x04 record allocation (8 byte alignment). 属性名偏移. 如果名称不存在，则此点在属性处或0处? 它总是指向名称所在的位置(0x1A) 0x04记录分配(8字节对齐).
	// The offset, in bytes, from the start of the ATTRIBUTE_LIST structure to the attribute name. The attribute name is stored as a Unicode string.
	// 属性名偏移，从ATTRIBUTE_LIST结构开始到属性名的偏移量(以字节为单位)。属性名存储为Unicode字符串。
	USHORT NameOffset;					
	
	// 0x08 	8 	Starting VCN: zero if the attribute is resident 起始VCN，如果属性为常驻，则为零
	// The lowest valid Virtual Cluster Number (VCN) of this portion of the attribute value.
	// 起始VCN，属性值的这一部分的最低有效虚拟集群号(VCN)。
	ULONGLONG LowVcn;					

	// 0x10 	8 	Base File Reference of the attribute 属性的文件参考号
	// The file reference number of the MFT entry containing the NONRESIDENT_ATTRIBUTE structure for this portion of the attribute value.
	// 属性的文件参考号，包含属性值的这一部分的NONRESIDENT_ATTRIBUTE结构的MFT条目的文件引用号。
	ULONGLONG FileReferenceNumber;		

	// 0x18 	2 	Attribute Id: Each attribute has a unique identifier. 属性标识，每个属性都有一个唯一的标识符。
	// A numeric identifier for the instance of the attribute.
	// 属性实例的数字标识符。
	USHORT AttributeNumber;				

	// 0x1A 	2N 	Name in Unicode (if N > 0)
	// 对齐或保留
	USHORT AligmentOrReserved[3];		
} ATTRIBUTE_LIST, *PATTRIBUTE_LIST;

/* 
	0x30 	  	
	$FILE_NAME
	file attribute is always resident
	ULONGLONG informations only updated, if name changes
	文件名属性
	文件属性始终是常驻属性
*/
typedef struct 
{
	// 0x00 	8 	File reference to the parent directory. 父目录的FRN 
	// The file reference number of the directory in which the filename is entered.
	// points to a MFT Index of a directory
	// 父目录的FRN。
	// 指向目录的MFT索引
	ULONGLONG DirectoryFileReferenceNumber;	

	// 0x08 	8 	C Time - File Creation MFT创建时间 
	// The time when the file was created in the standard time format (that is. the number of 100-nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure
	// 以标准时间格式创建文件的时间(即。自1601年1月1日起的100纳秒间隔数)。
	// 此成员只在文件名更改时更新，并且可能与STANDARD_INFORMATION结构中同名字段不同。
	FILETIME CreationTime;					

	// 0x10 	8 	A Time - File Altered 最后一次MFT更新时间 
	// The time when the file attributes were last changed in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// 文件属性最后一次以标准时间格式更改的时间(即自1601年1月1日以来的100 -纳秒间隔)。
	// 此成员仅在文件名更改时才会更新，并且可能与STANDARD_INFORMATION结构中同名字段不同。
	FILETIME ChangeTime;					

	// 0x18 	8 	M Time - MFT Changed 最后一次MFT写入时间
	// The time when the file was last written in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// 文件最后一次以标准时间格式写入的时间(即自1601年1月1日以来的100纳秒间隔)。
	// 此成员仅在文件名更改时才会更新，并且可能与STANDARD_INFORMATION结构中同名字段不同。
	FILETIME LastWriteTime;					

	// 0x20 	8 	R Time - File Read 最后一次MFT访问时间 
	// The time when the file was last accessed in the standard time format(that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// 文件最后一次以标准时间格式访问的时间(即自1601年1月1日以来的100纳秒间隔)。
	// 此成员只在文件名更改时更新，并且可能与STANDARD_INFORMATION结构中同名字段不同。
	FILETIME LastAccessTime;				

	// 0x28 	8 	Allocated size of the file
	// The size, in bytes, of disk space allocated to hold the attribute value.This member is only updated when the filename changes.
	// 分配给保存属性值的磁盘空间的大小，以字节为单位。此成员仅在文件名更改时更新。
	ULONGLONG AllocatedSize;				

	// 0x30 	8 	Real size of the file 偶尔与文件大小GetFileSize不同 
	// The size, in bytes, of the attribute value. This member is only updated when the filename changes.
	// 属性值的大小(以字节为单位)。此成员仅在文件名更改时更新。
	ULONGLONG DataSize;						

	// 0x38 	4 	Flags, e.g. Directory, compressed, hidden
	// The attributes of the file.
	// This member is only updated when the filename changes and may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// 文件的属性。
	// 此成员只在文件名更改时更新，并且可能与STANDARD_INFORMATION结构中同名字段不同。
	ULONG FileAttributes;					

	// 0x3c 	4 	Used by EAs and Reparse
	// 对齐或保留
	ULONG AligmentOrReserved;				

	// 0x40 	1 	Filename length in characters (L)
	// The length, in characters, of the filename.
	// 文件名的长度, 以字符数量为单位。
	UCHAR NameLength;						

	// 0x41 	1 	Filename namespace: 0x01 Long 0x02 Short 0x00 Posix?
	// The type of the name.A type of zero indicates an ordinary name, a type of one indicates a long name corresponding to a short name, and a type of two indicates a short name corresponding to a long name.
	// 名称的类型。类型0表示普通名称，类型1表示与短名称对应的长名称，类型2表示与长名称对应的短名称。
	UCHAR NameType;							

	// 0x42 	2L 	File name in Unicode (not null terminated)
	// The name, in Unicode, of the file.
	// 文件的Unicode名称(不以空字符结尾)。
	WCHAR Name[1];							
} FILENAME_ATTRIBUTE, *PFILENAME_ATTRIBUTE;

/* NTFS_RECORD_HEADER.Type */
#define NRH_FILE_TYPE  0x454C4946  /* 'FILE' */

#define FRH_IN_USE    0x0001    /* Record is in use */
#define FRH_DIRECTORY 0x0002    /* Record is a directory */
#define FRH_UNKNOWN1  0x0004    /* Don't know */
#define FRH_UNKNOWN2  0x0008    /* Don't know */

#define MAX_DISK_COUNT 32

/*
参考：https://docs.microsoft.com/zh-cn/windows/desktop/api/winioctl/ns-winioctl-retrieval_pointers_buffer
逻辑簇和虚拟族的概念：
簇：指代磁盘的一组扇区。目的是将磁盘以簇为单位进行管理。
逻辑簇：是磁盘物理簇的逻辑映射，通过逻辑簇能够直接访问磁盘物理簇中的数据。如果用火车车厢做比喻，逻辑簇相当于整节车厢，扇区就相当于车厢中大小相同的各个座位。
虚拟簇：是逻辑簇的分组。同样用火车车厢来做比喻，可将车厢分成1节或多节的软卧车厢、硬卧车厢、硬座车厢，虚拟簇就相当于软卧车厢、硬卧车厢、硬座车厢。
逻辑簇号(LCN)，逻辑簇的编号, 相当于一节车厢的编号。
虚拟簇号(VCN)，虚拟簇的编号, 相当于软卧车厢的编号。

结构如下：

↓StartVCN                   ↓NextVCN             ↓NextVCN              ↓NextVCN
+-------------------------------------------------------------------------------
| LCN1 | LCN2 | LCN3 | LCN4 | LCN5 | LCN6 | LCN7 | LCN8 | LCN9 | LCN10 | ...
+-------------------------------------------------------------------------------
|           VCN1            |           VCN2     |            VCN3     | ...

typedef struct RETRIEVAL_POINTERS_BUFFER 
{
	// 虚拟簇的数量
	DWORD ExtentCount; 

	// 开始虚拟簇号
	LARGE_INTEGER StartingVcn;

	// 虚拟簇，其数量在ExtentCount变量中已经指定
	struct 
	{
		// 下一个虚拟簇的开始VCN。
		// 这个值减去StartingVcn或前一个NextVcn，即是当前虚拟簇的长度（逻辑簇的数量）。
		// 此长度是FSCTL_MOVE_FILE操作的输入。
		LARGE_INTEGER NextVcn;

		// 当前虚拟簇中的开始逻辑簇号LCN。
		// 这个值是FSCTL_MOVE_FILE操作的输入。
		LARGE_INTEGER Lcn; 
	} Extents[1];
} RETRIEVAL_POINTERS_BUFFER, *PRETRIEVAL_POINTERS_BUFFER;

如何根据虚拟和逻辑簇获得文件的记录信息？

1、获得每个逻辑簇的大小(字节数)：
ULONG sectorsPerCluster, bytesPerSectors, bytesPerCluster;
GetDiskFreeSpace("C:", &sectorsPerCluster, &bytesPerSectors, NULL, NULL);
bytesPerCluster = sectorsPerCluster * bytesPerSectors; //通常：8 * 512 = 4096

2、获得文件的大小(字节数)：
HANDLE hFile = CreateFile("H:\\$MFT::$DATA", FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
BY_HANDLE_FILE_INFORMATION fileInfo;
GetFileInformationByHandle(hFile, &fileInfo);
ULONGLONG fileSize = fileInfo.nFileSizeLow + ((ULONGLONG)fileInfo.nFileSizeHigh << 32);

3、计算文件包含的逻辑簇数量
ULONG lclusterCount = (fileSize + bytesPerCluster - 1) / bytesPerCluster;

4、计算文件包含的虚拟簇数量
STARTING_VCN_INPUT_BUFFER vcnBuf;
PRETRIEVAL_POINTERS_BUFFER pOutBuf;
ULONG outBufSize, readBytes;

outBufSize = (ULONG) & (((PRETRIEVAL_POINTERS_BUFFER)0)->Extents) + lclusterCount * sizeof(pOutBuf->Extents); //consider that each cluster is in different extent
pOutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(outBufSize * sizeof(char));
memset(pOutBuf, 0x0, outBufSize * sizeof(char));

vcnBuf.StartingVcn.QuadPart = 0;
DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &vcnBuf, sizeof(vcnBuf), pOutBuf, outBufSize * sizeof(char), &readBytes, NULL);
ULONG vclusterCount = pOutBuf->ExtentCount; 

5、读出每个虚拟簇中的逻辑簇
ULONGLONG* lcClusters = new ULONGLONG[lclusterCount]; //逻辑簇数组
LARGE_INTEGER vcn = pOutBuf->StartingVcn;
LARGE_INTEGER lcn;
ULONG vcIndx, lcIndx, lcCount;

for (vcIndx = 0, lcIndx = 0; vcIndx < vclusterCount; ++vcIndx, ++lcIndx)
{
	lcn = pOutBuf->Extents[vcIndx].Lcn;
	for (lcCount = (ULONG)(pOutBuf->Extents[vcIndx].NextVcn.QuadPart - vcn.QuadPart); //逻辑簇数量=NextVcn-PrevVcn
		lcCount;
		++lcIndx, --lcCount)
	{
		lcClusters[lcIndx] = lcn.QuadPart++;//逻辑簇号(lcn), 逻辑簇始终是按顺序编号的。
	}
	vcn = pOutBuf->Extents[vcIndx].NextVcn;//定位到下一个虚拟簇
}
free(pOutBuf);
CloseHandle(hFile);

6、从每个逻辑簇中读取文件记录
#define CLUSTERSPERREAD 1024

BOOL bOk;
PUCHAR buffer = new UCHAR[CLUSTERSPERREAD * bytesPerCluster]; //记录缓存
HANDLE hDrive = CreateFile(pDriveFile, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
LARGE_INTEGER offset;
ULONGLONG curLcn = 0;
for (lcIndx = 0; lcIndx < lclusterCount; ++lcIndx)
{
	//取出虚拟簇的开始逻辑簇号
	curLcn = lcClusters[lcIndx];

	//定位到逻辑簇
	offset.QuadPart = curLcn * bytesPerCluster;
	SetFilePointer(hDrive, offset.LowPart, &offset.HighPart, FILE_BEGIN);

	//读出逻辑簇中的每条记录
	bOk = ReadFile(hDrive, buffer, CLUSTERSPERREAD * bytesPerCluster, &readBytes, NULL);
	
	//从buffer中解析记录
	if (bOk && readBytes > 0)
	{
		PFILE_RECORD_HEADER pfileRecordheader = (PFILE_RECORD_HEADER)buffer;
	}
}
delete buffer;
delete lcClusters;
CloseHandle(hDrive);

*/

#endif //__NTFS_H__
