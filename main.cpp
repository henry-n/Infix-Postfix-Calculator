#include "Calculator.h"


main(){
	
   int choice;
   std::list <std::string> expressionList;


   do{
        choice = menu();

        if (choice == 1){
            expressionList = getInput();
			
			std::cout << '\n';
			std::cout << "\nPostfix Expression: ";
			for (auto it = expressionList.begin(); it != expressionList.end(); ++it)
				std::cout << "" << *it;
			std::cout << "\n\n";
			
        }

        else if (choice == 2){

			if(expressionList.empty()){
				std::cout << "\n**Please Enter An Infix Expression First.**\n\n";
			}
			else
				postfixEvaluate(expressionList);
        }


   }while(choice != 3);	
   

}

//---------------------------------------------------------------------------------------------------------------------------------------

int menu(){

    std::cout << "(1)Convert Infix to Postfix\n";
    std::cout << "(2)Evaluate Expression\n";
    std::cout << "(3)Exit\n";

    std::string choice;
    std::cout << "Enter Choice: "; 
	std::getline(std::cin, choice);
    int filtered = atoi(choice.c_str());

    //check and filter input
	if(choice.length() > 1 )
		return menu();
    if (filtered >= 1 && filtered <= 3 )
        return filtered;
    else
        return menu();//recursion loop menu


}

std::list <std::string> getInput(){

    std::string input;

    std::cout << "Enter Infix Expression: ";
    std::getline(std::cin, input);

    if(checkExpression(input))
    {
		return(convertInfix(input));
    }
    else
    {
        return getInput();//recursion loop function again if incorrect
    }

}

bool checkExpression(std::string input){

    //check for illegal characters
   std::size_t found = input.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-/*#()1234567890 ");

   //check if # is at end of expression
   std::size_t foundEnd = input.find_last_of("#");

   if(found != std::string::npos)
   {
       std::cout <<"Invalid Characters In Expression [Numerics] [A-Z][a-z] [*,/,+,-,(,)] Only \n";
       return false;
   }
    else if(foundEnd ==std::string::npos )
    {
        std::cout <<"Need '#' at end of expression\n";
        return false;
    }
    else
        return true;


}

//CONVERT INFIX FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------------------

void transferStack1(std::stack <std::string> * expressionStack, std::list <std::string> * expressionList){
	auto transfer = (*expressionStack).top();
	(*expressionList).push_back(transfer);
	(*expressionStack).pop();
}

void transferStack2(std::stack <std::string> * expressionStack, std::list <std::string> * expressionList, char * token){
	
	std::string x(token); 
	auto transfer = (*expressionStack).top();
	(*expressionList).push_back(transfer);
	(*expressionStack).pop();
	(*expressionStack).push(x);
	
}

void pushToStack(std::stack <std::string> * expressionStack, char * token){
	std::string x(token);
	(*expressionStack).push(x);
}
 
//---------------------------------------------------------------------------------------------------------------------------------------

std::list <std::string> convertInfix(std::string expression){


	std::list <std::string> expressionList;
	std::stack <std::string> expressionStack;
	
	
	char expressionArray[expression.size() + 1];
	char * token;
	strcpy(expressionArray, expression.c_str() );
	//printf ("Splitting string \"%s\" into tokens:\n",expressionArray);
	token =	strtok(expressionArray," #");
	
	while(token != NULL){
		
		//std::cout << "Token IS:" << *token << '\n';

		//if token is *,/,+,-,(,) using binary expression. 
		if( (*token > 0b100111) && (*token < 0b110000) ){

			
			if( (expressionStack.empty() or *token == ('(') ) or (expressionStack.top() == ("(") and *token != (')') ) ){

				pushToStack(&expressionStack, token);
				token = strtok(NULL," #");
			}
			
			//-----------------------------------------------------------------------------------------------------------------
			
			else if (*token == (')') ){
				while(expressionStack.top() != ("(") ){
					
					//transfer everything in enclosed ( )
					transferStack1(&expressionStack, &expressionList);
				}
				token = strtok(NULL," #");
				expressionStack.pop();//pop '('
				
			}
			
			//-----------------------------------------------------------------------------------------------------------------
			
			else if( *token == ('*') or *token == ('/') ){

				if(expressionStack.top() != ("*") and expressionStack.top() != ("/") ){
						
					//assuming the operator is either + or - if not * or /
					pushToStack(&expressionStack, token);
					token = strtok(NULL," #");
				}
				else{
					//operator is * or /
					transferStack2(&expressionStack, &expressionList, token);
					token = strtok(NULL," #");
				}
			}
			
			//-----------------------------------------------------------------------------------------------------------------
			
			else if(  *token == ('+') or *token == ('-') ){

				if(expressionStack.top() == ("*") or expressionStack.top() == ("/") ){
					
					//pop * or / and transfer to list
					transferStack1(&expressionStack, &expressionList);

					//push if empty stack or top is '('
					if(expressionStack.empty() or expressionStack.top() == ("(") ){
						
						pushToStack(&expressionStack, token);
						token = strtok(NULL," #");
					}


					else if(expressionStack.top() == ("+") or expressionStack.top() == ("-") ){
						
						//pop + or - and transfer to list
						transferStack2(&expressionStack, &expressionList, token);
						token = strtok(NULL," #");
					}

				}
				
				//-----------------------------------------------------------------------------------------------------------------
				
				else if(expressionStack.top() == ("+") or expressionStack.top() == ("-") ){

						//pop + or - and transfer new + or - to list
						transferStack1(&expressionStack, &expressionList);

						if(expressionStack.empty() or expressionStack.top() == ("(") ){
							
							pushToStack(&expressionStack, token);
							token = strtok(NULL," #");
						}

						else if(expressionStack.top() == ("*") or expressionStack.top() == ("/") ){
							
							transferStack2(&expressionStack, &expressionList, token);
							token = strtok(NULL," #");
						}
						else{
							
							//expressionStack.push(x);
							pushToStack(&expressionStack, token);
							token = strtok(NULL," #");
						}
				}
				
			}	
			
			//-----------------------------------------------------------------------------------------------------------------
			
			else{
	
				pushToStack(&expressionStack, token);
				token = strtok(NULL," #");
			}
			

		}
		
		else{
			std::string x(token);
			expressionList.push_back(x);
			token = strtok(NULL," #");
		}
				
				
	}
	
	
	//empty stack and finish expression
	while( not expressionStack.empty() ){
		
		transferStack1(&expressionStack, &expressionList);
	}

	/*
	std::cout << "Final Stack Is: ";
	while ( not expressionStack.empty() ){
		std::cout << expressionStack.top();
		expressionStack.pop();
		
	}
	
	std::cout << '\n';
	*/
	
	return expressionList;

}

