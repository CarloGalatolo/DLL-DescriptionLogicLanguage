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

/**
 * ========== CLASS ONTHOLOGY ==========
 */

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

void DL::Onthology::put_c (std::string& t)
{
	string s = correctDoubleNot(t);
	if (checkNames(s))	// Nome già esistente.
	{
		throw std::logic_error(string("Concept " + s + " already exists."));
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
	std::string name = s1 + "CON" + s2;
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
	std::string name = s1 + "DIS" + s2;
	Concept res(name);
	put(res);
	Concept &c1 = get_c(s1), &c2 = get_c(s2), &c = get_c(name);
	auto c1Indvs = c1.getIndividuals(), c2Indvs = c2.getIndividuals();

	subsumption(s1, name);
	subsumption(s2, name);

	if (!c1Indvs.empty())
	{
		for(auto it = c1Indvs.begin(); it != c1Indvs.end(); it++)
		{
			try
			{
				c.addIndividual(*it); // Checks for nullptr.
			}
			catch (std::exception e)
			{
				// No problem.
			}
		}
	}
	// else the disjunction has no individuals from the first concept.

	if (!c2Indvs.empty())
	{
		for(auto it = c1Indvs.begin(); it != c1Indvs.end(); it++){
			try
			{
				c.addIndividual(*it); // Checks for nullptr.
			}
			catch (std::exception e)
			{
				// No problem.
			}
		}	
	}
	// else the disjunction has no individuals from the second concept.

	return name;
}

string DL::Onthology::negation (string& s)
{
	/**
	 * Funzione che genera il negato del conetto il cui nome è passato a parametro,
	 * lo inserisce nell'Ontologia, poi inserisce una pair nella mappa dei negati.
	 * Se il concetto parametro ha già un negato nell'Ontologia, la funzione ritorna
	 * quel concetto invece di crearne un duplicato.
	 */

	auto& ont = DL::Onthology::getInstance();

	DL::Concept* c = &ont.get_c(s);		// Recupera il concetto il cui nome è passato a parametro.
	string neg = "NOT_" + c->getName();
	
	//std::cout << "Sono prima del try" << std::endl;

	try 
	{
		ont.put_c(neg);
	}
	catch (std::logic_error e)
	{
		//std::cout << "Sono dentro la catch" << std::endl;

		for (std::pair<DL::Concept *, DL::Concept *> p : ont.negateMap)
		{
			std::cout << "Sto ciclando per cercare il negato misterioso" << std::endl;

			if(p.first == nullptr || p.second == nullptr){
				std::cout << "Sono nullo" << std::endl;
				std::cout << p.first->getName() << " " << p.second->getName() << std::endl;
			}
			else{
				std::cout << p.first->getName() << " " << p.second->getName() << std::endl;
				if (p.first->getName().compare(s) == 0)	
				{
					std::cout << "IF" << std::endl;
					return p.second->getName();	
				}
				else if (p.second->getName().compare(s) == 0)
				{
					std::cout << "ELSEIF" << std::endl;
					return p.first->getName();
				}
			}

		}

		//std::cout << "Esco dal for" << std::endl;

		DL::Concept* n = &ont.get_c(s.erase(0, 4));
		ont.negateMap.insert(std::make_pair(n, c));
	}

	// Ciclo della mappa dei negati per controllare se il negato esiste già.
	for (std::pair<DL::Concept *, DL::Concept *> p : ont.negateMap)
	{
		//std::cout << "Sto ciclando per evitare i NOT_NOT_etc..." << std::endl;

		if (p.first->getName() == s)	
		{
			// Se il parametro ha già un negato, ritorna il suo negato.
			return p.second->getName();	
		}
		else if (p.second->getName() == s)
		{
			// Se il parametro è il negato di un concetto esistente, ritorna il concetto originale.
			return p.first->getName();
		}
	}

	// Se non è stato trovata una coppia nella mappa dei negati, viene generato il negato.

	std::vector<DL::Individual*> cIndvs = c->getIndividuals();
	
	bool check = false;
	DL::Concept* c_2 = &ont.get_c(neg);
	
	if (ont.negateMap.find(c) == ont.negateMap.end())
	{
		std::cout << "Sto per inserire nella mappa dei negati " << c->getName() << " e " << c_2->getName() << std::endl;
		std::pair<DL::Concept*,DL::Concept*> negPair = std::make_pair(c, c_2);
		ont.negateMap.insert(negPair);
	}
	
	if (cIndvs.empty())
	{
		for (DL::Individual& indv : ont.allIndividuals)
		{
			//std::cout << "Provo a recuperare il concetto" << std::endl;
			ont.get_c(neg).addIndividual(&indv);
		}
	}
	else
	{
		for (DL::Individual& indv : ont.allIndividuals)
		{
			//std::cout << "Ciclo l'individuo " << indv.getName() <<  std::endl;
			for (DL::Individual* idual : cIndvs)
			{
				if(idual == nullptr){
					//std::cout << "Sono nullo" << std::endl;
				}
				else{
					//std::cout << "Ciclo gli individui del complementare" << std::endl;
					if (idual->getName().compare(indv.getName()) == 0)
					{
						check = true;
						break;
					}
				}

			}

			if (!check)
			{
				//std::cout << "Provo a recuperare il concetto" << std::endl;
				ont.get_c(neg).addIndividual(&indv);
			}

			check = false;
		}		
	}

	return neg;
}

string DL::Onthology::universal (std::string& r, std::string& c)
{
	/**
	 * Quantificatore esistenziale.
	 * Controlla tutti gli individui del dominio in cerca di quelli che sono soggetti di
	 * almeno un ruolo "role" con individui appartenenti al concetto "concept".
	 * Gli individui così trovati vanno a formare un nuovo concetto.
	 */

	DL::Onthology& ont = DL::Onthology::getInstance();
	ont.get_c(c); // Controlla se il ruolo esiste. Se no, causa un errore critico. Non viene usata una reference al concetto effettivo.
    DL::Role& role = ont.get_r(r);;	// Controlla se il ruolo esiste. Se non esiste, causa un errore critico.
	std::string name = "UNIV" + r + "_" + c;
	
	try
    {
        ont.put_c(name);
    }
    catch(std::logic_error x)
    {
		std::cerr << x.what() << std::endl;
        return name;
    }

	bool found = false;

	for (DL::Individual& ind : ont.allIndividuals) // Itera ogni individuo del dominio.
	{
		if ( ind.getRoles().size() != 1 ) // "ind" non può essere soggetto di ruoli diversi o di nessun ruolo (v. sopra).
		{
			found = false;
			continue;
		}
		
		if ( ind.getRoles().at(0).compare(r) != 0 ) // L'unico ruolo in cui "ind" è soggetto non è quello cercato.
		{
			found = false;
			continue;				
		}
		else // "ind" è soggetto soltanto del ruolo cercato.
		{
			// Lista delle coppie di individui facenti parte del ruolo cercato.
			//std::multimap<std::string, std::string> pairList = role.getPairs();
			
			if ( !role.getPairs().empty() )
			{
				for ( auto it = role.getPairs().begin(); it != role.getPairs().end(); it++/*std::pair<Individual*, Individual*> p : pairList*/ )
				{
					if ( ind.getName().compare(it->first) == 0 )
					{
						//std::vector<std::string> concList = ont.get_i(it->second).getConcepts();
						
						if ( !ont.get_i(it->second).getConcepts().empty() )
						{
							for ( auto itr = ont.get_i(it->second).getConcepts().begin(); itr != ont.get_i(it->second).getConcepts().end(); itr++/*DL::Concept* conc : concList*/ )
							{
								if (itr->compare(c) != 0)
								{
									found = false;
								}
								else{
									found = true;
									break;
								}
							}							
						}

						if (found)
						{
							break;
						}
					}
				}
			}

			if(found){
				std::string indName = ind.getName();
				ont.get_c(name).addIndividual(indName);
				found = false;
			}
		}
	}

	return name;
}

string DL::Onthology::existential (string& role, string& concept)
{    
	/**
	 * Quantificatore esistenziale.
	 * Controlla tutti gli individui del dominio in cerca di quelli che sono soggetti di
	 * almeno un ruolo "role" con individui appartenenti al concetto "concept".
	 * Gli individui così trovati vanno a formare un nuovo concetto.
	 */
	
    DL::Onthology& ont = DL::Onthology::getInstance();
    ont.get_c(concept); // Controlla se il ruolo esiste. Se no, causa un errore critico. Non viene usata una reference al concetto effettivo.
    DL::Role* r = &ont.get_r(role);	// Controlla se il ruolo esiste. Se non esiste, causa un errore critico.
    string exist = "EXIST" +role+"_"+concept;

    try
    {
        ont.put_c(exist);
    }
    catch(std::logic_error x)
    {
		std::cerr << x.what() << std::endl;
        return exist;
    }
    
	if (!r->getPairs().empty())
	{
		std::cout << "Sto per ciclare gli individui di ontologia" << std::endl;	
		for (DL::Individual& ind : ont.allIndividuals)
		{
			std::cout << "Controllo " << ind.getName() << std::endl;
			std::cout << "Sto per ciclare le coppie di " << r->getName() << std::endl;
			std::multimap<DL::Individual *, DL::Individual *> pairList = r->getPairs();
			if(!pairList.empty()){
				for (std::pair<DL::Individual *, DL::Individual *> p : pairList)
				{

					if(p.first == nullptr){
						std::cout << "Sono vuoto" << std::endl;
					}
					else{
						std::cout << "Controllo che " << p.first->getName() << " coincida con " << ind.getName() << std::endl;
						if(ind.getName().compare(p.first->getName()) == 0){
							std::cout << "Coincidono!" << std::endl;
							std::vector<DL::Concept*> pCons = p.second->getConcepts();
							if (!pCons.empty())
							{
								std::cout << "Cerco " << concept << " tra i concetti di " << p.second->getName() << std::endl;
								for( DL::Concept* c : pCons ){
									if(c != nullptr){
										std::cout << "Confronto " << c->getName() << " con " << concept << std::endl;
										if (c->getName().compare(concept) == 0)	// Se lo trova
										{
											std::cout << "Trovato! Aggiungo " << ind.getName() << " al concetto " << exist << std::endl;
											ont.get_c(exist).addIndividual(&ind);
											break;
										}
										else{
											std::cout << "Non esite una corrispondenza" << std::endl;
											break;
										}									
									}
									else{
										std::cout << "Sono nullo" << std::endl;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return exist;
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

template<class InputIterator, class T>
InputIterator DL::Onthology::myFindPtr (InputIterator first, InputIterator last, const T& val) const
{
	while (first!=last) {
		if ((*first)->getName()==val) return first;
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

string DL::Onthology::correctDoubleNot (string& s)
{
	if (s.substr(0, 8) == "NOT_NOT_")
	{
		s.erase(0, 8);
		correctDoubleNot(s);
		std::cerr << "WARNING: concept name with double NOT is very unsafe; concept: " << s << std::endl;
	}
	return s;
}

/**
 *  ========== CLASS INDIVIDUAL ==========
 */

DL::Individual::Individual (std::string& name)
{
	this->name = name;
}

void DL::Individual::addConcept (string& con)
{
	if (Onthology::getInstance().myFind(concepts.begin(), concepts.end(), con) == concepts.end())
	{
		concepts.push_back(con);
	}
	else
	{
		// Already there.
	}
}

void DL::Individual::addRole (string& role)
{
	if (Onthology::getInstance().myFind(roles.begin(), roles.end(), role) == roles.end())
	{
		roles.push_back(role);
	}
	else
	{
		// Already there.
	}
}

std::string DL::Individual::getName () const
{
	return this->name;
}

std::vector<string> DL::Individual::getConcepts () const
{
	return this->concepts;
}

std::vector<string> DL::Individual::getRoles () const
{
	return this->roles;
}

/**
 *  ========== CLASS ROLE ==========
 */

DL::Role::Role (std::string& name)
{
	this->name = name;
}

std::string DL::Role::getName () const
{
	return this->name;
}

std::multimap<string, string> DL::Role::getPairs () const
{
	return this->pairs;
}

void DL::Role::insert (string& s1, string& s2)
{
	/*
	DL::Individual* first = &DL::Onthology::getInstance().get_i(s1);
	DL::Individual* second = &DL::Onthology::getInstance().get_i(s2);
*/
	if(!DL::Onthology::getInstance().checkIndividual(s1)||!DL::Onthology::getInstance().checkIndividual(s2)){
		exit(1);
	}

	std::pair<string, string> p = std::make_pair(s1,s2);

	if (find_pair(pairs, p) == pairs.end())
	{
		DL::Onthology::getInstance().get_i(s1).addRole(this->name);
		pairs.insert(p);
	}
	else
	{
		throw std::logic_error("In role assertion: already existing.");
	}
}

/**
 *  ========== CLASS CONCEPT ==========
 */

DL::Concept::Concept (std::string& name)
{
	this->name = name;
}

std::string DL::Concept::getName () const
{
	return this->name;
}

std::vector<std::string> DL::Concept::getIndividuals () const
{
	return this->individuals;
}

std::vector<string> DL::Concept::getSubsumes () const
{
	/*ATTENZIONE NON USARE CON GLI ITERATORI*/
	return (*this).subsumes;
}

std::vector<std::string> DL::Concept::getSubsumed () const
{
	/*ATTENZIONE NON USARE CON GLI ITERATORI*/
	return (*this).subsumed;
}

void DL::Concept::addIndividual (string& s)
{
	Onthology& ont = Onthology::getInstance();

	DL::Individual* i = &ont.get_i(s);
	
	if (i == nullptr)
	{
		std::cerr << " In Concept::addIndividual(std::string&): get_i() returned a null pointer. " << std::endl;
		return;
	}

	if (this->checkIndividuals(s))
	{
		throw std::logic_error(" Adding Individual: already exists ");
	}
	else
	{
		individuals.push_back(s);
		i->addConcept(this->name);
		for(auto it = ont.subsGraph.begin(); it != ont.subsGraph.end(); it++)
		{
			if(it->first.compare(this->name) == 0){
				ont.get_c(it->second).addIndividual(s);
			}
		}
	}
}

bool DL::Concept::checkIndividuals (const std::string s) const
{

	auto res = std::find(this->individuals.begin(), this->individuals.end(), s);

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