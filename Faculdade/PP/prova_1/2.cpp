#include <iostream>

using namespace std;

class dPonto(){
    float x,y;

    public: 
        dPonto(float x=0, float y=0): x(x), y(y)
        {}
    
        float getX(){
            return x;
        }
        float getY(){
            return y;
        }
        bool operator==(const dPonto other){
            return (x==other.x)&&(y==other.y);
        }
        bool operator!=(const dPonto other){ 
            return (x!=other.x)||(y!=other.y);
        }
        dPonto operator+(const dPonto other){
            return dPonto(x+other.x, y+other.y);
        }
        dPonto operator-(const dPonto other){
            return dPonto(x+other.x, y+other.y);
        }
        dPonto operator--(){
            x--, y--;
            return dPonto(x,y);
        }
        dPonto operator--(int){
            x--, y--;
            return dPonto(x+1, y+1);
        }
        dPonto operator++(){
            x++, y++;
            return dPonto(x,y);
        }
        dPonto operator++(int){
            x++, y++;
            return dPonto(x+1, y+1);
        }

};


class Dpolar{
    float r;
    float phi;
    
    public:
        Dpolar(float r=0, floar phi=0): r(r), phi(phi)
        {}
        Dpolar(const Dponto &other){
            phi = atan2(other.getY(), other.getX);
            r = 
        
        }


}
