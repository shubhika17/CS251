
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  
  _buckets = new HashTableTemplateEntry<Data> * [TableSize];
  for (int i = 0; i < TableSize; i++){
  	_buckets[i] = NULL;
  }
  // Add implementation here
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  // Add implementation here
  
  // Add implementation here
  int i = hash(key);
  HashTableTemplateEntry<Data> * list = _buckets[i];
  if(_buckets[i] != NULL){
  while((list != NULL) ){
  	 //printf("here \n");
	 if(strcmp(list -> _key,key) == 0){
		list -> _data = data;
		return true;
	 	
	 }
		list = list -> _next;
  }
  	//printf("here \n");
	HashTableTemplateEntry<Data> * entry = new HashTableTemplateEntry<Data>;
	entry -> _key = strdup(key);
	entry -> _data = data;
	entry -> _next = _buckets[i];
	_buckets[i] = entry;
	
	//printf("entry %s %d \n", _buckets[i] -> _key, i);
	return false;
	}else {
	HashTableTemplateEntry<Data> * entry = new HashTableTemplateEntry<Data>;
	entry -> _key = strdup(key);
	entry -> _data = data;
	entry -> _next = NULL;
	_buckets[i] = entry;
			
	}


	return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  // Add implementation here
int i = hash(key);
  if(_buckets[i] == NULL){
  	return false;
  }
  HashTableTemplateEntry<Data> * list = _buckets[i];
  while((list!= NULL)){
	 if(strcmp(list -> _key,key) == 0){
		*data = list -> _data;
		return true;
	 	
	 }
	list = list -> _next;		
  }
  // Add implementation here
  return false;
}
template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
int i = hash(key);
  HashTableTemplateEntry<Data> * list = _buckets[i];
  while((list!= NULL)){
	 if(strcmp(list -> _key,key) == 0){
		Data d = list -> _data;
		return d;
	 	
	 }
	list = list -> _next;	
  }
  //int i = hash(key);
  
 Data d = 0; 
  return d;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
  
  int i = hash(key);
	HashTableTemplateEntry<Data> * first = NULL;
	HashTableTemplateEntry<Data> * temp = _buckets[i];
  while((temp != NULL) ){
  	//printf("here \n" );
  	if (strcmp(temp -> _key, key) == 0){
	//printf("here" );
	break;
	}
		first = temp;
		temp = temp -> _next;
  }
if(temp != NULL){
	if (first == NULL ){
		delete temp;
		_buckets[i] = NULL;
		return true;
	}
	first -> _next = temp -> _next;
	 //temp -> _data = NULL;
	//temp -> _data = NULL;
	temp -> _key = NULL;
	delete temp;
	return true;
}
return false;
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
	_hashTable = hashTable;
	int i;
	for (i = 0; _hashTable -> _buckets[i] != NULL; i++);
	_currentBucket = i;
	_currentEntry = _hashTable -> _buckets[_currentBucket];
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  // Add implementation here
	while (_currentBucket < _hashTable -> TableSize){
		if(_currentEntry != NULL){
			key = _currentEntry -> _key;
			data = _currentEntry -> _data;
			_currentEntry = _currentEntry -> _next;
			return true;
		}
		_currentBucket += 1;
		_currentEntry = _hashTable -> _buckets[_currentBucket];
	
	}
  return false;
}
