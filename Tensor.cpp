//
// Created by burgo on 31/3/2026.
//

#include "Tensor.h"
Tensor::Tensor(const vector<size_t>& shape,const vector<double>& values) {
    if (shape.size()>3 or shape.empty()) //Valida la cantidad de dimensiones ingresadas (1-3)
        throw invalid_argument("Dimensiones no validas");
    size_t cont_val=1; //Contador temporal para determinar la cantidad de valores que debe tener el tensor
    for (const auto& i:shape)
        cont_val*=i;
    if (cont_val != values.size()) //Valida que la cantidad de valores esperados coincida con los valores ingresados
        throw invalid_argument("La cantida de valores ingresado no coincide");
    dimensiones = shape;
    data = new double[cont_val]; //Utiliza memoria dinamica para guardar los valores del tensor
    for (size_t i=0;i<values.size();i++)
        data[i] = values[i];
    dim_total = cont_val;
}

Tensor::~Tensor() { //El destructor libera la memoria dinamica reservada
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

Tensor Tensor::arange(int ini, int fin) {
    vector<double> valores;
    for (int i=ini;i<fin;i++)
        valores.push_back(i);
    size_t dimension = fin-ini;
    return Tensor({ dimension},valores);
}


//Gestion de memoria y ciclo de vida
Tensor::Tensor(const Tensor &other) { //Constructor de copia
    dimensiones = other.dimensiones;
    dim_total = other.dim_total;
    data = new double[dim_total]; //Se accede a nueva memoria dinamica para que no existan problemas con el destructor
    for (int i=0;i<dim_total;i++)
        data[i] = other.data[i];
}


Tensor::Tensor(Tensor &&other) noexcept { //Constructor de movimiento
    dimensiones = other.dimensiones;
    dim_total = other.dim_total;
    data = other.data;
    //Al ser constructor de movimiento se 'elimina' la informacion del tensor que se esta pasando
    other.data = nullptr;
    other.dim_total = 0;
    other.dimensiones={};
}

Tensor &Tensor::operator=(const Tensor &other) { //Asignador de copia
    if (this!=&other) { //Verifica que el tensor no sea asignado consigo mismo
        double* new_data = new double[other.dim_total];
        copy(other.data,other.data+other.dim_total,new_data);
        delete[] this->data; //elimina el espacio asignado para los valores originales
        data = new_data; //Se asigna a la nueva memoria dinamica reservada, con las nuevas dimensiones
        dimensiones = other.dimensiones;
        dim_total = other.dim_total;
    }
    return *this;
}

Tensor &Tensor::operator=(Tensor &&other) noexcept { //Asignador de movimiento
    if (this!=&other) { //Verifica que el tensor no sea asignado consigmo mismo
        delete[] this->data;
        dimensiones = other.dimensiones;
        dim_total = other.dim_total;
        //Se elimina la informacion asociada a los valores del tensor ingresado.
        data = other.data;
        other.data = nullptr;
        other.dim_total=0;
    }
    return *this;
}


//Sobrecarga de operadores
Tensor Tensor::operator+(const Tensor &tensorB) const {
    if (this->dimensiones.size()!=tensorB.dimensiones.size()) //Verifica que los tensores tengan la misma cantidad de dimensiones
        throw invalid_argument("Las dimensiones de los tensores no coinciden");

    //Suma con bias (solo aplica para tensores 2D). Tomamos la notacion 'fila X columna'
    //Asumimos que en la suma bias, el primer tensor es el que tiene dimensiones con valores mayores
    if (dimensiones.size()==2 and tensorB.dimensiones.size()==2) {
        size_t fil_A = dimensiones[0]; //Cantidad de filas del primer tensor
        size_t col_A = dimensiones[1]; //Cantidad de columnas del primer tensor
        size_t fil_B = tensorB.dimensiones[0]; //Cantidad de filas del segundo tensor
        size_t col_B = tensorB.dimensiones[1]; //Cantidad de columnas del segundo tensor
        if (fil_B==1 and col_A==col_B) { //Verifica si la dimension con valor 1 se encuentra en la 'fila' del segundo tensor
            vector<double> resultado(dim_total);
            for (size_t i=0;i<fil_A;i++)
                for (size_t j=0;j<col_A;j++)
                    resultado[i*col_A+j] = data[i*col_A+j] + tensorB.data[j];
            return Tensor(dimensiones,resultado);
        }
        else if (col_B==1 and fil_A==fil_B) { //Verifica si la dimension con valor 1 se encuentra en la 'columna' del segundo tensor
            vector<double> resultado(dim_total);
            for (size_t i=0;i<fil_A;i++)
                for (size_t j=0;j<col_A;j++)
                    resultado[i*col_A+j] = data[i*col_A+j]+tensorB.data[i];
            return Tensor(dimensiones,resultado);
        }
        else
            throw invalid_argument("Las dimensiones no son validas");
        //Se envia un mensjae de error en el caso que el segundo tensor no tenga las dimensiones adeacuadas para la suma bias
    }

    //Suma general, permite tensores 1D, 2D y 3D
    else if (dimensiones == tensorB.dimensiones) {
        for (size_t i=0;i<this->dimensiones.size();i++)
            if (this->dimensiones[i]!=tensorB.dimensiones[i]) //Verifica que el valor d elas dimensiones sean iguales
                throw invalid_argument("Las dimensiones de los trensores no coinciden");
        vector<double> resultado(dim_total);
        for (size_t i=0;i<this->dim_total;i++)
            resultado[i] = this->data[i]+tensorB.data[i];
        return Tensor(this->dimensiones,resultado);
    }
}


Tensor Tensor::operator-(const Tensor &tensorB) const {
    if (this->dimensiones.size()!=tensorB.dimensiones.size())
        throw invalid_argument("Las dimensiones de los tensores no coinciden");
    for (size_t i=0;i<this->dimensiones.size();i++)
        if (this->dimensiones[i]!=tensorB.dimensiones[i])
            throw invalid_argument("Las dimensiones de los trensores no coinciden");
    vector<double> resultado(dim_total);
    for (size_t i=0;i<this->dim_total;i++)
        resultado[i] = this->data[i]-tensorB.data[i];
    return Tensor(this->dimensiones,resultado);
}

Tensor Tensor::operator*(const Tensor &tensorB) const {
    if (this->dimensiones.size()!=tensorB.dimensiones.size())
        throw invalid_argument("Las dimensiones de los tensores no coinciden");
    for (size_t i=0;i<this->dimensiones.size();i++)
        if (this->dimensiones[i]!=tensorB.dimensiones[i])
            throw invalid_argument("Las dimensiones de los trensores no coinciden");
    vector<double> resultado(dim_total);
    for (size_t i=0;i<this->dim_total;i++)
        resultado[i] = this->data[i]*tensorB.data[i];
    return Tensor(this->dimensiones,resultado);
}

Tensor Tensor::operator*(double num) const{
    vector<double> resultado;
    for (size_t i=0;i<this->dim_total;i++)
        resultado.push_back(this->data[i]*num);
    return Tensor(this->dimensiones,resultado);
}

//Override del metodo apply para la clase ReLU y Sigmoid
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

//Aplicacion del metodo apply en la clase Tensor
Tensor Tensor::apply(const TensorTransform &transform) const {
    return transform.apply(*this);
}

Tensor Tensor::concat(vector<Tensor> tensores, int eje) {
    if (eje<0 or eje>3) //Verifica que el eje ingresado sea valido (0-2)
        throw invalid_argument("Eje no valido");
    if (tensores.empty())
        throw invalid_argument("No existen tensores para concatenar");
    for (const auto& tensor:tensores)
        if (tensor.dim_total==0)
            throw invalid_argument("No podemos concatenar un tensor vacio");
    if (tensores[0].dimensiones.size()!=tensores[1].dimensiones.size())
        throw invalid_argument("No podemos concatenar tensores con diferente dimensionalidad");
    //Realizamos un 'truco' dependiendo de la cantidad de dimensiones del primer tensor
    //Esto nos hace mas facil manejar las tres dimenesiones en los condicionales
    if (tensores[0].dimensiones.size()==1) {
        tensores[0].dimensiones.push_back(1);
        tensores[0].dimensiones.push_back(1);
    }
    if (tensores[0].dimensiones.size()==2)
        tensores[0].dimensiones.push_back(1);

    size_t P,F,C; //Tomamos tres dimensiones P(profundidad), F(filas), C(columnas)
    vector<size_t> copiaTensor = tensores[0].dimensiones; //Copia de las dimensiones del primer tensor
    copiaTensor[eje]+=tensores[1].dimensiones[eje]; //estamos garantizando que es para dos tensores
    size_t newDimension_Total = copiaTensor[0]*copiaTensor[1]*copiaTensor[2];
    double* newTensor;

    if (eje==0) { //Al tener 3 dimensiones, el eje 0 corresponde a la profundidad
        if (tensores[0].dimensiones[1]!=tensores[1].dimensiones[1] or tensores[0].dimensiones[2]!=tensores[1].dimensiones[2])
            throw invalid_argument("Dimensiones incorrectas");

        F = tensores[0].dimensiones[1];
        C = tensores[0].dimensiones[2];
        newTensor= new double[newDimension_Total];
        size_t desplazamiento=0;
        for (const auto& tensor:tensores) {
            size_t dim_tensorAct = tensor.dimensiones[0]*F*C;
            for (size_t i=0;i<dim_tensorAct;i++)
                newTensor[desplazamiento+i] = tensor.data[i];
            desplazamiento+=dim_tensorAct;
        }
    }
    else if (eje==1) { //El eje 1 corresponde a las filas
        if (tensores[0].dimensiones[0]!=tensores[1].dimensiones[0] or tensores[0].dimensiones[2]!=tensores[1].dimensiones[2])
            throw invalid_argument("Dimensiones incorrectas");
        P = tensores[0].dimensiones[0];
        C = tensores[0].dimensiones[2];
        newTensor= new double[newDimension_Total];
        size_t desplazamiento=0;
        for (size_t i=0;i<P;i++)
            for (const auto& tensor:tensores) {
                size_t filasTensor = tensor.dimensiones[1];
                size_t indice = i*(filasTensor*C);
                for (size_t j=0;j<filasTensor*C;j++) {
                    newTensor[desplazamiento] = tensor.data[indice+j];
                    desplazamiento++;
                }
            }
    }
    else { //El eje restante (2) corresponde a las columnas
        if (tensores[0].dimensiones[0]!=tensores[1].dimensiones[0] or tensores[0].dimensiones[1]!=tensores[1].dimensiones[1])
            throw invalid_argument("Dimensiones incorrectas");
        P = tensores[0].dimensiones[0];
        F = tensores[0].dimensiones[1];
        newTensor= new double[newDimension_Total];
        size_t desplazamiento=0;
        for (size_t i=0;i<P;i++)
            for (size_t j=0;j<F;j++)
                for (const auto& tensor:tensores) {
                    size_t colTensor = tensor.dimensiones[2];
                    size_t indice = (i*F*colTensor) + (j*colTensor);
                    for (size_t k=0;k<colTensor;k++) {
                        newTensor[desplazamiento] = tensor.data[indice+k];
                        desplazamiento++;
                    }
                }
    }
    Tensor TensorResul;
    TensorResul.data = newTensor;
    TensorResul.dimensiones = {P,F,C};
    TensorResul.dim_total = P*F*C;
    return move(TensorResul);
}


Tensor Tensor::view(const vector<size_t>& nueva_dim) {
    if (nueva_dim.size()>3 or nueva_dim.empty()) //Valida que las nuevas dimensiones sean apropiadas ()
        throw invalid_argument("Las dimensiones no son validas");
    size_t dim_tem = 1;
    for (const auto& i:nueva_dim)
        dim_tem*=i;
    if (dim_tem!=this->dim_total) //Valida que la cantida de valores previstos sea igual al del tensor
        throw invalid_argument("Las dimensiones no son compatibles");
    Tensor resultado;
    resultado.data = this->data;
    resultado.dimensiones = nueva_dim;
    resultado.dim_total = dim_tem;

    this->data = nullptr;
    this->dimensiones = {};
    this->dim_total=0;

    return resultado;
}

Tensor Tensor::unsqueeze(size_t num) {
    if (this->dimensiones.size()==3) //Como este metodo agrega una dimension con valor 1, el Tensor no debe tener 3 dimensiones
        throw invalid_argument("El limite maximo de dimensiones es 3");
    if (num>2 or num<0) //El numero ingresado debe estar entre 0-2, ya que se refiere a la posicion en la que se añadira la nueva dimension
        throw invalid_argument("Dimension no valida");
    Tensor resultado;
    resultado.data = this->data;
    resultado.dim_total = this->dim_total;
    resultado.dimensiones = this->dimensiones;
    resultado.dimensiones.insert(resultado.dimensiones.begin()+num,1);
    //Usamos el metodo insert para ingresar la dimension de valor 1 en la posicion indicada

    this->data = nullptr;
    this->dimensiones = {};
    this->dim_total=0;

    return resultado;
}

Tensor dot(const Tensor& a, const Tensor& b) {
    if (a.dimensiones != b.dimensiones) //Valida que las dimensiones de los dos tensores sean iguales
        throw invalid_argument("Dimensiones incompatibles para dot");
    double suma = 0;
    for (size_t i = 0; i < a.dim_total; i++)
        suma += a.data[i] * b.data[i];
    return move(Tensor({1},{suma}));
}


Tensor matmul(const Tensor& a, const Tensor& b) {
    if (a.dimensiones.size()!=2 or b.dimensiones.size()!=2) //Valida que los tensores sean bidimensionales
        throw invalid_argument("matmul solo soporta tensores 2D");
    //Al ser tensores bidimensionales, utilizamos la notacion 'filas X columnas' para mejor comprension
    size_t m = a.dimensiones[0]; //Cantidad de filas del primer tensor
    size_t n = a.dimensiones[1]; //Cantidad de columnas del primer tensor
    size_t n2 = b.dimensiones[0]; //Cantidad de filas del segundo tensor
    size_t p = b.dimensiones[1]; //Cantidad de columnas del segundo tensor
    if (n != n2) //Para el producto matricial se requiere que las columnas del primer tensor sean iguales a las filas del segundo
        throw invalid_argument("Dimensiones incompatibles para matmul");
    vector<double> valores(m * p);
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < p; j++) {
            double suma = 0;
            for (size_t k = 0; k < n; k++) {
                size_t indexA = i * n + k;
                size_t indexB = k * p + j;
                suma += a.data[indexA] * b.data[indexB];
            }
            valores[i * p + j] = suma;
        }
    }
    return move(Tensor({m,p},valores));
}
//(OPCIONAL) Sobrecarga de operador '<<' para verificar el funcionamiento de la red neuronal
ostream& operator<<(ostream& os,const Tensor& t) {
    if (t.dimensiones.size()==0)
        return os;
    vector<size_t> dims = t.dimensiones;
    size_t num_dims = t.dim_total;
    //Imprimir tensores 1D
    if (dims.size()==1) {
        os<<"<<<---------------------------------->>>\n";
        for (size_t i=0;i<num_dims;i++)
            os<<setw(10)<<t.data[i];
        os<<"\n";
    }
    //Imprimir 2D
    else if (dims.size()==2) {
        os<<"<<<---------------------------------->>>\n";
        for (size_t i=0;i<num_dims;i++) {
            os<<setw(10)<<t.data[i];
            if ((i+1)%dims[1]==0)
                os<<"\n";
        }
    }
    //Imprimir 3D
    else {
        os<<"<<<---------------------------------->>>\n";
        int cont_capas = 1;
        os<<"Capa: "<<cont_capas<<"\n";
        for (size_t i=0;i<num_dims;i++) {
            os<<setw(10)<<t.data[i];
            if ((i+1)%(dims[0]*dims[1])==0 and (i+1)<num_dims) {
                cont_capas++;
                os<<"\n\nCapa : "<<cont_capas<<"\n";
            }
            else if ((i+1)%dims[1]==0)
                os<<"\n";
        }
    }
    return os;
}
