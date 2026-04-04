//
// Created by burgo on 31/3/2026.
//

#ifndef TAREA1_TENSOR_H
#define TAREA1_TENSOR_H
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
using namespace std;

class TensorTransform;
class Tensor {
    double* data;
    vector<size_t> dimensiones;
    size_t dim_total;
public:
    Tensor(const vector<size_t>& shape,const vector<double>& values); //Constructor principal
    ~Tensor(); //Destructor
    double getData(size_t indice)const {return data[indice];} //Getter para data en un indice especifico
    size_t getDim_Total()const {return dim_total;} //Getter del valor de las dimensiones totales
    vector<size_t> getDimensiones()const {return dimensiones;} //Getter del vector con dimensiones
    static Tensor zeros(const vector<size_t>& _dimensiones); //Tensor pre-definido 'zeros'
    static Tensor ones(const vector<size_t>& _dimensiones); //Tensor pre-definido 'ones'
    static Tensor random(const vector<size_t>& _dimensiones,double ini,double fin); //Tensor pre-definido 'random'
    static Tensor arange(int ini, int fin); //Tensor pre-definido 'arange'
    Tensor(const Tensor& other); //Constructor de copia
    Tensor(Tensor&& other) noexcept; //Constructor de movimiento
    Tensor& operator=(const Tensor& other); //Asignador de copia
    Tensor& operator=(Tensor&& other) noexcept; //Asignador de movimiento
    Tensor apply(const TensorTransform& transform) const; //Metodo para aplicar la interfaz de transformacion
    Tensor operator+(const Tensor& tensorB) const; //sobrecarga de operador 'suma' entre tensores
    Tensor operator-(const Tensor& tensorB) const; //sobrecarga de operador 'resta' entre tensores
    Tensor operator*(const Tensor& tensorB) const; //sobrecarga de operador 'multiplicacion' entre tensores
    Tensor operator*(double num) const; //sobrecarga de operador 'multiplicacion' entre un tensor y un numero
    static Tensor concat(vector<Tensor> tensores,int eje); //metodo estatico 'concat'
    Tensor view(const vector<size_t>& nueva_dim) const; //Metodo 'view' para cambiar la organizacion de las dimensiones
    Tensor unsqueeze(size_t num) const; //Metodo 'unsqueeze' para agregar una dimension de tamanio 1, en una posicion especifica
    friend Tensor dot(const Tensor& a, const Tensor& b); //funcion amiga 'dot' para el producto punto entre tensores
    friend Tensor matmul(const Tensor& a, const Tensor& b); //funcion amiga matmul para multiplicacion matricial
    Tensor():data(nullptr),dimensiones({}),dim_total(0){} // Provisional hasta que revise
};

class TensorTransform { //clase para realizar transformaciones a cada elemento del tensor
public:
    virtual Tensor apply(const Tensor& t) const =0;
    virtual ~TensorTransform() = default;
};

class ReLU : public TensorTransform {
public:
    Tensor apply(const Tensor& t) const override;

};

class Sigmoid : public TensorTransform {
public:
    Tensor apply(const Tensor& t) const override;
};


#endif //TAREA1_TENSOR_H
