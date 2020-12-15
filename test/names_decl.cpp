#include <iostream>
#include <cstdlib>
#include <cstring>

#include "../src/dl_driver.hpp"
#include "../src/Ontology.hpp"

std::ostream& print( std::ostream &stream )
{
	DL::Ontology& o = DL::Ontology::getInstance();
	stream << "\nAll Names:" << std::endl;
	for (auto i = o.allNames.begin(); i != o.allNames.end(); i++)
	{
		stream << *i << std::endl;
	}

	stream << "\nAll Concepts:" << std::endl;
	for (auto i = o.allConcepts.begin(); i != o.allConcepts.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	stream << "\nAll Roles:" << std::endl;
	for (auto i = o.allRoles.begin(); i != o.allRoles.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	stream << "\nAll Individuals:" << std::endl;
	for (auto i = o.allIndividuals.begin(); i != o.allIndividuals.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	return(stream);
}

int main ( const int argc, const char **argv )
{
	if (argc == 2)
	{
		DL::DL_Driver driver;
		driver.parse( argv[1] );
		print( std::cout ) << std::endl;
		return (EXIT_SUCCESS);
	}
	else
	{
		std::cerr << "Argomenti incompatibili." << std::endl;
		return (EXIT_FAILURE);
	}
	
}