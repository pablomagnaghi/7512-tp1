#include "Filtros.h"
#include "string.h"
#include "strings.h"

void Crear_Lista_Filtros(TListaSimple *Lista_Filtros, FILE *parchivofiltros)
{
/* Variable aux que se insertará como elemento de la lista una vez leida 
 * una linea del archivo de filtros*/
TFilter aux;
/*Variables que seràn los tipos de datos_filtros segun la funcion a invocar*/
int cant;
TPalabra palabra;
/*Variables que contendràn temporariamente lo leido del archivo*/
char linea[100];
char funcion[35];
char parametros[65];
/*Variables controladoras de los ciclos*/
int j,i,error_filtro;
/* Se crea una listra de elementos de tipo filtro*/
    ls_Crear(Lista_Filtros, sizeof(TFilter));
    
     while (!feof(parchivofiltros))   /*mientras no sea el fin del archivo..*/
      {
/* Se levanta una nueva linea del archivo*/    	 
    	 fscanf(parchivofiltros,"%[^\n]\n",linea);
    	 strcat(linea,"\0");
		 i=0;
		 j=0;
/* Se recupera el nombre del filtro*/		  	
         while ( (linea[i]!=' ') && (linea[i] != '\0'))
         {	
           funcion[j] = linea[i];
           i++;
           j++;
		 }
	     
         funcion[j++]= '\0';
/* Si la linea de definicion no llego a su fin se intenta recuperar los
 * parametros.*/
         if (linea[i] != '\0')
         {
        	 i++;
        	 j=0;
        	 while ((linea[i]!=' ') && (linea[i] != '\0'))
        	 {	
        		parametros[j] = linea[i];
        	 	i++;
        	 	j++;
        	 }    
        	 parametros[j++]='\0'; 
         }

/* Se compara el nombre de filtro recuperado para asignar
 * un puntero a la funcion y a los parametros si
 * corresponde.*/ 
/* Se inicia la variable de error en filtro en 0 */
         error_filtro = 0;
         
          if (strcmp(funcion,"MAYUSCULAS")==0)
		  {
            aux.fnProceso=Pasar_a_mayuscula_Procesar;
            aux.datos_filtro=NULL; 
          }
          else if (strcmp(funcion,"MINUSCULAS")==0)
          {
            aux.fnProceso=Pasar_a_minuscula_Procesar;
            aux.datos_filtro=NULL; 
          }
          else if (strcmp(funcion,"PRIMEROS")==0)
          {
            cant= atoi(parametros);
            aux.fnProceso=PrimerosN_procesar;
            aux.datos_filtro=malloc(sizeof(Primeros_Dato));
            PrimerosN_Crear(aux.datos_filtro,cant);
          }
          else if (strcmp(funcion,"ULTIMOS")==0)
          {
            cant= atoi(parametros);
            aux.fnProceso=UltimosN_procesar;
            aux.datos_filtro=malloc(sizeof(Ultimos_Dato));
            UltimosN_Crear(aux.datos_filtro,cant);
          }
          else if (strcmp(funcion,"EXTRAER")==0)
          {
             cant= atoi(parametros);
             aux.fnProceso=ExtraerN_procesar;
             aux.datos_filtro=malloc(sizeof(Extraer_Dato));
             ExtraerN_Crear(aux.datos_filtro,cant);
          }
          else if (strcmp(funcion,"FILTRAR")==0)
          {
            strcpy(palabra,parametros);
            aux.fnProceso=FiltrarPalabra_procesar;
            aux.datos_filtro=malloc(sizeof(FiltrarPalabra_Dato));
            FiltrarPalabra_Crear(aux.datos_filtro,palabra);
          }
        else if (strcmp(funcion,"ORDENAR")==0)
          {
            strcpy(palabra,parametros);
            aux.fnProceso=Ordenar_procesar;
            aux.datos_filtro=malloc(sizeof(Ordenar_Dato));
            Ordenar_Crear(aux.datos_filtro,palabra);
          }
          else if (strcmp(funcion,"ELIMINAR_REP_CONSEC")==0)
          {
            aux.fnProceso=EliminarRepetidas_procesar;
            aux.datos_filtro=NULL;
          }
          else if (strcmp(funcion,"CONTAR_CONSEC")==0)
          {
            aux.fnProceso=ContarRepetidasConsecutivas_procesar;
            aux.datos_filtro=NULL;
          }
          else
          {
        	error_filtro = 1;
          }
    
        if(!error_filtro)
        {
          if (  ls_Vacia(*(Lista_Filtros)) )
          ls_Insertar(Lista_Filtros,LS_PRIMERO,&aux);
          else
          ls_Insertar(Lista_Filtros,LS_SIGUIENTE,&aux);
        }
  } /*Fin del while*/
/* Se cierra el archivo de definiciones de filtro*/     
       fclose(parchivofiltros);	
}	     
/*Fin de la función*/	



