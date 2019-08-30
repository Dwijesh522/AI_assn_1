#include<iostream>
#include<algorithm>
#include<cmath>
#include "classes.h"
using namespace std;
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
//	it is a max heap
//	always maintains fix number of elements in the heap
//	ordered wrt cost of a neighbour_id
// ------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
bool compare_neighbour_cost(neighbour_id n1, neighbour_id n2)
{
	return n1.get_cost()<n2.get_cost();
}

k_max_heap::k_max_heap(int k)
{
	make_heap(neighbours.begin(), neighbours.end(), compare_neighbour_cost);
	this->k = k;
}
void k_max_heap::push(neighbour_id neighbour)
{
	neighbours.push_back(neighbour);
	push_heap(neighbours.begin(), neighbours.end(), compare_neighbour_cost);
	if(neighbours.size() == k+1)	
	{
		pop_heap(neighbours.begin(), neighbours.end(), compare_neighbour_cost);
		neighbours.pop_back();
	}
}
float k_max_heap::read_max_cost()
{
	if(neighbours.size() == 0)	return INFINITY;
	return neighbours.front().get_cost();
}
vector<neighbour_id> k_max_heap::get_k_best_neighbour_ids()
{
	return neighbours;
}
int k_max_heap::get_heap_size()
{
	return neighbours.size();
}
