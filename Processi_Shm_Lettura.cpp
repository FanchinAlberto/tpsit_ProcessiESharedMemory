#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

void fork_getFileName(string* name)
{
     pid_t p = fork();
    
    if(p == 0){ //controllo se sono all'interno del processo figlio
        string filename = *name;
        string line;
        ifstream myfile (filename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                cout << line << endl;
            }
        myfile.close();
        }
        exit(EXIT_SUCCESS);
    }
    else if(p > 0){ //altrimenti controllo se sono all'interno del processo padre
        waitpid(p, nullptr, 0);
    }
}

int main()
{
    string filename = "test.txt";
    key_t key = ftok("sas", 0);  //genero una chiave univoca per la shared memory
    int shmid = shmget(key, sizeof(filename), 0666|IPC_CREAT); //richiedo al siatema un blocco di memoria 
    string *name = (string*)shmat(shmid, NULL, 0); //prendo il blocco di memoria condiviso per utilizzarlo
    *name = filename;
    ofstream myfile (filename);
        if (myfile.is_open())
        {
            myfile << "Fanchin Alberto\n";
            myfile << "4CII 31/03/2022\n";
            myfile << "Esercizio Processi, Shared Memory e Lettura + Scrittura di file\n";
            myfile.close();
        }
    fork_getFileName(name);

    shmdt(name); //libero il blocco di memoria richiesto
    shmctl(shmid, IPC_RMID, NULL); //cancello il blocco di memoria
}
