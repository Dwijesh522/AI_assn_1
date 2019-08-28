#include "classes.h"
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

//comparator
bool compare_states (state s1, state s2)
{
  return (s1.get_cost() < s2.get_cost());
}

float prob_greedy(state s)
{
	float res = 1;
	//INCOMPELETE
	return res;
}
