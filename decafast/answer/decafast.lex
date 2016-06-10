
%{

#include "decafast-defs.h"
#include "decafast.tab.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int lineno = 1;
int tokenpos = 1;


%}

ALL_CHAR     [\a\b[:print:][:space:]]
CHAR_CHAR    [\a\b[:print:][:space:]]{-}[\'\n\\]
STRING_CHAR  [\a\b[:print:][:space:]]{-}[\"\n\\]
CHAR_NO_NL   [\a\b[:print:][:space:]]{-}[\n]
ESC_CHAR     \\(n|r|t|v|f|a|b|\\|\'|\")
HEX_DIGIT    [0-9A-Fa-f]
LETTER       [a-zA-Z\_]
IDENTIFIER   {LETTER}({LETTER}|[0-9])*
COMMENT      \/\/{CHAR_NO_NL}*\n
DECIMAL_LIT  [0-9]+
HEX_LIT      0(x|X){HEX_DIGIT}+
INT_LIT      {HEX_LIT}|{DECIMAL_LIT}
CHAR_LIT     \'({CHAR_CHAR}|{ESC_CHAR})\'
STRING_LIT   \"({STRING_CHAR}|{ESC_CHAR})*\"
BAD_ESC_CHAR \\[^nrtvfab\\\'\"]
BAD_STRING   \"({STRING_CHAR}|{BAD_ESC_CHAR})*\"
NL_IN_STRING \"({STRING_CHAR}|{ESC_CHAR}|\n)*\"
OPEN_STRING  \"({STRING_CHAR}|{ESC_CHAR}|\n)*
BAD_CHAR     \'({CHAR_CHAR}|{ESC_CHAR})({CHAR_CHAR}|{ESC_CHAR})+\'
OPEN_CHAR    \'({CHAR_CHAR}|{ESC_CHAR})[^\']*
ZERO_CHAR    \'\'

%%
  /*
    Pattern definitions for all tokens 
  */

"&&"            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_AND; }
"="             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_ASSIGN; }
","             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_COMMA; }
"/"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_DIV; }
"."             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_DOT; }
"=="            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_EQ; }
">="            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_GEQ; }
">"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_GT; }
"{"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LCB; }
"<<"            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LEFTSHIFT; }
"<="            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LEQ; }
"("             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LPAREN; }
"["             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LSB; }
"<"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_LT; }
"-"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_MINUS; }
"%"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_MOD; }
"*"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_MULT; }
"!="            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_NEQ; }
"!"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_NOT; }
"||"            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_OR; }
"+"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_PLUS; }
"}"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_RCB; }
">>"            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_RIGHTSHIFT; }
")"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_RPAREN; }
"]"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_RSB; }
";"             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_SEMICOLON; }
bool            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_BOOLTYPE; }
break           { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_BREAK; }
continue        { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_CONTINUE; }
else            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_ELSE; }
extern          { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_EXTERN; }
false           { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_FALSE; }
for             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_FOR; }
func            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_FUNC; }
if              { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_IF; }
int             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_INTTYPE; }
null            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_NULL; }
package         { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_PACKAGE; }
return          { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_RETURN; }
string          { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_STRINGTYPE; }
true            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_TRUE; }
var             { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_VAR; }
void            { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_VOID; }
while           { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_WHILE; }
{CHAR_LIT}      { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_CHARCONSTANT; }
{INT_LIT}       { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_INTCONSTANT; }
{STRING_LIT}    { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_STRINGCONSTANT; }
[[:space:]]+    { string lexeme; lexeme.assign(yytext); for (string::iterator it = lexeme.begin(); it != lexeme.end(); ++it) { if (*it == '\n') { lineno++; tokenpos = 1; } else { tokenpos++; } } }
{IDENTIFIER}    { yylval.sval = new string(yytext); tokenpos = tokenpos + yylval.sval->length(); return T_ID; }
{COMMENT}       { yylval.sval = new string(yytext); lineno++; tokenpos=1; }
{BAD_STRING}    { cerr << "Error: unknown escape sequence in string constant" << endl; return -1; }
{NL_IN_STRING}  { cerr << "Error: newline in string constant" << endl; return -1; }
{OPEN_STRING}   { cerr << "Error: string constant is missing closing delimiter" << endl; return -1; }
{BAD_CHAR}      { cerr << "Error: char constant length is greater than one" << endl; return -1; }
{OPEN_CHAR}     { cerr << "Error: unterminated char constant" << endl; return -1; }
{ZERO_CHAR}     { cerr << "Error: char constant has zero width" << endl; return -1; }
.               { cerr << "Error: unexpected character in input" << endl; return -1; }

%%

int yyerror(const char *s) {
  cerr << lineno << ": " << s << " at char " << tokenpos << endl;
  return 1;
}
