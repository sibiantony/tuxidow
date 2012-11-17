/****************************************************************

FILE : EXT2FS.H
DETAILS : TRAVERSES THE EXT2 FILESYSTEM , CHECKS SUPER BLOCK ,
		  CALCULATES LBA ADDRESS FROM INODE NUMBER
CONTACT : tuxidow@yahoo.com

*****************************************************************/

#include "Tuxidow.h"
#include "diskio.h"

#ifndef __EXT2FS_H
#define __EXT2FS_H

/*sector size*/
#define SECTOR_SIZE 512
#define EXT2_SUPER_OFFSET 2
#define REISERFS_SUPER_OFFSET 128
/*superblock lies at a fixed offset of 1024 B from partition start;ie 2 sectors*/
#define SUPER_BLOCK_OFFSET 2

WORD	size;
int		address_len;
char	ch,dirnam[MAX_PATH];
BYTE	buf[1024];
BYTE	groupdescr[1024];	
int		num_files ;
DWORD   present_inode , progress_mode , progress_count;
TCHAR	pathname[100][MAX_PATH];

struct filedetails
{
	char filename[156];
	DWORD inode;
	BYTE file_type;
	DWORD isize;
	char acrights[13];
}guifile[8000];

BYTE SEC_PER_BLOCK;
WORD BLOCKSIZE,TREE_HEIGHT;
DWORD ROOT_BLOCK,PART_START,FREE_BLOCKS,TOTAL_BLOCKS;
DWORD ROOT_DIR_BLOCK,CURRENT_DIR_BLOCK;
BYTE *BLOCK_BUF;
struct group *grblock;


struct ext2_inode {
	WORD	i_mode;		/* File mode */
	WORD	i_uid;		/* Owner Uid */
	DWORD	i_size;		/* Size in bytes */
	DWORD	i_atime;	/* Access time */
	DWORD	i_ctime;	/* Creation time */
	DWORD	i_mtime;	/* Modification time */
	DWORD	i_dtime;	/* Deletion Time */
	WORD	i_gid;		/* Group Id */
	WORD	i_links_count;	/* Links count */
	DWORD	i_blocks;	/* Blocks count */
	DWORD	i_flags;	/* File flags */
	union {
		struct {
			DWORD  l_i_reserved1;
		} linux1;
		struct {
			DWORD  h_i_translator;
		} hurd1;
		struct {
			DWORD  m_i_reserved1;
		} masix1;
	} osd1;				/* OS dependent 1 */
	DWORD	i_block[15];/* Pointers to blocks */
	DWORD	i_version;	/* File version (for NFS) */
	DWORD	i_file_acl;	/* File ACL */
	DWORD	i_dir_acl;	/* Directory ACL */
	DWORD	i_faddr;	/* Fragment address */
	union {
		struct {
			BYTE	l_i_frag;	/* Fragment number */
			BYTE	l_i_fsize;	/* Fragment size */
			WORD	i_pad1;
			DWORD	l_i_reserved2[2];
		} linux2;
		struct {
			BYTE	h_i_frag;	/* Fragment number */
			BYTE	h_i_fsize;	/* Fragment size */
			WORD	h_i_mode_high;
			WORD	h_i_uid_high;
			WORD	h_i_gid_high;
			DWORD	h_i_author;
		} hurd2;
		struct {
			BYTE	m_i_frag;	/* Fragment number */
			BYTE	m_i_fsize;	/* Fragment size */
			WORD	m_pad1;
			DWORD	m_i_reserved2[2];
		} masix2;
	} osd2;				/* OS dependent 2 */
} *intab[4],*intab1[4];
struct dindirect
{
 DWORD add[1024];
}*dind;


