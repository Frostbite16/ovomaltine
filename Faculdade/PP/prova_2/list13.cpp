#include<iostream>
#define PI 3.1412592

class FormaGeometrica{

	public:
		FormaGeometrica()
		{}
		virtual float calcularArea() = 0;
};

class FormaRetangulo:public FormaGeometrica{
	float lado1, lado2;
	public:
		FormaRetangulo(float l1=0, float l2=0): lado1(l1), lado2(l2)
		{}
		float calcularArea(){
			return lado1*lado2;
		}
};

class FormaCirculo:public FormaGeometrica{
	float raio;
	public:
		FormaCirculo(float r=0): raio(r)
		{}
		float calcularArea(){
			return raio*raio*PI;
		}
};

class FormaTriangulo:public FormaGeometrica{
	float base, altura;
	public:
		FormaTriangulo(float base, float altura): base(base), altura(altura)
		{}
		float calcularArea(){
			return altura*base/2;
		}
};

class objetoAbstrato:FormaGeometrica{
	float area;
	public:
		objetoAbstrato()
		{	
			area=0;
		}
		void areaTotal(FormaGeometrica* obj){
			area += obj->calcularArea(); 
		}

		float calcularArea(){
			return area;
		}
};

int main(){
	FormaCirculo c(1);
	FormaRetangulo r(1,1);
	FormaTriangulo t(45,10);
	objetoAbstrato obj;
	obj.areaTotal(&r);
	obj.areaTotal(&c);
	obj.areaTotal(&t);
	std::cout<<obj.calcularArea()<<"\n";
	
}
