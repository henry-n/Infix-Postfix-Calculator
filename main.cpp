#include "Calculator.h"


main(){
	
   int choice;
   std::list <std::string> expressionList;


   do{

        choice = menu();

        if (choice == 1){
            expressionList = getInput();
			
			std::cout << "\nPostfix Expression: ";
			for (auto it = expressionList.begin(); it != expressionList.end(); ++it)
				std::cout << "" << *it;
			std::cout << '\n';
			
        }

        else if (choice == 2){

        }


   }while(choice != 3);	
	
}


int menu(){

    std::cout << "(1)Convert Infix to Postfix\n";
    std::cout << "(2)Evaluate Expression\n";
    std::cout << "(3)Exit\n";

    std::string choice;
    std::cout << "Enter Choice: "; std::cin >> choice;
    std::cin.ignore();//needed for getline function. discards input buffer (newline)

    int filtered = atoi(choice.c_str());

    //check and filter input
    if(filtered >= 1 && filtered <= 3 )
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
 


std::list <std::string> convertInfix(std::string expression){


	std::list <std::string> expressionList;
	std::stack <std::string> expressionStack;
	
	
	char expressionArray[expression.size() + 1];
	char * token;
	strcpy(expressionArray, expression.c_str() );
	printf ("Splitting string \"%s\" into tokens:\n",expressionArray);
	token =	strtok(expressionArray," #");
	
	while(token != NULL){

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