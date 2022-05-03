#ifndef _SFMLTTT_HPP
#define _SFMLTTT_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include "game.hpp"

//==========================================================================================

class SFMLGame{
private:
    char player = 'X';
    char winner;
    bool click = true;
    bool done = false;
    bool turnDone = false;
    int number = 0;
    bool flagDone = false;
    int boardTileIndex = 0;

public:
    SFMLGame(){}

    void run(){
        std::vector<pictureObject> imageList = {};
        sf::RenderWindow window{ sf::VideoMode{ 640, 480 },"SFML window" };
        tictactoe game;
        
        while(window.isOpen()){
            game.makeBoard(window);
            auto board = game.getBoardList();
            window.display();
            
            
            while(true){
                sf::sleep( sf::milliseconds( 20 ));
                sf::Event event;
                if(window.pollEvent(event)){
                    if(event.type == sf::Event::MouseButtonReleased){
                        for(auto & image : board){
                            if(image.getPictureName() == "undo.png"){
                                if(game.clicked(image, window)){ //undo button
                                    if(imageList.size() > 0){
                                        imageList.pop_back();
                                    }

                                    game.undo(1); 
                                    if(player == 'X'){
                                        player = 'O';
                                    }
                                    else{
                                        player = 'X';
                                    }
                                }
                            }
                            
                            else if(game.clicked(image, window)){
                                if(player == 'X'){  
                                    auto cross = pictureObject(image.getPosition(), "cross.png");  
                                    number = boardTileIndex + 1;  
                                    game.turn(number, player); //plek + player
                                    imageList.push_back(cross);
                                    player = 'O';
                                }
                                else if(player == 'O'){
                                    auto naught = pictureObject(image.getPosition(), "circle.png");
                                    number = boardTileIndex + 1;
                                    game.turn(number, player); //plek + player;
                                    imageList.push_back(naught);
                                    player = 'X';
                                }
                                turnDone = true;
                                click = true; //volgende zet
                            }
                            boardTileIndex++;


                                
                        }

                
                        if(turnDone){ //er is een valide zet gedaan
                            if(player == 'X'){
                                player = 'O';
                            }
                            else{
                                player = 'X';
                            }
                            done = game.checkWin(player,done);
                            if(player == 'X'){
                                player = 'O';
                            }
                            else{
                                player = 'X';
                            }
                            
                            if(done){
                                flagDone = true;
                                imageList.clear();
                                break;
                            }

                            done = game.checkDraw(done);
                            if(done){
                                flagDone = true;
                                imageList.clear();
                                break;
                            }
                            turnDone = false;
                        }
                        boardTileIndex = 0;
                        window.clear();
                        game.makeBoard(window);
                
                        for(auto & image : imageList){
                            image.draw(window);
                        }

                        window.display();
                    }
                }
            }
            if(flagDone){
                break;
            }
        }

    }

};

#endif