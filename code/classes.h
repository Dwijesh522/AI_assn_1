#pragma once
#include<vector>
using namespace std;

//GLOBAL Variables
extern float dash_cost;
extern float Time;
extern int v_size;
extern int K;
extern float *cost_matrix; // Matching array
extern vector<char> vocab;

class state
{
private:
	int string_length;
	vector<vector<int>> genes;
	float cost;
public:
	state();
	void push_gene_seq(vector<int>);
	void push_init_seq(vector<int> p);
	vector<state> neighbourhood_states(float prob_grd_rnd, bool tabu, bool restart, bool stochastic, int beam_size);
	float get_cost();
};
