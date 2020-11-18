#include <cctype>
#include <fstream>
#include <cassert>

#include "dl_driver.hpp"
#include "Onthology.hpp"

DL::DL_Driver::~DL_Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void DL::DL_Driver::parse( const char* const filename )
{
   /**
    * Remember, if you want to have checks in release mode
    * then this needs to be an if statement 
    */
   assert( filename != nullptr );
   std::ifstream in_file( filename );
   if( ! in_file.good() )
   {
       exit( EXIT_FAILURE );
   }
   parse_helper( in_file );
   return;
}

void
DL::DL_Driver::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return;
   }
   //else
   parse_helper( stream ); 
   return;
}

void DL::DL_Driver::parse_helper( std::istream &stream )
{
   delete(scanner);
   try
   {
      scanner = new DL::DL_Scanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   
   delete(parser); 
   try
   {
      parser = new DL::DL_Parser( (*scanner) /* scanner */, (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }

   const int accept = 0;
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!" << std::endl;
   }
   return;
}

void DL::DL_Driver::critical_error (const DL::DL_Parser::location_type& l, const std::string& err_message)
{
	std::cerr << "Error: " << err_message << std::endl;
	exit(EXIT_FAILURE);
}

void 
DL::DL_Driver::add_upper()
{ 
   uppercase++; 
   chars++; 
   words++; 
}

void 
DL::DL_Driver::add_lower()
{ 
   lowercase++; 
   chars++; 
   words++; 
}

void 
DL::DL_Driver::add_word( const std::string &word )
{
   words++; 
   chars += word.length();
   for(const char &c : word ){
      if( islower( c ) )
      { 
         lowercase++; 
      }
      else if ( isupper( c ) ) 
      { 
         uppercase++; 
      }
   }
}

void 
DL::DL_Driver::add_newline()
{ 
   lines++; 
   chars++; 
}

void 
DL::DL_Driver::add_char()
{ 
   chars++; 
}




std::ostream& 
DL::DL_Driver::print( std::ostream &stream )
{
   /** NOTE: Colors are defined as class variables w/in DL_Driver **/
   stream << red  << "Results: " << norm << "\n";
   stream << blue << "Uppercase: " << norm << uppercase << "\n";
   stream << blue << "Lowercase: " << norm << lowercase << "\n";
   stream << blue << "Lines: " << norm << lines << "\n";
   stream << blue << "Words: " << norm << words << "\n";
   stream << blue << "Characters: " << norm << chars << "\n";
   return(stream);
}
