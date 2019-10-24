#include <iostream>
#include <string>
#include <vector>
#include "LexicalAnalyzer.cpp"
#define SizeArrayOfTypes 6
using namespace std;


bool FindStringInArray(string str,string (&ArrayOfTypes)[SizeArrayOfTypes]){
	for (int i = 0; i < SizeArrayOfTypes; i++)
	{
		if(str == ArrayOfTypes[i])
		{
			return true;
		}
	}
	return false;
}

string TabPlus(string str, int tab){
	string end = "";
	for (int i = 0; i < tab; i++){
		end+=str;
	}
	return end;
}

string Parser(vector <string> v)
{
	string str = "";
	int checkInclude=0;
	int checkStrInclude=0;
	int checkUsing=0;
	int paramOfFunc = 0;
	int paramOfFuncIsArray = 0;
	int paramOfFuncIsVariable = 0;
	int tab = 0;
	int checkCout = 0;
	int checkReturn = 0;
	int itIsVariable = 0;
	int itIsCondition = 0;
	int itIsAction = 0;
	int checkFor = 0;
	string Action = "";
	vector <string> AF;
	vector <int> TabFor;
	int ecv = 0;
	int itIsArray = 0;
	int VariableClose = 0;
	int itIsStringTwo = 0;
	int itIsStringOne = 0;
	string ArrayOfTypes[] = {"string", "int", "char", "bool", "void","LexicalAnalyzer"};
	
	for (int i = 0; i < v.size(); i++)
	{
		if(itIsStringTwo == 0 && itIsStringOne == 0 ){
			if(itIsAction){
				if (v[i]!=")"){
					if((v[i+1] == "+" && v[i+2] == "+") || (v[i+1] == "-" && v[i+2] == "-")){
					AF.push_back(v[i]+v[i+1]+"=1");
					TabFor.push_back(tab);
					itIsAction = 0;
					checkFor = 0;
					i+=3;
					Action = "";
					continue;
					}
					Action += v[i];
				}
				else{
					AF.push_back(Action);
					TabFor.push_back(tab);
					checkFor = 0;
					itIsAction = 0;
					Action = "";
					continue;
				}
			}
			

			///////////////////////off includes
			else if (v[i] == "#" && v[i+1] == "define")
			{
				str += TabPlus("\t", tab) + v[i + 2] + " = " + v[i + 3]+"\n";
				i += 3;
			}

			else if ((v[i] == "#" && v[i+1] == "include")||checkInclude)
			{
				checkInclude = 1;
				if(v[i] == ">")
				{
					checkInclude = 0;
					continue;
				}
				if(v[i] == "\"")
				{
					checkStrInclude++;
					if(checkStrInclude >= 2)
					{
						checkStrInclude=0;
						checkInclude = 0;
					}
				}
				continue;
			}
			//////////////////////////////////

			///////////////////////off using
			else if((v[i] == "using")||checkUsing)
			{
				checkUsing = 1;
				if(v[i] == ";")
				{
					checkUsing = 0;
				}
				continue;
			}
			
			//////////////////////////////////
			

			////////////////////////////////////////////////////////////////////

			else if(v[i] == "true")
			{
				str += "True";
				continue;
			}

			////////////////////////////////////////////////////////////////////

			else if((v[i] == "&" && v[i+1] == "&") || (v[i] == "|" && v[i+1] == "|"))
			{
				if(v[i] == "&")
				{
					str += " and ";
				}
				else{
					str += " or ";
				}
				
				i++;
				continue;
			}

			////////////////////////////////////////////////////////////////////

			else if(v[i] == "&" || v[i] == "|" )
			{
				if(v[i] == "&")
				{
					str += " and ";
				}
				else{
					str += " or ";
				}
				continue;
			}

			////////////////////////////////////////////////////////////////////

			else if(v[i] == "else" && v[i+1] == "if")
			{
				str += TabPlus("\t", tab) + "elif ";
				i++;
				continue;
			}

			////////////////////////////////////////////////////////////////////

			else if(v[i] == "if")
			{
				str += TabPlus("\t", tab) + "if ";
				continue;
			}
			////////////////////////////////////////////////////////////////////

			else if(v[i] == "else")
			{
				str += TabPlus("\t", tab) + "else ";
				continue;
			}

			////////////////////////////////////////////////////////////////////

			

			////////////////////////////////////////////////////////////////////

			else if(v[i] == "for" || checkFor)
			{

				if (checkFor)
				{
					if(v[i] == ";")
					{
						
						if(itIsVariable || v[i-1] == "(")
						{
							itIsVariable = 0;
							itIsCondition =1;
							str+="\n"+ TabPlus("\t", tab) + "while ";
							continue;
						}

						else if(itIsCondition)
						{
							if(itIsCondition == 1)
							{
								str+="True";
							}
							itIsCondition = 0;
							itIsAction = 1;
							
							continue;
						}
					}
					else if(itIsVariable)
					{
						itIsVariable++;
						str += v[i];
						continue;
					}
					else if(itIsCondition)
					{
						itIsCondition++;
						str += v[i];
						continue;
					}
					
					else if(FindStringInArray(v[i], ArrayOfTypes))
					{
						itIsVariable = 1;
						str += TabPlus("\t", tab);
						continue;
					}
				}

				checkFor = 1;
				i++;
				continue;	
			}

			//////////////////////////////////////////////////////////////////
	if(FindStringInArray(v[i], ArrayOfTypes)||paramOfFunc)
			{
				
				if(paramOfFuncIsVariable)
				{

					if (v[i] == "="){
						ecv = 1;
						str += v[i];
						continue;
					}
					if (v[i] == ";")
					{	
						if (ecv == 0)
						{
							str += "= None";
						}
						str += "\n";
						paramOfFuncIsVariable = 0;
						paramOfFunc = 0;
						itIsArray = 0;
						ecv = 0;
						continue;
					}
					if (ecv && itIsArray)
					{
						if(v[i] == "{")
						{
							str += "[";
							continue;
						}
						else if(v[i] == "}")
						{
							str+="]";
							cout<<i+1<<endl;
							continue;
						}
					}

					str += v[i];
					continue;
				}
				else if(paramOfFunc)
				{
					if(v[i]==")")
					{
						str+=")";
						
						paramOfFunc = 0;
						paramOfFuncIsArray = 0;
						continue;
					}
					if(v[i+1] == "[" || paramOfFuncIsArray)
					{
						paramOfFuncIsArray = 1;
						if(v[i] == "]")
						{
							paramOfFuncIsArray = 0;
						}
						continue;
					}
					if(FindStringInArray(v[i], ArrayOfTypes))
					{
						continue;
					}
					str+=v[i];
					continue;
				}
				if(v[i+2] == "(" )
				{
					str+= TabPlus("\t", tab) + "def " + v[i+1] + "(";
					paramOfFunc = 1;
					i+=2;
					continue;
				}
				else if(v[i+5] == "(")
				{
					str+= TabPlus("\t", tab) + "def " + v[i+4] + "(";
					paramOfFunc = 1;
					i+=5;
					continue;
				}
				else if(v[i+2] == "[")
				{
					
					itIsArray = 1;
					paramOfFuncIsVariable = 1;
					paramOfFunc = 1;
					str += TabPlus("\t", tab) +v[i+1];
					if (v[i+4]=="]"){
						i+=4;
					}
					else if(v[i+3]=="]"){
						i+=3;
					}
					continue;
				}
				else
				{
					str+= TabPlus("\t", tab);
					paramOfFuncIsVariable = 1;
					paramOfFunc = 1;
					continue;
				}
			}
			
			//////////////////////////////////////////////////////////////////


			else if(v[i] == "{")
			{
				
				str += " :\n";
				tab++;
				continue;
			}
			/////////////////////

			else if(v[i] == "}")
			{
				tab--;
				if (TabFor.size() != 0)
				{
					if (tab == TabFor[TabFor.size()-1])
					{
						str +=  TabPlus("\t", tab+1) + AF[AF.size()-1] + "\n";
						AF.pop_back();
						TabFor.pop_back();
					}
				}
				continue;
			}

			//////////////////////
			else if(v[i] == "cout" || checkCout)
			{
				if(checkCout && v[i] != ";")
				{
					if (v[i] == "<")
					{
						str += "+";
						i+=2;
					}

					if (v[i] == "endl")
					{
						str += "'\\n'";
						continue;
					}
					str += v[i];
					continue;
				}
				else if(v[i] == ";")
				{
						str += ")\n";
						checkCout = 0;
						continue;
				}
				str += TabPlus("\t", tab) + "print(";
				checkCout = 1;
				i+=2;
				continue;
			}

			//////////////////////////////////////////////////////////////////

			else if(v[i] == "return" || checkReturn)
			{
				if(v[i] == ";")
				{	
					str += "\n";
					checkReturn = 0;
					continue;
				}
				else if (v[i] != ";" && checkReturn)
				{
					str += v[i];
					continue;
				}
				checkReturn = 1;
				str += TabPlus("\t", tab) + v[i] + " ";
				continue;
			}
			else if (v[i] == "\""){
				itIsStringTwo = 1;
				str+=v[i];
				continue;
			}
			else if (v[i] == "'"){
				itIsStringOne = 1;
				str+=v[i];
				continue;
			}
			
			if (v[i]==";")
				{
				str+="\n";
				continue;
				}
			if(str[str.size()-1] == '\n'){
					str +=TabPlus("\t", tab);
				}

			if((v[i+1] == "+" && v[i+2] == "+") || (v[i+1] == "-" && v[i+2] == "-")){
				str +=v[i]+v[i+1]+"=1";
				i+=2;
				continue;
			}
				str +=v[i];
				continue;
				
			}
			
			else{
				if(itIsStringTwo){
					if(v[i] == "\"" && v[i-1] != "\\"){
						itIsStringTwo = 0;
					}
				}
				else{
					if(v[i] == "\'" && v[i-1] != "\\"){
						itIsStringOne = 0;
					}
				}
			
			str+=v[i];
			continue;
			}
		
	}
		
		//////////////////////////////////////////////////////////////////

		
		
		//////////////////////////////////////////////////////////////////
		str+="\n";
		str+="main()";
		return str;

}

int main(){

	LexicalAnalyzer LA;
	vector <string> v = LA.foreman("Parser.cpp");
	// for(int i = 0; i < v.size();i++){
	// 	cout<<v[i]<<endl;
	// }
	cout<<Parser(v)<<endl;
	return 1;
}