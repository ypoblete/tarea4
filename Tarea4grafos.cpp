#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <queue>

using namespace std;

struct Nodo{ //Estructura que permitirá ir haciendo los punteros de las relaciones entre usuarios.
    int dato;
    Nodo *sig;
    Nodo(){
        dato =-1;
        sig = NULL;
    }
    Nodo(int d){
        dato = d;
        sig = NULL;
    }
    Nodo(int d,Nodo* nuevo_nodo){
        dato = d;
        sig =nuevo_nodo;
    }
};

void menu();
string *inicial = new string; //array que almacena los nombres de los usuarios y que se relacionarán de acuerdo a la posición
string *aux = new string; //array auxiliar que se utiliza al momento de hacer resize del array inicial
int contador = 0; //Contador que almacena la cantidad de usuarios actuales en la red social
int contador2 = 0;
void createArray(int n); //Se hace petición de memoria con malloc para el arreglo incial de acuerdo a la cantidad de usuarios
void resizeArray(int n); //Permite hacer el resize del array inicial con el array auxiliar "aux"
Nodo **vec; //Arreglo de punteros que hará la relación de usuarios en forma de listas enlazadas con los Nodos
Nodo **aux2; //Arreglo auxiliar que se utiliza cuando se hace resize al arreglo vec al momento de agregar mas usuario a la red social
void createNvector(int n); //Se hace la petición de memoria con malloc para el arreglo de punteros de Nodos
void resizeNvector(int n); //Permite hacer el resize del array vec con el array Nodo **aux al momento de ingresar mas usuarios
void insertRelacion(int m); //Función que hace la relación de los usuarios. Va creando nodos de acuerdo a las relaciones y los conecta considerando las posiciones en el arreglo "vec"
void showRelacion(); //Muestra la relación de los usuarios luego hacer la inserción de relaciones. Recorre las posiciones del arreglo vec y por cada posición recorre los punteros a los nodos siguientes Nodo->sig definido en la estructura Nodo inicialmente. Esto lo muestra en forma de lista de adyacencia.
vector <int> Compare;
vector <vector <int>> resultCliques; //Vector de vectores que almacena los diferentes cliques que se encuentren en el grafo
int factorial(int n); //Función que permite indicar la cantidad máxima de relaciones a ingresar entre usuarios
string Findu(string u); //Método para buscar usuario u en la red social utilizando el recorrido por anchura. Utiliza como estructura principal una queue
vector <int> R,P,X; //Vectores que se utilizarán en el algoritmo de Bron-Kerbosch. R vector que almacenará los potenciales vértices que formen cliques. X es el vector de vértices que fueron proesados o que forman parte de clique y P es el vector que inicialmente contiene los vértices del grafo
vector <int> XinterNv(vector<int> R,vector<int> P, vector<int> X, vector<int> v); //Vector que hace la intersección entre el vértice en el vector X y los vecinos de este
vector <int> PinterNv(vector<int> R, vector<int> P,vector<int> X, vector<int> v); //Vector que hace la intersección entre el vértice en el vector P y los vecinos de este
vector <int> Xunionv(vector<int> R, vector<int> P, vector<int> X, vector<int> v); //Vector que almacena vertices de P luego de ser analizado en la llamada recursiva
vector <int> Psinv(vector<int> R, vector<int> P, vector<int> X, vector<int> v); //Vector que va recibiendo los elementos de P que no fueron analizados.
vector <int> RunionV(vector<int> R, vector<int> P,vector<int> X, vector<int> v); //Vector que va recibiendo los vértices de P que pueden formar un clique.
int bronKerbosch(vector<int> R, vector<int> P, vector<int> X); //Algoritmo de Bron-Kerbosch descrito en informe. Se considera el algoritmo básico sin pivot
vector <int> CompareV(vector<vector <int>> Cliques); //Vector que hace permite hacer el método Compact. Crea 1 vértice nuevo por cada clique y lo une de acuerdo a la relación que presentan los elementos que no formaban parte de los cliques maximales.
void cuentaVecis(); //Método que permite saber quien de los usuarios tiene mayor cantidad de conexiones.




int main(){
    menu();
    return 0;
}

