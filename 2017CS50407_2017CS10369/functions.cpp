#include "functions.h"
#include "classes.h"
#include <iostream>
#include<vector>
using namespace std;

// returns indices of the dashes present in argument string
vector<int> get_dash_indices(vector<int> str)
{
	vector<int> dash_indices;
	int size = str.size();
	for(int i=0; i<size; i++)
		if(str[i] == v_size)
			dash_indices.push_back(i);
	return dash_indices;
}
// given a string, index of dash, probability of swapping the dash with left charecter
// following function returns new swapped string
vector<int> swap_string(vector<int> gene, int index, int prob_left_move)
{
	vector<int> swapped_string;
	int gene_size = gene.size();
	// can not be moved left so move dash to right
	if(index == 0)	prob_left_move = 0;
	else if(index == gene_size-1)	prob_left_move = 1;
	// index th element is a dash
	// first: (index-prob_left_move) == either a (index)th or (index-1)th charecter
	if(prob_left_move)
	{
		for(int i=0; i<gene_size; i++)
			if(i == index-1)
				swapped_string.push_back(v_size);
			else if(i == index)
				swapped_string.push_back(gene[i-1]);
			else
				swapped_string.push_back(gene[i]);
	}
	else
	{
		for(int i=0; i<gene_size; i++)
			if(i == index)
				swapped_string.push_back(gene[i+1]);
			else if(i == index+1)
				swapped_string.push_back(v_size);
			else
				swapped_string.push_back(gene[i]);
	}
	return swapped_string;
}
//size of vocabulary
int get_number_of_dashes(vector<int> gene)
{
	int size = gene.size(), dashes = 0;
	for (int i = 0; i < size; i++)
		if (gene[i] == v_size)
			dashes++;
	return dashes;
}
// total match cost
float get_intergene_cost(vector<int> gene1, vector<int> gene2)
{
	float intergene_cost = 0;
	int size;
	// initially for the first time, while reading the file, sizes of the strings will not be same. Afterwords it must be the same
	if(gene1.size() <= gene2.size())	size = gene1.size();
	else					size = gene2.size();
		
	for (int i = 0; i < size; i++)
		intergene_cost += cost_matrix[gene1[i]*(v_size+1)+gene2[i]];
	return intergene_cost;
}
// will delete the charecter from original index insert it just before the target_index
vector<int> delete_push_before(vector<int> gene, int char_index, int target_index)
{
	int char_val = gene[char_index];
	if(char_index > target_index)
	{
		gene.erase(gene.begin()+char_index);
		gene.insert(gene.begin()+target_index, char_val);
	}
	else if(char_index < target_index)
	{
		gene.erase(gene.begin()+char_index);
		gene.insert(gene.begin()+target_index-1, char_val);
	}
	return gene;
}
// given gene_sequences, length of final states, number of states required 
// will return k random states of length specified in the parameter list
// remember to seed for random function
vector<state> get_k_beam_points(vector<vector<int> > gene_sequences, int state_length, int number_of_states)
{
	vector<state> k_beam_points;
	// equalising each string length to state_length
	for(int i=0; i<K; i++)
	{
		int gene_length = gene_sequences[i].size();
		while(gene_length != state_length)
		{
			gene_sequences[i].push_back(v_size);
			gene_length++;
		}
	}

	// generating (number_of_states) states
	for(int i=0; i<number_of_states; i++)
	{
		state random_state = state();
		for(int j=0; j<K; j++)	// string index
		{
			vector<int> gene = gene_sequences[j];
			//0.5 probability of selecting thisi string
			float prob_string = ((double) rand() / (RAND_MAX));
			if(prob_string >= 0.5)									// changable
			{
				// finding the dash in this string
				for(int m=0; m<state_length; m++)	// dash index
				{
					if(gene[m] == v_size)	// a dash
					{
						//0.5 probability of selecting this dash
						float prob_dash = ((float) rand() / (RAND_MAX));
						if(prob_dash >= 0.5)
						{
							// selecting index of jump
							int random_jump_index = rand()%state_length;
							gene = delete_push_before(gene, m, random_jump_index);
						}
					}
				}
			}
			random_state.push_gene_seq(gene);
		}
		k_beam_points.push_back(random_state);
	}
	return k_beam_points;
}
//comparator
bool compare_states (state s1, state s2)
{
  return (s1.get_cost() < s2.get_cost());
}

float prob_greedy()
{
	float res = 1;
	res = ((float) rand() / (RAND_MAX));
	return res;
}
