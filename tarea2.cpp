#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "tablero.hpp"

using namespace std;

const int CNT_HIJOS = 3;

typedef struct {
   pid_t pid;
   jugador player;
   int fd[2];
} hijo_t;

pid_t todos_padre;
int hijo_index = -1;
hijo_t padre_hijo[CNT_HIJOS];

int hijo_con_turno;
int hijo_cntTurno;

bool hijo_debe_salir;
int padre_hijo_actual;

bool padre_esperando_a_hijo;
bool padre_hayUnGanador;

Tablero tablero;

void hijo_turno_usuario(int signal_number){
    //Jugada
    int n;
    bool flag =  true;
    
    cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
    cout << "\nTu turno!\n" << endl;
    while(flag){
        cout << "> ";
        cin >> n;
        switch(n){
        case 1:
            padre_hijo[hijo_index].player.dado();
            cout << "Dado: " << padre_hijo[hijo_index].player.getSiguiente() << endl;
            break;
        case 2:
            try{padre_hijo[hijo_index].player.play(tablero.getTablero());}
            catch(bad_alloc& ba){
                cerr << "bad_alloc CAUGHT IN PLAYER1" << endl;
            }
            flag = false;
            break;
        case 3:
            cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
            break;
        default:
            cout << "Ingrese un valor valido" << endl;
            cout << ">" << endl;
            cin >> n;
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

void hijo_salir(int signal_number){
    hijo_debe_salir = true;
}

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
    return 0;
}