#include<iostream>
#include<vector>
#include "classes.h"

using namespace std;

int main()
{
	vector<int> gene1, gene2, gene3;
	// -ACTGTGA
	(gene1).push_back(4);
	(gene1).push_back(0);
	(gene1).push_back(1);
	(gene1).push_back(2);
	(gene1).push_back(3);
	(gene1).push_back(2);
	(gene1).push_back(3);
	(gene1).push_back(0);
	// TACT--GC
	(gene2).push_back(2);
	(gene2).push_back(0);
	(gene2).push_back(1);
	(gene2).push_back(2);
	(gene2).push_back(4);
	(gene2).push_back(4);
	(gene2).push_back(3);
	(gene2).push_back(1);
	// -ACTG--A
	(gene3).push_back(4);
	(gene3).push_back(0);
	(gene3).push_back(1);
	(gene3).push_back(2);
	(gene3).push_back(3);
	(gene3).push_back(4);
	(gene3).push_back(4);
	(gene3).push_back(0);
	
	state state1 = state();
	state1.push_gene_seq(&gene1);
	cout << state1.get_cost() << endl;
	state1.push_gene_seq(&gene2);
	cout << state1.get_cost() << endl;
	state1.push_gene_seq(&gene3);

	cout << state1.get_cost() << endl;
	return 0;
}
