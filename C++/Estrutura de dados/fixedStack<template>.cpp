using namespace std;

template <class T=int>


class stack{
    T vet[5];
    unsigned topo;

    public:
        stack(): topo(0) {}

        stack(T val): topo(5) {
            unsigned i;
            for(i=0;i<topo;i++){
                vet[i] = val;
            }

        }
        bool push(const T &val){
            if(topo>=5)
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
        T vetor(){
            return vet[topo-1];
        }
};
