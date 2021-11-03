#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "tablero.hpp"

using namespace std;

const int CNT_HIJOS = 3;



typedef struct { // Este struct se usa para guardar toda la informacion relevante a los procesos jugadores + su pipe para comunicar los jugadores
   pid_t pid;
   jugador player;
   int fd[2];
} hijo_t;

pid_t todos_padre; // pid del proceso padre
int hijo_index = -1; // marca del numero de jugador de cada hijo, para el padre (que no es jugador) es -1
hijo_t padre_hijo[CNT_HIJOS]; // array con la informacion relevante al juego de los 3 procesos

bool hijo_debe_salir; // bool para indicar si el hijo debe dejar de jugar
int padre_hijo_actual; // iteracion global de los hijos por el padre

bool padre_esperando_a_hijo; // bool para indicar si se esta esperando la jugada de un hijo
bool padre_hayUnGanador; // bool para indicar la presencia de un ganador

Tablero tablero;

/*****
* void hijo_turno_usuario
******
* Gestiona y ejecuta la jugada del jugador 1 (el usuario)
******
* Input:
* int signal_number: utilizado por signal()
******
* Returns:
* Sin retorno
*****/

void hijo_turno_usuario(int signal_number){
    //Jugada
    char n;
    bool flag =  true;
    bool dado_tirado = false;
    
    cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
    cout << "\nTu turno!\n" << endl;
    while(flag){
        cout << "> ";
        cin >> n;
        switch(n){
        case '1':
            padre_hijo[hijo_index].player.dado();
            cout << "Dado: " << padre_hijo[hijo_index].player.getSiguiente() << endl;
            dado_tirado = true;
            break;
        case '2':
            if (!dado_tirado){
                cout << "No ha tirado el dado, no sea tramposo y juege!" << endl;
            }
            else{
                try{padre_hijo[hijo_index].player.play(tablero.getTablero());}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN PLAYER1" << endl;
                }
                flag = false;
            }
            break;
        case '3':
            cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
            break;
        default:
            cout << "Ingrese un valor valido" << endl;
            break;
        }
    }
    if (write(padre_hijo[hijo_index].fd[1], &padre_hijo[hijo_index].player, sizeof(jugador)) == -1){
        cout << "Hijo " << hijo_index+1 << ": PIPE 1 WRITE ERROR" << endl;
    }
    if (padre_hijo[hijo_index].player.getPesos() >= 500) {
        kill(todos_padre,SIGTERM);
    } else {
        kill(todos_padre,SIGINT);
    }
    //Final de la funcion
}

/*****
* void hijo_turno
******
* Jugada automatica de los jugadores IA
******
* Input:
* int signal_number: utilizado por signal()
******
* Returns:
* Sin retorno
*****/

void hijo_turno(int signal_number){
    string c;

    //Jugada
    padre_hijo[hijo_index].player.dado();
    cout << "Dado: " << padre_hijo[hijo_index].player.getSiguiente() << endl;
    try{
        padre_hijo[hijo_index].player.play(tablero.getTablero());
    } catch(bad_alloc& ba) {
        cerr << "bad_alloc CAUGHT IN padre_hijo[" << hijo_index << "]" << endl;
    }

    //Final de la funcion
    if (write(padre_hijo[hijo_index].fd[1], &padre_hijo[hijo_index].player, sizeof(jugador)) == -1){
        cout << "Hijo " << hijo_index+1 << ": PIPE 1 WRITE ERROR" << endl;
    }
    cout << "Turno terminado." << endl;
    cout << "Escriba algo para continuar:\n>";
    cin >> c;
    if(padre_hijo[hijo_index].player.getPesos() >= 500) {
        kill(todos_padre,SIGTERM);
    } else {
        kill(todos_padre,SIGINT);
    }
}

/*****
* void hijo_salir
******
* Hace que el hijo deje de esperar y pueda seguir y terminar su ejecucion
******
* Input:
* int signal_number: utilizado por signal
******
* Returns:
* Sin retorno
*****/

void hijo_salir(int signal_number){
    hijo_debe_salir = true;
}

