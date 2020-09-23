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

#include <utils.h>

#include <creature.h>

using namespace std;

/******
Main implementations of predator and prey logic
******/

static void procreate(Creature* parent, vector<Creature*>& all_creatures ,int child_position);


/* 
    Prey behavior
*/
void Creature::prey_action( vector<vector<int>>& world, vector<Creature*>& all_creatures, vector<int>& grass)
{
    /* Check if it is time to die of old age */
    if(this->state.age++ >= this->traits.max_age)
    {
        this->state.alive = false;
        return;
    } 

    /* Check if death by starvation occurs */
    this->state.energy -= this->traits.metabolism;
    if(this->state.energy <= 0)
    {
        this->state.alive = false;
        return;
    } 

    /* Eat grass in current spot */
    this->state.energy = min( (int) this->traits.capacity, this->state.energy + (int) grass[this->state.position] );
    grass[this->state.position] = 0;   

    /* If conditions are met (maturity, energy, space), give birth  */
    if(this->state.age >= this->traits.maturity_age && this->state.energy >= 2*this->traits.birth_cost
       && this->state.nchild < this->traits.max_nchild)
    {
        int free_pos = INT32_MIN;
        for(auto n : world[this->state.position]) 
            if(all_creatures[n] == NULL)
            {
                free_pos = n;
                break;
            }

        if(free_pos != INT32_MIN)
            procreate(this, all_creatures, free_pos);
    }


    /* 
        Move to adjacent node with maximum food.
        Avoid cells occupied by other creatures. 
    */
    int new_pos = INT32_MIN, gain = 0;
    for(auto n : world[this->state.position]) 
    if(all_creatures[n] == NULL)
        if(grass[n] > gain)
        {
            gain = grass[n];
            new_pos = n;
        }

    if(new_pos != INT32_MIN)
    {
        all_creatures[this->state.position] = NULL;
        this->state.position = new_pos;
        all_creatures[new_pos] = this;
    }
        
}



/* 
    Predator behavior
*/
void Creature::predator_action( vector<vector<int>>& world, vector<Creature*>& all_creatures)
{
    /* Check if it is time to die of old age */
    if(this->state.age++ >= this->traits.max_age)
    {
        this->state.alive = false;
        return;
    } 

    /* Check if death by starvation occurs */
    this->state.energy -= this->traits.metabolism;
    if(this->state.energy <= 0)
    {
        this->state.alive = false;
        return;
    } 

    /* If conditions are met (maturity, energy, space), give birth  */
    if(this->state.age >= this->traits.maturity_age && this->state.energy >= 2*this->traits.birth_cost
       && this->state.nchild < this->traits.max_nchild)
    {
        int free_pos = INT32_MIN;
        for(auto n : world[this->state.position]) 
            if(all_creatures[n] == NULL)
            {
                free_pos = n;
                break;
            }

        if(free_pos != INT32_MIN)
            procreate(this, all_creatures, free_pos);
    }

    /* Predator hunts the juiciest animal, or takes a random walk if no animals arround */
    vector<int> free_spots;
    size_t best_prey = 0; 
    int best_prey_index ;
    for(auto n : world[this->state.position]) 
    {
        if(all_creatures[n] == NULL)
        {
            free_spots.push_back(n);
            continue;
        }

        if(all_creatures[n]->traits.porp == PREY && all_creatures[n]->state.energy > best_prey )
        {
            best_prey = all_creatures[n]->state.energy;
            best_prey_index = n;
        }
    }

    /* Move to juiciest animal and eat it */
    if(best_prey > 0)
    {
        /* Deed */
        this->state.energy = min( this->traits.capacity, this->state.energy + best_prey);

        /* Delete prey and take its place */
        delete all_creatures[best_prey_index];

        all_creatures[this->state.position] = NULL;
        this->state.position = best_prey_index;
        all_creatures[best_prey_index] = this;        
    }
    else
    {
        /* If no food around, just go to a random neighboring node */   
        if(!free_spots.empty())
        {
            size_t i = random_at_most(free_spots.size()-1);

            all_creatures[this->state.position] = NULL;
            this->state.position = free_spots[i];
            all_creatures[free_spots[i]] = this;        
        }
    }
    
}


/*
    Parent gives birth to a child into a given position
*/
static void procreate(Creature* parent, vector<Creature*>& all_creatures ,int child_position)
{
    /* Birth effects on parent */
    parent->state.nchild++;
    parent->state.energy -= parent->traits.birth_cost;

    /* Generate child */
    Creature* child = new Creature(parent->traits);
    child->state.energy = child->traits.init_energy;
    child->traits.gen = parent->traits.gen + 1;
    child->state.position = child_position;

    all_creatures[child_position] = child;
    child->state.acted = true;
}
