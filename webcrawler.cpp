#include <string.h>
#include <stdio.h>
#include "webcrawler.h"
//#include <stdlib.h>
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
	if(word == NULL) {
		word = new char[1];
		word[0] = '\0';
	}
	if((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||(c >= '0' && c <= '9')){
		char * letter = new char[2];
		letter[0] = c;
		letter[1] = '\0';
		word = strcat(word,letter);
	}else{
		if(strlen(word) <= 0){
			return;
		}
		URLRecordList * n = NULL;
		if(_wordToURLRecordList -> find(word, &n) == false){
			URLRecordList * item = new URLRecordList();
			item -> _urlRecordIndex = _headURL;
			item -> _next = NULL;
			_wordToURLRecordList -> insertItem(word,item);
		}else {
			URLRecordList * item = new URLRecordList();
			item -> _urlRecordIndex = _headURL;
			item -> _next = n;
			_wordToURLRecordList -> insertItem(word, item);
		}
		delete [] word;
	} 
}
void WebCrawler::onAnchorFound(char *url){
	if(count >= _maxUrls){
		return;
	}
	char * currURL = _urlArray[_headURL]._url;
	int len = strlen(currURL);
	if(currURL[len - 1] != '/'){
		currURL = strcat(currURL, "/");
	}
	if(url[0] == '/'){
		int sum = 0;
		for(int i = 0; i < len; i++){
			if(currURL[i] == '/'){
				sum++;
			}
			if(sum > 2) {
				currURL[i] = '\0';
				break;
			}
		
		}
		url = strcat(currURL,url);
	}else {
		char * start1 = (char *)"http://";
		char * start2 = (char *)"http://";
		if ((strncmp(url,start1,7) != 0)&&(strncmp(url,start2,8) != 0)){
			url = strcat(currURL,url);
		}
	}
	int n;
	if(_urlToUrlRecord -> find(url,&n) == false) {
		_urlArray[_tailURL]._url = url;
		_urlArray[_tailURL]._description = NULL;
		_tailURL++;
		_urlToUrlRecord -> insertItem(url,_headURL);
		count++;
	
	}
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
		_urlArray[_headURL]._description = description;
		delete [] description;
		_headURL += 1;
	}
}
void WebCrawler::writeURLFile(const char * urlFileName){
	FILE * fd;
	fd = fopen(urlFileName, "w");
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
	fd = fopen(wordFileName, "w");
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
int main (int argc, char ** argv ) {
	if(argc < 2){
		exit(1);
	}
	int maxUrls;
	//maxUrls = 1000;
	count = 0;
	int temp = 1;
	if(strcmp(argv[1], "-u") == 0){
		maxUrls = atoi(argv[2]);
		temp = 3;
	}
	const char ** urlRoots = new const char *[argc - temp];
	int initialUrls = argc - temp;
	for (int i = 0; i < initialUrls; i++) {
		urlRoots[i] = strdup(argv[temp - initialUrls - i]);
	}
	WebCrawler *web = new WebCrawler(maxUrls, initialUrls, urlRoots);
	web -> crawl();
	web -> writeURLFile("url.txt");
	web -> writeWordFile("word.txt");
}// Add your implementation here
