#include <iostream>
#include "../src/Ontology.hpp"

/*
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
*/
int main ()
{
	std::string str_1("PC");
	std::string str_2("Componente");
	std::string str_3("Transistor");
	std::string str_4("GPU");
	DL::Ontology::getInstance().put_c(str_1);
	DL::Ontology::getInstance().put_c(str_3);
	DL::Ontology::getInstance().put_c(str_2);
	DL::Ontology::getInstance().put_i(str_4);

	try 
	{
		//Ontology::getInstance().subsumption(&Ontology::getInstance().allConcepts.at(2), &Ontology::getInstance().allConcepts.at(1));
		//Ontology::getInstance().subsumption(&Ontology::getInstance().allConcepts.at(1), &Ontology::getInstance().allConcepts.at(0));

		/*print_subs(Ontology::getInstance().allConcepts.at(0));*/

		/*std::cout << Ontology::getInstance().allConcepts.at(0).getSubsumes().at(0)->getName() << std::endl;*/

		/*for(int i = 0; i < Ontology::getInstance().allConcepts.at(0).getSubsumes().size(); i++){
			std::cout << Ontology::getInstance().allConcepts.at(0).getSubsumes().at(i)->getName() << std::endl;
		}*/

		/*for(auto i = Ontology::getInstance().allConcepts.begin(); i!= Ontology::getInstance().allConcepts.end(); i++){
			print_subs((*i));
		}*/
		DL::Concept c = DL::Ontology::getInstance().get_c(str_2);
		//Individual i = Ontology::getInstance().get_i(str_4);
		c.addIndividual(str_4);

		std::cout << c.getIndividuals().at(0)->getName() << std::endl;
	}
	catch (std::logic_error e)
	{
		std::cerr << e.what();
		exit(EXIT_FAILURE);
	}
}