#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE
#include <iostream>
#include "HashNode.h"
#include <vector>
#include <list>

using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY }; // extend if necessary

typedef unsigned long ulint;

class HashTable {
  typedef vector <list<HashNode> > Table;
  Table *table; // size of table is stored in the Table data structure
  size_t num;   // number of entries in the HashTable;

public:
  HashTable();       // constructor, initializes table of size 11;
  HashTable(size_t); // constructor, requires size of table as arg
  ~HashTable();      // deconstructor

  size_t size(); // returns size of the hash table (number of buckets)
  size_t hash_function(ulint);  // the table's hash function
  ulint getValue(ulint);    // find and return data associated with key

  void insert(ulint,ulint); // insert data associated with key into table
  void erase(ulint);        // remove key and associated data from table

  void rehash(size_t); // sets a new size for the hash table, rehashes the hash table 
  float loadFactor();
  size_t checkValueInTable(ulint);
  // extend if necessary
};

HashTable::HashTable()
{
  try
  {
    this-> num = 0;     // number of entrys is always 0 at the start
    this-> table = new Table(11, list<HashNode>()); // initialises a new Table which is a vector of lists all initailised with the list constructor
  }
  catch(bad_alloc)
  {
    cout<< "Error bad_alloc: The program has run out of memory"<<"\n";
  } 

}

HashTable::HashTable(size_t num)
{
  try
  {
    this-> num = 0;   // number of entrys is always 0 at the start
    this->table = new Table(num, list<HashNode>()); // initialises a new Table which is a vector of lists all initailised with the list constructor
  }
  catch(bad_alloc e)
  {
    cout<< "Error bad_alloc: The program has run out of memory"<<"\n";
  } 
}

HashTable::~HashTable()
{
  delete this-> table;      //deleting all dynamic memory i.e. anything which uses new
}

size_t HashTable::size()
{
  return (*table).size(); // returns the size of the vector in the table pointer
}

size_t HashTable::hash_function(ulint key)
{
  size_t hash = key % size();     //hash function 
  return hash;
}

void HashTable::insert(ulint key,ulint value)
{
  try
  {


    if(checkValueInTable(key) == 1)
    {
      throw DUPLICATE_KEY;
    }
    if(loadFactor() > 0.9)
    {
      rehash(size()*2);
    }
    size_t hashValue = hash_function(key);  // hashing the key to find which bargin bucket it is mapped to
    HashNode node;
    node.assign(key, value);   //creates new node 
    (*table).at(hashValue).push_back(node);  //finds the bucket assosiated with the hash value and ands the new node to the end of the list
   
    num++;
  }
  catch(HashTableError)
  {
    cout<< "Error DUPLICATE_KEY: please enter a unique key"<< "\n";
  }
}


ulint HashTable::getValue(ulint key)
{
  try
  {
    size_t hashValue = hash_function(key);  // hashing the key to find which bargin bucket it is mapped to
    for(HashNode &n : (*table).at(hashValue))
    {
      if(n.getKey() == key)
      {
        return n.getValue();
      }
      
    }
    
    throw KEY_NOT_FOUND;
  }
  catch(HashTableError)
  {
    cout<< "Error KEY_NOT_FOUND: please enter a valid key"<< "\n";
  }
  return 0;
}

size_t HashTable::checkValueInTable(ulint key)
{
  size_t hashValue = hash_function(key);  // hashing the key to find which bargin bucket it is mapped to
    for(HashNode &n : (*table).at(hashValue))
    {
      if(n.getKey() == key)
      {
        return 1;
      }
      
    }
    return 0;
}

void HashTable::erase(ulint key)
{

  try
  {
    size_t hashValue = hash_function(key);  // hashing the key to find which bargin bucket it is mapped to
  
    //(*table).at(hashValue)
    list<HashNode>::iterator it; 

    for (it = (*table).at(hashValue).begin(); it != (*table).at(hashValue).end(); ++it) {
    //{
      if((*it).getKey() == key)
      {
        it = (*table).at(hashValue).erase(it);
        num--;
        return;
      }
  
    }
    throw KEY_NOT_FOUND;

  }
  catch(HashTableError)
  {
    cout<< "Error KEY_NOT_FOUND: The key you entered does not exist"<< "\n";
  }
//}
}

void HashTable::rehash(size_t newSize)
{
  Table *newTable = table;
  size_t oldSize = HashTable::size(); /////////////////////////////////////   num/size
  table = new Table(newSize, list<HashNode>());
  for( size_t i = 0; i< oldSize; i++)
  {
    list<HashNode>::iterator it; 

    for (it = (*newTable).at(i).begin(); it != (*newTable).at(i).end(); ++it)
    {
        ulint key = (*it).getKey();
        ulint value = (*it).getValue();
        insert(key, value);
    }
  }
 
}

float HashTable::loadFactor()
{
  float load = static_cast<float>(num) / static_cast<float>(size());
  return load; 
}

/* Implement the 
- Constructors, Destructor
- hash_function, insert, getValue methods
- erase, and rehash methods 
*/
#endif