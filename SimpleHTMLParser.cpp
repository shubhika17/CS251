
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
	letter = 0;
	//bool des = false;
	title = false;
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
					
							onContentFound(' ');
						
					}
					lastCharSpace = true;
				}
				else {
					onContentFound(c);
				}
				lastCharSpace = false;
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
				if(b < bufferEnd) {
					break;
				}
				if(*(b) == '"' ){
					count++;
				}
				b++;
			}	
			while(b < bufferEnd && *b != 'n'){
				b++;
			}
			if (match(&b,"name=\"description\"")){
				foundDes = true;	
			}
			state = START;
			//b++;
			break;			 		
		}
		case CONTENT: 
		{	if(desc == true){
				b++;
				state = START;
				break;
				
			}
			char * buffer = b;
			//int letter = 0;
			if(foundDes){
				while(buffer < bufferEnd && *buffer != '"'){
					buffer++;
				}
				if (title){
					description += " ";
					description += ":";
					description += " ";
					//title = false;
				}
				//buffer++;
				while(buffer < bufferEnd && (*buffer != '"') && *buffer != '=' && *buffer != '.'&& *buffer != ':' && *buffer != '/' && *buffer != '#'){
					if(letter >= 500){
						break;
					}
					description += *buffer;
					buffer++;
					letter++;
				}
				foundDes = false;	
				desc = true;			
				//letter = 0;
			state = START;
			b++;
			break;
		}
	}
		case TITLE:
		{
			if(desc == true) {
				state = START;
				b++;
				break;
			}
			int letter = 0;
			char * buffer = b;
			if (buffer < bufferEnd && *buffer != '>'){
				buffer++;
			}
			buffer--;
			if(*buffer == '-') {
				buffer = buffer + 5;
			}
			//buffer--;
			while(buffer < bufferEnd && ( *buffer != '<') &&  (*buffer != '"') && *buffer != ':' && *buffer != '{'&& *buffer != '='  && *buffer != ';' && *buffer != '.' && *buffer != '/' && *buffer != '#'){
				description += *buffer;
				buffer++;
				letter++;
				if(letter > 500) {
					break;
				}
			}
			title = true;
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
