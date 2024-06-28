// Implementar operador <
// Implementar operador >
// Implementar operador /
// Implementar operador <=
// Implementar operador >=
// Implementar operador - (Unário) Inverte o sinal
// Implementar operador - (binario)
// Implementar operador -- (Pos); feito
// Implementar operador -- (Pre); feito


#include <iostream>

class frac{
    int num,den;

    public:
        frac(const int num=0, const int den=1): num(num), den(den)
        {}

        int getNum(){
            return num;
        }

        int getDen(){
            return den;
        }

        frac operator =(const frac other){
            den = other.den;
            num = other.num;
            return frac(den,num);
        }

        frac operator *(const frac other) const{
            return frac(num*other.num, den*other.den);
        }

        frac operator +(const frac other) const{
            return frac(num*other.den + other.num + den, den*other.den);
        }

        frac operator --(){
            num-=den;
            return frac(num,den);
        }

        frac operator ++(){
            num+=den;
            return frac(num,den);
        }

        frac operator ++(int){
            num+=den;
            return frac(num-den,den);
        }

        bool operator ==(const frac other) const{
            return num*other.den == den*other.num;
        }

        bool operator !=(const frac other) const{
            return num*other.den != den*other.num;
        }

        bool operator <(const frac other) const{
            return num*other.den < den*other.num;
        }

        frac operator --(){
            num-=den;
            return frac(num,den);
        }

        frac operator --(int){
            num-=den;
            return frac(num+den,den);
        }
        
                           

};

int main(){

    frac f1(1,3), f2(3,-1);
    std::cout << f1.getNum() << " " << f1.getDen() << "\n";
    std::cout << (f1<f2) << "\n";



}

