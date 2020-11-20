#include <iostream>
#include <cstdlib>
#include <cstring>

#include "../src/dl_driver.hpp"
#include "../src/Onthology.hpp"

std::ostream& print( std::ostream &stream )
{
	DL::Onthology& o = DL::Onthology::getInstance();
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

void print_subs (DL::Concept &c)
{
	std::cout << "I concetti contenuti da " << c.getName() << std::endl;


	if(!c.getSubsumes().empty()){
		for(int i = 0; i < c.getSubsumes().size(); i++){
			std::cout << c.getSubsumes().at(i)->getName() << std::endl;
		}
	}

	std::cout << "I concetti che contengono " << c.getName() << std::endl;
	if(!c.getSubsumed().empty()){
		for(int i = 0; i < c.getSubsumed().size(); i++){
			std::cout << c.getSubsumed().at(i)->getName() << std::endl;
		}
	}
}

int main ( const int argc, const char **argv )
{
	if (argc == 2)
	{
		DL::DL_Driver driver;
		driver.parse( argv[1] );
		print( std::cout ) << std::endl;

		for (auto it = DL::Onthology::getInstance().allConcepts.begin(); it != DL::Onthology::getInstance().allConcepts.end(); it++)
		{
			print_subs(*it);
		}

		return (EXIT_SUCCESS);
	}
	else
	{
		std::cerr << "Argomenti incompatibili." << std::endl;
		return (EXIT_FAILURE);
	}
	
}