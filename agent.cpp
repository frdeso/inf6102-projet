#include "agent.h"
#include <assert.h>
#include <iostream>
#include <utility>

#define MAX_TILE 0
#define NB_TILE 1
#define TOTAL_SUM 2

agent::agent(){
	std::mt19937& r =  rand_gen();
	/*critMaxTile_ = r()/(double)(r.max()+1);
	critTotalsum_ = r()/(double)(r.max()+1);
	critnbTile_ = r()/(double)(r.max()+1);
*/	crits_.resize(3);
	crits_[MAX_TILE] = r()/(double)(r.max()+1);
	crits_[NB_TILE] = r()/(double)(r.max()+1);
	crits_[TOTAL_SUM] = r()/(double)(r.max()+1);
	normalize();
}

agent::agent(const agent &a){
	/*critMaxTile_ = a.critMaxTile_;
	critTotalsum_ = a.critTotalsum_;
	critnbTile_ = a.critnbTile_;
*/
	crits_.resize(3);
	crits_[MAX_TILE] = a.crits_[MAX_TILE];
	crits_[NB_TILE] = a.crits_[NB_TILE];
	crits_[TOTAL_SUM] = a.crits_[TOTAL_SUM];
}
void agent::print() const
{
	cout<<"|";
	cout<<crits_[MAX_TILE]<<"|";
	cout<<crits_[NB_TILE]<<"|";
	cout<<crits_[TOTAL_SUM]<<"|"<<endl;
}
direction agent::chooseDirection(grid &g){
	vector<pair<grid, int> > possibleOutcomes; // std::pair n'a pas de compare < et > trouver solution

	//Create one copy for each possible moves
	grid n = grid(g);
	grid s = grid(g);
	grid w = grid(g);
	grid e = grid(g);

	possibleOutcomes.push_back(make_pair (n, (int)direction::NORTH));
	possibleOutcomes.push_back(make_pair (s, (int)direction::SOUTH));
	possibleOutcomes.push_back(make_pair (e, (int)direction::EAST));
	possibleOutcomes.push_back(make_pair (w, (int)direction::WEST));

	/*
		On verifie que des mouvements sont possibles dans les differentes directtions.
	*/
	for(pair<grid, int> &iter : possibleOutcomes)
	{	
		bool canmove = iter.first.action((direction)iter.second);

		if(!canmove)
		{
			iter.second = -1;
		}
	}


	direction bestMove = direction::SOUTH;

	for(pair<grid, int> &iter : possibleOutcomes)
	{
		if(iter.second != -1)
		{
			bestMove = (direction) iter.second;
			break;
		}
	}

	int largest = 1, i = 0;
	int score = 0;
	for(pair<grid, int> iter : possibleOutcomes)
	{
		score = compMaxTileCrit(iter.first)
				+ compMaxSumCrit(iter.first)
				+ compNbTileCrit(iter.first);
		if (score > largest && iter.second != -1)
		{
			largest = score ;
			bestMove = (direction) iter.second;
		}
	}
	return bestMove;
}

double agent::compMaxTileCrit(grid g)
{
	return crits_[MAX_TILE]*g.largest();
}

double agent::compMaxSumCrit(grid g)
{

	unsigned int sum = 0;
	for(int y = 0; y < g.size(); ++y)
    {
        for(int x = 0; x < g.size(); ++x)
        {
        	sum += g.get(x,y);
        }
    }

	return crits_[TOTAL_SUM]*(double)sum;
}

double agent::compNbTileCrit(grid g)
{
	unsigned int nb = 0;
	for (int i = 0; i < g.size(); ++i)
    {
        for (int j = 0; j < g.size(); ++j)
        {
        	if( g.get(j,i) != 0)
        	{
        		nb++;
        	}
        }
    }
    return crits_[NB_TILE] *( 16 - nb);
}
void agent::normalize()
{
	double sum = 0;
	for(auto i : crits_)
	{
		sum += i;
	}
	for(auto &i : crits_)
	{
		i /= sum;
	}
}