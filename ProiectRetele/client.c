#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define mTransport "done"
#define PORT 2018
extern int errno;
int typeOfUser;

void conectat(int descriptor)
{
    char comanda[100]; 
    char nameOfFile[50];
    char messageFromServer[50];
    char msg[1024];
    char fisiere[2048];
    char realPath[100];
    char *path;
    char *command;
    char *name;
    printf("[client]Put here your command:");
    bzero(comanda,100);
    fgets(comanda,100,stdin);
    write(descriptor,comanda,100);
    command=strtok(comanda," ");//pentru a verifica daca comanda primita este de download sau este de alt tip 
    name=strtok(NULL,"\n");
    if(strcmp(command,"download")==0 && (typeOfUser==0 || typeOfUser==1))
    {
      printf("[client]Choose path for your file:");
      bzero(comanda,100);
      fgets(comanda,100,stdin);
      path=strtok(comanda,"\n");
      bzero(realPath,100);
      strcat(realPath,path);
      strcat(realPath,"/");
      bzero(messageFromServer,50);
      read(descriptor,messageFromServer,50);
      write(descriptor,mTransport,5);
      if (strcmp(messageFromServer,"The file was found!")==0)
      {
      	int n;
        read(descriptor, &n, sizeof(n));
        printf("[client]The size of file is %d%s\n",n," bytes");
        read(descriptor,nameOfFile,100);
        printf("[client]The name of file that you want:%s\n",nameOfFile);
        strcat(realPath,nameOfFile);
        int x;
        x=open(realPath,O_WRONLY|O_CREAT);
        while(n>0)
        {
         int bytes;
         bytes=read (descriptor, msg, 1024);
         write(x,msg,bytes);
         n=n-bytes;
         printf("[client]I recived %d%s%d%s\n",bytes," there are still ", n ," bytes left");
         sleep(0.1);
        }
       }
       if(strcmp(messageFromServer,"Unable to open file!")==0)
       {
       	 printf("[client]The file you wanted does not exist\n");
       }
       bzero(messageFromServer,50);
       conectat(descriptor);
    }
    else if (strcmp(comanda,"ls\n")==0 && (typeOfUser==0 || typeOfUser==1))//functia de afisare a fisierelor ce se afla pe server
    { 
      bzero(fisiere,2048);
      read(descriptor,fisiere,2048);
      if(strcmp(fisiere,"start")==0)
      {
        bzero(fisiere,2048);    
        read(descriptor,fisiere,2048);
        printf("%s",fisiere);
      }
      conectat(descriptor);
    }
    else if (strcmp(comanda,"exit\n")==0 && (typeOfUser==0 || typeOfUser==1))//exit,mai trebuie lucrat :|
    {
      char exit[20];
      bzero(exit,20);
      read(descriptor,exit,20);
      printf("[client]Message recived:%s\n",exit);
      printf("[client]:");
      bzero(exit,20);
      fgets(exit,20,stdin);
      if(strcmp(exit,"yes\n")==0)
      {
       write(descriptor,"yes",3);
       close(descriptor);//write(descriptor,"yes",3);
      }
       else if(strcmp(exit,"no\n")==0) 
      {
        write(descriptor,"no",2);
        conectat(descriptor);
      } 
    }
    else if (strcmp(command,"delete")==0 && typeOfUser==1)//functia de delete
    { 
      bzero(msg,1024);
      read(descriptor,msg,1024);
      printf("[client]%s\n",msg);
      bzero(msg,1024);
      conectat(descriptor);
    }
    else if (strcmp(command,"delete")==0 && typeOfUser==0)
   {
     printf("[client]You dont have permission to do that! Try another command!\n");
     conectat(descriptor);
   }
    else if (strcmp(command,"copy")==0 && typeOfUser==1)
    { 
      char copyFile[30];    //noul nume al fisierului copiat
      bzero(msg,1024);
      read(descriptor,msg,1024);
      if (strcmp(msg,"File found!")==0) 
        {
          printf("[client]Choose a name for the new file\n");
          printf("[client]");
          fgets(copyFile,20,stdin);
          write(descriptor,copyFile,20);
          bzero(copyFile,20);
          read(descriptor,copyFile,20);
          printf("[client]%s\n",copyFile);
          conectat(descriptor);
        }
       else if (strcmp(msg,"File not found!")==0) 
         {
          printf("[client]%s\n",msg);
          conectat(descriptor);
         }
    }
    else if (strcmp(command,"copy")==0 && typeOfUser==0)
     {
       printf("[client]You dont have permission to do that! Try another command!\n");
       conectat(descriptor);
     }
  else 
    {
      printf("[client]Unknown command!\n");
      conectat(descriptor);
    }
}
    

int port;
int loginClient(int descriptor)
{   
    char msg[100];
    char nume[100];
    char parola[100];
    char prenume[100];
    char test[50];
    char c;
    int y;
    int i=0;
    bzero(msg,100);
    printf ("[client]Username:");
    fgets(nume,100,stdin);
    printf ("[client]Password:");
    fgets(parola,100,stdin);
    strcat(msg,nume);
    while(parola[i]!='\0')
    {
      y=(int)parola[i];
      y++;
      c=y;
      test[i]=c;
      i++;
    }
    test[i]='\0';                                              //Am criptat parola
    strcat(msg,test);
    if (write (descriptor, msg, 100) <= 0)                     //Scriem datele de autentificare catre server
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if (read (descriptor, msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");    //Primim raspuns
      return errno;
    }
  printf ("[client]%s\n", msg);
  if(strcmp(msg,"Connected")==0)
  {
    read(descriptor, &typeOfUser, sizeof(typeOfUser));
    conectat(descriptor);
  }
  else if (strcmp(msg,"Try again")==0)
  {
    while(1)
   {
    printf ("[client]Type login or exit:");
    bzero(msg,100);
    fgets(msg,100,stdin);
    if (strcmp(msg,"login\n")==0) 
      {
        loginClient(descriptor);
        break;
      }
     else if (strcmp(msg,"exit\n")==0) 
      {
        close(descriptor);
        break;
      }
      else printf("[client]Unknown command!\n");
    }
  }
}



int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	
  char msg[100];		

  if (argc != 1)
    {
      printf ("Sintaxa: %s\n", argv[0]);
      return -1;
    }

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)//socketul pt client
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port =htons (PORT);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)//legam socketul creat la un socket al serverului
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  loginClient(sd);//daca conectarea a fost aprobata putem comunica prin socketul creat cu serverul
  close (sd);
}
