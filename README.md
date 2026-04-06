# Tarea1-ProgramacionIII
El programa se centra en la construcción de la librería **Tensor++**, la cual abre diferentes posibilidades en el manejo de tensores unidimensionales, bidimensionales y tridimensionales. Como muestra de su versatilidad, se desarrolló una **red neuronal** basada en el uso de tensores bidimensionales y tridimensionales. En esta aplicación se trato de implementar las diversas funciones y operaciones que permite realizar la librería en relación al manejo de tensores.

En lo que respecta al diseño interno de la librería, esta cuenta con distintos constructores, asignadores de copia y movimiento, así como sobrecarga de operadores y funciones especializadas que le permiten al usuario ejecutar diferentes operaciones sobre los tensores tales como suma, resta, multiplicación, concatenación, multiplicación matricial, producto punto, entre otras. 


## Ejecución
Para la ejecución del programa es necesario guardar los siguientes archivos en una carpeta:
- cmake-build-debug
- Tensor.h
- Tensor.cpp
- main.cpp

*En este caso recomendamos usar CLion, aunque el programa puede ser ejecutada con normalidad en cualquier otro IDE.*

## Diseño interno
La librería Tensor++ tiene sus bases en la clase Tensor, cuya característica principal es el uso de memoria dinámica para almacenar los valores del tensor. El constructor principal recibe dos listas entre llaves, la primera correspondiente a las dimensiones del tensor y la segunda correspondiente a los valores que tendrá este tensor. A través de estructuras condicionales, el constrructor valida que el tensor tenga como máximo tres dimensiones y que los valores ingresados correspondan a la cantidad de valores esperados por las dimensiones ingresadas. Si la validación es correcta, se utiliza un puntero double para acceder a la memoria dinámica y guardar los valores en un array. Es decir, que la clase se desarrolla en base a un array dinámico contiguo, el cual permite almacenar los valores de manera lineal (1 dimension). Esta opción facilita el manejo de los datos, especialmente al trabajar con tensores de dos y tres dimensiones, ya que únicamente se debe usar los indices (dimensiones) para acceder a los datos y a organizarse de manera lineal, facilita las operaciones con los tensores.

Adicionalmente, el constructor guarda en otra variable un vector con los valores de las dimensiones, así como otra variable con el valor total de las dimensiones para facilitar el posterior manejo de los tensores.

En lo que respecta a la versatilidad de la clase Tensor, esta también cuenta con un constructor de copia, de movimiento y asignadores de copia y movimiento que son importantes.




Durante la ejecución se llaman a otras funciones como:

- matmul: Realiza la multiplicación matricial entre tensores bidimensionales, combina filas del primer tensor con columnas del segundo mediante sumas de productos, permitiendo transformar los datos de entrada a un nuevo espacio.
- Operator(+): Realiza la suma elemento a elemento entre tensores, se utiliza principalmente para añadir los vectores de bias, ajustando los valores resultantes de la multiplicación matricial.
- View: permite cambiar la forma del tensor sin modificar ni copiar los datos. Esto se logra reinterpretando el arreglo interno, lo que permite adaptar las dimensiones del tensor para operaciones posteriores sin uso adicional de memoria.
- Unsqueeze: Agrega una dimensión de tamaño 1 en una posición específica, esto es útil para alinear correctamente las dimensiones de los tensores y permitir operaciones como la suma con bias sin inconsistencias dimensionales.
- Apply: Aplica transformaciones elemento a elemento mediante polimorfismo. Se utiliza para ejecutar funciones de activación como ReLU (que reemplaza valores negativos por cero) y Sigmoid (que comprime los valores en un rango entre 0 y 1).

Además, el programa imprime las dimensiones de los tensores en cada etapa, lo que facilita verificar que las operaciones se realizan correctamente.
