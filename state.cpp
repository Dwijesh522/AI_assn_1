// state: array of pointers to strings
// thete are k strings
#include<iostream>
#include<vector>
#include<ctime>
#include "classes.h"
#include "functions.h"

using namespace std;
//methods of the class: state
//constructor
state::state()
{
	this->cost = 0;
	this->string_length = 0;
}
float state::get_cost()
{
	return cost;
}
void state::modify_cost(float cost)
{
	this->cost = cost;
}
void state::print()
{
	cout << "---------------------------------\n";
	for(int i=0; i<K; i++)
	{
		for(int j=0; j<this->string_length; j++)
			cout << genes[i][j] << " ";
		cout << endl;
	}
	cout << "cost of this state is: " << this->cost << endl;
	cout << "string length: " << string_length << endl;
}
// add a gen sequence to state
void state::push_gene_seq(vector<int> p)
{
	this->string_length = p.size();
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
	this->string_length = p.size();
	genes.push_back(p);
}
// returns one random neighbouring state
// if no neighbour state exists then returns an empty state.
state state::get_random_shuffling()
{
	state neighbour;
	int random_str_ind = rand()%K;
	bool any_dash = false;
	int counter = 0;	//number of strings that does not have any dashes
	while( (not any_dash) and counter < K)
	{
		vector<int> kth_string = genes[random_str_ind];		 // kth randomly picked string
		vector<int> dash_indices = get_dash_indices(kth_string); // function defined in functions.h file
		int number_of_dash_indices = dash_indices.size();
		if(number_of_dash_indices)
		{
			int random_dash_ind = rand()%number_of_dash_indices;	// select random dash to swap
			any_dash = true;
			for(int i=0; i<K; i++)
			{
				if(i == random_str_ind)
				{
					vector<int> swapped_string = swap_string(this->genes[i], dash_indices[random_dash_ind], rand()%2);
					neighbour.push_gene_seq(swapped_string);
				}
				else
					neighbour.push_gene_seq(this->genes[i]);
			}
		}
		else
		{
			counter++;
			if(random_str_ind <= K-2)	random_str_ind++;
			else				random_str_ind=0;
		}
	}
	return neighbour;
}

vector<state> state::neighbourhood_states(float prob_grd_rnd, bool tabu, bool restart, bool stochastic, int beam_size)
{
	// prob_grd_rnd: with p prob  	:	k greedy neighbours with prob q_dash
	// 		 with 1-p prob	:	k random neighbours
	// tabu				:	past history will be saved. tabu=true means use the history
	// 								    tabu=false means not to use hisory
	// 					it is used for duplicate check
	// restart			: 	randomly select k state to come out of the local extremas
	// stochastic			:	select k states with probability q_dash among good states
	// 					if stochastic = true : q_dash = fxn of steepness of the state
	// 					else		     : q_dash = 1 for best k solutions
	// 									0 for others
	// beam_size			: 	 = k = number of neighbours returned.

	vector<state> neighbours;

	//-----------------------------------------------------------------------------------------------
	// Random walk: 							(0, false, false, false, k)
	// Greedy best neighbour, non-tabu, non-restart, non-stochastic:	(1, false, false, false, k)
	// ----------------------------------------------------------------------------------------------

	// Greedy best neighbour, non-tabu, non-restart, non-stochastic
	if(prob_grd_rnd >= 0.5 and not tabu and not restart and not stochastic)
	{
		k_max_heap heap = k_max_heap(beam_size);
		for(int i=0; i<K; i++)	// ith string
		{
			for(int j=0; j<string_length; j++)	// jth character
			{
				if(genes[i][j] == v_size)	// is a dash
				{
					// calculate the new cost due to "LEFT" swapping
					if(j != 0)
					{
						float new_cost = this->cost;
						for(int m=0; m<K; m++)
						{
							if(m != i)
							{
								new_cost += 	(cost_matrix[genes[m][j-1]*(v_size+1) + genes[i][j]] +
										cost_matrix[genes[m][j]*(v_size+1) + genes[i][j-1]] -
										cost_matrix[genes[m][j]*(v_size+1) + genes[i][j]] -
										cost_matrix[genes[m][j-1]*(v_size+1) + genes[i][j-1]]);
							}
						}
						// state with a lower cost and better than the states in the heap, hence a good state
						if(new_cost <= this->cost)
						{
							if(heap.get_heap_size() < beam_size or (new_cost < heap.read_max_cost()))
							{
								neighbour_id temp_neighbour = neighbour_id(i, j, 1, new_cost);
								heap.push(temp_neighbour);
							}
						}
					}
					// calculate the new cost due to "RIGHT" swapping
					if(j != string_length-1)
					{
						float new_cost = this->cost;
						for(int m=0; m<K; m++)
						{
							if(m!= i)
							{
								new_cost += 	(cost_matrix[genes[m][j+1]*(v_size+1) + genes[i][j]]+
										cost_matrix[genes[m][j]*(v_size+1) + genes[i][j+1]]-
										cost_matrix[genes[m][j]*(v_size+1) + genes[i][j]]-
										cost_matrix[genes[m][j+1]*(v_size+1) + genes[i][j+1]]);
							}
						}
						// state with lower cost
						if(new_cost <= this->cost)
						{
							// state is batter than the worst state in the heap
							if(heap.get_heap_size() < beam_size or (new_cost < heap.read_max_cost()))
							{
								neighbour_id temp_neighbour = neighbour_id(i, j, 0, new_cost);
								heap.push(temp_neighbour);
							}
						}
					}
				}
			}
		}
		// ids to real neighbours
		vector<neighbour_id> k_best_neighbour_ids = heap.get_k_best_neighbour_ids();
		int neighbour_id_size = k_best_neighbour_ids.size();
		for(int i=0; i<neighbour_id_size; i++)
		{
			// fatching ith neighbour info
			neighbour_id ith_neighbour_id = k_best_neighbour_ids[i];
			int 	string_index = ith_neighbour_id.get_string_index(), dash_index = ith_neighbour_id.get_dash_index(),
				move_left = ith_neighbour_id.get_move_left();
			float cost = ith_neighbour_id.get_cost();
			//creating a state
			state temp_neighbour = state();
			for(int j=0; j<K; j++)
			{
				if(j != string_index)
					temp_neighbour.push_init_seq(genes[j]);
				else
					temp_neighbour.push_init_seq(swap_string(genes[j], dash_index, move_left));
				temp_neighbour.modify_cost(cost);
			}
			neighbours.push_back(temp_neighbour);
		}
	}
	// random walk, non-tabu, non-restart, non-stochastic
	if(prob_grd_rnd==0 and not tabu and not restart and not stochastic)
	{
		// random walk
		srand(time(0));
		for(int i=0; i<beam_size; i++)
		{
			state neighbour = this->get_random_shuffling();
			neighbours.push_back(neighbour);
		}
	}

	return neighbours;
}
