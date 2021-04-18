# **:hamburger:Subway Argento:hamburger:**
## Trabajo práctico de semáforos, Sistemas Operativos y Redes




# Introduccion

En la cocina de Subway Argento compiten cuatro equipos de cocineros
para ganarse el puesto de trabajo en el local. ¿Quién será el primero en
entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del
local?

Necesitamos demostrar la concurrencia y organización para poder simular una competencia real, por eso es de suma importancia que utilicemos los semáforos correctos y la cantidad de threads necesarios para cada equipo y acción dentro del programa.


### _Pseudocódigo_ 📋

Veamos como se comporta inicialmente con un pseudocódigo básico:
```
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
_No necesita esperar la señal de nadie ya que es el primer paso de la receta_
  cortar;
_Doy la señal a la siguiente accion (cortarcondimentosiniciales me habilita mezclar)_
  v(sem_mezclar);
}
```
