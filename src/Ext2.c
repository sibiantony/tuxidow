/****************************************************************

FILE : EXT2.C
DETAILS : TRAVERSES THE EXT2 FILESYSTEM , CHECKS SUPER BLOCK ,
		  CALCULATES LBA ADDRESS FROM INODE NUMBER
CONTACT : tuxidow@yahoo.com

*****************************************************************/

#include "Ext2FS.h"

//Get the access rights of owner,
//user or group,from the i_mode field of the inode table.
char* PrintMode(WORD m)
{
	WORD iss,mask;
	char *accrights;
	char table[]="USGrwxrwxrwx";

	mask=0x0800;
	accrights = (char*)malloc(13);
 
	for(iss=0;iss<12;iss++)
    {
		accrights[iss]=m&mask?table[iss]:'-';
		mask>>=1;
    }
	accrights[12]='\0';
	return(accrights);
}

//Checks the super block of both the Ext2 as well
//as Reiser filesystems.
char * printsuper(enum DISK_TYPE disk,DWORD peslb)
{
	int l=0;
	if(ReadDiskSector(buf,disk,peslb+EXT2_SUPER_OFFSET,2)==FALSE)
		PrintError("Can not read disk sector");
	sb = (struct super_block *)malloc(sizeof(struct super_block));
 	sb = (struct super_block *)buf;

	if(sb->magic==0xEF53) 
	{
		strcpy(pathname[0],sb->volume_name);
		l = (int)strlen(pathname[0]);
		if(pathname[0][l-1]!='/')
		{
			pathname[0][l]='/';
			pathname[0][l+1]='\0';
		}
		address_len = (int)strlen(pathname[0]);
		size = (WORD)pow(2,10+sb->log_block_size);

		DebugReport(SEPARATOR,"\nVolume Name : %s , Block Size :%d\n",
			sb->volume_name,size);
		DebugReport(0,"Inodes Count :%d,  Block Count: %d\n",
			sb->inodes_count,sb->blocks_count);
		DebugReport(0,"Inodes per group :%d ,Blocks per group :%d\n",
			sb->inodes_per_group,sb->blocks_per_group);
		DebugReport(0,"Revision level :%d , Minor revision level :%d\n",
			sb->minor_rev_level,sb->rev_level);

		return sb->volume_name;
	}
	else
	{
		char magic[13],buf[512];

		reiser_sb = (struct reiser_super_block *)malloc(sizeof(struct reiser_super_block));
		if(ReadDiskSector(buf,disk,peslb+REISERFS_SUPER_OFFSET,1)==FALSE)
			PrintError("Can not read disk sector");
		reiser_sb=(struct reiser_super_block *)buf;
		wsprintf(magic,"%s",reiser_sb->magic);
		magic[12]='\0';
		if(strcmp(magic,"ReIsErFs")==0 || strcmp(magic,"ReIsEr2Fs")==0)
		{
			DebugReport(0,"ReiserFS with magic word: %s\n",
				reiser_sb->magic);
			return ("ReiserFS");
  		}
	}
	return("No support");
}

//Converts an inode number to a LBA address.

DWORD Inode_to_Lba(DWORD inode)
{
	int i,j,small_size;
	i=inode%sb->inodes_per_group;	
	j=i;   
	inode=inode/sb->inodes_per_group;
	small_size=size/128;
	if(inode==0||inode==1||inode==3||inode==9||inode==27||
		inode==5||inode==25||inode==7||inode==49||inode==81)
	{
		inode = (inode*sb->blocks_per_group*small_size + (j-1) + grblock->inode_table*small_size)/4;
	}
	else
	{
		inode = (inode*sb->blocks_per_group*small_size + (j-1) + 2*small_size)/4;
		if(size==1024) inode+=2;
	}
	return(inode);
}

int Get_Index(DWORD inode)
{
	int i,j;
	i = inode%sb->inodes_per_group;	
	j = (i-1)%4;
	return(j);
}

