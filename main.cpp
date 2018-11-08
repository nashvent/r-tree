#include <bits/stdc++.h>
using namespace std;
typedef float TypeData;
typedef vector<float> Data;
typedef vector<Data> vData;
/*
class IR{ //Index record / No implementado por el momento
};
*/

float area(vData mm){
    float areaCalc=1;
    for(int i=0;i<mm.size();i++){
        areaCalc=areaCalc*(mm[i][1]-mm[i][0]);
    }
    return areaCalc;
}

vData makeRectangle(vData E1,vData E2){
    vData R;
    for(int i=0;i<E1.size();i++){
        vector<float>tempDim(2);
        if(E1[i][0]>E2[i][0])
            tempDim[0]=E2[i][0];
        else
            tempDim[0]=E1[i][0];

        if(E1[i][1]<E2[i][1])
            tempDim[1]=E2[i][1];
        else
            tempDim[1]=E1[i][1];
        R.push_back(tempDim);
    }
    return R;
}


struct Nodo{
    bool isLeaf,wasSplit;
    vData tuples; //tuplas [tupla,tupla,tupla] || [x,y,z,r,...],[x,y,z,r,...],[x,y,z,r,...]
    vData I; //Limites del rectangulo I Nodo x:[min,max] y:[min,max] z:[min,max] r:[min,max]
    vector<Nodo*>child; // [Nodo1,Nodo2,Nodo3,Nodo4]
    int dim;
    Nodo *parent;
    Nodo(int n_dim,Nodo*n_parent=NULL){
        dim=n_dim;
        isLeaf=true;
        parent=n_parent;
        I.resize(n_dim);
    }
    bool overlap(vData pI){
        for(int i=0;i<dim;i++){
            if(I[i][0]>pI[i][0] or pI[i][1]>I[i][1]){
                return false;
            }
        }
        return true;
    }

    void addEntry(Data dt){
        tuples.push_back(dt);
    }

    bool searchData(Data T){
        if ( std::find(tuples.begin(), tuples.end(), T) != tuples.end() )
            return true;
        return false;
    }
    

    float calcArea(vData dt){
        vData nArea=I;
        for(int i=0;i<dim;i++){
            if(nArea[i][0]>dt[i][0]){
                nArea[i][0]=dt[i][0];
            }
            if(nArea[i][1]<dt[i][1]){
                nArea[i][1]=dt[i][0];
            }
        }
        return area(nArea)-area(I);
    }

    float updateRectangleI(){
        for(int i=0;i<child.size();i++){
            for(int j=0;j<dim;j++){
                if(child[i]->I[j][0] < I[j][0] ){
                    I[j][0]=child[i]->I[j][0];
                }
                if(child[i]->I[j][1] > I[j][1] ){
                    I[j][1]=child[i]->I[j][1];
                }
            }
        }
    }

};


struct RTree{
    int M,m;
    int dim;
    Nodo *root;
    RTree(int n_M, int n_m,int n_dim){
        M=n_M;
        m=n_m;
        root=new Nodo(n_dim);
        dim=n_dim;
    }

    void chooseLeaf(vData E,Nodo *N){
        N=root;
        while(!N->isLeaf){
            float tempArea=10000;
            Nodo *TN;
            for(int i=0;i<N->child.size();i++){
                float nTempArea=N->child[i]->calcArea(E);
                if(tempArea>nTempArea){
                    TN=N->child[i];
                }
            }
            N=TN;
        }
        return;
    }

    void splitNode(Nodo* dt){
        Nodo *E1,*E2;
        float Ed1,Ed2;
        vector<Nodo*>LP;
        LP=dt->child;
        pickSeeds(LP,E1,E2);

        Nodo *G1,*G2;
        G1=new Nodo(dim);
        G2=new Nodo(dim);
        G1->child.push_back(E1);
        G2->child.push_back(E2);
        G1->I=E1->I;
        G2->I=E2->I;
        while(LP.size()>0){
            if(G1->child.size()>=m or G2->child.size()>=m and(G1->child.size()!=G2->child.size())){
                if(G1->child.size()<m ){
                    G1->child.insert( G1->child.end(), LP.begin(), LP.end());
                    break;
                }
                if(G2->child.size()<m){
                    G2->child.insert( G2->child.end(), LP.begin(), LP.end());
                    break;
                }
                
            }

        }
        
    }

