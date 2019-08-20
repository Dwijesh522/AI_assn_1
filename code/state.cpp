// state: array of pointers to strings
// thete are k strings
#include<iostream>
#include<vector>
#include "classes.h"

using namespace std;
//size of vocabulary
int v=4;
float dash_cost=3;
int cost_matrix[5][5] = {{0,2,2,2,1}, {2,0,2,2,1}, {2,2,0,2,1}, {2,2,2,0,1}, {1,1,1,1,0}};

int get_number_of_dashes(vector<int> gene)
{
	int size = gene.size(), dashes = 0;
	for(int i=0; i<size; i++)
		if(gene[i] == v)
			dashes++;
	return dashes;
}
float get_intergene_cost(vector<int> gene1, vector<int> gene2)
{
	float intergene_cost = 0;
	int size = gene1.size();
	for(int i=0; i<size; i++)
		intergene_cost += cost_matrix[gene1[i]][gene2[i]];
	return intergene_cost;
}

//methods of the class: state
//constructor
state::state()
{
	this->cost = 0;
}
// add a gen sequence to state
void state::push_gene_seq(vector<int>* p)
{
	genes.push_back(p);
	//modify the cost
	int size = genes.size();
	// conversion cost
	cost += get_number_of_dashes(*p)*dash_cost;
	// matching cost
	for(int i=0; i<size-1; i++)
		cost += get_intergene_cost(*genes[i], *p);
}
// returns cost of this state
float state::get_cost()
{
	return this->cost;
}


