# Tarea1-ProgramacionIII
El programa se centra en la construcción de la librería **Tensor++**, la cual abre diferentes posibilidades en el manejo de tensores unidimensionales, bidimensionales y tridimensionales. Como muestra de su versatilidad, se desarrolló una **red neuronal** basada en el uso de tensores bidimensionales y tridimensionales. En esta aplicación se trato de implementar las diversas funciones y operaciones que permite realizar la librería en relación al manejo de tensores.

En lo que respecta al diseño interno de la librería, esta cuenta con distintos constructores, asignadores de copia y movimiento, así como sobrecarga de operadores y funciones especializadas que le permiten al usuario ejecutar diferentes operaciones sobre los tensores tales como suma, resta, multiplicación, concatenación, multiplicación matricial, producto punto, entre otras. 


## Ejecución
Para la ejecución del programa es necesario descargar los siguientes archivos del repositorio y guardarlos en una carpeta.
- CMakeLists.txt
- Tensor.h
- Tensor.cpp
- main.cpp

*En este caso recomendamos usar CLion, aunque el programa puede ser ejecutada con normalidad en cualquier otro IDE.*

## Diseño interno
La librería Tensor++ tiene sus bases en la clase Tensor, cuya característica principal es el uso de memoria dinámica para almacenar los valores del tensor. El constructor principal recibe dos listas entre llaves, la primera correspondiente a las dimensiones del tensor y la segunda correspondiente a los valores que tendrá este tensor. A través de estructuras condicionales, el constrructor valida que el tensor tenga como máximo tres dimensiones y que los valores ingresados correspondan a la cantidad de valores esperados por las dimensiones ingresadas. Si la validación es correcta, se utiliza un puntero double para acceder a la memoria dinámica y guardar los valores en un array. Es decir, que la clase se desarrolla en base a un array dinámico contiguo, el cual permite almacenar los valores de manera lineal (1 dimension). Esta opción facilita el manejo de los datos, especialmente al trabajar con tensores de dos y tres dimensiones, ya que únicamente se debe usar los indices (dimensiones) para acceder a los datos y a organizarse de manera lineal, facilita las operaciones con los tensores.

A continuación se detallan las principales funcionalidades y componentes de la librería
## Constructor principal

El constructor principal (Tensor(shape, values)) se encarga de crear un tensor validando primero que el número de dimensiones esté entre 1 y 3, después de eso calcula manualmente la cantidad total de elementos multiplicando cada dimensión (cont_val). Esta cantidad se compara con el tamaño del vector values. Una vez validado, se reserva memoria dinámica usando new double[cont_val], lo que implica que los datos se almacenan en un bloque contiguo en memoria[double*]. Por último, se copian los valores uno por uno al arreglo interno usando un bucle for. Finalmente, se guarda el total de elementos en dim_total, lo que permite recorrer el tensor de forma lineal en otras operaciones.

El destructor (~Tensor) libera la memoria dinámica utilizando delete[] data. 

## Creación de tensores
#### Zeros:

Se crea un tensor inicializado en cero calculando primero el número total de elementos mediante el producto de sus dimensiones, luego se construye un vector<double> con ese tamaño, inicializado automáticamente en 0, y finalmente se pasa al constructor principal. 

#### Ones:

Funciona de manera similar a zeros, pero el vector se inicializa con valores iguales a 1. Esto se logra usando el constructor de vector<double>(n,1), lo cual simplifica la inicialización y evita bucles manuales.

#### Random:

Se genera un tensor con valores aleatorios utilizando la librería <random>. Se calcula el tamaño total, luego se usa random_device para obtener una semilla, mt19937 como generador y uniform_real_distribution para obtener valores en el rango [min, max). Los valores se insertan en un vector mediante un bucle, que luego se pasa al constructor del tensor.

#### arange:
Crea un tensor unidimensional generando valores secuenciales desde inicio hasta fin-1. Se utiliza un bucle for simple que inserta los valores en un vector<double>.

