#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#include <stdint.h>


#define LIMITE 50
#define MAXCHAR 1000
int GANADOR = 0;
int ganador_auxiliar = 1;
char * equipo_ganador;


static pthread_mutex_t mutex_salar = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_plancha = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_horno = PTHREAD_MUTEX_INITIALIZER;


//creo estructura de semaforos 
struct semaforos {

	sem_t sem_cortar;
	sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_armar_medallones;
    sem_t sem_plancha;
	sem_t sem_cortar_extras;
	sem_t sem_horno;
	sem_t sem_plancha_aux;

};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {

	FILE *fp1;
	fp1 = fopen("resultado.txt", "a");

	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		
		fprintf(fp1,"\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		
		fprintf(fp1,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							
							fprintf(fp1,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}

	fclose(fp1);

}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;

	sem_wait(&mydata->semaforos_param.sem_cortar);

	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 1000000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}

//funcion mezclar
void* mezclar(void *data) {

        //creo el nombre de la accion de la funcion 
        char *accion = "mezclar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		sem_wait(&mydata->semaforos_param.sem_mezclar);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
    	sem_post(&mydata->semaforos_param.sem_salar);

    pthread_exit(NULL);
}

//funcion salar
void* salar(void *data) {
		pthread_mutex_lock(&mutex_salar);
        //creo el nombre de la accion de la funcion 
        char *accion = "salar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		sem_wait(&mydata->semaforos_param.sem_salar);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
    	sem_post(&mydata->semaforos_param.sem_armar_medallones);
		pthread_mutex_unlock(&mutex_salar);
		
    	pthread_exit(NULL);
}

//funcion armar medallones
void* armar_medallones(void *data) {

        //creo el nombre de la accion de la funcion 
        char *accion = "armar_medallones";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		sem_wait(&mydata->semaforos_param.sem_armar_medallones);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
    	sem_post(&mydata->semaforos_param.sem_plancha);

    pthread_exit(NULL);
}

//funcion plancha
void* plancha(void *data) {
		
        //creo el nombre de la accion de la funcion 
        char *accion = "plancha";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		
        pthread_mutex_lock(&mutex_plancha);
		sem_wait(&mydata->semaforos_param.sem_plancha);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
        
		
        sem_post(&mydata->semaforos_param.sem_plancha_aux);
		pthread_mutex_unlock(&mutex_plancha);

   		pthread_exit(NULL);
}

//funcion cortar extras
void* cortar_extras(void *data) {

        //creo el nombre de la accion de la funcion 
        char *accion = "cortar_extras";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );

		sem_post(&mydata->semaforos_param.sem_cortar_extras);

    pthread_exit(NULL);
}

//funcion horno
void* horno(void *data) {
		pthread_mutex_lock(&mutex_horno);
        //creo el nombre de la accion de la funcion 
        char *accion = "horno";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		//sem_wait(&mydata->semaforos_param.sem_horno);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );

		sem_post(&mydata->semaforos_param.sem_horno);

		pthread_mutex_unlock(&mutex_horno);
    	pthread_exit(NULL);
}

