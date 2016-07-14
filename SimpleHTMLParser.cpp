
#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include <string.h>

SimpleHTMLParser::SimpleHTMLParser()
{
}

bool
SimpleHTMLParser::match(char **b, const char *m) {
	int len = strlen(m);
	if ( !strncasecmp(*b, m, len)) {
		(*b)+=len;
		return true;
	}
	return false;
}

bool
SimpleHTMLParser::parse(char * buffer, int n)
{
	enum { START, TAG, SCRIPT, ANCHOR, HREF,
	       COMMENT, FRAME, SRC, HTML, META, CONTENT, TITLE} state;

	state = START;
	
	char * bufferEnd = buffer + n;
	char * b = buffer;
	bool lastCharSpace = false;
	bool foundDes = false;
	bool foundWord = false;
	bool title = false;
	int count = 0;
	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		switch (state) {
		case START: {
			if (match(&b,"<SCRIPT")) {
				state = SCRIPT;
			}
			else if (match(&b,"<!--")) {
				state = COMMENT;
			}
			else if (match(&b,"<A ")) {
				state = ANCHOR;
			}
			else if (match(&b,"<FRAME ")) {
				state = FRAME;
			}
			else if(match(&b, "HTML " )){
				state = HTML;
			}else if(match(&b, "<meta")){
				state = META;
			}
			else if(match(&b, "content")){
				state = CONTENT;
			}else if(match(&b, "<title>")){
				state = TITLE;
			}
			else if(match(&b,"<")) {
				state = TAG;
			} 
			else {
				char c = *b;
						//Substitute one or more blank chars with a single space
				if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
					if (!lastCharSpace) {	
						if (foundWord){
							onContentFound(' ');
						}
					}
					lastCharSpace = true;
				}
				else {
					if (foundWord){
						onContentFound(c);
						if(*b == '"'){
							count ++;
						}
						if((count == 1)){
							foundWord = false;
							count = 0;
						}
					}
					lastCharSpace = false;
				}
				
				b++;
			}
			break;
		}
		case ANCHOR: {
			if (match(&b,"href=\"")) {
				state = HREF;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case HREF: {
			if (match(&b,"\"")) {
				// Found ending "
				state = ANCHOR;
		
		urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
		
		//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case FRAME: {
			if (match(&b,"src=\"")) {
				state = SRC;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case SRC: {
			if (match(&b,"\"")) {
				// Found ending "
				state = FRAME;
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case SCRIPT: {
			if (match(&b,"/SCRIPT>")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case COMMENT: {
			if (match(&b,"-->")) {
				// End comments
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case TAG: {
			if (match(&b, ">")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case HTML:{
			if(match(&b, ">")){
				//hasDocument = true;
				state = START;
			}else {
				b++;
			}
			break;
		}
		case META: 
		{
			char * buffer = b;
			int count = 0;
			for(int i = 0; count < 2; i++){
			
			if(*(buffer) == '"'){
					count++;
				}
				buffer++;
			}
			while( *buffer != 'n'){
				buffer++;
			}
			if (match(&buffer,"name=\"description\"")){
				foundDes = true;	
			}
			if(match(&buffer,"name=\"keywords\"")){
				foundWord = true;
			}
			state = START;
			b++;
			break;			 		
		}
		case CONTENT: 
		{
			if(foundDes){
				while(*b != '"'){
					b++;
				}
				if (title){
					delete description;
					title = false;
				}
				b++;
				while(*b != '"'){
					if (description == NULL){
						description =(char *) "\0";
					}
					char * single = new char[1];
					single[0] = *b;
					b++;
					strcat(description,single);
					delete  single; //ask	
				}
				foundDes = false;			
			}
			if(foundWord){
				while(*b != '"'){
					b++;
				}
			}
			state = START;
			b++;
			break;
		}
		case TITLE:
		{
			if (description == NULL){
				if (*b == '>'){
					b++;
				}
				while( *b != '<'){
					char * single = new char[1];
					single[0] = *b;
					b++;
					strcat(description,single);
					delete  single; //ask	
				}
				title = true;
			}
			state = START;
			b++;
			break;
		}
		default:;
		}
		
	}
}

void
SimpleHTMLParser::onContentFound(char c) {
}

void
SimpleHTMLParser::onAnchorFound(char * url) {
}
