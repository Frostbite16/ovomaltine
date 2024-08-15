#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

int main(){
	sf::RenderWindow window(sf::VideoMode(800,600), "teste");
	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
		}

	}



}
