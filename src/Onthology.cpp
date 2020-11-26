#include "Onthology.hpp"
using std::string;

template<class K, class V>
typename std::multimap<K, V>::const_iterator find_pair(const std::multimap<K, V>& map, const std::pair<K, V>& pair)
{
    typedef typename std::multimap<K, V>::const_iterator it;
    std::pair<it,it> range = map.equal_range(pair.first);
    for (it p = range.first; p != range.second; ++p)
        if (p->second == pair.second)
            return p;
    return map.end();
}

// === CLASS ONTHOLOGY ===

void DL::Onthology::put (DL::Concept& c)
{
	if (checkNames(c.getName()))	// Nome già esistente.
	{
		throw std::logic_error("Name already existing");
	}
	else
	{
		allNames.push_back(c.getName());
		allConcepts.push_back(c);
	}
}

void DL::Onthology::put (DL::Role& r)
{
	if (checkNames(r.getName()))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(r.getName());
		allRoles.push_back(r);
	}
}

void DL::Onthology::put (DL::Individual& i)
{
	if (checkNames(i.getName()))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(i.getName());
		allIndividuals.push_back(i);
	}
}

void DL::Onthology::put_c (std::string& s)
{
	if (checkNames(s))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(s);
		allConcepts.push_back(DL::Concept(s));
	}
}

void DL::Onthology::put_r (std::string& s)
{
	if (checkNames(s))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(s);
		allRoles.push_back(DL::Role(s));
	}
}

void DL::Onthology::put_i (std::string& s)
{
	if (checkNames(s))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(s);
		allIndividuals.push_back(DL::Individual(s));
	}
}

