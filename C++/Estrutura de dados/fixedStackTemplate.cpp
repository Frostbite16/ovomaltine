using namespace std;

template <class T=int>


class stack{
    T vet[100];
    unsigned topo;

    public:
        stack(): topo(0) {}

        stack(T val): topo(100) {
            unsigned i;
            for(i=0;i<topo;i++){
                vet[i] = val;
            }

        }
        
        int GetTopo() const{
            return topo;
        }

        bool push(const T &val){
            if(topo>=100)
                return false;
            vet[topo++]=val;
            return true;
        }
        bool pop(){
            if(topo<=0)
                return false;
            --topo;
            return true;
        }
        T ReturnTopo(){
            return *vet[topo-1];
        }
        T readStack(const int i;){
            return *vet[topo-i];
        }
};
