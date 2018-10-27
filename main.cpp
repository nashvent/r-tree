#include <bits/stdc++.h>
using namespace std;
typedef vector<float> Data;

class IR{ //Index record / No implementado por el momento
};

class Nodo{
    bool isLeaf;
    vector<Data>IR; //Index Record en n dimesion / donde Data = I, su propio limite
    vector<Data>IN; //Limites del rectangulo I Nodo
    Nodo*child;
    int dim;
    Nodo(int n_dim){
        dim=n_dim;
        child=NULL;
        isLeaf=true;
    }
};


class RTree{
    int M,m;

    RTree(int n_M, int n_m){
        M=n_M;
        m=n_m;
    }

};


int main(){
    
    return 0;
}