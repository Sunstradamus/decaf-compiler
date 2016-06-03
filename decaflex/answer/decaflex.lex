
%{

#include <iostream>
#include <cstdlib>

using namespace std;

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
MULT_COMMENT "/*"[^"*/"]*"*/"
DECIMAL_LIT  [0-9]+
HEX_LIT      0(x|X){HEX_DIGIT}+
INT_LIT      {HEX_LIT}|{DECIMAL_LIT}
CHAR_LIT     \'({CHAR_CHAR}|{ESC_CHAR})\'
STRING_LIT   \"({STRING_CHAR}|{ESC_CHAR})*\"

%%
  /*
    Pattern definitions for all tokens 
  */

"&&"            { return 1; }
"="             { return 2; }
","             { return 3; }
"/"             { return 4; }
"."             { return 5; }
"=="            { return 6; }
">="            { return 7; }
">"             { return 8; }
"{"             { return 9; }
"<<"            { return 10; }
"<="            { return 11; }
"("             { return 12; }
"["             { return 13; }
"<"             { return 14; }
"-"             { return 15; }
"%"             { return 16; }
"*"             { return 17; }
"!="            { return 18; }
"!"             { return 19; }
"||"            { return 20; }
"+"             { return 21; }
"}"             { return 22; }
">>"            { return 23; }
")"             { return 24; }
"]"             { return 25; }
";"             { return 26; }
bool            { return 27; }
break           { return 28; }
continue        { return 29; }
else            { return 30; }
extern          { return 31; }
false           { return 32; }
for             { return 33; }
func            { return 34; }
if              { return 35; }
int             { return 36; }
null            { return 37; }
package         { return 38; }
return          { return 39; }
string          { return 40; }
true            { return 41; }
var             { return 42; }
void            { return 43; }
while           { return 44; }
{CHAR_LIT}      { return 45; }
{INT_LIT}       { return 46; }
{STRING_LIT}    { return 47; }
[[:space:]]+    { return 48; }
{IDENTIFIER}    { return 49; }
{COMMENT}       { return 50; }
{MULT_COMMENT}  { return 51; }
.               { cerr << "Error: unexpected character in input" << endl; return -1; }

%%

int main () {
  int token;
  int line_count = 1;
  int char_location = 1;
  string lexeme;
  string new_line;
  while ((token = yylex())) {
    if (token > 0) {
      lexeme.assign(yytext);
      char_location += lexeme.length();
      switch(token) {
                case 1: cout << "T_AND " << lexeme << endl; break;
                case 2: cout << "T_ASSIGN " << lexeme << endl; break;
                case 3: cout << "T_COMMA " << lexeme << endl; break;
                case 4: cout << "T_DIV " << lexeme << endl; break;
                case 5: cout << "T_DOT " << lexeme << endl; break;
                case 6: cout << "T_EQ " << lexeme << endl; break;
                case 7: cout << "T_GEQ " << lexeme << endl; break;
                case 8: cout << "T_GT " << lexeme << endl; break;
                case 9: cout << "T_LCB " << lexeme << endl; break;
                case 10: cout << "T_LEFTSHIFT " << lexeme << endl; break;
                case 11: cout << "T_LEQ " << lexeme << endl; break;
                case 12: cout << "T_LPAREN " << lexeme << endl; break;
                case 13: cout << "T_LSB " << lexeme << endl; break;
                case 14: cout << "T_LT " << lexeme << endl; break;
                case 15: cout << "T_MINUS " << lexeme << endl; break;
                case 16: cout << "T_MOD " << lexeme << endl; break;
                case 17: cout << "T_MULT " << lexeme << endl; break;
                case 18: cout << "T_NEQ " << lexeme << endl; break;
                case 19: cout << "T_NOT " << lexeme << endl; break;
                case 20: cout << "T_OR " << lexeme << endl; break;
                case 21: cout << "T_PLUS " << lexeme << endl; break;
                case 22: cout << "T_RCB " << lexeme << endl; break;
                case 23: cout << "T_RIGHTSHIFT " << lexeme << endl; break;
                case 24: cout << "T_RPAREN " << lexeme << endl; break;
                case 25: cout << "T_RSB " << lexeme << endl; break;
                case 26: cout << "T_SEMICOLON " << lexeme << endl; break;
                case 27: cout << "T_BOOLTYPE " << lexeme << endl; break;
                case 28: cout << "T_BREAK " << lexeme << endl; break;
                case 29: cout << "T_CONTINUE " << lexeme << endl; break;
                case 30: cout << "T_ELSE " << lexeme << endl; break;
                case 31: cout << "T_EXTERN " << lexeme << endl; break;
                case 32: cout << "T_FALSE " << lexeme << endl; break;
                case 33: cout << "T_FOR " << lexeme << endl; break;
                case 34: cout << "T_FUNC " << lexeme << endl; break;
                case 35: cout << "T_IF " << lexeme << endl; break;
                case 36: cout << "T_INTTYPE " << lexeme << endl; break;
                case 37: cout << "T_NULL " << lexeme << endl; break;
                case 38: cout << "T_PACKAGE " << lexeme << endl; break;
                case 39: cout << "T_RETURN " << lexeme << endl; break;
                case 40: cout << "T_STRINGTYPE " << lexeme << endl; break;
                case 41: cout << "T_TRUE " << lexeme << endl; break;
                case 42: cout << "T_VAR " << lexeme << endl; break;
                case 43: cout << "T_VOID " << lexeme << endl; break;
                case 44: cout << "T_WHILE " << lexeme << endl; break;
                case 45: cout << "T_CHARCONSTANT " << lexeme << endl; break;
                case 46: cout << "T_INTCONSTANT " << lexeme << endl; break;
                case 47: cout << "T_STRINGCONSTANT " << lexeme << endl; break;
                case 48:
                  new_line = "";
                  // Subtract the string length and then add it char by char since it may contain a \n
                  char_location -= lexeme.length();
                  for (string::iterator it = lexeme.begin(); it != lexeme.end(); ++it) {
                    if (*it == '\n') {
                      line_count++;
                      char_location = 1;
                      new_line.push_back('\\');
                      new_line.push_back('n');
                    } else {
                      char_location++;
                      new_line.push_back(*it);
                    }
                  }
                  cout << "T_WHITESPACE " << new_line << endl;
                  break;
                case 49: cout << "T_ID " << lexeme << endl; break;
                case 50: 
                  new_line.assign(lexeme);
                  new_line.resize(new_line.size()-1);
                  new_line.push_back('\\');
                  new_line.push_back('n');
                  line_count++;
                  char_location = 1;
                  cout << "T_COMMENT " << new_line << endl; break;
                case 51:
                  new_line = "";
                  char_location -= lexeme.length();
                  for (string::iterator it = lexeme.begin(); it != lexeme.end(); ++it) {
                    if (*it == '\n') {
                      line_count++;
                      char_location = 1;
                      new_line.push_back('\\');
                      new_line.push_back('n');
                    } else {
                      char_location++;
                      new_line.push_back(*it);
                    }
                  }
                  cout << "T_MULTILINE_COMMENT " << new_line << endl; break;
                default: exit(EXIT_FAILURE);
      }
    } else {
      if (token < 0) {
        cerr << "Error on line " << line_count << ", character " << char_location << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
  exit(EXIT_SUCCESS);
}

