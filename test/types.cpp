#include <iostream>
#include "../src/Onthology.hpp"
using namespace std;
using namespace DL;

void print_subs(Concept &c){
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

int main ()
{
	std::string str_1("PC");
	std::string str_2("Componente");
	std::string str_3("Transistor");
	std::string str_4("GPU");
	Onthology::getInstance().put_c(str_1);
	Onthology::getInstance().put_c(str_2);
	Onthology::getInstance().put_c(str_3);
	Onthology::getInstance().put_i(str_4);

	try 
	{
		//Onthology::getInstance().subsumption(&Onthology::getInstance().allConcepts.at(2), &Onthology::getInstance().allConcepts.at(1));
		//Onthology::getInstance().subsumption(&Onthology::getInstance().allConcepts.at(1), &Onthology::getInstance().allConcepts.at(0));

		/*print_subs(Onthology::getInstance().allConcepts.at(0));*/

		/*std::cout << Onthology::getInstance().allConcepts.at(0).getSubsumes().at(0)->getName() << std::endl;*/

		/*for(int i = 0; i < Onthology::getInstance().allConcepts.at(0).getSubsumes().size(); i++){
			std::cout << Onthology::getInstance().allConcepts.at(0).getSubsumes().at(i)->getName() << std::endl;
		}*/

		/*for(auto i = Onthology::getInstance().allConcepts.begin(); i!= Onthology::getInstance().allConcepts.end(); i++){
			print_subs((*i));
		}*/
		Concept c = Onthology::getInstance().get_c(str_2);
		//Individual i = Onthology::getInstance().get_i(str_4);
		c.addIndividual(str_4);

		std::cout << c.getIndividuals().at(0)->getName() << std::endl;
	}
	catch (logic_error e)
	{
		cerr << e.what();
		exit(EXIT_FAILURE);
	}
}