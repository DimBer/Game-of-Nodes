#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <random>
#include <cassert>
#include <chrono>
#include <set>

/*
  LET THERE BE LIGHT
*/

#include "creature.h"
#include "opts.h"
#include "utils.h"


using namespace std;


static void creatures_init(vector<Creature*>& all_creatures, init_t init_config );

static bool census(const vector<Creature*>& all_creatures, const vector<int>& grass, const string fout);

/*******************************************************************************************

********************************************************************************************/

#define MAX_YEAR 100000

int main(int argc, char **argv)
{

  /* Get options from cmd args */
  struct my_opt opts = get_options(argc, argv);

  /* Load world */
  vector<vector<int>> world = load_graph( opts.graph );

  /* Load initial configurations */
  init_t init_config = load_init( opts.init_config );

  print_config(init_config);

  /*Vector of pointers to all Creatures (index corresponds to node position) */
  vector<Creature*> all_creatures(world.size(), NULL);

  /* Initialize the ``grass'' */
  vector<int> grass(world.size(), init_config.init_grass);

  srandom( (unsigned int) time(NULL));

  /* Initialize creatures */
  creatures_init( all_creatures, init_config );

 /////////////////////////////////////////////////////////////////////////////////////////////
                    /*  ENTER MAIN LOOP  */
 /////////////////////////////////////////////////////////////////////////////////////////////

  cout << "\n\nLET THERE BE LIGHT\n\n";

  size_t cnt = 1;
  while(census(all_creatures, grass, opts.out))
  {
      cout << "Year " << cnt++ << "\n";

     /* Grass grows by 1 */
     for(auto& g : grass)
      g = min( g + 1, (int) init_config.max_grass);  

     /* Set all creatures to not acted */
      for(auto c: all_creatures)
        if(c != NULL)
          c->state.acted = false; 

     /* Creatures act */ 
      for(auto c: all_creatures)
        if(c != NULL && !c->state.acted)
          c->action(world, all_creatures, grass); 

     /* Remove those who died of old age */ 
      for(size_t i=0; i<all_creatures.size(); i++)
        if(all_creatures[i] != NULL && !all_creatures[i]->state.alive)
        {
          delete all_creatures[i];
          all_creatures[i] = NULL;
        }
  }

 /////////////////////////////////////////////////////////////////////////////////////////////
                    /*  EXIT MAIN LOOP  */
 /////////////////////////////////////////////////////////////////////////////////////////////

  cout << "\n\nWORLD HAS FINISHED\n\n";

  return 0;
}


/*******************************************************************************************

********************************************************************************************/




/*
  Initialize creatures based on the init configuration
*/
static void creatures_init(vector<Creature*>& all_creatures, init_t init_config )
{

  /* First generation of prey */
  size_t nprey = 0;

  trait_t prey_traits;
  prey_traits.porp = PREY; prey_traits.gen = 1; prey_traits.capacity = init_config.prey_capacity;
  prey_traits.maturity_age = init_config.prey_maturity_age; prey_traits.max_age = init_config.max_prey_age;
  prey_traits.init_energy = init_config.prey_init_energy; prey_traits.metabolism = init_config.prey_metabolism;
  prey_traits.max_nchild = init_config.prey_max_nchild; prey_traits.birth_cost = init_config.prey_birth_cost;

  while(nprey < init_config.nprey_init)
  {
    size_t i = random_at_most(all_creatures.size()-1);
    if(all_creatures[i] == NULL)
    {
      Creature* new_prey = new Creature( prey_traits);
      new_prey->state.position = i;
      new_prey->state.energy = init_config.prey_init_energy;

      all_creatures[i] = new_prey;

      nprey++;
    }
  }

  /* First generation of predators */
  size_t npred = 0;

  trait_t pred_traits;
  pred_traits.porp = PREDATOR; pred_traits.gen = 1; pred_traits.capacity = init_config.pred_capacity;
  pred_traits.maturity_age = init_config.pred_maturity_age; pred_traits.max_age = init_config.max_pred_age;
  pred_traits.init_energy = init_config.pred_init_energy; pred_traits.metabolism = init_config.pred_metabolism;
  pred_traits.max_nchild = init_config.pred_max_nchild; pred_traits.birth_cost = init_config.pred_birth_cost;

  while(npred < init_config.npred_init)
  {
    size_t i = random_at_most(all_creatures.size()-1);
    if(all_creatures[i] == NULL)
    {
      Creature* new_pred = new Creature( pred_traits);
      new_pred->state.position = i;
      new_pred->state.energy = init_config.pred_init_energy;

      all_creatures[i] = new_pred;

      npred++;
    }
  }


}


/*
  Measure populations and store reults locally.
  When both populations die out, save results and return false.
*/
static bool census(const vector<Creature*>& all_creatures, const vector<int>& grass, const string fout)
{
  static vector<size_t> nprey_hist, npred_hist, grass_hist;
  static size_t year = 0;
  year++;

  size_t nprey = 0, npred = 0, total_grass = 0;

  for(auto c : all_creatures)
    if(c != NULL)
    {
      if(c->traits.porp == PREDATOR)
        npred++;
      else
        nprey++;    
    }

  if( (nprey == 0 && npred == 0) || year >= MAX_YEAR )
  {
    save_history(fout, nprey_hist, npred_hist, grass_hist);
    return false;
  }

  nprey_hist.push_back(nprey);
  npred_hist.push_back(npred);

  for(auto g : grass)
    total_grass += g;

  grass_hist.push_back(total_grass);  

  cout << "\n# prey: " << nprey << ", # pred: " << npred << ", total grass: "<< total_grass << "\n";  

  return true;
}