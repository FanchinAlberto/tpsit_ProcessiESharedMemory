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
        string filename = *name; //prendo il nome del file dalla shared memory
        string line;
        ifstream myfile (filename); //leggo il file e lo stampo in console
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                cout << line << endl;
            }
        myfile.close(); //chiudo il file
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
     string poesia = "Onde dorate, e l’onde eran capelli,\n"
     "navicella d’avorio un dì fendea;\n"
     "una man pur d’avorio la reggea\n"
     "per questi errori preziosi e quelli;\n"
     "\n"
     "e, mentre i flutti tremolanti e belli\n"
     "con drittissimo solco dividea,\n"
     "l’òr delle rotte fila Amor cogliea,\n"
     "per formarne catene a’ suoi rubelli.\n"
     "\n"
     "Per l’aureo mar, che rincrespando apria\n"
     "il procelloso suo biondo tesoro,\n"
     "agitato il mio core a morte gìa.\n"
     "\n"
     "Ricco naufragio, in cui sommerso io moro,\n"
     "poich’almen fur, ne la tempesta mia,\n"
     "di diamante lo scoglio e ‘l golfo d’oro!";
    ofstream myfile (filename);
        if (myfile.is_open()) //scrivo il file
        {
            myfile << poesia;
            myfile.close();
        }
    fork_getFileName(name);

    shmdt(name); //libero il blocco di memoria richiesto
    shmctl(shmid, IPC_RMID, NULL); //cancello il blocco di memoria
}
