#ifndef _OBJECTS_HPP
#define _OBJECTS_HPP

//================================================================

class screen_object{
protected:

    sf::Vector2f position;
    std::string name; 

public:
    screen_object( sf::Vector2f position, std::string name):
    position( position ),
    name( name )
    {}

    virtual float getRad() {return 0;}
    virtual std::string getColor() {return 0;}
    virtual std::string getPictureName() {return 0;}
    virtual sf::Vector2f getSize() {return {};}
    virtual sf::Vector2f getCoords(){ return position; }
    virtual std::string getName(){ return name; }

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
	circleObject( sf::Vector2f position, std::string name,float size, sf::Color color):
    	screen_object(position,name), size(size), color(color)
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

    std::string getColor(){ 
        std::string objectColor;
        if(color == sf::Color::Blue){
            objectColor = "blue";
        }
        else if(color == sf::Color::Red){
            objectColor = "red";
        }
        else if(color == sf::Color::Green){
            objectColor = "green";
        }
        return objectColor;
    }

    float getRad(){ return size; }

    
};

//================================================================

class rectangleObject : public screen_object {
protected:
	sf::Vector2f size;
    sf::Color color;
	sf::RectangleShape rectangle;
public:
    rectangleObject( sf::Vector2f position, std::string name, sf::Vector2f size, sf::Color color):
		screen_object(position,name), size(size), color(color)
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

    std::string getColor(){ 
        std::string objectColor;
        if(color == sf::Color::Blue){
            objectColor = "blue";
        }
        else if(color == sf::Color::Red){
            objectColor = "red";
        }
        else if(color == sf::Color::Green){
            objectColor = "green";
        }
        return objectColor; 
    }
    sf::Vector2f getSize(){ return size; }
};

//============================================================================

class pictureObject : public screen_object {
protected:
	sf::Vector2f size;
	std::string pictureName;
    sf::Texture pictureTexture;
    sf::RectangleShape picture{sf::Vector2f(100.0f, 100.0f)};
    
public:
    pictureObject( sf::Vector2f position, std::string name, std::string pictureName):
		screen_object(position,name),pictureName(pictureName)
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

    std::string getPictureName(){ return pictureName; }

};

//============================================================================

class objectArray{
protected:
    screen_object * objects[10];
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
