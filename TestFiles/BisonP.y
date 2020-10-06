%{	
	#include <stdio.h> 
	#include <string.h> 	
%}

%union 
{
        char *str;
}


%token <str> NOME 
%token <str> DIM 
%token <str> NUM 
%token <str> TYPE
%token <str> DATA
%token C R A D S DA W L DB TAB EXIT TYPE
%token EOL

%type <str> field 
%type <str> exp

%%
DDL1: C NOME DB EOL{		creaDB($2);}
|C NOME TAB EOL {	creaTabella($2);
						aggiungiValori($2,$5);
				}
|C NOME TAB field EOL {	creaTabella($2);
						aggiungiValori($2,$5);
				}
;

field: field NAME YTPE  {}
| field NAME TYPE DIM  {}
| NAME TYPE {}
| NAME TYPE DIM {}


DDL2: R NOME DB {}
| R NOME TAB {}


DML1: A NOME DATA EOL{}
|A NOME EOL{}



DML2: D NOME W exp {}
| D NOME


%type <str> field 
/*field: NOME TYPE EOL field {printf("ciao");}
| NOME TYPE DIM EOL field {printf("come");}
| EXIT EOL {printf("franco");}
| {printf("recall info function");}*/


%%
