#include <iostream>
#include <cstdlib>
#include <cstring>

#include "src/dl_driver.hpp"
#include "src/Onthology.hpp"

int main ( const int argc, const char **argv )
{
	if (argc == 2)
	{
		DL::DL_Driver driver;
		driver.parse( argv[1] );
		driver.print( std::cout ) << std::endl;
		return (EXIT_SUCCESS);
	}
	else
	{
		std::cerr << "Argomenti incompatibili." << std::endl;
		return (EXIT_FAILURE);
	}
	
}