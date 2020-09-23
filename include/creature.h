#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>


#include "utils.h"


enum porp_t { PREDATOR, PREY };

/*------------------------------------------------------------------------

  Contains creature class and interface

-------------------------------------------------------------------------*/

/* 
  Traits are given at birth and remain constant throughput life 
*/
typedef struct 
{
  size_t gen;
  size_t capacity;
  size_t max_age;
  size_t maturity_age;
  int init_energy;
  size_t metabolism;
  size_t max_nchild;
  size_t birth_cost;
  enum porp_t porp;  
} trait_t;

/* 
  State of creature evolves with time 
*/
typedef struct 
{
  int energy;
  size_t age;
  size_t nchild;
  int position;
  bool acted; //<- Prevent creatures from acting twice in a round
  bool alive;
} state_t;


/* Creature class */
class Creature
{

  /* Attributes */
  public:
    long int ID;
    trait_t traits;
    state_t state;


  /* Functionality */
  public:

    /*
      Default constructor.
    */
    Creature()
    {

    }

    /*
      Constructor by configurable traits
    */
    Creature( trait_t traits )
    {
      this->traits = traits;
      this->state.age = 1;
      this->state.nchild = 0;
      this->state.alive = true;
    }

    /* 
      Creature performs action 
    */
    void action( std::vector<std::vector<int>>& world, std::vector<Creature*>& all_creatures, std::vector<int>& grass )
    {
      if(this->traits.porp == PREDATOR)
        this->predator_action(world, all_creatures);
      else
        this->prey_action(world, all_creatures, grass);      

      /* Mark creature as acted */    
      this->state.acted = true;
    }  

    /* 
      Predator behavior
    */
    void predator_action( std::vector<std::vector<int>>& world, std::vector<Creature*>& all_creatures);

    /* 
      Prey behavior
    */
    void prey_action( std::vector<std::vector<int>>& world, std::vector<Creature*>& all_creatures, std::vector<int>& grass);

};

#endif