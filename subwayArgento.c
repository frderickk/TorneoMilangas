#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>

#define LIMITE 50

pthread_mutex_t mutex_salar;
pthread_mutex_t mutex_freir;
pthread_mutex_t mutex_hornear;


//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_empanar;
    sem_t sem_freir;
    sem_t sem_cortarfinal;
    sem_t sem_armar;
    sem_t sem_hornear;
    sem_t sem_equipoganador;
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
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}



void* cortarcondimentosiniciales(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Cortando Ingredientes";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(4);
	//doy la señal a la siguiente accion (cortarcondimentosiniciales me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_mezclar);
	pthread_exit(NULL);
}

void* mezclar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Mezclando Ingredientes";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(5);
	//doy la señal a la siguiente accion (mezclar me habilita salar)
	sem_post(&mydata->semaforos_param.sem_salar);
	pthread_exit(NULL);
}

void* salar(void *data) {
	//bloqueo el mutex del salero asi solo lo utiliza un equipo en ese instante
	pthread_mutex_lock(&mutex_salar);
	//creo el nombre de la accion de la funcion 
	char *accion = "Salando a gusto";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_salar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(2);
	//doy la señal a la siguiente accion (salar me habilita empanar)
	sem_post(&mydata->semaforos_param.sem_empanar);
	//desbloqueo el salero
	pthread_mutex_unlock(&mutex_salar);
	pthread_exit(NULL);
}

void* empanar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Empanando Milanga";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_empanar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(6);
	//doy la señal a la siguiente accion (empanar me habilita freir)
	sem_post(&mydata->semaforos_param.sem_freir);
	pthread_exit(NULL);
}

void* freir(void *data) {
	//bloqueo el sarten para que lo use un solo equipo
	pthread_mutex_lock(&mutex_freir);
	//creo el nombre de la accion de la funcion 
	char *accion = "Friendo Milanga";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_freir);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(12);
	//doy la señal a la siguiente accion (cortarcondimentosiniciales me habilita mezclar)
    	sem_post(&mydata->semaforos_param.sem_cortarfinal);
    	//desbloqueo el sarten
	pthread_mutex_unlock(&mutex_freir);
	pthread_exit(NULL);
}

void* hornear(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Horneando Pan";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	pthread_mutex_lock(&mutex_hornear);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(25);
	sem_post(&mydata->semaforos_param.sem_hornear);
	sem_post(&mydata->semaforos_param.sem_hornear);
	pthread_mutex_unlock(&mutex_hornear);
	pthread_exit(NULL);
}

void* cortarfinal(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Cortando condimentos";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cortarfinal);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(6);
	//doy la señal a la siguiente accion (cortarcondimentosiniciales me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_armar);
	pthread_exit(NULL);
}

void* armar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "Sanguche de Milanga Completado!";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_armar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	sleep(2);
	sem_post(&mydata->semaforos_param.sem_equipoganador);
	
	
    pthread_exit(NULL);
}

void* equipoganador(void *data){
       struct parametro *mydata = data; 
       sem_wait(&mydata->semaforos_param.sem_equipoganador); 
       printf("El ganador es... Equipo %d !\n",mydata->equipo_param); 
       exit(-1);
       pthread_exit(NULL); 

}

