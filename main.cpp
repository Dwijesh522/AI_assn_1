#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "classes.h"

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
	//file PARSING
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
		{
			vocab.push_back(line[3 * i]);
			cout << line[3*i] << endl;
		}
		getline(inp, line);
		K = stoi(line);
		cout << K << endl;
		//input strings
		for (int i = 0; i < K; i++)
		{
			cout << i << endl;
			getline(inp, line);
			vector<int> temp;
			cout << line << endl;
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
				}
			}
			cout << "debug1" << endl;
			state1.push_gene_seq(temp);
			cout << "debug2" << endl;
		}
		getline(inp, line);
		dash_cost = stof(line);
		//M matrix
		cost_matrix = (float*)malloc((v_size + 1)*(v_size + 1) * sizeof(float));
		for (int i = 0; i < v_size + 1; i++)
		{
			for (int j = 0; j < v_size + 1; j++)
			{
				cout << 3 << endl;
				float temp;
				inp >> temp;
				cost_matrix[i*(v_size + 1) + j] = temp;
			}
		}
		//finish parsing
		getline(inp,line);
		if (line == "#") {break; cout << "end";}
	}

	return 0;
}
