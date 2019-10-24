#include <iostream>//hell  asaaoman
#include <string>
#include <vector>
#include <fstream>
class LexicalAnalyzer
{
private:int size(std::string str){//Узнать длину сроки(input:строка, output:длина строки).
	int i = 0;
	while(true){
		if (str[i] == '\0'){
			return i;
		}
		i++;
	}
}

std::string ReadingFile(std::string file){//Считывает содержимое файла(input:имя файла, output:содержимое файла).
	std::ifstream fin(file);
	std::string str;
	std::string allText = "";
	int count = 0;
	std::string Line = "";
	while(!fin.eof()){
		getline(fin,str);
		allText += str + "\n";
	}
	return allText;
 }

std::string strip(std::string str){	//Удаляет пробелы, табуляции и переносы строки
							//(input:строка, output:строка без пробелов, табуляций и переносов).
	std::string withoutGap = "";
	for (int i = 0;i<size(str);i++){
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'){
			continue;
		}
		withoutGap+=str[i];
	}
	return withoutGap;
}

std::vector <std::string> split(std::string str){//разбивает строку (input:строка, output:вектор строк).
	std::vector <std::string> Return;
	std::vector <std::string> ReturnWithoutGap;
	std::string safe = "";
	bool STRING = false;
	bool CHARS = false;
	bool COMMENT = false;
	bool CHECK = false;
	for (int i = 0;i<size(str);i++){
		if (COMMENT){
			if (str[i] != '\n'){
				continue;
			}
			COMMENT = false;
			continue;
		}
		if (CHECK){
			if (str[i] == '/'){
				COMMENT = true;
			}
			CHECK=false;
			continue;
		}
		if ((str[i] == '+' || str[i] == '-' || str[i] == '(' 		//
			|| str[i] == ')' || str[i] == '*' 						//
			|| str[i] == '#' || str[i] == '<' || str[i] == '>'		//
			|| str[i] == '{' || str[i] == '}' || str[i] == ';'		// 
			|| str[i] == ',' || str[i] == '=' || str[i] == ' '   	// Настройка.
			|| str[i] == '\t' || str[i] == '\n' || str[i] == '!'	// Можешь добавить или убрать разделение,
			|| str[i] == '[' || str[i] == ']' || str[i] == '|'		// но может все сломаться )).
			|| str[i] == '&' || str[i] == '.' || str[i] == ':') && !STRING && !CHARS){
			if (size(strip(safe)) == 0){
				safe += str[i];
				Return.push_back(strip(safe));
				safe = "";
				continue;
			}
			Return.push_back(strip(safe));
			safe = "";
			safe += str[i];
			Return.push_back(safe);
			safe = "";
			continue;
		}
		else if(str[i] == '/' && !STRING && !CHARS){
			CHECK = true;
			continue;
		}
		else if(str[i] == '"' && !CHARS &&
				(str[i-1] != '\\' || (str[i-1] == '\\' && str[i-2] == '\\'))){
			if (size(strip(safe)) == 0){
				safe += str[i];
				Return.push_back(safe);
				safe = "";
			}
			else{
				if(STRING){
					Return.push_back(safe);
				}
				else{
					Return.push_back(safe);
				}
				safe = "";
				safe += str[i];
				Return.push_back(safe);
				safe = "";
			}
			STRING = !STRING;
			continue;
		}
		else if(str[i] == '\'' && !STRING  &&
				(str[i-1] != '\\' || (str[i-1] == '\\' && str[i-2] == '\\'))){
			if (size(strip(safe)) == 0){
				safe += str[i];
				Return.push_back(safe);
				safe = "";
			}
			else{
				if(CHARS){
					Return.push_back(safe);
				}
				else{
					Return.push_back(safe);
				}
				safe = "";
				safe += str[i];
				Return.push_back(safe);
				safe = "";
			}
			CHARS = !CHARS;
			continue;
		}
		safe+=str[i];

	}

	if (size(safe) != 0){
		Return.push_back(strip(safe));
	}

	for (std::string s:Return){
		if(s == " "){
			continue;
		}
		else if(s == ""){
			continue;
		}
		ReturnWithoutGap.push_back(s);
	} 

	if(STRING || CHARS){
		std::cout<<"Error with \' or \""; // Завершает программу если не закрыты кавычки.
		exit(0);					// Если убрать if, то будет возвращать вектор 
	}								// со строкой от последней открытой кавычки до конца файла.

	return ReturnWithoutGap;
}

std::vector <std::string> EndChecker(std::vector <std::string> v){
	std::vector <std::string> v2;
	int str = 0;
	for (int i = 0; i < v.size(); i++){
		if(str && v[i] != "\"" && v[i] != "\'"){
			if (v[i][v[i].size()-1] == '"' && v[i][v[i].size()-2] != '\\' || v[i][v[i].size()-1] == '\'' && v[i][v[i].size()-2] != '\\' ){
				std::string s = "";
				for(int j = 0; j<v[i].size()-1; j++){
					s+=v[i][j];
				}
				v2.push_back(s);
				s = v[i][v[i].size()-1];
				v2.push_back(s);
				continue;
			}
			v2.push_back(v[i]);
			continue;
		}
		else if (v[i] == "\n" || v[i] == "\t" || v[i] == " "){
			continue;
		}
		else if(v[i] == "\"" || v[i] == "\'"){
			if(str){
				str = 0;
			}
			else
				str = 1;
			v2.push_back(v[i]);
			continue;

		}
		v2.push_back(strip(v[i]));
	}
	return v2;
}
public:
	std::vector <std::string> foreman(std::string file){ //функция-прораб(input:имя файла, output:вектор строк из функции split).
	std::vector <std::string> str;
	str.push_back(ReadingFile(file));
	
	return EndChecker(split(str[0]));
}
	
};



