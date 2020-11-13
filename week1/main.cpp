#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "objects.hpp"

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
		sf::Keyboard::Key key,
		std::function< void() > work
	) :
		condition(
			[ key ]()->bool { return sf::Keyboard::isKeyPressed( key ); }
		),
		work(work)
	{}


	action(
		std::function< void() > work
	) :
		condition(
			[]()->bool { return true; }
		),
		work(work)
	{}

	action(
		ball & my_ball,
		wall & my_wall,
		std::function< void() > work
	) :
		condition(
			[& my_ball, & my_wall ]()->bool { return my_ball.getCircle().getGlobalBounds().intersects(my_wall.getRectangle().getGlobalBounds()); }	
		),
		work(work)
	{}

	void operator()(){
		if( condition() ){
			work();
		}
	}
};

sf::Vector2f overlap(sf::Vector2f & direction){
	if(direction == sf::Vector2f( +2.0, +2.0 )){
        direction = sf::Vector2f( -2.0, +2.0 );
    }
    else if(direction == sf::Vector2f( -2.0, +2.0 )){
        direction = sf::Vector2f( -2.0, -2.0 );
    }
    else if(direction == sf::Vector2f( -2.0, -2.0 )){
        direction = sf::Vector2f( +2.0, -2.0 );
    }
    else if(direction == sf::Vector2f( +2.0, -2.0 )){
        direction = sf::Vector2f( +2.0, +2.0 );
    }
    return direction;
}


int main( int argc, char *argv[] ){
	auto direction = sf::Vector2f(-2.0, +2.0);
	sf::RenderWindow window{ sf::VideoMode{ 640, 480 }, "SFML window" };
	ball my_ball{ sf::Vector2f{ 320.0, 240.0 }, sf::Color{200,200,255} };
	wall my_wall1{ sf::Vector2f{ 0.0, 0.0 }, sf::Color{0,0,255}, sf::Vector2f{ 640.0, 20.0 } };
	wall my_wall2{ sf::Vector2f{ 0.0, 0.0 }, sf::Color{0,0,255}, sf::Vector2f{ 20.0, 480.0 } };
	wall my_wall3{ sf::Vector2f{ 0.0, 460.0 }, sf::Color{0,0,255}, sf::Vector2f{ 640.0, 440.0 } };
	wall my_wall4{ sf::Vector2f{ 620.0, 0.0 }, sf::Color{0,0,255}, sf::Vector2f{ 640.0, 480.0 } };
	wall my_square{ sf::Vector2f{ 40.0, 40.0 }, sf::Color{0,200,255}, sf::Vector2f{ 50.0, 50.0 } };

	action actions[] = {
		action( sf::Keyboard::Left,  [&](){ my_square.move( sf::Vector2f( -5.0,  0.0 )); }),
		action( sf::Keyboard::Right, [&](){ my_square.move( sf::Vector2f( +5.0,  0.0 )); }),
		action( sf::Keyboard::Up,    [&](){ my_square.move( sf::Vector2f(  0.0, -5.0 )); }),
		action( sf::Keyboard::Down,  [&](){ my_square.move( sf::Vector2f(  0.0, +5.0 )); }),

		action( my_ball, my_wall1,   [&](){ overlap(direction); }) , 
		action( my_ball, my_wall2,   [&](){ overlap(direction); }) , 
		action( my_ball, my_wall3,   [&](){ overlap(direction); }) , 
		action( my_ball, my_wall4,   [&](){ overlap(direction); }) , 
		action( my_ball, my_square,  [&](){ overlap(direction); }) ,

		action( [&](){my_ball.move(direction); })
	};

	while (window.isOpen()) {
		for( auto & action : actions ){
			action();
		}

		window.clear();
		my_ball.draw( window );
		my_square.draw( window );
		my_wall1.draw( window );
		my_wall2.draw( window );
		my_wall3.draw( window );
		my_wall4.draw( window );
		window.display();
		sf::sleep( sf::milliseconds( 20 ));

        sf::Event event;		
	    while( window.pollEvent(event) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}
		}	
	}

	std::cout << "Terminating application\n";
	return 0;
}

