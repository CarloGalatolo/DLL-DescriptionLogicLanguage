#include "Onthology.hpp"

// === CLASS ONTHOLOGY ===

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

void DL::Onthology::put (DL::Concept c)
{
	if (checkNames(c.getName()))	// Nome già esistente.
	{
		throw std::logic_error("Name already exists.");
	}
	else
	{
		allNames.push_back(c.getName());
		allConcepts.push_back(c);
	}
}

void DL::Onthology::put (DL::Role r)
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

void DL::Onthology::put (DL::Individual i)
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

// === CLASS INDIVIDUAL ===

DL::Individual::Individual (std::string name)
{
	this->name = name;
}

inline void DL::Individual::addConcept (DL::Concept* con)
{
	concepts.push_back(con);
}

inline void DL::Individual::addRole (DL::Role* role)
{
	roles.push_back(role);
}

inline std::string DL::Individual::getName () const
{
	return this->name;
}

inline std::vector<DL::Concept*> DL::Individual::getConcepts () const
{
	return this->concepts;
}

inline std::vector<DL::Role*> DL::Individual::getRoles () const
{
	return this->roles;
}

// === CLASS ROLE ===

DL::Role::Role (std::string name)
{
	this->name = name;
}

inline std::string DL::Role::getName () const
{
	return this->name;
}

inline std::multimap<DL::Individual*, DL::Individual*> DL::Role::getPairs () const
{
	return this->pairs;
}

inline void DL::Role::insert (DL::Individual* first, DL::Individual* second)
{
	pairs.insert(std::pair<DL::Individual*, DL::Individual*>(first, second));
}

// === CLASS CONCEPT ===

DL::Concept::Concept (std::string name)
{
	this->name = name;
}

inline std::string DL::Concept::getName () const
{
	return this->name;
}

inline std::vector<DL::Individual*> DL::Concept::getIndividuals () const
{
	return this->individuals;
}

void DL::Concept::addIndividual (DL::Individual* i)
{
	if (this->checkIndividuals(i))
	{
		throw std::logic_error("Individual already exists.");
	}
	else
	{
		individuals.push_back(i);
	}
}

void DL::Concept::addSubsumes (DL::Concept* c)
{
	/**
	 * Aggiunge un puntatore al vettore dei concetti contenuti da questo concetto.
	 */

	if (!this->checkSubs(c, true))
	{
		this->subsumes.push_back(c);

		for (auto i = this->subsumed.begin(); i != this->subsumed.end(); i++)
		{
			(*i)->addSubsumes(c);
		}
	}
}

void DL::Concept::addSubsumed (DL::Concept* c)
{
	/**
	 * Aggiunge un puntatore al vettore dei concetti che contengono questo concetto.
	 */

	if (!this->checkSubs(c, false))
	{
		this->subsumed.push_back(c);

		for (auto i = this->subsumes.begin(); i != this->subsumes.end(); i++)
		{
			(*i)->addSubsumed(c);
		}
	}
}

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