void Destruir_Lista_Filtros(TListaSimple * Lista_Filtros)
{   
    TFilter aux; 
    ls_MoverCorriente(Lista_Filtros,LS_PRIMERO);
    ls_ElemCorriente(*(Lista_Filtros),&aux);
    free(aux.datos_filtro);
    while (ls_MoverCorriente(Lista_Filtros,LS_SIGUIENTE))
    {
     ls_ElemCorriente(*(Lista_Filtros),&aux);
     free(aux.datos_filtro);
    }
    ls_Vaciar(Lista_Filtros);    
}    




/*Mayuscula*/  

/* Pre: entrada y salida creados.
datos_filtro apunta a una estructura conocida por la función 
eof indica si hay fin del archivo, es decir que las palabras que
están en el tubo
de entrada son todas las que quedan.
Post: Ejecuta el procesamiento.Pasa la palabra a mayusculas
Devuelve TRUE si eof == TRUE y procesó todas las palabras de la
entrada o
si es seguro que no va a emitir más palabras, sino devuelve FALSE.
Si pudo generar un resultado, lo emite en el tubo de salida y
devuelve el control al ejecutor.
*/
int Pasar_a_mayuscula_Procesar(void * datos_filtro, TTubo *entrada, 
							   TTubo *salida, int eof)
{  
TPalabra nueva;

  
  while(!C_Vacia(*(entrada)))
  {
    C_Sacar(entrada,nueva);
    int t=0;    
    while(nueva[t]!='\0')
    {    
      nueva[t] = toupper(nueva[t]);
      t++;
    }  
    C_Agregar(salida,nueva);
  }
  if (!eof) //Si el anterior no termino entonces quedan palabras por procesar
	  return FALSE;
  else		//Si el anterior termino entonces se proceso todo.
	  return TRUE;
}
   



/*Minuscula */    


/* Pre: entrada y salida creados.
datos_filtro apunta a una estructura conocida por la función 
eof indica si hay fin del archivo, es decir que las palabras que
están en el tubo
de entrada son todas las que quedan.
Post: Ejecuta el procesamiento. Pasa la palabra a minusculas
Devuelve TRUE si eof == TRUE y procesó todas las palabras de la
entrada o
si es seguro que no va a emitir más palabras, sino devuelve FALSE.
Si pudo generar un resultado, lo emite en el tubo de salida y
devuelve el control al ejecutor.
*/
int Pasar_a_minuscula_Procesar(void * datos_filtro, TTubo *entrada, 
TTubo *salida, int eof)
{  
   TPalabra nueva;
   
   while(!C_Vacia(*entrada))
   {
     C_Sacar(entrada,nueva);
     int t=0;
     while(nueva[t]!='\0')
     {
     nueva[t] = tolower(nueva[t]);
     t++;
     }
     C_Agregar(salida,nueva);
   }
   if (!eof) //Si el anterior no termino entonces quedan palabras por procesar
 	  return FALSE;
   else		//Si el anterior termino entonces se proceso todo.
 	  return TRUE;
}
 


/*Pasar primeros N*/
/*
pre: Estructura no creada
post: Estructura creada
*/
void PrimerosN_Crear(void *filtro,int n )
{
   Primeros_Dato *pPuntero;
   pPuntero = (Primeros_Dato*)filtro;
   pPuntero->numero=n;      
}

