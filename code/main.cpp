#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "classes.h"
#include "functions.h"
#include <ctime>
using namespace std;
 float dash_cost;
 float Time;
 int v_size;
 int K;
 float *cost_matrix; // Matching array
 vector<char> vocab;

//MAIN function
int main()
{
	state state1 = state();
	// ------------------------------------------- file PARSING start ---------------------------------------------------
	ifstream inp;
	inp.open("input.txt");
	string line;
	while(inp)
	{
		getline(inp, line);
		Time = stof(line);
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
			for (int j = 0; j < line.length(); j++)
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
				cost_matrix[i*(v_size + 1) + j] = temp;
			}
		}
		// pointer is in the same line of cost matrix.
		getline(inp,line);
		// Now pointer points to next line, so we can read #
		getline(inp, line);	
		if (line == "#") {break;}
	}	
	state1.print();
	vector<state> beam_states = get_k_beam_points(state1.get_gene_sequences(), 8, 4);
	for(int i=0; i<beam_states.size(); i++)	beam_states[i].print();	
	// ------------------------------------------- file PARSING end ---------------------------------------------------
	// ------------------------------------------- random walk example start-------------------------------------------
//	state1.print();
//	vector<state> neigh_states = state1.neighbourhood_states(0, false, false, false, 3);
//	for(int i=0; i< neigh_states.size(); i++)
//		neigh_states[i].print();
	// ------------------------------------------- random walk example end-------------------------------------------
	// ------------------------------------------- greedy best k neighbours example start-------------------------------------------
//	state1.print();
//	vector<state> neigh_states = state1.neighbourhood_states(1, false, false, false, 1);
//	for(int i=0; i< neigh_states.size(); i++)
//		neigh_states[i].print();
	// ------------------------------------------- greedy best k neighbour example end-------------------------------------------
	return 0;
}
