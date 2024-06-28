#include <iostream>

class meters{
	float size;
	public:
	
		meters(float size): size(0)
		{
			this->size=size;
		}
		meters(const miles miles){

		}
		meters operator =(meters other){
			size = other.size;
			return (meters(size));
		}
		meters operator =(float other){
			size = other;
			return (meters(size));
		}

		float getSize(){
			return size;
		}

		meters operator +(float other) const{
			return meters(size+other);
		}
		meters operator +(meters other) const{
			return meters(size+other.size);
		}
		meters operator -(float other) const{
			return meters(size-other);
		}
		meters operator -(meters other) const{
			return meters(size-other.size);
		}
		meters operator ++(){
			return meters(size-1);
		}
		meters operator ++(int){
			size++;
			return meters(size--);
		}
		bool operator ==(meters other){
			return (size==other.size);
		}
		bool operator < (meters other){
			return (size<other.size);
		}
		bool operator > (meters other){
			return (size>other.size);
		}
		bool operator <= (meters other){
			return !(*this>other);
		}
		bool operator >=(meters other){
			return !(*this<other);
		}

};

class miles{
	float size;
	public:
		miles(float size): size(0)
		{
			this->size = size;
		}
		miles(meters meters): size(0)
		{
			this->size = meters.getSize()/1609.4;
		}

		meters convertToMeters(){
			return meters(size*1609.4);
		}

		float getSize(){
			return size;
		}

		miles operator =(meters other){
			return miles(other.getSize()/1609.4);
		}
		
};	

int main(){
	meters mt1(2), mt2(4);
	mt1++;
	mt1 = mt1 + mt2;
	mt2 = ++mt1;

}


