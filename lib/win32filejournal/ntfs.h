/*
	ģ�飺NTFS�ṹ
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����
	�ο���
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
	NTFS���ϵ�һ�ж����ļ���
	�ֳ�����: Ԫ�����ļ�����ͨ�ļ���
	Ԫ�����ļ��������ھ����Ϣ������ͨ�ļ�������������ݡ�

	Below is a table of files found on a Win2K volume (Key).
	��������Win2K��(Key)�ϵ��ļ���ṹ��
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
	ע�⣺
	����ĳ��ԭ��$ObjId��$Quota��$Reparse��$UsnJrnl��������Ԫ�����ļ�һ����inode��ŵ���24��
	���⣬ÿ��ϵͳ�ļ������к����ǵ������ǵ�mft��¼�ţ��������Ӳ��޸ġ�
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
	$Boot������ϵͳ������ϵͳ�ļ���
	���Ԫ�����ļ�ָ��������������
	���������ھ��С���غ�MFT����Ϣ��
	����Ωһ�������¶�λ���ļ���

*/
typedef struct
{
	UCHAR		Jump[3];				// 0x0000 	3 	Jump to the boot loader routine ��ת���������س���
	UCHAR		Format[8];				// 0x0003 	8 	System Id: "NTFS    "
	USHORT		BytesPerSector;			// 0x000B 	2 	Bytes per sector ÿ�������ֽ���
	UCHAR		SectorsPerCluster;		// 0x000D 	1 	Sectors per cluster ÿ�������� 
	USHORT		BootSectors;			// ����������
	UCHAR		Mbz1;					// ����0 
	USHORT		Mbz2;					// ����0 
	USHORT		Reserved1;				// ����0 
	UCHAR		MediaType;				// 0x0015 	1 	Media descriptor: A media descriptor of 0xF8 means a hard disk. ��������: 0xF8��ý����������ʾӲ�̡�
	USHORT		Mbz3;					// ��Ϊ0 
	USHORT		SectorsPerTrack;		// 0x0018 	2 	Sectors per track ÿ�ŵ�������
	USHORT		NumberOfHeads;			// 0x001A 	2 	Number of heads ��ͷ�� 
	ULONG		PartitionOffset;		// �÷�����ƫ�ƣ����÷���ǰ������������ һ��Ϊ�ŵ�������0x3f 63�� 
	ULONG		Rserved2;
	ULONG		Rserved3;				// 0x0024 	4 	Usually 80 00 80 00. A value of 80 00 00 00 has been seen on a USB thumb drive which is formatted with NTFS under Windows XP. Note this is removable media and is not partitioned, the drive as a whole is NTFS formatted. ��Windows XP��ʹ��NTFS��ʽ����USB u������ʾֵΪ8000 00 00��ע�⣬���ǿ��ƶ�ý�壬û�з�������������Ϊһ��������NTFS��ʽ��
	ULONGLONG	TotalSectors;			// 0x0028 	8 	Number of sectors in the volume. �÷�����������
	ULONGLONG	MftStartLcn;			// 0x0030 	8 	LCN of VCN 0 of the $MFT. MFT��($MFT)����ʼ�غ�LCN
	ULONGLONG	Mft2StartLcn;			// 0x0038 	8 	LCN of VCN 0 of the $MFTMirr. MFT���ݱ�($MFTMirr)����ʼ�غ�LCN 			
	ULONG		ClustersPerFileRecord;  // 0x0040 	4 	Clusters per MFT Record: This can be negative, which means that the size of the MFT record is smaller than a cluster. In this case the size of the MFT record in bytes is equal to 2^(-1 * Clusters per MFT record). So for example if Clusters per MFT Record is 0xF6 (-10 in decimal), the MFT record size is 2^(-1 * -10) = 2^10 = 1024 bytes. ÿ��MFT��¼���������أ���¼���ֽڲ�һ��ΪClustersPerFileRecord*SectorsPerCluster*BytesPerSector��������Ǹ��ģ�����ζ��MFT��¼�Ĵ�СС�ڴء��ڱ����У����ֽ�Ϊ��λ��MFT��¼�Ĵ�С����2^(-1 *Clusters per MFT��¼)����ˣ����磬���ÿ��MFT��¼�ļ�Ⱥ��0xF6(ʮ����Ϊ-10)����ôMFT��¼��С��2^(-1 * -10)= 2^10 = 1024�ֽڡ�
	ULONG		ClustersPerIndexBlock;	// 0x0044 	4 	Clusters per Index Record: This can be negative, which means that the size of the Index record is smaller than a cluster. In this case the size of the Index record in bytes is equal to 2^(-1 * Clusters per Index record). So for example if Clusters per MFT Record is 0xF6 (-10 in decimal), the MFT record size is 2^(-1 * -10) = 2^10 = 1024 bytes. ÿ��������Ĵ�����������Ǹ��ģ�����ζ��Index��¼�Ĵ�СС�ڴء���¼���ֽڲ�һ��Ϊ��ClustersPerFileRecord*SectorsPerCluster*BytesPerSector���ڱ����У����ֽ�Ϊ��λ��Index��¼�Ĵ�С����2^(-1 *Clusters per Index��¼)����ˣ����磬���ÿ��MFT��¼�ļ�Ⱥ��0xF6(ʮ����Ϊ-10)����ôMFT��¼��С��2^(-1 * -10)= 2^10 = 1024�ֽڡ�
	ULONGLONG	VolumeSerialNumber;		// 0x0048 	8 	Volume serial number �����к�
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
	MFT��һ���ļ���¼��
	���е�ÿ���ļ�����һ�������������ļ���¼������������
	��Unix�����У��ļ���¼�൱��inode��
	���������ļ��ĵ�һ���ļ���¼��Ϊ�����ļ���¼�������ļ���¼��Ϊ��չ�ļ���¼��

	A FILE Record is built up from a header, several variable length attributes and an end marker (simply 0xFFFFFFFF).
	�ļ���¼��һ��ͷ�������ɱ䳤�����Ժ�һ���������(�򵥵�˵����0xFFFFFFFF)��ɡ�

	The file record Layout is as follows
	�ļ���¼�������£�

		Record Header
		Attribute
		Attribute
		...
		End Marker (0xFFFFFFFF)


	The file record structure is as follows
	�ļ���¼�ṹ���£�

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

	(a) The offset to these two fields depends on your operating system. �������ֶε�ƫ����ȡ�������Ĳ���ϵͳ��
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
	NTFS��¼ͷ
*/
typedef struct
{
	// 0x00 	4 	  	Magic number 'FILE'
	// The type of NTFS record.When the value of Type is considered as a sequence of four one - byte characters, 
	// it normally spells an acronym for the type.Defined values include:
	// NTFS��¼�����͡���Type��ֵ����Ϊ�����ĸ����ֽ��ַ���ɵ�����ʱ����ͨ��Ϊ������ƴдһ�����Դʡ�����ֵ����:
	// 'FILE' 'INDX' 'BAAD' 'HOLE' *CHKD'
	ULONG Type;						

	// 0x04 	2 	  	Offset to the Update Sequence
	// The offset, in bytes, from the start of the structure to the Update Sequence Array(USA).
	// ������������(USA)ƫ�ƣ�У��ֵ��ַ���ӽṹ��ʼ���������������ƫ���������ֽ�Ϊ��λ��
	USHORT UsaOffset;				
	
	// 0x06 	2 	  	Size in words of Update Sequence (S)
	// The number of values in the Update Sequence Array.
	// ������������(USA)��ֵ����Ŀ��1+n��1ΪУ��ֵ������nΪ���滻ֵ������
	USHORT UsaCount;				

	// 0x08 	8 	  	$LogFile Sequence Number (LSN)
	// The Update Sequence Number(USN) of the NTFS record.
	// This is changed every time the record is modified. 
	// NTFS��¼�ĸ������к�(USN)��
	// ÿ���޸ļ�¼ʱ������Ĵ�ֵ��
	USN Usn;						
} NTFS_RECORD_HEADER, * PNTFS_RECORD_HEADER;

/*
	FILE_RECORD_HEADER
	�ļ���¼ͷ

	An record in the MFT consists of a FILE_RECORD_HEADER followed by a sequence of attributes.
	MFT��¼��FILE_RECORD_HEADER��һϵ��������ɡ�
*/
typedef struct
{
	// An NTFS_RECORD_HEADER structure with a Type of 'FILE'
	// ����'FILE'���͵�NTFS_RECORD_HEADER�ṹ��
	// Ntfs.Type����'ELIF'�� 
	NTFS_RECORD_HEADER Ntfs;

	// 0x10 	2 	  	Sequence number
	// Number of times this mft record has been reused.
	// N.B.The increment(skipping zero) is done when the file is deleted.
	// N.B.If this is set to zero it is left as zero.
	// ��MFT��¼�����õĴ���
	// ע�⣬����(������)����ɾ���ļ�ʱ��ɵġ�
	// ע�⣬�������Ϊ�㣬���ͻᱻ����Ϊ�㡣
	USHORT SequenceNumber;			

	// 0x12 	2 	  	Hard link count
	// The number of directory links to the MFT record.
	// Number of hard links, i.e.the number of directory entries referencing this record.
	// N.B.Only used in mft base records.
	// ָ��MFT��¼��Ŀ¼���ӵ�������
	// Ӳ���ӵ������������ô˼�¼��Ŀ¼��Ŀ��������
	// ע��, ������mft������¼��
	USHORT LinkCount;				

	// 0x14 	2 	  	Offset to the first Attribute
	// The offset, in bytes, from the start of the structure to the first attribute of the MFT entry.
	// ��һ�����Ե�ƫ�ƣ��ӽṹ��ʼ��MFT��Ŀ�ĵ�һ�����Ե�ƫ��������λΪ�ֽڡ�
	USHORT AttributesOffset;		

	// 0x16 	2 	  	Flags
	// A bit array of flags specifying properties of the MFT entry. The values defined include :
	// InUse 0x0001  The MFT entry is in use
	// Directory 0x0002  The MFT entry represents a directory
	// ָ��MFT��Ŀ���Ե�λ��־���顣�����ֵ����:
	// 0x0001	InUse		Record is in use MFT��¼����ʹ��
	// 0x0002	Directory	Record is a directory	MFT��¼��ʾһ��Ŀ¼	
	// 0x0004 	Don't know
	// 0x0008 	Don't know
	USHORT Flags;					

	// 0x18 	4 	  	Real size of the FILE record 
	// The number of bytes used by the MFT record.
	// The Real Size is a count of how many bytes of the Record are actually used. 
	// N.B.The Real Size will be padded to an 8 byte boundary.
	// ��¼MFT��¼(ͷ������)���ܳ���(�ֽ���)������¼�ڴ�����ʵ��ռ�õ��ֽڿռ䡣
	// ʵ�ʴ�С�Ǽ�¼ʵ��ʹ�õ��ֽ�����
	// ע�⣬ʵ�ʴ�С�����Ϊ8�ֽڵı߽硣
	ULONG BytesInUse;				
	
	// 0x1C 	4 	  	Allocated size of the FILE record
	// The number of bytes allocated for the MFT record.
	// The Allocated Size is how much space the Record takes up on disk. 
	// This should be a multiple of the cluster size and should probably be equal to the size of an MFT File Record. 
	// �����MFT��¼���ֽ�����
	// ����Ĵ�С�Ǽ�¼ռ�ô����ϵĿռ䡣
	// ��Ӧ���Ǵش�С�ı��������ҿ��ܵ���MFT�ļ���¼�Ĵ�С��
	ULONG BytesAllocated;			
	
	// 0x20 	8 	  	File reference to the base FILE record
	// If the MFT record contains attributes that overflowed a base MFT record, this member contains the file reference number of the base record; otherwise, it contains zero.
	// This is zero for Base MFT Records.
	// When it is not zero it is a MFT Reference pointing to the Base MFT Record to which this Record belongs.
	// The Base Record contains the information about the Extension Record.
	// This information is stored in an ATTRIBUTE_LIST attribute.
	// ���MFT��¼�����������MFT����¼�����ԣ���˳�Ա����������¼���ļ����ú�; ����������0��
	// ���ڻ���MFT��¼�������㡣
	// ������Ϊ��ʱ������ָ��ü�¼�����Ļ���MFT��¼��MFT���á�
	// ������¼����������չ��¼����Ϣ��
	// ����Ϣ�洢��ATTRIBUTE_LIST�����С�
	ULARGE_INTEGER BaseFileRecord;	

	// 0x28 	2 	  	Next Attribute Id
	// The number that will be assigned to the next attribute added to the MFT record
	// ��Ҫ�������ӵ�MFT��¼����һ�����Եĺ��룬����һ����ID
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
	ÿ��MFT�ļ���¼���������Թ����ġ����ܵ������б���$AttrDef�ж��塣

	Note: 
	$PROPERTY_SET, $SYMBOLIC_LINK and $VOLUME_VERSION existed in NTFS v1.2, but weren't used. They no longer exist in NTFS v3.0 (that used by Win2K).
	Each MFT FILE Record is built up from Attributes. The list of possible Attributes is defined in $AttrDef.
	Each MFT record has a Standard Header, followed by a list of attributes (in order of ascending Attribute Id) and an end marker. The end marker is just four bytes: 0xFFFFFFFF.
	ע�⣺
	NTFS v1.2�д���$PROPERTY_SET��$SYMBOLIC_LINK��$VOLUME_VERSION����û��ʹ�����ǡ����ǲ��ٴ�����NTFS v3.0 (Win2Kʹ�õİ汾)�С�
	ÿ��MFT�ļ���¼���������Թ����ġ����ܵ������б���$AttrDef�ж��塣
	ÿ��MFT��¼����һ����׼��ͷ��Ȼ����һ�������б�(����������Id����)��һ��������ǡ��������ֻ���ĸ��ֽ�: 0xFFFFFFFF��

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
	//������Ϊ��פ����(Resident Attributes)���洢���磺
	//�ļ�ֻ�����浵�����ԣ� 
	//ʱ��������ļ�����ʱ��, ���һ���޸�ʱ�䣻 
	//����Ŀ¼ָ����ļ�(Ӳ���Ӽ���hard link count)��
	StandardInformation = 0x10,	

	//$ATTRIBUTE_LIST
	//��������ֵ���ܻ���������˴����Կ����Ƿǳ�פ����(Nonresident Attributes)��
	//��һ���ļ�Ҫ����MFT�ļ���¼ʱ, ���и����ԡ�
	AttributeList		= 0x20,	

	//$FILE_NAME
	//������Ϊ��פ����(Resident Attributes)���ļ������Կ����ж���� 
	//1.���ļ����Զ�Ϊ����ļ���(�Ա�MS-DOS��16λ�������) 
	//2.�����ļ�����Ӳ����ʱ 
	FileName			= 0x30, 

	//NT: $VOLUME_VERSION		
	//2K: $OBJECT_ID
	//������Ϊ��פ����(Resident Attributes)�� 
	//�����Դ���һ���ļ���Ŀ¼��64�ֽڱ�ʶ�������е�16�ֽڶ��ڸþ���˵��Ψһ������-���ٷ��񽫶���ID�������ǿ�ݷ�ʽ��OLE����Դ�ļ��� 
	//NTFS�ṩ����Ӧ��API����Ϊ�ļ���Ŀ¼����ͨ�������ID��������ͨ�����ļ����򿪡� 
	ObjectId			= 0x40, 

	//$SECURITY_DESCRIPTOR
	//�����Խ�������$Secure_DescriptorԪ�����ļ��С�
	//Ϊ��NTFS��ǰ�汾����������, ���о�����ͬ��ȫ���������ļ���Ŀ¼����ͬ���İ�ȫ����,
	//��ǰ�汾��NTFS��˽�еİ�ȫ��������Ϣ��ÿ���ļ���Ŀ¼�洢��һ��
	SecurityDescripter	= 0x50,	 

	//$VOLUME_NAME
	//�����˸þ�����Ƶ���Ϣ
	//�����Խ�������$VolumeNameԪ�����ļ��� 
	VolumeName			= 0x60, 

	//$VOLUME_INFORMATION
	//�����˸þ�İ汾����Ϣ
	//�����Խ�������$VolumeInformationԪ�����ļ��� 
	VolumeInformation	= 0x70, 

	//$DATA
	//��������ֵ���ܻ���������˴����Կ����Ƿǳ�פ����(Nonresident Attributes)��
	//�����Լ�¼�ļ������ݣ�һ���ļ�����һ��δ�������������ԣ������ж����������������ԣ� 
	//��һ���ļ������ж����������Ŀ¼û��Ĭ�ϵ��������ԣ������ж����ѡ�������������ԡ�
	Data				= 0x80, 

	//������������ʵ�ִ�Ŀ¼���ļ��������λͼ���� 
	IndexRoot			= 0x90, // $INDEX_ROOT
	IndexAllocation		= 0xA0, // $INDEX_ALLOCATION
	Bitmap				= 0xB0, // $BITMAP

	//NT: $SYMBOLIC_LINK		
	//2K: $REPARSE_POINT
	//�洢��һ���ļ����ؽ��������ݣ�NTFS�Ľ���(junction)�͹��ص���������ԡ�
	ReparsePoint		= 0xC0, 
	
	//��������Ϊ��չ���ԣ����Ѳ��ٱ�����ʹ�ã�֮�����ṩ��Ϊ��OS/2���򱣳������� 
	EAInformation		= 0xD0,	// $EA_INFORMATION
	EA					= 0xE0,	// $EA

	PropertySet			= 0xF0, // NT: $PROPERTY_SET
	LoggedUtilityStream = 0x100 // 2K: $LOGGED_UTILITY_STREAM
} ATTRIBUTE_TYPE, *PATTRIBUTE_TYPE;

/* 
	ATTRIBUTE Structure
	���Խṹ
*/
typedef struct 
{
	// The type of the attribute.
	// ���Ե����͡�
	ATTRIBUTE_TYPE AttributeType;

	// The size, in bytes, of the resident part of the attribute.
	// �����Գ��ȣ���������ֵ��: ���Եĳ�פ���ֵĴ�С�����ֽ�Ϊ��λ��
	ULONG Length;

	// Specifies, when true, that the attribute value is nonresident.
	// ָ����Ϊ��ʱ������Ϊ�ǳ�פ��
	BOOLEAN Nonresident;

	// The size, in characters, of the name (if any) of the attribute
	// �����������Ƴ���: ��������(����еĻ�)���ַ�����
	UCHAR NameLength;

	// The offset, in bytes, from the start of the structure to the attribute name.
	// The attribute name is stored as a Unicode string.
	// ������ƫ��: �ӽṹ��ʼ����������ƫ���������ֽ�Ϊ��λ���������洢ΪUnicode�ַ�����
	USHORT NameOffset;

	// A bit array of flags specifying properties of the attribute.The values defined include :
	// 0x0001 = Compressed  The attribute is compressed
	// ���Ա�־�����Ե�ָ�����Ե�λ��־���顣�����ֵ����:
	// 0x0001 = Compressed  ���Ա�ѹ��
	USHORT Flags; 

	// A numeric identifier for the instance of the attribute.
	// ���Ա�ʶ������ʵ�������ֱ�ʶ����
	USHORT AttributeNumber;
} ATTRIBUTE, *PATTRIBUTE;

/* 
	Resident ATTRIBUTE 
	��פ����
*/
typedef struct 
{
	// An ATTRIBUTE structure containing members common to resident and nonresident attributes.
	// �������ͣ�һ�����Խṹ��������פ���Ժͷǳ�פ���Թ��еĳ�Ա��
	ATTRIBUTE Attribute;

	// The size, in bytes, of the attribute value.
	// ����ֵ�Ĵ�С, ���ֽ�Ϊ��λ��
	ULONG ValueLength;

	// The offset, in bytes, from the start of the structure to the attribute value
	// ����ֵƫ��, �ӽṹ��ʼ������ֵ��ƫ���������ֽ�Ϊ��λ
	USHORT ValueOffset;

	// A bit array of flags specifying properties of the attribute.The values defined include:
	// 0x0001 = Indexed  The attribute is indexed
	// ������־, ���Ե�ָ�����Ե�λ��־���顣�����ֵ����:
	// 0x0001 = Indexed ���Ա�����
	USHORT Flags; 
} RESIDENT_ATTRIBUTE, * PRESIDENT_ATTRIBUTE;

/* 
	nonresident ATTRIBUTE 
	�ǳ�פ����
*/
typedef struct 
{
	// An ATTRIBUTE structure containing members common to resident and nonresident attributes.
	// һ�����Խṹ��������פ���Ժͷǳ�פ���Թ��еĳ�Ա��
	ATTRIBUTE Attribute;

	// The lowest valid Virtual Cluster Number (VCN) of this portion of the attribute value.
	// Unless the attribute value is very fragmented(to the extent that an attribute list is needed to describe it), 
	// there is only one portion of the attribute value, and the value of LowVcn is zero.
	// ����ֵ����һ���ֵ������Ч����غ�(VCN)������ʼ����غš�
	// ��������ֵ�ǳ���ɢ(����Ҫһ�������б����������ĳ̶�)����������ֵֻ��һ���֣�����LowVcn��ֵΪ�㡣
	ULONGLONG LowVcn;

	// The highest valid VCN of this portion of the attribute value.
	// ����ֵ����һ���ֵ������ЧVCN���������������
	ULONGLONG HighVcn;

	// The offset, in bytes, from the start of the structure to the run array that contains the
	// mappings between VCNs and Logical Cluster Numbers(LCNs)
	// �ӽṹ��ʼ������VCNs���߼��غ�(LCNs)֮��ӳ������������ƫ���������ֽ�Ϊ��λ��
	USHORT RunArrayOffset;

	// The compression unit for the attribute expressed as the logarithm to the base two of
	// the number of clusters in a compression unit.If CompressionUnit is zero, the attribute is not compressed.
	// ���Ե�ѹ����Ԫ����ʾΪѹ����Ԫ�д���������2Ϊ�׵Ķ�����
	// ���CompressionUnitΪ�㣬������δ��ѹ����
	UCHAR CompressionUnit;

	// �������
	UCHAR AligmentOrReserved[5];

	// The size, in bytes, of disk space allocated to hold the attribute value.
	// �������������ֵ�Ĵ��̿ռ�Ĵ�С�����ֽ�Ϊ��λ��
	ULONGLONG AllocatedSize;

	// The size, in bytes, of the attribute value.
	// This may be larger than the AllocatedSize if the attribute value is compressed or sparse.
	// ����ֵ�Ĵ�С, ���ֽ�Ϊ��λ��
	// �������ֵ��ѹ����ϡ�裬���ֵ���ܴ����ѷ���Ĵ�С��
	ULONGLONG DataSize;

	// The size, in bytes, of the initialized portion of the attribute value.
	// ����ֵ�ĳ�ʼ�����ֵĴ�С, ���ֽ�Ϊ��λ��
	ULONGLONG InitializedSize;

	// The size, in bytes, of the attribute value after compression.
	// This member is only present when the attribute is compressed.
	// ѹ��������ֵ�Ĵ�С�����ֽ�Ϊ��λ��
	// �˳�Ա�������Ա�ѹ��ʱ�ų��֡�
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
	��׼��Ϣ����

	In old version of NTFS this Attribute contained little more than the DOS File Permissions and the file times.
	Windows 2000 introduced four new fields which are used to reference Quota, Security, File Size and Logging information.
	As defined in $AttrDef, this attribute has a minimum size of 48 bytes and a maximum of 72 bytes.
	�ھɰ汾��NTFS�У��������ֻ����DOS�ļ�Ȩ�޺��ļ�ʱ�䡣
	Windows 2000�������ĸ��µ��ֶΣ�������������ȫ�ԡ��ļ���С����־��Ϣ��
	������$AttrDef�ж���ģ������Ե���С��СΪ48�ֽڣ�����СΪ72�ֽڡ�

*/
typedef struct
{
	// 0x00 	8 	  	C Time - File Creation
	// The time when the file was created in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601)
	// �Ա�׼ʱ���ʽ�����ļ���ʱ��(����1601��1��1������100������������)
	FILETIME CreationTime;	

	// 0x08 	8 	  	A Time - File Altered
	// The time when the file attributes were last changed in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// �ļ��������һ���Ա�׼ʱ���ʽ���ĵ�ʱ��(����1601��1��1������100������������)��
	FILETIME ChangeTime;					

	// 0x10 	8 	  	M Time - MFT Changed
	// The time when the file was last written in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601)
	// �ļ����һ���Ա�׼ʱ���ʽд���ʱ��(����1601��1��1��������100������)
	FILETIME LastWriteTime;					

	// 0x18 	8 	  	R Time - File Read
	// The time when the file was last accessed in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601)
	// ���һ���Ա�׼ʱ���ʽ�����ļ���ʱ��(����1601��1��1��������100������)
	FILETIME LastAccessTime;				

	// 0x20 	4 	  	DOS File Permissions
	// FILE_ATTRIBUTES_* like in windows.h
	// The attributes of the file.Defined attributes include:
	// �ļ������ԡ���������԰���:
	// 0x0001	FILE_ATTRIBUTE_READONLY			Read-Only		ֻ��
	// 0x0002	FILE_ATTRIBUTE_HIDDEN			Hidden			����
	// 0x0004 	FILE_ATTRIBUTE_SYSTEM			System			ϵͳ
	// 0x0010	FILE_ATTRIBUTE_DIRECTORY		Directory		Ŀ¼
	// 0x0020	FILE_ATTRIBUTE_ARCHIVE			Archive			�浵
	// 0x0040	FILE_ATTRIBUTE_DEVICE           Device			�豸
	// 0x0080	FILE_ATTRIBUTE_NORMAL			Normal			����
	// 0x0100	FILE_ATTRIBUTE_TEMPORARY		Temporary		��ʱ
	// 0x0200	FILE_ATTRIBUTE_SPARSE_FILE		SparseFile		ϡ��
	// 0x0400	FILE_ATTRIBUTE_REPARSE_POINT	ReparsePoint	������ 
	// 0x0800	FILE_ATTRIBUTE_COMPRESSED		Compressed		ѹ��
	// 0x1000	FILE_ATTRIBUTE_OFFLINE			Offline			����
	// 0x2000	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED NotContentIndexed	����
	// 0x4000	FILE_ATTRIBUTE_ENCRYPTED		Encrypted		����
	ULONG FileAttributes;

	// 0x24 	4 	  	Maximum Number of Versions
	// Maximum allowed versions for file. Zero means that version numbering is disabled. 
	// �ļ����������汾��0��ʾ���ð汾��š�
	ULONG MaximumNumberOfVersions;			

	// 0x28 	4 	  	Version Number
	// This file's version (if any). Will be zero if Maximum Number of Versions is zero. 
	// ����ļ��İ汾(����еĻ�)��������汾��Ϊ�㣬��Ϊ�㡣
	ULONG VersionNumber;					
	
	// 0x2C 	4 	  	Class Id
	// Class Id from bidirectional Class Id index. 
	// ����˫����Id��������Id��
	ULONG ClassId;							
	
	// NTFS 3.0 or higher
	// A numeric identifier of the disk quota that has been charged for the file (probably an index into the file "\$Extend\$Quota").
	// If quotas are disabled, the value of QuotaId is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, this member is only present if the file has been accessed since the upgrade.
	// ��Ϊ�ļ��շѵĴ����������ֱ�ʶ��(�������ļ�"\$Extend\$Quota"�е�����)��
	// ���������QuotaId��ֵΪ�㡣
	// �˳�Աֻ������NTFS 3.0�С�
	// ������Ѵ�NTFS�����ڰ汾������3.0�汾����ֻ��������������˸��ļ�ʱ�Ż���ָó�Ա��
	// Quota Id of the user owning the file. This Id is a key in the $O and $Q Indexes of the file $Quota. If zero, then quotas are disabled. 
	// ӵ���ļ����û������Id�����Id���ļ�$Quota��$O��$Q�����е�һ���������Ϊ�㣬�������
	ULONG QuotaId; 

	// NTFS 3.0 or higher
	// A numeric identifier of the security descriptor that applies to the file(probably an index into the file "\$Secure").This member is only present in NTFS 3.0.
	// If a volume	has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// Ӧ�����ļ��İ�ȫ�����������ֱ�ʶ��(�������ļ�"\$Secure"�е�����)���˳�Աֻ������NTFS 3.0�С�
	// ������Ѵ�NTFS�����ڰ汾������3.0�汾����ֻ��������������˸��ļ�ʱ�Ż���ָó�Ա��
	// This should not be confused with a Security Identifier. The Security Id is a key in the $SII Index and $SDS Data Stream in the file $Secure. 
	// �ⲻӦ���밲ȫ��ʶ����������ȫId��$SII�����е�һ��������$SDS���������ļ�$Secure�е�һ������
	ULONG SecurityID; 

	// NTFS 3.0 or higher
	// The size, in bytes, of the charge to the quota for the file.
	// If quotas are disabled, the value of QuotaCharge is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// �ļ��������õĴ�С, ���ֽ�Ϊ��λ��
	// ���������QuotaCharge��ֵΪ�㡣
	// �˳�Աֻ������NTFS 3.0�С�
	// ������Ѵ�NTFS�����ڰ汾������3.0�汾����ֻ��������������˸��ļ�ʱ�Ż���ָó�Ա��
	// The number of bytes this file user from the user's quota. This should be the total data size of all streams. If zero, then quotas are disabled. 
	// ���ļ��û����û�����л�õ��ֽ�������Ӧ�����������������ݴ�С�����Ϊ�㣬�������
	ULONGLONG QuotaCharged; 

	// NTFS 3.0 or higher
	// The Update Sequence Number of the file.
	// If journaling is not enabled, the value of Usn is zero.
	// This member is only present in NTFS 3.0.
	// If a volume has been upgraded from an earlier version of NTFS to version 3.0, 
	// this member is only present if the file has been accessed since the upgrade.
	// �ļ��ĸ������кš�
	// �����������־��¼��Usn��ֵΪ�㡣
	// �˳�Աֻ������NTFS 3.0�С�
	// ������Ѵ�NTFS�����ڰ汾������3.0�汾����ֻ��������������˸��ļ�ʱ�Ż���ָó�Ա��
	// Last Update Sequence Number of the file. This is a direct index into the file $UsnJrnl. If zero, the USN Journal is disabled. 
	// �ļ������¸������кš�����ָ���ļ�$UsnJrnl��ֱ�����������Ϊ�㣬���ʾUSN��־���ڱ�����״̬��
	USN Usn; 
} STANDARD_INFORMATION, * PSTANDARD_INFORMATION;

/* 
	0x20 	  	
	$ATTRIBUTE_LIST
	is always nonresident and consists of an array of ATTRIBUTE_LIST
	�����б�
	�����б����Ƿ�פ���ģ�������ATTRIBUTE_LIST�������

	The attribute list attribute is always nonresident and consists of an array of ATTRIBUTE_LIST structures.
	An attribute list attribute is only needed when the attributes of a file do not fit in a single MFT record. 
	Possible reasons for overflowing a single MFT entry include:
	1.The file has a large numbers of alternate names (hard links).
	2.The attribute value is large, and the volume is badly fragmented.
	3.The file has a complex security descriptor (does not affect NTFS 3.0).
	4.The file has many streams.
	�����б��������Ƿ�פ���ģ�����ATTRIBUTE_LIST�ṹ������ɡ�
	ֻ�е��ļ������Բ��ʺϵ���MFT��¼ʱ������Ҫ�����б����ԡ�
	�����һMFT��¼����ATTRIBUTE_LIST�Ŀ���ԭ�����:
	1.���ļ��д����������(Ӳ����)��
	2.����ֵ�ܴ󣬲��Ҿ����طָ
	3.���ļ����и��ӵİ�ȫ������(��Ӱ��NTFS 3.0)��
	4.���ļ����������
*/
typedef struct 
{
	// 0x00 	4 	Type
	// The type of the attribute.
	// �������͡�
	ATTRIBUTE_TYPE Attribute;			

	// 0x04 	2 	Record length 
	// The size, in bytes, of the attribute list entry.
	// ����¼���ȣ������б���Ĵ�С�����ֽ�Ϊ��λ��
	USHORT Length;						

	// 0x06 	1 	Name length (N) 
	// The size, in characters, of the name (if any) of the attribute.
	// ���������ȣ���������(����еĻ�)���ַ�������
	UCHAR NameLength;					

	// 0x07 	1 	Offset to Name: If the name doesn't exist, does this point at the attribute or zero? it always points to where the name would be (0x1A) 0x04 record allocation (8 byte alignment). ������ƫ��. ������Ʋ����ڣ���˵������Դ���0��? ������ָ���������ڵ�λ��(0x1A) 0x04��¼����(8�ֽڶ���).
	// The offset, in bytes, from the start of the ATTRIBUTE_LIST structure to the attribute name. The attribute name is stored as a Unicode string.
	// ������ƫ�ƣ���ATTRIBUTE_LIST�ṹ��ʼ����������ƫ����(���ֽ�Ϊ��λ)���������洢ΪUnicode�ַ�����
	USHORT NameOffset;					
	
	// 0x08 	8 	Starting VCN: zero if the attribute is resident ��ʼVCN���������Ϊ��פ����Ϊ��
	// The lowest valid Virtual Cluster Number (VCN) of this portion of the attribute value.
	// ��ʼVCN������ֵ����һ���ֵ������Ч���⼯Ⱥ��(VCN)��
	ULONGLONG LowVcn;					

	// 0x10 	8 	Base File Reference of the attribute ���Ե��ļ��ο���
	// The file reference number of the MFT entry containing the NONRESIDENT_ATTRIBUTE structure for this portion of the attribute value.
	// ���Ե��ļ��ο��ţ���������ֵ����һ���ֵ�NONRESIDENT_ATTRIBUTE�ṹ��MFT��Ŀ���ļ����úš�
	ULONGLONG FileReferenceNumber;		

	// 0x18 	2 	Attribute Id: Each attribute has a unique identifier. ���Ա�ʶ��ÿ�����Զ���һ��Ψһ�ı�ʶ����
	// A numeric identifier for the instance of the attribute.
	// ����ʵ�������ֱ�ʶ����
	USHORT AttributeNumber;				

	// 0x1A 	2N 	Name in Unicode (if N > 0)
	// �������
	USHORT AligmentOrReserved[3];		
} ATTRIBUTE_LIST, *PATTRIBUTE_LIST;

/* 
	0x30 	  	
	$FILE_NAME
	file attribute is always resident
	ULONGLONG informations only updated, if name changes
	�ļ�������
	�ļ�����ʼ���ǳ�פ����
*/
typedef struct 
{
	// 0x00 	8 	File reference to the parent directory. ��Ŀ¼��FRN 
	// The file reference number of the directory in which the filename is entered.
	// points to a MFT Index of a directory
	// ��Ŀ¼��FRN��
	// ָ��Ŀ¼��MFT����
	ULONGLONG DirectoryFileReferenceNumber;	

	// 0x08 	8 	C Time - File Creation MFT����ʱ�� 
	// The time when the file was created in the standard time format (that is. the number of 100-nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure
	// �Ա�׼ʱ���ʽ�����ļ���ʱ��(������1601��1��1�����100��������)��
	// �˳�Աֻ���ļ�������ʱ���£����ҿ�����STANDARD_INFORMATION�ṹ��ͬ���ֶβ�ͬ��
	FILETIME CreationTime;					

	// 0x10 	8 	A Time - File Altered ���һ��MFT����ʱ�� 
	// The time when the file attributes were last changed in the standard time format (that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// �ļ��������һ���Ա�׼ʱ���ʽ���ĵ�ʱ��(����1601��1��1��������100 -������)��
	// �˳�Ա�����ļ�������ʱ�Ż���£����ҿ�����STANDARD_INFORMATION�ṹ��ͬ���ֶβ�ͬ��
	FILETIME ChangeTime;					

	// 0x18 	8 	M Time - MFT Changed ���һ��MFTд��ʱ��
	// The time when the file was last written in the standard time format (that is, the number of 100-nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// �ļ����һ���Ա�׼ʱ���ʽд���ʱ��(����1601��1��1��������100������)��
	// �˳�Ա�����ļ�������ʱ�Ż���£����ҿ�����STANDARD_INFORMATION�ṹ��ͬ���ֶβ�ͬ��
	FILETIME LastWriteTime;					

	// 0x20 	8 	R Time - File Read ���һ��MFT����ʱ�� 
	// The time when the file was last accessed in the standard time format(that is, the number of 100 - nanosecond intervals since January 1, 1601).
	// This member is only updated when the filename changesand may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// �ļ����һ���Ա�׼ʱ���ʽ���ʵ�ʱ��(����1601��1��1��������100������)��
	// �˳�Աֻ���ļ�������ʱ���£����ҿ�����STANDARD_INFORMATION�ṹ��ͬ���ֶβ�ͬ��
	FILETIME LastAccessTime;				

	// 0x28 	8 	Allocated size of the file
	// The size, in bytes, of disk space allocated to hold the attribute value.This member is only updated when the filename changes.
	// �������������ֵ�Ĵ��̿ռ�Ĵ�С�����ֽ�Ϊ��λ���˳�Ա�����ļ�������ʱ���¡�
	ULONGLONG AllocatedSize;				

	// 0x30 	8 	Real size of the file ż�����ļ���СGetFileSize��ͬ 
	// The size, in bytes, of the attribute value. This member is only updated when the filename changes.
	// ����ֵ�Ĵ�С(���ֽ�Ϊ��λ)���˳�Ա�����ļ�������ʱ���¡�
	ULONGLONG DataSize;						

	// 0x38 	4 	Flags, e.g. Directory, compressed, hidden
	// The attributes of the file.
	// This member is only updated when the filename changes and may differ from the field of the same name in the STANDARD_INFORMATION structure.
	// �ļ������ԡ�
	// �˳�Աֻ���ļ�������ʱ���£����ҿ�����STANDARD_INFORMATION�ṹ��ͬ���ֶβ�ͬ��
	ULONG FileAttributes;					

	// 0x3c 	4 	Used by EAs and Reparse
	// �������
	ULONG AligmentOrReserved;				

	// 0x40 	1 	Filename length in characters (L)
	// The length, in characters, of the filename.
	// �ļ����ĳ���, ���ַ�����Ϊ��λ��
	UCHAR NameLength;						

	// 0x41 	1 	Filename namespace: 0x01 Long 0x02 Short 0x00 Posix?
	// The type of the name.A type of zero indicates an ordinary name, a type of one indicates a long name corresponding to a short name, and a type of two indicates a short name corresponding to a long name.
	// ���Ƶ����͡�����0��ʾ��ͨ���ƣ�����1��ʾ������ƶ�Ӧ�ĳ����ƣ�����2��ʾ�볤���ƶ�Ӧ�Ķ����ơ�
	UCHAR NameType;							

	// 0x42 	2L 	File name in Unicode (not null terminated)
	// The name, in Unicode, of the file.
	// �ļ���Unicode����(���Կ��ַ���β)��
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
�ο���https://docs.microsoft.com/zh-cn/windows/desktop/api/winioctl/ns-winioctl-retrieval_pointers_buffer
�߼��غ�������ĸ��
�أ�ָ�����̵�һ��������Ŀ���ǽ������Դ�Ϊ��λ���й���
�߼��أ��Ǵ�������ص��߼�ӳ�䣬ͨ���߼����ܹ�ֱ�ӷ��ʴ���������е����ݡ�����û𳵳������������߼����൱�����ڳ��ᣬ�������൱�ڳ����д�С��ͬ�ĸ�����λ��
����أ����߼��صķ��顣ͬ���û𳵳��������������ɽ�����ֳ�1�ڻ��ڵ����Գ��ᡢӲ�Գ��ᡢӲ�����ᣬ����ؾ��൱�����Գ��ᡢӲ�Գ��ᡢӲ�����ᡣ
�߼��غ�(LCN)���߼��صı��, �൱��һ�ڳ���ı�š�
����غ�(VCN)������صı��, �൱�����Գ���ı�š�

�ṹ���£�

��StartVCN                   ��NextVCN             ��NextVCN              ��NextVCN
+-------------------------------------------------------------------------------
| LCN1 | LCN2 | LCN3 | LCN4 | LCN5 | LCN6 | LCN7 | LCN8 | LCN9 | LCN10 | ...
+-------------------------------------------------------------------------------
|           VCN1            |           VCN2     |            VCN3     | ...

typedef struct RETRIEVAL_POINTERS_BUFFER 
{
	// ����ص�����
	DWORD ExtentCount; 

	// ��ʼ����غ�
	LARGE_INTEGER StartingVcn;

	// ����أ���������ExtentCount�������Ѿ�ָ��
	struct 
	{
		// ��һ������صĿ�ʼVCN��
		// ���ֵ��ȥStartingVcn��ǰһ��NextVcn�����ǵ�ǰ����صĳ��ȣ��߼��ص���������
		// �˳�����FSCTL_MOVE_FILE���������롣
		LARGE_INTEGER NextVcn;

		// ��ǰ������еĿ�ʼ�߼��غ�LCN��
		// ���ֵ��FSCTL_MOVE_FILE���������롣
		LARGE_INTEGER Lcn; 
	} Extents[1];
} RETRIEVAL_POINTERS_BUFFER, *PRETRIEVAL_POINTERS_BUFFER;

��θ���������߼��ػ���ļ��ļ�¼��Ϣ��

1�����ÿ���߼��صĴ�С(�ֽ���)��
ULONG sectorsPerCluster, bytesPerSectors, bytesPerCluster;
GetDiskFreeSpace("C:", &sectorsPerCluster, &bytesPerSectors, NULL, NULL);
bytesPerCluster = sectorsPerCluster * bytesPerSectors; //ͨ����8 * 512 = 4096

2������ļ��Ĵ�С(�ֽ���)��
HANDLE hFile = CreateFile("H:\\$MFT::$DATA", FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
BY_HANDLE_FILE_INFORMATION fileInfo;
GetFileInformationByHandle(hFile, &fileInfo);
ULONGLONG fileSize = fileInfo.nFileSizeLow + ((ULONGLONG)fileInfo.nFileSizeHigh << 32);

3�������ļ��������߼�������
ULONG lclusterCount = (fileSize + bytesPerCluster - 1) / bytesPerCluster;

4�������ļ����������������
STARTING_VCN_INPUT_BUFFER vcnBuf;
PRETRIEVAL_POINTERS_BUFFER pOutBuf;
ULONG outBufSize, readBytes;

outBufSize = (ULONG) & (((PRETRIEVAL_POINTERS_BUFFER)0)->Extents) + lclusterCount * sizeof(pOutBuf->Extents); //consider that each cluster is in different extent
pOutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(outBufSize * sizeof(char));
memset(pOutBuf, 0x0, outBufSize * sizeof(char));

vcnBuf.StartingVcn.QuadPart = 0;
DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &vcnBuf, sizeof(vcnBuf), pOutBuf, outBufSize * sizeof(char), &readBytes, NULL);
ULONG vclusterCount = pOutBuf->ExtentCount; 

5������ÿ��������е��߼���
ULONGLONG* lcClusters = new ULONGLONG[lclusterCount]; //�߼�������
LARGE_INTEGER vcn = pOutBuf->StartingVcn;
LARGE_INTEGER lcn;
ULONG vcIndx, lcIndx, lcCount;

for (vcIndx = 0, lcIndx = 0; vcIndx < vclusterCount; ++vcIndx, ++lcIndx)
{
	lcn = pOutBuf->Extents[vcIndx].Lcn;
	for (lcCount = (ULONG)(pOutBuf->Extents[vcIndx].NextVcn.QuadPart - vcn.QuadPart); //�߼�������=NextVcn-PrevVcn
		lcCount;
		++lcIndx, --lcCount)
	{
		lcClusters[lcIndx] = lcn.QuadPart++;//�߼��غ�(lcn), �߼���ʼ���ǰ�˳���ŵġ�
	}
	vcn = pOutBuf->Extents[vcIndx].NextVcn;//��λ����һ�������
}
free(pOutBuf);
CloseHandle(hFile);

6����ÿ���߼����ж�ȡ�ļ���¼
#define CLUSTERSPERREAD 1024

BOOL bOk;
PUCHAR buffer = new UCHAR[CLUSTERSPERREAD * bytesPerCluster]; //��¼����
HANDLE hDrive = CreateFile(pDriveFile, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
LARGE_INTEGER offset;
ULONGLONG curLcn = 0;
for (lcIndx = 0; lcIndx < lclusterCount; ++lcIndx)
{
	//ȡ������صĿ�ʼ�߼��غ�
	curLcn = lcClusters[lcIndx];

	//��λ���߼���
	offset.QuadPart = curLcn * bytesPerCluster;
	SetFilePointer(hDrive, offset.LowPart, &offset.HighPart, FILE_BEGIN);

	//�����߼����е�ÿ����¼
	bOk = ReadFile(hDrive, buffer, CLUSTERSPERREAD * bytesPerCluster, &readBytes, NULL);
	
	//��buffer�н�����¼
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
