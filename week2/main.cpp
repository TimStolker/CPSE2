#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <array>

#include "exceptions.hpp"
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
std::ifstream & operator>>(std::ifstream & input, sf::Color & rhs){
    std::string s;
    input >> s;
    const struct{ const char * name; sf::Color color; } colors[]{
        { "blue", sf::Color::Blue},
        { "red" , sf::Color::Red },
        { "green" , sf::Color::Green }
    };
    for( auto const & color : colors){
        if( color.name == s ){
            rhs = color.color;
            return input;
        }
    }
    if ( s == ""){
        throw end_of_file();
    }
    throw unknown_color( s );
    
}

//============================================================================

std::ifstream & operator>>( std::ifstream & input, sf::Vector2f & rhs){  //input check, klopt de input?
    char c;
    if( !( input >> c )){ throw end_of_file();}

    try{
        if( c != '(' ){ throw invalid_position( c );}

        if( !( input >> rhs.x )){ throw invalid_position( rhs.x );} 

        if( !( input >> c )){ throw invalid_position( c );}

        if( !( input >> rhs.y )){ throw invalid_position( rhs.y );}

        if( !( input >> c )){ throw invalid_position( c );}
        if( c != ')'){ throw invalid_position( c );}
    }
    catch(const invalid_position & e){
        abort();
    }



    return input;
}

//============================================================================

screen_object * screen_object_read( std::ifstream & input ){
    sf::Vector2f position;
    std::string name;
    sf::Color color;

    input >> position >> name;

    if ( name == "CIRCLE"){
        int diameter;
        input >> color >> diameter;
        return new circleObject(position, "CIRCLE",diameter, color );
    }
    else if ( name == "RECTANGLE" ){
        sf::Vector2f secondPosition;
        input >> color >> secondPosition;
        return new rectangleObject(position,"RECTANGLE", secondPosition, color );
    }
    else if ( name == "PICTURE" ){
        std::string pictureName;
        input >> pictureName;
        return new pictureObject(position,"PICTURE", pictureName );
    }
    else if ( name == "" ){
        throw end_of_file();
    }	
    throw unknown_shape( name );
}

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

void writeFile( screen_object * object, std::ofstream & myfile){
    std::string objectName = object->getName();
    sf::Vector2f position = object->getCoords();
    if(objectName == "CIRCLE"){
        myfile << "(" << position.x << "," << position.y << ") " << object->getName() << " " << object->getColor() << " " << object->getRad() << "\n";
    }
    else if(objectName == "RECTANGLE"){
        myfile << "(" << position.x << "," << position.y << ") " << object->getName() << " " << object->getColor() << " (" << object->getSize().x << "," << object->getSize().y << ")\n";
    }
    else if(objectName == "PICTURE"){
        myfile << "(" << position.x << "," << position.y << ") " << object->getName() << " " << object->getPictureName() << "\n";
    }
}

int main(){
    sf::RenderWindow window{ sf::VideoMode{ 640, 480 },"SFML window" };
    screen_object * ObjectList[10];
    int size = 0;
    std::ifstream input( "objects.txt" );
    screen_object * selectedObject;
    screen_object * tempObject;
    bool exit = false;

    action actions[] = {
        action( sf::Mouse::Left,  [&](){
            for(int i=0; i < size; i++){
                tempObject = selectObject( sf::Mouse::getPosition( window ), ObjectList[i] );
                if(!(tempObject==0)){
                    selectedObject = tempObject;
                }
            }
        }),

        action( sf::Mouse::Right, [&](){ 
            move(sf::Mouse::getPosition( window ), selectedObject);
        })
    };

    try {
        for(;;){ 
            ObjectList[size] = screen_object_read( input );
            size++;
        }
    }
    
    catch( const end_of_file & ){}
    catch( std::exception & problem){
        std::cout << problem.what();
        sf::sleep(sf::milliseconds(1000));
        abort();
    }

    while(window.isOpen()){
        for( auto & action : actions ){
			action();
		}


        selectedObject = ObjectList[0];
        for(;;){

            for( auto & action : actions ){
                action();
            }
            window.clear();
            for(int i=0; i < size; i++){
                ObjectList[i]->draw( window );
            }
            window.display();

            sf::sleep( sf::milliseconds(30) );

            sf::Event event;		
	        while( window.pollEvent(event) ){
			    if( event.type == sf::Event::Closed ){
                    exit = true;
				    window.close();
			    }
		    }
            if(exit == true){
                break;
            }
            
        }

        	
	}
    std::ofstream myfile;
    myfile.open("objects.txt");
    for(int i=0; i < size; i++){
        writeFile(ObjectList[i], myfile);
    }
    
}
