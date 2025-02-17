%{
//#include "yaccExample.h"
#include "IOMngr.h"
#include "y.tab.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = getNextSourceChar(); \
          result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
        }

%}


letter [A-Za-z]
digit [0-9]

%%
if					{return IF;}
else        {return ELSE;}
while       {return WHILE;}
for         {return FOR;}
void        {return VOID;}
int					{return Int;}
return      {return RETURN;}
print					{return Write;}
read          {return Read;}
printline     {return PrintLine;}
printspaces    {return PrintSpaces;}
{letter}({letter}|{digit})*	{return Ident;}
{digit}{digit}*		{return IntLit;}
\^													{return EXPNT;}
\<\=          {return LTE;}
\>\=          {return GTE;}
\=\=				{return EQ;}
\!\=				{return NOTEQ;}
\&\&        {return AMP;}
\|\|         {return OR;}
\,          {return ',';};
\<          {return '<';}
\>          {return '>';}
\!          {return '!';}
\=					{return '=';}
\+					{return '+';}
\-          {return '-';}
\*					{return '*';}
\/					{return '/';}
\%					{return '%';}
\;					{return ';';}
\{					{return '{';}
\}					{return '}';}
\(					{return '(';}
\)					{return ')';}
\[          {return '[';}
\]          {return ']';}

[ ]				{}
\t				{}
\r				{}
\n				{}

.				{writeIndicator(getCurrentColumnNum());
				   writeMessage("Illegal Character in lex");}

%%


int yywrap () {
    return 1;
}
