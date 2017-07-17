
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void imprimirVector(float vector[], int tamanio){
	int i;
	printf("{");
	for(i = 0; i < tamanio; i++){
		printf("%f ", vector[i]);
	}
	printf("}\n");
}

float valorAbsoluto(float n){
	if (n < 0)
		return n*-1;
	return n;
}

float normaInfinito(float vector[], int dimension){
	float aux = valorAbsoluto(vector[0]);
	int i;
	for(i = 1; i < dimension; i++){
		if (valorAbsoluto(vector[i]) > aux)
			aux = valorAbsoluto(vector[i]);
	}
	return aux;
}

void restar(float vectorA[], float vectorB[], float vectorC[], int dimension){
	int i;
	for(i = 0; i < dimension; i++){
		vectorC[i] = vectorA[i] - vectorB[i];
	}
}

int condicionDeCorte(float rtol, float vectorX[], float vectorXAnterior[], int dimension){
	int corte = 0;

	float resta[dimension];
	restar(vectorX, vectorXAnterior, resta, dimension);
	float numerador = normaInfinito(resta, dimension);
	float denominador = normaInfinito(vectorX, dimension);

	if (numerador / denominador <= rtol)
		corte = 1;

	return corte;
}

void generarVectoresDiagonales(float diagonal1[], float diagonal2[], float diagonal3[],
		                       float diagonal4[], int dimensionMatriz){

	int i;

	/*/ genero la diagonal que tiene los valoes 222333 o 222444333 o 222444444333.../*/
	for(i = 0; i < 3; i++){
		diagonal1[i] = 2;
		diagonal1[dimensionMatriz - 1 - i] = 3;
	}
	if (dimensionMatriz > 6){
		for(i = 3; i < dimensionMatriz - 3; i++){
			diagonal1[i] = 4;
		}
	}

	/*/ genero la diagonal que tiene los valoes/*/
	/*/ -1/2 -1/2 0 -1/2 -1/2/*/
	/*/ -1/2 -1/2 0 -1 -1 0 -1/2 -1/2/*/
	/*/ -1/2 -1/2 0 -1 -1 0 -1 -1 0 -1/2 -1/2/*/
	/*/ .../*/
	for(i = 0; i < 2; i++){
		diagonal2[i] = -0.5f;
		diagonal2[dimensionMatriz - 2 - i] = -0.5f;
	}
	for(i = 2; i < dimensionMatriz - 3; i++){
		if ((i+1) % 3 != 0)
			diagonal2[i] = -1;
	}
	for(i = 2; i < dimensionMatriz - 3; i++){
		if ((i+1) % 3 == 0)
			diagonal2[i] = 0;
	}

	/*/ genero la diagonal que tiene los valoes 0.../*/
	for(i = 0; i < dimensionMatriz - 2; i++){
		diagonal3[i] = 0;
	}
	for(i = dimensionMatriz - 2; i < dimensionMatriz; i++){
		diagonal3[i] = 0;
	}

	/*/ genero la diagonal que tiene los valoes -1.../*/
	for(i = 0; i < dimensionMatriz - 3; i++){
		diagonal4[i] = -1;
	}
	for(i = dimensionMatriz - 3; i < dimensionMatriz; i++){
		diagonal4[i] = 0;
	}
}

void generarFila(float* arregloDiagonales[], int numeroFila, int dimension, float filaRetorno[]){

	int i;
	int j = 0;
	int k = 0;
	for(i = numeroFila - 1; i > 0; i--){
		if (i < 4){
			filaRetorno[j] = (arregloDiagonales[i])[j];
		}
		else{
			filaRetorno[j] = 0;
		}
		k++;
		j++;
	}
	for(i = 0; i < dimension - k; i++){
		if (i < 4)
			filaRetorno[j] = (arregloDiagonales[i])[k];
		else
			filaRetorno[j] = 0;
		j++;
	}
}

float calculoX_SP(float filaMatriz[], int tamanioFila, float vectorXAnterior[],
		          float b, float omega, int i){

	float sumatoria = 0;
	int j;
	float x;
	float a;

	for(j = 0; j < tamanioFila; j++){
		if (j != i - 1){
			x = vectorXAnterior[j];
			a = filaMatriz[j];
			sumatoria += x * a;
		}
	}

	float resultado = ((((b - sumatoria) / filaMatriz[i-1]) - vectorXAnterior[i-1]) * omega) + vectorXAnterior[i-1];

	vectorXAnterior[i-1] = resultado;

	return resultado;
}

