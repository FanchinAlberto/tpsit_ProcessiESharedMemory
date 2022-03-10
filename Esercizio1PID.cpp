#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {
    int pid = fork();
    int status;
    if (pid == -1) {
        cout << "Errore!"<<endl;
        return -1;
    } else if (pid > 0) {
        cout << "Padre: " << getpid() << endl;
        waitpid(pid, &status, 0);
    } else {
        cout << "Figlio: " << getpid() << endl;
        return 0;
    }
    
    return 0;
}