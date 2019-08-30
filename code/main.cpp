#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "classes.h"
#include "functions.h"

#define Num_Threads 4
#define iter_threshold 15

using namespace std;
using namespace std::chrono;

string input_file, output_file;
int beam_size = 100;
float random_walk_threshold = 0.15;	// out of every 100 moves 2 will be the random walk
int time_threshold2 = 1000;
int time_threshold1 = 3000;
float dash_cost;
float Time;
int v_size;
int K;
float *cost_matrix; // Matching array
vector<char> vocab;
time_point<system_clock> start_time = system_clock::now();
//global variable for state with minimum cost so far
state result = state();
float min_cost = INFINITY;
float end_time;
//Data for the multithreading functioni
struct thread_data
{
	vector<state> begin_states;
	bool tabu;
	bool restart;
	bool stochastic;
	int beam_size;
	int string_length;
	int max_string_length;
	thread_data(vector<state> begin_states, bool tabu, bool restart, bool stochastic, int beam_size, int string_length, int max_string_length)
	{
		this->begin_states = begin_states;
		this->tabu = tabu;
		this->restart = restart;
		this->stochastic = stochastic;
		this->beam_size = beam_size;
		this->string_length = string_length;
		this->max_string_length = max_string_length;
	}
	thread_data(){};
};
static pthread_mutex_t mutex;
//function for MultiThreading
void* search(void* arg)
{
	cout << "starting search...\n";
	thread_data*  data = (thread_data*)arg;
	vector<state> begin_states = data -> begin_states;
	bool tabu = data -> tabu;
	bool restart = data -> restart;
	bool stochastic = data -> stochastic;
	int beam_size = data -> beam_size;
	int string_length = data -> string_length;
	int max_string_length = data -> max_string_length;
	int iter_num = 0; //number of iterations with no change in global minimum
	vector<state> temp_begin_states;
	while(string_length <= max_string_length)
	{
		beam_size = 100 * sigmoid(string_length%4);
		cout << "beam size: " << beam_size << endl;
		cout << "string length: " << string_length << endl;
		cout << "iteration threshold: " << iter_threshold*string_length << endl;
		while(iter_num < iter_threshold*string_length && (duration_cast<milliseconds>(system_clock::now() - start_time).count() < Time - time_threshold1))
//		while(iter_num < iter_threshold && (duration_cast<milliseconds>(system_clock::now() - start_time).count() < 10000))

		{
			float temp_min = min_cost;
			state temp_result = result;
			vector<state> random, greedy; //neighbour and greedy preferred states of all k^2 neighbours
			int n1 = 0; // n1 -> number of start states preferring random walk
			int n2; // n2 -> number of start states preferring greedy climbing
			for(int i=0; i<beam_size; i++)
			{
				float prob = prob_greedy();	// radom value from 0 to 1
				if( not (duration_cast<milliseconds>(system_clock::now() - start_time).count() < Time - time_threshold2))	break;
//				if( not (duration_cast<milliseconds>(system_clock::now() - start_time).count() < 10000))	break;
				vector<state> neighs = begin_states[i].neighbourhood_states(prob, tabu, restart, stochastic, beam_size);
				if(prob < random_walk_threshold)
				{
					for(int j=0; j<neighs.size(); j++)
						random.push_back(neighs[j]);
					n1++;
				}
				else
					for(int j=0; j<neighs.size(); j++)
					{
						greedy.push_back(neighs[j]);		//###############
					}
	      		}
			n2 = beam_size - n1;
	      		int n1_ = random.size();									//##############
	      		int n2_ = greedy.size();									//#############
	      		for(int j=0; j<n1; j++)
	      		{
				int x = rand()%n1_;
				float temp_cost = random[x].get_cost();
				if(temp_min > temp_cost)
				{
		 	 		temp_min = temp_cost;
					temp_result = random[x];
				}
				temp_begin_states.push_back(random[x]);
				random.erase(random.begin()+x);
				n1_ --;
	      		}
	      		sort(greedy.begin(), greedy.end(), compare_states);
	      		if(n2 != 0 and temp_min > greedy[0].get_cost())					//##############
	      		{
				temp_min = greedy[0].get_cost();
				temp_result = greedy[0];
	      		}
	     		for(int j=0; j<n2; j++)	temp_begin_states.push_back(greedy[j]);			//#############

	      		begin_states =  temp_begin_states;
	      		temp_begin_states.clear();
	      		if(temp_min < min_cost)
	     		{
				iter_num = 0;
				pthread_mutex_lock(&mutex);
				min_cost = temp_min;
				result = temp_result;
//				result.print();
				pthread_mutex_unlock(&mutex);
	      		}
	      		else	iter_num++;
    		}
    		if(iter_num < iter_threshold*string_length)	// running out of time
    			break;
		string_length += Num_Threads;
		iter_num = 0;
//		begin_states = get_k_beam_points(begin_states[0].get_gene_sequences(), string_length, beam_size);
		begin_states = get_k_beam_points(begin_states[0].get_gene_sequences(), string_length, 200*sigmoid(string_length%4));
	}
	pthread_exit(NULL);
}