int generarTablaSOR_SP_Diag(float* arregloDiagonales[], int dimension,
		                float vectorX[], float vectorB[], float omega,
		                int numeroIteraciones, float rtol, float* tabla[]){

	int i,j;

	float* copia = (float*)malloc(dimension * sizeof(float));
	for(i = 0; i < dimension; i++){
		copia[i] = vectorX[i];
	}
	tabla[0] = copia;

	float fila[dimension];
	int corte = 0;
	int it = 0;
	for(i = 1; (i <= numeroIteraciones); i++){
		copia = (float*)malloc(dimension * sizeof(float));
		for(j = 0; j < dimension; j++){
			generarFila(arregloDiagonales, j+1, dimension, fila);
			copia[j] = calculoX_SP(fila, dimension, vectorX, vectorB[j], omega, j+1);
		}
		tabla[i] = copia;
		corte = condicionDeCorte(rtol, copia, tabla[i-1], dimension);
		it++;
		if (corte)
			break;
	}

	if (corte)
		return it;
	else
		return numeroIteraciones;
}

void puntoA(int dimension){

	printf("\n******************************** PUNTO A ********************************\n");

	printf("\nDimension: %d\n", dimension);

	float diagonal1[dimension];
	float diagonal2[dimension];
	float diagonal3[dimension];
	float diagonal4[dimension];

	generarVectoresDiagonales(diagonal1, diagonal2, diagonal3, diagonal4, dimension);

	printf("\nDiagonales:\n");
	imprimirVector(diagonal1, dimension);
	imprimirVector(diagonal2, dimension-1);
	imprimirVector(diagonal3, dimension-2);
	imprimirVector(diagonal4, dimension-3);

	float* arregloDiagonales[dimension];
	arregloDiagonales[0] = diagonal1;
	arregloDiagonales[1] = diagonal2;
	arregloDiagonales[2] = diagonal3;
	arregloDiagonales[3] = diagonal4;
	/*
	int filaNumero;
	float fila[dimension];
	for(filaNumero = 1; filaNumero <= dimension; filaNumero++){
		generarFila(arregloDiagonales, filaNumero, dimension, fila);
		printf("\nFila %d: ", filaNumero);
		imprimirVector(fila, dimension);
	}*/
}

void puntoB(int dimension){

	printf("\n******************************** PUNTO B ********************************\n");

	int i,j;
	printf("\nDimension: %d\n", dimension);

	float diagonal1[dimension];
	float diagonal2[dimension];
	float diagonal3[dimension];
	float diagonal4[dimension];

	generarVectoresDiagonales(diagonal1, diagonal2, diagonal3, diagonal4, dimension);

	float* arregloDiagonales[dimension];
	arregloDiagonales[0] = diagonal1;
	arregloDiagonales[1] = diagonal2;
	arregloDiagonales[2] = diagonal3;
	arregloDiagonales[3] = diagonal4;

	float vectorX[dimension];
	for(i = 0; i < dimension; i++){
		vectorX[i] = 1;
	}

	printf("\nVectorX: ");
	imprimirVector(vectorX, dimension);

	float resultado[dimension];

	float suma;
	float fila[dimension];
	for(i = 0; i < dimension; i++){
		suma = 0;
		generarFila(arregloDiagonales, i+1, dimension, fila);
		for(j = 0; j < dimension; j++){
			suma += fila[j];
		}
		resultado[i] = suma;
	}

	printf("\nVectorB GENERADO: ");
	imprimirVector(resultado, dimension);
}

int puntoC(int dimension, float omega, float rtol, int numeroIteraciones, int imprimir){

	if (imprimir == 1){
		printf("\n******************************** PUNTO C ********************************\n");
		printf("\nDimension: %d\n", dimension);
		printf("\nN° de Iteraciones: %d\n", numeroIteraciones);
		printf("\nOmega: %f\n", omega);
	}

	int i,j;

	float diagonal1[dimension];
	float diagonal2[dimension];
	float diagonal3[dimension];
	float diagonal4[dimension];

	generarVectoresDiagonales(diagonal1, diagonal2, diagonal3, diagonal4, dimension);

	float* arregloDiagonales[dimension];
	arregloDiagonales[0] = diagonal1;
	arregloDiagonales[1] = diagonal2;
	arregloDiagonales[2] = diagonal3;
	arregloDiagonales[3] = diagonal4;

	float vectorB[dimension];
	float suma;
	float fila[dimension];
	for(i = 0; i < dimension; i++){
		suma = 0;
		generarFila(arregloDiagonales, i+1, dimension, fila);
		for(j = 0; j < dimension; j++){
			suma += fila[j];
		}
		vectorB[i] = suma;
	}

	float vectorX[dimension];
	for(i = 0; i < dimension; i++){
		vectorX[i] = 0;
	}

	if (imprimir == 1){
		printf("\nVectorB: ");
		imprimirVector(vectorB, dimension);
		printf("\nVectorX: ");
		imprimirVector(vectorX, dimension);
	}

	float* tabla[numeroIteraciones + 1];
	int it = generarTablaSOR_SP_Diag(arregloDiagonales, dimension, vectorX, vectorB, omega, numeroIteraciones, rtol, tabla);

	if (imprimir == 1){
		printf("\nTABLA: \n");
		for(i = 0; i <= it; i++){
			printf("%d: ", i);
			imprimirVector(tabla[i], dimension);
		}
	}

	return it;
}

