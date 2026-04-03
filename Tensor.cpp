//
// Created by burgo on 31/3/2026.
//

#include "Tensor.h"
Tensor::Tensor(const vector<size_t>& shape,const vector<double>& values) {
    size_t x,y,z;
    if (shape.size() == 1) {
        x = shape[0],y = 1, z = 1;
    }
    else if (shape.size() == 2) {
        x = shape[0],y = shape[1],z = 1;
    }
    else if (shape.size() == 3) {
        x = shape[0],y = shape[1],z = shape[2];
    }
    else {
        throw invalid_argument("Dimensiones invalidas");
    }
    dimensiones = {x,y,z};
    size_t cont_val=1;
    for (const auto& i:shape)
        cont_val*=i;
    if (cont_val != values.size()-1)
        throw invalid_argument("La cantida de valores ingresado no coincide");
    data = new double[cont_val];
    for (int i=0;i<values.size();i++)
        data[i] = values[i];
    dim_total = cont_val;
}

Tensor::~Tensor() {
    delete[] data;
}

//Tensores pre-definidos
Tensor Tensor::zeros(const vector<size_t>& _dimensiones) {
    size_t cont_val=1;
    for (const auto& i:_dimensiones)
        cont_val*=i;
    return Tensor(_dimensiones,vector<double>(cont_val,0));
}

Tensor Tensor::ones(const vector<size_t>& _dimensiones) {
    size_t cont_val=1;
    for (const auto& i:_dimensiones)
        cont_val*=i;
    return Tensor(_dimensiones,vector<double>(cont_val,1));
}

Tensor Tensor::random(const vector<size_t>& _dimensiones, double ini, double fin) {
    size_t cont_temp=1;
    for (const auto& i:_dimensiones)
        cont_temp*=i;
    vector<double> valores;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(ini,fin); //[inicio,fin)
    for (int i=0;i<cont_temp;i++)
        valores.push_back(dist(gen));
    return Tensor(_dimensiones,valores);
}


//Podriamos añadir un verificador del numero mayor (control de errores)
Tensor Tensor::arange(int ini, int fin) {
    vector<double> valores;
    for (int i=ini;i<fin;i++)
        valores.push_back(i);
    size_t dimension = fin-ini;
    return Tensor({ dimension},valores);
}


//Gestion de memoria y ciclo de vida
Tensor::Tensor(const Tensor &other) {
    dimensiones = other.dimensiones;
    dim_total = other.dim_total;
    data = new double[dim_total];
    for (int i=0;i<dim_total;i++)
        data[i] = other.data[i];
}


Tensor::Tensor(Tensor &&other) noexcept {
    dimensiones = other.dimensiones;
    dim_total = other.dim_total;
    data = other.data;
    other.data = nullptr;
}

Tensor &Tensor::operator=(const Tensor &other) {
    if (this!=&other) {
        double* new_data = new double[other.dim_total];
        copy(other.data,other.data+other.dim_total,new_data);
        delete[] this->data;
        data = new_data;
        dimensiones = other.dimensiones;
        dim_total = other.dim_total;
    }
    return *this;
}

Tensor &Tensor::operator=(Tensor &&other) noexcept {
    if (this!=&other) {
        delete[] this->data;
        dimensiones = other.dimensiones;
        dim_total = other.dim_total;
        data = other.data;
        other.data = nullptr;
        other.dim_total=0;
    }
    return *this;
}


//Sobrecarga de operadores
Tensor Tensor::operator+(const Tensor &tensorB) {
    if (this->dimensiones.size()!=tensorB.dimensiones.size())
        throw invalid_argument("Las dimensiones de los tensores no coinciden");
    for (size_t i=0;i<this->dimensiones.size();i++)
        if (this->dimensiones[i]!=tensorB.dimensiones[i])
            throw invalid_argument("Las dimensiones de los trensores no coinciden");
    vector<double> resultado;
    for (size_t i=0;i<this->dim_total;i++)
        resultado.push_back(this->data[i]+tensorB.data[i]);
    return Tensor(this->dimensiones,resultado);
}

Tensor Tensor::operator-(const Tensor &tensorB) {
    if (this->dimensiones.size()!=tensorB.dimensiones.size())
        throw invalid_argument("Las dimensiones de los tensores no coinciden");
    for (size_t i=0;i<this->dimensiones.size();i++)
        if (this->dimensiones[i]!=tensorB.dimensiones[i])
            throw invalid_argument("Las dimensiones de los trensores no coinciden");
    vector<double> resultado;
    for (size_t i=0;i<this->dim_total;i++)
        resultado.push_back(this->data[i]-tensorB.data[i]);
    return Tensor(this->dimensiones,resultado);
}

Tensor Tensor::operator*(const Tensor &tensorB) {
    if (this->dimensiones.size()!=tensorB.dimensiones.size())
        throw invalid_argument("Las dimensiones de los tensores no coinciden");
    for (size_t i=0;i<this->dimensiones.size();i++)
        if (this->dimensiones[i]!=tensorB.dimensiones[i])
            throw invalid_argument("Las dimensiones de los trensores no coinciden");
    vector<double> resultado;
    for (size_t i=0;i<this->dim_total;i++)
        resultado.push_back(this->data[i]*tensorB.data[i]);
    return Tensor(this->dimensiones,resultado);
}

Tensor Tensor::operator*(double num) {
    vector<double> resultado;
    for (size_t i=0;i<this->dim_total;i++)
        resultado.push_back(this->data[i]*num);
    return Tensor(this->dimensiones,resultado);
}

Tensor ReLU::apply(const Tensor &t) const {
    vector<double> valores;
    for (size_t i=0;i<t.getDim_Total();i++) {
        if (t.getData(i)<0)
            valores.push_back(0);
        else
            valores.push_back(t.getData(i));
    }
    return Tensor(t.getDimensiones(),valores);
}

Tensor Sigmoid::apply(const Tensor &t) const {
    vector<double> valores;
    for (size_t i=0;i<t.getDim_Total();i++) {
        double x = t.getData(i);
        valores.push_back(1/(1+exp(-x)));
    }
    return Tensor(t.getDimensiones(),valores);
}

Tensor Tensor::apply(const TensorTransform &transform) const {
    return transform.apply(*this);
}

Tensor Tensor::concat(const vector<Tensor> &tensores, int eje) {
    if (eje<0 or eje>3)
        throw invalid_argument("Eje no valido");
    if (eje==0) {
        if (tensores[0].dimensiones[1]!=tensores[1].dimensiones[1] and tensores[0].dimensiones[2]!=tensores[1].dimensiones[2])
            throw invalid_argument("Dimensiones incorrectas");
        size_t newDimension_Total = (tensores[0].dimensiones[0]+tensores[1].dimensiones[0])*tensores[0].dimensiones[1]*tensores[0].dimensiones[2];
        double* newTensor = new double[newDimension_Total];
        


    }



}
