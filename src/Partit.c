/****************************************************************
 
FILE : PARTIT.C
DETAILS : DETECTS ALL LINUX PARTITIONS BY TRAVERSING THE PRIMARY
		  AND EXTENDED PARTITIONS
CONTACT : tuxidow@yahoo.com

*****************************************************************/
#include "partit.h"

extern int numlin = 0;
extern int flag = 0;
extern int partition_index = 0;

// Read the MBR.The first 446 bytes are skipped.
// The next 64 bytes contains the partition table.
// and the last 2 bytes is the magic word "AA55"
void ReadMBR()
{
	
	BYTE buf[512],i;
	bslist=(struct bootsector *)malloc(sizeof(struct bootsector));
	bslist->next=NULL;
	
	ReadDiskSector(buf,disk,0,1);
	for(i=0;i<4;i++)
		bslist->part[i] = *((struct PartitionEntry*)&buf[446 + i*16]);
    	
	bslist->signature=*((WORD *)&buf[510]);


}

void CheckIfExt2FS(struct PartitionEntry *pe,DWORD lba)
{
/*
	0x82 Linux swap
	0x83 Linux
	0x93 Linux Fedora Core Distro
	0x85 Linux extended

*/ 

	switch(pe->tb)
	{
		case 0x0C:
					strcpy(partitioninfo[partition_index++],"FAT32 ( Primary )");
					break;
		case 0x0B:
					strcpy(partitioninfo[partition_index++],"FAT32");
					break;
		case 0x82:	
					strcpy(partitioninfo[partition_index++],"Linux Swap");
					break;
		case 0x83:case 0x93:
					strcpy(partitioninfo[partition_index++],"Linux Partition");
					break;
		case 0x05:case 0x0F:
					strcpy(partitioninfo[partition_index++],"Extended Partition");
					break;
	}

	
	if(pe->tb==0x83||pe->tb==0x93)
	{
		peslba[numlin].lba=lba+pe->slba;
		peslba[numlin].disk_no = disk;
		peslba[numlin].numsectors = pe->numsec;		
		numlin++;
	}

}

//Traverse the 4 primary partitions.

void TraversePrimaryPartitions()
{
	
	if(bslist->signature==0xAA55)
	{   

	    CheckIfExt2FS(&bslist->part[0],0);
		CheckIfExt2FS(&bslist->part[1],0);
		CheckIfExt2FS(&bslist->part[2],0);
		CheckIfExt2FS(&bslist->part[3],0);
	}
	else
		PrintError("\nBad MBR signature...");

}

//Read the bootsectors and check whether 
//the signature is valid.
void GetBootSector(DWORD lba)
{
	BYTE buf[512],i;
	temp->next=(struct bootsector *)malloc(sizeof(struct bootsector));
	temp=temp->next;
	temp->next=NULL;
	if(ReadDiskSector(buf,disk,lba,1)==TRUE)
	{
          for(i=0;i<4;i++)
			temp->part[i]=*((struct PartitionEntry*)&buf[446 + i*16]);
		  
          temp->signature=*((WORD *)&buf[510]);
		if(temp->signature==0xAA55)
			CheckIfExt2FS(&temp->part[0],lba);
		else
			PrintError("\nBad signature...");
	}
	else
		PrintError("\n ReadSector failed ");
	if(temp->part[1].tb)
		GetBootSector(temp->part[1].slba+extlba);

}

void CheckIfExtended(struct PartitionEntry *pe)
{
	if(pe->tb==0x05 || pe->tb==0x0F || pe->tb==0x85)
	{
		extlba=pe->slba;
		GetBootSector(extlba);
	}
}

void TraverseExtendedPartitions()
{
	
	temp=bslist;
	CheckIfExtended(&temp->part[0]);
	CheckIfExtended(&temp->part[1]);
	CheckIfExtended(&temp->part[2]);
	CheckIfExtended(&temp->part[3]);
}

void FindExt2FSDrives()
{
	WORD disk_no ;
	for(disk_no =0; disk_no < 4;disk_no++)
	{
		if(IDEDrive[disk_no].found)
		{
			disk = disk_no;
			ReadMBR();
			TraversePrimaryPartitions();
			TraverseExtendedPartitions();
		}
	}
	if(numlin == 0)
	{
		PrintError("No Linux partition found on disk\n");
	}
		
}