//Saves a block of data to a file.
//the blocksize depends on the size of the partition.

void CopyExt2Block(DWORD block_address , FILE *outfile , int *blocksize)
{
	
	char dataread[4096];

	if(ReadDiskSector(dataread ,disk ,block_address,size/512)==FALSE)
			PrintError("Can not read disk sector");
	progress_count += 1;
	if(progress_count==progress_mode) 	
	{
		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
		progress_count =0;
	}

	if(*blocksize > size)
	{
		fwrite((char *)&dataread,size,1,outfile);
		*blocksize = *blocksize - size;
	}
	else
		fwrite((char *)&dataread,(*blocksize),1,outfile);
}

//
//Save a file from the partition to a file.
//Checks all the singly, doubly and triply indirect 
//blocks and if allocated , calls the CopyExt2Block() function.
//
void CopyExt2File(DWORD inode,DWORD peslb,char * filename)
{
	FILE * outfile;
	int small_size=0,i;
	char *inodebuf ;       // to store the inode info of a file or folder		
	BYTE sindbuf[4096],dindbuf[4096],tindbuf[4096];
	WORD j , k ,sindblock;
	DWORD m,inodesam , fil_si ;
	struct ext2_inode *intab;
	struct tindirect  *tind;

	progress_count = 0;
	inodebuf = (char*)malloc(512);
	grblock = (struct group *)groupdescr;
	inodesam = Inode_to_Lba(inode);

	if(ReadDiskSector(inodebuf,disk,peslb+inodesam,1)==FALSE)
		PrintError("Can not read disk sector");
	i = Get_Index(inode);
	intab = (struct ext2_inode *)&inodebuf[i*128];
 	progress_mode = ((intab->i_size/PROGRESS_RANGE)/size);
	fil_si = intab->i_size;
	if((outfile = fopen(filename,"wb"))==NULL)
	{
		return;
	}
		/********** Copying Direct Blocks *********/
	for(m=0;m<12;m++)
	{
		if(intab->i_block[m]!=0)
			CopyExt2Block(peslb+(size/512)*intab->i_block[m],outfile ,&fil_si);
    }

		/********* Copying Singly Indirect Block ********/
	if(intab->i_block[12]!=0)
    {
		sindblock=size/4;
		if(ReadDiskSector(sindbuf,disk,peslb+(size/512)*intab->i_block[12],size/512)==FALSE)
			PrintError("Can not read disk sector");
		sind=(struct sindirect*)sindbuf;	//number of single indirect blocks
		for(j=0;j<sindblock;j++)
		{
			if(sind->add[j]!=0)
				CopyExt2Block(peslb+(size/512)*sind->add[j],outfile ,&fil_si);
		}
    }
	   
		/********** Copying Doubly Indirect Block *********/
	if(intab->i_block[13]!=0)
    {
		if(ReadDiskSector(dindbuf,disk,peslb+(size/512)*intab->i_block[13],size/512)==FALSE)
			PrintError("Can not read disk sector");
		dind = (struct dindirect*)dindbuf;
		for(j=0;j<sindblock;j++)
		{
			if(dind->add[j]!=0)
			{
				if(ReadDiskSector(sindbuf,disk,peslb+(size/512)*dind->add[j],size/512)==FALSE)
					PrintError("Can not read disk sector");
				sind = (struct sindirect*)sindbuf;
				for(k=0;k<sindblock;k++)
					if(sind->add[k]!=0)	
						CopyExt2Block(peslb+(size/512)*sind->add[k],
													outfile,&fil_si);
			}
		}
    }

		/********** Copying Triply Indirect Block ********/
	if(intab->i_block[14]!=0)
    {
		if(ReadDiskSector(tindbuf,disk,peslb+(size/512)*intab->i_block[14],size/512)==FALSE)
			PrintError("Can not read disk sector");

		tind = (struct tindirect*)tindbuf;
		for(m=0;m<sindblock;m++)
		{
			if(tind->add[m]!=0)
			{
				if(ReadDiskSector(dindbuf,disk,peslb+(size/512)*tind->add[m],size/512)==FALSE)
					PrintError("Can not read disk sector");

				dind = (struct dindirect*)dindbuf;
				for(j=0;j<sindblock;j++)
				{
					if(dind->add[j]!=0)
					{
						if(ReadDiskSector(sindbuf,disk,peslb+(size/512)*dind->add[j],size/512)==FALSE)
							PrintError("Can not read disk sector");
						sind = (struct sindirect*)sindbuf;
						for(k=0;k<sindblock;k++)
							if(sind->add[k]!=0)
								CopyExt2Block(peslb+(size/512)*sind->add[k],
															outfile ,&fil_si);		
					}	
				}  
			}
		}   
	}
	fclose(outfile);
	free(inodebuf);
}
//
//Copy a folder.This function makes recursive calls
// and once a regular file is encountered it calls 
// the CopyExt2File() function.
//
void CopyFolder(DWORD inode,DWORD peslb)
{
	int tdir =0;
	DWORD  prev_rec_len =0 ;
	BYTE inodebuf[512];
	struct ext2_inode *intab;
	BYTE dirbuf[4096];
	int i,k;
	DWORD inodesam;
	grblock = (struct group *)groupdescr;
	inodesam=Inode_to_Lba(inode);
	if(ReadDiskSector(inodebuf,disk,peslb+inodesam,1)==FALSE)
		PrintError("Can not read disk sector");
	i=Get_Index(inode);
    intab=(struct ext2_inode *)&inodebuf[i*128];

    for(k=0;k<12;k++)
    {  
		if(intab->i_block[k]!=0)
		{  
			prev_rec_len=0;
			do{
				strcpy(dirnam,"");
				if(ReadDiskSector(dirbuf,disk,peslb+(size/512)*intab->i_block[k],size/512)==FALSE)
					MessageBox(NULL , "Cannot Read Disk Sector ", "Error" , MB_OK);
				direntry=(struct ext2_dir_entry_2 *)&dirbuf[prev_rec_len];
				if(direntry->file_type==1)
				{
					strncpy(dirnam,direntry->name,direntry->name_len);
					dirnam[direntry->name_len]='\0';
		  
					CopyExt2File(direntry->inode,peslb,dirnam);
				}
				if(direntry->file_type==2 && strcmp(direntry->name,".") && strcmp(direntry->name,".."))
				{
					strncpy(dirnam,direntry->name,direntry->name_len);
					dirnam[direntry->name_len]='\0';
					_mkdir(dirnam);
					_chdir(dirnam);
					tdir=direntry->rec_len;
					CopyFolder(direntry->inode,peslb);
					_chdir("..");
					direntry->rec_len=tdir;
				}
				if(direntry->rec_len+prev_rec_len>=size) break;
					prev_rec_len = direntry->rec_len+prev_rec_len;
			}while(direntry->rec_len>0);
		}
    }
    return;
}

