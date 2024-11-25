#include "tempfs.h"
#include "defs.h"
#include "types.h"
#include "memlayout.h"
#include "stat.h"


struct tempfs tempfs;


void tempfsinit()
{
	tempfs.supernode.total_inodes=MAX_INODES;
	tempfs.supernode.free_inodes=MAX_INODES;
	tempfs.supernode.total_blocks=MAX_BLOCKS;
	tempfs.supernode.free_blocks=MAX_BLOCKS;
	memset(tempfs.supernode.inode_bitmap,0,sizeof(tempfs.supernode.inode_bitmap));
	memset(tempfs.supernode.block_bitmap,0,sizeof(tempfs.supernode.block_bitmap));
	cprintf("Tempfs initialized");
}

int tempfscreate(const char *filename)
{
	int available_inode=-1;
	if(tempfs.supernode.free_inodes!=0)
	{
		for(int i=0;i<MAX_INODES;i++)
		{
			if(tempfs.supernode.inode_bitmap[i]==0)
			{
				available_inode=i;
				break;
			}
		}
	}
	if(available_inode==-1)
	{
		cprintf("No inodes available to create file");
		return -1;
	}
	tempfs.supernode.inode_bitmap[available_inode]=1;
	tempfs.supernode.free_inodes--;

	struct tempfs_inode *new_inode = &tempfs.inodes[available_inode];
	new_inode->inode_num=available_inode;
	new_inode->size=0;
	memset(new_inode->blocks,-1,sizeof(new_inode->blocks));
	memmove(new_inode->filename,filename,sizeof(filename));
	return available_inode;
}
int tempfswrite(const char *filename,const char * data, int size)
{
	int available_inode=-1;
	if(tempfs.supernode.free_inodes!=0)
	{
		for(int i=0;i<MAX_INODES;i++)
		{
			if(tempfs.supernode.inode_bitmap[i]==0)
			{
				available_inode=i;
				break;
			}
		}
	}
	if(available_inode==-1)
	{
		cprintf("No inodes available to create file");
		return -1;
	}

	int blocks_required=size/BLOCK_SIZE;
        int blocks_available=0;
        int available_blocks[MAX_BLOCKS];
     	if(tempfs.supernode.free_blocks!=0)
	{
		for(int i=0;i<MAX_BLOCKS;i++)
		{
			if(tempfs.supernode.block_bitmap[i]==0)
			{
				available_blocks[blocks_available]=1;
				blocks_available=blocks_available+1;
		
			}
		}
	}
	if(blocks_available<blocks_required) 
    	{
		cprintf("No blocks available to write to file");
	}

	tempfs.supernode.inode_bitmap[available_inode]=1;
	tempfs.supernode.free_inodes--;

	for(int i=0;i<blocks_required;i++)
        {
		tempfs.supernode.block_bitmap[available_blocks[i]]=1;
		tempfs.supernode.free_blocks--;
	}
	for(int i=0;i<blocks_required;i++)
	{
		char * page = kalloc();
		if(page==0)
		{
			cprintf("Memory is full. Couldn't write to file");
			return -1;
		}
		memmove(page,data,size);


		struct tempfs_inode *new_inode=&tempfs.inodes[available_inode];
		new_inode->inode_num=available_inode;
		new_inode->size=size;
		memmove(new_inode->filename,filename,sizeof(filename));
		new_inode->blocks[i]=V2P(page);
	}
	return 0;
}
	