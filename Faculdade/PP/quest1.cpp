#include<vector>
#include<iostream>

using namespace std;

template<typename T=int>
class ordenedVector: vector<T>{
	vector<T> lista;
	public:
		ordenedVector()
		{}
		
		void insert(T data){
			typename vector<T>::iterator index, local;
			bool allocated=0;
			size_t i=lista.size()-1;
			if(!(lista.size())){
				lista.push_back(data);
				allocated = 1;
			}
			else{
				for(index=lista.end(); (*index>data)&&(index!=lista.begin()); index--, i--){
					if(!(allocated)){
						lista.push_back(*index);
						allocated = 1; 
					}
					else{
						lista[i+1] = lista[i];
					}
				}
				if(!(allocated)){
					if(*index>data){
						lista.push_back(*index);
						lista[i] = data;
						allocated = 1;
					}
					else{
						lista.push_back(data);
						allocated = 1;
					}
				}	
				else{
					if(index==lista.begin()){
						lista[i+1] = lista[i];
						lista[i] = data;
					}
					else{
						lista[i] = data;
					}
				}

			}
		}

};

int main(){
	ordenedVector<int> Ov;
	Ov.insert(5);
	Ov.insert(4);
	Ov.insert(3);
	Ov.insert(2);
	Ov.insert(1);


	return 1;
}

