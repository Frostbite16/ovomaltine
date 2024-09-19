#include<iostream>

using namespace std;

class animal{
	public:
		virtual void andar()=0;
};

class oviparos:public virtual animal{
	public:
		void andar(){
			cout<<"Quac quac";
		}
		virtual void botarOvo(){
			cout<<"Ploc";
		}
};

class mamiferos:public virtual animal{
	public:
		virtual void andar(){
			cout<<"MOOOOOOO";
		}
		virtual void leitada(){
			cout<<"ploft";
		}
};

class ornitorrinco:public mamiferos,oviparos{
	public:
		void andar(){
			cout<<"pleft pleft";
		}
		void botarOvo(){
			cout<<"PLOU";
		}
		void leitada(){
			cout<<"PLIIIIISSH";
		}
};

int main(){
	animal *a;
	ornitorrinco perry;
	a=&perry;
	a->andar();
}
