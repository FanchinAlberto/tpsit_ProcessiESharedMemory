#include <iostream>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

struct F4Info{
    char Giocatore1[50];
    char Pedina1;
    char Giocatore2[50];
    char Pedina2;
};
void fork_get_names(string tag, F4Info* forza_4){
    pid_t p = fork();
    
    if(p == 0){ //controllo se sono all'interno del processo figlio
        cout << "Nome : " << endl;
        if(tag == "first"){
            cin >> forza_4 -> Giocatore1;
        }
        if(tag == "second"){
            cin >> forza_4 -> Giocatore2;
        }
        exit(EXIT_SUCCESS);
    }
    else if(p > 0){ //altrimenti controllo se sono all'interno del processo padre
        waitpid(p, nullptr, 0);
    }
}

int main()
{
    key_t key = ftok("sas", 0);  //genero una chiave univoca per la shared memory
    int shmid = shmget(key, sizeof(F4Info), 0666|IPC_CREAT); //richiedo al siatema un blocco di memoria 
    F4Info* forza_4 = (F4Info*)shmat(shmid, NULL, 0); //prendo il blocco di memoria condiviso per utilizzarlo
    forza_4 -> Pedina1 = 'O'; //assegno i valori alle pedine
    forza_4 -> Pedina2 = 'X';
    
    fork_get_names("first", forza_4); //ottengo i nomi dei 2 giocatori
    fork_get_names("second", forza_4);
    
    cout << "Giocatore1: " << forza_4 -> Giocatore1 << endl; //stampo i nomi e le pedine
    cout << "Pedina1: " << forza_4 -> Pedina1 << endl;
    cout << "Giocatore2: " << forza_4 -> Giocatore2 << endl;
    cout << "Pedina2: " << forza_4 -> Pedina2 << endl;
    
    shmdt(forza_4); //libero il blocco di memoria richiesto
    shmctl(shmid, IPC_RMID, NULL); //cancello il blocco di memoria
    return 0;
}
