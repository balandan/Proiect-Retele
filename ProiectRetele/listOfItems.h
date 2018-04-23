#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void listOfItems(int descriptor)
{
  char buffer[2048];
  DIR *dir;
  struct dirent *file;
  dir = opendir (".");
  if(dir==NULL)
  {
  	printf("Fail to open this directory");
  	exit(1);
  }
  bzero(buffer,2048);
  while(  (file=readdir(dir))!=NULL )
  {
  	if( strcmp(file->d_name,".")!=0 && strcmp(file->d_name,"..")!=0 && strcmp(file->d_name,"client.c")!=0 &&strcmp(file->d_name,"server.c")!=0) 
  	  { printf("%s\n",file->d_name);
            strcat(buffer,file->d_name);
            strcat(buffer,"\n");
          }
  }
  write(descriptor,buffer,2048);
  closedir(dir);
 
}
