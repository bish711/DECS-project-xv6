#define MAX_INODES 128
#define MAX_BLOCKS 128
#define BLOCK_SIZE 64
#define FILENAME_LEN 10

struct tempfs_supernode
{
	int total_inodes;
	int free_inodes;
	int total_blocks;
	int free_blocks;
	int inode_bitmap[MAX_INODES];
	int block_bitmap[MAX_BLOCKS];
};

struct tempfs_inode
{
	int inode_num;
	//int is_directory;
	int size;
	int blocks[MAX_BLOCKS];
	char filename[FILENAME_LEN];
};


struct tempfs
{
	struct tempfs_supernode supernode;
	struct tempfs_inode inodes[MAX_INODES];
	//char data[MAX_BLOCKS][BLOCK_SIZE];
};

void tempfsinit(void);
int tempfscreate(const char* filename);
int tempfswrite(const char *filename, const char *data, int size);
int tempfsread(const char *filename);
int tempfsdelete(const char *filename);
int tempfsupdate(const char *filename,const char *data,int size);