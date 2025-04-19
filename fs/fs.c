#include "fs.h"
static void *img;
void fs_init(void *fs_img) {
    img = fs_img;
}
int fs_ls(char *buf, int bufsize) {
    (void)buf; (void)bufsize;
    return 0;
}
int fs_cd(const char *path) {
    (void)path;
    return 0;
}