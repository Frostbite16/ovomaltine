#include<iostream>
#include<cmath>

class node{
	double data;
	node* lineProx;
	node* rowProx;
	node* lineStart;

	public:
		node(int data=0.0f){
			this->data = data;
			lineProx = nullptr;
			rowProx = nullptr;
			lineStart = nullptr;
		}

		void setLineProx(node* other){
			lineProx = other;
		}

		void setRowProx(node* other){
			rowProx = other;
		}
		
		void setLineStart(node* other){
			lineStart = other;
		}
		void setData(float data){
			this->data = data;
		}

		node* getLineProx() const{
			return lineProx;
		}
		node* getRowProx() const{
			return rowProx;
		}

		node* getLineStart() const{
			return lineStart;
		}
		float getData() const{
			return data;
		}

};


class matrix{
	
	protected:
		unsigned line=0, row=0;
		node* root = nullptr;
	
	public:
		matrix()
		{}
		int insertNewLine(double data, size_t rowChoice){
			node *newNode = new node(data), *current=root, *lineStart;
			if((rowChoice==1)&&(root==nullptr)){
				newNode->setLineStart(newNode);
				root = newNode;
				line++;
				row++;
				return 1;
			}
			else if(row>=rowChoice){

				for(int i=0; i<rowChoice-1; i++){
					current = current->getRowProx();
				}	
				lineStart = current;
				while(current->getLineProx()!=nullptr){
					current = current->getLineProx();
				}
				line++;
				current->setLineProx(newNode);
				newNode->setLineStart(lineStart);
				return 1;
				
			}
			return 0;

		}

		void insertNewRow(double data){
			node *newNode = new node(data), *current = root, *lineStart = current;
			if(root==nullptr){
				root = newNode;
				row++;
				line++;
			}
			else{
				for(int i=0; i<row-1; i++){
					current = current->getRowProx();
				}
				row++;
				current->setRowProx(newNode);
				newNode->setLineStart(lineStart);
			}
		}

		void createMatrix(size_t order){
			for(size_t i=0; i<order; i++){
				insertNewRow(0);
				for(size_t j=0; j<order; j++){
					insertNewLine(0,i+1);
				}
			}
		}

		void insertAt(unsigned rowChoice, unsigned lineChoice, double data){
			node* current = root;
			for(int i=0; i<rowChoice-1;i++){
				current=current->getRowProx();
			}
			for(int i=0; i<lineChoice-1;i++){
				current=current->getLineProx();
			}
			current->setData(data);
		}

		int multiplyLine(unsigned rowChoice, double number){
			node* current = root;
			if(root!=nullptr){
				if(row>=rowChoice){
					for(unsigned i=0; i<rowChoice-1; i++){
						current = current->getRowProx();
					}
					while(current->getLineProx()){
						current->setData(current->getData() * number);
						current = current->getLineProx();
					}
				
					return 1;
				}
			}
			return 0;
		}
		
		int subtractWithOtherWeightedLine(unsigned rowChoice, unsigned otherRow, double number){
			node *current = root, *other = root;
			if(root!=nullptr){
				if((row>=rowChoice)&&(row>=otherRow)){
					for(int i=0; i<rowChoice-1; i++){
						current=current->getRowProx();
					}	
					for(int i=0; i<otherRow-1; i++){
						other=other->getRowProx();
					}
					while((current->getLineProx())&&(other->getLineProx())){
						current->setData(current->getData() - number*(other->getData()));
						current = current->getLineProx();
						other = other->getLineProx();
					}
					return 1;
				}
			}
			return 0;
		}

		int switchLines(unsigned rowChoice, unsigned otherRow){
			node *current = root, *other = root;
			if(root!=nullptr){
				if((row>=rowChoice)&&(row>=otherRow)){
					multiplyLine(rowChoice, -1);
					subtractWithOtherWeightedLine(rowChoice, otherRow, -1);
					subtractWithOtherWeightedLine(otherRow, rowChoice, 1);
					subtractWithOtherWeightedLine(rowChoice, otherRow, -1);
					return 1;
				}
			}
			return 0;
		}
		node* returnRow(unsigned rowChoice){
			node* current=root;
			if(root){
				for(int i=0; i<rowChoice-1; i++){
					current=current->getRowProx();
				}
				return current;
			}
			return nullptr;
		}
		
		void desalocar(node* current){
			if(current!=nullptr){
				desalocar(current->getRowProx());
				desalocar(current->getLineProx());
				delete current;
			}
		}

		~matrix(){
			desalocar(root);	
		}

};

class gaussPivot : public matrix
{
	unsigned maxRow=0;
	double multiplicador;
	public:
		gaussPivot()
		{}
		

		

		void FindMax(unsigned lineChoice, unsigned supLim){
			node* current = root, *start;
			unsigned i, currentRow = supLim;
			if(root){
			
				for(i=supLim-1;i<row;i++){
					start = current;
					for(i = 0;(current->getLineProx())&&(i < lineChoice-1); i++){
						current = current->getLineProx();
					}
					if(fabs(current->getData())>maxRow)
						maxRow = currentRow;
					currentRow++;
					current=start;
					current = current->getRowProx();
				}
			}
		}

		void calculate(){
			unsigned supLim=1, i, j, lineChoice = 1;
			float mult;
			node *current = root, *auxPtr = root, *multAux1, *multAux2, *pivo;
			if(root){
				while((auxPtr->getLineProx())&&(auxPtr->getLineProx()->getLineProx())){
					FindMax(lineChoice, supLim);
					if(maxRow!=supLim)
						switchLines(supLim, maxRow);
					pivo = returnRow(supLim);
					multAux2=pivo;
					current = pivo->getRowProx();
					for(i=supLim; i<row; i++){
						multAux1 = current;
						for(j=supLim-1;j<lineChoice-1;j++){
							multAux1 = multAux1->getLineProx();
							multAux2 = multAux2->getLineProx();
						}
						subtractWithOtherWeightedLine(i+1, supLim, multAux1->getData()/multAux2->getData());
						current = current->getRowProx();
					}
					supLim++, lineChoice++;
					auxPtr = auxPtr->getLineProx();
					maxRow=0;
				}	
			}
		}

};

int main(){
	gaussPivot gs1;
	gs1.createMatrix(3);
	gs1.insertAt(1,1,0.25);
	gs1.insertAt(1,2, 0.30);
	gs1.insertAt(1,3, 0.12);
	gs1.insertAt(1,4, 0.795);
	gs1.insertAt(2,1, 0.12);
	gs1.insertAt(2,2, 0.18);
	gs1.insertAt(2,3,0.24);
	gs1.insertAt(2,4,0.6);
	gs1.insertAt(3,1, 0.24);
	gs1.insertAt(3,2, 0.13);
	gs1.insertAt(3,3, 0.22);
	gs1.insertAt(3,4,0.71);
	gs1.calculate();
	return 1;
}


