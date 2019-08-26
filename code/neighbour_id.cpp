#include "classes.h"

neighbour_id::neighbour_id(int string_index, int dash_index, int move_left, float cost)
{
	this->string_index = string_index;
	this->dash_index = dash_index;
	this->move_left = move_left;
	this->cost = cost;
}
int neighbour_id::get_string_index()
{
	return string_index;
}
int neighbour_id::get_dash_index()
{
	return dash_index;
}
int neighbour_id::get_move_left()
{
	return move_left;
}
float neighbour_id::get_cost()
{
	return cost;
}
