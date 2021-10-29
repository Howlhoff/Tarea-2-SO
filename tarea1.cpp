#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "tablero.hpp"

using namespace std;

int hijos = 3;

int todos_padre;
jugador hijo_yoo;
jugador jugadores[3];
int hijo_con_turno;
int hijo_cntTurno;
bool hijo_debe_salir;
int padre_hijo[3];
int padre_hijo_actual;
bool padre_esperando_a_hijo;
bool padre_hayUnGanador;
Tablero tablero;

void hijo_turno_usuario(int signal_number){
    //Jugada
    int n;
    bool flag =  true;
    cout << "Tu turno: " << endl;
    while(flag){
        cout << "> ";
        cin >> n;
        switch(n){
        case 1:
            hijo_yo.dado();
            break;
        case 2:
            tablero.ejecutar(hijo_yo);
            flag = false;
            break;
        case 3:
            tablero.show(jugadores[0],jugadores[1],jugadores[2]);
            break;
        case 4:
            cout << "Menu:\n1. Tirar dado\n2. Ejecutar accion\n3. Ver el tablero actual\n4. Este menu." << endl;
            break;
        default:
            cout << "Ingrese un valor valido" << endl;
            break;
        }
    }
     
    //Final de la funcion
    if(hijo_yo.getPesos() >= 500){
        kill(todos_padre,SIGTERM);
    }
    else{
        kill(todos_padre,SIGINT);
    }
}

void hijo_turno(int signal_number){
    //Jugada
    hijo_yo.dado();
    tablero.ejecutar(hijo_yo);
    //Final de la funcion
    if(hijo_yo.getPesos() >= 500){
        kill(todos_padre,SIGTERM);
    }
    else{
        kill(todos_padre,SIGINT);
    }
}

void hijo_salir(int signal_number){
    hijo_debe_salir = true;
}

void padre_terminar_hijo(int signal_number){
    padre_hayUnGanador = true;
}

void padre_el_hijo_movio(int signal_number){
    padre_esperando_a_hijo = false;
}

int main() {
    srand(time(NULL));
    todos_padre = getpid();
    for(int i = 0; i < hijos; i++){
        padre_hijo[i] = fork();
        if(padre_hijo[i] == 0){
            hijo_yo.setId(getpid());
            jugadores[i] = hijo_yo;
            break;
        }
    }
    if(hijo_yo.getId() == 0){
        sleep(1);
        signal(SIGTERM, padre_terminar_hijo);
        signal(SIGINT, padre_el_hijo_movio);
        while(!padre_hayUnGanador){
            for(int i = 0; i < hijos; i++){
                padre_hijo_actual = i;
                padre_esperando_a_hijo = true;
                kill(padre_hijo[i],SIGINT);
                while(padre_esperando_a_hijo && !padre_hayUnGanador){
                    usleep(100);
                }
                if(padre_hayUnGanador){
                    break;
                }
            }
        }
    }
    else{
        if(padre_hijo[0] == hijo_yo.getId()){
            signal(SIGINT, hijo_turno_usuario);
            signal(SIGTERM, hijo_salir);
            while(!hijo_debe_salir){
                sleep(rand() % 10 + 1);
            }
        }
        else{
            signal(SIGINT, hijo_turno);
            signal(SIGTERM, hijo_salir);
            while(!hijo_debe_salir){
                sleep(rand() % 10 + 1);
            }
        }
    }
    return 0;
}