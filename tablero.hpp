#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

class jugador{
    private:
        pid_t id;
        int pesos;
        int actual;
        int siguiente;
        bool turno;
    
    public:
        jugador(){
            pesos = 0;
            actual = 0;
            siguiente = 0;
            turno = false;
        }

        pid_t getId(){
            return this->id;
        }

        void setId(pid_t newId){
            this->id = newId;
        }

        bool getTurno(){
            return this->turno;
        }

        void setTurno(bool nuevo){
            this->turno = nuevo;
        }

        int getPesos(){
            return this->pesos;
        }

        int getActual(){
            return this->actual;
        }

        int getSiguiente(){
            return this->siguiente;
        }

        int suma(int n){
            this->pesos += n;
            return this->pesos;
        }

        int resta(int n){
            this->pesos -= n;
            return this->pesos;
        }

        void forward(int n){
            for(int i = 0; i < n; i++){
                this->actual++;
            }
            if(actual > 28){
                this->actual = this->actual%28;
            }
        }

        void backward(int n){
            for(int i = 0; i < n; i++){
                this->actual--;
            }
            if(actual < 0){
                this->actual = (28-this->actual)%28;
            }
        }
        void dado(){
            int ran = (rand() % 6) + 1;
            this->siguiente = ran;
            this->forward(this->siguiente);
        }

};


class Tablero{
    private:
        string* tablero;

    public:
        Tablero(){
            this->tablero = new string[28];
            this->tablero[0]="start";
            this->tablero[1]="+50";
            this->tablero[2]="+50";
            this->tablero[3]="-25";
            this->tablero[4]="b 2";
            this->tablero[5]="+50";
            this->tablero[6]="-25";
            this->tablero[7]="jail";
            this->tablero[8]="+75";
            this->tablero[9]="-50";
            this->tablero[10]="-50";
            this->tablero[11]="+75";
            this->tablero[12]="+75";
            this->tablero[13]="b 3";
            this->tablero[14]="free";
            this->tablero[15]="f 3";
            this->tablero[16]="+50";
            this->tablero[17]="b 4";
            this->tablero[18]="+50";
            this->tablero[19]="-50";
            this->tablero[20]="-25";
            this->tablero[21]="jail";
            this->tablero[22]="+75";
            this->tablero[23]="f 5";
            this->tablero[24]="+75";
            this->tablero[25]="b 4";
            this->tablero[26]="-25";
            this->tablero[27]="-75";
        }
        ~Tablero(){
            delete[] this->tablero;
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
            this->ejecutar(j);
        }
        void retroceso(jugador j, int n){
            j.backward(n);
            this->ejecutar(j);
        }
        void ejecutar(jugador j){
            string s = this->tablero[j.getActual()];
            if(s == "+75"){
                this->plus(j,75);
            }
            else if(s == "+50"){
                this->plus(j,50);
            }
            else if(s == "-25"){
                this->minus(j,25);
            }
            else if(s == "-50"){
                this->minus(j,50);
            }
            else if(s == "-75"){
                this->minus(j,75);
            }
            else if(s == "b 2"){
                this->retroceso(j,2);
            }
            else if(s == "b 3"){
                this->retroceso(j,3);
            }
            else if(s == "b 4"){
                this->retroceso(j,4);
            }
            else if(s == "f 3"){
                this->avance(j,3);
            }
            else if(s == "f 5"){
                this->avance(j,5);
            }
            else if(s == "start"){
                this->start(j);
            }
            else if(s == "jail"){
                this->jail(j);
            }
            else if(s == "free"){
                this->free(j);
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
                    temp[i] = "J 1";
                }
                else if (i == pos[1]){
                    temp[i] = "J 2";
                }
                else if (i == pos[2]){
                    temp[i] = "J 3";
                }
                else{
                    temp[i] = this->tablero[i];
                }
            }

            cout << " _______________________________________________" << endl;
            cout << "|\t\t\t\t\t        |" << endl;
            cout << "| " << temp[7] << "| " <<  temp[8] << " | " << temp[9] << " | " << temp[10] << " | " << temp[11] << " | " << temp[12] << " | " << temp[13] << " |" << temp[14] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[6] <<" |\t\t\t\t\t  | " << temp[15] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[5] <<" |\t\t\t\t\t  | " << temp[16] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[4] << " |\t\t\t\t\t  | " << temp[17] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[3] << " |\t\t\t\t\t  | " << temp[18] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[2] << " |\t\t\t\t\t  | " << temp[19] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "| " << temp[1] << " |\t\t\t\t\t  | " << temp[20] << " |" << endl;
            cout << "|-----|\t\t\t\t\t  |-----|" << endl;
            cout << "|" << temp[0] << "| " << temp[27] << " | " << temp[26] << " | " << temp[25] << " | " << temp[24] << " | " << temp[23] << " | " << temp[22] << " |" << temp[21] << " |" << endl;
            cout << "|_______________________________________________|" << endl;

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