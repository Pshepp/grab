#ifndef INC_LAZYPRINTS_H_
#define INC_LAZYPRINTS_H_

#include <iostream>

std::string starLine = "****************************************\n";
//LAZY INFORMATION OUTPUT
void lazyInfo(int lineCalled, const char *funcName, std::string infoToPass)
{
	std::cout << starLine << "\t\tINFORMATION\n" << starLine;
	std::cout << "Function Name: " << funcName << std::endl;
	std::cout << "Line Number: " << lineCalled << std::endl;
	std::cout << "Msg: " << infoToPass << std::endl << std::endl;
}

void badBehavior(int lineBroke, const char *funcNameBroke)
{
	std::cout << starLine << "\t\tBORKED\n" << starLine;
	std::cout << "Borked Function Name: " << funcNameBroke << std::endl;
	std::cout << "Line Number: " << lineBroke << std::endl << std::endl;
}


#endif /* INC_LAZYPRINTS_H_ */