/* Pre: entrada y salida creados.
datos_filtro apunta a una estructura conocida por la función 
eof indica si hay fin del archivo, es decir que las palabras que
están en el tubo
de entrada son todas las que quedan.
Post: Ejecuta el procesamiento. Devuelve los primeros N del total
Devuelve TRUE si eof == TRUE y procesó todas las palabras de la
entrada o
si es seguro que no va a emitir más palabras, sino devuelve FALSE.
Si pudo generar un resultado, lo emite en el tubo de salida y
devuelve el control al ejecutor.
*/
int PrimerosN_procesar(void * datos_filtro, TTubo *entrada, 
TTubo *salida, int eof)
{
    TPalabra nueva;
    int n;
    Primeros_Dato *filtro=(Primeros_Dato*) datos_filtro;
    
    n=filtro->numero;
     
    while ((n >0)&&(!C_Vacia(*entrada)))
    { 
      C_Sacar (entrada,nueva);
      C_Agregar(salida,nueva);
      n--;                
    }
    filtro->numero = n;
    
    if (eof) // Si el anterior termino entonces se dejaron pasar como maximo N
    	return TRUE;
    else // Si el anterior no termino
    {
    	if (n > 0) // Si aun quedan vacantes para pasar..
    		return FALSE;
    	else	// Si ya pasaron N termino su tarea
    		return TRUE;
    }
    
}
         

void PrimerosN_destruir(Primeros_Dato *filtro)
{
   free(filtro);
}



/*
pre: Estructura no creada
post: Estructura creada
*/
void UltimosN_Crear(void *filtro,int n)
{
   Ultimos_Dato *pPuntero;
   pPuntero = (Ultimos_Dato*)filtro;
   pPuntero->numero=n;      
}


/* Pre: entrada y salida creados.
datos_filtro apunta a una estructura conocida por la función 
eof indica si hay fin del archivo, es decir que las palabras que
están en el tubo
de entrada son todas las que quedan.
Post: Ejecuta el procesamiento. Devuelve los ultimos N del total
Devuelve TRUE si eof == TRUE y procesó todas las palabras de la
entrada o
si es seguro que no va a emitir más palabras, sino devuelve FALSE.
Si pudo generar un resultado, lo emite en el tubo de salida y
devuelve el control al ejecutor.
*/
int UltimosN_procesar(void * datos_filtro, TTubo *entrada, 
					  TTubo *salida, int eof)
{
	if(eof) // Si el anterior termino se puede procesar los ultimos N
	{
    TPalabra nueva;
    int n;
    TPila aux,aux2;
    P_Crear(&aux,sizeof(TPalabra));
    P_Crear(&aux2,sizeof(TPalabra));
    
    Ultimos_Dato *filtro=(Ultimos_Dato *) datos_filtro;
    
    n=filtro->numero;
/* Invierto el orden de los elementos sacandolos de la cola y apilandolos.*/
    while (!C_Vacia(*entrada))
       {
         //Saco  un elemento de la cola y lo pongo en la pila
         C_Sacar (entrada,nueva);
         P_Agregar(&aux,&nueva);
       }
/* Saco los primeros N de la pila que son los ultimos N de la pila original.
 * Los apilo reinvirtiendo el orden para lograr el estado original.*/    
    while (!P_Vacia(aux) && n>0)
        {
        //Saco elemento un elemento de la pila y la guardo en la cola 
        //de salida   
        P_Sacar (&aux, nueva);
        P_Agregar (&aux2,nueva);
        n--;
        filtro->numero = n;
        }
/* Ya con el orden original saco los elementos a la cola de salida.*/    
    while (!P_Vacia(aux2))
    	{
    		P_Sacar (&aux2, nueva);
    	    C_Agregar (salida,nueva);
    	}
   return TRUE;
   }
   else
	   return FALSE;
}

void UltimosN_destruir(void *filtro)
{
    free(filtro);
 
}


/*
pre: Estructura no creada
post: Estructura creada
*/
void ExtraerN_Crear(void *filtro,int n)
{
   Extraer_Dato *pPuntero;
   pPuntero = (Extraer_Dato*)filtro;
   pPuntero->numero=n;      
}

