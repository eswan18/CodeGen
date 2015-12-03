%{
	#include "token.h"
	#include "parser.tab.h"
	void convertString(char *string);
	void simplifyString(char *string);
%}
DIGIT [0-9]
LETTER [a-zA-Z]
LEGALSTRING [^\n\t\r\\\"]|(\\[^\n\t\r])|(\\\")
LEGALCOMMENT [^\*\\]|(\\\*)|(\\[^\*])
%%
"/*"{LEGALCOMMENT}*"*/" { /*return TOKEN_COMMENT;*/ }
"//".*\n { /*return TOKEN_COMMENT;*/ }
print { return TOKEN_PRINT; }
function { return TOKEN_FUNCTION; }
for { return TOKEN_FOR; }
if { return TOKEN_IF; }
else { return TOKEN_ELSE; }
return { return TOKEN_RETURN; }
"{" { return TOKEN_LEFT_BRACE; }
"}" { return TOKEN_RIGHT_BRACE; }
\( { return TOKEN_LEFT_PAREN; }
\) { return TOKEN_RIGHT_PAREN; }
"[" { return TOKEN_LEFT_BRACKET; }
"]" { return TOKEN_RIGHT_BRACKET; }
string { return TOKEN_STRING; }
integer { return TOKEN_INTEGER; }
char { return TOKEN_CHAR; }
void { return TOKEN_VOID; }
boolean { return TOKEN_BOOLEAN; }
array { return TOKEN_ARRAY; }
">=" { return TOKEN_GE; }
"<=" { return TOKEN_LE; }
"==" { return TOKEN_EQ; }
"!=" { return TOKEN_NE; }
"<" { return TOKEN_LT; }
">" { return TOKEN_GT; }
"&&" { return TOKEN_AND; }
"||" { return TOKEN_OR; }
"!" { return TOKEN_NOT; }
"++" { return TOKEN_INCREMENT; }
"--" { return TOKEN_DECREMENT; }
"+" { return TOKEN_ADD; }
"-" { return TOKEN_SUBTRACT; }
"*" { return TOKEN_MULTIPLY; }
"/" { return TOKEN_DIVIDE; }
"%" { return TOKEN_MODULUS; }
"^" { return TOKEN_EXPONENTIATE; }
"=" { return TOKEN_ASSIGN; }
, { return TOKEN_COMMA; }
: { return TOKEN_COLON; }
; { return TOKEN_SEMICOLON; }
true { return TOKEN_TRUE; }
false { return TOKEN_FALSE; }
\'\\?.\' { return TOKEN_CHAR_LITERAL; }
{DIGIT}+ { return TOKEN_INTEGER_LITERAL; }
"\"\"" { return TOKEN_STRING_LITERAL; }
\"{LEGALSTRING}+\" { 
	if (strlen(yytext) > 255 ) {
		fprintf(stderr,"scan error: %s is too long to be a valid string literal.\n",yytext);
		exit(1);
	}
	return TOKEN_STRING_LITERAL;
}
({LETTER}|_)({LETTER}|{DIGIT}|_)*	{ 
	if (strlen(yytext) > 255 ) {
		fprintf(stderr,"scan error: %s is too long to be a valid identifier.\n",yytext);
		exit(1);
	}
	return TOKEN_IDENTIFIER;
}
[ \t\n\r] {}
. {
	fprintf(stderr,"scan error: %s is not a valid symbol.\n",yytext);
	exit(1);
}

%%

int yywrap() {
	return(1);
}

void convertString (char *string) {
	/* Remove first and last character */
	int i;
	int len = strlen(string) - 1;
	string[len] = '\0';
	for ( i = 0; i < len; i++ ) {
		string[i] = string[i+1];
	}

	/*Remove escaped characters*/
	simplifyString(string);
	if ( strlen(string) > 255 ) {
		fprintf(stderr,"scan error: %s is too long to be a valid string.\n", yytext);
	}
	return;
}

/* Remove escaped characters */
void simplifyString (char *string) {
	int i, j;
	for ( i = 0; i < strlen(string); i++ ) {
		if ( string[i] == '\\' ) {
			char c = string [ i + 1];
			switch (c) {
				case 'n':
					string[i] = '\n';
					break;
				case '0':
					string[i] = '\0';
					break;
				default:
					string[i] = c;
			}
			/* Shift all later letters left */
			int newlen = strlen(string);
			for ( j = i + 1; j < newlen; j++) {
				string[j] = string[j+1];
			}
			
		}
	}
}