//
//Traverse a directory.The data blocks are 
//mapped to the directory entry strcuture and 
//assign the details of the files into the guifile structure.
//
void printinode(enum DISK_TYPE disk,DWORD peslb,DWORD inode)
{   
	int k=0,i,num=0,small_size=0;
	WORD sindblock ;
	DWORD prev_rec_len=0;
	int count =0,isam;
	BYTE inodebuf[512],indbuf[512];
	BYTE sindbuf[4096];
	BYTE dirbuf[4097];
	DWORD past=0,inodesam;
	DWORD n=0,m=0,j;

	if((size==1024) || (size == 2048))
		n=4;
	else if(size==4096)
		n=8;

	
	if(ReadDiskSector(groupdescr,disk,peslb+n,1)==FALSE) 
		PrintError("Can not read disk sector");
	grblock=(struct group *)malloc(sizeof(struct group));
	grblock=(struct group *)groupdescr;
	do
	{
		inodesam=Inode_to_Lba(inode);
		if(ReadDiskSector(inodebuf,disk,peslb+inodesam,1)==FALSE)
			PrintError("Can not read disk sector");
		sb = (struct super_block *)buf;
		i=Get_Index(inode);
		intab[i]=(struct ext2_inode *)&inodebuf[i*128];
		k=(intab[i]->i_mode&0xF000)>>12 ;
   
	 /********************READING DIRECTORY ENTRY **********************/
	 //printf("******DIRECT DATA BLOCKS\n");
		for(k=0;k<12;k++)
		{
	
			if(intab[i]->i_block[k]!=0)
			{
				prev_rec_len=0;
				if(ReadDiskSector(dirbuf,disk,peslb+(size/512)*intab[i]->i_block[k],size/512)==FALSE)
				{
					MessageBox(NULL , "Cannot Read Disk Sector ", "Error" , MB_OK);
					exit(0);
					return;
				}
				do
				{
					direntry=(struct ext2_dir_entry_2*)&dirbuf[prev_rec_len];			 
					strcpy(dirbuf,"");
					if(strcmp(direntry->name,".")==0){ present_inode=direntry->inode;goto da;}
					if(strcmp(direntry->name,"..")==0){ past=direntry->inode;}
					for(j=0;j<direntry->name_len;j++)
					{
						guifile[count].filename[j] = direntry->name[j];
					}
					guifile[count].filename[direntry->name_len]='\0';
					guifile[count].inode = direntry->inode;
					inodesam=Inode_to_Lba(direntry->inode);
					if(ReadDiskSector(indbuf,disk,peslb+inodesam,1)==FALSE)
						PrintError("Can not read disk sector");
					isam = Get_Index(direntry->inode);
					intab1[isam]=(struct ext2_inode *)&indbuf[isam*128];
					guifile[count].isize=intab1[isam]->i_size;
					guifile[count].file_type=direntry->file_type;
					strncpy(guifile[count].acrights,PrintMode(intab1[isam]->i_mode),13);
					count++;
			da:
					if(direntry->rec_len+prev_rec_len>=size) goto a;
					prev_rec_len = direntry->rec_len+prev_rec_len;
					
				} while(direntry->rec_len>0);
			}a: ;
		}
	 //*********************READING DATA BLOCK ************************
	 //singly indirect block 
       
	 
		if(intab[i]->i_block[12]!=0)
		{ 
			sindblock=size/4;
			if(ReadDiskSector(sindbuf,disk,peslb+(size/512)*intab[i]->i_block[12],size/512)==FALSE)
				PrintError("Can not read disk sector");
			sind=(struct sindirect*)sindbuf;
			for(j=0;j<sindblock;j++)
			{     
				prev_rec_len=0;
				if(sind->add[j]!=0)
				{
					if(ReadDiskSector(dirbuf,disk,peslb+(size/512)*sind->add[j],8)==FALSE)
						PrintError("Can not read disk sector");
					do
					{
						direntry=(struct ext2_dir_entry_2*)&dirbuf[prev_rec_len];
						strcpy(dirbuf,"");
						direntry->name[direntry->name_len]='\0';
						strcpy(guifile[count].filename,direntry->name);
						guifile[count].inode = direntry->inode;
						inodesam=Inode_to_Lba(direntry->inode);
						if(ReadDiskSector(indbuf,disk,peslb+inodesam,1)==FALSE)
							PrintError("Can not read disk sector");
						isam = Get_Index(direntry->inode);
						intab1[isam]=(struct ext2_inode *)&indbuf[isam*128];
						guifile[count].file_type=direntry->file_type;
						strncpy(guifile[count].acrights,PrintMode(intab1[isam]->i_mode),13);
						count++;	
						if(direntry->rec_len+prev_rec_len>=size) goto b;
						prev_rec_len = direntry->rec_len+prev_rec_len;

					} while(direntry->rec_len>0);
				}
			b:;	 
			}
	 
		}
	 num_files = count;
	 	
	}while(ch=='y');
}