/* Pre: entrada y salida creados.
datos_filtro apunta a una estructura conocida por la función 
eof indica si hay fin del archivo, es decir que las palabras que
están en el tubo de entrada son todas las que quedan.
Post: Ejecuta el procesamiento. Extrae el campo N del total
Devuelve TRUE si eof == TRUE y procesó todas las palabras de la
entrada o si es seguro que no va a emitir más palabras, sino devuelve FALSE.
Si pudo generar un resultado, lo emite en el tubo de salida y
devuelve el control al ejecutor.
*/
int ExtraerN_procesar(void * datos_filtro, TTubo *entrada, TTubo *salida, int eof){
	TPalabra nueva;
	int n;
	Extraer_Dato *filtro=(Extraer_Dato*) datos_filtro;
	n = filtro->numero;
	char* linea;
	char palabrasSeparadas[64][64];
	int cantPalabras;
	
	while ((!C_Vacia(*entrada))){ 
    	C_Sacar (entrada, nueva);
    	linea = nueva;
    	cantPalabras = 0;
    	separarPalabras(linea, palabrasSeparadas, &cantPalabras);
    	
    	if (n <= cantPalabras){
    		C_Agregar(salida, &palabrasSeparadas[n - 1]);
    	}
    }
	filtro->numero = n;
    
	if (eof) // Si el anterior termino entonces si habia N o mas se lo extrajo
		return TRUE;
	else{ // Si el anterior no termino
		if (n > 0) // Si aun quedan vacantes para pasar..
			return FALSE;
		else	// Si ya pasaron N termino su tarea
			return TRUE;
    }
}

void ExtraerN_destruir(void *filtro){
    free(filtro);
}

/*  PRE: -
 * 	POS: El filtro queda listo para ser utilizado.
 * 	
 * 	TPalabra palabra: Es la palabra que se desea filtrar.
 */
void FiltrarPalabra_Crear(void *filtro, TPalabra palabra){
	FiltrarPalabra_Dato *pPuntero;
	pPuntero = (FiltrarPalabra_Dato*)filtro;
	strcpy(pPuntero->palabra, palabra);
}

/*  PRE: El filtro debe haber sido creado. Los tubos deben haber sido creados.
 * 	POS: Se filtran del tubo de entrada la palabra deseada y se coloca el resultado en el tubo de salida.
 * 		 Se devuelve TRUE si se ha procesado el tubo por completo.
 */
int FiltrarPalabra_procesar(void *datos_filtro, TTubo *entrada, TTubo *salida, int eof){
	TPalabra palabraAux;
	char* linea;
	FiltrarPalabra_Dato *filtro = (FiltrarPalabra_Dato*)datos_filtro;
	linea = filtro->palabra;
	
	char palabrasSeparadas[64][64];
	int cantPalabras;
	separarPalabras(linea, palabrasSeparadas, &cantPalabras);
	
	int esPalabraAFiltrar = 0;
	int j;
	while (!C_Vacia(*entrada)){
		C_Sacar(entrada, palabraAux);
		esPalabraAFiltrar = 0;
		for (j = 0; (j < cantPalabras)&&(!esPalabraAFiltrar); j++ ){
			char* palabraAFiltrar = palabrasSeparadas[j];
			esPalabraAFiltrar = (! strcmp(palabraAux, palabraAFiltrar));
		}
		if (! esPalabraAFiltrar){
			C_Agregar(salida, palabraAux);
		}
	}
	
	if (eof) // Si el anterior termino entonces ya se proceso todo
	    	return TRUE;
	else // Si el anterior no termino
	    	return FALSE;
}

