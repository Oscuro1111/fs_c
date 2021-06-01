#include<string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../includes/fs_c.h"


int is_directory(char *path){
  struct stat statbuf;

  if(stat(path,&statbuf)==-1){
    return 0;
  }else{
    return S_ISDIR(statbuf.st_mode);
  }
}


int is_regular_file(char *path) {

  struct stat statbuf;

  if (stat(path, &statbuf) == -1) {
    return 0;
  } else {
    return S_ISREG(statbuf.st_mode);
  }
}

int fs_read_dir(FS_Dir * fs){

  char dir_path[PATH_MAX];

  DIR *dirp;

 struct dirent *direntp;

 snprintf(dir_path, 1024, "%s/%s", fs->path, fs->name);

if((dirp=opendir(dir_path))==NULL){
    perror("Unable to open directory");

    fprintf(stderr,"Unable to open directory");
    return 1;
}

while((direntp=readdir(dirp))!=NULL){

  snprintf(dir_path, 1024, "%s/%s/%s", fs->path,fs->name,direntp->d_name);

  if(is_directory(dir_path)){
    printf("%s\n",direntp->d_name);
  }else if(is_regular_file(dir_path)){
    printf("%s\n", direntp->d_name);
  }
}

 while((closedir(dirp)==-1)&&(errno==EINTR));

 return 0;
}