//POSTFIX EVALUATOR
//---------------------------------------------------------------------------------------------------------------------------------------

void calculatePost(std::stack <int> * expressionStack, std::string operatorType){
	
	auto variables = popVariables(expressionStack);
	
	if ( operatorType == "+" ){

		(*expressionStack).push( std::get<0>(variables) + std::get<1>(variables) );  
	}
	
	else if ( operatorType == "-" ){
		
		(*expressionStack).push( std::get<0>(variables) - std::get<1>(variables) );  
	}
	
	else if ( operatorType == "*" ){
		
		(*expressionStack).push( std::get<0>(variables) * std::get<1>(variables) );  
	}
	
	else if ( operatorType == "/" ){
		
		(*expressionStack).push( std::get<0>(variables) / std::get<1>(variables) );  
	}
	
	//throw std::invalid_argument("Not A Valid Operator") ;
	
}

std::tuple <int, int> popVariables(std::stack <int> * expressionStack){
	
	int y = (*expressionStack).top();
	(*expressionStack).pop();
	int x = (*expressionStack).top();
	(*expressionStack).pop();
	
	return std::make_tuple(x, y); //return 2 values
	
}

int convertVariable(std::stack <int> * expressionStack, std::string variable ){
	
	std::string input;
	printf("Enter a Value For Variable ' %s ' : ", variable.c_str());
	std::getline(std::cin, input);
	std::size_t found = input.find_first_not_of("-1234567890 ");
	
	if(found != std::string::npos){
		input.clear();
		return convertVariable(expressionStack, variable);
	}
	else{
		
		return std::stoi(input);
	}
}

void postfixEvaluate(std::list <std::string> expressionList){
	
	std::stack <int> expressionStack;
	
	for (auto it = expressionList.begin(); it != expressionList.end(); ++it){
		
		//std::cout << "token is: " << *it << "\n";

		if( isdigit((*it)[0]) or ( (*it)[0] == '-' and isdigit((*it)[1]) ) ){
			expressionStack.push( std::stoi((*it)) );
			
		}
		else if ( isalpha( (*it)[0]) ){
			
			expressionStack.push( convertVariable(&expressionStack, (*it)) );
		}
		else{
			
			calculatePost(&expressionStack, (*it));
			
		}
	}
	
	std::cout << "\nPostfix Calculation Is: " << expressionStack.top() << "\n\n";
}
