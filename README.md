# **:hamburger:Subway Argento:hamburger:**
## Semáforos y Threads

# Intro 📋

------------

_En la cocina de Subway Argento compiten cuatro equipos de cocineros
para ganarse el puesto de trabajo en el local. ¿Quién será el primero en
entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del
local?_

_Necesitamos demostrar la concurrencia y organización para poder simular una competencia real, por eso es de suma importancia que utilicemos los semáforos correctos y la cantidad de threads necesarios para cada equipo y acción dentro del programa._


### Pseudocódigo 📦

------------

_Veamos como se comporta inicialmente con un pseudocódigo básico:_

- Los mutex para el salero y para freir cumplen la función de comprobar si algún equipo está utilizando alguno de los elementos de cocina y de ser así, negarle su uso a los otros equipos. 

- También definimos un semáforo para el horno y poder setearle la cantidad de panes que pueden cocinarse en este.

- Hilos definidos en el main: equipo1 , equipo2, equipo3, equipo 4. 

- Cada hilo apunta a la función *ejecutarReceta()*.

  - *ejecutarReceta() :*  crea y setea los hilos y semáforos necesarios para la ejecución. Cada hilo creado aqui representa los pasos de dicha receta y apuntan a sus funciones correspondientes. También dentro de esta función, leemos la receta.

  - *cortarCondimentosIniciales() , mezclar() , salar() , empanar(), freir(), hornear(), cortarfinal(), armar()* :  estas funciones representan los pasos de la receta, en donde dentro de cada una se hace una llamada a imprimirAccion() que imprime por pantalla y también la escribe en un archivo. En cada función mencionada, con ayuda de los semáforos, se logra sincronizar los pasos para ejecutarlos en el orden que plantea la receta, y con ayuda de los mutex, solo un equipo a la vez podrá acceder a los elementos de la cocina (salero, horno o plancha para freir). 
  En *armarHamburguesa()* indicamos por pantalla y escribimos en el archivo el ganador de la competencia.
  
------------
  
```c

semáforos y mutex;
acciones, ingredientes y equipos;

main {
  creamos e inicializamos hilos de equipos;
  semáforo_hornear seteado en 2;
}

ejecutarReceta {
  creamos e inicializamos hilos de las acciones;
  inicializamos semáforos de las acciones;
  leemos la receta desde un txt;
}

imprimirAccion {
  verifica e imprime los pasos e ingredientes de la receta;
  exportamos el resultado del programa a un txt;
}

cortarCondimentosIniciales {
  //No necesita esperar la señal de nadie ya que es el primer paso de la receta
  cortarCondimentosIniciales;
  v(sem_mezclar);
}

mezclar {
  p(sem_mezclar);
  mezclar;
  v(sem_salar);
}

salar {
  lock(mutex_salar);
  p(sem_salar);
  salar;
  v(sem_empanar);
  unlock(mutex_salar);
}

empanar {
  p(sem_empanar);
  empanar;
  v(sem_freir);
}

freir {
  lock(mutex_freir);
  p(sem_freir);
  freir;
  v(sem_cortarFinal);
  unlock(mutex_freir);
}

hornear {
  //Puede empezar desde el comienzo de la competencia y es compartido
  p(sem_hornear);
  hornear;
  v(sem_hornear);
  v(sem_armar2);
}

cortarFinal {
  p(sem_cortarFinal);
  cortarFinal;
  v(sem_armar);
}

armar {
  p(sem_armar);
  p(sem_armar2);
  armar;
  print(Ganador);
  exit;
}
```

------------

![PSEUDO](https://user-images.githubusercontent.com/22090492/115168134-43e8d800-a090-11eb-9221-f3ee32743eba.png)



## Prueba de escritorio ⌨️

------------

![prueba](https://user-images.githubusercontent.com/22090492/115170533-43077480-a097-11eb-9377-1ec8e7709821.png)