DL::Concept& DL::Onthology::get_c (std::string& s)
{
	auto res = myFind(allConcepts.begin(), allConcepts.end(), s);

	try
	{
		if (res == allConcepts.end()) throw std::runtime_error("Concept doesn't exixt: ");
	}
	catch (std::runtime_error e) // Critical error
	{
		std::cerr << "Error in getting a concept: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Concept& r = *res;
	return r;
}

DL::Role& DL::Onthology::get_r (std::string& s)
{
	auto res = myFind(allRoles.begin(), allRoles.end(), s);
	
	try
	{
		if (res == allRoles.end()) throw std::runtime_error("Concept doesn't exixt");
	}
	catch (std::runtime_error e)
	{
		std::cerr << "Error in getting a role: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Role& r = *res;
	return r;
}

DL::Individual& DL::Onthology::get_i (std::string& s)
{
	auto res = myFind(allIndividuals.begin(), allIndividuals.end(), s);
	
	try
	{
		if (res == allIndividuals.end()) throw std::runtime_error("Concept doesn't exixt");
	}
	catch (std::runtime_error e) // Critical error
	{
		std::cerr << "Error in getting an individual: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Individual& r = *res;
	return r;
}

void DL::Onthology::subsumption (std::string& c_1, std::string& c_2) // a subsumed by b
{
	DL::Concept* subsumed = &DL::Onthology::getInstance().get_c(c_1);
	DL::Concept* subsumes = &DL::Onthology::getInstance().get_c(c_2);
	auto subsumedIndvs = subsumed->getIndividuals();

	std::pair<DL::Concept*, DL::Concept*> p = std::make_pair(subsumed, subsumes);
	if (find_pair(this->subsGraph, p) == this->subsGraph.end())
	{
		this->subsGraph.insert(p);
		// All the individuals of the subsumed concept are put into the subsuming concept as well.
		for (Individual* it : subsumedIndvs)
		{
			subsumes->addIndividual(it);
		}
	}
	else
	{
		throw std::logic_error(" Subsuming: pair already exists ");
	}
}

std::string DL::Onthology::conjunction (std::string& s1, std::string& s2) // Intersezione
{
	std::string name = s1 + "CONJ" + s2;
	Concept res(name);
	put(res);
	Concept &c1 = get_c(s1), &c2 = get_c(s2), &c = get_c(name);
	auto c1Indvs = c1.getIndividuals(), c2Indvs = c2.getIndividuals();

	subsumption(name, s1);
	subsumption(name, s2);

	if (!(c1Indvs.empty() || c2Indvs.empty()))
	{
		for (DL::Individual* i1 : c1Indvs)
		{
			for (DL::Individual* i2 : c2Indvs)
			{
				if (!(i1 == nullptr || i2 == nullptr))
				{
					if (i1->getName().compare(i2->getName()) == 0)
					{
						try
						{
							c.addIndividual(i1);
							std::cout << c.getIndividuals().at(0)->getName() << std::endl;
						}
						catch(const std::exception& e)
						{
							// Not a problem.
						}
					}					
				}
			}
		}		
	}
	// else the conjunction is an empty concept.

	return name;
}

std::string DL::Onthology::disjunction (std::string& s1, std::string& s2) // Unione
{
	std::string name = s1 + "DISJ" + s2;
	Concept res(name);
	put(res);
	Concept &c1 = get_c(s1), &c2 = get_c(s2), &c = get_c(name);
	auto c1Indvs = c1.getIndividuals(), c2Indvs = c2.getIndividuals();

	subsumption(s1, name);
	subsumption(s2, name);

	if (!(c1Indvs.empty() || c2Indvs.empty()))
	{
		for(DL::Individual* i1 : c1Indvs)
		{
			try
			{
				c.addIndividual(i1); // Checks for nullptr.
			}
			catch (std::exception e)
			{
				// No problem.
			}
		}

		for(Individual* i2 : c2Indvs){
			try
			{
				c.addIndividual(i2); // Checks for nullptr.
			}
			catch (std::exception e)
			{
				// No problem.
			}
		}	
	}
	// else the disjunction is an empty concept.

	return name;
}

string DL::Onthology::negation (string& s)
{
	auto& ont = DL::Onthology::getInstance();

	DL::Concept *c = &ont.get_c(s);
	string neg = "not" + c->getName();
	auto cIndvs = c->getIndividuals();
	bool check = false;
	ont.put_c(neg);
	
	if (ont.negateGraph.find(c) == ont.negateGraph.end())
	{
		std::pair<DL::Concept*,DL::Concept*> negPair = std::make_pair(c, &ont.get_c(neg));
		ont.negateGraph.insert(negPair);
	}
	
	if (cIndvs.empty())
	{
		for (DL::Individual& indv : ont.allIndividuals)
		{
			ont.get_c(neg).addIndividual(&indv);
		}
	}
	else
	{
		for (DL::Individual& indv : ont.allIndividuals)
		{
			for (DL::Individual* idual : cIndvs)
			{
				if (idual->getName().compare(indv.getName()) == 0)
				{
					check = true;
					break;
				}
			}

			if (!check)
			{
				ont.get_c(neg).addIndividual(&indv);
			}

			check = false;
		}		
	}


	return neg;
}

string DL::Onthology::universal(std::string& r, std::string& c){
	DL::Onthology& ont = DL::Onthology::getInstance();
	std::string name = "UNIV" + r + "." + c;
	DL::Concept res(name);
	ont.put(res);
	bool exit = false;
	bool found = false;

	for(DL::Individual ind : ont.allIndividuals){
		if(!ind.getRoles().empty()){
			if((ind.getRoles().at(0)->getName().compare(r) != 0) || (ind.getRoles().size() != 1)){
				break;
				found = false;
			}
			else{
				std::multimap<DL::Individual *, DL::Individual *> pairList = ind.getRoles().at(0)->getPairs();
				if(!pairList.empty()){
					for(auto p : pairList){
						auto concList = p.second->getConcepts();
						if(!concList.empty()){
							for(DL::Concept* conc : concList){
								if(conc->getName().compare(c) != 0){
									break;
									exit = true;
								}
							}							
						}
						if(exit){
							found = false;
							break;
						}
						found = true;
					}
				}
				if(found){
					ont.get_c(name).addIndividual(&ind);
					found = false;
				}
			}

		}
	}


	return name;
}

bool DL::Onthology::checkNames (const std::string& s) const
{
	auto res = std::find(allNames.begin(), allNames.end(), s);

	if (res != allNames.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class InputIterator, class T>
InputIterator DL::Onthology::myFind (InputIterator first, InputIterator last, const T& val) const
{
	while (first!=last) {
		if ((*first).getName()==val) return first;
		++first;
	}
	return last;
}

bool DL::Onthology::checkConcepts (const std::string& s) const
{
	if (myFind(allConcepts.begin(), allConcepts.end(), s) != allConcepts.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DL::Onthology::checkRoles (const std::string& s) const
{
	if (myFind(allRoles.begin(), allRoles.end(), s) != allRoles.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DL::Onthology::checkIndividuals (const std::string& s) const
{
	if (myFind(allIndividuals.begin(), allIndividuals.end(), s) != allIndividuals.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ===== CLASS INDIVIDUAL =====

DL::Individual::Individual (std::string& name)
{
	this->name = name;
}

void DL::Individual::addConcept (DL::Concept* con)
{
	if (con == nullptr)
	{
		std::cerr << " In Individual::addConcept(): argument is a null pointer. " << std::endl;
		return;
	}
	concepts.push_back(con);
}

void DL::Individual::addRole (DL::Role* role)
{
	if (role == nullptr)
	{
		std::cerr << " In Individual::addRole(): argument is a null pointer. " << std::endl;
		return;
	}
	roles.push_back(role);
}

std::string DL::Individual::getName () const
{
	return this->name;
}

std::vector<DL::Concept*> DL::Individual::getConcepts () const
{
	return this->concepts;
}

std::vector<DL::Role*> DL::Individual::getRoles () const
{
	return this->roles;
}

// ===== CLASS ROLE =====

DL::Role::Role (std::string& name)
{
	this->name = name;
}

std::string DL::Role::getName () const
{
	return this->name;
}

std::multimap<DL::Individual*, DL::Individual*> DL::Role::getPairs () const
{
	return this->pairs;
}

void DL::Role::insert (string& s1, string& s2)
{
	DL::Individual* first = &DL::Onthology::getInstance().get_i(s1);
	DL::Individual* second = &DL::Onthology::getInstance().get_i(s2);

	std::pair<DL::Individual*, DL::Individual*> p = std::make_pair(first, second);

	if (find_pair(pairs, p) == pairs.end())
	{
		pairs.insert(p);
	}
	else
	{
		throw std::logic_error("In role assertion: already existing.");
	}
}

// === CLASS CONCEPT ===

DL::Concept::Concept (std::string& name)
{
	this->name = name;
}

std::string DL::Concept::getName () const
{
	return this->name;
}

std::vector<DL::Individual*> DL::Concept::getIndividuals () const
{
	return this->individuals;
}

std::vector<DL::Concept*> DL::Concept::getSubsumes () const
{
	/*ATTENZIONE NON USARE CON GLI ITERATORI*/
	return (*this).subsumes;
}

std::vector<DL::Concept*> DL::Concept::getSubsumed () const
{
	/*ATTENZIONE NON USARE CON GLI ITERATORI*/
	return (*this).subsumed;
}

void DL::Concept::addIndividual (DL::Individual* i)
{
	if (i == nullptr)
	{
		std::cerr << " In Concept::addIndividual(Individual*): argument is a null pointer. " << std::endl;
		return;
	}

	if (this->checkIndividuals(i))
	{
		throw std::logic_error(" Adding Individual: already exists ");
	}
	else
	{
		individuals.push_back(i);
		i->addConcept(this);
		for(auto it = Onthology::getInstance().subsGraph.begin(); it != Onthology::getInstance().subsGraph.end(); it++)
		{
			if(it->first->name.compare(this->name) == 0){
				it->second->addIndividual(i);
			}
		}
	}
}

void DL::Concept::addIndividual (string& s)
{
	DL::Individual* i = &DL::Onthology::getInstance().get_i(s);
	
	if (i == nullptr)
	{
		std::cerr << " In Concept::addIndividual(std::string&): get_i() returned a null pointer. " << std::endl;
		return;
	}

	if (this->checkIndividuals(i))
	{
		throw std::logic_error(" Adding Individual: already exists ");
	}
	else
	{
		individuals.push_back(i);
		i->addConcept(this);
		for(auto it = Onthology::getInstance().subsGraph.begin(); it != Onthology::getInstance().subsGraph.end(); it++)
		{
			if(it->first->name.compare(this->name) == 0){
				it->second->addIndividual(s);
			}

		}
	}
}

bool DL::Concept::checkIndividuals (const DL::Individual* indv) const
{
	if (indv == nullptr)
	{
		std::cerr << " In Concept::checkIndividuals(Individual*): argument is a null pointer. " << std::endl;
		exit(EXIT_FAILURE);
	}

	auto res = std::find(this->individuals.begin(), this->individuals.end(), indv);

	if (res != this->individuals.end())	// Individuo già presente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

// subsume = true -> subsumeS; subsume = false -> subsumeD.
bool DL::Concept::checkSubs (const DL::Concept* c, const bool subsume) const
{
	if (c == nullptr)
	{
		std::cerr << " In Concept::checkSubs(): argument is a null pointer. " << std::endl;
		exit(EXIT_FAILURE);
	}

	// subsume = true -> subsumeS; subsume = false -> subsumeD.
	if (subsume)
	{
		auto res = std::find(this->subsumes.begin(), this->subsumes.end(), c);

		if (res != this->subsumes.end())	// Già presente.
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		auto res = std::find(this->subsumed.begin(), this->subsumed.end(), c);

		if (res != this->subsumed.end())	// Già presente.
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}