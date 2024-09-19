#include<string>
#include<vector>

using namespace std;

class genericPerson{ // Personagem generico
	float health, mana, attack, shield;
	string name;
	unsigned mapPositionX, mapPositionY, level;
	vector<unsigned short> inventory;
	protected:	
		void addInventory(){
			// Adiciona item ao inventario
		}
		virtual void setName(string name){
			this->name.assign(name);
		}
		void newLevel(float h, float m, float a, float s){
			health += (50.0/level)*h;
			mana += (50.0/level)*m;
			attack += (50.0/level)*a;
			shield += (50.0/level)*s;
			level++;
		}
	public:
		genericPerson(float h=0, float m=0, float a=0, float s=0, float x=0, float y=0, unsigned l=0): health(h), mana(m), attack(a), shield(s), mapPositionX(x), mapPositionY(y), level(l)
		{}

		unsigned getLevel(){
			return level;
		}
		
};

class character: genericPerson{
	float levelCap, levelProgress;
	float healthMeter, manaMeter, attackMeter, shieldMeter;

	protected:
		void checkLevelCap(){
			if(levelProgress>levelCap){
				newLevel(healthMeter, manaMeter, attackMeter, shieldMeter);
				levelCap += levelCap*getLevel();
				levelProgress = 0;
			}
				
		}
		void upLevelProgress(float quantity){
			levelProgress+=quantity;
			checkLevelCap();
		}
	public:
		character(float h=1, float m=1, float a=1, float s=1): healthMeter(h), manaMeter(m), attackMeter(a), shieldMeter(s)
		{
			genericPerson(100*healthMeter, 50*manaMeter, 50*attackMeter, 25*shieldMeter, 0, 0);
		}
		void setName(string name){
			genericPerson::setName(name);
		}
};

class genericEnemy:public genericPerson{
		
};

class warrior:character{
	public:
		warrior(){
			character(1.2, 0.2, 1.3, 1.3);
		}
};

class priest:character{
	public:
		priest(){
			character(0.7,1.5,0.8,0.2);
		}
};







