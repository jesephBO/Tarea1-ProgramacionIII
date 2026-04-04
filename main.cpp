#include "Tensor.h"

int main() {
    Tensor A = Tensor::random({1000,20,20},0.0,10.0);
    cout << "A shape: ";for(auto d : A.getDimensiones()) cout << d << " ";cout << endl;
    Tensor B = A.view({1000,400});
    cout << "B shape: ";for(auto d : B.getDimensiones()) cout << d << " ";cout << endl;
    Tensor W1 = Tensor::random({400,100},0.0,10.0);
    Tensor C = matmul(B, W1);
    cout << "C shape: ";for(auto d : C.getDimensiones()) cout << d << " ";cout << endl;
    Tensor b1 = Tensor::random({100},0.0,10.0);
    Tensor b1_1 = b1.unsqueeze(0);
    Tensor D = C+b1_1;
    cout << "D shape: ";for(auto d : D.getDimensiones()) cout << d << " ";cout << endl;
    ReLU relu;
    Tensor E = D.apply(relu);
    cout << "E shape: ";for(auto d : E.getDimensiones()) cout << d << " ";cout << endl;
    Tensor W2 = Tensor::random({100,10},0.0,10.0);
    Tensor F = matmul(E, W2);
    cout << "F shape: ";for(auto d : F.getDimensiones()) cout << d << " ";cout << endl;
    Tensor b2 = Tensor::random({10},0.0,10.0); //Cambiado
    Tensor b2_2 = b2.unsqueeze(0);
    Tensor G = F + b2_2;
    cout << "G shape: ";for(auto d : G.getDimensiones()) cout << d << " ";cout << endl;
    Sigmoid sigmoid;
    Tensor H = G.apply(sigmoid);
    cout << "H shape: ";for(auto d : H.getDimensiones()) cout << d << " ";cout << endl;


    return 0;
}