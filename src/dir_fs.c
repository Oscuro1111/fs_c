#include<string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../includes/fs_c.h"
#include "../includes/string_builder.h"

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

int fs_read_dir(FS_Dir * fs,List *list){

  char dir_path[PATH_MAX];

  DIR *dirp;

 struct dirent *direntp;

 snprintf(dir_path, 1024, "%s/%s", fs->path, fs->name);

 if(is_null(list)){
   return -1;
 }


if((dirp=opendir(dir_path))==NULL){

   //print errno with string message.
    perror("Unable to open directory");
    return 1;
}

while((direntp=readdir(dirp))!=NULL){

  snprintf(dir_path, 1024, "%s/%s/%s", fs->path,fs->name,direntp->d_name);


  if(is_directory(dir_path)&&(strcmp(direntp->d_name,".")!=0)&&(strcmp(direntp->d_name,"..")!=0)){
    FS_Dir fs_dir;
   
    snprintf(dir_path, 1024, "%s/%s", fs->path,fs->name,direntp->d_name);

    fs_dir.name = direntp->d_name;
    fs_dir.path = dir_path;

    fs_read_dir(&fs_dir,list);

  }else if(is_regular_file(dir_path)){
    String *string  = allocate_string(0);
   
    insert_string(&string, dir_path);
    
    mpm_list_add(list,string);
  }
}

while((closedir(dirp)==-1)&&(errno==EINTR));

 return 0;
}


