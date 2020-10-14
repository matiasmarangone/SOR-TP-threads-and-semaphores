# :traffic_light: Trabajo Práctico de hilos y semáforos :traffic_light:	

_Este repositorio es utilizado para entregar un trabajo práctico de Sistemas Operativos y Redes._
Alumno: Matias Marangone

## Tabla de contenidos
* [Información](#info)  
* [Prueba de escritorio](#prueba)
* [Implementación](#implementacion)  
* [Ejecución](#ejecucion)
* [Conclusión](#conclusion)  
  

<a name="info"/>

### Información :information_source:
En la cocina de Hell’s Burgers compiten tres equipos de cocineros. ¿Quién será el primero en entregar 2 hamburguesas al jurado?
La dificultad de la competencia es que la cocina no cuenta con el equipamiento individual para cada equipo, sino que algunos de ellos son compartidos. Se tienen que ordenar para
cocinar los medallones ya que solo tienen una plancha para todos los equipos y esta cocina un medallón a la vez, existe solo un horno que cocina dos panes a la vez y un solo salero.

<a name="prueba"/>

### Prueba de escritorio
A continuación se muestra la prueba de escritorio final con la que me base para realizar la implementación:

![GitHub Logo](/images/semaphorestp.png)
Format: ![Alt Text](/images/semaphorestp.png)

Y el estado de los semaforos para la prueba de escritorio:
![GitHub Logo](/images/semaphorestp2.png)
Format: ![Alt Text](/images/semaphorestp2.png)

<a name="implementacion"/>

### Implementación :computer:	
_La entrega es un archivo con extensión ".C" que contiene la implementación y resolución del problema presentado en la consigna_

<a name="ejecucion"/>

### Ejecución :man_technologist:	
_El archivo debe ser compilado y ejecutado por consola, preferentemente en un ambiente Linux_
```
Compilar:   gcc HellsBurgers.c -o ejecutable -lpthread
Ejecutar:   ./ejecutable
```
<a name="conclusion"/>

### Conclusión
Entre los conceptos aprendidos puedo destacar que usamos los mutex ya que son muy útiles para proteger data (sección crítica) de las condiciones de carrera, las cuales hay que evitar para lograr una correcta ejecución de los programas.
Los semaforos se utilizan usualmente para sincronizar hilos más que proteger a la sección critica.
La gran diferencia que pude apreciar es que los mutex "tienen dueño" en cambio los semaforos no. Con esto me refiero a que por ejemplo, una vez que se bloqueo un mutex, solo ese thread puede liberarlo y se asegura que nadie mas lo intente utilizar. Los semaforos por su parte pueden ser "prendidos" o "apagados" (post y wait respectivamente) por cualquier thread y en cualquier momento.