void menu(){
    int opcion,n,m;
    string u,name;
    
    do{ //Se define un menu para realizar las operaciones requeridas con diferentes casos que serán los métodos pedidos.
        cout<<"\t.:MENU:."<<endl;
        cout<<"1. Insertar usuarios"<<endl;
        cout<<"2. Insertar relaciones entre usuarios"<<endl;
        cout<<"3. Buscar usuario"<<endl;
        cout<<"4. Buscar Cliques"<<endl;
        cout<<"5. Compact"<<endl;
        cout<<"6. Influencer"<<endl;
        cout<<"7. Salir"<<endl;
        cout<<"Opción: ";
        cin>>opcion;
        switch(opcion){
            case 1: cout <<"\nDigite número de usuarios: ";
                cin>>n;
                if(inicial[0].empty()){ //Si el arreglo inicial no contiene usuarios entonces realiza esto
                    createArray(n); //Crea arreglo de usuarios en string
                    createNvector(n); //Crea arreglo de nodos con las posiciones
                    cout <<"Ingrese nombre de cada usuario: " << endl;
                    for(int i=0;i<n;i++){ //Por cada usuario digitamos el nombre
                        cin >> inicial[i];
                        contador++;
                    }
                    cout <<"\nLista de usuarios actuales: "<<endl;
                    for(int i=0;i<n;i++){
                        cout << i << "\t: " << inicial[i] << endl; //Muestra los usuarios en string con su relación numérica
                    }
                }
                else{
                    cout <<"\nIngrese el nombre de los nuevos usuarios: "<<endl; //En caso que el arreglo inicial ya tenga usuarios ingresados, hace el resize y lo agrega en la posición siguiente
                    resizeNvector(n); //Realiza el resize del arreglo de punteros a nodos y usa el auxiliar para incorporar los que ya estaban y agregar el nuevo al final
                 
                    resizeArray(n); //Realiza el resize del arreglo inicial de string
                    for(int i=contador;i<(contador+n);i++){
                        cin >> inicial[i]; //Agrega el usuario como string al arreglo inicial
                    }
                    cout <<"\nLista de usuarios actuales: "<< endl;
                    for(int i=0;i<(contador+n);i++){ //Recorre el arreglo inicial para indicar los nombres de los usuarios con su posición respectiva.
                        cout << i << "\t: " << inicial[i] << endl;
                    }
                    contador = contador + n;
                }
                cout << "\n";
                break;
            case 2: cout <<"\nInserte el numero de conexiones: " << endl;
                cout <<"(El número máximo de conexiones es " << factorial(contador)/(factorial(contador-2)*2) << ")" << endl; //Realiza la cantidad máxima de relaciones entre usuarios a digitar
                cin >> m;
                if(m > (factorial(contador)/(factorial(contador-2)*2))){
                    cout << "Numero conexiones no puede ser mayor que: " << ((factorial(contador)/(factorial(contador-2)*2))) << endl;
                }
                else{
                    cout <<"Ingrese conexiones entre usuarios con los números respectivos a cada uno: " << endl;
                    insertRelacion(m); //Inserta las relaciones de acuerdo a lo descrito en el método
                    showRelacion(); //Muestra las relaciones entre usuarios como una lista de adyacencia
                }
                cout << "\n";
                break;
            case 3: cout <<"\nIngrese nombre de usuario a buscar: ";
                cin >> u;
                cout << "--------> " << Findu(u) << endl; //Retorna Yes o No en caso que el usuario se encuentro o no en la red social. Usa algoritmo BFS en caso que el usuario tenga conexiones y de otra forma usa un búsqueda con un ciclo for en el arreglo inicial comparando el nombre "u" ingresado con los que están presentes en el arreglo de string
                cout << "\n";
                break;
            case 4: cout <<"\nCliques máximos: " << endl;
                //cout << contador << endl; SI
                if(resultCliques.size() == 0){ //En caso que el vector de vectores de cliques esté vacío arma un vector P con los vértices del grafo y luego realiza el algoritmo de bron-kerbosch y retorna los cliques.
                    for(int i=0;i<contador;i++){
                        P.push_back(i); //Agregamos los usuarios la vector P
                       //cout << P[i] << endl; SI
                    }
                    //cout << P.size() << endl; SI
                    bronKerbosch(R,P,X); //Algoritmo de Bron-Kerbosch
                    for(int i=0;i<resultCliques.size();i++){ //Doble ciclo for que muestra cliques con el respectivo nombre de usuario
                        for(int j=0;j<resultCliques[i].size();j++){
                            cout << inicial[resultCliques[i][j]] << "\t";
                        }
                        cout <<"\n";
                    }
                }
                else{ //En caso que el vector de cliques no esté vacío lo elimina y vuelve a analizar los vértices
                    resultCliques.erase(resultCliques.begin(),resultCliques.end());
                    for(int i=0;i<contador;i++){
                        P.push_back(i);
                       //cout << P[i] << endl; SI
                    }
                    //cout << P.size() << endl; SI
                    bronKerbosch(R,P,X);
                    for(int i=0;i<resultCliques.size();i++){
                        for(int j=0;j<resultCliques[i].size();j++){
                            cout << inicial[resultCliques[i][j]] << "\t";
                        }
                        cout <<"\n";
                    }
                }
                break;
            case 5: cout <<"\nCompact: " << endl; //Convierte los cliques maximales en un vértice nuevo y realiza la relación entre los usuarios que no pertencen al clique y el nuevo vértice formado.
                if(resultCliques.size() == 0){
                    cout << "No hay cliques formados, usar opción 4 " << endl;
                }
                else{
                    cout << "Las nuevas conexiones son: " << endl;
                    CompareV(resultCliques); //Muestra las relaciones de los elementos que no forman parte de los cliques maximales con el nuevo vértice creado por clique maximal
                }
                break;
            case 6: cout <<"\nLos influencers son: ";
                cuentaVecis(); //Cuenta la cantidad de usuarios a los que está conectado cada usuario independiente y luego selecciona el número máximo e imprime todos los usuarios que poseen esa cantidad de conexiones.
                cout << "\n";
        }
    }while(opcion != 7);
}