    void pickSeeds(vector<Nodo*>&LP,Nodo* &E1,Nodo* &E2){
        float d=0;
        int indxE1,indxE2;
        for(int x1=0;x1<LP.size();x1++){
            for(int x2=0;x2<LP.size();x2++){
                if(x1!=x2){
                    vData J=makeRectangle(LP[x1]->I,LP[x2]->I);
                    float td=area(J)*area(LP[x1]->I)*area(LP[x2]->I);
                    if(td>d){
                        E1=LP[x1];
                        E2=LP[x2];
                        d=td;
                        indxE1=x1;
                        indxE2=x2;
                    }
                }
            }
        }
        /*Limpiar grupos*/
        if(indxE1>indxE2){
            swap(indxE1,indxE2);
        }
        LP.erase(LP.begin()+indxE2);
        LP.erase(LP.begin()+indxE1);
    }

    void pickNext(vector<Nodo*>&LP,Nodo* &G1,Nodo* &G2){
        int indxE1,indxE2;
        float dG1=1000000,dG2=1000000;
        vData G1,G2;
        float aG1=area(G1->I);
        float aG2=area(G2->I);
        vData ntG1,nTG2;
        for(int i=0;i<LP.size();i++){
            vData tG1=makeRectangle(G1->I,LP[i]->I);
            vData tG2=makeRectangle(G2->I,LP[i]->I);
            float tdG1=area(tG1)-aG1;
            float tdG2=area(tG2)-aG2;
            if(tdG1<dG1){
                indxE1=i; //Indice en LP
                dG1=tdG1; //Diferencia agregando la nueva entrada
                ntG1=tG1; //nuevo Rectangulo ->  I
            }
            if(tdG2<dG2){
                indxE2=i;
                dG2=tdG2;
                nTG2=tG2            
            }
            
        }

        if(dG1<dG2){
            G1->child.push_back(LP[indxE1]);
            G1->I=ntG1;
            LP.erase(LP.begin()+indxE1);
            return;
        }
        else{   
            G2->child.push_back(LP[indxE2]);
            G2->I=ntG2;
            LP.erase(LP.begin()+indxE2);
            return;
        }
    }

    void adjustTree(Nodo* L,Nodo*NN=NULL){ // Expand tree 
        Nodo *N=L;
        while(L!=root){
            Nodo *P=N->parent;
            for(int i=0;i<dim;i++){
                if(N->I[i][0] < P->I[i][0])
                    P->I[i][0] = N->I[i][0];
                if(N->I[i][1] > P->I[i][1])
                    P->I[i][1] = N->I[i][1];
            }   

            if(NN!=NULL){
                Nodo *ENN=new Nodo(dim);
                //Propagate node split een progresooo
            }
            N=P;
        }
    }

    bool search(Nodo *&p,vector<Data> pI){
        if(p->isLeaf){
            return p->overlap(pI);
        }
        for(int x=0;x<p->child.size();x++){
            Nodo*currChild=p->child[x];
            if(currChild->overlap(pI)){
                search(currChild,pI);
            }
        }
        return false;
    }

    bool insert(Data E){ 
        Nodo* L,*LL;
        chooseLeaf(E,L);
        if(L->tuples.size()<M){
            L->addEntry(dt);
        }
        else{
            splitNode(E,L,LL);
            L->addEntry(dt);
        }
        adjustTree(L,LL); // SI no hubo split LL==NULL
        //SI la raiz se dividio crear una nueva raiz y q sus dos hijos sean la division
    
        return true;
    }
    
};


int main(){
    
    return 0;
}