import time

def leer_archivo(nombre_archivo):
    archivo = open(nombre_archivo, 'r')
    lineas = archivo.readlines()
    archivo.close()

    primera_linea = lineas[0].split()
    n = int(primera_linea[0])
    m = int(primera_linea[1])

    subconjuntos = []
    
    for i in range(1, m + 1):
        numeros_texto = lineas[i].split()
        conjunto = set()
        for num in numeros_texto:
            conjunto.add(int(num))
        subconjuntos.append(conjunto)
            
    return n, m, subconjuntos

def greedy_set_cover(universo, subconjuntos):
    no_cubiertos = set(universo)
    seleccion = []
    
    while len(no_cubiertos) > 0:
        mejor_indice = -1
        mejor_cobertura = 0
        mejor_subconjunto = set()
        
        for i in range(len(subconjuntos)):
            elementos_utiles = no_cubiertos.intersection(subconjuntos[i])
            cobertura_actual = len(elementos_utiles)
            
            if cobertura_actual > mejor_cobertura:
                mejor_cobertura = cobertura_actual
                mejor_indice = i
                mejor_subconjunto = subconjuntos[i]
                
        if mejor_indice == -1:
            break
            
        seleccion.append(mejor_indice)
        
        for elemento in mejor_subconjunto:
            if elemento in no_cubiertos:
                no_cubiertos.remove(elemento)
        
    return seleccion

def resolver(indice, seleccion_actual, elementos_cubiertos, universo, subconjuntos, mejor_solucion, mejor_tamano):
    if len(elementos_cubiertos) == len(universo):
        if len(seleccion_actual) < mejor_tamano:
            mejor_tamano = len(seleccion_actual)
            mejor_solucion = []
            for num in seleccion_actual:
                mejor_solucion.append(num)
        return mejor_solucion, mejor_tamano
    
    if indice == len(subconjuntos):
        return mejor_solucion, mejor_tamano
        
    mejor_solucion, mejor_tamano = resolver(indice + 1, seleccion_actual, elementos_cubiertos, universo, subconjuntos, mejor_solucion, mejor_tamano)

    nuevos_cubiertos = set(elementos_cubiertos)
    for elemento in subconjuntos[indice]:
        nuevos_cubiertos.add(elemento)
        
    seleccion_actual.append(indice)
    mejor_solucion, mejor_tamano = resolver(indice + 1, seleccion_actual, nuevos_cubiertos, universo, subconjuntos, mejor_solucion, mejor_tamano)
    
    seleccion_actual.pop()
    
    return mejor_solucion, mejor_tamano

def backtracking_set_cover(universo, subconjuntos):
    mejor_solucion = []
    mejor_tamano = 999999

    mejor_solucion, mejor_tamano = resolver(0, [], set(), universo, subconjuntos, mejor_solucion, mejor_tamano)

    return mejor_solucion

nombre_archivo = 'SetCover/caso1.txt'
n, m, subconjuntos = leer_archivo(nombre_archivo)

universo = set()
for i in range(1, n + 1):
    universo.add(i)
    
print(f"Universo: {n} elementos | Subconjuntos disponibles: {m}\n")
    
inicio_greedy = time.time()
solucion_greedy = greedy_set_cover(universo, subconjuntos)
fin_greedy = time.time()
    
print("Solución Mejorada Greedy")
resultado_greedy = []
for i in solucion_greedy:
    resultado_greedy.append(i + 1)
print(f"Subconjuntos elegidos: {resultado_greedy}")
print(f"Cantidad utilizada: {len(solucion_greedy)}")
print(f"Tiempo: {(fin_greedy - inicio_greedy) * 1000:.4f} ms")
print("------------------------------------------------------")
inicio_back = time.time()
solucion_backtracking = backtracking_set_cover(universo, subconjuntos)
fin_back = time.time()
    
print("Solución Base Backtracking")
resultado_back = []
for i in solucion_backtracking:
    resultado_back.append(i + 1)
print(f"Subconjuntos elegidos: {resultado_back}")
print(f"Cantidad utilizada: {len(solucion_backtracking)}")
print(f"Tiempo: {(fin_back - inicio_back) * 1000:.4f} ms")