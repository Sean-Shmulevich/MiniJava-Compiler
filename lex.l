%{
/** Code by @author Wonsun Ahn, Fall 2024
 * 
 * The lexer implementation file.
 */

#include <stdlib.h>

extern void addStringTable(char* text, int tokenid ); /* install string constants & identifiers into string tbl */

void reportError(int type);

/* TODO: Add additional function declarations as needed. */

%}

comment [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]

newline \n
int     "int"
program     "program"
declarations "declarations"
enddeclarations "enddeclarations"
class       "class"
while       "while"
return       "return"
if       "if"
else       "else"
val       "val"
method      "method"
void        "void"
left_brace        "{"
right_brace        "}"
left_brac        "["
right_brac        "]"
left_paren        "("
right_paren        ")"
semi ";"
dot "."
comma ","
equal "="
equivalent "=="
not_equal "!="
and "&&"
or "||"
plus "+"
minus "-"
nah_bruh "!"
multi "*"
divide "/"
greater ">"
less "<"
greater_eq ">="
less_eq "<="
assignment ":="
space " "
tab ("    ")|\t
identifier [A-Za-z][A-Za-z0-9]*
string_const \'(\\.|[^'\\\n])*\'
integer_const [0-9]*
bad_str \'(\\.|[^'\\\n])*$
invalid_id [0-9]+[A-Za-z][A-Za-z0-9]*|([A-Za-z]+[^a-zA-Z0-9()+\-\[\],=;{}\*]+[0-9a-zA-Z][^a-zA-Z0-9()+\-\[\],=;{}\*]+[A-Za-z][A-Za-z0-9]*)

bad_comment [/][*][^*]*([^*/][^*]*)

%%

 /* TODO: Add rules for actions for each declared token. */
{comma} { yycolumn += yyleng; return COMMAnum; }
{comment} {
// ([.\n]|[^(*/)(/*)])*[^*/]
// invalid_id [0-9]+[A-Za-z][A-Za-z0-9]*|([A-Za-z]+[^a-zA-Z0-9()+\-]+[0-9a-zA-Z][^a-zA-Z0-9()+\-]+[A-Za-z][A-Za-z0-9]*)
    int i;
    for (i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            yyline++;
            yycolumn = 1;
        } else {
            yycolumn++;
        }
    }
}
 /* Letting semi and comma, out of invalid id, maybe a big problem I should try to make 
it so that the new character that I added only exclude the string as an invalid string if they are at the end*/
{invalid_id} {
    int i;
    // for (i = 0; yytext[i] != '\0'; i++) {
    //     // Check if the character is valid for an identifier
    //     if (yytext[i] == ',') { 
    //         // Push back characters from the current position to the end
    //         for (int j = yyleng - 1; j >= i; j--) {
    //             unput(yytext[j]);
    //         }

    //         // Null-terminate the valid portion and return it as an identifier
    //         yytext[i] = '\0';
    //         addStringTable(yytext, IDnum);
    //         yycolumn += i; // Update column count for the valid portion
    //         return IDnum;
    //     }
    // }

    // If the entire text is invalid, report an error
    reportError(1);
    yycolumn += yyleng;
}


{bad_str}     {reportError(2); yycolumn += yyleng;}
{bad_comment} {
    int i;
    for (i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            yyline++;
            yycolumn = 1;
        } else {
            yycolumn++;
        }
    }
    reportError(3);
}
{int} { yycolumn += yyleng; return INTnum; }
{integer_const} {
   yycolumn += yyleng;
   yylval.intg = atoi(yytext);
   return ICONSTnum; 
}

{program}   { yycolumn += yyleng; return PROGRAMnum; }
{class}     { yycolumn += yyleng; return CLASSnum; }
{while}     { yycolumn += yyleng; return WHILEnum; }
{if}     { yycolumn += yyleng; return IFnum; }
{else}     { yycolumn += yyleng; return ELSEnum; }
{val}     { yycolumn += yyleng; return VALnum; }
{return} { yycolumn += yyleng; return RETURNnum; }

{method}    { yycolumn += yyleng; return METHODnum; }
{void}      { yycolumn += yyleng; return VOIDnum; }
{declarations}      { yycolumn += yyleng; return DECLARATIONSnum; }
{enddeclarations}      { yycolumn += yyleng; return ENDDECLARATIONSnum; }
{identifier} { yycolumn += yyleng; addStringTable(yytext,IDnum);  return IDnum; }


{string_const} { 
    yycolumn += yyleng; 
    // Remove closing quote
    yytext[yyleng-1] = '\0';
    // Skip opening quote
    addStringTable(yytext+1, SCONSTnum);
    return SCONSTnum;
}

{left_brace} { yycolumn += yyleng; return LBRACEnum; }
{right_brace} { yycolumn += yyleng; return RBRACEnum; }

{left_brac} { yycolumn += yyleng; return LBRACnum; }
{right_brac} { yycolumn += yyleng; return RBRACnum; }

{left_paren} { yycolumn += yyleng; return LPARENnum; }
{right_paren} { yycolumn += yyleng; return RPARENnum; }

{less} { yycolumn += yyleng; return LTnum; }
{greater} { yycolumn += yyleng; return GTnum; }
{less_eq} { yycolumn += yyleng; return LEnum; }
{greater_eq} { yycolumn += yyleng; return GEnum; }
{plus} { yycolumn += yyleng; return PLUSnum; }
{minus} { yycolumn += yyleng; return MINUSnum; }
{nah_bruh} { yycolumn += yyleng; return NOTnum; }
{multi} { yycolumn += yyleng; return TIMESnum; }
{divide} { yycolumn += yyleng; return DIVIDEnum; }


{semi} { yycolumn += yyleng; return SEMInum; }
{dot} { yycolumn += yyleng; return DOTnum; }
{assignment} { yycolumn += yyleng; return ASSGNnum; }
{equal} { yycolumn += yyleng; return EQUALnum; }
{or} { yycolumn += yyleng; return ORnum; }
{and} { yycolumn += yyleng; return ANDnum; }

{equivalent} { yycolumn += yyleng; return EQnum; }
{not_equal} { yycolumn += yyleng; return NEnum; }



{space}     { yycolumn += yyleng; }
{tab} {yycolumn += yyleng;}
{newline} { yycolumn = 1; yyline++; }
. {reportError(4); yycolumn += yyleng;}

%%

/** Prints out an error message depending on error type.
 *
 * @param errorType - A number designating an error type.
 */
void reportError(int errorType)
{
    if(errorType == 1) {
        printf("Error: Malformed identifier '%s', at line %d column %d\n", yytext, yyline, yycolumn);
    }
    else if (errorType == 2) {
        printf("Error: Unmatched string constant, at line %d column %d\n", yyline, yycolumn);
    }
    else if (errorType == 3) {
        printf("Error: EOF found in comment, at line %d column %d\n", yyline, yycolumn);
    }
    else if (errorType == 4) {
        printf("Error: Undefined token '%s', at line %d column %d\n", yytext, yyline, yycolumn);
        // printf("Error: Undefined token '%s', at line %d column %d\n", yytext, yyline, yycolumn);
    }
}
