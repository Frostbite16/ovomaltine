Trabalho de CG, lista de padronização. Para o envio do arquivo,apenas o que está descrito a baixo, não precisa enviar todo o arquivo canva.cpp 

Lista de padronizações:

-> Criar uma Define com o nome da sua primitiva valor que ele assume:
	Questões de 1 a 10 ( #define  PRIMITIVA numero_questão+3)
	Questões de 13 até o fim ( #define PRIMITIVA numero_questão+1)
	

-> Se não existir ou caso as classes atuais não suportem a primitiva na qual está tentando implementar
crie uma nova classe para a mesma.


-> Adcionar Vector global com suas primitivas:
	Vector<Primitiva>;
	Vector<Primitiva>::interator;
	
-> Implementar Drawn com a suas primitivas:
	void DrawnPrimitiva();
	// SUBSTITUIR PRIMITIVA PELO NOME DA SUA PRIMITIVA

-> Implementar Drawn Primitiva Selection Box:
	void DrawnPrimitivaSelectionBox();
	// SUBSTITUIR PRIMITIVA PELO NOME DA SUA PRIMITIVA


-> Em geral siga o canva e faça as alterações necessárias , qualquer duvida entre em contado com José Gabriel ou João Vitor.

OQUE FAZER: Adicione a opção de uma reta 
perpendicular. Crie um ícone adequado. O clique 
esquerdo do mouse no ícone deverá habilitar 
esta opção. O clique subsequente determina um 
ponto da reta e o próximo clique deverá ser em 
cima de um vetor ou de uma reta perpendicular.
