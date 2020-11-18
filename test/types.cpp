#include <iostream>
#include "../src/Onthology.hpp"
using namespace std;
using namespace DL;

/*void print_subs(Concept &c){
	std::cout << "I concetti contenuti da " << Onthology::getInstance().allConcepts.at(0).getName() << std::endl;


	if(!c.getSubsumes().empty()){
		std::cout << "Sono prima del primo for" << std::endl;
		for(auto it = c.getSubsumes().begin(); it != c.getSubsumes().end(); it++){
			std::cout << "Sono entrato nel primo for" << std::endl;
			std::cout << (*it)->getName() << std::endl;
		}
	}

	std::cout << "I concetti che contengono " << Onthology::getInstance().allConcepts.at(0).getName() << std::endl;
	if(!c.getSubsumed().empty()){
		for(auto it = c.getSubsumed().begin(); it != c.getSubsumed().end(); it++){
			std::cout << (*it)->getName() << std::endl;
		}
	}
}*/

int main ()
{
	std::string str_1("PC");
	std::string str_2("Componente");
	std::string str_3("Transistor");
	Onthology::getInstance().put_c(str_1);
	Onthology::getInstance().put_c(str_2);
	Onthology::getInstance().put_c(str_3);
	try 
	{
		Onthology::getInstance().subsumption(&Onthology::getInstance().allConcepts.at(2), &Onthology::getInstance().allConcepts.at(1));
		Onthology::getInstance().subsumption(&Onthology::getInstance().allConcepts.at(1), &Onthology::getInstance().allConcepts.at(0));

		/*print_subs(Onthology::getInstance().allConcepts.at(0));*/

		std::cout << Onthology::getInstance().allConcepts.at(0).getSubsumes().at(0)->getName() << std::endl;

		for(vector<Concept*>::iterator i = Onthology::getInstance().allConcepts.at(0).getSubsumes().begin(); i!= Onthology::getInstance().allConcepts.at(0).getSubsumes().end(); i++){
			Concept* co = (*i);
			std::cout << co->getName() << std::endl;
		}

		/*for(auto i = Onthology::getInstance().allConcepts.begin(); i!= Onthology::getInstance().allConcepts.end(); i++){
			print_subs((*i));
		}*/
	}
	catch (logic_error e)
	{
		cerr << e.what();
		exit(EXIT_FAILURE);
	}
}