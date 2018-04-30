#ifndef __MATRIZ_DE_ADYACENCIA__
#define __MATRIZ_DE_ADYACENCIA__

#include <vector>
#include <utility>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include <set>
#include <iostream>
#include <limits.h>

using namespace std;

typedef pair<double, double> city;

double euclidean_distance(city c1, city c2){ return sqrt(pow(c2.first - c1.first, 2) + pow(c2.second - c1.second, 2)); }

class graph{
  private:
    vector<vector<double> > m;
    vector<city> cities;
    vector<bool> visited;

    void fill(const vector<city> &v);
    bool end_path();
    bool cycle(vector<int> path, int node);
    void clear();
    int east_city();
    int west_city();
    int north_city();
    
  public:
    graph(const char* fichero);

    double get_weight(int i, int j);
    double total_weight(vector<int> path);

    vector<int> optimal_path(double &l_min);

    vector<int> min_path1(int i, double &l);
    vector<int> min_path2(int i, double &l);
    vector<int> min_path3(int i, double &l);
    
};

double graph::get_weight(int i, int j){
  if( i==j ) return 0;
  else if( i>j ) return m[i][j];
  else return m[j][i];
}

bool graph::end_path(){ return count(visited.begin(), visited.end(), false) == 0; }

bool graph::cycle(vector<int> path, int node){
  for(unsigned int i= 0; i<path.size(); i++)
    if(path[i] == node)
      return true;
  return false;
}

void graph::fill(const vector<city> &v){
  int n= v.size();
  m.resize(n);

  for(unsigned int i= 0; i< m.size(); i++)
    m[i].resize(n);

  for(int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
      m[i][j]= euclidean_distance(v[i], v[j]);
}

graph::graph(const char *file){
  int num_cities, n;
  double x, y;
  string header;
  ifstream f;

  f.open(file);

  if(!f){
    cerr << "No se pudo abrir el fichero" << endl;
    exit(-1);
  }

  f >> header;
  f >> num_cities;

  for(int i=0; i< num_cities; i++){
    f >> n;
    f >> x;
    f >> y;
    cities.push_back( make_pair(x, y) );
  }

  visited.resize(n, false);

  fill(cities);
  
  f.close();
}

void graph::clear(){
  visited.clear();
  visited.resize(m.size(), false);
}

vector<int> graph::optimal_path(double &l_min){
  double l= 0;
  l_min= LONG_MAX;
  vector<int> current, min;
  for(int i=0; i<m.size(); i++){
    l= 0; clear();
    current= min_path1(i, l);
    if(l< l_min){
      min= current;
      l_min= l;
    }
  }
  
  return min;
}



vector<int> graph::min_path1(int i, double &l){
  int n=cities.size(), j;

  set<pair<double, int> > posibilities;
  set<pair<double, int> >::iterator it;
  double min_dist, destination;
  vector<int> r;
  r.push_back(i);
  visited[i]= true;

  //Mientras haya ciudades por recorrer
  while( !end_path() ){
    for(j=0; j<n; j++){
      if( !cycle(r, j) ){                                       //Si estamos en el triángulo superior de la matriz de adyacencia
        if( i > j) posibilities.insert(make_pair(m[j][i], j));  //insertamos la distancia entre las ciudades y a qué ciudad va
        else if( i< j) posibilities.insert(make_pair(m[i][j], j)); //Si no está en el triángulo superior obtenemos la coordenada simétrica
      }
    }

    //Como el set ordena automáticamente sus componentes, en la primera posición estará la mínima distancia
    it= posibilities.begin();
    min_dist= it->first;
    destination= it->second;

    //Sumamos la distancia a la cantidad de camino recorrido
    l+=min_dist;

    //Añadimos la ciudad destino a la lista de ciudades recorridass
    r.push_back(destination);
    visited[destination]= true;
    //Nos situamos en la ciudad destino y buscamos desde ahí el mínimo camino a la siguiente que no haya sido recorrida
    i=destination;

    posibilities.clear();
  }

  //Para cerrar el camino
  r.push_back( (*r.end()) );
  l+=euclidean_distance( cities[r[0]], cities[r[r.size()-2]] );

  return r;
}

int graph::west_city(){
  double x=LONG_MAX;
  int index;
  for(int i=0; i<cities.size(); i++)
    if( cities[i].first < x ){
      x=cities[i].first;
      index=i;
    }
  return index;
}

int graph::east_city(){
  double x=-LONG_MAX;
  int index;
  for(int i=0; i<cities.size(); i++)
    if( cities[i].first > x ){
      x=cities[i].first;
      index=i;
    }
  return index;
}

int graph::north_city(){
  double x=-LONG_MAX;
  int index;
  for(int i=0; i<cities.size(); i++)
    if( cities[i].second > x ){
      x=cities[i].second;
      index=i;
    }
  return index;
}

double graph::total_weight(vector<int> path){
  double l;
  for(int i=0; i<path.size()-1; i++)
    l+=get_weight(path[i], path[i+1]);
}

vector<int> graph::min_path2(int i, double &l)
{
  vector<int> r;
}

#endif