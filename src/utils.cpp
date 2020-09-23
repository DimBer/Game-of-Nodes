#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>
#include <cassert>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <deque>
#include <dirent.h>
#include "omp.h"

#include "utils.h"


using namespace std;

/*------------------------------------------------------------------------

                        Usefull routines

-------------------------------------------------------------------------*/

/*
  Loads undirected graph from file.
  The undirected graph acts as the world ``topology'' 
*/
vector<vector<int>> load_graph( string fname )
{

    vector<vector<int>> edge_list;

    size_t N = 0;

    /* Read edge list */
    ifstream myfile(fname);
    int node_a{0}, node_b{0};
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            node_a = INT32_MIN;
            node_b = INT32_MIN;
            myfile >> node_a;
            myfile >> node_b;

            N = max( (int) N, max(node_a, node_b));

            if(node_a == INT32_MIN && node_b == INT32_MIN )
                continue;

            edge_list.push_back({node_a, node_b});
        }
        myfile.close();
    }
    else
        cout << "Unable to open graph file";

    /* Convert to graph */

    vector<unordered_set<int>> temp(N, unordered_set<int>());
    for(auto& e: edge_list)
    {
      temp[e[0]-1].insert(e[1]-1);
      temp[e[1]-1].insert(e[0]-1);
    }

    vector<vector<int>> res(N);

    for(size_t i=0; i< N; i++)
      res[i] = vector<int>(temp[i].cbegin(), temp[i].cend());

    return res;
}

/*
  Read the initialization file
*/
init_t load_init( string fname )
{
  string temp;
  init_t res;
  ifstream myfile(fname);
  if (myfile.is_open())
  {
      while(temp != ":") myfile >> temp;

      myfile >> res.max_grass;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.init_grass;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.npred_init;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.nprey_init;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.max_pred_age;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.max_prey_age;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.pred_maturity_age;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.prey_maturity_age;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.pred_capacity;

      myfile >> temp;
      while(temp != ":") myfile >> temp;      

      myfile >> res.prey_capacity;

      myfile >> temp;
      while(temp != ":") myfile >> temp;      

      myfile >> res.pred_birth_cost;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.prey_birth_cost;       

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.pred_metabolism;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.prey_metabolism;       

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.pred_init_energy;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.prey_init_energy;  

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.pred_max_nchild;

      myfile >> temp;
      while(temp != ":") myfile >> temp;

      myfile >> res.prey_max_nchild;  

      myfile.close();
  }
  else
      cout << "Unable to open graph file";


  return res;
}

/*
 Print configurations
*/
void print_config(init_t conf)
{
  cout << "\n\nLOADED CONFIG:\n\n";
  cout << "/********************************************/";
  cout << "\nInit grass: " << conf.init_grass;
  cout << "\nMax grass: " << conf.max_grass;
  cout << "\nMax pred age: " << conf.max_pred_age;
  cout << "\nMax prey age: " << conf.max_prey_age;
  cout << "\nInit # of pred: " << conf.npred_init;
  cout << "\nInit # of prey: " << conf.nprey_init;  
  cout << "\nPred birth cost: " << conf.pred_birth_cost;
  cout << "\nPrey birth cost: " << conf.prey_birth_cost;  
  cout << "\nPred capacity: " << conf.pred_capacity;
  cout << "\nPrey capacity: " << conf.prey_capacity;
  cout << "\nPred init energy: " << conf.pred_init_energy;
  cout << "\nPrey init energy: " << conf.prey_init_energy;
  cout << "\nPred maturity age: " << conf.pred_maturity_age;
  cout << "\nPrey maturity age: " << conf.prey_maturity_age;
  cout << "\nPred max # children: " << conf.pred_max_nchild;
  cout << "\nPrey max # children: " << conf.prey_max_nchild;
  cout << "\nPred metabolism: " << conf.pred_metabolism;
  cout << "\nPrey metabolism: " << conf.prey_metabolism;
  cout << "/********************************************/";  
}


/*
  Save census history
*/
void save_history(string fname, vector<size_t>& nprey_hist, vector<size_t>& npred_hist, vector<size_t>& grass_hist)
{
    assert(nprey_hist.size() == npred_hist.size());

    ofstream myfile;
    myfile.open(fname);

    for(size_t i=0; i<nprey_hist.size(); i++)
        myfile << nprey_hist[i] << " " << npred_hist[i] << " " << grass_hist[i] << "\n";

    myfile.close();
}


/*
  Assumes 0 <= max <= RAND_MAX. Returns in the closed interval [0, max] 
   Use srandom() first.. 
  Taken from: https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range
 */
size_t random_at_most( size_t max) 
{
  unsigned long
    /* max <= RAND_MAX < ULONG_MAX, so this is okay.*/
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  /* Prevents overflow */
  while (num_rand - defect <= (unsigned long)x);

  /* Truncated division is intentional */
  return (size_t) x/bin_size;
}

// /* 
//   BFS of range k on the world graph
// */
// vector<int> bfs( vector<vector<int>>& graph, int pos, size_t k)
// {

// }