void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
    	sem_t sem_salar;
    	pthread_mutex_t mutex_salar;
    	sem_t sem_empanar;
    	sem_t sem_freir;
    	pthread_mutex_t mutex_freir;
    	sem_t sem_cortarfinal;
    	sem_t sem_armar;
    	sem_t sem_hornear;
    	pthread_mutex_t mutex_hornear;
    	sem_t sem_equipoganador;
	
	//variables hilos
	pthread_t p1; 
	pthread_t p2;
	pthread_t p3;
	pthread_t p4; 
	pthread_t p5; 
	pthread_t p6; 
	pthread_t p7; 
	pthread_t p8;
	pthread_t p9;
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	usleep(5000);
	printf("Ejecutando equipo %d \n", p);
	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));
	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;
	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	mutex_salar = mutex_salar;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_freir = sem_freir;
	mutex_freir = mutex_freir;
	pthread_data->semaforos_param.sem_cortarfinal = sem_cortarfinal;
	pthread_data->semaforos_param.sem_armar = sem_armar;
	pthread_data->semaforos_param.sem_hornear = sem_hornear;
	mutex_hornear = mutex_hornear;
	pthread_data->semaforos_param.sem_equipoganador = sem_equipoganador;
	
	
	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
	strcpy(pthread_data->pasos_param[0].accion, "Cortando Ingredientes");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "Ajo");
	strcpy(pthread_data->pasos_param[0].ingredientes[1], "Perejil");
	strcpy(pthread_data->pasos_param[1].accion, "Mezclando Ingredientes");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "Ajo");
	strcpy(pthread_data->pasos_param[1].ingredientes[1], "Perejil");
	strcpy(pthread_data->pasos_param[1].ingredientes[2], "Huevo");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "Carne");
	strcpy(pthread_data->pasos_param[2].accion, "Salando a gusto");
	strcpy(pthread_data->pasos_param[3].accion, "Empanando Milanga");
	strcpy(pthread_data->pasos_param[4].accion, "Friendo Milanga");
	strcpy(pthread_data->pasos_param[5].accion, "Cortando condimentos");
	strcpy(pthread_data->pasos_param[6].accion, "Sanguche de Milanga Completado!");
	strcpy(pthread_data->pasos_param[7].accion, "Horneando Pan");
	
	
	//inicializo los semaforos
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar), 0, 0);
	pthread_mutex_init(&(mutex_salar),NULL);
	sem_init(&(pthread_data->semaforos_param.sem_empanar), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_freir), 0, 0);
	pthread_mutex_init(&(mutex_freir),NULL);
	sem_init(&(pthread_data->semaforos_param.sem_cortarfinal), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_armar), 0, 0);
	sem_init(&(pthread_data->semaforos_param.sem_hornear), 0, 0);
	pthread_mutex_init(&(mutex_hornear),NULL);
	sem_init(&(pthread_data->semaforos_param.sem_equipoganador),0,0);
	
	
	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
	int rc;
	rc = pthread_create(&p1,                        //identificador unico
                        NULL,                          //atributos del thread
                        cortarcondimentosiniciales,    //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p2,                       //identificador unico
                        NULL,                          //atributos del thread
                        mezclar,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p3,                       //identificador unico
                        NULL,                          //atributos del thread
                        salar,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p4,                       //identificador unico
                        NULL,                          //atributos del thread
                        empanar,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p5,                       //identificador unico
                        NULL,                          //atributos del thread
                        freir,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
	rc = pthread_create(&p6,                       //identificador unico
                        NULL,                          //atributos del thread
                        cortarfinal,    		 //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
        rc = pthread_create(&p7,                       //identificador unico
                        NULL,                          //atributos del thread
                        armar,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia                
        rc = pthread_create(&p8,                       //identificador unico
                        NULL,                          //atributos del thread
                        hornear,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
        rc = pthread_create(&p9,                       //identificador unico
                        NULL,                          //atributos del thread
                        equipoganador,    		         //funcion a ejecutar
                        pthread_data);                 //parametros de la funcion a ejecutar, pasado por referencia
                        
                                        
	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2, NULL);
	pthread_join (p3, NULL);
	pthread_join (p4, NULL);
	pthread_join (p5, NULL);
	pthread_join (p6, NULL);
	pthread_join (p7, NULL);
	pthread_join (p8, NULL);
	pthread_join (p8, NULL);
	

	//valido que el hilo se haya creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }
   
	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	pthread_mutex_destroy(&mutex_salar);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_freir);
	pthread_mutex_destroy(&mutex_freir);
	sem_destroy(&sem_cortarfinal);
	sem_destroy(&sem_armar);
	sem_destroy(&sem_hornear);
	pthread_mutex_destroy(&mutex_hornear);
	sem_destroy(&sem_equipoganador);
	
	//salida del hilo
	 pthread_exit(NULL);
}

int main ()
{

	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;
	
	
	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;
  
  
	//inicializo los hilos de los equipos
	rc = pthread_create(&equipo1,				//identificador unico
                            NULL,				//atributos del thread
                            ejecutarReceta,             	//funcion a ejecutar
                            equipoNombre1); 
	rc = pthread_create(&equipo2,                         //identificador unico
                            NULL,                          	//atributos del thread
                            ejecutarReceta,             	//funcion a ejecutar
                            equipoNombre2);
        rc = pthread_create(&equipo3,                         //identificador unico
                            NULL,                          	//atributos del thread
                            ejecutarReceta,             	//funcion a ejecutar
                            equipoNombre3);
        rc = pthread_create(&equipo4,                         //identificador unico
                            NULL,                          	//atributos del thread
                            ejecutarReceta,             	//funcion a ejecutar
                            equipoNombre4);
 
	if (rc) {
		printf("Error:unable to create thread, %d \n", rc);
		exit(-1);
	} 
	
	
	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
    pthread_exit(NULL);
}
