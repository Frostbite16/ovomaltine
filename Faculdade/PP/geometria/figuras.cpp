#include<iostream>
#include<vector>
using namespace std;

class Dgeometria{
public:
		virtual float calcArea() = 0;
		virtual ~Dgeometria(){};
};

class DQuadrado : public Dgeometria {
	float lado;
	public:
	DQuadrado(float lado=1.0): lado(lado)
	{}
	virtual float calcArea(){
		return lado*lado;
	}
	virtual ~DQuadrado(){}
};

class DRetangulo : public Dgeometria{
	float l1,l2;
	public:
	DRetangulo(float l1 = 1, float l2 = 1): l1(l1), l2(l2)
	{}
	virtual float calcArea(){
		return l1*l2;
	}
	virtual ~DRetangulo(){}
};

class Dcalc{
	vector<Dgeometria*> geos;
public:
	void insert(Dgeometria* pGeo){
		geos.push_back(pGeo);
	}
	float areaTotal(){
		vector<Dgeometria*>::iterator it;
		float area = 0.0;
		for(it=geos.begin();it!=geos.end();++it){
			area += (*it)->calcArea();
		}
		return area;
	}

	~Dcalc(){
		vector<Dgeometria*>::iterator it;
		for(it=geos.begin(); it!=geos.end();++it)
			delete *it;
	}	
};


int main(){





}

