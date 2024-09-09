#include <iostream>
#include <vector>

class base{
	int elem1;
	protected:
		int elem2;
	public:
		int elem3;
		virtual void print() const;
};

class derivada : public base{
	public:
		int getElem2() const{
			return elem2;
		}
		void print() const{
			std::cout << 2; 
		}

};

class derivadaDaDerivada : public derivada{
	public:
		int getElem2() const{
			return elem2;
		}
};

int main(){
	base* pontBase;
	pontBase = new derivada;
	pontBase->print();

}
