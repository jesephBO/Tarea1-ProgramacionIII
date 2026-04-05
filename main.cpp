#include "Tensor.h"

int main() {
    //Se crea la red neuronal del apartado 10
    //Para validar la funcionalidad de la red neuronal se imprimen las dimensiones de los tensores
    Tensor prueba({2,2,2},{1,2,3,4,5,6,7,8});
    Tensor A = Tensor::random({1000,20,20},-0.1,0.1);
    cout << "A shape: ";for(auto d : A.getDimensiones()) cout << d << " ";cout << endl;
    Tensor B = A.view({1000,400});
    cout << "B shape: ";for(auto d : B.getDimensiones()) cout << d << " ";cout << endl;
    Tensor W1 = Tensor::random({400,100},-0.1,0.1);
    Tensor C = matmul(B, W1);
    cout << "C shape: ";for(auto d : C.getDimensiones()) cout << d << " ";cout << endl;
    Tensor b1 = Tensor::random({100},-0.1,0.1);
    Tensor b1_1 = b1.unsqueeze(0);
    Tensor D = C+b1_1;
    cout << "D shape: ";for(auto d : D.getDimensiones()) cout << d << " ";cout << endl;
    ReLU relu;
    Tensor E = D.apply(relu);
    cout << "E shape: ";for(auto d : E.getDimensiones()) cout << d << " ";cout << endl;
    Tensor W2 = Tensor::random({100,10},-0.1,0.1);
    Tensor F = matmul(E, W2);
    cout << "F shape: ";for(auto d : F.getDimensiones()) cout << d << " ";cout << endl;
    Tensor b2 = Tensor::random({10},-0.1,0.1);
    Tensor b2_2 = b2.unsqueeze(0);
    Tensor G = F + b2_2;
    cout << "G shape: ";for(auto d : G.getDimensiones()) cout << d << " ";cout << endl;
    Sigmoid sigmoid;
    Tensor H = G.apply(sigmoid);
    cout << "H shape: ";for(auto d : H.getDimensiones()) cout << d << " ";cout << endl;
    cout<<H;
    return 0;
}