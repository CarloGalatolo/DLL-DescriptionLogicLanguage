#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>
using std::string;

template<class K, class V>
	typename std::multimap<K, V>::const_iterator find_pair(const std::multimap<K, V>& map, const std::pair<K, V>& pair);

namespace DL
{
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

		std::multimap<string, string> subsGraph; // Grafo delle sussunzioni
		std::map<string, string> coincidenceMap; // Mappa delle coincidenze
		std::map<string, string> negateMap;	 // Mappa delle negazioni

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
		void   subsumption (std::string&, std::string&);
		string conjunction (std::string&, std::string&);
		string disjunction (std::string&, std::string&);
		string negation (string&);
		string universal (std::string&, std::string&);
		string existential (string&, string&);
		void addSubs (std::string&, std::string&);	// throws exception

	private:
		Onthology () {}
		Onthology (Onthology const&) = delete;
		~Onthology () {}
		void operator = (Onthology const&) = delete;
		bool checkNames (const std::string&) const;
		template<class InputIterator, class T>
  			InputIterator myFind (InputIterator first, InputIterator last, const T& val) const;
		template<class InputIterator, class T>
  			InputIterator myFindPtr (InputIterator first, InputIterator last, const T& val) const;
		bool checkConcepts (const std::string&) const;
		bool checkRoles (const std::string&) const;
		bool checkIndividuals (const std::string&) const;
		string correctDoubleNot (string&);
	};

	class Individual
	{
	private:
		std::string name;
		std::vector<string> concepts; // Concetti a cui l'individuo appartiene.
		std::vector<string> roles; // Ruoli di cui l'individuo è soggetto.

	public:
		Individual () {}
		Individual (std::string&);
		~Individual () {}

		void addConcept (string&);
		void addRole (string&);
		std::string getName () const;
		std::vector<string> getConcepts () const;
		std::vector<string> getRoles () const;
	};

	class Role
	{
	private:
		std::string name;
		std::multimap<string, string> pairs;

	public:
		Role () {}
		Role (std::string&);
		~Role () {}

		std::string getName () const;
		std::multimap<string, string> getPairs () const;
		void insert (string&, string&); // throws exception
	};

	class Concept
	{
	private:
		std::string name;
		std::vector<string> individuals; // Individui appartenenti al concetto.
		std::vector<string> subsumes; // Concepts that this concept subsumes.
		std::vector<string> subsumed;	// Concepts that subsume this concept.

	public:
		Concept () {}
		Concept (std::string&);
		~Concept () {}

		std::string getName () const;
		std::vector<string> getIndividuals () const;
		std::vector<string>    getSubsumes () const;
		std::vector<string>    getSubsumed () const;

		void addIndividual (string&);		// throws exception

	private:
		bool checkIndividuals (const std::string) const;
		bool checkSubs (const Concept*, const bool) const;	// bool = true -> subsumes; bool = false -> subsumed.
	};
}