void puntoD(int dimension, float rtol, float omegaI, float omegaF,
		    float* oIRed, float* oFRed, int imprimir){

	if (imprimir == 1){
		printf("\n******************************** PUNTO D ********************************\n");
		printf("\nDimension: %d\n", dimension);
		printf("\nRTOL: %.3f\n", rtol);
	}

	float omega;
	int j = 0;
	int cantMuestras = 50;
	float tabla[cantMuestras][2];

	if (imprimir == 1)
		printf("\nOmega\t\tN° Iteraciones");
	float step = (omegaF - omegaI) / cantMuestras;
	for(omega = omegaI; omega < omegaF; omega += step){
		tabla[j][0] = omega;
		tabla[j][1] = puntoC(dimension, omega, rtol, 1000, 0);
		if (imprimir == 1)
			printf("\n%.4f\t\t%.0f", tabla[j][0], tabla[j][1]);
		j++;
	}

	*oIRed = omegaI;
	*oFRed = omegaF;
	int itMin = 1000;

	int i;
	for(i = 0; i < j; i++){
		if (tabla[i][1] < itMin)
			itMin = tabla[i][1];

	}
	for(i = 0; i < j; i++){
		if (tabla[i][1] == itMin){
			*oIRed = tabla[i][0];
			break;
		}
	}
	for(i = j; i >= 0; i--){
		if (tabla[i][1] == itMin){
			*oFRed = tabla[i][0];
			break;
		}
	}
	if (imprimir == 1){
		printf("\n");
		printf("\nLa cantidad minima de iteraciones fue: %d", itMin);
		printf("\nEl valor acotado de omega inicial es: %.4f", *oIRed);
		printf("\nEl valor acotado de omega final es: %.4f", *oFRed);
		printf("\n");
	}
}


void puntoE(int dimension, float rtol, float omegaI, float omegaF,
		    float* oIRed, float* oFRed, int imprimir){

	if (imprimir == 1){
		printf("\n******************************** PUNTO E ********************************\n");
		printf("\nDimension: %d\n", dimension);
		printf("\nRTOL: %.2f\n", rtol);
	}

	float omega;
	int j = 0;
	int cantMuestras = 25;
	float tabla[cantMuestras][2];

	if (imprimir == 1)
		printf("\nOmega\t\tN° Iteraciones");
	float step = (omegaF - omegaI) / cantMuestras;
	for(omega = omegaI; omega < omegaF; omega += step){
		tabla[j][0] = omega;
		tabla[j][1] = puntoC(dimension, omega, rtol, 1000, 0);
		if (imprimir == 1)
			printf("\n%.4f\t\t%.0f", tabla[j][0], tabla[j][1]);
		j++;
	}

	*oIRed = omegaI;
	*oFRed = omegaF;
	int itMin = 1000;

	int i;
	for(i = 0; i < j; i++){
		if (tabla[i][1] < itMin)
			itMin = tabla[i][1];

	}
	for(i = 0; i < j; i++){
		if (tabla[i][1] == itMin){
			*oIRed = tabla[i][0];
			break;
		}
	}
	for(i = j; i >= 0; i--){
		if (tabla[i][1] == itMin){
			*oFRed = tabla[i][0];
			break;
		}
	}
	if (imprimir == 1){
		printf("\n");
		printf("\nLa cantidad minima de iteraciones fue: %d", itMin);
		printf("\nEl valor acotado de omega inicial es: %.4f", *oIRed);
		printf("\nEl valor acotado de omega final es: %.4f", *oFRed);
		printf("\n");
	}
}

