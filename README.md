# Tarea1-ProgramacionIII
Compilación:
Para su ejecución es necesario tener los siguientes archivos:
- Tensor.h
- Tensor.cpp
- main.cpp
En este caso recomendamo usar CLion, pero en si debería poder compilarse de forma manual u con otro entorno de programación.
Ejecución:
El archivo main.cpp tiene la simulación de una red neuronal que utiliza la clase Tensor. La clase Tensor almacena sus datos en memoria dinámica mediante un puntero double *, osea se trabaja con un array dinámico contiguo. Cada operación que se realice accede directamente a los elementos mediante índices que se recorre de forma lineal.
Durante la ejecución se llaman a otras funciones como:
-matmul: Combina los datos con los pesos.
- Operator(+): Suma los tensores.
- View: Reorganiza los datos sin hacer una copia.
- Unsqueeze: Ajusta las dimensiones para que se pueda realizer operaciones
- Apply: Aplica transformaciones usando ReLU y Sigmoid.

Además, el programa imprime las dimensiones de los tensores en cada etapa, lo que facilita verificar que las operaciones se realizan correctamente.
