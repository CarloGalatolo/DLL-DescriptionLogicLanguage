%{
	#include <string>

	/* Implementation of yyFlexScanner */ 
	#include "src/dl_scanner.hpp"
	#undef  YY_DECL
	#define YY_DECL int DL::DL_Scanner::yylex( DL::DL_Parser::semantic_type * const lval, DL::DL_Parser::location_type *location )

	/* using "token" to make the returns for the tokens shorter to type */
	using token = DL::DL_Parser::token;

	/* define yyterminate as this instead of NULL */
	#define yyterminate() return( token::END )

	/* msvc2010 requires that we exclude this header file. */
	#define YY_NO_UNISTD_H

	/* update location on matching */
	#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

	int oldstate;	// Usato nei commenti.
	int comment_start = 0;
%}

%option nodefault
%option yyclass="DL::DL_Scanner"
%option noyywrap
%option c++
%option yylineno
%option case-insensitive

%x COMMENT



%%

%{  /* Code executed at the beginning of yylex */
	yylval = lval;
%}

	/* Changing section with "%%" */
	/* A line containing only "%%" and white spaces. */
^[ \t]*%%[ \t]*$ 	{ return token::SECTION; }
^[ \t]*.+[ \t]*%%[ \t]*$			{ fprintf(stderr, "Section line must contain only '%%' and no other chracters, not even comments.\n"); exit(EXIT_FAILURE); }
^[ \t]*%%[ \t]*.+[ \t]*$			{ fprintf(stderr, "Section line must contain only '%%' and no other chracters, not even comments.\n"); exit(EXIT_FAILURE); }
^[ \t]*.+[ \t]*%%[ \t]*.+[ \t]*$	{ fprintf(stderr, "Section line must contain only '%%' and no other chracters, not even comments.\n"); exit(EXIT_FAILURE); }

 /* Single character literal tokens */
	/* DL punctuation */
"."		{ return token::DOT; } // Used in restrictions. E.g.: EX role . Concept
":" |	// Used in assertions. E.g.: indv : Concept
"," |	// Separator. Used in role assertions. E.g.: (i1, i2) : role
";" |	// End of statenìment.
"!" |	// Complement of a concept. E.g.: !Concept
"=" |	// Alias operator, i.e. rename operator.
	/* Brackets */
"(" |
")" 	{ return yytext[0]; }
"=="	{ return token::COINCIDENCE; }

	/* Keywords */
ALL				  { return token::ALL; 	   }
CONCEPT			  { return token::CONCEPT; }
CONJ(UNCTION)?	  { return token::CONJ;    }
DISJ(UNCTION)?	  { return token::DISJ;    }
EX(ISTS)?		  { return token::EX;      }
(INDIVIDUAL|INDV) { return token::INDV;    }
ROLE			  { return token::ROLE;    }
SUBS(UMPTION)?	  { return token::SUBS;    }

	/* Names */
[a-zA-Z_][a-zA-Z0-9_]* { yylval->build<std::string>(yytext); return token::NAME; }

	/* C-style comments */
"/*"			 { comment_start = yylineno; oldstate = YY_START; BEGIN COMMENT;  }
<COMMENT>"*/"	 { BEGIN oldstate; }
<COMMENT>.|\n 	 ;
<COMMENT><<EOF>> { fprintf(stderr, "Comment at line %d unclosed before EOF.\n", comment_start); exit(EXIT_FAILURE); }

	/* Ignore empty spaces and comments. */
[ \t\n] |
"//".*  ;	// C++-style comment.

%%
