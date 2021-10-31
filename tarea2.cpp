#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "tablero.hpp"

using namespace std;

int hijos = 3;

int todos_padre;
jugador hijo_yo;
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
    // Set up
    int fd1[2];
    int fd2[2];
    int fd3[2];

    if(pipe(fd1) == -1){
        cout << "PIPE 1 CREATION ERROR" << endl;
        return -1;
    }
    
    if(pipe(fd2) == -1){
        cout << "PIPE 2 CREATION ERROR" << endl;
        return -1;
    }

    if(pipe(fd3) == -1){
        cout << "PIPE 3 CREATION ERROR" << endl;
        return -1;
    }
    cout << "Pipes created." << endl;


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
    
/*
    // Juego
    if(hijo_yo.getId() == 0){
        close(fd1[1]);
        close(fd2[1]);
        close(fd3[1]);
        
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
                if (read(fd1[0], &jugadores[0], sizeof(jugador)) == -1){
                    cout << "PIPE 1 READ ERROR" << endl;
                    return -4;
                }
                else if (read(fd2[0], &jugadores[1], sizeof(jugador)) == -1){
                    cout << "PIPE 2 READ ERROR" << endl;
                    return -4;
                }
                else if (read(fd3[0], &jugadores[2], sizeof(jugador)) == -1){
                    cout << "PIPE 3 READ ERROR" << endl;
                    return -4;
                }
            }
        }
    }
    else{
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        
        if(padre_hijo[0] == hijo_yo.getId()){
            signal(SIGINT, hijo_turno_usuario);
            signal(SIGTERM, hijo_salir);

            while(!hijo_debe_salir){
                if (write(fd1[1], &hijo_yo, sizeof(jugador)) == -1){
                    cout << "PIPE 1 WRITE ERROR" << endl;
                    return -2;
                }

                sleep(rand() % 10 + 1);
            }
        }
        else if (padre_hijo[1] == hijo_yo.getId()){
            signal(SIGINT, hijo_turno);
            signal(SIGTERM, hijo_salir);

            while(!hijo_debe_salir){
                if (write(fd2[1], &hijo_yo, sizeof(jugador)) == -1){
                    cout << "PIPE 2 WRITE ERROR" << endl;
                    return -2;
                }

                sleep(rand() % 10 + 1);
            }
        }
        else if (padre_hijo[2] == hijo_yo.getId()){
            signal(SIGINT, hijo_turno);
            signal(SIGTERM, hijo_salir);

            while(!hijo_debe_salir){
                if (write(fd3[1], &hijo_yo, sizeof(jugador)) == -1){
                    cout << "PIPE 3 WRITE ERROR" << endl;
                    return -2;
                }

                sleep(rand() % 10 + 1);
            }
        }
        else {
            cout << "PLAYER ERROR" << endl;
            return -3;
        }
    }*/
    return 0;
}