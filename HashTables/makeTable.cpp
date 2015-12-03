//C++ Program for implementing Hash Tables
//
#include <iostream.h>
#include <cstdlib.h>
#include <string.h>
#include <cstdio.h>
 
using namespace std;
const int TABLE_SIZE = 128;

class HashEntry

    public:
	int key;
	int value;
	HashEntry(int key, int val)
	{
	    this->key = key;
	    this->value = val;

	}
	class HashMap
	{
	    private:
		HashEntry **table;
	    public:
		HashMap()
		{
		    //initializes an empty hash table (null)
		    table = new HashEntry * [TABLE_SIZE];
		    for (int i =0; i< TABLE_SIZE; i++)
			{
			table[i] = NULL;
			}

		}

		int HashFunction(int key)
		{
		    return key % TABLE_SIZE;
		}
		//function for inserting an element at a key
		void Insert()
		{
		    int hash = HashFunction(key);
		    while (table[hash] != NULL && table[hash]->key != key)
			{
			    hash = HashFunction(hash+1);
			}
		    if (table[hash] != NULL)
			{
			    delete table[hash];
			}
		    table[hash] = new HashEntry(key, value);
		}

		int Search(int key)
		{
		    int hash = HashFunction(key);
		    while (table[hash] != NULL && table[hash]->key != NULL)
			{
			    hash = HashFunction(hash+1);
			}
		    if (table[hash] == NULL)
			return -1;
		    else
			return table[hash]->value;

		}	
		void Remove(int key)
		{
		    int hash = HashFunction(key);
		    while (table[hash] != NULL)
			{
			    if (table[hash]->key == key)
				break;
			
			    hash = HashFunction(hash+1);


			}
		    if (table[hash] == NULL)
			{
			    cout <<"No element found at key <<key<< endl;
			    return;
			
			}
		    else
			{
		    	    delete table[hash];
			}
		    cout<<"deleted element"<<endl;
		}		
		~HashMap()
		{
		    for (int i=0; i<TABLE_SIZE;i++)
			{
			    if (table[i]!=NULL )
				{
				    delete table[i];
				    
				}
			    delete[] table;

	

			}


		}

	int main()
	{F
	//FINISH!!: add a switch statement so the user can interface with the hash functions
	HashMap hash;
	int key,val;
	int choice;
	while (1)
	{
	    
	}
	}
}


