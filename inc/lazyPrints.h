/**
 * @file lazyPrints.h
 * @author Preston Sheppard
 * @date 10 Mar 2021
 * @brief In order to help use keep a track on behavior this was thrown together.
 *
 *	Wherever badBehavior is present we want to add error catching
 */

#ifndef INC_LAZYPRINTS_H_
#define INC_LAZYPRINTS_H_

#include <iostream>

std::string starLine = "****************************************\n";

//LAZY INFORMATION OUTPUT
void lazyInfo(int lineCalled, const char *funcName)
{
	std::cout << starLine << "\tINFORMATION: " << funcName << "\n" << starLine;
	std::cout << "Function Name: " << funcName << std::endl;
	std::cout << "Line Number: " << lineCalled << std::endl << std::endl;
}

void lazyInfo(int lineCalled, const char *funcName, std::string infoToPass)
{
	std::cout << starLine << "\t\tINFORMATION\n" << starLine;
	std::cout << "Function Name: " << funcName << std::endl;
	std::cout << "Line Number: " << lineCalled << std::endl;
	std::cout << "Msg: " << infoToPass << std::endl << std::endl;
}

/* NOTE: Please note that these are to temporarily keep track of unwanted behavior
 * 			and we will eventually replace these with correct error handling etc.
 * 			but while the program and logic are in flux I would prefer to keep
 * 			our error handling ambiguous.
 *
 */
void badBehavior(int lineBroke, const char *funcNameBroke)
{
	std::cout << starLine << "\t\tBORKED\n" << starLine;
	std::cout << "Borked Function Name: " << funcNameBroke << std::endl;
	std::cout << "Line Number: " << lineBroke << std::endl << std::endl;
}

void badBehavior(int lineBroke, const char *funcNameBroke,
		std::string infoToPass)
{
	std::cout << starLine << "\t\tBORKED\n" << starLine;
	std::cout << "Borked Function Name: " << funcNameBroke << std::endl;
	std::cout << "Line Number: " << lineBroke << std::endl << std::endl;
	std::cout << "Msg: " << infoToPass << std::endl << std::endl;
}

#endif /* INC_LAZYPRINTS_H_ */
