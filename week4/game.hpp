#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

class pictureObject {
protected:
    sf::Vector2f position;
	sf::Vector2f size;
	std::string pictureName;
    sf::Texture pictureTexture;
    sf::RectangleShape picture{sf::Vector2f(100.0f, 100.0f)};
    
public:
    pictureObject( sf::Vector2f position, std::string pictureName):
		position(position),pictureName(pictureName)
	{}
    pictureObject(){}

	void draw( sf::RenderWindow & window ){ 
        picture.setPosition(position);
		pictureTexture.loadFromFile(pictureName);
        picture.setTexture(&pictureTexture);
		window.draw(picture);
	}

    bool collision( sf::Vector2i MousePosition ){
        sf::Vector2f tmp {float(MousePosition.x) , float(MousePosition.y)};
        return picture.getGlobalBounds().contains(tmp);
    }

    std::string getPictureName(){ return pictureName; }
    sf::Vector2f getPosition(){return position; }

};

//=======================================================================

class tictactoe { 
private:
    char board[3][3];
    std::vector<std::pair<int,char>> moves = {};
    std::vector<pictureObject> boardList = {};

public:
    tictactoe(){}

    void makeBoard(sf::RenderWindow &window){
        auto square1 = pictureObject(sf::Vector2f(100.0    ,  0.0  ), "square.png"); //links boven
        auto square2 = pictureObject(sf::Vector2f(210.0    ,  0.0  ), "square.png");
        auto square3 = pictureObject(sf::Vector2f(320.0    ,  0.0  ), "square.png");
        auto square4 = pictureObject(sf::Vector2f(100.0    ,  110.0), "square.png");
        auto square5 = pictureObject(sf::Vector2f(210.0    ,  110.0), "square.png");
        auto square6 = pictureObject(sf::Vector2f(320.0    ,  110.0), "square.png");
        auto square7 = pictureObject(sf::Vector2f(100.0    ,  220.0), "square.png");
        auto square8 = pictureObject(sf::Vector2f(210.0    ,  220.0), "square.png");
        auto square9 = pictureObject(sf::Vector2f(320.0    ,  220.0), "square.png"); //rechts onder
        auto undoButton = pictureObject(sf::Vector2f(210.0 ,  330.0), "undo.png"  ); //undo knop
        boardList = {square1, square2, square3, square4, square5, square6, square7, square8, square9, undoButton};
        for(auto & image : boardList){
            image.draw(window);
        }
    }

    std::vector<pictureObject> getBoardList(){
        return boardList;
    }

    void printBoard(){
        bool placed = false;
        for(int i = 1; i < 10; i++){
            for( auto & move : moves){
                if(move.first == i){
                    std::cout<<  " | ";
                    std::cout<< move.second;
                    std::cout<< " | ";
                    placed = true;
                    break;
                }
            }
            if(!placed){
                std::cout<< " | - | ";
                
            }
            placed = false;
            
            if(i == 3 || i == 6 || i == 9){
                std::cout << "\n";
            }
        }
    }

    void turn(int number, char player){ //number = 1-9, player = X/O
        bool falseMove = false;
        if(moves.size() == 0){
            
            moves.push_back(std::pair<int,char>(number,player));
            return;
        }
        for(auto & move : moves){
            if(move.first == number){
                falseMove = true;
            }
        }
        if(falseMove){
            return;
        }
        else{
            moves.push_back(std::pair<int,char>(number,player));
        }
    }

    bool checkWin(char player, bool done){
        int column;
        int row;
        for(int i = 0; i<3;i++){
            for(int j = 0; j<3;j++){
                board[i][j] = '-';
            }
        }
        for(auto & move : moves){
            if(move.first<4){
                row = 0;
            }
            else if(move.first < 7){
                row = 1;
            }
            else{
                row = 2;
            }
            if(move.first == 3 || move.first == 6 || move.first == 9){
                column = 2;
            }
            else{
                column = move.first%3-1;
            }
            board[row][column] = move.second;
        }
        
        for(int i = 0; i<3;i++){ //check rijen
            if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '-'){
                done = true;
            }
        }
        for(int i = 0; i<3;i++){ //check kolommen
            if(board[0][i] == board[1][i] && board[1][i] == board[2][i]&& board[0][i] != '-'){ 
                done = true;
            }
        }

        if(board[0][0] == board[1][1] && board[1][1] == board[2][2]&& board[0][0] != '-'){
            done = true;
        }
        if(board[0][2] == board[1][1] && board[1][1] == board[2][0]&& board[0][2] != '-'){
            done = true;
        }

        if(done){
            std::cout<< "=========[]=========" << std::endl;
            std::cout<< " " << player << " heeft gewonnen" << std::endl; 
            std::cout<< "=========[]=========" << std::endl;
        }

        return done;
    }

    bool checkDraw(bool done){
        if(moves.size() == 9){
            std::cout<<"gelijkspel" << std::endl;
            return true;
        }
        else{
            return false;
        }
    }

    void undo(int undoAmount){
        for(int i = 0; i < undoAmount ; i++){
            if(moves.size() > 0 ){
                moves.pop_back();
            }
            
        }   
    }

    bool clicked(pictureObject &object, sf::RenderWindow &window){
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        if(object.collision(mouse)){
            return true;
        }
        return false;
    }
};

#endif
