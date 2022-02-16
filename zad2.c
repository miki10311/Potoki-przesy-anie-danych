#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define MaxSize 256     // maksymalna ilosc przesylanych danych

int main(int argc,char *argv[])
{
    int pipefd[2];         // zmienna potrzebna do potoku
    int filefd;            // zmienna służąca do otwarcia pliku
    int ilosc_znakow;      // zmianna pomagajaca w zliczaniu znakow do jadnej paczki
    char buf[MaxSize];     // tworzymy bufor na znaki
    
    pipe(pipefd);   // tworzymy potok
    pid_t pid = fork();  // uruchamiamy podproces


    if (pid==0) // proces dziecko
    {
        close(pipefd[1]);
        close(0);
        dup(pipefd[0]); // duplikuje się file deskryptor
        close(pipefd[0]);

        execlp("display", "display", "-", NULL);
    }
    else if (pid>0) // proces rodzic
    {
        close(pipefd[0]);
        filefd=open(argv[1],O_RDONLY); // otwieramy plik tekstowy

        while((ilosc_znakow=read(filefd,buf,MaxSize))>0)  // sprawdzamy czy jest co czytac
        {
            write(pipefd[1],buf,ilosc_znakow);   // wysyłamy kolejne paczki danych do dziecka
        }

        close(pipefd[1]);
    }
}