#pragma once
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <exception>

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
		std::vector<Individual> AllIndividuals;
		std::vector<Role> AllRoles;
		std::vector<Concept> AllConcepts;
		std::vector<std::string> AllNames;
		
		std::vector<Concept>& thing = AllConcepts;
		std::vector<Concept> nothing;

		static Onthology& getInstance ()
		{
			static Onthology instance;
			return instance;
		}

		bool check_name (std::string&) const;
		Concept find_c (std::string) const;

		void put (Concept);
		void put (Role);
		void put (Individual);
		void put_c (std::string&);
		void put_r (std::string&);
		void put_i (std::string&);

	private:
		Onthology () {}
		Onthology (Onthology const&) = delete;
		~Onthology () {}
		void operator = (Onthology const&) = delete;
	};

	class Individual
	{
	private:
		std::string name;
		std::vector<Concept*> concepts;
		std::vector<Role*> roles;

	public:
		Individual () {}
		Individual (std::string);
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
		std::map<Individual*, Individual*> pairs;

	public:
		Role () {}
		Role (std::string);
		~Role () {}

		std::string getName () const;
		std::map<Individual*, Individual*> getPairs () const;
		void insert (Individual*, Individual*);
	};

	class Concept
	{
	private:
		std::string name;
		std::vector<Individual*> individuals;

	public:
		Concept () {}
		Concept (std::string);
		~Concept () {}

		std::string getName () const;
		std::vector<Individual*> getIndividuals () const;
		void addIndividual (Individual*);
	};
}