//MAIN function
int main(int argc, char**argv)
{
	cout << "parsing start...\n";
	srand(time(0));
	input_file = argv[1];
	output_file = argv[2];
  	bool equal_length = true;
	int current_length = 0;
	state state1 = state();
	state state2 = state();
  int length_max = 0;
  int sum_lengths = 0;
	// ------------------------------------------- file PARSING start ---------------------------------------------------
	ifstream inp;
	inp.open(input_file);
	string line;
	while(inp)
	{
		getline(inp, line);
		Time = 60*1000*stof(line); //time in milliseconds
		cout << "Time: " << Time << endl;
		getline(inp, line);
		v_size = stoi(line);
		getline(inp, line);
		//VOCABULARY
		for (int i = 0; i < v_size; i++)
			vocab.push_back(line[3 * i]);
		getline(inp, line);
		K = stoi(line);
		//input strings
		for (int i = 0; i < K; i++)
		{
			getline(inp, line);
			vector<int> temp;
      			int len = line.length();
			if(i==0) current_length = len;
			if( equal_length and current_length != len)	equal_length = false;
		      	sum_lengths += len;
		      	if(len > length_max)
				length_max = len;
			for (int j = 0; j < len; j++)
			{
				char temp1 = line[j];
				for(int k=0; k<v_size; k++)
				{
					if(vocab[k] == temp1)
					{
						temp.push_back(k);
						break;
					}
					else if(temp1 == '-')
					{
						temp.push_back(v_size);
						break;
					}
				}
			}
			state1.push_init_seq(temp);
		}
		getline(inp, line);
		dash_cost = stof(line);
		//M matrix
		cost_matrix = (float*)malloc((v_size + 1)*(v_size + 1) * sizeof(float));
		for (int i = 0; i < v_size + 1; i++)
		{
			for (int j = 0; j < v_size + 1; j++)
			{
				float temp;
				inp >> temp;
				cost_matrix[i*(v_size+1) + j] = temp;
			}
		}
		// pointer is in the same line of cost matrix.
		getline(inp,line);
		// Now pointer points to next line, so we can read #
		getline(inp, line);
		if (line == "#") {break;}
	}
	cout << "parsing done...\n";
	// ------------------------------------------- file PARSING end ---------------------------------------------------

	// ------------------------------------------- random walk example start-------------------------------------------
//	state1.print();
//	vector<state> neigh_states = state1.neighbourhood_states(0, false, false, false, 3);
//	for(int i=0; i< neigh_states.size(); i++)
//		neigh_states[i].print();
	// ------------------------------------------- random walk example end-------------------------------------------

	// ------------------------------------------- greedy best k neighbours example start-------------------------------------------
//	state1.print();
//	vector<state> neigh_states = state1.neighbourhood_states(1, false, false, false, 3);
//	for(int i=0; i< neigh_states.size(); i++)
//		neigh_states[i].print();
	// ------------------------------------------- greedy best k neighbour example end-------------------------------------------

	if(equal_length)
	{
		vector<vector<int> > state1_gene = state1.get_gene_sequences();
		for(int i=0; i<K; i++)	state2.push_gene_seq(state1_gene[i]);
		min_cost = state2.get_cost();
		result = state2;
//		result.print();
	}
  //------------Main program--------------------
  //MultiThreading starts
  pthread_t threads[Num_Threads];
  thread_data temp_data[Num_Threads];
  int i0=0;
  if(equal_length)	i0 = 1;
  cout << "creating threads...\n";
  int new_beam_size;
  for(int i=i0; i<Num_Threads + i0; i++)
  {
//    new_beam_size = beam_size*sigmoid(length_max+i);
    vector<state> start = get_k_beam_points(state1.get_gene_sequences(), length_max+i, beam_size); // random start states
    if(i==i0)	start[0].print(); 
    temp_data[i-i0] = thread_data
    (
      start,
      false,
      false,
      false,
      beam_size,//beam_size;
      length_max+i,//string_length;
      sum_lengths//max_string_length
    );
    pthread_create(&threads[i-i0], NULL, search, (void*)(&temp_data[i-i0]));
  }
	for(int i=0; i<Num_Threads; i++)	pthread_join(threads[i], NULL);
	cout << duration_cast<microseconds>(system_clock::now() - start_time).count() /1000000.0 << endl;
	result.print();
	cout << duration_cast<microseconds>(system_clock::now() - start_time).count() /1000000.0 << endl;
	cout << "threads joined...\n";
	return 0;
}
