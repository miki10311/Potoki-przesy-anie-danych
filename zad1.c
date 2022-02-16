#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define MaxSize 10     // maksymalna ilosc przesylanych danych

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
        while((ilosc_znakow=read(pipefd[0],buf,MaxSize))>0)   //sprawdzamy czy jest co czytac
        {
            write(1," #",2*sizeof(char));  // dziecko wypisuje to co chcemy
            write(1,buf,ilosc_znakow);
            write(1,"# ",2*sizeof(char));
        }
        close(pipefd[0]);
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