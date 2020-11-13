#include <iostream>
#include "../src/Onthology.hpp"
using namespace std;
using namespace DL;

int main ()
{
	std::string pippo("pippo");
	Onthology::getInstance().put_c(pippo);
	try 
	{
		Onthology::getInstance().put_c(pippo);
	}
	catch (logic_error e)
	{
		cerr << e.what();
		exit(EXIT_FAILURE);
	}
}