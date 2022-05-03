#ifndef _TEKSTTTT_HPP
#define _TEKSTTTT_HPP

#include <iostream>
#include <string>
#include "game.hpp"

//==========================================================================================

class TekstGame{
private:
    char player = 'O';
    bool done = false;
    int number;
    char undoChar;
    int turnNum = 0;

public:
    TekstGame(){}

    void run(){

        std::cout<< "Tic-Tac-Toe" << std::endl;
        tictactoe game;

        while(true){
            if(player == 'X'){
                player = 'O';
            }
            else{
                player = 'X';
            }

            game.printBoard();
            std::cout<< "undo? (Y / N)" << std::endl;
            std::cin >> undoChar;

            while(true){
                if(undoChar == 'y' || undoChar == 'Y' ){
                    std::cout<< "hoever terug?" << std::endl;
                    int undoAmount = 0;
                    while(true){
                        std::cin >> undoAmount;
                        if(std::cin.fail()){
                            std::cout << "dat is geen nummer" << std::endl;
                            std::cin.clear();
                            std::cin.ignore(10, '\n');
                        }
                        if(undoAmount > 0 && undoAmount < turnNum){
                            game.undo(undoAmount);
                            if(undoAmount%2 != 0){
                                if(player == 'X'){
                                    player = 'O';
                                }
                                else{
                                    player = 'X';
                                }
                            }

                            game.printBoard();
                            break;
                        }
                    }
                }
                else if(undoChar == 'n' || undoChar == 'N'){
                    break;
                }
                else{
                    std::cout<< "voor het correcte character in" << std::endl;
                }
                std::cout<< "undo? (Y / N)" << std::endl;
                std::cin >> undoChar;
            }
            std::cout<< "op welke plek wil je een zet doen? (1-9)" << std::endl;
            number = 0;
            while(true){
                std::cin >> number;
                if(std::cin.fail()){
                    std::cout << "verkeerd nummer" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(10, '\n');
                }
                break;
            }

            std::cout << "\n";
            game.turn(number,player);
            done = game.checkWin(player,done);
            if(done){
                break;
            }
            done = game.checkDraw(done);
            if(done){
                break;
            }

            turnNum++;
        }

    }
};

#endif