void separarPalabras(char cadena[], char palabrasSeparadas[64][64], int* cantPalabras){
	char palabraAux[64] = "";
	int i;
	int cantLetras = 0;
	int longitud = strlen(cadena);
	for (i = 0; i <= longitud; i++){
		if ((cadena[i] == '\0')||(cadena[i] == ';')||(cadena[i] == ',')||(cadena[i] == '\n')||(cadena[i] == ' ')){
			if (cantLetras > 0){
				int k;
				for (k = 0; k < cantLetras; k++){
					palabrasSeparadas[(*cantPalabras)][k] = palabraAux[k];
				}
				palabrasSeparadas[(*cantPalabras)][cantLetras] = '\0';
				(*cantPalabras)++;
				
				int j;
				int longitudAux = cantLetras;
				for (j = 0; j < longitudAux; j++){
					palabraAux[j] = '\0';
				}
				cantLetras = 0;
			}
		}
		else{
			palabraAux[cantLetras] = cadena[i];
			cantLetras++;
		}
	}
}

/*  PRE: El filtro debe haber sido creado.
 * 	POS: Se liberan los recursos utilizados por el filtro.
 */
void FiltrarPalabra_destruir(FiltrarPalabra_Dato *filtro){
   free(filtro);
   return;
}

/*  PRE: El orden debe ser ASC o DESC.
 * 	POS: El filtro queda listo para ser utilizado.
 * 	
 * 	int orden: Es el tipo de orden deseado (ASC o DESC).
 */
void Ordenar_Crear(void *filtro, TPalabra orden){
	Ordenar_Dato *pPuntero;
	pPuntero = (Ordenar_Dato*)filtro;
	strcpy(pPuntero->orden, orden);
}

/*  PRE: El filtro debe haber sido creado. Los tubos deben haber sido creados.
 * 	POS: Se le da el orden especificado al tubo de entrada y se coloca el resultado en el tubo de salida.
 * 		 Se devuelve TRUE si se ha procesado el tubo por completo.
 */
int Ordenar_procesar(void *datos_filtro, TTubo *entrada, TTubo *salida, int eof){
/* Si cuenta con todos los elementos para procesar...*/	
	if (eof)
	{
		TListaOrdenada listaOrdenada;
		
		char* orden;
		Ordenar_Dato *filtro = (Ordenar_Dato*)datos_filtro;
		orden = filtro->orden;
		char* ordenAux = "ASC";
		int longitud = strlen(ordenAux);
		if (! strncmp(ordenAux, orden, longitud)){
			lo_Crear(&listaOrdenada, sizeof(TPalabra), compararStrings1);
		}
		else{
			lo_Crear(&listaOrdenada, sizeof(TPalabra), compararStrings2);
		}

		int i;
		for (i = 0; !C_Vacia(*entrada); i++){
			TPalabra palabraAux;
			C_Sacar(entrada, &palabraAux);
			lo_Insertar(&listaOrdenada, &palabraAux);
		}
		
		lo_MoverCorriente(&listaOrdenada, LO_PRIMERO);
		while(!lo_Vacia(listaOrdenada)){
			TPalabra palabraAux;
			lo_ElemCorriente(listaOrdenada, &palabraAux);
			C_Agregar(salida, &palabraAux);
			lo_BorrarCorriente(&listaOrdenada);
			lo_MoverCorriente(&listaOrdenada, LO_PRIMERO);
		}
		
		lo_Vaciar(&listaOrdenada);
		
		return TRUE;
	}
	else
		return FALSE;
}

int compararStrings1(void* dato1, void* dato2){
    TPalabra* palabra1 = (TPalabra*)dato1;
    TPalabra* palabra2 = (TPalabra*)dato2;
    return strcmp(*palabra1, *palabra2);
}

int compararStrings2(void* dato1, void* dato2){
    int valor = compararStrings1(dato1, dato2);
	if (valor > 0)
    	return -1;
	else{
		if (valor == 0)
			return 0;
		else{
			return 1;
		}
	}
}

void imprimirString(void* dato){
	printf("%s\n", (char*)dato);
}

/*  PRE: El filtro debe haber sido creado.
 * 	POS: Se liberan los recursos utilizados por el filtro.
 */
void Ordenar_destruir(Ordenar_Dato *filtro){
	free(filtro);
	return;
}

/*  PRE: -
 * 	POS: El filtro queda listo para ser utilizado.
 */
void EliminarRepetidas_Crear(void *filtro){
	return;
}

