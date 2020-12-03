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

void DL::Onthology::subsumption (std::string& s1, std::string& s2) // a subsumed by b
{
	Onthology& ont = Onthology::getInstance();
	Concept& subsumed = ont.get_c(s1);
	Concept& subsumes = ont.get_c(s2);
	std::vector<string> individualsInTheSubsumedConcept = subsumed.getIndividuals();

	std::pair<string, string> pair = std::make_pair(s1, s2);

	if (find_pair(subsGraph, pair) == subsGraph.end()) // Not there.
	{
		this->subsGraph.insert(pair);
		// All the individuals of the subsumed concept are put into the subsuming concept as well.
		for (string ind : individualsInTheSubsumedConcept)
		{
			subsumes.addIndividual(ind);
		}
	}
	else
	{
		throw std::logic_error(" Subsuming: pair already exists. ");
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
		for (string i1 : c1Indvs)
		{
			for (string i2 : c2Indvs)
			{
				//if (!(i1 == nullptr || i2 == nullptr))
				{
					if (i1.compare(i2) == 0)
					{
						try
						{
							c.addIndividual(i1);
							//std::cout << c.getIndividuals().at(0) << std::endl;
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

string DL::Onthology::negation (string& s, bool flag)
{
	/**
	 * Genera il negato del conetto il cui nome è "s" e lo inserisce nell'Ontologia,
	 * poi inserisce una pair nella mappa dei negati. Se "s" ha già un negato nell'Ontologia,
	 * la funzione ritorna il nome quel concetto invece di crearne un duplicato.
	 */

	Onthology& ont = Onthology::getInstance();
	ont.get_c(s);	// Verifico che il concetto di nome "s" esista. Per tutto il resto ripetere questa chiamata di get_c().
	string name;
	bool wasInCatch = false;

	if (flag==false)
	{
		name = "NOT_" + s;
	}
	else 
	{
 		name = "NOT_[" + s +"]";
	}
	
	try 
	{
		ont.put_c(name);
	}
	catch (std::logic_error e) // Negated concept already there.
	{
		std::cout << "Sono dentro la catch" << std::endl;

		// Cerca nella mappa dei negati se esiste già una corrispondenza.
		for (std::pair<string, string> pair : ont.negateMap)
		{
			std::cout << "Sto ciclando per cercare il negato misterioso" << std::endl;
			std::cout << pair.first << " " << pair.second << std::endl;

			// Corrispondenza trovata come primo o secondo elemento.
			if (pair.first.compare(s) == 0)	
			{
				std::cout << "IF" << std::endl;
				name = pair.second;	
			}
			if (pair.second.compare(s) == 0)
			{
				std::cout << "ELSEIF" << std::endl;
				name = pair.first;
			}
		}
		// Non c'è corrispondenza nella mappa dei negati.
		std::cout << "Esco dal for" << std::endl;

		ont.negateMap.insert(std::make_pair(name, s));
		wasInCatch = true;
	}

	if (!wasInCatch && !ont.negateMap.empty())
	{
		// Ciclo della mappa dei negati per controllare se il negato esiste già.
		for (std::pair<string, string> pair : ont.negateMap)
		{
			std::cout << "Sto ciclando per evitare i NOT_NOT_etc..." << std::endl;

			if (pair.first == s)	
			{
				// Se il parametro ha già un negato, ritorna il suo negato.
				name = pair.second;	
			}
			else if (pair.second == s)
			{
				// Se il parametro è il negato di un concetto esistente, ritorna il concetto originale.
				name = pair.first;
			}
		}		
	}
	else
	{
		std::cout << "La mappa dei negati è vuota." << std::endl;
	}
	
	// Se non è stato trovata una coppia nella mappa dei negati, viene generato il negato.

	std::cout << "Creo il negato di " << s << " : " << name << std::endl;
	bool check = false;
	DL::Concept& c_2 = ont.get_c(name);
	std::cout << "Recupero gli individui di " << ont.get_c(s).getName() << std::endl;
	std::vector<string> cIndvs = ont.get_c(s).getIndividuals();
	std::cout << "Negato creato." << std::endl;
	if (ont.negateMap.find(s) == ont.negateMap.end())
	{
		std::cout << "Sto per inserire nella mappa dei negati " << ont.get_c(s).getName() << " e " << c_2.getName() << std::endl;
		std::pair<string, string> negPair = std::make_pair(s, name);
		ont.negateMap.insert(negPair);
	}
	
	if (cIndvs.empty()) // Se il concetto è vuoto, il suo negato ha tutti gli individui.
	{
		for (DL::Individual ind : ont.allIndividuals)
		{
			std::cout << "Provo a recuperare il concetto" << std::endl;
			string tmp = ind.getName();
			ont.get_c(name).addIndividual(tmp);
		}
	}
	else
	{
		for (DL::Individual indv : ont.allIndividuals)
		{
			std::cout << "Ciclo l'individuo " << indv.getName() <<  std::endl;
			for (string idual : cIndvs)
			{
				std::cout << "Ciclo gli individui di " << s << std::endl;
				if (idual.compare(indv.getName()) == 0)
				{
					check = true;
					break;
				}
			}

			if (!check)
			{
				std::cout << "Provo a recuperare il concetto" << std::endl;
				string tmp = indv.getName();
				ont.get_c(name).addIndividual(tmp);
			}

			check = false;
		}		
	}

	return name;
}

string DL::Onthology::universal (std::string& r, std::string& c)
{
	/**
	 * Quantificatore universale.
	 * Controlla tutti gli indivdui del dominio in cerca di quelli la cui totalità
	 * di ruoli di cui sono soggetti sono ruoli di tipo "r" e verso individui
	 * appartenenti al concetto "c". Gli individui trovati vanno a formare un nuovo concetto.
	 * 
	 * Questa funzione esclude il seguente caso, che nella teoria è corretto
	 * ma è scomodo nelle applicazioni pratiche:
	 * 		Se un individuo non è soggetto di nessun ruolo, va considerato.
	 * 		Dimostrazione:
	 * 			∀r.C == ¬∃r.¬C
	 * 		Cioè l'individuo non è soggetto di ruoli verso ¬C,
	 * 		che è vero perché non è soggetto di nessun ruolo.
	 */

	Onthology& ont = Onthology::getInstance();
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
			std::multimap<std::string, std::string> pairList = role.getPairs();

			if ( !pairList.empty() )
			{
				for ( auto it = pairList.begin(); it != pairList.end(); it++/*std::pair<Individual*, Individual*> p : pairList*/ )
				{
					if ( ind.getName().compare(it->first) == 0 )
					{
						std::vector<std::string> concList = ont.get_i(it->second).getConcepts();

						if ( !concList.empty() )
						{
							for (string itr : concList)
							{
								if (itr.compare(c) != 0)
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
		//std::cout << "Sto per ciclare gli individui di ontologia" << std::endl;	
		for (DL::Individual& ind : ont.allIndividuals)
		{
			//std::cout << "Controllo " << ind.getName() << std::endl;
			//std::cout << "Sto per ciclare le coppie di " << r->getName() << std::endl;
			std::multimap<string, string> pairList = r->getPairs();
			if(!pairList.empty()){
				for (std::pair<string, string> p : pairList)
				{	
					//std::cout << "Controllo che " << p.first << " coincida con " << ind.getName() << std::endl;
					if(ind.getName().compare(p.first) == 0){
						//std::cout << "Coincidono!" << std::endl;
						std::vector<std::string> pCons = ont.get_i(p.second).getConcepts();
						if (!pCons.empty())
						{
							//std::cout << "Cerco " << concept << " tra i concetti di " << p.second << std::endl;
							for( string c : pCons ){
								//if(c != nullptr)
								{
									//std::cout << "Confronto " << c << " con " << concept << std::endl;
									if (c.compare(concept) == 0)	// Se lo trova
									{
										//std::cout << "Trovato! Aggiungo " << ind.getName() << " al concetto " << exist << std::endl;
										string save = ind.getName();
										ont.get_c(exist).addIndividual(save);
										break;
									}
									else
									{
										//std::cout << "Non esite una corrispondenza" << std::endl;
										break;
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

template<class InputIterator>
InputIterator DL::Onthology::myFind (InputIterator first, InputIterator last, const string& val) const
{
	while (first!=last) {
		if ((*first).getName()==val) return first;
		++first;
	}
	return last;
}

template<class InputIterator>
InputIterator DL::Onthology::myFindPtr (InputIterator first, InputIterator last, const string& val) const
{
	while (first!=last) {
		if ((*first)->getName()==val) return first;
		++first;
	}
	return last;
}

template<class InputIterator>
InputIterator DL::Onthology::myFindStr (InputIterator first, InputIterator last, const string& val) const
{
	while (first!=last) {
		if ((*first)==val) return first;
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
	if (s.substr(0, 10) == "NOT_[NOT_[")
	{
		s.erase(0, 10);
		s.erase(s.size()-2,2);
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
	if (Onthology::getInstance().myFindStr(concepts.begin(), concepts.end(), con) == concepts.end())
	{
		concepts.push_back(con);
	}
}

void DL::Individual::addRole (string& role)
{
	if (Onthology::getInstance().myFindStr(roles.begin(), roles.end(), role) == roles.end())
	{
		roles.push_back(role);
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
	Onthology& ont = Onthology::getInstance();

	if(!ont.checkIndividuals(s1) || !ont.checkIndividuals(s2)){
		exit(1);
	}

	std::pair<string, string> p = std::make_pair(s1,s2);

	if (find_pair(pairs, p) == pairs.end())
	{
		ont.get_i(s1).addRole(this->name);
		pairs.insert(p);
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

	if (!this->checkIndividuals(s))
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
{/*
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
	}*/
	return true;
}