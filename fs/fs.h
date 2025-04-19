#ifndef FS_H
#define FS_H

typedef struct {
    char name[16];
    unsigned int size;
    unsigned int first_block;
} dir_entry;

void fs_init(void *fs_img);
int fs_ls(char *buf, int bufsize);
int fs_cd(const char *path);
#endif