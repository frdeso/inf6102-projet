#ifndef _AGENT_H_
#define _AGENT_H_

#include <vector>
#include "grid.hpp"
#include "utils.hpp"
using namespace std;
class  agent{
	public:
		agent();
		agent(const agent &a);
		void print() const;
		direction chooseDirection(grid &g);
		double compMaxTileCrit(grid g);
		double compMaxSumCrit(grid g);
		double compNbTileCrit(grid g);
		double compHoriMonoto(grid g);
		double compVertMonoto(grid g);
		void normalize();
		std::vector<double> getCriterion();

	private:
		vector<double> crits_;
		//double critTotalsum_;
		//double critMaxTile_;
		//double critnbTile_;
};

#endif //_AGENT_H_