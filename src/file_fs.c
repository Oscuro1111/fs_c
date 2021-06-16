#include "../includes/fs_c.h"
#include "../includes/mpm_list.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int check_fs_file(FS_File *file) {

  IS_NULL(file);
  IS_NULL(file->name);
  IS_NULL(file->path);

  return 0;
}

void fs_close_file(FS_File *file) {

  if (check_fs_file(file) != 0) {
    return;
  }

  if (file->file != NULL) {
    if (file->fd == -1) {
      fclose(file->file);
    }
  } else {
    if (file->fd != -1) {
      close(file->fd);
    }
  }

  if ((file->is_alloc == 1) && file->buffer != NULL) {
    free(file->buffer);
  }

  file->buffer = NULL;
  file->fd = -1;
  file->file = NULL;
  file->size = 0;
  file->is_alloc = 0;
}

int fs_file_size(FS_File *file) {

  struct stat statbuf;

  char file_path[1024];

  if (check_fs_file(file) != 0) {
    return -1;
  }
  snprintf(file_path, 1024, "%s/%s", file->path, file->name);
  fprintf(stderr,"file_path=%s\n",file_path);
  if (stat(file_path, &statbuf) != -1) {
    return statbuf.st_size;
  }
  return -1;
}

int fs_read_file(FS_File *file) {
  int size;
  uint32_t i;

  char ch;
  char *buffer;
  char file_path[1024];


  if (check_fs_file(file) != 0) {
    return -1;
  }


  // check for any prev alloactions
  fs_close_file(file);


  // total size of file
  if (((size = fs_file_size(file)) == -1)) {
    fprintf(stderr,"fs_size unable to work\n");
    return -1;
  }
  // joining path and file name for absolute path
  snprintf(file_path, 1024, "%s/%s", file->path, file->name);

  IS_NULL((buffer = (char *)malloc(sizeof(char) * size + 4)));


  FILE *file_stream = fopen(file_path, "r");

  if (file_stream == NULL) {
    free(buffer);
    return -1;
  }

  file->is_alloc = 1;
  file->file = file_stream;

  file->fd = -1;
  if(buffer==NULL){
    fprintf(stderr, "buffer==NJULL\n");
  }

  i=fread(buffer,sizeof(char),size,file_stream);
  
  file->buffer = buffer;
  file->size = i;

  fclose(file->file);

  file->file = NULL;

  return 0;
}

int fs_delete_file(FS_File *file) {
  char file_path[1024];

  snprintf(file_path, 1024, "%s/%s", file->path, file->name);

  fs_close_file(file);

  if (unlink(file_path) == -1) {
    fprintf(stderr, "\nunable to delete file-[%s]:ERROR:%s\n", file_path,
            strerror(errno));
    return -1;
  }

  return 0;
}

int fs_write_file(FS_File *file) {

  char file_path[1024];

  if ((check_fs_file(file) != 0) && (file->buffer == NULL) &&
      (file->size <= 0)) {
    return -1;
  }

  snprintf(file_path, 1024, "%s/%s", file->path, file->name);

  FILE *file_stream = fopen(file_path, "a+");

  if (file_stream == NULL) {
    fprintf(stderr, "%s:unable  to open file for read.\n", file_path);
    return -1;
  }

  file->file = file_stream;
  file->fd = -1;

  fwrite(file->buffer, file->size, sizeof(char), file_stream);

  fs_close_file(file);
  return 0;
}
