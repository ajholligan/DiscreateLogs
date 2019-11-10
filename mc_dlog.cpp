#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include "HashTable.h"

using namespace std;
/* Implement mc_dlog in this file */

enum mc_dlogError { INVALID_ARGUMENTS };
enum mc_dlogNotFound { DLOG_NOT_FOUND };

// This code was heavily inspired by https://www.sanfoundry.com/cpp-program-implement-modular-exponentiation-algorithm/
ulint power(ulint g, ulint r, ulint n)
{
	ulint answer = 1;
	while(r > 0)
	{
		 if(r % 2 == 1)
		 {
		 	answer = (answer*g) % n;
		 }

		 r = r >> 1; 
		 g = (g * g) % n;
	}	
	return answer;
}

ulint randomNum(ulint start, ulint end)
{
	static std::default_random_engine r(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<ulint> rand(start, end);
	ulint temp= rand(r);
	return temp;
}

ulint findYOrd(ulint g, ulint r, ulint n)
{
	ulint powerg = power(g, r, n);
	ulint mod = powerg % n;
	return mod;
}

ulint findYLog(ulint g, ulint r, ulint a, ulint n)
{
	ulint powerg =  power(g,r, n) * a;
	ulint mod = powerg % n;
	return mod;
}

ulint orderG(ulint g, ulint n)
{
	HashTable Ord;
	ulint sqrtN = sqrt(n);

	for(ulint i=0; i<sqrtN; i++)
	{
		//cout << "order iteration: " << i+1 << endl;
		ulint r = randomNum(0, n-1);
		ulint y = findYOrd(g, r, n);
	//	cout<< y<< endl;

		if(Ord.checkValueInTable(y) == 1)
		{
			if(Ord.getValue(y) < r)
			{
				return r- Ord.getValue(y);
			}

			if(Ord.getValue(y) > r)
			{
				return Ord.getValue(y)-r;
			}

		}
		else
		{
			Ord.insert(y,r);
		}
		
	}
	return n-1;
}

ulint disLog(ulint g, ulint a, ulint n, ulint o)
{
	HashTable A;
	HashTable B;
	ulint sqrtN = sqrt(n);

	for(ulint i=0; i<sqrtN; i++)
	{
		//cout << "disLog iteration: " << i << endl;
		ulint r = randomNum(0,n-1);
		ulint y = findYLog(g, r , a , n);

		if(B.checkValueInTable(y) ==1)
		{
			if(B.getValue(y) < r)
			{
				return o+ B.getValue(y) - r %n;
			}
			return B.getValue(y) - r;
		}
		else
		{
			if(A.checkValueInTable(y) == 0)
			{
				A.insert(y, r);
			}
		}

		r = randomNum(0,n-1);
		y = power(g,r,n);

		if(A.checkValueInTable(y) == 1)
		{
			if(A.getValue(y) > r)
			{
				// cout << o+r << "   " << A.getValue(y)<< endl;
				return o + r - A.getValue(y) % n;
			}
			return r - A.getValue(y);
		}
		else
		{
			if( B.checkValueInTable(y) == 0)
			{
				B.insert(y, r);
			}
			
		}
	}
	throw DLOG_NOT_FOUND;
	return 0;
}

ulint bringTogether(ulint g, ulint a, ulint n)
{
	ulint order = orderG(g,n);
	ulint dLog = disLog(g,a,n, order);
	ulint result = dLog % order;


	return result;
}

int main(int argc, char** argv)
//int main()
{
	try
	{
		if((argv[4] != nullptr))
		{
			throw INVALID_ARGUMENTS;
		}
		if(argv[1] == nullptr || argv[2] == nullptr ||argv[3] == nullptr)
		{
			throw INVALID_ARGUMENTS;
		}
		 if (!(*argv[1] >= 48 && *argv[1] <= 57) || !(*argv[2] >= 48 && *argv[2] <= 57) || !(*argv[3] >= 48 && *argv[3] <= 57))   
		{
			throw INVALID_ARGUMENTS;
		}

		ulint g =  atof(argv[1]);
		ulint a =  atof(argv[2]);
		ulint n =  atof(argv[3]);

	//	ulint results[10];
	//	for(int i = 0; i < 10; i++)
	//	{
			//results[i] = bringTogether(3,8436400,8436401); // expect 4218200
			//results[i] = bringTogether(2 8434170 8434171); // expect 8434170
			//results[i] = bringTogether(g,a,n);
		ulint result = bringTogether(g,a,n);
		cout<< result <<endl;
	//	}


	//	for(int i =0; i<10; i++)
	//	{
	//		cout<< results[i]<< "\n"; 
	//	}

		return 0;
	
	}
	catch(mc_dlogError e)
	{
		cout <<"Error INVALID_ARGUMENTS: Please only enter 3 positive values with spaces between"<< endl;
		return 1;
	}
	catch(mc_dlogNotFound e)
	{
		cout << "The discrete log could not be calculated this time" << endl;
	}
	
}

// 2 8434170 8434171