/*
*
* Archivo: lib_archivos.c
*
* Nombres:
*	Alejandra Cordero / Carnet: 12-10645
*	Pablo Maldonado   / Carnet: 12-10561
*
* Descripcion: Libreria de funciones para
* el manejo de una base de datos de preguntas
* almacenadas en un archivo de texto plano.
* 
* Ultima modificacion: 18/04/2015
*
*/

// Directivas de preprocesador:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicion de tipos:

typedef struct pregunta { 

	int codigo; 
	int nivel;
	char area;
	char *pregunta;
	char *opcion1;
	char *opcion2;
	char *opcion3;
	int respuesta;
	struct pregunta *siguiente;

} PREGUNTA; 


// Definicion de funciones: 

void CambiarSaltoDeLinea(char *frase) {

	/* Descripcion de la funcion:
		Esta funcion dada una frase verifica si esta 
	contiene un salto de linea, de ser asi, este salto 
	de linea es remplzado por un espacio en blanco
	*/

	// Declaracion de variables:
	char *ptr_Caracter;

	// Se verifica si la frase contiene un espacio en blanco
	ptr_Caracter = strchr(frase,10);

	if (ptr_Caracter != NULL){
		//Si hay un caracter nulo se agrega un espacio en blanco
       	*ptr_Caracter=32;
    }
}

//------------------------------------------------------------//


PREGUNTA* LeerBaseDeDatos(char *nombre_archivo) {
	/*  Descripcion de la funcion:
		Esta funcion dado el nombre de un archivo lee todo su 
	contenido.Si el archivo no esta vacio se crea una estructura 
	de datos PREGUNTA que contiene los siguientes (componentes):
	 		-codigo:   Entero que representa el codigo de una 
	 				 pregunta
	 		-nivel:    Entero que representa el nivel de dificultad 
	 				de una pregunta
	 		-area: 	   Char que representa el area de conocimiento
	 				a la que pertenece una pregunta
	 		-pregunta: String que representa la pregunta
	 		-opcion1:  String que representa una posible respuesta 1
	 		-opcion2:  String que representa una posible respuesta 2
	 		-opcion3:  String que representa una posible respuesta 3
	 		-respuesta: Entero que representa cual es el numero
	 					de la opcion con la respuesta correcta.
	 		-siguiente: Apuntador al siguiente elemento de tipo 
	 					PREGUNTA
	 Si el archivo esta vacio la funcion solo creara la cabecera de 
	 la estructuctura descrita anteriormente. 
	*/

	if ( (fopen(nombre_archivo,"r")) == NULL) {
		printf("\nError: El archivo indicado no fue encontrado\n");
		printf("El programa finalizara su ejecucion.\n");
		exit(1);
	}

	else {

		FILE *archivo;
		archivo = fopen(nombre_archivo,"r");
		int codigo,complejidad;
		char tipo;
		int resp_correcta;
		PREGUNTA *primeraPregunta;
		PREGUNTA *temporal;
		PREGUNTA *anterior;

		//Se crea la cabecera
		primeraPregunta=(PREGUNTA*)malloc(sizeof(PREGUNTA));
		primeraPregunta->siguiente=NULL;
		anterior=primeraPregunta;

		// Se verifica si el archivo esta vacio
		fseek(archivo,0,SEEK_END);

   		if (ftell(archivo) == 0 )	{
   			//printf("\n--------------------\n");
   			printf("  \n* El archivo esta vacio\n");
   			//printf("\n--------------------\n\n");
    	}

    	else {

    		fseek(archivo,0,SEEK_SET);

			while (1)  {

				// Se reserva el espacio de memoria para cada
				// pregunta y sus posibles respuestas:

				char *pregunta1;
				pregunta1 = (char*)malloc(sizeof(char)*101);
				char *resp_1;
				resp_1 = (char*)malloc(sizeof(char)*101);

				char *resp_2;	
				resp_2 = (char*)malloc(sizeof(char)*101);

				char *resp_3;
				resp_3 = (char*)malloc(sizeof(char)*101);

				char *pch;										  
				fscanf(archivo," %d %d %c \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" \"%[^\"]\" %d ", \
				&codigo,&complejidad,&tipo,pregunta1,resp_1,resp_2,resp_3,&resp_correcta);

				//Se verifica si hay un salto de linea para ser cambiado 
				// por un espacio en blanco
				CambiarSaltoDeLinea(pregunta1);
				CambiarSaltoDeLinea(resp_1);
				CambiarSaltoDeLinea(resp_2);
				CambiarSaltoDeLinea(resp_3);


				// Hay que verificar si el codigo de la pregunta ya existe
				// tambien hay que verificar el tema
				temporal=(PREGUNTA*)malloc(sizeof(PREGUNTA));
				temporal->codigo=codigo;
				temporal->nivel=complejidad;
				temporal->area=tipo;
				temporal->pregunta=pregunta1;
				temporal->opcion1=resp_1;
				temporal->opcion2=resp_2;
				temporal->opcion3=resp_3;
				temporal->respuesta=resp_correcta;
				temporal->siguiente=NULL;
				anterior->siguiente=temporal;
				anterior=temporal;

				// Verifica si se ha llegado al fin del archivo
				if (feof(archivo)) {
						break;
					}
			}

		}

		printf("\n--------------------\n");
		printf("\nLos datos han sido cargados correctamente a memoria.\n");
		printf("\n--------------------\n\n");
		fclose(archivo);

		return primeraPregunta;

	}
}

