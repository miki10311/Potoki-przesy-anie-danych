#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define potokfifo "FIFO"  // nazwa potoku FIFO
#define MaxSize 16        // maksymalna ilosc przesylanych danych

int main(int argc,char *argv[])
{
    int pipefd;           // zmienna potrzebna do potoku
    int filefd;           // zmienna służąca do otwarcia pliku
    int ilosc_znakow;     // zmianna pomagajaca w zliczaniu znakow do jadnej paczki
    char buf[255];        // tworzymy bufor na znaki
    int teksty=argc;      // ilość plików do otwarcia

    pipefd=open(potokfifo,O_WRONLY); // otwieramy potok

    while (--teksty>0)  // bierzemy każdy plik
    {
        filefd=open(argv[teksty],O_RDONLY); // otwieramy plik

        while((ilosc_znakow=read(filefd,buf,MaxSize))>0) // sprawdzamy czy mamy co czytać
        {
            write(pipefd,buf,ilosc_znakow); // piszemy
        }
        fprintf(stdout,"Czekam zauważalny czas\n");
        sleep(5); // oczekiwanie
    }
    close(pipefd);
}
