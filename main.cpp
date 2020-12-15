#include <iostream>
#include <cstdlib>
#include <cstring>
#include "src/dl_driver.hpp"
#include "src/Ontology.hpp"
using namespace std;
using namespace DL;

std::ostream& print( std::ostream &stream )
{
	DL::Ontology& o = DL::Ontology::getInstance();

	stream << "\n>All Names:" << std::endl;
	for (auto i = o.allNames.begin(); i != o.allNames.end(); i++)
	{
		stream << *i << std::endl;
	}

	stream << "\n>All Concepts:" << std::endl;
	for (auto i = o.allConcepts.begin(); i != o.allConcepts.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	stream << "\n>All Roles:" << std::endl;
	for (auto i = o.allRoles.begin(); i != o.allRoles.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	stream << "\n>All Individuals:" << std::endl;
	for (auto i = o.allIndividuals.begin(); i != o.allIndividuals.end(); i++)
	{
		stream << i->getName() << std::endl;
	}

	return(stream);
}

void print_indv (DL::Concept &c)
{
	std::cout << "\n>The individuals istantiated in concept: " << c.getName() << std::endl;

	if(!c.getIndividuals().empty()){
		for(size_t i = 0; i < c.getIndividuals().size(); i++){
			std::cout << c.getIndividuals().at(i) << std::endl;
		}
	}
}

void print_pair (DL::Role &r)
{
	std::cout << "\n>The pairs of individuals instantiated in role: " << r.getName() << std::endl;

	if(!r.getPairs().empty()){
		for (auto it : r.getPairs())
		{
			std::cout << it.first << ", " << it.second << std::endl;
		}
	}
}

void print_subs ()
{
	Ontology& o = Ontology::getInstance();
	
	cout << "\n>Subsumption Graph" << endl;

	for (auto it : o.subsGraph)
	{
		cout << it.first << " subsumed by " << it.second << endl;
	}
}

void print_negate ()
{
	Ontology& o = Ontology::getInstance();
	
	cout << "\n>Pairs of negations" << endl;
	
	for(auto p : o.negateMap)
	{
		cout << p.first << " is the negation of " << p.second << std::endl;
	}
}

int main ( const int argc, const char **argv )
{
	if (argc == 2)
	{
		string filename = argv[1];
		if (filename.substr(filename.length() - 3, 3) == ".dl")
		{
			DL::DL_Driver driver;
			DL::Ontology& ont = DL::Ontology::getInstance();
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

			print_negate();

			return (EXIT_SUCCESS);			
		}
		else
		{
			std::cerr << "Incompatible extension; must be '.dl'." << std::endl;
			return (EXIT_FAILURE);	
		}
		
	}
	else
	{
		std::cerr << "Incompatible arguments; only one file required." << std::endl;
		return (EXIT_FAILURE);
	}
}