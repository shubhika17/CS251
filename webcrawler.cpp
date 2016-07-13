#include "webcrawler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char * des;
char * word;
int count;
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots){
	_maxUrls = maxUrls;
	_tailURL = nurlRoots; 
	_urlArray = new URLRecord[_tailURL + _maxUrls];
	_headURL = 0;
	for(int i = 0; i < _tailURL; i++){
            char *temp = strdup(urlRoots[i]);
	    if (temp[strlen(temp) - 1] != '/'){
	    	temp = strcat(temp, "/");
	    }
	    _urlArray[i]._url = temp;
	    _urlArray[i]._description = NULL;
	}
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>(); 
}
void WebCrawler::onContentFound(char c){

}
void WebCrawler::onAnchorFound(char *url){
	if(count >= _maxUrls){
		return;
	}
	char * currURL = _urlArray[_headURL]._url;
}
void WebCrawler::crawl() {
	while(_headURL < _tailURL){
		char * currURL = _urlArray[_headURL]._url;
		_headURL++;
		int n;
		char * currBuffer = fetchHTML(currURL, &n);
		if(currBuffer == NULL){
			continue;
		}
		parse(currBuffer, n);
		_urlArray[_headURL]._description = des;
		word = NULL;
		des = NULL;
		_headURL += 1;
	}
}
void WebCrawler::writeURLFile(const char * urlFileName){
	FILE * fd;
	fd = open(urlFileName, "w");
	for(int i = 0; i < _tailURL; i++){
		fprintf(fd,"%d %s\n", i, _urlArray[i]._url);
		if(_urlArray[i]._description != NULL){
			fprintf(fd,"%s\n",_urlArray[i]._description);
		}else{
			fprintf(fd,"%s\n","");
		}
		fprintf(fd,"\n");
	}
	fclose(fd);
}
void WebCrawler::writeWordFile(const char *wordFileName){
	FILE * fd;
	fd = open(wordFileName, "w");
	for(int i = 0; i < 2039; i++){
		if(!(_wordToURLRecordList -> _buckets[i] == NULL)){
			fprintf(fd, "%s", _wordToURLRecordList -> _buckets[i] -> _key);
			URLRecordList *curr = _wordToURLRecordList -> _buckets[i] -> _data;
			while (curr != NULL) {
				if (!(curr -> _urlRecordIndex < 0 || curr -> _urlRecordIndex >= 2039)){
					fprintf(fd, "%d", curr -> _urlRecordIndex);	
				}
				curr = curr -> _next;
			}
			fprintf(fd, "\n");
		}	
	}
	fclose(fd);
}
// Add your implementation here