/*****
* void padre_terminar_hijo
******
* Termina el juego
******
* Input:
* int signal_number: utilizado por signal
******
* Returns:
* Sin retorno
*****/

void padre_terminar_hijo(int signal_number){
    if (read(padre_hijo[padre_hijo_actual].fd[0], &padre_hijo[padre_hijo_actual].player, sizeof(jugador)) == -1) {
        cout << "Padre: PLAYER " << padre_hijo_actual+1 << " PIPE READ ERROR" << endl;
    }
    else{
        cout << "Padre: PLAYER " << padre_hijo_actual+1 << " READ" << endl;
    }
    if (padre_hijo[padre_hijo_actual].player.getPesos() >= 500) {
        cout << "Tenemos un ganador! Felicidades, Jugador " << padre_hijo_actual+1 << endl;
    } else {
        cout << "padre_hijo_actual " << padre_hijo_actual+1 << " no gano ¿¿¿?!!! \t [[Hyperlink Blocked]]" << endl;
    }
    padre_hayUnGanador = true;
}

/*****
* void padre_el_hijo_movio
******
* Lee la informacion del hijo que acaba de terminar su jugada y permite la continuacion del loop del juego
******
* Input:
* int signal_number: utilizado por signal
******
* Returns:
* Sin retorno
*****/

void padre_el_hijo_movio(int signal_number){
    if (read(padre_hijo[padre_hijo_actual].fd[0], &padre_hijo[padre_hijo_actual].player, sizeof(jugador)) == -1) {
        cout << "Padre: PLAYER " << padre_hijo_actual+1 << " PIPE READ ERROR" << endl;
    }
    else{
        cout << "Padre: PLAYER " << padre_hijo_actual+1 << " READ" << endl;
    }
    padre_esperando_a_hijo = false;
}

int main() {
    // Set up
    srand(time(NULL));

    todos_padre = getpid();
    for(int i = 0; i < CNT_HIJOS; i++) {
        if(pipe(padre_hijo[i].fd) == -1) {
            cout << "PLAYER PIPE " << i << " CREATION ERROR" << endl;
            return -1;
        }
        padre_hijo[i].pid = fork();
        if(padre_hijo[i].pid == 0){ // si es el hijo
            hijo_index = i;
            padre_hijo[i].player.setId(getpid());
            close(padre_hijo[i].fd[0]);
            break;
        } else { // si es el padre
            close(padre_hijo[i].fd[1]);
        }
    }
    // Juego
    if(hijo_index == -1){ // osea es el padre
        sleep(1);
        signal(SIGTERM, padre_terminar_hijo);
        signal(SIGINT, padre_el_hijo_movio);
        tablero.show(padre_hijo[0].player, padre_hijo[1].player, padre_hijo[2].player);
        while(!padre_hayUnGanador){
            for(padre_hijo_actual = 0; padre_hijo_actual < CNT_HIJOS; padre_hijo_actual++){
                padre_esperando_a_hijo = true;

                cout << "\nJugador " << padre_hijo_actual+1 << " jugando..." << endl;
                kill(padre_hijo[padre_hijo_actual].pid,SIGINT);

                while(padre_esperando_a_hijo && !padre_hayUnGanador){
                    sleep(5);
                }
                if(padre_hayUnGanador){
                    break;
                }
            }
            tablero.show(padre_hijo[0].player, padre_hijo[1].player, padre_hijo[2].player);
        }
    }
    else{
        if (hijo_index == 0) {
            signal(SIGINT, hijo_turno_usuario);
        } else {
            signal(SIGINT, hijo_turno);
        }
        signal(SIGTERM, hijo_salir);
        while(!hijo_debe_salir){
            sleep(rand()%5+2);
        }
    }

    //Cleanup post-juego de los fds de los jugadores

    if (hijo_index == -1){
        for (int i = 0; i < CNT_HIJOS; i++){
            close(padre_hijo[i].fd[0]);
        }
    }
    else{
        close(padre_hijo[hijo_index].fd[1]);
    }
    return 0;
}