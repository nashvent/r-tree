#include <bits/stdc++.h>
using namespace std;
typedef vector<float> Data;
/*
class IR{ //Index record / No implementado por el momento
};
*/

struct Nodo{
    bool isLeaf;
    vector<Data> tuples; //tupla 
    vector<Data>I; //Limites del rectangulo I Nodo
    vector<Nodo*>child;
    int dim;
    Nodo(int n_dim){
        dim=n_dim;
        isLeaf=true;
    }
    bool overlap(Data n_data){
        return true;
    }
    void addEntry(Data dt){
        tuples.push_back(dt);
    }
};


struct RTree{
    int M,m;

    Nodo *root;
    RTree(int n_M, int n_m,int dim){
        M=n_M;
        m=n_m;
        root=new Nodo(dim);
    }
    Nodo* chooseLeaf(Data dt){

    }
    Nodo* splitNode(Nodo* dt){

    }

    bool search(Nodo *&p,Data dt){
        if(p->isLeaf){
            return true;
        }
        for(int x=0;x<p->child.size();x++){
            Nodo*currChild=p->child[x];
            if(currChild->overlap(dt)){
                search(currChild,dt);
            }
        }
        return false;
    }

    bool insert(Data dt){
        Nodo* p=chooseLeaf(dt);
        if(p->tuples.size()<M){
            p->addEntry(dt);
        }
        else{
            Nodo *np=splitNode(p);
            np->addEntry(dt);
        }
        return true;
    }
    
};


int main(){
    
    return 0;
}