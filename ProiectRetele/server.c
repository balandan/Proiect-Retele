#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "functions.h"

#define PORT 2018

extern int errno;

int main ()
{
  int proces;
  struct sockaddr_in server; //structura pt server	
  struct sockaddr_in from;	 //structura pt client
  char msg[100];	       
  int sd;    //descriptor pentru socket
 // crearea unui socket 
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1) 
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  // pregatirea structurilor de date 
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)                      //asculta pentru conexiuni la adresa asignata socketului
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  while (1)
    {
      int client;
      int length = sizeof (from);
      /* acceptam un client */
      client = accept (sd, (struct sockaddr *) &from, &length); //atasam socketul creat la o structura cu datele clientului

      // eroare la acceptarea conexiunii de la un client 
      if (client < 0)
	    {
	      perror ("[server]Eroare la accept().\n");
	       continue;
	    }
      pid_t proces=fork();
      if(proces==0)
      { 
      	printf("[server]A new client has come online!\n");
        login(client);	
      }
    }
}		
			
