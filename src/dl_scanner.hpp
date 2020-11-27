#ifndef __DLSCANNER_HPP__
#define __DLSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include "../parser.tab.hh"
#include "../location.hh"

namespace DL{

class DL_Scanner : public yyFlexLexer{
public:
   
   DL_Scanner(std::istream *in) : yyFlexLexer(in)
   {
      loc = new DL::DL_Parser::location_type();
   };
  
   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( DL::DL_Parser::semantic_type * const lval, 
              DL::DL_Parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc

	void yyerror (std::string s) {
		std::cerr << s << std::endl;
	}

private:
   /* yyval ptr */
   DL::DL_Parser::semantic_type *yylval = nullptr;
   /* location ptr */
   DL::DL_Parser::location_type *loc    = nullptr;
};

} /* end namespace DL */

#endif /* END __DLSCANNER_HPP__ */