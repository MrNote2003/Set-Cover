#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int n = 0;
int m = 0;
vector<set<int>> subconjuntos;

vector<int> mejor_solucion;
int mejor_tamano = 999999;

void leer_archivo(string nombre_archivo) {
    ifstream archivo(nombre_archivo);
    
    string linea;
    getline(archivo, linea);
    
    stringstream ss_primera(linea);
    ss_primera >> n >> m;

    for (int i = 0; i < m; i++) {
        getline(archivo, linea);
        stringstream ss_linea(linea);
        
        int numero;
        set<int> conjunto;
        while (ss_linea >> numero) {
            conjunto.insert(numero);
        }
        subconjuntos.push_back(conjunto);
    }
    
    archivo.close();
}

vector<int> greedy_set_cover(set<int> universo) {
    set<int> no_cubiertos = universo;
    vector<int> seleccion;
    
    while (no_cubiertos.size() > 0) {
        int mejor_indice = -1;
        int mejor_cobertura = 0;
        set<int> mejor_subconjunto;
        
        for (int i = 0; i < subconjuntos.size(); i++) {
            int cobertura_actual = 0;
            
            for (int elemento : subconjuntos[i]) {
                if (no_cubiertos.count(elemento) > 0) {
                    cobertura_actual++;
                }
            }
            
            if (cobertura_actual > mejor_cobertura) {
                mejor_cobertura = cobertura_actual;
                mejor_indice = i;
                mejor_subconjunto = subconjuntos[i];
            }
        }
        
        if (mejor_indice == -1) {
            break;
        }
        
        seleccion.push_back(mejor_indice);
        
        for (int elemento : mejor_subconjunto) {
            no_cubiertos.erase(elemento);
        }
    }
    
    return seleccion;
}

void resolver(int indice, vector<int> seleccion_actual, set<int> elementos_cubiertos) {
    if (elementos_cubiertos.size() == n) {
        if (seleccion_actual.size() < mejor_tamano) {
            mejor_tamano = seleccion_actual.size();
            
            mejor_solucion.clear();
            for (int i = 0; i < seleccion_actual.size(); i++) {
                mejor_solucion.push_back(seleccion_actual[i]);
            }
        }
        return;
    }
    
    if (indice == subconjuntos.size()) {
        return;
    }
    
    resolver(indice + 1, seleccion_actual, elementos_cubiertos);
    
    set<int> nuevos_cubiertos = elementos_cubiertos;
    for (int elemento : subconjuntos[indice]) {
        nuevos_cubiertos.insert(elemento);
    }
    
    seleccion_actual.push_back(indice);
    resolver(indice + 1, seleccion_actual, nuevos_cubiertos);
    
    seleccion_actual.pop_back();
}

int main() {
    leer_archivo("caso1.txt");
    
    set<int> universo;
    for (int i = 1; i <= n; i++) {
        universo.insert(i);
    }
    
    cout << "Universo: " << n << " elementos | Subconjuntos disponibles: " << m << endl << endl;
    
    auto inicio_greedy = high_resolution_clock::now();
    vector<int> sol_greedy = greedy_set_cover(universo);
    auto fin_greedy = high_resolution_clock::now();
    double tiempo_greedy = duration_cast<nanoseconds>(fin_greedy - inicio_greedy).count() / 1e6;
    
    cout << "Solucion Mejorada Greedy" << endl;
    cout << "Subconjuntos elegidos: [";
    for (int i = 0; i < sol_greedy.size(); i++) {
        cout << sol_greedy[i] + 1;
        if (i < sol_greedy.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Cantidad utilizada: " << sol_greedy.size() << endl;
    cout << "Tiempo: " << tiempo_greedy << " ms" << endl;
    
    mejor_tamano = 999999;
    mejor_solucion.clear();
    
    vector<int> seleccion_vacia;
    set<int> cubiertos_vacios;
    
    auto inicio_back = high_resolution_clock::now();
    resolver(0, seleccion_vacia, cubiertos_vacios);
    auto fin_back = high_resolution_clock::now();
    double tiempo_back = duration_cast<nanoseconds>(fin_back - inicio_back).count() / 1e6;
    cout << "------------------------------------------------------" << endl;
    
    cout << "Solucion Base Backtracking" << endl;
    cout << "Subconjuntos elegidos: [";
    for (int i = 0; i < mejor_solucion.size(); i++) {
        cout << mejor_solucion[i] + 1;
        if (i < mejor_solucion.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Cantidad utilizada: " << mejor_solucion.size() << endl;
    cout << "Tiempo: " << tiempo_back << " ms" << endl;
    
    return 0;
}