//funcion armar hamburguesa
void* armar_hamburguesa(void *data) {

        //creo el nombre de la accion de la funcion 
        char *accion = "armar_hamburguesa";
        
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
        struct parametro *mydata = data;

		sem_wait(&mydata->semaforos_param.sem_horno);
		sem_wait(&mydata->semaforos_param.sem_cortar_extras);
		sem_wait(&mydata->semaforos_param.sem_plancha_aux);

        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        
        //uso sleep para simular que que pasa tiempo
        usleep( 1000000 );

        sem_post(&mydata->semaforos_param.sem_horno);
		//sem_post(&mydata->semaforos_param.sem_cortar_extras);
		//sem_post(&mydata->semaforos_param.sem_plancha);

         if(GANADOR==0){
            //printf("\tEquipo %d \n " , mydata->equipo_param);
            GANADOR = mydata->equipo_param;
        }
       
    pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {

	//variables semaforos
	sem_t sem_cortar;
	sem_t sem_mezclar;
    sem_t sem_salar;
	sem_t sem_armar_medallones;
	sem_t sem_plancha;
	sem_t sem_cortar_extras;
	sem_t sem_horno;
	sem_t sem_plancha_aux;

	//crear variables semaforos aqui

	//variables hilos
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;


	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);

	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct

	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_cortar = sem_cortar;
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_armar_medallones = sem_armar_medallones;
	pthread_data->semaforos_param.sem_plancha = sem_plancha;
	pthread_data->semaforos_param.sem_cortar_extras = sem_cortar_extras;
	pthread_data->semaforos_param.sem_horno = sem_horno;
	pthread_data->semaforos_param.sem_plancha_aux = sem_plancha_aux;

	//---------------seteo las acciones y los ingredientes 	------------------------------------

	FILE *fp;
	char* filename = "receta.txt";

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(filename, "r");
	if (fp == NULL)
    	exit(EXIT_FAILURE);

	int contadorLineas=0;

	while ((read = getline(&line, &len, fp)) != -1) {

	int init_size = strlen(line);
	int contador = 0;
	char *ptr = strtok(line,"|");

	while (ptr != NULL)
	{

		if(contador ==0){
			//printf("Numero de linea %d", contadorLineas);
			//printf("Esta es la accion '%s'\n", ptr);
			strcpy(pthread_data->pasos_param[contadorLineas].accion, ptr);
			ptr = strtok(NULL, "|");
		}else{
			//printf("Numero de linea %d", contadorLineas);
			//printf("Este es el ingrediente '%s'\n", ptr);
			strcpy(pthread_data->pasos_param[contadorLineas].ingredientes[contador], ptr);
			ptr = strtok(NULL, "|");
		}

		contador++;
	}
		
		contadorLineas++;

    }

    fclose(fp);


	//--------------------------inicializo los semaforos-------------------------------------
	sem_init(&(pthread_data->semaforos_param.sem_cortar),0,1);
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armar_medallones),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_plancha),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cortar_extras),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_horno),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_plancha_aux),0,0);


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,NULL,cortar,pthread_data);

	int rc2;
   	rc2 = pthread_create(&p2,NULL,mezclar,pthread_data);
   	
	int rc3;
   	rc3 = pthread_create(&p3,NULL,salar,pthread_data);
  	
	int rc4;
    rc4 = pthread_create(&p4,NULL,armar_medallones,pthread_data);

	int rc5;
    rc5 = pthread_create(&p5,NULL,plancha,pthread_data);

	int rc6;
    rc6 = pthread_create(&p6,NULL,cortar_extras,pthread_data);

	int rc7;
    rc7 = pthread_create(&p7,NULL,horno,pthread_data);

	int rc8;
    rc8 = pthread_create(&p8,NULL,armar_hamburguesa,pthread_data);

	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);

	//valido que el hilo se haya creado bien
    if (rc){
    	printf("Error:unable to create thread, %d \n", rc);
       	exit(-1);
    }

   	if (rc2){
       		printf("Error:unable to create thread, %d \n", rc);
       		exit(-1);
     	}

  	if (rc3){
       		printf("Error:unable to create thread, %d \n", rc);
       		exit(-1);
     	}

    if (rc4){
            printf("Error:unable to create thread, %d \n", rc);
            exit(-1);
    }

    if (rc5){
            printf("Error:unable to create thread, %d \n", rc);
            exit(-1);
    }

    if (rc6){
            printf("Error:unable to create thread, %d \n", rc);
            exit(-1);
    }

	if (rc7){
            printf("Error:unable to create thread, %d \n", rc);
            exit(-1);
    }

    if (rc8){
            printf("Error:unable to create thread, %d \n", rc);
            exit(-1);
    }


	//destruccion de los semaforos
	sem_destroy(&sem_cortar);
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_armar_medallones);
	sem_destroy(&sem_plancha);
	sem_destroy(&sem_cortar_extras);
	sem_destroy(&sem_horno);
	sem_destroy(&sem_plancha_aux);

	//salida del hilo
	 pthread_exit(NULL);
}

int main (){

	//creo los nombres de los equipos
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;

	//creo las variables los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;

	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1);

    rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);

    rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);

	pthread_mutex_destroy(&mutex_salar);
	pthread_mutex_destroy(&mutex_plancha);
	pthread_mutex_destroy(&mutex_horno);

    
            printf("//////////////////////////////////////////////////////\n");
            printf("//                                                  //\n");
            printf("//\t\tEquipo ganador es: %d                //\n" , GANADOR);
            printf("//                                                  //\n");
            printf("//////////////////////////////////////////////////////\n");

    pthread_exit(NULL);
}
