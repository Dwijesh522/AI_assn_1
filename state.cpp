// state: array of pointers to strings
// thete are k strings
#include<iostream>
#include<vector>
#include "classes.h"

using namespace std;

//size of vocabulary

int get_number_of_dashes(vector<int> gene)
{
	int size = gene.size(), dashes = 0;
	for (int i = 0; i < size; i++)
		if (gene[i] == v_size)
			dashes++;
	return dashes;
}
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

//methods of the class: state
//constructor
state::state()
{
	this->cost = 0;
}
// add a gen sequence to state
void state::push_gene_seq(vector<int> p)
{
//	vector<int>* temp = (vector<int>*)malloc(sizeof(p));
	
//	*temp = p;
	genes.push_back(p);
	//modify the cost
	int size = genes.size();
	// conversion cost
	cost += get_number_of_dashes(p)*dash_cost;
	// matching cost
	for (int i = 0; i < size - 1; i++)
		cost += get_intergene_cost(genes[i], p);
}
// while initially storing the strings into the states, we can not calculate its cost, bcz cost matrix has not been read yet.
void state::push_init_seq(vector<int> p)
{
	genes.push_back(p);
}
// returns cost of this state
float state::get_cost()
{
	return this->cost;
}
