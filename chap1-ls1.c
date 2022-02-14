#include "apue.h"
#include "error.c"
#include <dirent.h>
/*
3、DIR *opendir(const char *name);
     –参数：目录的路径。
     –返回值：成功返回指向目录流的指针，错误返回NULL
4、int closedir(DIR *dirp);
     –参数：opendir 返回的dir 指针
     –返回值：成功返回0， 失败返回-1
3、读目录信息函数
     struct dirent *readdir(DIR *dirp);
     –参数dirp：opendir 函数打开目录后返回的文件指针。
     –返回值：成功返回指向dirp 的指针dirent ，错误返回NULL。
*/
//struct dirent {
 //      ino_t          d_ino;       /* inode number */ 
//      off_t          d_off;       /* not an offset; see NOTES */
//       unsigned short d_reclen;    /* length of this record */
//       unsigned char  d_type;      /* type of file; not supported
//                                              by all filesystem types */
//       char           d_name[256]; /* filename */} //
void main(int argc, char *argv[])
{
    DIR* dp;
    struct dirent* dirp;
    
    if (argc != 2)
    {
      err_quit("usage: ls directory_name");
    }
		
    
    if ((dp=opendir(argv[1]))==NULL)
    {
        err_sys("can't open %s", argv[1]);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        printf("%s\n", dirp->d_name);
    }
    


}