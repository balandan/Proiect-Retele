#include <stdio.h>
#include "listOfItems.h"


int typeOfUser;

int searchInServer(int descriptor,char fileName[30])
{
  DIR *dir;
  struct dirent *file;
  dir = opendir (".");
  if(dir==NULL)
  {
    printf("Fail to open this directory");
    exit(1);
  }
  while(  (file=readdir(dir))!=NULL )
  {
    if( strcmp(file->d_name,fileName)==0) 
    {
      closedir(dir);
      return 1;
    }
  }
  closedir(dir);
  return 0;
 }

int download(char nameOfFile[30],int descriptor)
{


    int ptr_myfile;
    int counter=0;
    char buffer[1024];
    char mesaj[100];
    bzero(mesaj,100);
    char messageFromClient[50];
    ptr_myfile=open(nameOfFile,O_RDONLY);
    if (ptr_myfile < 0)
    {
      strcat(mesaj,"Unable to open file!");
      //printf("%s",mesaj);
      //write(descriptor,"Unable to open file!",30);
    }
    else strcat(mesaj,"The file was found!");
    //printf("%d\n",bytes);
    //printf("%d",counter);
    printf("[server]%s\n",mesaj);
    write(descriptor,mesaj,35);
    bzero(messageFromClient,50);
    read(descriptor,messageFromClient,50);
    if(strcmp(messageFromClient,"done")==0 && strcmp(mesaj,"The file was found!")==0)
    {
      int bytes;
      while ((bytes=read(ptr_myfile,buffer,1024))>0)
      {
           
        counter+=bytes;
        //printf("%d\n",bytes);
      }
      int n = counter;
      write(descriptor, &n, sizeof(n));
      close(ptr_myfile);
      write(descriptor,nameOfFile,100);
      ptr_myfile=open(nameOfFile,O_RDONLY);
      while (n>0)
       {
          bytes=read(ptr_myfile,buffer,1024);
          n=n-bytes;
          printf("[server]We sent %d%s%d%s\n",bytes," there are still ", n ," bytes to send");
          sleep(0.1);
          write(descriptor,buffer,bytes);
        //printf("[server]We sent %d%s"," bytes");
       }
    }
    bzero(messageFromClient,50);
    close(ptr_myfile);

}
void primireComenzi(int descriptor) 
{
  char message[100];
  char *firstWord;
  char *secondWord;
  char path[100];
  read(descriptor,message,100);
  printf("[server]The message recived:%s",message);  //primim comanda
  //printf("%s",firstWord);
  if(strcmp(message,"ls\n")==0 && (typeOfUser==0 || typeOfUser==1)) //comanda pentru ls 
    {
      write(descriptor,"start",6);
      listOfItems(descriptor);
      primireComenzi(descriptor);
    }
  else if (strcmp(message,"exit\n")==0 && (typeOfUser==0 || typeOfUser==1))
  {
    char answer[5];
    write(descriptor,"Are you sure?",15);
    printf ("[server]Message sent:%s\n","Are you sure?\n");
    read(descriptor,answer,5);
    if(strcmp(answer,"yes")==0) {
      printf("[server]A client has disconnected\n");
      close(descriptor);
      exit(0);
    }
    else primireComenzi(descriptor);
  }
  firstWord=strtok(message," ");
  secondWord=strtok(NULL,"\n");
  if(strcmp(firstWord,"download")==0 && (typeOfUser==0 || typeOfUser==1)) 
  {
    printf("[server]The file that the client ask for:%s\n",secondWord);
    //read(descriptor,path,100);
    //printf("%s",path);
    download(secondWord,descriptor);
    primireComenzi(descriptor);
  }
  else if (strcmp(firstWord,"delete")==0 && typeOfUser==1)
  {
    //printf("[server]Message recived:delete %s",secondWord);
    if(remove(secondWord)==0) write(descriptor,"The file was deleted!",30);
     else write(descriptor,"The file can't be deleted or do not exist",50);
    primireComenzi(descriptor);
  }
  else if (strcmp(firstWord,"delete")==0 && typeOfUser==0)
  {
    primireComenzi(descriptor);
  }
  else if (strcmp(firstWord,"copy")==0 && typeOfUser==1)
  {
     int x=searchInServer(descriptor,secondWord);
     if (x==1)
     {
        char nameForTheNewFile[30];
        char buffer[1024];
        char file[30];//numele final al fisierului pe care il deschidem
        char theFileWeOpen[30];
        char *extension;//folosim pentru a afla extensia noului fisier
        char *nume;//folosim pentru a afla denumirea noului fisier
        int openFile;
        write(descriptor,"File found!",15);
        printf("%s\n","[server]File found!");
        read(descriptor,nameForTheNewFile,30);
        bzero(theFileWeOpen,30);
        strcat(theFileWeOpen,secondWord);
        extension=strtok(secondWord,".");
        extension=strtok(NULL,"\n");
        nume=strtok(nameForTheNewFile,"\n");
        bzero(file,30);
        strcat(file,nume);
        strcat(file,".");
        strcat(file,extension);
        openFile=open(theFileWeOpen,O_RDONLY);
        if (openFile < 0)
        {
          printf("%s\n","[server]Unable to open file!");
        }
        int x;
        x=open(file,O_WRONLY|O_CREAT);
        int bytes=0;
        while ((bytes=read(openFile,buffer,1024))>0)
        {
           write(x,buffer,bytes);
        }
        close(x);
        close(openFile);
        printf("[server]The file was copied!\n");
        write(descriptor,"Done",5);
        primireComenzi(descriptor);

  }
      else 
       {
         write(descriptor,"File not found!",15);
         printf("%s\n","[server]File not found!");
         primireComenzi(descriptor);
       }
  }
  else if (strcmp(firstWord,"copy")==0 && typeOfUser==0)
  {
    primireComenzi(descriptor);
  }
  else primireComenzi(descriptor);
  //primireComenzi(descriptor);

}
int searchInFile(char user[20],char pass[20])
{
  int nr_linie=0;
  int nr_linie2=0;
  char buffer[20];
  FILE *a;
  a=fopen("user.txt","r");
  if (a!=NULL)
  { 
    while(!feof(a))
    { 
      fgets(buffer,20,a);
      nr_linie+=1;
      if (strcmp(buffer,user)==0) 
      { 
        break;
      }
      bzero(buffer,20);
    }
    fclose(a);
    if(nr_linie %2==1)
    {
      a=fopen("user.txt","r");
      while(!feof(a))
      {
        fgets(buffer,20,a);
        nr_linie2+=1;
        if(nr_linie2==nr_linie+1 && strcmp(buffer,pass)==0) return 1;
        bzero(buffer,20);
      }
    }
   }
  return 0;
}
int typeUser (char username[30])
{ 
  char buffer[30];
  FILE *b;
  b=fopen("whitelist.txt","r");
  if(b!=NULL)
  {
    while(!feof(b))
    {
       fgets(buffer,30,b);
       if(strcmp(buffer,username)==0) return 1;
    }
  }
  fclose(b);
  b=fopen("blacklist.txt","r");
  if(b!=NULL)
  {
    while(!feof(b))
    {
       fgets(buffer,30,b);
       if(strcmp(buffer,username)==0) return 0;
    }
  }
  fclose(b);
}
void login (int descriptor)
{       
        char *username;
        char *password;
        char buffer[100];  
        char msg[100];      //mesajul primit de la client
        int i=0;
        int y;
        char c;
        char newPassword[50]; 
        int bytes;
        bytes = read (descriptor, msg, sizeof (buffer));  //Citim datele de autentificare
        if (bytes < 0)
       {
         perror ("Eroare la read() de la client.\n");
         close(descriptor);
         exit(0);
       }
       printf ("[server]Message recived:%s\n", msg);
       username = strtok(msg,"\n");                       //Le prelucram
       password = strtok(NULL,"\n");
       while(password[i]!='\0')
      {
       y=(int)password[i];
       y--;
       c=y;
       newPassword[i]=c;
       i++;
      }
       newPassword[i]='\0';                              //Am decriptat parola
       char spatiu[3]="\n";
       char nume[20];
       bzero(nume,20);
       strcat(nume,username);
       strcat(nume,spatiu);
       int x=searchInFile(nume,newPassword);             //cautam in fisierul de logare daca datele primite sunt valide
       if(x){
              bzero(msg,100);
              strcat(msg,"Connected");
              printf ("[server]Message sent:%s\n",msg);
              typeOfUser=typeUser(nume);                 //daca sunt valide determinam din ce fisier face parte userul conectat
              //printf("%d",typeOfUser);
            }
       else
      {
       bzero(msg,100);
       strcat(msg,"Try again");
       printf ("[server]Message sent:%s\n",msg); 
      }
       if (bytes && write (descriptor, msg, bytes) < 0)      //trimitem mesaj de raspuns
      {
        perror ("[server] Eroare la write() catre client.\n");
        //return 0;
      }
      if (strcmp(msg,"Connected")==0)                        //daca mesajul trimis este connected apelam functia unde putem primi comenzi
      {
        write(descriptor, &typeOfUser, sizeof(typeOfUser));//scriem clientului tipul de user ce tocmai s-a logat
        //printf("%d",typeOfUser);
        primireComenzi(descriptor);
      } 
      else if(strcmp(msg,"Try again")==0)         //daca datele sunt gresite se ofera posibilitatea de a incerca de nou
      {
        login(descriptor);
      }
      close(descriptor);
      exit(0);
}
