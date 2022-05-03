#ifndef _OBJECTS_HPP
#define _OBJECTS_HPP

//================================================================

class screen_object{
protected:
    sf::Vector2f position;

public:
    screen_object( sf::Vector2f position):
    position( position )
    {}
    virtual sf::Vector2f getCoords(){ return position; }
    virtual sf::Color getColor() {return sf::Color(255,0,0);}
    virtual void setColor(sf::Color color) = 0;
    virtual void draw( sf::RenderWindow & window ) = 0;
    virtual void jump( sf::Vector2f target ) = 0;
    virtual bool collision( sf::Vector2f mousePosition) = 0;
    

    
};

//================================================================

class circleObject : public screen_object {
protected:
    sf::CircleShape circle;
	float size;
    sf::Color color;
public:
	circleObject( sf::Vector2f position,float size, sf::Color color):
    	screen_object(position), size(size), color(color)
	{}

	void draw( sf::RenderWindow & window ){
		
		circle.setRadius(size);
		circle.setPosition(position);
		circle.setFillColor(color);
		window.draw(circle);
	}
    void jump( sf::Vector2f target ){
        position = target;
    }

    bool collision( sf::Vector2f MousePosition ){
        return circle.getGlobalBounds().contains(MousePosition);
    }

    sf::Color getColor(){
        return color;
    }
    void setColor(sf::Color newColor){
        color = newColor;
    }
};

//================================================================

class rectangleObject : public screen_object {
protected:
	sf::Vector2f size;
    sf::Color color;
	sf::RectangleShape rectangle;
public:
    rectangleObject( sf::Vector2f position, sf::Vector2f size, sf::Color color):
		screen_object(position), size(size), color(color)
	{}

	void draw( sf::RenderWindow & window ){
		rectangle.setSize(size);
		rectangle.setPosition(position);
		rectangle.setFillColor(color);
		window.draw(rectangle);
	}

    void jump( sf::Vector2f target ){
        position = target;
    }

    bool collision( sf::Vector2f MousePosition ){
        return rectangle.getGlobalBounds().contains(MousePosition);
    }

    sf::Color getColor(){
        return color;
    }

    void setColor(sf::Color newColor){
        color = newColor;
    }
};

//============================================================================

class pictureObject : public screen_object {
protected:
	sf::Vector2f size;
	std::string pictureName;
    sf::Texture pictureTexture;
    sf::RectangleShape picture{sf::Vector2f(20.0f, 20.0f)};
    
public:
    pictureObject( sf::Vector2f position, std::string pictureName):
		screen_object(position),pictureName(pictureName)
	{}

	void draw( sf::RenderWindow & window ){ 
        picture.setPosition(position);
		pictureTexture.loadFromFile(pictureName);
        picture.setTexture(&pictureTexture);
		window.draw(picture);
    }

    void jump( sf::Vector2f target ){
	    position = target;
    }

    bool collision( sf::Vector2f MousePosition ){
        return picture.getGlobalBounds().contains(MousePosition);
    }

    void setColor( sf::Color newColor ){ return; }
};

//============================================================================

class objectArray{
protected:
    screen_object * objects[100];
    int i = 0;

public:
    void add(screen_object * object){
        std::cout<< "add \n";
        objects[i]=object;
        i++;
    }
};

//================================================================

#endif