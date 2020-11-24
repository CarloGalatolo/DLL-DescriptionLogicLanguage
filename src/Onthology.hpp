#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>
#define SUBSUMES true
#define SUBSUMED false
using std::string;

namespace DL
{
	template<class K, class V>
		typename std::multimap<K, V>::const_iterator find_pair(const std::multimap<K, V>& map, const std::pair<K, V>& pair);

	class Onthology;
	class Individual;
	class Role;
	class Concept;

	// Singleton class
	class Onthology
	{
	public:
		std::vector<Individual> allIndividuals;	// Same as THING
		std::vector<Role> allRoles;
		std::vector<Concept> allConcepts;
		std::vector<std::string> allNames;

		std::multimap<Concept*, Concept*> subsGraph; // Grafo delle sussunzioni
		std::map<Concept*, Concept*> negateGrahp;	 // Grafo delle negazioni

		static Onthology& getInstance ()
		{
			static Onthology instance;
			return instance;
		}
	
		void put (Concept&);	// throws exception
		void put (Role&);		// throws exception
		void put (Individual&);	// throws exception
		void put_c (std::string&);	// throws exception
		void put_r (std::string&);	// throws exception
		void put_i (std::string&);	// throws exception
		Concept&    get_c (std::string&);
		Role& 	    get_r (std::string&);
		Individual& get_i (std::string&);
		void 		subsumption (std::string&, std::string&);
		std::string disjunction (std::string&, std::string&);
		std::string conjunction (std::string&, std::string&);

	private:
		Onthology () {}
		Onthology (Onthology const&) = delete;
		~Onthology () {}
		void operator = (Onthology const&) = delete;
		bool checkNames (const std::string&) const;
		template<class InputIterator, class T>
  			InputIterator myFind (InputIterator first, InputIterator last, const T& val) const;
		bool checkConcepts (const std::string&) const;
		bool checkRoles (const std::string&) const;
		bool checkIndividuals (const std::string&) const;
	};

	class Individual
	{
	private:
		std::string name;
		std::vector<Concept*> concepts;
		std::vector<Role*> roles;

	public:
		Individual () {}
		Individual (std::string&);
		~Individual () {}

		void addConcept (Concept*);
		void addRole (Role*);
		std::string getName () const;
		std::vector<Concept*> getConcepts () const;
		std::vector<Role*> getRoles () const;
	};

	class Role
	{
	private:
		std::string name;
		std::multimap<Individual*, Individual*> pairs;

	public:
		Role () {}
		Role (std::string&);
		~Role () {}

		std::string getName () const;
		std::multimap<Individual*, Individual*> getPairs () const;
		void insert (string&, string&); // throws exception
	};

	class Concept
	{
	private:
		std::string name;
		std::vector<Individual*> individuals;
		std::vector<Concept*> subsumes; // Concepts that this concept subsumes.
		std::vector<Concept*> subsumed;	// Concepts that subsume this concept.

	public:
		Concept () {}
		Concept (std::string&);
		~Concept () {}

		std::string getName () const;
		std::vector<Individual*> getIndividuals () const;
		std::vector<Concept*>    getSubsumes () const;
		std::vector<Concept*>    getSubsumed () const;

		void addIndividual (Individual*);	// throws exception
		void addIndividual (string&);		// throws exception
		//void addSubsumes (Concept*);	// throws exception
		//void addSubsumed (Concept*);	// throws exception

	private:
		bool checkIndividuals (const Individual*) const;
		bool checkSubs (const Concept*, const bool) const;	// bool = true -> subsumes; bool = false -> subsumed.
	};
}