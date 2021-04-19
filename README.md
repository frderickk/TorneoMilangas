# **:hamburger:Subway Argento:hamburger:**
## Trabajo práctico de semáforos, Sistemas Operativos y Redes




# Introduccion

_En la cocina de Subway Argento compiten cuatro equipos de cocineros
para ganarse el puesto de trabajo en el local. ¿Quién será el primero en
entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del
local?_

_Necesitamos demostrar la concurrencia y organización para poder simular una competencia real, por eso es de suma importancia que utilicemos los semáforos correctos y la cantidad de threads necesarios para cada equipo y acción dentro del programa._


### Pseudocódigo 📋

_Veamos como se comporta inicialmente con un pseudocódigo básico:_
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
