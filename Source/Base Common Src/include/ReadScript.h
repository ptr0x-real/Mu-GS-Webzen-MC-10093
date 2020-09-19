#ifndef __READSCRIPT_H__
#define __READSCRIPT_H__

#include <stdio.h>

static enum SMDToken 
{
	NAME, 
	NUMBER, 
	END, 
	COMMAND = '#',
	LP = '{',
	RP = '}',
	COMMA = ',',
	SEMICOLON = ';',
	SMD_ERROR
};

static FILE     *SMDFile;
static float    TokenNumber;
static char     TokenString[100];
static SMDToken CurrentToken;

static SMDToken GetToken()
{
	unsigned char ch;
	TokenString[0] = '\0';
	do
	{
		if ( (ch =(unsigned char) fgetc(SMDFile)) == (BYTE)EOF) return END;
		if (ch=='/' && (ch =(unsigned char) fgetc(SMDFile) )=='/')	
		{
			while((ch != '\n') && (ch != (BYTE)EOF)) {			// 2003.09.02 H.J.I ����
				ch = (unsigned char) fgetc( SMDFile);
			}

			if (ch == (BYTE)EOF)
				return END;
		}
	} while(  isspace(ch) );
	
	char *p, TempString[100];
	switch(ch)
	{	
	case '#':
		return CurrentToken = COMMAND;
	case ';':
		return CurrentToken = SEMICOLON;
	case ',':
		return CurrentToken = COMMA;
	case '{':
		return CurrentToken = LP;
	case '}':
		return CurrentToken = RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		ungetc(ch,SMDFile);
		p = TempString;
		while ( (  (ch = getc(SMDFile) ) !=(BYTE)EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;
		*p = 0;
		TokenNumber = (float)atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = getc(SMDFile) ) !=(BYTE)EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			ungetc(ch,SMDFile);
		*p = 0;
		return CurrentToken = NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = getc(SMDFile) ) !=(BYTE)EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			ungetc(ch,SMDFile);
			*p = 0;
			return CurrentToken = NAME;
		}
		return CurrentToken = SMD_ERROR;
	}
}

#endif