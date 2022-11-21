#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
using namespace std;


//struct de las paginas del indice
struct NumeroDePagina{
    int numeroPagina;
    NumeroDePagina *izquierda = NULL;
    NumeroDePagina *derecha = NULL;

    NumeroDePagina(int numeroPagina){
        this->numeroPagina = numeroPagina;
    }
};

//metodo para insertar paginas en el árbol de ord
void insertarPagina(NumeroDePagina *&arbolpag, int numeroPagina){
    if(arbolpag == NULL){
      arbolpag = new NumeroDePagina(numeroPagina);
    }
    else{
        if(numeroPagina < arbolpag->numeroPagina){
          insertarPagina(arbolpag->izquierda, numeroPagina);
        }
        else{
          insertarPagina(arbolpag->derecha, numeroPagina);
        }
    }
}

//ordenar las paginas
void ordenarPag(NumeroDePagina *arbolpag){
    if(arbolpag != NULL){
      ordenarPag(arbolpag->izquierda);
      
      cout<<arbolpag->numeroPagina<<" ";
      
      ordenarPag(arbolpag->derecha);
    }
}

//buscar la posicion de la pagina
NumeroDePagina* buscarPag(NumeroDePagina *&arbolpag, int numeroPagina){
    if(arbolpag == NULL){
      return NULL;
    }
    else{
        if(numeroPagina == arbolpag->numeroPagina){
          return arbolpag;
        }
        else if(numeroPagina < arbolpag->numeroPagina){
          return buscarPag(arbolpag->izquierda, numeroPagina);
        }else{
          return buscarPag(arbolpag->derecha, numeroPagina);
        }
    }
}




//struct del subtermino
struct subTermino{
    subTermino *izquierda = NULL;
    subTermino *derecha = NULL;

    NumeroDePagina *paginas = NULL; 

    string palabra;
    subTermino(string palabra){
      this->palabra = palabra;
    }
};

//insertar un subtermino en el arbol de ord
void insertarSubT(subTermino *&arbolsubt, string palabra){
    if(arbolsubt == NULL){
        arbolsubt = new subTermino(palabra);
    }
    else{
        if(palabra < arbolsubt->palabra){
          insertarSubT(arbolsubt->izquierda, palabra);
        }
        else{
          insertarSubT(arbolsubt->derecha, palabra);
        }
    }
}

//ordenar el arbol de subterminos
void ordenarSubT(subTermino *&arbolsubt){
    if(arbolsubt != NULL){
      ordenarSubT(arbolsubt->izquierda);
      cout<<"   "<<arbolsubt->palabra<<": ";
      
      ordenarPag(arbolsubt->paginas);
      cout<<endl;
      
      ordenarSubT(arbolsubt->derecha);
    }
}

//buscar posicion del subtermino del arbol
subTermino* buscarSubT(subTermino *&arbolsubt, string palabra){
    if(arbolsubt == NULL){
      return NULL;
    }
    else{
      if(palabra == arbolsubt->palabra){
        return arbolsubt;
      }
      else if(palabra<arbolsubt->palabra){
        return buscarSubT(arbolsubt->izquierda, palabra);
      }
      else{
        return buscarSubT(arbolsubt->derecha, palabra);
      }
    }
}



//struct del arbol con los términos principales
struct Termino{
    string palabra; 
    Termino *izquierda = NULL;
    Termino *derecha = NULL;

    NumeroDePagina *paginas = NULL; 

    subTermino *subT = NULL;

    Termino(string palabra){
      this->palabra = palabra;
    }
};

//insertar un termino en el arbol de ord
void insertarTermino(Termino *&arbolt, string palabra){
    if(arbolt == NULL){
        arbolt = new Termino(palabra);
    }
    else{
        if(palabra < arbolt->palabra){
          insertarTermino(arbolt->izquierda, palabra);
        }
        else{
          insertarTermino(arbolt->derecha, palabra);
        }
    }
}

