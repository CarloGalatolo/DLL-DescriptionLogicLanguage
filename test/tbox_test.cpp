#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../src/dl_driver.hpp"
#include "../src/Onthology.hpp"
using namespace std;
using namespace DL;

std::ostream& print( std::ostream &stream )
{
	DL::Onthology& o = DL::Onthology::getInstance();

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

void print_subs ()
{
	Onthology& o = Onthology::getInstance();
	for (auto it : o.subsGraph)
	{
		cout << it.first->getName() << " e' contenuto da " << it.second->getName() << endl;
	}
}

void print_indv (DL::Concept &c)
{
	std::cout << "Gli individui istanziati nel concetto: " << c.getName() << std::endl;


	if(!c.getIndividuals().empty()){
		for(size_t i = 0; i < c.getIndividuals().size(); i++){
			std::cout << c.getIndividuals().at(i)->getName() << std::endl;
		}
	}
}

void print_pair (DL::Role &r)
{
	std::cout << "Le coppie di individui istanziate nel ruolo: " << r.getName() << std::endl;

	if(!r.getPairs().empty()){
		for (auto it : r.getPairs())
		{
			std::cout << it.first->getName() << ", " << it.second->getName() << std::endl;
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

		print_subs();

		for (auto it = DL::Onthology::getInstance().allConcepts.begin(); it != DL::Onthology::getInstance().allConcepts.end(); it++)
		{
			print_indv(*it);
		}

		for (auto it = DL::Onthology::getInstance().allRoles.begin(); it != DL::Onthology::getInstance().allRoles.end(); it++)
		{
			print_pair(*it);
		}
/*
		DL::Concept* c = &DL::Onthology::getInstance().allConcepts.at(0);

		std::cout << "Gli individui istanziati nel concetto: " << c->getName() << std::endl;
		for(size_t i = 0; i < c->getIndividuals().size(); i++){
			std::cout << c->getIndividuals().at(i)->getName() << std::endl;
		}
*/
		//DL::Individual i = *(c.getIndividuals().at(0));

		return (EXIT_SUCCESS);
	}
	else
	{
		std::cerr << "Argomenti incompatibili." << std::endl;
		return (EXIT_FAILURE);
	}
	
}