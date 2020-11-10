#pragma once
#include <string>
#include <vector>
#include <map>

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
		
		std::vector<Concept>& thing = AllConcepts;
		std::vector<Concept> nothing;

		static Onthology& getInstance ()
		{
			static Onthology instance;
			return instance;
		}

		void put (Concept);
		void put (Role);
		void put (Individual);
		void put_c (std::string);
		void put_r (std::string);
		void put_i (std::string);

	private:
		Onthology () {}
		Onthology (Onthology const&) = delete;
		void operator = (Onthology const&) = delete;
	};

	class Individual
	{
	private:
		std::string name;
		std::vector<Concept*> concepts;
		std::vector<Role*> roles;

	public:
		Individual () {};
		Individual (std::string);

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
		Role () {};
		Role (std::string);

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
		Concept () {};
		Concept (std::string);

		std::string getName () const;
		std::vector<Individual*> getIndividuals () const;
		void addIndividual (Individual*);
	};
}