//------------------------------------------------------------//

void EliminarLista(PREGUNTA *CabeceraData){

	/*  Descripcion de la funcion:
		Esta funcion destruye una lista enlazada de una 
	estructura de datos de tipo PREGUNTA.

	*/

	// Caso en donde la estructura no tiene elementos
	if(CabeceraData->siguiente==NULL){
		free(CabeceraData);
	}

	// Caso en donde la estructura tiene al menos un elemento
	else{

		PREGUNTA *aux;
		while(CabeceraData->siguiente!=NULL){

			aux=CabeceraData->siguiente;
			CabeceraData->siguiente=aux->siguiente;
			free(aux);
		}

		// Se elimina la cabecera de la estructura de datos
		free(CabeceraData);
	}
}

//------------------------------------------------------------//


void imprimirPreguntas(PREGUNTA *CabeceraArchivo) {

	/*  Descripcion de la funcion:
		Esta funcion imprime todas la preguntas presentes en 
	una estructura de datos llamada PREGUNTA.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;
				
	aux = CabeceraArchivo->siguiente;

	printf("\nLas preguntas que se encuentran la base de datos son:\n\n");
	while (aux != NULL) {

		printf("Pregunta: %s? \n",aux->pregunta);
		printf("Codigo: %d / Complejidad: %d / Area: %c \n",aux->codigo,aux->nivel,aux->area);
		printf("Las opciones son: \n");
		printf("	1.- %s. \n",aux->opcion1);
		printf("	2.- %s. \n",aux->opcion2);
		printf("	3.- %s. \n",aux->opcion3);
		printf("Respuesta correcta: %d \n",aux->respuesta);
		printf("\n--------------------\n\n");
		aux = aux->siguiente;

	}

}

//------------------------------------------------------------//

void imprimirPreguntasComplejidad(PREGUNTA *CabeceraArchivo,char complejidad) {
	/*  Descripcion de la funcion:
		Esta funcion imprime las preguntas almacenadas en una 
	estructura de datos llamada PREGUNTAS que poseen un cierto nivel
	de complejidad.
	*/

	// Declaracion de variables:
	int num_complejidad = (int)(complejidad-48); 
	PREGUNTA *aux;				
	aux = CabeceraArchivo->siguiente;

	printf("\nLas preguntas que se encuentran la base de datos son:\n\n");
	while (aux != NULL) {

		if (aux->nivel==num_complejidad) {

			printf("Pregunta: %s? \n",aux->pregunta);
			printf("Codigo: %d / Complejidad: %d / Area: %c \n",aux->codigo,aux->nivel,aux->area);
			printf("Las opciones son: \n");
			printf("	1.- %s. \n",aux->opcion1);
			printf("	2.- %s. \n",aux->opcion2);
			printf("	3.- %s. \n",aux->opcion3);
			printf("Respuesta correcta: %d \n",aux->respuesta);
			printf("\n--------------------\n\n");

		}

		aux = aux->siguiente;

	}

}

//------------------------------------------------------------//

void Eliminar(PREGUNTA *Cabecera,int clave) {

	/*  Descripcion de la funcion:
		Esta funcion dada una clave, elimina una pregunta 
	almacenada en una estructura de datos llamada PREGUNTAS 
	que posea dicha clave.

	*/

	// Declaracion de variables:
	PREGUNTA *aux;
	PREGUNTA *anterior;	
	anterior=Cabecera;			
	aux = Cabecera->siguiente;

	while (aux != NULL) {

		if(aux->codigo == clave){
			anterior->siguiente=aux->siguiente;
			aux->siguiente=NULL;
			free(aux);
			printf("\n--------------------\n");
			printf("\nLa pregunta %d",clave);
			printf(" ha sido eliminada\n");
			printf("\n--------------------\n\n"); 
			return;

		}
		anterior = aux;
		aux = aux->siguiente;

	}
	printf("\n--------------------\n");
	printf("\nError: La clave introducida no corresponde a ");
	printf("ninguna \npregunta de la base de datos.\n");
	printf("\n--------------------\n\n");
}

//------------------------------------------------------------//

