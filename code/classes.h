#include<vector>
using namespace std;
class state
{
	private:
		vector<vector<int>*> genes;
		float cost;
	public:
		state();
		void push_gene_seq(vector<int>*);
		float get_cost();	
};