## Gestion de memoria y ciclo de vida

#### Constructor de copia:

Primero se copian las dimensiones (dimensiones) y el total de elementos (dim_total) del tensor original. Luego se reserva una nueva región de memoria dinámica usando new double[dim_total], lo cual es fundamental para evitar que dos objetos compartan el mismo puntero. Después, se realiza una copia elemento por elemento mediante un bucle que recorre todo el arreglo lineal, copiando cada valor desde other.data hacia el nuevo data. Esto asegura que el nuevo tensor tenga su propio bloque de memoria independiente.

#### Constructor de movimiento: 

En este caso no se copian los datos, sino que se transfiere directamente el puntero data del tensor original al nuevo objeto. También se copian las dimensiones y el tamaño total. Luego, el tensor original se deja en un estado vacío asignando nullptr a su puntero, 0 a su tamaño total y un vector vacío a sus dimensiones. El tensor original sigue siendo válido, pero queda vacío. Esta operación es mucho más eficiente que la copia porque no recorre el arreglo.

#### Asignador de copia (=) : 

Primero se verifica que no exista auto-asignación comparando (this != &other), lo cual evita errores cuando un objeto se asigna a sí mismo, luego se reserva nueva memoria dinámica para almacenar los datos del otro tensor. Se copian los valores usando la función copy, lo que permite copiar todo el bloque de datos de manera eficiente. Después, se libera la memoria antigua del objeto actual con delete[] data para evitar fugas de memoria. Finalmente, se asigna el nuevo puntero y se actualizan las dimensiones y el tamaño total.

#### Asignador de movimiento (=) : 

Al igual que el asignador de copia, primero verifica que no haya auto-asignación. Luego libera la memoria actual del objeto usando delete[] data. Después, toma directamente el puntero data del otro tensor, junto con sus dimensiones y tamaño total. Finalmente, el tensor original se limpia asignando nullptr a su puntero y 0 a su tamaño total. Esto evita problemas de doble liberación de memoria. Esta operación es más eficiente que la copia porque no se duplican los datos, solo se transfiere la referencia al bloque de memoria.

## Sobrecarga de operadores

#### Operator + 
Primero valida que ambos tensores tengan la misma cantidad de dimensiones. Luego, si ambos son tensores 2D, implementa un caso especial de suma con bias. Si el segundo tensor tiene forma (1, n), se suma por columnas reutilizando los valores de esa fila en todas las filas del primer tensor, accediendo mediante índices como i*col_A + j. Si el segundo tensor tiene forma (m, 1), se suma por filas reutilizando el valor correspondiente a cada fila. 

En caso de que no se cumpla el escenario de bias, se ejecuta la suma general. Aquí sí se incluyen tensores 1D, 2D y 3D. Primero se verifica que las dimensiones coincidan exactamente en tamaño y forma. Luego, como los datos están almacenados de manera lineal en memoria (double*), se recorre todo el arreglo usando un único bucle desde 0 hasta dim_total, realizando la suma elemento a elemento sin importar la cantidad de dimensiones.

#### Operador resta (operator-) 
funciona únicamente para tensores con dimensiones exactamente iguales (incluyendo 1D, 2D y 3D). Realiza una validación de cada dimensión y luego recorre el arreglo lineal restando elemento a elemento.

#### Operador de multiplicación (operator*) 
funciona de dos maneras distintas. La primera es la multiplicación de un tensor con otro tensor, donde se recorre y se multiplica cada par de elementos, y el otro caso es la multiplicación de un tensor por un número, donde se recorre todo el arreglo data y se construye el vector con resultados nuevos después de haber sumado cada elemento con un valor constante. 

## Polimorfismo y Transformaciones
El programa cuenta con una clase 'TensorTransform'
Además, el programa imprime las dimensiones de los tensores en cada etapa, lo que facilita verificar que las operaciones se realizan correctamente.
