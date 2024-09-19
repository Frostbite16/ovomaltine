#include<vector>
#include<iostream>
#include<cmath>

using namespace std;

template<typename T=int>
class ordenedVec:public vector<T>{
	public:
		ordenedVec()
		{}
		
		void inserir(T data){
			size_t index;
			bool alocou=0;
			if(!(this->size())){
				this->push_back(data);
				alocou = 1;
			}
			else{
				index=this->size()-1;
				for(index=this->size()-1;((*this)[index]>data)&&(index>0);index--){
					if(!(alocou)){
						this->push_back((*this)[index]);
						alocou=1;
					}
					else{
						(*this)[index+1]=(*this)[index];
					}
				}
				if(!(alocou)){
					if((*this)[index]>data){
						this->push_back((*this)[index]);
						(*this)[index] = data;
					}
					else{
						this->push_back(data);
					}
				}
				else{
					if((*this)[index]>data){
						(*this)[index+1] = (*this)[index];
						(*this)[index] = data;
					}
					else{
						(*this)[index] = data;
					}	
				}
			}
		}

		void removeValue(T data){
			typename vector<T>::iterator it;
			for(it=this->begin(); (it!=this->end())&&(*it!=data); ++it);
			if(*it==data){
				this->erase(it);
			}	
		}

		void insertMiddle(T data){
			typename vector<T>::iterator it;
			float middle;
			middle = static_cast<float>(this->size())/2;
			it = this->begin();
			it += static_cast<int>(middle);
			this->insert(it, data);
			cout<<"Done";

		}

		~ordenedVec()
		{}
};

int main(){
	ordenedVec ov;
	for(int i=75; i>=70; i--){
		ov.inserir(i);
	}
	ov.inserir(105);
	ov.inserir(-15);
	ov.inserir(100);
	ov.removeValue(75);
	return 1;
}

