#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

int divide_nr(vector<int>& v, int n1, int n2)
{
  if( n1==n2 )
    if( v[n1]==n1 ) return n1;
  else return -1;
  else {
    int middle=(n1+n2)/2;
    if( v[middle]==middle ) return middle;
    else if ( v[middle] > middle ) return divide_nr(v, n1, middle-1);
    else return divide_nr(v, middle+1, n2);
  }
}

double uniforme()
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

int main(int argc, char * argv[])
{

  if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }
    
  int n = atoi(argv[1]);
  int m=2*n-1;

  int * T = new int[n];
  assert(T);
  int * aux = new int[m];
  assert(aux);

  srand(time(0));
  
  //genero todos los enteros entre -(n-1) y n-1
  for (int j=0; j<m; j++) aux[j]=j-(n-1);

  //algoritmo de random shuffling the Knuth (permutaci�n aleatoria) 
  for (int j=m-1; j>0; j--) {
    double u=uniforme();
    int k=(int)(j*u);
    int tmp=aux[j];
    aux[j]=aux[k];
    aux[k]=tmp;
  }

  //me quedo con los n primeros del vector
  for (int j=0; j<n; j++) T[j]=aux[j];

  //Y ahora ordeno el vector T
  vector<int> myvector (T, T+n);
  vector<int>::iterator it;

  sort(myvector.begin(),myvector.end());

  for(int i=0; i<myvector.size(); i++) cout << myvector[i] << " ";
  cout << endl;

  //Calculamos la eficiencia
  chrono::high_resolution_clock::time_point after, before;
  chrono::duration<double> passed;

  before = chrono::high_resolution_clock::now();
  int  i=divide_nr(myvector, 0, myvector.size()-1);
  after = chrono::high_resolution_clock::now();

  passed = chrono::duration_cast<chrono::duration<double> >(after - before);

  // cout << n << " " << passed.count() << endl;
  cout << "Resultado: " << i << endl;
  delete [] aux;
}