#include <iostream>
#include <string>
#include "Onthology.hpp"

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

DL::Role::Role (std::string name)
{
	this->name = name;
}

std::string DL::Role::getName () const
{
	return this->name;
}

std::map<DL::Individual*, DL::Individual*> DL::Role::getPairs () const
{
	return this->pairs;
}

void DL::Role::insert (DL::Individual* first, DL::Individual* second)
{
	pairs.insert(std::pair<DL::Individual*, DL::Individual*>(first, second));
}

DL::Concept::Concept (std::string name)
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

void DL::Concept::addIndividual (DL::Individual* i)
{
	individuals.push_back(i);
}