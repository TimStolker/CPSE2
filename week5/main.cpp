#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <array>

#include "objects.hpp"

//============================================================================

class action {
private:
	std::function< bool() > condition;
	std::function< void() > work;
public:
	action(
	   std::function< bool() > condition, 
	   std::function< void() > work
	) : condition( condition ), 
		work( work ) 
	{}

	action(
		sf::Mouse::Button button,
		std::function< void() > work
	) :
		condition(
			[ button ]()->bool { return sf::Mouse::isButtonPressed( button ); }
		),
		work(work)
	{}

	void operator()(){
		if( condition() ){
			work();
		}
	}
};


//============================================================================

screen_object * selectObject( sf::Vector2i mousePosition, screen_object * object ){
    auto floatPosition = static_cast<sf::Vector2f>(mousePosition);
    if(object->collision( floatPosition )){
        return object;
    }
    return 0;
}

void move(sf::Vector2i target, screen_object * selectedObject){
    auto floatPosition = static_cast<sf::Vector2f>(target);
    selectedObject->jump(floatPosition);
}

int main(){
    std::string mode = "select";  //0 = select, 1 = delete
    sf::RenderWindow window{ sf::VideoMode{ 640, 480 },"SFML window" };
    screen_object * ObjectList[100];
    screen_object * chooseObjectList[10];
    screen_object * selectedObject;
    screen_object * chooseSelectedObject;
    screen_object * tempObject;
    sf::Color selectColor = sf::Color(255,0,0); //default kleur
    
    int size = 0;
    int chooseSize = 9;

    chooseObjectList[0] = new rectangleObject(sf::Vector2f{ 550.0, 10.0 }, sf::Vector2f{20.0,20.0}, sf::Color(255,255,255));
    chooseObjectList[1] = new circleObject(sf::Vector2f{ 550.0, 30.0 }, 10, sf::Color(255,255,255));

    chooseObjectList[2] = new circleObject(sf::Vector2f{ 575.0, 10.0 }, 10, sf::Color(255,0,0));
    chooseObjectList[3] = new circleObject(sf::Vector2f{ 575.0, 30.0 }, 10, sf::Color(0,255,0));
    chooseObjectList[4] = new circleObject(sf::Vector2f{ 575.0, 50.0 }, 10, sf::Color(0,0,255));
    chooseObjectList[5] = new circleObject(sf::Vector2f{ 575.0, 70.0 }, 10, sf::Color(255,0,255));
    chooseObjectList[6] = new circleObject(sf::Vector2f{ 575.0, 90.0 }, 10, sf::Color(255,255,0));
    chooseObjectList[7] = new pictureObject(sf::Vector2f{ 550.0, 50.0 }, "select.png" );
    chooseObjectList[8] = new pictureObject(sf::Vector2f{ 550.0, 70.0 }, "delete.png" );
    
    action actions[] = {
        action( sf::Mouse::Left,  [&](){
            for(int i=0; i < size; i++){
                tempObject = selectObject( sf::Mouse::getPosition( window ), ObjectList[i] );
                if(!(tempObject==0)){
                    selectedObject = tempObject;
                    if(mode == "delete"){ //delete
                        for(int j = i; j < size-1; j++){
                            ObjectList[j] = ObjectList[j + 1]; // copy element links
                        }
                    }
                    selectedObject->setColor(selectColor); 
                }
            }
        }),

        action( sf::Mouse::Right, [&](){ 
            if(mode == "select"){ //mode is select
                move(sf::Mouse::getPosition( window ), selectedObject);
            }
        })
    };

    action chooseActions[] = {
        action( sf::Mouse::Left,  [&](){
            for(int i = 0; i < 9; i++){
                tempObject = selectObject( sf::Mouse::getPosition( window ), chooseObjectList[i] );
                if(!(tempObject==0)){
                    chooseSelectedObject = tempObject;
                    if(i > 1 && i < 7){ // het is geen object maar een kleur
                        selectColor = chooseSelectedObject->getColor();
                        chooseObjectList[0]->setColor(selectColor);
                        chooseObjectList[1]->setColor(selectColor);
                    }
                    else if(i == 7){ //select 
                        mode = "select";
                    }
                    else if(i == 8){ //delete
                        mode = "delete";
                    }
                }
                
                if(i == 0){
                    ObjectList[size] = new rectangleObject(sf::Vector2f{ 550.0, 10.0 }, sf::Vector2f{20.0,20.0}, sf::Color(selectColor));
                    size++;
                }
                else if(i == 1){
                    ObjectList[size] = new circleObject(sf::Vector2f{ 550.0, 30.0 }, 10, sf::Color(selectColor));
                    size++;
                }
            }
        })
    };

    ObjectList[0] = new rectangleObject(sf::Vector2f{ 550.0, 10.0 }, sf::Vector2f{20.0,20.0}, sf::Color(255,0,0)); //zet iets in objectlist om uit te lezen
    selectedObject = ObjectList[0]; //zorgt ervoor dat er zoizo iets kan worden gepakt
    while(window.isOpen()){
        for( auto & action : actions ){
			action();
		}
        for( auto & action : chooseActions ){
			action();
		}
        
        window.clear();

        for(int i=0; i < size; i++){ //drawed constant
            ObjectList[i]->draw( window );
        }
        for(int i=0; i < chooseSize; i++){
            chooseObjectList[i]->draw( window );
        }

        window.display();

        sf::sleep( sf::milliseconds(30) );

        sf::Event event;		
        while( window.pollEvent(event) ){
            if( event.type == sf::Event::Closed ){
                window.close();
            }
        }
	}
}
