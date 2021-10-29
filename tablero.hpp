#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

class jugador{
    private:
        int id;
        int pesos;
        int actual;
        int siguiente;
        bool turno;
    
    public:
        jugador(){
            id = 0;
            pesos = 0;
            actual = 0;
            siguiente = 0;
            turno = false;
        }

        int getId(){
            return id;
        }

        void setId(int id){
            id = id;
        }

        bool getTurno(){
            return turno;
        }

        void setTurno(bool nuevo){
            turno = nuevo;
        }

        int getPesos(){
            return pesos;
        }

        int getActual(){
            return actual;
        }

        int getSiguiente(){
            return siguiente;
        }

        int suma(int n){
            pesos += n;
            return pesos;
        }

        int resta(int n){
            pesos -= n;
            return pesos;
        }

        void forward(int n){
            for(int i = 0; i < n; i++){
                actual++;
            }
            if(actual > 28){
                actual = actual%28;
            }
        }

        void backward(int n){
            for(int i = 0; i < n; i++){
                actual--;
            }
            if(actual < 0){
                actual = (28-actual)%28;
            }
        }
        void dado(){
            int ran = (rand() % 6) + 1;
            siguiente = ran;
            forward(siguiente);
        }

};


class Tablero{
    private:
        string* tablero;

    public:
        Tablero(){
            tablero = new string[28];
            tablero[0]="start";
            tablero[1]="+50";
            tablero[2]="+50";
            tablero[3]="-25";
            tablero[4]="b 2";
            tablero[5]="+50";
            tablero[6]="-25";
            tablero[7]="jail";
            tablero[8]="+75";
            tablero[9]="-50";
            tablero[10]="-50";
            tablero[11]="+75";
            tablero[12]="+75";
            tablero[13]="b 3";
            tablero[14]="free";
            tablero[15]="f 3";
            tablero[16]="+50";
            tablero[17]="b 4";
            tablero[18]="+50";
            tablero[19]="-50";
            tablero[20]="-25";
            tablero[21]="jail";
            tablero[22]="+75";
            tablero[23]="f 5";
            tablero[24]="+75";
            tablero[25]="b 4";
            tablero[26]="-25";
            tablero[27]="-75";
        }
        ~Tablero(){
            delete[] tablero;
        }
        void start(jugador j){
            j.suma(100);
        }
        void jail(jugador j){
            if(j.getTurno()==true){
                j.setTurno(false);
            }
            else{
                j.setTurno(true);
            }
        }
        void free(jugador j){
            j.setTurno(true);
        }
        void plus(jugador j, int n){
            j.suma(n);
        }
        void minus(jugador j, int n){
            j.resta(n);
        }
        void avance(jugador j, int n){
            j.forward(n);
            ejecutar(j);
        }
        void retroceso(jugador j, int n){
            j.backward(n);
            ejecutar(j);
        }
        void ejecutar(jugador j){
            string s = tablero[j.getActual()];
            if(s == "+75"){
                plus(j,75);
            }
            else if(s == "+50"){
                plus(j,50);
            }
            else if(s == "-25"){
                minus(j,25);
            }
            else if(s == "-50"){
                minus(j,50);
            }
            else if(s == "-75"){
                minus(j,75);
            }
            else if(s == "b 2"){
                retroceso(j,2);
            }
            else if(s == "b 3"){
                retroceso(j,3);
            }
            else if(s == "b 4"){
                retroceso(j,4);
            }
            else if(s == "f 3"){
                avance(j,3);
            }
            else if(s == "f 5"){
                avance(j,5);
            }
            else if(s == "start"){
                start(j);
            }
            else if(s == "jail"){
                jail(j);
            }
            else if(s == "free"){
                free(j);
            }
            else{
                return;
            }
        }

        void show(jugador j1, jugador j2, jugador j3){
            //Aqui te dejo el bosquedo
            int pos[3] = {j1.getActual(), j2.getActual(), j3.getActual()};
            string temp[28];
            for (int i = 0; i < 28; i++){
                if (i == pos[0]){
                    temp[i] = "J1";
                }
                else if (i == pos[1]){
                    temp[i] == "J2";
                }
                else if (i == pos[2]){
                    temp[i] == "J3";
                }
                else{
                    temp[i] == tablero[i];
                }
            }

            //El resto
            //Turno del jugador
            if(j1.getTurno() == true){
                cout << "El turno actual es de J1" << endl;
            }
            else if(j2.getTurno() == true){
                cout << "El turno actual es de J2" << endl;
            }
            else if(j2.getTurno() == true){
                cout << "El turno actual es de J3" << endl;
            }
            else{
                cout << "No es turno de ningun jugador" << endl;
            }

            //Dado
            if(j1.getTurno() == true){
                cout << "El dado toco: "<< j1.getSiguiente() << endl;
            }
            else if(j2.getTurno() == true){
                cout << "El dado toco: "<< j2.getSiguiente() << endl;
            }
            else if(j2.getTurno() == true){
                cout << "El dado toco: "<< j3.getSiguiente() << endl;
            }
            else{
                cout << "Ningun jugador ha tirado el dado" << endl;
            }

            //Cantidad de cada jugador

            cout << "Dinero actual de J1: " << j1.getPesos() << endl;
            cout << "Dinero actual de J2: " << j2.getPesos() << endl;
            cout << "Dinero actual de J3: " << j3.getPesos() << endl;
        }
};