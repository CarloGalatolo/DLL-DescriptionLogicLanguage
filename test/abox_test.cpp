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

void print_indv (DL::Concept &c)
{
	std::cout << "Gli individui istanziati nel concetto: " << c.getName() << std::endl;


	if(!c.getIndividuals().empty()){
		for(DL::Individual* ind : c.getIndividuals()){
			if(ind == nullptr){
				std::cout << "Sono vuoto" << std::endl;
			}
			else{
				std::cout << ind->getName()<< std::endl;
			}

		}
	}
}

void print_pair (DL::Role &r)
{
	std::cout << "Le coppie di individui istanziate nel ruolo: " << r.getName() << std::endl;


	if(!r.getPairs().empty()){
		for(auto it = r.getPairs().begin(); it != r.getPairs().end(); it++){
			std::cout << it->first->getName() << ", "  << it->second->getName() << std::endl;
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

		/*for (auto it = DL::Ontology::getInstance().allConcepts.begin(); it != DL::Ontology::getInstance().allConcepts.end(); it++)
		{
			print_subs(*it);
		}*/

		for (auto it = DL::Ontology::getInstance().allConcepts.begin(); it != DL::Ontology::getInstance().allConcepts.end(); it++)
		{
			print_indv(*it);
		}

		/*for (auto it = DL::Ontology::getInstance().allRoles.begin(); it != DL::Ontology::getInstance().allRoles.end(); it++)
		{
			print_pair(*it);
		}*/
/*
		DL::Concept* c = &DL::Ontology::getInstance().allConcepts.at(0);

		std::cout << "Gli individui istanziati nel concetto: " << c->getName() << std::endl;
		for(size_t i = 0; i < c->getIndividuals().size(); i++){
			std::cout << c->getIndividuals().at(i)->getName() << std::endl;
		}
*/
		//DL::Individual i = *(c.getIndividuals().at(0));

		std::cout << " ciao " << std::endl;



		return (EXIT_SUCCESS);
	}
	else
	{
		std::cerr << "Argomenti incompatibili." << std::endl;
		return (EXIT_FAILURE);
	}
	
}