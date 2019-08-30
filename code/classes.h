#pragma once
#include<vector>
#include<string>
#include<algorithm>


// #include <stdio.h>
// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <cmath>

using namespace std;

//GLOBAL Variables
//extern int beam_size;								//******
extern float random_walk_threshold;
extern float dash_cost;
extern float Time;
extern int v_size;
extern int K;
extern float *cost_matrix; // Matching array
extern vector<char> vocab;
extern string output_file;
extern string input_file;

class neighbour_id
{
	private:
		int string_index, dash_index, move_left;
		float cost;
	public:
		neighbour_id(int, int, int, float);
		int get_string_index();
		int get_dash_index();
		int get_move_left();
		float get_cost();
};
class k_max_heap
{
	private:
		vector<neighbour_id> neighbours;
		int k;
	public:
		k_max_heap(int);
		void push(neighbour_id);
		float read_max_cost();
		int get_heap_size();
		vector<neighbour_id> get_k_best_neighbour_ids();
};
class state
{
private:
	int string_length;
	vector<vector<int> > genes;
	float cost;
public:
	state();
	void push_gene_seq(vector<int>);
	void push_init_seq(vector<int> p);
	vector<state> neighbourhood_states(float prob_grd_rnd, bool tabu, bool restart, bool stochastic, int beam_size);
	state get_random_shuffling();
	float get_cost();
	void modify_cost(float);
	void print();
	vector<vector<int> > get_gene_sequences();
};
