#include "Ontology.hpp"
using std::string;

template<class K, class V>
typename std::multimap<K, V>::const_iterator find_pair(const std::multimap<K, V>& map, const std::pair<K, V>& pair)
{
    /**
	 * Individua e restituisce un iteratore alla specifica coppi chiave-valore @param pair cercata all'interno
	 * della multimap @param map.
	 */

	typedef typename std::multimap<K, V>::const_iterator it;
    std::pair<it,it> range = map.equal_range(pair.first);
    for (it p = range.first; p != range.second; ++p)
        if (p->second == pair.second)
            return p;
    return map.end();
}

/*
 * ========== CLASS Ontology ==========
 */

void DL::Ontology::put (DL::Concept& c)
{
	/**
	 * Funzione in overload che inserisce un oggetto @param c di tipo DL::Concept nell'Ontologia.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
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

void DL::Ontology::put (DL::Role& r)
{
	/**
	 * Funzione in overload che inserisce un oggetto @param r di tipo DL::Role nell'Ontologia.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
	
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

void DL::Ontology::put (DL::Individual& i)
{
	/**
	 * Funzione in overload che inserisce un oggetto @param i di tipo DL::Individual nell'Ontologia.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
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

void DL::Ontology::put_c (std::string& t)
{
	/**
	 * Crea un oggetto di tipo DL::Concept a partire dal nome @param t.
	 * Inoltre, effettua un controllo sulla presenza del doppio "not" nel nome inserito, per
	 * evitare l'inserimento di concetti ridondanti.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
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

void DL::Ontology::put_r (std::string& s)
{
	/**
	 * Crea un oggetto di tipo DL::Role a partire dal nome @param s.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
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

void DL::Ontology::put_i (std::string& s)
{
	/**
	 * Crea un oggetto di tipo DL::Individual a partire dal nome @param s.
	 * Se l'oggetto è già presente nell'Ontologia, lancia un'eccezione che va raccolta dalla
	 * funzione al livello superiore.
	 */
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

DL::Concept& DL::Ontology::get_c (std::string& s)
{
	/**
	 * Trova e restituisce l'oggetto di tipo DL::Concept di nome @param s.
	 * Se l'oggetto non è presente nell'Ontologia, è considerato un errore critico,
	 * dunque l'esecuzione del programma termina.
	 */
	
	auto res = myFind(allConcepts.begin(), allConcepts.end(), s);

	try
	{
		if (res == allConcepts.end()) throw std::runtime_error("concept doesn't exist: ");
	}
	catch (std::runtime_error e) // Critical error
	{
		std::cerr << "Error in getting a concept: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Concept& r = *res;
	return r;
}

DL::Role& DL::Ontology::get_r (std::string& s)
{
	/**
	 * Trova e restituisce l'oggetto di tipo DL::Role di nome @param s.
	 * Se l'oggetto non è presente nell'Ontologia, è considerato un errore critico,
	 * dunque l'esecuzione del programma termina.
	 */
	
	auto res = myFind(allRoles.begin(), allRoles.end(), s);
	
	try
	{
		if (res == allRoles.end()) throw std::runtime_error("role doesn't exist: ");
	}
	catch (std::runtime_error e)
	{
		std::cerr << "Error in getting a role: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Role& r = *res;
	return r;
}

DL::Individual& DL::Ontology::get_i (std::string& s)
{
	/**
	 * Trova e restituisce l'oggetto di tipo DL::Individual di nome @param s.
	 * Se l'oggetto non è presente nell'Ontologia, è considerato un errore critico,
	 * dunque l'esecuzione del programma termina.
	 */
	
	auto res = myFind(allIndividuals.begin(), allIndividuals.end(), s);
	
	try
	{
		if (res == allIndividuals.end()) throw std::runtime_error("individual doesn't exist: ");
	}
	catch (std::runtime_error e) // Critical error
	{
		std::cerr << "Error in getting an individual: " << e.what() << s << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Individual& r = *res;
	return r;
}

void DL::Ontology::alias (string& before, string& after)
{
	/**
	 * Operatore alias.
	 * Permette di fornire un alias a un concetto presente nell'Ontologia o creato
	 * da un operatore logico. Il concetto corrispondente a @param before sarà
	 * ridenominato come @param after.
	 */
	
	Ontology& ont = Ontology::getInstance();

	//std::cout << "\n\n\n\n\n\n" <<  "Devo sostituire " << before << " con " << after << "\n\n\n\n\n\n" << std::endl;

	if (!checkConcepts(after))
	{
		Concept& c = ont.get_c(before); // Controllo che il concetto esista.
		
		c.setName(after);
		
		// Aggiorno il vettore di tutti i nomi.
		//std::cout << "Aggiorno il vettore di tutti i nomi." << std::endl;
		for (string& s : allNames)
		{
			if (s == before)
			{
				s = after;
			}
		}

		// Aggiorno il grafo delle sussunzioni.
		//std::cout << "Aggiorno il grafo delle sussunzioni." << std::endl;
		size_t erase_counter = 0;
		if (!subsGraph.empty())
		{
			for (auto& pair : subsGraph)
			{
				if (pair.first == before)
				{
					auto tmp = std::pair<string,string>(pair.first, pair.second);
					//std::cout << "tmp" << std::endl;
					subsGraph.insert( std::pair<string,string>(after, pair.second) );
					//std::cout << "insert" << std::endl;
					erase_counter++;
				}
				if (pair.second == before)
				{
					pair.second = after;
				}
			}

			for (size_t i = 0; i < erase_counter; i++)
			{
				for (auto& pair : subsGraph)
				{
					auto tmp = std::pair<string,string>(before, pair.second);
					auto f = find_pair(subsGraph, tmp);
					//std::cout << "find_pair" << std::endl;
					if (f != subsGraph.end())
					{
						subsGraph.erase( f );
						//std::cout << "erase" << std::endl;
						break;
					}					
				}
			}
		}

		// Aggiorno la mappa dei negati.
		//std::cout << "Aggiorno la mappa dei negati." << std::endl;
		erase_counter = 0;
		if (!negateMap.empty())
		{
			//std::cout << "Sto per ciclare la mappa dei negati" << std::endl;
			for (auto& pair : negateMap)
			{
				//std::cout << "Verifico che " << pair.first << " sia uguale a " << before << std::endl;
				if (pair.first == before)
				{
					//std::cout << "Copincidono!" << std::endl;
					negateMap.insert( std::pair<string,string>(after, pair.second) );
					erase_counter++;
					
				}
				//std::cout << "Verifico che " << pair.second << " sia uguale a " << before << std::endl;
				if (pair.second == before)
				{
					//std::cout << "Coincidono!" << std::endl;
					pair.second = after;
				}
			}
			
			//std::cout << "Sto per eliminare le vecchie coppie" << std::endl;
			for (size_t i = 0; i < erase_counter; i++)
			{
				//std::cout << "EXTERMINATE!" << std::endl;
				auto f = negateMap.find(before);
				if (f != negateMap.end())
				{
					negateMap.erase( f );
				}	
			}
		}

		// Aggiorno tutti gli individui.
		//std::cout << "Aggiorno tutti gli individui." << std::endl;
		if (!ont.allIndividuals.empty())
		{
			//std::cout << "Sto per ciclare gli individui dell'ontologia" << std::endl;
			for (Individual& ind : ont.allIndividuals)	
			{
				if (!ind.getConcepts().empty())
				{
					for (string& c : ind.getConcepts())
					{
						//std::cout << "\nCiclo i concetti di " << ind.getName() << std::endl;
						//std::cout << "Confronto " << c << " e " << before << std::endl;
						if (c == before)
						{
							//std::cout << "Coincidono!" << std::endl;
							c = after;
						}
					}
				}
			}			
		}
	}
	else
	{
		std::cerr << "Can't create alias " << after << ": concept with that name already exists." << std::endl;
	}
	//std::cout << "Ho finito" << std::endl;
}

void DL::Ontology::subsumption (std::string& s1, std::string& s2) // s1 contenuto da s2
{
	/**
	 * Operatore di sussunzione di un concetto @param s1 in un altro concetto @param s2.
	 * Quando un concetto A è sussunto dal concetto B, tutti gli individui di A diventano
	 * parte anche del concetto B. Tutti gli oggetti vengono aggiornati di conseguenza,
	 * in modo che B abbia informazione sui nuovi individui al suo interno ed essi abbiano
	 * l'informazione sul nuovo concetto di cui fanno parte.
	 * Inoltre, viene aggiornato il grafo delle sussunzioni, indicando che A è sussunto da B,
	 * che tutti i concetti sussunti da A sono anche sussunti da B e che tutti i concetti
	 * che sussumono B sussumono anche A.
	 */
	
	Ontology& ont = Ontology::getInstance();
	Concept& subsumed = ont.get_c(s1);
	Concept& subsumes = ont.get_c(s2);
	std::vector<string> individualsInTheSubsumedConcept = subsumed.getIndividuals();

	std::pair<string, string> pair = std::make_pair(s1, s2);

	if (find_pair(subsGraph, pair) == subsGraph.end()) // La coppia non è presente nel grafo.
	{
		this->subsGraph.insert(pair);

		// Tutti i concetti contenuti da s1 sono anche contenuti da s2.
		std::vector<string> subsumedByS1, subsumingS2;
		for (auto pair : subsGraph)
		{			
			if (pair.second == s1 && pair.first != s2)
			{
				// s1 sussume un altro concetto => s2 deve sussumere quel concetto.
				subsumedByS1.push_back(pair.first);
				continue;
			}

			if (pair.first == s2 && pair.second != s1)
			{
				// s2 è sussunto da un altro concetto => s1 deve essere sussunto da quel concetto.
				subsumingS2.push_back(pair.second);
				continue;
			}
		}

		std::pair<string, string> pair;

		for (string s : subsumedByS1)
		{
			pair = std::make_pair(s, s2);

			if (find_pair(subsGraph, pair) == subsGraph.end())
			{
				subsGraph.insert(pair);
			}
		}

		for (string s : subsumingS2)
		{
			pair = std::make_pair(s1, s);

			if (find_pair(subsGraph, pair) == subsGraph.end())
			{
				subsGraph.insert(pair);
			}
		}

		// Tutti gli individui del concetto sussunto appartengono anche al concetto sussumente.
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

void DL::Ontology::coincidence (string& s1, string& s2)
{
	/**
	 * Operatore coincidenza di due concetti di nomi @param s1 e @param s2.
	 * Un concetto A coincide con il concetto B se e solo se sia A è sussunto da B
	 * sia B è sussunto da A.
	 * La funzione svolge l'operazione di doppia sussunzione tra i due concetti.
	 */
	
	Ontology& ont = Ontology::getInstance();

	ont.get_c(s2);

	try
	{
		ont.put_c(s2);
	}
	catch(const std::exception& e)
	{
		//std::cerr << "Warning: " << e.what() << std::endl;
	}

	ont.subsumption(s1, s2);
	ont.subsumption(s2, s1);
}

std::string DL::Ontology::conjunction (std::string& s1, std::string& s2) // Intersezione
{
	/**
	 * Operatore congiunzione di due concetti di nomi @param s1 e @param s2.
	 * La congiunzione di due concetti A e B restituisce un nuovo concetto, il cui nome sarà
	 * la composizione "s1CONs2", che conterrà tutti gli individui facenti parte contemporaneamente
	 * sia del concetto A che del concetto B.
	 * Inoltre, il grafo delle sussunzioni sarà aggiornato in modo tale che il concetto risultante
	 * dalla congiunzione sia sussunto sia da A che da B.
	 */
	
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

std::string DL::Ontology::disjunction (std::string& s1, std::string& s2) // Unione
{
	/**
	 * Operatore disgiunzione di due concetti di nomi @param s2 e @param s2.
	 * La disgiunzione di due concetti A e B restituisce un nuovo concetto, il cui nome è la
	 * composizione "s1DISs2", che contiene tutti gli individui contenuti indifferentemente da
	 * A oppure da B.
	 * Inoltre, l'operazione aggiorna il grafo delle sussunzioni in modo che il concetto
	 * risultante contenga sia il concetto A che B.
	 */
	
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

string DL::Ontology::negation (string& s, bool flag)
{
	/**
	 * Operatore negazione del concetto di nome @param s.
	 * La negazione di un concetto A restituisce un nuovo concetto, il cui nome è "NOT_s" se
	 * si tratta di un concetto semplice (indicato dal @param flag pari a false) oppure "NOT_[s]"
	 * si tratta di un concetto complesso (indicato dal @param flag pari a true), che contiene
	 * tutti gli individui non contenuti nel concetto A.
	 * Inoltre, aggiorna la mappa dei negati, inserendo una coppia chiave-valore che indica
	 * che il concetto appena creato è il negato del concetto indicato.
	 */
	
	Ontology& ont = Ontology::getInstance();
	ont.get_c(s);	// Verifico che il concetto di nome "s" esista. Per tutto il resto ripetere questa chiamata di get_c().
	string name;

	if (flag==false)
	{
		name = "NOT_" + s;
	}
	else 
	{
 		name = "NOT_[" + s + "]";
	}
	
	if (!ont.negateMap.empty())
	{
		// Ciclo della mappa dei negati per controllare se il negato esiste già.
		for (std::pair<string, string> pair : ont.negateMap)
		{
			//std::cout << "Sto ciclando per evitare i NOT_NOT_etc..." << std::endl;

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
		//std::cout << "La mappa dei negati è vuota." << std::endl;
	}

	try 
	{
		ont.put_c(name);
	}
	catch (std::logic_error e) // Negated concept already there.
	{
		//std::cout << "Sono dentro la catch" << std::endl;

		// Cerca nella mappa dei negati se esiste gConceptià una corrispondenza.
		for (std::pair<string, string> pair : ont.negateMap)
		{
			//std::cout << "Sto ciclando per cercare il negato misterioso" << std::endl;
			//std::cout << pair.first << " " << pair.second << std::endl;

			// Corrispondenza trovata come primo o secondo elemento.
			if (pair.first.compare(s) == 0)	
			{
				//std::cout << "IF" << std::endl;
				name = pair.second;
				ont.negateMap.insert(std::make_pair(s, name));
			}
			if (pair.second.compare(s) == 0)
			{
				//std::cout << "ELSEIF" << std::endl;
				name = pair.first;
				ont.negateMap.insert(std::make_pair(name, s));
			}
		}
		// Non c'è corrispondenza nella mappa dei negati.
		//std::cout << "Esco dal for" << std::endl;
	}

	// Se non è stato trovata una coppia nella mappa dei negati, viene generato il negato.

	//std::cout << "Il negato di " << s << " è " << name << std::endl;
	bool check = false;
	//std::cout << "Recupero gli individui di " << ont.get_c(s).getName() << std::endl;
	std::vector<string> cIndvs = ont.get_c(s).getIndividuals();
	//std::cout << "Negato creato." << std::endl;
	if (ont.negateMap.find(s) == ont.negateMap.end())
	{
		//std::cout << "Sto per inserire nella mappa dei negati " << s << " e " << name << std::endl;
		std::pair<string, string> negPair = std::make_pair(s, name);
		ont.negateMap.insert(negPair);
	}
	
	if (cIndvs.empty()) // Se il concetto è vuoto, il suo negato ha tutti gli individui.
	{
		for (DL::Individual ind : ont.allIndividuals)
		{
			//std::cout << "Provo a recuperare il concetto" << std::endl;
			string tmp = ind.getName();
			ont.get_c(name).addIndividual(tmp);
		}
	}
	else
	{
		for (DL::Individual indv : ont.allIndividuals)
		{
			//std::cout << "Ciclo l'individuo " << indv.getName() <<  std::endl;
			for (string idual : cIndvs)
			{
				//std::cout << "Ciclo gli individui di " << s << std::endl;
				if (idual.compare(indv.getName()) == 0)
				{
					check = true;
					break;
				}
			}

			if (!check)
			{
				//std::cout << "Provo a recuperare il concetto" << std::endl;
				//std::cout << "Aggiungo " << indv.getName() << " a " << name << std::endl;
				string tmp = indv.getName();
				ont.get_c(name).addIndividual(tmp);
			}

			check = false;
		}		
	}

	return name;
}

string DL::Ontology::universal (std::string& r, std::string& c)
{
	/**
	 * Quantificatore universale.
	 * Controlla tutti gli indivdui del dominio in cerca di quelli la cui totalità
	 * di ruoli di cui sono soggetti sono ruoli di tipo "r" e verso individui
	 * appartenenti al concetto "c". Gli individui trovati vanno a formare un nuovo concetto.
	 * 
	 * Questa funzione esclude il seguente caso, che nella teoria è corretto
	 * ma è scomodo nelle applicazioni pratiche:
	 * 		Se un individuo non è soggetto di nessun ruolo, va considerato in ogni
	 * 		applicazione del quantificatore universale.
	 * 		> Dimostrazione:
	 * 			∀r.C == ¬∃r.¬C
	 * 		Cioè l'individuo non è soggetto di ruoli verso ¬C,
	 * 		che è vero perché non è soggetto di nessun ruolo.
	 */

	Ontology& ont = Ontology::getInstance();
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

string DL::Ontology::existential (string& role, string& concept)
{    
	/**
	 * Quantificatore esistenziale.
	 * Controlla tutti gli individui del dominio in cerca di quelli che sono soggetti di
	 * almeno un ruolo "role" con individui appartenenti al concetto @param concept.
	 * Gli individui così trovati vanno a formare un nuovo concetto.
	 */
	
    DL::Ontology& ont = DL::Ontology::getInstance();
    ont.get_c(concept); // Controlla se il ruolo esiste. Se no, causa un errore critico. Non viene usata una reference al concetto effettivo.
    DL::Role* r = &ont.get_r(role);	// Controlla se il ruolo esiste. Se non esiste, causa un errore critico.
    string exist = "EXIST" + role + "_" + concept;

    try
    {
        ont.put_c(exist);
    }
    catch(std::logic_error x)
    {
		//std::cerr << x.what() << std::endl;
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
								//std::cout << "Confronto " << c << " con " << concept << std::endl;
								if (c.compare(concept) == 0)	// Se lo trova
								{
									//std::cout << "Trovato! Aggiungo " << ind.getName() << " al concetto " << exist << std::endl;
									string save = ind.getName();
									ont.get_c(exist).addIndividual(save);
									break;
								}
								/*else
								{
									std::cout << "Non esite una corrispondenza" << std::endl;
								}*/
							}
						}
					}
				}
			}
		}
	}
	return exist;
}

bool DL::Ontology::checkNames (const std::string& s) const
{
	/**
	 * Funzione che verifica la presenza del nome @param s nell'Ontologia.
	 */
	
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
InputIterator DL::Ontology::myFind (InputIterator first, InputIterator last, const string& val) const
{
	/**
	 * Replica della funzione std::find(), che cerca un valore corrispondente a @param val
	 * nella collezione compresa tra gli iteratori @param first e @param last.
	 */
	
	while (first!=last) {
		if ((*first).getName()==val) return first;
		++first;
	}
	return last;
}

template<class InputIterator>
InputIterator DL::Ontology::myFindPtr (InputIterator first, InputIterator last, const string& val) const
{
	/**
	 * Replica della funzione std::find(), che cerca un valore corrispondente a @param val
	 * nella collezione compresa tra gli iteratori @param first e @param last.
	 */

	while (first!=last) {
		if ((*first)->getName()==val) return first;
		++first;
	}
	return last;
}

template<class InputIterator>
InputIterator DL::Ontology::myFindStr (InputIterator first, InputIterator last, const string& val) const
{
	/**
	 * Replica della funzione std::find(), che cerca un valore corrispondente a @param val
	 * nella collezione compresa tra gli iteratori @param first e @param last.
	 */

	while (first!=last) {
		if ((*first)==val) return first;
		++first;
	}
	return last;
}

bool DL::Ontology::checkConcepts (const std::string& s) const
{
	/**
	 * Funzione che verifica la presenza del concetto di nome @param s nell'Ontologia.
	 */
	
	if (myFind(allConcepts.begin(), allConcepts.end(), s) != allConcepts.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DL::Ontology::checkRoles (const std::string& s) const
{
	/**
	 * Funzione che verifica la presenza del ruolo di nome @param s nell'Ontologia.
	 */
	
	if (myFind(allRoles.begin(), allRoles.end(), s) != allRoles.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DL::Ontology::checkIndividuals (const std::string& s) const
{
	/**
	 * Funzione che verifica la presenza dell'individuo di nome @param s nell'Ontologia.
	 */
	
	if (myFind(allIndividuals.begin(), allIndividuals.end(), s) != allIndividuals.end())	// Nome già esistente.
	{
		return true;
	}
	else
	{
		return false;
	}
}

string DL::Ontology::correctDoubleNot (string& s)
{
	/**
	 * Funzione che verifica e corregge i nomi di concetto che presentano la doppia
	 * dicitura "NOT" all'inizio, poiché possono causare situaizoni indesiderate e
	 * inserimenti di concetti ridondanti.
	 */
	
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

void DL::Ontology::validate ()
{
	/**
	 * Procedura di validazione della consistenza dell'interpretazione.
	 * Se viene rintracciata una contraddizione, viene indicato l'oggetto che la causa
	 * e viene interrotta l'esecuzione del programma.
	 */
	
	DL::Ontology& ont = DL::Ontology::getInstance();
	if(!ont.negateMap.empty())
	{
	
		for(std::pair<string, string> negPair : ont.negateMap)
		{	
			string c1Name = negPair.first;
			string c1NegName = negPair.second ;

			//std::cerr << "Controllo " << c1Name << " and "<< c1NegName << std::endl;
			
			DL::Concept c1 = ont.get_c(c1Name);
			DL::Concept nC1 = ont.get_c(c1NegName);

			for(auto c1Indvs : c1.getIndividuals())
			{	
				for(auto c1NegIndvs : nC1.getIndividuals())
				{
					//std::cerr << "Confronto " << c1Indvs << " di " << c1Name << " e " << c1NegIndvs << " di " << c1NegName << std::endl;
					if(c1Indvs == c1NegIndvs)
					{
							std::cerr << "ERROR: logic error between " << negPair.first << " and "<< negPair.second << std::endl;
							std::cerr << "At " << c1Indvs << std::endl;
							std::cerr << "Invalid Interpretation "<< std::endl;
							exit(1);
					}
				}
			} 
		}
	}
}

/*
 *  ========== CLASS INDIVIDUAL ==========
 */

// Costruttore della classe DL::Individual. Prende a parametro il nome dell'individuo.
DL::Individual::Individual (std::string& name) : name(name) {}

void DL::Individual::addConcept (string& con)
{
	/**
	 * Funzione che aggiunge un nome @param con alla lista dei concetti a cui appartiene
	 * l'individuo in questione, così che ogni individuo sappia sempre a quali concetti appartiene.
	 */
	
	if (Ontology::getInstance().myFindStr(concepts.begin(), concepts.end(), con) == concepts.end())
	{
		concepts.push_back(con);
	}
}

void DL::Individual::addRole (string& role)
{
	/**
	 * Funzione che aggiunge il nome di un ruolo @param role alla lista dei ruoli di cui
	 * l'individuo in questione è il soggetto (non colui che lo subisce).
	 */
	
	if (Ontology::getInstance().myFindStr(roles.begin(), roles.end(), role) == roles.end())
	{
		roles.push_back(role);
	}
}

std::string DL::Individual::getName () const
{
	/**
	 * Funzione getter dell'ttributo name.
	 */
	return this->name;
}

std::vector<string> DL::Individual::getConcepts () const
{
	/**
	 * Funzione getter dell'attributo concepts.
	 */

	return this->concepts;
}

std::vector<string> DL::Individual::getRoles () const
{
	/**
	 * Funzione getter dell'attiìributo roles.
	 */
	
	return this->roles;
}

/*
 *  ========== CLASS ROLE ==========
 */

// Costrutore della classe DL::Role. Prende a parametro il nome del ruolo.
DL::Role::Role (std::string& name) : name(name) {}

std::string DL::Role::getName () const
{
	/**
	 * Funzione getter dell'attributo name.
	 */
	
	return this->name;
}

std::multimap<string, string> DL::Role::getPairs () const
{
	/**
	 * Funzione getter dell'attributo pairs.
	 */
	
	return this->pairs;
}

void DL::Role::insert (string& s1, string& s2)
{
	/**
	 * Procedura di istanziazione di una coppia chiave-valore di individui di nome @param s1 e @param s2
	 * nel ruolo in questione. L'elemento chiave sarà l'individuo che è soggetto del ruolo, mentre
	 * l'elemento valore sarà l'individuo che subisce il ruolo.
	 */
	
	Ontology& ont = Ontology::getInstance();

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

/*
 *  ========== CLASS CONCEPT ==========
 */

// Costruttore della classe DL::Concept. Prende a parametro il nome del concetto.
DL::Concept::Concept (std::string& name) : name(name) {}

void DL::Concept::setName (string& name)
{
	/**
	 * Funzione setter dell'attributo name.
	 */
	
	this->name = name;
}

std::string DL::Concept::getName () const
{
	/**
	 * Funzione getter dell'attributo name.
	 */
	
	return this->name;
}

std::vector<std::string> DL::Concept::getIndividuals () const
{
	/**
	 * Funzione getter dell'attributo individuals.
	 */
	
	return this->individuals;
}

void DL::Concept::addIndividual (string& s)
{
	/**
	 * Procedura di inserimento di un individuo di nome @param s all'interno del concetto.
	 * La procedura si assicura che l'individuo non sia già presente nel concetto, e inoltre
	 * si chiama ricorsivamente per aggiungere l'individuo a tutti i concetti che sussumono
	 * il concetto che la chiama.
	 */
	
	Ontology& ont = Ontology::getInstance();

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
	/**
	 * Funzione che verifica se l'individuo di nome @param s appartiene al concetto.
	 */

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