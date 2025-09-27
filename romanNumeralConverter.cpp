#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

class Roman {
public:
    std::unordered_map<int,std::string> numerals;
    Roman(){
        setNumerals();
    }

    void setNumerals(){
        numerals.insert({1,"I"});
        numerals.insert({4,"IV"});
        numerals.insert({5,"V"});
        numerals.insert({9,"IX"});
        numerals.insert({10,"X"});
        numerals.insert({40,"XL"});
        numerals.insert({50,"L"});
        numerals.insert({90,"XC"});
        numerals.insert({100,"C"});
        numerals.insert({400,"CD"});
        numerals.insert({500,"D"});
        numerals.insert({900,"CM"});
        numerals.insert({1000,"M"});
    }
 
    int romanToInt(const std::string s){
        int num = 0;
        int i = 0;
        while(i < s.length()){
            int toAdd = 0;
            std::string symbol1 = "";
            std::string symbol2 = s.substr(i , 1);
            //Check possible pairs such as IV, IX , XL etc.
            if(i + 1 < s.length() && s[i] != s[i + 1]){
                symbol1 = s.substr(i , 2);
            }
            bool pairSymbolFound = false;
            for(const auto numeral : numerals){
                if(!symbol1.compare(numeral.second)){
                    toAdd = numeral.first;
                    pairSymbolFound = true;
                    break;
                }
                if(!symbol2.compare(numeral.second)){
                    toAdd = numeral.first;
                }
            }

            num += toAdd;
            if(pairSymbolFound){
                i += 2;
            } else {
                i++;
            }
        }
        return num;
    }

    std::string intToRoman(int number){
        //1 + log10 gives us how many digit number contains.
        std::vector<int> digits(1 + (int)log10(number));
        
        for(int i = digits.size() - 1;i >= 0;i--){
            digits[i] = number % 10;
            number /= 10;
        }

        std::string result = "";
        int power = digits.size() - 1;
        while(!digits.empty()){
            int key = digits[0];
            int product = (int)pow(10 , power);
            if(key != 4 && key != 5 && key != 9){
                if(key > 5){
                    result += numerals[product * 5];
                    key -= 5;
                }
                while(key > 0){
                    result += numerals[product];
                    key--;
                }
            } else {
                key *= product;
                result += numerals[key];
            }
            digits.erase(digits.begin() , digits.begin() + 1);
            power--;
        }
        return result;
    }

};

class File {
public:
    std::fstream file;
    File(std::string filename){
        file.open(filename , std::ios::in | std::ios::out | std::ios::trunc);
    }
    
    void writeOnFile(int num , const std::string literal , std::string filename){
        if(filename == "roman_to_int.txt"){
            file << literal << " : " << num << std::endl;
        } else if(filename == "int_to_roman.txt"){
            file << num << " : " << literal << std::endl;
        }
        file.flush();
    }

    void readOnFile(){
        file.seekg(0);
        std::string line;
        while(getline(file , line)){
            std::cout << line << std::endl;
        }
    }
    ~File(){
        if(file.is_open()){
            file.close();
        }
    }
};

int main(){
    Roman roman;
    std::string fileNames[2] = {"roman_to_int.txt" , "int_to_roman.txt"};
    File text_file(fileNames[0]);

    std::vector<std::string> romanNumerals;
    for(int i = 1;i <= 3999;i++){
        std::string romanNumeral = roman.intToRoman(i);
        romanNumerals.push_back(romanNumeral);
        text_file.writeOnFile(i,romanNumeral , fileNames[0]);
        romanNumeral.clear();
    }
    //text_file.readOnFile();
    File text_file2(fileNames[1]);

    while(!romanNumerals.empty()){
        int number = roman.romanToInt(romanNumerals[0]);
        text_file2.writeOnFile(number , romanNumerals[0] , fileNames[1]);
        romanNumerals.erase(romanNumerals.begin(), romanNumerals.begin() + 1);
    }
    //text_file2.readOnFile();
}