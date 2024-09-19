#include<vector>
#include<iostream>

using namespace std;

class solidos{
	public:
		virtual float calculoAreaSuperficie()=0;
		virtual float calculoVolume()=0;
};

class paralelepiped:public solidos{
	float x,y,z;
	public:
		paralelepiped(float _x=0, float _y=0, float _z=0): x(_x), y(_y), z(_z) 
		{}

		float calculoAreaSuperficie(){
			return x*y;
		}
		
		float calculoVolume(){
			return x*y*z;
		}
};

class sphere:public solidos{
	float radius;
	public:
		sphere(float _radius=0): radius(_radius)
		{}
		float calculoAreaSuperficie(){
			return 4*3.1412592*radius*radius;
		}
		float calculoVolume(){
			return (4.0/3)*3.1412592*radius*radius*radius;
		}
};

class objetoAbstrato{
	vector<solidos*> listaDeObjetos;
	float volume, area;
	public:
		objetoAbstrato(){
			listaDeObjetos.clear();
			volume=0;
			area=0;
		}
		void addObject(solidos* objeto){
			listaDeObjetos.push_back(objeto);
		}
		float calcularVolume(){
			vector<solidos*>::iterator it;
			for(it=listaDeObjetos.begin();it<listaDeObjetos.end();++it){
				volume=(*it)->calculoVolume();
			}
			return volume;
		}
		float calcularArea(){
			vector<solidos*>::iterator it;
			for(it=listaDeObjetos.begin();it<listaDeObjetos.end();++it){
				area=(*it)->calculoAreaSuperficie();
			}
			return area;
		}
};

int main(){
	sphere s1(5);
	paralelepiped p1(1,2,3);
	sphere p2(10);
	objetoAbstrato abs1;

	abs1.addObject(&p1);
	abs1.addObject(&s1);
	abs1.addObject(&p2);

	cout<< "Area: " <<abs1.calcularArea()<<"\n";
	cout<< "Volume: " <<abs1.calcularVolume()<<"\n";


	return 1;
}