void createArray(int n){
    inicial = (string*) malloc(sizeof(string)*n);
    for(int i=0;i<n;i++){ //Asignación de valores al array inicial O(n)
        inicial[i] = -1;
    }
}

void resizeArray(int n){
    aux = inicial; //Se almacenan los valores del array inicial en un vector auxiliar
    inicial = (string*) realloc(inicial,sizeof(string)*(contador+n));
    for(int i=0;i<contador;i++){ //Asignacion de valores al array inicial cuando con nueva cantidad de memoria. O(n)
        inicial[i] = aux[i]; //Se agregan los usuarios del array auxiliar al nuevo array inicial
    }
}

void createNvector(int n){
    vec = (Nodo**) malloc(sizeof(Nodo*)*n);
    for(int i=0; i<n;i++){ //Se crea el array de punteros a punteros vec O(n)
        vec[i] = NULL;
    }
}

void resizeNvector(int n){
    aux2 = vec; //Almaceno los punteros del array vec en un array auxiliar cuando se agregan nuevos usuarios a la red social.
    vec = (Nodo**) realloc(vec,sizeof(Nodo*)*(contador+n)); //Solicito memoria para los nuevos espacios
    for(int i=0;i<contador;i++){ //Ingreso los punteros al nuevo array copiandolo del array aux2. O(n)
        vec[i] = aux2[i];
    }
    for(int i=contador;i<(contador+n);i++){
        vec[i] = NULL;
    }
}