/*  PRE: El filtro debe haber sido creado. Los tubos deben haber sido creados.
 * 	POS: Se eliminan las palabras repetidas en el tubo de entrada y se coloca el resultado en el tubo de salida.
 * 		 Se devuelve TRUE si se ha procesado el tubo por completo.
 */
int EliminarRepetidas_procesar(void *datos_filtro, TTubo *entrada, TTubo *salida, int eof){
	if (eof){
		TPalabra palabra;
		TPalabra palabraAnterior = "";
		while(!C_Vacia(*entrada)){
			C_Sacar(entrada, &palabra);
			if (C_Vacia(*entrada)){
				if (strcmp(palabra, palabraAnterior)){
					C_Agregar(salida, &palabraAnterior);
					C_Agregar(salida, &palabra);
				}
				else{
					C_Agregar(salida, &palabra);
				}
			}
			else{
				if (strcmp(palabra, palabraAnterior)&&strcmp("",palabraAnterior)){
					C_Agregar(salida, &palabraAnterior);
				}
			}
			strcpy(palabraAnterior, palabra);
		}
	
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/*  PRE: El filtro debe haber sido creado.
 * 	POS: Se liberan los recursos utilizados por el filtro.
 */
void EliminarRepetidas_destruir(Ordenar_Dato *filtro){
	return;
}

void ContarRepetidasConsecutivas_Crear(void *filtro){
	return;
}

int ContarRepetidasConsecutivas_procesar(void *datos_filtro, TTubo *entrada, TTubo *salida, int eof){
	if (eof){
		TPalabra palabra;
		TPalabra palabraAnterior;
		TPalabra final;
		
		int cantidad=1;
		char cantidadChar[4];
		
		palabraAnterior[0] = '\0';
		cantidadChar[0] = '\0';
		final[0] = '\0';
		/* Mientras haya elementos para sacar*/
		while(!C_Vacia(*entrada)){
			/* Saco una palabra de la cola*/
			C_Sacar(entrada, palabra);
			/* Si es igual a la anterior acumulo*/		
			if (strcmp(palabra, palabraAnterior) == 0){
				cantidad++;
				/* Si fue la ultima palabra se la agrega en la cola de salida.*/
				if (C_Vacia(*entrada)){
					/* Se pasa a char la cantidad*/
					itoa(cantidad,cantidadChar,10);
					/* Se concatena para conformar el formato NNNN;palabra*/
					strcpy(final, cantidadChar);
					strcat(final, ";");
					strcat(final, palabraAnterior);
					/* Se agrega en la cola de salida*/	  
					C_Agregar(salida, final);
				}
			}
			/* Si es distinta..*/			
			else{
				/* Si no es la primera, se la agrega a la cola de salida.
				 * Tambien se verifica que la cadena final no sea mayor a 64*/				
				if ((strlen(palabraAnterior) > 0) && (strlen(palabraAnterior) < 60)){
					/* Se pasa a char la cantidad*/
					itoa(cantidad, cantidadChar, 10);
					/* Se concatena para conformar el formato NNNN;palabra*/
					strcpy(final, cantidadChar);
					strcat(final, ";");
					strcat(final, palabraAnterior);
					/* Se agrega en la cola de salida*/
					C_Agregar(salida, final);
					final[0] = '\0';
					cantidadChar[0] = '\0';
				}
				/* Se setea como palabra anterior la ultima sacada
				 * y se setea como cantidad 1*/	      
				strcpy (palabraAnterior, palabra);
				cantidad = 1;
				/* Si fue la ultima palabra se la agrega en la cola de salida.*/
				if (C_Vacia(*entrada)){
					/* Se pasa a char la cantidad*/
					itoa(cantidad,cantidadChar,10);
					/* Se concatena para conformar el formato NNNN;palabra*/
					strcpy(final, cantidadChar);
					strcat(final, ";");
					strcat(final, palabraAnterior);
					/* Se agrega en la cola de salida*/	  
					C_Agregar(salida, final);
				}
			}
		}
		return TRUE;
	}
	else
		return FALSE;
}

void ContarRepetidasConsecutivas_destruir(void *filtro){
	return;
}
