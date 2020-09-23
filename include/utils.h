#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <set>
#include <unordered_set>
#include <deque>
#include <unordered_map>


typedef struct init_t{

 size_t max_grass;
 int init_grass;
 size_t nprey_init;
 size_t npred_init;
 size_t max_pred_age; 
 size_t max_prey_age; 
 size_t prey_maturity_age;
 size_t pred_maturity_age; 
 size_t prey_capacity;
 size_t pred_capacity; 
 size_t prey_init_energy; 
 size_t pred_init_energy;
 size_t prey_birth_cost; 
 size_t pred_birth_cost;
 size_t prey_metabolism; 
 size_t pred_metabolism;
 size_t prey_max_nchild; 
 size_t pred_max_nchild;

} init_t;


/*
  Loads undirected graph from file.
  The undirected graph acts as the world ``topology'' 
*/
std::vector<std::vector<int>> load_graph( std::string fname );


/*
  Read the initialization file
*/
init_t load_init( std::string init_config );

/*
  Save census history
*/
void save_history(std::string fout, std::vector<size_t>& nprey_hist, std::vector<size_t>& npred_hist, std::vector<size_t>& grass_hist);

/*
  Assumes 0 <= max <= RAND_MAX. Returns in the closed interval [0, max] 
   Use srandom() first.. 
  Taken from: https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range
 */
size_t random_at_most( size_t max); 

/*
 Print configurations
*/
void print_config(init_t conf);

// /* 
//   BFS of range k on the world graph
// */
// std::vector<int> bfs( std::vector<std::vector<int>>& graph, int pos, size_t k);

#endif