void insertRelacion(int m){ //Inserta las relaciones estilo lista de adyacencia O(1)
    int n1,n2;
    for(int i=0;i<m;i++){
        int existe1 = 0,existe2 = 0; //Variables que ayudaran a verificar si el usuario que corresponde al numero insertado para cada relación está en la red o no
        fflush(stdin);
        scanf("%d %d",&n1,&n2); //Ingreso las relaciones que quiero tener entre los ususarios.
        if(n1 == n2) continue; //Si ingreso el mismo número no hace nada
        for(int i=0;i<contador;i++){ //Si existe el usuario n1 insertado existe1 será 1
            if(n1 == i) existe1 = 1;
        }
        for(int i=0;i<contador;i++){ //Si existe el usuario n2 insertado existe2 será 1
            if(n2 == i) existe2 = 1;
        }
        if(existe1 == 1 && existe2 == 1){ //Si ambos son 1 entonces entra a este condicional y crea las relaciones
                Nodo* nuevo_nodo1 = new Nodo(n1); //Crea el nodo para el primer usuario
                Nodo* nuevo_nodo2 = new Nodo(n2); //Crea el nodo para el segundo usuario
                Nodo* aux3; //Nodo auxiliar de vec
                if(vec[n1] == NULL){
                    vec[n1] = nuevo_nodo2; //Inserta la relación entre n1 y n2 si es NULL vec en la posición de n1. Sino se mueve al espacio siguiente y lo inserta
                }
                else{
                    aux3 = vec[n1];
                    while(aux3->sig != NULL && aux3->dato != n2){
                        aux3 = aux3->sig; //Me traslado al otro elemento
                    }
                    if(aux3->dato != n2) aux3->sig = nuevo_nodo2; //Inserto nuevo nodo a la cola
                }
                if(vec[n2] == NULL){ //Similar al usuario n1, se inserta la relación para n2 con n1 si el espacio es NULL. De otro modo avanza un espacio
                    vec[n2] = nuevo_nodo1;
                }
                else{
                    aux3 = vec[n2];
                    while(aux3->sig != NULL && aux3->dato != n1){
                        aux3 = aux3->sig;
                    }
                    if(aux3->dato != n1) aux3->sig = nuevo_nodo1;
                }
            }
        else{ //En caso que algun usuario de la relacion insertada no exista, se imprime el mensaje
            if(existe1 != 1){
                cout << "Usuario " << n1 << " no existe en la red social"<< endl;
            }
            else if(existe2 != 1){
                cout << "Usuario " << n2 << " no existe en la red social"<< endl;
            }
            else if(existe2 != 1 && existe1 != 1){
                cout << "Ninguno de los dos usuarios existe en la red social" << endl;

            }
        }
    }
    cout << "\n";
}

void showRelacion(){ //Muestra la relación entre los usuarios luego de ser insertados
    for(int i=0;i<contador;i++){ //recorre el arreglo de punteros de punteros y va recorriendo por cada posición los punteros hacia los nodos siguientes O(n)
        fflush(stdin);
        cout << i << " " << inicial[i] << "\t: ";
        if(vec != NULL){
            Nodo *aux3 = vec[i];
            if(aux3 != NULL){
                cout << aux3->dato << " "; //puede que sea null
                while(aux3->sig != NULL){
                aux3 = aux3->sig;
                cout << aux3->dato << " ";
                }
            }
        }
        cout << endl;
    }
}

int factorial(int n){ //Calcula el factorial de los usuarios insertados para indicar el número máximo de relaciones que puede haber entre todos. O(n)
    int fact=1;
    if(n<0) fact =0;
    else if(n==0) fact=1;
    else{
       for (int i = 1; i <= n; i++){
          fact = fact*i;
       }
    }
    return fact;
}

string Findu(string u){ //Busca el usuario insertado u por el recorrido por anchura (BFS) O(V+E)
    queue <int> cola; //cola que permite añadir los vecinos visitados y añadirlos a la cola para revisar sus vecinos
    vector <bool> insertados(contador,false); //Indicará si el elemento en la cola fué visitado o no
    string verificar = "No"; //Verifica si el usuario buscado está conectado a alguien o no. En caso de no estar conectado, la búsqueda del usuario se realizará por comparación de "u" con los elementos en el array "inicial"
    
    Nodo *aux3;
    cola.push(0); //Para iniciar recorrido de grafo desde nodo 0
    insertados[0] = true; //Indicamos que 0 ya fue visitado
    while(!cola.empty()){ //Recorremos la cola hasta que esté vacía
        aux3 = vec[cola.front()];
        if(aux3 != NULL){
            while(aux3 != NULL){
                if(u == inicial[aux3->dato]){ //Verificamos si el usuario está o no a través de la busqueda por anchura
                    verificar = "Yes";
                    return "Yes";
                }
                if(!insertados[aux3->dato]){ //Si el usuario no fue visitado anteriormente entonces lo insertamos a la cola
                    cola.push(aux3->dato);
                    insertados[aux3->dato] = true;
                }
                aux3 = aux3->sig; //Nos movemos al vecino siguiente
            }
        }
        cola.pop(); //Sacamos al ususario de la cola
    }
    
    if(verificar == "No"){ //Como se comentó anteriormente, si el ususario no está conectado con otro, se realiza la búsqueda por comparación O(n)
        for(int i=0;i<contador;i++){
            if(u == inicial[i]){
                return "Yes but the user is not conected to the main graph ";
            }
        }
    }
        

    return "No";
}

