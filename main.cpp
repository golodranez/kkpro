#include "mainwindow.h"
#include <iostream>
#include <QCoreApplication>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <fstream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

  /*  std::ofstream of;
    of.open("/opt/sntermo/dead.log");

    pid_t pid = fork();
    if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        int dead_count = 0;
        of << dead_count << "\n";
        of.close();
        while(true) // LAUNCHER CICLE
        {
            std::cout << "DC - " << dead_count << " \n";
            pid_t pid2 = fork();
            if(pid2 < 0)
            {
                exit(EXIT_FAILURE);
            }
            if(pid2 > 0)
            {
                wait(NULL);
                dead_count++;
                of.open("/opt/sntermo/dead.log");
                of << dead_count << "\n";
                of.close();

            }
            else
            {
                break;
            }
        }
    }*/
    MainClass w;
    return a.exec();
}