void insertarPregunta(PREGUNTA *CabeceraArchivo) {
	/*  Descripcion de la funcion:

		Esta funcion inserta una nueva pregunta en una lista
	enlazada de una estructura de datos llamada PREGUNTA.
	*/

	// Declaracion de variables:
		PREGUNTA *aux;				
		aux = CabeceraArchivo;


		// buscar guardar una variable que 
		// tenga el apuntador al ultimo

		// ver si se puede gastar menos memoria
		// asignando de una vez en el scanf

		while (aux->siguiente != NULL) {
			aux = aux->siguiente;
		}

		PREGUNTA *nueva_pregunta;
		nueva_pregunta = (PREGUNTA*)malloc(sizeof(PREGUNTA));
		nueva_pregunta->siguiente=NULL;
		aux->siguiente = nueva_pregunta;

		// Declaracion de variables:

		int codigo_nuevo; 
		int nivel_nuevo;
		char area_nueva;

		char *pregunta_nueva;
		pregunta_nueva = (char*)malloc(sizeof(char)*101);

		char *opcion1_nueva;
		opcion1_nueva = (char*)malloc(sizeof(char)*101);

		char *opcion2_nueva;
		opcion2_nueva = (char*)malloc(sizeof(char)*101);

		char *opcion3_nueva;
		opcion3_nueva = (char*)malloc(sizeof(char)*101);

		int respuesta_nueva;

		//Se pide el codigo de la pregunta al usuario

		printf("\nIntroduzca el codigo correspondiente a su pregunta\n");
		printf("(numero entero): ");
		scanf(" %d",&codigo_nuevo);

		// Se pide el nivel de complejidad al usuarioi

		while(1) {


			printf("\nIntroduzca el nivel de complejidad de su pregunta\n");
			printf("(0 Basico, 1 Intermedio, 2 Avanzado): ");

			scanf(" %d",&nivel_nuevo);
			printf("\n");	

			if ( nivel_nuevo >= 0 && nivel_nuevo <= 2) {
				break;
				}

			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");

		}

		// Se pide el tema correspondiente de la pregunta 
		// ingresada por el usuario
		while(1) {


			printf("\nIntroduzca el caracter correspondiente al tema de su pregunta\n");
			printf("(H - Historia, C - Cine, L - Literatura, G - Geografia\n");
			printf("M - Matematicas, T - Tecnologia e Informatica): ");
			scanf(" %c",&area_nueva);
			printf("\n");	

			if ( area_nueva == 72 || area_nueva == 84 || area_nueva == 67 || \
				area_nueva == 76 || area_nueva == 71 || area_nueva == 77) {
					break;
			}

			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");

		}

		// Se pide la pregunta 
		printf("\nIntroduzca su pregunta sin utilizar comillas y sin signo de interrogacion\n");
		printf("(Ejemplo: Cuál es la capital de Alemania): ");
		scanf(" %100[^\n]",pregunta_nueva);

		// Se pide la opcion 1
		printf("\nIntroduzca su primera opcion de respuesta (Ejemplo: Hamburg): ");
		scanf(" %100[^\n]",opcion1_nueva);

		// Se pide la opcion 2
		printf("\nIntroduzca su segunda opcion de respuesta (Ejemplo: Stuttgart): ");
		scanf(" %100[^\n]",opcion2_nueva);

		// Se pide la opcion 3
		printf("\nIntroduzca su tercera posible respuesta (Ejemplo: Berlin): ");
		scanf(" %100[^\n]",opcion3_nueva);


		// Se solicita el numero de la opcion que sera marcada como
		// respuesta correcta
		while(1) {


			printf("\nIntroduzca el numero correspondiente a la respuesta correcta\n");
			printf("(1 - primera opcion, 2 - segunda opcion, 3 - tercera opcion): ");

			scanf(" %d",&respuesta_nueva);
			printf("\n");	

			if ( respuesta_nueva >= 1 && respuesta_nueva <= 3) {
				break;
				}

			printf("\nError: La respuesta no es valida.\n");
			printf("Intente de nuevo.\n");

		}

		// Se almacena en la estructura de datos toda la 
		// informacion suministrada por el usuario
		nueva_pregunta->codigo=codigo_nuevo;
		nueva_pregunta->nivel=nivel_nuevo;
		nueva_pregunta->area=area_nueva;
		nueva_pregunta->pregunta=pregunta_nueva;
		nueva_pregunta->opcion1=opcion1_nueva;
		nueva_pregunta->opcion2=opcion2_nueva;
		nueva_pregunta->opcion3=opcion3_nueva;
		nueva_pregunta->respuesta=respuesta_nueva;
		
		printf("\n--------------------\n");
		printf("\nSu pregunta ha sido agregada a la base de datos.\n");
		printf("\n--------------------\n\n");

	}

//------------------------------------------------------------//


void escribirArchivo(PREGUNTA *CabeceraArchivo,char *nombre_archivo) {
	/*  Descripcion de la funcion:
		Dada una estructura de datos de tipo PREGUNTA 
	y el nombre de un archivo esta funcion creara un archivo
	con el nombre dado y escribira en el archivo toda la 
	informacion almacenada en la estructura de datos suministrada.
	*/

	// Declaracion de variables:
	PREGUNTA *aux;				
	aux = CabeceraArchivo->siguiente;

	FILE *archivoSalida;
	archivoSalida = fopen(nombre_archivo,"w");

		while (aux != NULL) {

			fprintf(archivoSalida,"%d %d %c \"%s\" \"%s\" \"%s\" \"%s\" %d \n", \
				aux->codigo,aux->nivel,aux->area,aux->pregunta, \
				aux->opcion1,aux->opcion2,aux->opcion3,aux->respuesta);

			aux = aux->siguiente;

		}

	fclose(archivoSalida);

	}


//------------------------------------------------------------//