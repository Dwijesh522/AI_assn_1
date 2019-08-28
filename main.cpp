
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
#define iter_threshold 100

using namespace std;
using namespace std::chrono;

float dash_cost;
float Time;
int v_size;
int K;
float *cost_matrix; // Matching array
vector<char> vocab;
//global variable for state with minimum cost so far
state result = state();
float min_cost = INFINITY;
float end_time;
//Data for the multithreading function
typedef struct thread_data
{
  vector<state> begin_states;
  bool tabu;
  bool restart;
  bool stochastic;
  int beam_size;
  int string_length;
  int max_string_length;
}  thread_data;
//function for MultiThreading
void* search(void* arg)
{
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
    auto current_time = duration_cast<milliseconds>(system_clock::now());
    while(iter_num < iter_threshold && duration_cast<milliseconds>(system_clock::now()) < end_time - 1)
    {
      float temp_min = min_cost;
      state temp_result = result;
      vector<state> random, greedy; //neighbour and greedy preferred states of all k^2 neighbours
      int n1 = 0; // n1 -> number of start states preferring random walk
      int n2; // n2 -> number of start states preferring greedy climbing
      for(int i=0; i<beam_size; i++)
      {
        float prob = prob_greedy(begin_states[i]);
        vector<state> neighs = begin_states[i].neighbourhood_states(prob, tabu, restart, stochastic, beam_size);
        if(prob < 0.5)
        {
          for(int j=0; j<beam_size; j++)
          {
            random.push_back(neighs[j]);
            n1++;
          }
        }
        else
        {
          for(int j=0; j<beam_size; j++)
          {
            greedy.push_back(neighs[j]);
          }
        }
      }
      n2 = beam_size - n1;
      srand(time(0));
      int n1_ = n1 * beam_size;
      int n2_ = n2 * beam_size;
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
      }
      sort(greedy.begin(), greedy.end(), compare_states);
      if(n2 != 0 && temp_min > greedy[0].get_cost())
      {
        temp_min = greedy[0].get_cost();
        temp_result = greedy[0];
      }
      for(int j=0; j<n2; j++)
      {
        temp_begin_states.push_back(greedy[j]);
      }

      begin_states =  temp_begin_states;
      temp_begin_states.clear();
      if(temp_min < min_cost)
      {
        iter_num = 0;
        min_cost = temp_min;
        result = temp_result;
      }
      else
      {
        iter_num++;
      }
    }
    string_length += 4;
  }
  pthread_exit(NULL);
}

//MAIN function
int main()
{
	state state1 = state();
  int length_max = 0;
  int sum_lengths = 0;
	// ------------------------------------------- file PARSING start ---------------------------------------------------
	ifstream inp;
	inp.open("input.txt");
	string line;
	while(inp)
	{
		getline(inp, line);
		float Time = 60*1000*stof(line); //time in milliseconds
    auto end_time = duration_cast<milliseconds>(system_clock::now()) + Time;
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
      sum_lengths += len;
      if(len > length_max) length_max = len;
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
			}
		}
		// pointer is in the same line of cost matrix.
		getline(inp,line);
		// Now pointer points to next line, so we can read #
		getline(inp, line);
		if (line == "#") {break;}
	}
	// ------------------------------------------- file PARSING end ---------------------------------------------------

	// ------------------------------------------- random walk example start-------------------------------------------
//	state1.print();
//	vector<state> neigh_states = state1.neighbourhood_states(0, false, false, false, 3);
//	for(int i=0; i< neigh_states.size(); i++)
//		neigh_states[i].print();
	// ------------------------------------------- random walk example end-------------------------------------------

	// ------------------------------------------- greedy best k neighbours example start-------------------------------------------
	state1.print();
	vector<state> neigh_states = state1.neighbourhood_states(1, false, false, false, 3);
	for(int i=0; i< neigh_states.size(); i++)
		neigh_states[i].print();
	// ------------------------------------------- greedy best k neighbour example end-------------------------------------------

  //------------Main program--------------------
  //MultiThreading starts
  pthread_t threads[Num_Threads];
  for(int i=0; i<Num_Threads; i++)
  {
    vector<state> start; // random start states
    thread_data temp_data =
    {
      start,
      false,
      false,
      true,
      10,//beam_size;
      length_max+i,//string_length;
      sum_lengths,//max_string_length
    };
    pthread_create(&threads[i], NULL, search, (void*)(&temp_data));
  }

  for(int i=0; i<Num_Threads; i++)
  {
    pthread_join(threads[i], NULL);
  }
  result.print();
	return 0;
}