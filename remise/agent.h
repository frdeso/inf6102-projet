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
		friend ostream& operator<< (ostream &out, const agent &agent);
		friend ifstream& operator>> (ifstream &in, agent &agent);
		direction chooseDirection(grid g);
		double compMaxTileCrit(grid g);
		double compMaxSumCrit(grid g);
		double compNbTileCrit(grid g);
		double compHoriMonoto(grid g);
		double compVertMonoto(grid g);
		double compCorner(grid g);
		void normalize();
		std::vector<double> getCriterion();

	private:
		vector<double> crits_;
	};

#endif //_AGENT_H_
