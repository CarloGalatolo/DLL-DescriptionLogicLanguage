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

void DL::Onthology::subsumption (std::string& a, std::string& b) // a subsumed by b
{
	Concept& c1 = get_c(a), c2 = get_c(b);
	//c1.addSubsumed(&c2);
}

std::string DL::Onthology::disjunction (std::string& s1, std::string& s2) // Unione
{
	Concept& c1 = get_c(s1), c2 = get_c(s2);
	std::string name = s1 + "DISJ" + s2;
	Concept res(name);
/*
	for(auto i = c1.getIndividuals().begin(); i != c1.getIndividuals().end(); i++){
		try
		{
			res.addIndividual((*i));
		}
		catch (std::exception e)
		{
			// No problem.
		}
	}

	for(auto i = c2.getIndividuals().begin(); i != c2.getIndividuals().end(); i++){
		try
		{
			res.addIndividual((*i));
		}
		catch (std::exception e)
		{
			// No problem.
		}
	}
*/
	std::cout << " subs " << std::endl;
	put(res);
	//c1.addSubsumed(&get_c(name));
	//c2.addSubsumed(&get_c(name));
	return name;
}

std::string DL::Onthology::conjunction (std::string& s1, std::string& s2) // Intersezione
{
	Concept& c1 = get_c(s1), c2 = get_c(s2);
	std::string name = s1 + "CONJ" + s2;
	Concept res(name);
/*
	for (size_t i = 0; i != c1.getIndividuals().size(); i++)
	{
		for (size_t j = 0; j != c2.getIndividuals().size(); j++)
		{
			if (c1.getIndividuals().at(i)->getName() == c2.getIndividuals().at(j)->getName())
			{
				try
				{
					res.addIndividual(c1.getIndividuals().at(i));
				}
				catch(const std::exception& e)
				{
					// No problem.
				}
			}
		}
	}
*/
	put(res);
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

// === CLASS INDIVIDUAL ===

DL::Individual::Individual (std::string name)
{
	this->name = name;
}

void DL::Individual::addConcept (DL::Concept* con)
{
	concepts.push_back(con);
}

void DL::Individual::addRole (DL::Role* role)
{
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

// === CLASS ROLE ===

DL::Role::Role (std::string name)
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
	DL::Individual i1 = DL::Onthology::getInstance().get_i(s1);
	DL::Individual i2 = DL::Onthology::getInstance().get_i(s2);

	DL::Individual* first = &i1;
	DL::Individual* second = &i2;

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
	std::cout << "Controllo se posso inserire l'individuo: " << i->getName() << std::endl;
	if (this->checkIndividuals(i))
	{
		throw std::logic_error("Individual already exists.");
	}
	else
	{
		std::cout << "Sto per inserire l'individuo: " << i->getName() << std::endl;
		this->individuals.push_back(i);
		i->addConcept(this);
		for(auto it = this->subsumes.begin(); it != this->subsumes.end(); it++)
		{
			(*it)->addIndividual(i);
		}
	}
}

void DL::Concept::addIndividual (string& s)
{
	std::cout << "Controllo se posso inserire l'individuo: " << s << std::endl;
	DL::Individual* i = &DL::Onthology::getInstance().get_i(s);
	
	if (this->checkIndividuals(i))
	{
		throw std::logic_error(" Adding Individual: already exists ");
	}
	else
	{
		std::cout << "Sto per inserire l'individuo: " << s << std::endl;
		individuals.push_back(i);
		i->addConcept(this);
		for(auto it = this->subsumes.begin(); it != this->subsumes.end(); it++)
		{
			(*it)->addIndividual(i);
		}
		std::cout << "Ho inserito l'individuo: " << s << std::endl;
		std::cout << this->getIndividuals().at(0)->getName() << std::endl;
	}
}

/*
void DL::Concept::addSubsumes (DL::Concept* c)
{
	/**
	 * Aggiunge un puntatore al vettore dei concetti contenuti da questo concetto.
	 *
	std::cout << " subsumes " << std::endl;
	if (!this->checkSubs(c, true))
	{
		this->subsumes.push_back(c);
		c->subsumed.push_back(this);

		for (auto i = this->subsumed.begin(); i != this->subsumed.end(); i++)
		{
			(*i)->addSubsumes(c);
			c->addSubsumed(*i);
		}
	}
}

void DL::Concept::addSubsumed (DL::Concept* c)
{
	/**
	 * Aggiunge un puntatore al vettore dei concetti che contengono questo concetto.
	 *
	std::cout << " subsumed " << std::endl;
	if (!this->checkSubs(c, false))
	{
		this->subsumed.push_back(c);
		c->subsumes.push_back(this);

		for (auto i = this->subsumes.begin(); i != this->subsumes.end(); i++)
		{
			(*i)->addSubsumed(c);
			c->addSubsumes(*i);
		}
	}
}
*/
bool DL::Concept::checkIndividuals (const DL::Individual* indv) const
{
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