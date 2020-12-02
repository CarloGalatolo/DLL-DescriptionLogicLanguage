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

void print_indv (DL::Concept &c)
{
	std::cout << "Gli individui istanziati nel concetto: " << c.getName() << std::endl;

	if(!c.getIndividuals().empty()){
		for(size_t i = 0; i < c.getIndividuals().size(); i++){
			std::cout << c.getIndividuals().at(i) << std::endl;
		}
	}
}

void print_pair (DL::Role &r)
{
	std::cout << "Le coppie di individui istanziate nel ruolo: " << r.getName() << std::endl;

	if(!r.getPairs().empty()){
		for (auto it : r.getPairs())
		{
			std::cout << it.first << ", " << it.second << std::endl;
		}
	}
}


void print_subs ()
{
	Onthology& o = Onthology::getInstance();
	for (auto it : o.subsGraph)
	{
		cout << it.first << " e' contenuto da " << it.second << endl;
	}
}
/*
void print_negate ()
{
	Onthology& o = Onthology::getInstance();
	std::cout << "La dimensione della mappa dei negati e' " << o.negateMap.size() << std::endl;
	for(/*std::pair<Concept*, Concept*> p : o.negateGraph auto it = o.negateMap.begin(); it != o.negateMap.end(); it++){
		//cout << p.first->getName() << " e' il negato di " << p.second->getName() << std::endl;
		cout << it->first->getName() << " e' il negato di " << it->second->getName() << std::endl;
	}	
}
*/
int main ( const int argc, const char **argv )
{
	if (argc == 2)
	{
		DL::DL_Driver driver;
		DL::Onthology& ont = DL::Onthology::getInstance();
		driver.parse( argv[1] );
		print( std::cout ) << std::endl;

		for (auto it = ont.allConcepts.begin(); it != ont.allConcepts.end(); it++)
		{
			print_indv(*it);			
		}

		for (auto it = ont.allRoles.begin(); it != ont.allRoles.end(); it++)
		{
			print_pair(*it);
		}
		print_subs();
/*
		print_negate();
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