vector <int> XinterNv(vector<int> R,vector<int> P, vector<int> X, vector<int> v){ //Se obtiene la intersección entre elementos en X y vecinos del vértice
    vector <int> Xtemp = {};
    
    for(int i=0;i<X.size();i++){ //Itera sobre los vértices que fueron agregados a X
        Nodo *aux123 = vec[v[0]]; //Parte del vértice que está siendo analizado
        while(aux123 != NULL){ //Se mueve entre los vecinos el vértice
            if(X[i] == aux123->dato){ //Si el elemento en X es igual algun vecino de v entonces loa agrega al vector Xtemp
                Xtemp.push_back(X[i]); //Agrega el elemento al vector
            }
            aux123 = aux123->sig; //Me desplazo a otro vecino
        }
    }
        return Xtemp;
}

vector <int> PinterNv(vector<int> R, vector<int> P,vector<int> X, vector<int> v){ //Realiza la intersección entre los vértices en P y los vecinos del vértice en análisis
    vector <int> Ptemp = {};
    for(int i=0;i<P.size();i++){ //itera sobre el vector P que contiene los vértices del grafo.
        Nodo *aux123 = vec[v[0]];
        while(aux123 != NULL){
            if(P[i] == aux123->dato){ //Si encuentra un vecino de v igual al vértice en P lo agrega al vector Ptemp
                Ptemp.push_back(P[i]);
            }
            aux123 = aux123->sig; //Avanzo al vecino siguiente
        }
    }
    return Ptemp;
}

vector <int> Xunionv(vector<int> R, vector<int> P, vector<int> X, vector<int> v){ //Inserta el vértice v analizado en X
    
    vector<int> Xtemp;
    Xtemp = X;
    Xtemp.push_back(v.back()); //O(1)

    return Xtemp;
}

vector <int> Psinv(vector<int> R, vector<int> P, vector<int> X, vector<int> v){ //Elimina el vértice v analizado de P
    
    vector<int> Ptemp;
    for(int i=0;i<P.size();i++){ //O(n)
        if(P[i] != v.back()){
            Ptemp.push_back(P[i]); //O(1)
        }
    }
    return Ptemp;
}

vector <int> RunionV(vector<int> R, vector<int> P,vector<int> X, vector<int> v){ //Ingresa v a R que es posible parte de un clique
    vector<int> Rtemp;
    Rtemp = R;
    Rtemp.push_back(v.back()); //O(1)
    
    return Rtemp;
}

int bronKerbosch(vector<int> R, vector<int> P, vector<int> X){ //Algoritmo recursivo de Bron-kerbosch O(3^(n/3))
    
    if(P.empty() && X.empty()){
        if(R.size() > 2){
            resultCliques.push_back(R); //Si se cumple la condición mencionada los vertices se inserta en el vector de cliques
        }
    }
    for(int i=0;i<P.size();i++){ //Se hace el recorrido por el vector P que contiene todos los vértices
        vector <int> v_ = {};
        v_.insert(v_.end(),P[i]); //Se toma el vértice de P
        bronKerbosch(RunionV(R,P,X,v_),PinterNv(R,P,X,v_),XinterNv(R,P,X,v_)); //Se realiza el algoritmo recursivamente
        P = Psinv(R,P,X,v_); //Se elimina el vértice analizado de P
        X = Xunionv(R,P,X,v_); //Se agrega el vértice analizado a X
        
        if(P.empty()) return 1;
        else i = 0;
    }
    return 1;
}

