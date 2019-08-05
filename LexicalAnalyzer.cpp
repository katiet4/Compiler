#include <iostream>
#include <string>
#include <vector>
#include <fstream>
int size(std::string str){//Узнать длину сроки(input:строка, output:длина строки).
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

		for(int i = 0; i<size(str); i++){ // Удаляет комментарии.
			if(str[i] == '/'){
				if(str[i-1] == '/'){
					break;
				}
				continue;
			}
			Line+=str[i];
		}

		allText += Line + " ";
		Line = "";
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
	for (int i = 0;i<size(str);i++){
		if ((str[i] == '+' || str[i] == '-' || str[i] == '(' 		//
			|| str[i] == ')' || str[i] == '*' || str[i] == '/'		//
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
		else if(str[i] == '"' && !CHARS &&
				(str[i-1] != '\\' || (str[i-1] == '\\' && str[i-2] == '\\'))){
			if (size(strip(safe)) == 0){
				safe += str[i];
				Return.push_back(strip(safe));
				safe = "";
			}
			else{
				if(STRING){
					Return.push_back(safe);
				}
				else{
					Return.push_back(strip(safe));
				}
				safe = "";
				safe += str[i];
				Return.push_back(strip(safe));
				safe = "";
			}
			STRING = !STRING;
			continue;
		}
		else if(str[i] == '\'' && !STRING  &&
				(str[i-1] != '\\' || (str[i-1] == '\\' && str[i-2] == '\\'))){
			if (size(strip(safe)) == 0){
				safe += str[i];
				Return.push_back(strip(safe));
				safe = "";
			}
			else{
				if(CHARS){
					Return.push_back(safe);
				}
				else{
					Return.push_back(strip(safe));
				}
				safe = "";
				safe += str[i];
				Return.push_back(strip(safe));
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

std::vector <std::string> foreman(std::string file){ //функция-прораб(input:имя файла, output:вектор строк из функции split).
	std::vector <std::string> str;
	str.push_back(ReadingFile(file));
	return split(str[0]);
}

int main(){
	std::vector <std::string> result = foreman("nameFile");// result - результат, nameFile - имя файла.
	return 1;
}