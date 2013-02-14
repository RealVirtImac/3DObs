/*!
 * 
 * \file main.cpp
 * \brief Main program of 3DObs
 * \author R. Bertozzi & S. Bougeois 
 * 
 */

#include "../include/Application.hpp"

/*!
 * 
 * \brief Main 
 * \param argc : number of arguments
 * \param argv : arguments
 * \return 0
 * 
 */
int main(int argc, char** argv)
{
	Application app;
	return app.on_execute();
	
	return 0;
}