vector <int> CompareV(vector<vector <int>> Cliques){ //Realiza las conexiones al usar el método compact
    int existe[contador]; //Los numeros que existen como usuarios
    for(int i=0;i<contador;i++){ //Hacemos un array con los números de los usuarios para crear los nuevos vértices que reemplazarán al clique. Y donde luego almacenaremos en la posición del usuario el clique al cual está conectado
        existe[i] = i;
    }
    
    for(int j=0;j<Cliques.size();j++){
        for(int k=0;k<Cliques[j].size();k++){
            existe[Cliques[j][k]] = contador + j; //Asignamos el nuevo nombre al clique que está formado por usuarios el cual corresponde al numero de usuarios existentes + ID del clique. Donde ID es el número del clique entre la cantidad total de cliques.
        }
    }
    
    for(int i=0;i<contador;i++){ //Iterador que nos permite recorrer los nodos que existen y cual es el clique al que pertenece si es que pertenece a algun clique
        fflush(stdin);
        if(existe[i] != i) continue;
        cout << i << " " << inicial[i] << "\t: ";
        if(vec != NULL && existe[i] == i){
            Nodo *aux3 = vec[i];
            if(aux3 != NULL){
                cout << existe[aux3->dato]  << " ";
                while(aux3->sig != NULL){
                    aux3 = aux3->sig;
                    cout << existe[aux3->dato] << " ";
                }
            }
        }
        cout << endl; //Hasta aqui muestro la información de los vertices ya teníamos originalmente y que no pertenecen a los cliques y lo conecta con el nuevo vértice si es que está conectado
    }
    
    for(int i=0;i<Cliques.size();i++){ //A partir de aqui se muestra la información relacionada a los cliques. Donde imprime un nuevo número asociado como nombre del clique y le imprime los números u otros cliques a los cuales está conectado.
        int var1 = contador + Cliques.size();
        bool arreglo[var1];
        for(int j=0;j<var1;j++){
            arreglo[j] = false; //Esto es para no repetir los enlaces ya que puede haber un vértice conectado a 2 vertices del clique
        }
        for(int k=0;k<Cliques[i].size();k++){
            Nodo *aux50 = vec[Cliques[i][k]];
            while(aux50 != NULL){
                arreglo[existe[aux50->dato]] = true; //Para no volver a imprimir el enlace
                aux50 = aux50->sig;
            }
        }
        cout << i+contador << "\t: "; //Imprimo el nuevo nombre del clique
        
        for(int j=0;j<var1;j++){
            if(arreglo[j] && j != i+contador) cout << j << " ";
        }
        cout <<"\n";
    }
    
    for(int i=0;i<resultCliques.size();i++){
        resultCliques[i].erase(resultCliques[i].begin(),resultCliques[i].end()); //Elimino los cliques
    }
    
    resultCliques.erase(resultCliques.begin(),resultCliques.end()); //Con esto elimino el clique del vector de vectores de cliques ya que ahora están comprimidos en un vértice.
    
    return Compare;
}

void cuentaVecis(){ //Cuenta los vecinos de cada vértice, selecciona el número mayor e imprime los usuarios que tienen la cantidad mayor de vértices
    vector <int> cuentaVecisarr(contador,0); //Vector que almacenará los vecinos de cada uno de los usuarios
    
    for(int i=0;i<contador;i++){ //Le voy sumando los vecinos acada uno de los usuarios de mi red
        Nodo *aux3 = vec[i];
        while(aux3 != NULL){
            aux3 = aux3->sig;
            cuentaVecisarr[i] += 1;
        }
    }
    
    int mayor = -1;
    for(int i=0;i<cuentaVecisarr.size();i++){
        if(cuentaVecisarr[i] > mayor){
            mayor = cuentaVecisarr[i]; //Recorro mi vector de cantidad de vecinos y guardo en variable auxiliar mayor el número mas grande
        }
    }
    
    for(int i=0;i<cuentaVecisarr.size();i++){ //Imprimo los usuarios cuyos vecinos son igual al número mayor almacenado en la variable auxiliar
        if(cuentaVecisarr[i] == mayor){
            cout << inicial[i] << " ";
        }
    }
    cout << "\n";
}



