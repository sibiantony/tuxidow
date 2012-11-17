/****************************************************************
 
FILE : DISKIO.C
DETAILS : READS RAW SECTORS OF THE HARDDISK AND CHECKS THE NUMBER
		  OF HARDDISKS(MAXIMUM OF 4 IDE DRIVES)
CONTACT : tuxidow@yahoo.com

*****************************************************************/
#include "diskio.h"

#define DISK_BLOCK_SIZE  512UL					/*size of harddisk sectors in byte */
 			  
IDEDrive_struct IDEDrive[4] = {
				"\\\\.\\physicaldrive0",0,
				"\\\\.\\physicaldrive1",0,
				"\\\\.\\physicaldrive2",0,
				"\\\\.\\physicaldrive3",0
			   } ;

//Reads a disk sector from the given drive and 
// the lba address.
int ReadDiskSector(BYTE *buf,enum DISK_TYPE type,DWORD lba,WORD numblocks)
{   
   
    HANDLE hPhysicalDrive;
    DWORD dwBytesRead;
    unsigned long locLow, locHigh;
    
    hPhysicalDrive = CreateFile(IDEDrive[type].drivename,
								GENERIC_READ ,
								FILE_SHARE_READ ,
								0, OPEN_EXISTING, 0, 0);


    if (hPhysicalDrive == INVALID_HANDLE_VALUE)
    {	
		PrintError("Error opening the drive ");
		return 0;
    }
    
	locLow = lba << 9;
	locHigh = lba >> 23;

	if (SetFilePointer(hPhysicalDrive, locLow, (PLONG) & locHigh, FILE_BEGIN) == 0xFFFFFFFF)
	{
	    PrintError( "Seek failed \n" );
	    CloseHandle(hPhysicalDrive);
	    return 0;
	}
	/* Read the sectorfrom the selected drive. */

	if (!ReadFile(hPhysicalDrive, buf, numblocks * DISK_BLOCK_SIZE, &dwBytesRead, NULL))
	{
	    PrintError("Read failed  \n");
	    CloseHandle(hPhysicalDrive);
	    return 0;
	}
	CloseHandle(hPhysicalDrive);
	return 1;
		 
}

//Checks for the hard disks , a maximum
//of 4 hard disks are supported.
void CheckHardDisks()
{ 
	
	HANDLE hDevice; 			// handle to the drive to be examined 
	WORD disk_no ;
	BOOL foundDrive = FALSE;

	for(disk_no = 0 ;disk_no < 4 ; disk_no++)
	{
		hDevice = CreateFile(IDEDrive[disk_no].drivename, // drive to open 
				GENERIC_READ, 	
				FILE_SHARE_READ ,	// share mode 
				NULL, 				// default security attributes 
				OPEN_EXISTING,		// disposition 
				0,					// file attributes 
				NULL); 				// do not copy file attributes 


		if (hDevice != INVALID_HANDLE_VALUE) 
		{
			IDEDrive[disk_no].found = TRUE;
			foundDrive = TRUE ;
			DebugReport(SEPARATOR,"\nFound Hard Drive : IDE%d\n",disk_no);
		}
			
		CloseHandle(hDevice); 
		
	}
	if(!foundDrive)
		PrintError("No IDE HardDrives found \n");

}

