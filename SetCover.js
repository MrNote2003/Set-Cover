const fs = require('fs');

let n = 0;
let m = 0;
let subconjuntos = [];

let mejorTamano = 999999;
let mejorSolucion = [];

function leerArchivo(nombreArchivo) {
    let data = fs.readFileSync(nombreArchivo, 'utf8');
    let lineas = data.split('\n');

    let primeraLinea = lineas[0].trim().split(/\s+/);
    n = parseInt(primeraLinea[0]);
    m = parseInt(primeraLinea[1]);

    for (let i = 1; i <= m; i++) {
        let numerosTexto = lineas[i].trim().split(/\s+/);
        let conjunto = new Set();
        
        for (let j = 0; j < numerosTexto.length; j++) {
            conjunto.add(parseInt(numerosTexto[j]));
        }
        
        subconjuntos.push(conjunto);
    }
}

function greedySetCover(universo) {
    let noCubiertos = new Set();
    for (let elemento of universo) {
        noCubiertos.add(elemento);
    }
    
    let seleccion = [];

    while (noCubiertos.size > 0) {
        let mejorIndice = -1;
        let mejorCobertura = 0;
        let mejorSubconjunto = new Set();

        for (let i = 0; i < subconjuntos.length; i++) {
            let coberturaActual = 0;
            
            for (let elemento of subconjuntos[i]) {
                if (noCubiertos.has(elemento)) {
                    coberturaActual++;
                }
            }

            if (coberturaActual > mejorCobertura) {
                mejorCobertura = coberturaActual;
                mejorIndice = i;
                mejorSubconjunto = subconjuntos[i];
            }
        }

        if (mejorIndice === -1) {
            break;
        }

        seleccion.push(mejorIndice);

        for (let elemento of mejorSubconjunto) {
            noCubiertos.delete(elemento);
        }
    }
    return seleccion;
}

function resolver(indice, seleccionActual, elementosCubiertos) {
    if (elementosCubiertos.size === n) {
        if (seleccionActual.length < mejorTamano) {
            mejorTamano = seleccionActual.length;
            
            mejorSolucion = [];
            for (let i = 0; i < seleccionActual.length; i++) {
                mejorSolucion.push(seleccionActual[i]);
            }
        }
        return;
    }

    if (indice === subconjuntos.length) {
        return;
    }

    resolver(indice + 1, seleccionActual, elementosCubiertos);

    let nuevosCubiertos = new Set();
    for (let elemento of elementosCubiertos) {
        nuevosCubiertos.add(elemento);
    }
    for (let elemento of subconjuntos[indice]) {
        nuevosCubiertos.add(elemento);
    }

    seleccionActual.push(indice);
    resolver(indice + 1, seleccionActual, nuevosCubiertos);
    
    seleccionActual.pop();
}

leerArchivo('caso1.txt');

let universo = new Set();
for (let i = 1; i <= n; i++) {
    universo.add(i);
}

console.log("Universo: " + n + " elementos | Subconjuntos disponibles: " + m + "\n");

let inicioGreedy = performance.now();
let solGreedy = greedySetCover(universo);
let finGreedy = performance.now();

console.log("Solucion Mejorada Greedy");
let resultadoGreedy = [];
for (let i = 0; i < solGreedy.length; i++) {
    resultadoGreedy.push(solGreedy[i] + 1);
}
console.log("Subconjuntos elegidos: [" + resultadoGreedy.join(", ") + "]");
console.log("Cantidad utilizada: " + solGreedy.length);
let tiempoGreedy = finGreedy - inicioGreedy;
console.log("Tiempo: " + tiempoGreedy.toFixed(4) + " ms");

mejorTamano = 999999;
mejorSolucion = [];

let inicioBack = performance.now();
resolver(0, [], new Set());
let finBack = performance.now();
console.log("------------------------------------------------------");
console.log("Solucion Base Backtracking");
let resultadoBack = [];
for (let i = 0; i < mejorSolucion.length; i++) {
    resultadoBack.push(mejorSolucion[i] + 1);
}
console.log("Subconjuntos elegidos: [" + resultadoBack.join(", ") + "]");
console.log("Cantidad utilizada: " + mejorSolucion.length);
let tiempoBack = finBack - inicioBack;
console.log("Tiempo: " + tiempoBack.toFixed(4) + " ms");