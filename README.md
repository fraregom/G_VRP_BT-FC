# Implementación de G-VRP (Primera iteración)

Para ejecutar el programa simplemente se debe hacer lo siguiente (tras compilar mediante makefile):

.\G_VRP-FC <Carpeta_instancias>\<Nombre_instancia>

Por ejemplo:

./G_VRP-FC instances/testing.txt

Al ejecutarlo el algoritmo entregará lo siguiente:
1. Coste del tour más corto
2. La ruta tomada
3. Solución encontrada en x iteración recursiva
4. El número de llamadas recursivas realizadas
5. El tiempo de ejecución

Nota:

1. Lamentablemente aún no se implementa el código para m autos por lo que no será posible probarlo
para instancias muy grandes (el algoritmo tarda más al incrementarse el espacio de búsqueda).

2. Se adjunta una carpeta con instancias de prueba.