float puntoF(int dimension, int imprimir){

	if (imprimir == 1){
		printf("\n\n******************************** PUNTO F ********************************\n");
		printf("\nDimension: %d\n", dimension);
	}

	int i,j;

	float omega = 1.00f;
	int numeroIteraciones = 50;
	float rtol = 0.001f;

	float diagonal1[dimension];
	float diagonal2[dimension];
	float diagonal3[dimension];
	float diagonal4[dimension];

	generarVectoresDiagonales(diagonal1, diagonal2, diagonal3, diagonal4, dimension);

	float* arregloDiagonales[dimension];
	arregloDiagonales[0] = diagonal1;
	arregloDiagonales[1] = diagonal2;
	arregloDiagonales[2] = diagonal3;
	arregloDiagonales[3] = diagonal4;

	float vectorB[dimension];
	float suma;
	float fila[dimension];
	for(i = 0; i < dimension; i++){
		suma = 0;
		generarFila(arregloDiagonales, i+1, dimension, fila);
		for(j = 0; j < dimension; j++){
			suma += fila[j];
		}
		vectorB[i] = suma;
	}

	float vectorX[dimension];
	for(i = 0; i < dimension; i++){
		vectorX[i] = 0;
	}

	float* tabla[numeroIteraciones + 1];
	int it = generarTablaSOR_SP_Diag(arregloDiagonales, dimension, vectorX, vectorB, omega, numeroIteraciones, rtol, tabla);

	for(i = 0; i < dimension; i++){
		vectorX[i] = 1;
	}

	float vectorXSemilla[dimension];
	for(i = 0; i < dimension; i++){
		vectorXSemilla[i] = 0;
	}

	float error,p;
	float restaX[dimension];

	if (imprimir == 1){
		printf("\nTABLA CALCULO DE P:\n");
		printf("\nk\tERROR = ||x(k)-x||\tpGS = 10^(log(ERROR)/k)");
	}
	for(i = 1; i <= it; i++){
		if (imprimir == 1){
			printf("\n");
			printf("%d\t", i);
		}
		restar(tabla[i], vectorX, restaX, dimension);
		error = normaInfinito(restaX, dimension);
		if (imprimir == 1)
			printf("%f\t\t", error);

		p = pow(10, log10(error)/i);
		if (imprimir == 1)
			printf("%f\t", p);
	}

	return p;
}

void puntoG(){

	printf("\n\n******************************** PUNTO G ********************************\n");

	float p6 = puntoF(6, 0);
	float p9 = puntoF(9, 0);
	float p12 = puntoF(12, 0);
	float p30 = puntoF(30, 0);

	printf("\nValores de 'p' calculados en Punto F:");
	printf("\nN = 6: p=%f", p6);
	printf("\nN = 9: p=%f", p9);
	printf("\nN = 12: p=%f", p12);
	printf("\nN = 30: p=%f", p30);

	printf("\n\nValores de 'Woptimo' calculados a partir de 'p':");
	printf("\nN = 6: Woptimo=%f", (2 / (1 + sqrt(1 - p6))));
	printf("\nN = 9: Woptimo=%f", (2 / (1 + sqrt(1 - p9))));
	printf("\nN = 12: Woptimo=%f", (2 / (1 + sqrt(1 - p12))));
	printf("\nN = 30: Woptimo=%f", (2 / (1 + sqrt(1 - p30))));
}

int main(int argc, char* argv[]){

	int dimension;
	printf("\nIngrese el valor de 'n' (6/9/12/30): ");
	fscanf(stdin, "%d", &dimension);
	printf("\n");

	puntoA(dimension);

	puntoB(dimension);

	/*/ 1.0f es omega, 0.001 es RTOL, 100 es la cant. max. de it.,/*/
	/*/ 1 para que se muestre por pantalla el resultado/*/
	puntoC(dimension, 1.00f, 0.001f, 100, 1);

	/*/ 0.001f es RTOL,/*/
	/*/ 1.0f es el valor de inicio de omega,/*/
	/*/ 2.0f es el valor final de omega,/*/
	/*/ 1 para que se muestre por pantalla el resultado/*/
	float omegaInicialReducido;
	float omegaFinalReducido;
	puntoD(dimension, 0.001f, 1.0f, 2.0f, &omegaInicialReducido, &omegaFinalReducido, 1);

	/*/ 1.0f es el valor de inicio de omega,/*/
	/*/ 2.0f es el valor final de omega/*/

	puntoE(dimension, 0.01f, 1.1f, 1.4f, &omegaInicialReducido, &omegaFinalReducido, 1);
	/*/ 1 para que se muestre por pantalla el resultado/*/
	puntoF(dimension, 1);

	puntoG();


	return 0;
}


