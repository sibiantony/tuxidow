/****************************************************************
 
FILE : PARTIT.H
DETAILS : DETECTS ALL LINUX PARTITIONS BY TRAVERSING THE PRIMARY
		  AND EXTENDED PARTITIONS
CONTACT : tuxidow@yahoo.com

*****************************************************************/

#include "Tuxidow.h"
#include "Ext2FS.h"
#include "diskio.h"


#ifndef __PARTIT_H
#define __PARTIT_H
#define MAX_DRIVES 32

int		numlin,flag;
int		choice,ac,partition_index;

char	partitioninfo[64][1000];
char	CURRENT_PATH[MAX_LEN];

DWORD	extlba;
WORD	CURRENT_DRIVE;
DWORD	drivelist[MAX_DRIVES];

struct PartitionEntry
{
//16 byte partition entry starting from byte 446 in MBR
//there are 4 such 16 byte partition entries

	BYTE af;//active flag
	BYTE sh;//starting head
	BYTE ssc1;//starting sector+cylinder [CCSS SSSS ---- ----]
	BYTE ssc2;//starting sector+cylinder [---- ---- CCCC CCCC]
	BYTE tb;//partition type byte
	BYTE eh;//ending head
	BYTE esc1;//ending sector+cylinder [CCSS SSSS ---- ----]
	BYTE esc2;//ending sector+cylinder [---- ---- CCCC CCCC]
	DWORD slba;//starting LBA value
	DWORD numsec;//size of partition in sectors

} ;


struct bootsector
{   
	BYTE temp446[446];
	struct PartitionEntry part[4];
	WORD signature;
	struct bootsector *next;
}*bslist,*temp;

struct peslba_struct
{
	DWORD lba;
	WORD  disk_no ;
	DWORD numsectors;
}peslba[20];


/*function prototypes*/
void ReadMBR();
void CheckIfExt2FS(struct PartitionEntry *pe,DWORD lba);
void TraversePrimaryPartitions();
void GetBootSector(DWORD lba);
void CheckIfExtended(struct PartitionEntry *pe);
void TraverseExtendedPartitions();
void FindExt2FSDrives();
#endif