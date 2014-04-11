#ifndef _AGENT_H_
#define _AGENT_H_

#include <vector>
#include "grid.hpp"
#include "utils.hpp"
using namespace std;
class  agent{
	public:
		agent();
		agent(agent &a);
		direction chooseDirection(grid &g);

	private:
		vector<double> chromo_;
};

#endif //_AGENT_H_