struct group
{
	DWORD block_bitmap;
	DWORD inode_bitmap;
	DWORD inode_table;
	WORD free_blocks_count;
	WORD free_inodes_count;
	WORD used_dirs_count;
	WORD pad;
	DWORD reserved[3];
}; //*grblock;
struct ext2_dir_entry_2
{
	DWORD inode;
	WORD rec_len;
	BYTE name_len;
	BYTE file_type;
	char name[255];
}*direntry,*direntry1;
struct sindirect
{
  DWORD add[1024];
}*sind;

struct tindirect
{
  DWORD add[1024];
}*tind;
struct super_block /*structure of super block*/
{
	DWORD	inodes_count;		/* Inodes count */
	DWORD	blocks_count;		/* Blocks count */
	DWORD	r_blocks_count;	/* Reserved blocks count */
	DWORD	free_blocks_count;	/* Free blocks count */
	DWORD	free_inodes_count;	/* Free inodes count*/
	DWORD	first_data_block;	/* First Data Block */
	DWORD	log_block_size;	/* Block size */
      signed long int	log_frag_size;	/* Fragment size */
	DWORD	blocks_per_group;	/* # Blocks per group */
	DWORD	frags_per_group;	/* # Fragments per group */
	DWORD	inodes_per_group;	/* # Inodes per group */
	DWORD	mtime;		/* Mount time */
	DWORD	wtime;		/* Write time */
	WORD	mnt_count;		/* Mount count */
	WORD      max_mnt_count;	/* Maximal mount count */
	WORD	magic;		/* Magic signature */
	WORD	state;		/* File system state */
	WORD	errors;		/* Behaviour when detecting errors */
	WORD	minor_rev_level; 	/* minor revision level */
	DWORD	lastcheck;		/* time of last check */
	DWORD	checkinterval;	/* max. time between checks */
	DWORD	creator_os;		/* OS */
	DWORD	rev_level;		/* Revision level */
	WORD	def_resuid;		/* Default uid for reserved blocks */
	WORD	def_resgid;		/* Default gid for reserved blocks */
	DWORD first_ino;
	WORD 	inode_size;
	WORD block_group_nr;
	DWORD feature_compat;
	DWORD feature_incompat;
	DWORD feature_ro_compat;
	BYTE uuid[16];
	char volume_name[16];
	char last_mounted[64];
	DWORD algorithm_usage_bitmap;
	BYTE prealloc_blocks;
	BYTE prealloc_dir_blocks;
	BYTE padding[818];	
}*sb;
struct reiser_super_block /*structure of super block*/
{
	DWORD block_count;//number of blocks in partition
	DWORD free_blocks;//number free blocks
	DWORD root_block;//root block number
	DWORD journal_block;
	DWORD journal_dev;
	DWORD orig_journal_size;
	DWORD journal_trans_max;
	DWORD journal_block_count;
	DWORD journal_max_batch;
	DWORD journal_max_commit_age;
	DWORD journal_max_trans_age;
	WORD blocksize;//block size
	WORD oid_maxsize;
	WORD oid_cursize;
	WORD state;//2 for dirty, 1 for clean
	char magic[12];//ReIsErFs or ReIsEr2Fs
	DWORD hash_function_code;
	WORD tree_height;//height of B+ tree
	WORD bmap_nr;//number of bit map blocks
	WORD version;
	WORD reserved;
	DWORD inode_generation;
	DWORD flags;
	BYTE uid[16];
	BYTE label[16];
	char unused[88];
}*reiser_sb;/*Size of super block is (14*4)+(8*2)+(12+16+16+88)=204 bytes*/

//function prototypes

char* PrintMode(WORD );
char* printsuper(enum DISK_TYPE ,DWORD);
void	CopyExt2Block(DWORD , FILE * , int*);
void 	CopyExt2File(DWORD ,DWORD ,char *);
void 	CopyFolder(DWORD ,DWORD );
void 	printinode(enum DISK_TYPE ,DWORD ,DWORD );
DWORD Inode_to_Lba(DWORD);
int   Get_Index(DWORD);

#endif