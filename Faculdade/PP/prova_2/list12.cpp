#define PI 3.1412592

class FormaGeometrica{

	public:
		FormaGeometrica();
		virtual float calcularArea() = 0;
};

class FormaRetangulo:FormaGeometrica{
	float lado1, lado2;
	public:
		FormaRetangulo(float l1=0, float l2=0): lado1(l1), lado2(l2)
		{}
		float calcularArea(){
			return lado1*lado2;
		}
};

class FormaCirculo:FormaGeometrica{
	float raio;
	public:
		FormaCirculo(float r=0): raio(r)
		{}
		float calcularArea(){
			return raio*raio*PI;
		}
};

int main(){
	FormaCirculo c(1);
	FormaRetangulo r(1,1);
	
}
