#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <map>

std::map<int, std::string> swapMap(std::map<std::string, int> &wordCount) { //swapt de types van een map (zodat int eerst komt voor sorteren)

    std::map<int, std::string> intMap;

    for(std::map<std::string, int>::const_iterator it = wordCount.begin(); it != wordCount.end(); ++it)
        intMap.insert(std::pair<int, std::string>(it -> second, it -> first));

    return intMap;
}

std::vector<char> readChar(std::ifstream & myfile){  //zet elke character in de vector
    myfile.open("text.txt");
    std::vector<char> chars;
    char ch;
    while(myfile.get(ch)){
        chars.push_back(ch);
    }
    myfile.close();
    return chars; 
}

int countLines(std::vector<char> chars){        //telt alle lijnen in de file door de '\0' te tellen

    int lines = std::count(chars.begin(), chars.end(), '\n'); //tel all \n characters
    return lines;
}

int countAlfChars(std::vector<char> chars){     //telt alle letters in de file  //Gebruik std::count_if()
    return std::count_if(chars.begin(), chars.end(), [](unsigned char c){return std::isalpha(c); } );
}

std::list< std::pair <char,int> > countLetters(std::vector<char> chars){    //telt welke letters het vaakst voorkomen
    std::list< std::pair <char,int> > list;
    char c = 'a';
    while(true){
        int counter = count(chars.begin(), chars.end(), c);
        list.push_back(std::make_pair(c,counter));
        if(c == 'z'){ 
            return list;
        }
        c++;
    }
}

void printLetterlist(std::pair <char,int> listPair){ //print de letterList (char, int)
    std::cout << listPair.first << " = " << listPair.second << std::endl; 
}

std::vector<std::string> readWord(std::ifstream & myfile){ //zet alle woorden in een vector
    std::vector<std::string> wordVector;
    myfile.open("text.txt");
    std::string word;
    while(myfile >> word){
        wordVector.push_back(word);
    }
    myfile.close();
    return wordVector;
}



void countWords(std::vector<std::string> & wordVector){ //telt hoevaak een woord voorkomt
    std::map<std::string, int> wordCount;
    
    for(auto &word : wordVector){
        wordCount[word] += 1;
    }
    std::map<int, std::string> tempMap = swapMap(wordCount);
    
    std::cout<< "De meest voorkomende woorden zijn: " << std::endl;
    for (std::map<int, std::string>::iterator it=std::next(tempMap.end(), -1) ; it!=std::next(tempMap.end(), -11); --it){ //print top 10 woorden met hoeveelheid
        std::cout << it->first << " = " << it->second << std::endl;
    }
}


int main(){
    std::ifstream myfile;
    std::vector<char> chars = readChar(myfile);
    std::cout << "\n" << "De file bevat " << chars.size() << " characters" << std::endl;
    int lines = countLines(chars);
    std::cout << "De file bevat " << lines << " lijnen" << std::endl;
    int alfChar = countAlfChars(chars);
    std::cout << "De file bevat " << alfChar << " alfabetische characters" << std::endl;

    for(char &c : chars){
        if(c >= 'A' && c <= 'Z'){c += 32;}
    }

    std::list< std::pair <char,int> > letterList = countLetters(chars);
    letterList.sort([](auto const& a, auto const & b){ 
        if(a.second > b.second){return true;} 
        if(a.second < b.second) {return false;}
        return a.first < b.first;
    });
    std::cout << "Hoeveelheid letters die voorkomen: \n";
    for_each (letterList.begin(), letterList.end(), printLetterlist);
    std::vector<std::string> wordVector = readWord(myfile);
    countWords(wordVector);
}
