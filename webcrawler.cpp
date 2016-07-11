#include "webcrawler.h"
#include <string.h>

WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots){
	_maxUrls = maxUrls;
	_tailUrl = nurlRoots; 
	_urlArray = new URLRecord[_tailUrl + _maxUrls];
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
void WebCrawler::crawl() {
	while(_headURL < _tailURL){
		char * currURL = _url[_headURL]._url;
		_headURL++;
		int i;
		char * currBuffer = fetchHTML(curr, &n);
		if(currBuffer == NULL){
			continue;
		}
		parse(currBuffer, n);
	}
}

// Add your implementation here
