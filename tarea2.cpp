#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "tablero.hpp"

using namespace std;

int hijos = 3;

pid_t todos_padre;
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
    jugador Player1 = hijo_yo;
    Player1.setTurno(true);
    
    cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
    cout << "\nTu turno!\n" << endl;
    while(flag){
        cout << "> ";
        cin >> n;
        switch(n){
        case 1:
            Player1.dado();
            cout << "Dado: " << Player1.getSiguiente() << endl;
            break;
        case 2:
            tablero.ejecutar(Player1);
            flag = false;
            break;
        case 3:
            cout << "Menu:\t1. Tirar dado\t2. Ejecutar accion\t3. Este menu." << endl;
            break;
        default:
            cout << "Ingrese un valor valido" << endl;
            break;
        }
    }

    //Final de la funcion
    hijo_yo = Player1;

    if(hijo_yo.getPesos() >= 500){
        kill(todos_padre,SIGTERM);
    }
    else{
        kill(todos_padre,SIGINT);
    }
}

void hijo_turno(int signal_number){
    cout << "Jugador " << hijo_yo.getId() << " jugando..." << endl;

    jugador PlayerX = hijo_yo;
    srand(time(NULL));

    //Jugada
    PlayerX.dado();
    cout << "Dado: " << PlayerX.getSiguiente() << endl;
    tablero.ejecutar(PlayerX);

    //Final de la funcion
    hijo_yo = PlayerX;

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
        cout << "PLAYER PIPE 1 CREATION ERROR" << endl;
        return -1;
    }

    if(pipe(fd2) == -1){
        cout << "PLAYER PIPE 2 CREATION ERROR" << endl;
        return -1;
    }

    if(pipe(fd3) == -1){
        cout << "PLAYER PIPE 3 CREATION ERROR" << endl;
        return -1;
    }


    todos_padre = getpid();
    for(int i = 0; i < hijos; i++){
        padre_hijo[i] = fork();
        if(padre_hijo[i] == 0){
            hijo_yo.setId(getpid());
            jugadores[i] = hijo_yo;
            break;
        }
    }


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

                if (i == 0){
                    tablero.show(jugadores[0], jugadores[1], jugadores[2]);
                }
                kill(padre_hijo[i],SIGINT);

                while(padre_esperando_a_hijo && !padre_hayUnGanador){
                    usleep(100);
                }
                if(padre_hayUnGanador){
                    break;
                }
                if (read(fd1[0], &jugadores[0], sizeof(jugador)) == -1){
                    cout << "PLAYER PIPE 1 READ ERROR" << endl;
                    return -4;
                }
                else if (read(fd2[0], &jugadores[1], sizeof(jugador)) == -1){
                    cout << "PLAYER PIPE 2 READ ERROR" << endl;
                    return -4;
                }
                else if (read(fd3[0], &jugadores[2], sizeof(jugador)) == -1){
                    cout << "PLAYER PIPE 3 READ ERROR" << endl;
                    return -4;
                }
                tablero.ejecutar(jugadores[i]);

                cout << "DAD CHECK: POST-TURN" << endl;
            }
        }
    }
    else{
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);

        if(jugadores[0].getId() == hijo_yo.getId()){
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
        else if (jugadores[1].getId() == hijo_yo.getId()){
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
        else if (jugadores[2].getId() == hijo_yo.getId()){
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
    }
    return 0;
}