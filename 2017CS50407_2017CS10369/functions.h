#pragma once
#include "classes.h"
#include<vector>
using namespace std;

// returns indices of the dashes present in argument string
vector<int> get_dash_indices(vector<int> str);

// given a string, index of dash, probability of swapping the dash with left charecter
// following function returns new swapped string
vector<int> swap_string(vector<int> gene, int index, int prob_left_move);

//size of vocabulary
int get_number_of_dashes(vector<int> gene);

// total match cost
float get_intergene_cost(vector<int> gene1, vector<int> gene2);

// will delete the charecter from original index insert it just before the target_index
vector<int> delete_push_before(vector<int> gene, int char_index, int target_index);

// given gene_sequences, length of final states, number of states required 
// will return k random states of length specified in the parameter list
// remember to seed for random function
vector<state> get_k_beam_points(vector<vector<int> > gene_sequences, int state_length, int number_of_states);
//comparator
bool compare_states (state s1, state s2);

float prob_greedy();
