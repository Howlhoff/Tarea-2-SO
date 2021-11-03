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
        int siguiente; //atributo para guardar el dado
        bool inJail;
    
    public:
        jugador(){
            pesos = 100; // Se parte en 100 por que todos los jugadores empiezan en start
            actual = 0;
            siguiente = 0;
            inJail = false;
        }

        /*****
        * void getId
        ******
        * Getter del atributo 'id'
        ******
        * Input:
        * Sin input
        ******
        * Returns:
        * El valor actual del atributo id
        *****/

        pid_t getId(){
            return this->id;
        }

        /*****
        * void getId
        ******
        * Setter del atributo 'id'
        ******
        * Input:
        * pid_t newId: nuevo valor del atributo 'id'
        ******
        * Returns:
        * Sin retorno
        *****/

        void setId(pid_t newId){
            this->id = newId;
        }

        /*****
        * void getPesos
        ******
        * Getter del atributo 'pesos'
        ******
        * Input:
        * Sin input
        ******
        * Returns:
        * El valor actual del atributo 'pesos'
        *****/

        int getPesos(){
            return this->pesos;
        }

        /*****
        * void getActual
        ******
        * Getter del atributo 'actual'
        ******
        * Input:
        * Sin input
        ******
        * Returns:
        * El valor actual del atributo 'actual'
        *****/

        int getActual(){
            return this->actual;
        }

        /*****
        * void getSiguiente
        ******
        * Getter del atributo 'siguiente'
        ******
        * Input:
        * Sin input
        ******
        * Returns:
        * El valor actual del atributo 'siguiente'
        *****/

        int getSiguiente(){
            return this->siguiente;
        }

        /*****
        * void suma
        ******
        * Paga al jugador n pesos
        ******
        * Input:
        * int n: Dinero a pagar
        ******
        * Returns:
        * Sin retorno
        *****/

        int suma(int n){
            this->pesos += n;
            return this->pesos;
        }

        /*****
        * void resta
        ******
        * Cobra al jugador n pesos
        ******
        * Input:
        * int n: dinero a cobrar
        ******
        * Returns:
        * Sin retorno
        *****/

        int resta(int n){
            this->pesos -= n;
            return this->pesos;
        }

        /*****
        * void forward
        ******
        * Avanza al jugador de n espacios
        ******
        * Input:
        * int n: espacios a desplazar
        ******
        * Returns:
        * Sin retorno
        *****/

        void forward(int n){
            if (this->actual + n > 27){ // en este caso se cruza por start
                this->actual = (this->actual+n)%28; // nueva posicion
                this->suma(100); // sumarle los 100 que se ganan al cruzar start
            }
            else{
                this->actual += n;
            }
        }

        /*****
        * void backward
        ******
        * Retrocede al jugador de n espacios
        * Nota: Dado que el tablero es estatico y no hay ninguna instancia de
        * retorceso que haga que jugador el jugador retroceda mas alla de start,
        * no se implemento esa
        ******
        * Input:
        * int n: espacios a desplazar
        ******
        * Returns:
        * Sin retorno
        *****/

        void backward(int n){
            this->actual -= n;
        }

        /*****
        * void dado
        ******
        * Lanza el dado y mueve al jugador
        ******
        * Input:
        * Sin argumentos
        ******
        * Returns:
        * Sin retorno
        *****/

        void dado(){
            if (this->inJail == false){
                int ran = (rand() % 6) + 1;
                this->siguiente = ran;
                this->forward(this->siguiente);
            }
            else {
                cout << "Jugador en la carcel, debe saltar su turno." << endl;
            }
        }

        /*****
        * void play
        ******
        * Aplica al jugador los efectos de la casilla actual
        ******
        * Input:
        * string* table: el arreglo de las casillas del tablero
        ******
        * Returns:
        * Sin retorno
        *****/
        void play(string* table){
            string s = table[this->actual];
            if(s == "+75"){
                try{this->suma(75);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN +75" << endl;
                }
            }
            else if(s == "+50"){
                try{this->suma(50);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN +50" << endl;
                }
            }
            else if(s == "-25"){
                try{this->resta(25);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN -25" << endl;
                }
            }
            else if(s == "-50"){
                try{this->resta(50);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN -50" << endl;
                }
            }
            else if(s == "-75"){
                try{this->resta(75);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN -75" << endl;
                }
            }
            else if(s == "b 2"){
                try {
                    this->backward(2);
                    this->play(table);
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN B2" << endl;
                }
            }
            else if(s == "b 3"){
                try{
                    this->backward(3);
                    this->play(table);
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN B3" << endl;
                }
            }
            else if(s == "b 4"){
                try{
                    this->backward(4);
                    this->play(table);
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN B4" << endl;
                }
            }
            else if(s == "f 3"){
                try{
                    this->forward(3);
                    this->play(table);
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN F3" << endl;
                }
            }
            else if(s == "f 5"){
                try{
                    this->forward(5);
                    this->play(table);
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN F5" << endl;
                }
            }
            else if(s == "jail"){
                try{
                    if (this->inJail == false){
                        this->inJail = true;
                    }
                    else {
                        this->inJail = false;
                    }
                }
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN JAIL" << endl;
                }
            }
            else if(s == "start"){
                try{this->suma(100);}
                catch(bad_alloc& ba){
                    cerr << "bad_alloc CAUGHT IN +50" << endl;
                }
            }
            else{
                return;
            }
        }

};


class Tablero{
    private:
        string* tablero; // el arreglo de las casillas del tablero en orden desde 'start'

    public:
        Tablero(){
            this->tablero = new string[28];
            this->tablero[0]="sta";
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

        /*****
        * void getTablero
        ******
        * Getter del atributo 'tablero'
        ******
        * Input:
        * Sin input
        ******
        * Returns:
        * El valor actual del atributo 'tablero'
        *****/

        string* getTablero(){
            return this->tablero;
        }

        /*****
        * void show
        ******
        * Bosquejo grafico y printeo de informacion del tablero
        ******
        * Input:
        * jugador j1: objeto jugador de uno de los jugadores (Player 1: Usuario)
        * jugador j2: objeto jugador de uno de los jugadores (Player 2: Auto)
        * jugador j3: objeto jugador de uno de los jugadores (Player 3: Auto)
        ******
        * Returns:
        * El valor actual del atributo id
        *****/

        void show(jugador j1, jugador j2, jugador j3){
            //Tablero
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
            cout << "| " << temp[0] << " | " << temp[27] << " | " << temp[26] << " | " << temp[25] << " | " << temp[24] << " | " << temp[23] << " | " << temp[22] << " |" << temp[21] << " |" << endl;
            cout << "|_______________________________________________|" << endl;

            //Dinero y posicion de cada jugador

            cout << "J1 - Dinero: $" << j1.getPesos() << "\t Posicion: " << j1.getActual() << endl;
            cout << "J2 - Dinero: $" << j2.getPesos() << "\t Posicion: " << j2.getActual() << endl;
            cout << "J3 - Dinero: $" << j3.getPesos() << "\t Posicion: " << j3.getActual() << endl;
        }
};