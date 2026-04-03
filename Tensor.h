//
// Created by burgo on 31/3/2026.
//

#ifndef TAREA1_TENSOR_H
#define TAREA1_TENSOR_H
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "Tensor.h"
using namespace std;

class TensorTransform;
class Tensor {
    double* data;
    vector<size_t> dimensiones;
    size_t dim_total;
public:
    Tensor(const vector<size_t>& shape,const vector<double>& values);
    ~Tensor();
    double getData(size_t indice)const {return data[indice];} //Getter
    size_t getDim_Total()const {return dim_total;}
    vector<size_t> getDimensiones()const {return dimensiones;}
    static Tensor zeros(const vector<size_t>& _dimensiones);
    static Tensor ones(const vector<size_t>& _dimensiones);
    static Tensor random(const vector<size_t>& _dimensiones,double ini,double fin);
    static Tensor arange(int ini, int fin);
    Tensor(const Tensor& other);
    Tensor(Tensor&& other) noexcept;
    Tensor& operator=(const Tensor& other);
    Tensor& operator=(Tensor&& other) noexcept;
    Tensor apply(const TensorTransform& transform) const; // aspecto 5
    Tensor operator+(const Tensor& tensorB);
    Tensor operator-(const Tensor& tensorB);
    Tensor operator*(const Tensor& tensorB);
    Tensor operator*(double num);
    static Tensor concat(const vector<Tensor>& tensores,int eje);
};

class TensorTransform {
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