//ordenar el arbol
void ordenarTerminos(Termino *&arbolt){
    if(arbolt != NULL){
        ordenarTerminos(arbolt->izquierda);
        cout<<endl;
        cout<<arbolt->palabra;

        if(arbolt->paginas!=NULL){
          cout<<" ";
          
          ordenarPag(arbolt->paginas);
        }

        if(arbolt->subT != NULL){
          cout<<endl;
          
          ordenarSubT(arbolt->subT);
        }
        ordenarTerminos(arbolt->derecha);
    }
}


//buscar posicion del termino en el arbol de ord
Termino* buscarTermino(Termino *&arbolt, string palabra){
    if(arbolt == NULL){
      return NULL;
    }
    else{
      if(palabra == arbolt->palabra){
        return arbolt;
      }
      else if(palabra < arbolt->palabra){
        return buscarTermino(arbolt->izquierda, palabra);
      }
      else{
        return buscarTermino(arbolt->derecha, palabra);
      }
    }
}

/* Busca el numero que indica la cantidad de veces que una palabra aparece en el libro
Se pone el límite entre 0 y 9, ya que puede aparecer entre este rango nada más (Según código)
*/
int BuscarNumero(string num){
  for(int i = 0; i < num.length(); i++){
    if(num[i] >= '0' && num[i] <= '9'){
      return i;
      break;
    }
  }
  return -1;
}

// Extracción de las páginas en las que aparece el término o subtérmino
void extraerPag(NumeroDePagina *&arbp, int num, string pag){
  int numeroPag = stoi(pag.substr(0,1));
  string cadena = pag.substr(1);
  int x = 0;

    //Linkeamos la página al arbol de paginas
  for(int i = 0; i < numeroPag; i++){
    int pag = stoi(cadena.substr(x,num));
    insertarPagina(arbp, pag);
    x+=num;
  }
}

//Cargar todo en el archivo texto
void cargarArchivoIndice(){
  Termino *raizTermino = NULL;
  string TerminoActual;
  ifstream archivo;

    archivo.open("Indice.txt",ios::in);
    if(archivo.fail()){
      cout<<"No se ha podido abrir el archivo de texto";
      exit(1);
    }

    cout<<"Indice de palabras, Estructuras de Datos y Algortimos\n"<<endl;
    int numero = 2;

    while(!archivo.eof()){
        string linea;
        getline(archivo, linea);
        
        //Buscar los términos principales, aquellos que comienzan por m
        if(linea[0] == 's'){
            int pos = BuscarNumero(linea);
            if(pos != -1){
              string tempTermino = linea.substr(1,pos-1);
              insertarTermino(raizTermino, tempTermino);
              TerminoActual = tempTermino;
              string pag = linea.substr(pos);
              extraerPag(buscarTermino(raizTermino, TerminoActual)->paginas, numero, pag);
            }
            else{
              string tempTermino = linea.substr(1,linea.length()-1);
              insertarTermino(raizTermino, tempTermino);
              TerminoActual = tempTermino;
            }
        //Buscar los subtérminos, aquellos que comiencen por n
          
        }else if(linea[0] == 't'){
            int pos = BuscarNumero(linea);
            if(pos != -1){
              string tempTermino = linea.substr(1,pos-1);
              insertarSubT(buscarTermino(raizTermino, TerminoActual)->subT, tempTermino);
              string pag = linea.substr(pos);
              extraerPag(buscarSubT(buscarTermino(raizTermino, TerminoActual)->subT, tempTermino)->paginas, numero, pag);
            }
            else{
              string tempTermino = linea.substr(1,linea.length()-1);
              insertarSubT(buscarTermino(raizTermino, TerminoActual)->subT, tempTermino);
            }  
        }
    }
    archivo.close();
    ordenarTerminos(raizTermino);
}

int main(){
  cargarArchivoIndice();
  
  getchar();
  return 0;
}