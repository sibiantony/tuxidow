/****************************************************************
 
FILE : DISKIO.H
DETAILS : READS RAW SECTORS OF THE HARDDISK AND CHECKS THE NUMBER
		  OF HARDDISKS(MAXIMUM OF 4 IDE DRIVES)
CONTACT : tuxidow@yahoo.com

*****************************************************************/
#include "Tuxidow.h"

#ifndef __DISKIO_H
#define __DISKIO_H

enum DISK_TYPE{IDE0 , IDE1 , IDE2 , IDE3};
enum DISK_TYPE disk ;

typedef struct 
{
	char drivename[20];
	BOOL found ;
}IDEDrive_struct ;

IDEDrive_struct IDEDrive[4];

/*function prototypes*/
int ReadDiskSector(BYTE *,enum DISK_TYPE ,DWORD ,WORD);
void CheckHardDisks();


#endif