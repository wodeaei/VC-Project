/*
 *   Phishing detection automated testing & tools.
 *
 *  Copyright (C) 2006 Torok Edvin <edwintorok@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum wctype_t {ALNUM,DIGIT,PUNCT,ALPHA,GRAPH,SPACE,BLANK,LOWER,UPPER,CNTRL,PRINT,XDIGIT};
static struct std_classmap {
		const char* classname;
		const enum wctype_t type;
} std_class[] = {
	{"[:alnum:]",ALNUM},
	{"[:digit:]",DIGIT},
	{"[:punct:]",PUNCT},
	{"[:alpha:]",ALPHA},
	{"[:graph:]",GRAPH},
	{"[:space:]",SPACE},
	{"[:blank:]",BLANK},
	{"[:lower:]",LOWER}, 
	{"[:upper:]",UPPER},
	{"[:cntrl:]",CNTRL},
	{"[:print:]",PRINT},
	{"[:xdigit:]",XDIGIT}
};

static int cli_iswctype(const char c,const enum wctype_t type);

/* -------------- NON_THREAD_SAFE BEGIN --------------*/
/* Global variables and functions accessing them, not thread-safe!
 * they should be called on application startup/shutdown once! */
static const size_t std_class_cnt =  sizeof(std_class)/sizeof(std_class[0]);
#define STD_CLASS_CNT sizeof(std_class)/sizeof(std_class[0])
typedef char char_bitmap_t[32];
static unsigned char* char_class_bitmap[STD_CLASS_CNT];
static unsigned short int char_class[256];
static int engine_ok = 0;
static int cli_iswctype(const char c,const enum wctype_t type)
{
	switch(type) {
		case ALNUM:
			return isalnum(c);
		case DIGIT:
			return isdigit(c);
		case PUNCT:
			return ispunct(c);
		case ALPHA:
			return isalpha(c);
		case GRAPH:
			return isgraph(c);
		case SPACE:
			return isspace(c);
		case BLANK:
			return c=='\t' || c==' ';
		case LOWER:
			return islower(c);
		case UPPER:
			return isupper(c);
		case CNTRL:
			return iscntrl(c);
		case PRINT:
			return isprint(c);
		case XDIGIT:
			return isxdigit(c);
		default: {
				 return 0;
			 }
	}
}


void setup_matcher_engine(void)
{
	/*Set up std character classes*/
	printf("--------regex_list.c--------\n");
	printf("/* generated by contrib/phishing/generate_tables.c */\n");
	printf("static const unsigned char char_class_bitmap[STD_CLASS_CNT][32] = {\n");


	size_t i;
	size_t j;
	memset(char_class,0,256);
	for(i=0;i<std_class_cnt;i++) {
		enum wctype_t type = std_class[i].type;
		printf("        {");
		char_class_bitmap[i]=calloc(256>>3,1);
		for(j=0;j<256;j++)
			if(cli_iswctype(j,type)) {
				char_class[j] |= 1<<i;
				char_class_bitmap[i][j>>3] |= 1<<(j&0x07);
			}
		for(j=0;j<32;j++) {
			printf("0x%02x",char_class_bitmap[i][j]);
			if(j!=31) {
				printf(", ");
				if(j%8==7)
					printf("\n         ");
			}
		}
		printf("}");
		if(i!=std_class_cnt-1)
			printf(",\n\n");
	}	
        printf("\n};\n");
	printf("static const unsigned short int char_class[256] = {\n        ");
	for(i=0;i<256;i++) {
		printf("0x%03x",char_class[i]);
		if(i!=255) {
			printf(", ");
			if(i%16==15)
				printf("\n        ");
		}
	}

	printf("\n};\n");
	engine_ok  = 1;
}
static short int hextable[256];
static void init_hextable(void)
{
	unsigned char c;
	int i;
	memset(hextable,0,256);
	for(c='0';c<='9';c++)
		hextable[c] = c-'0';
	for(c='a';c<='f';c++)
		hextable[c] = 10+c-'a';
	for(c='A';c<='F';c++)
		hextable[c] = 10+c-'A';
	printf("-------phishcheck.c---------\n");
	printf("/* generated by contrib/phishing/generate_tables.c */\n");
	printf("static const short int hextable[256] = {\n       ");
	for(i=0;i<256;i++) {
		printf("0x%x",hextable[i]);
		if(i!=255) {
			printf(", ");
			if(i%16==15)
				printf("\n       ");
		}
	}
	printf("\n};\n");
}

int main()
{
	setup_matcher_engine();
	init_hextable();
	return 0;
}

