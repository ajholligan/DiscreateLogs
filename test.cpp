#include "HashTable.h"

using namespace std;

int main()
{
	HashTable t(10000);
	t.insert(3, 9);
	t.getValue(3);
	t.erase(3);
	t.getValue(3);

}