#include "agent.h"
#include <fstream>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <utility>

#define MAX_TILE 0
#define NB_TILE 1
#define TOTAL_SUM 2
#define HORI_MONO  3
#define VERT_MONO 4
#define CORNER 5

#define NEW true
agent::agent(){
	crits_.resize(6);
	for(auto &i : crits_)
	{
		i = (double)rand()/(double)(RAND_MAX);
	}
	normalize();
}

agent::agent(const agent &a){

	crits_.resize(6);
	crits_[MAX_TILE] = a.crits_[MAX_TILE];
	crits_[NB_TILE] = a.crits_[NB_TILE];
	crits_[TOTAL_SUM] = a.crits_[TOTAL_SUM];
	crits_[HORI_MONO] = a.crits_[HORI_MONO];
	crits_[VERT_MONO] = a.crits_[VERT_MONO];
	crits_[CORNER] = a.crits_[CORNER];
}

std::vector<double> agent::getCriterion(){
	return crits_;
}
void agent::print() const
{
	cout<<"";
	for(auto i : crits_)
	{
		cout<<std::fixed;
		cout<<setprecision(3)<<i<< "\t" ;
	}
}

ostream& operator<<(ostream &out,  const agent &agent)
{
	out<<"| ";
	for(auto i : agent.crits_)
	{
		out<<std::fixed;
		out<<setprecision(5)<<i<< " " ;
	}
	out<<"|";
	return out;
}

ifstream& operator>>(ifstream &in,  agent &agent)
{
	for (auto &i : agent.crits_)
	{
		in >> i;
	}
	return in;
}

direction agent::chooseDirection(grid g){
	vector<pair<grid, double> > possibleOutcomes; // std::pair n'a pas de compare < et > trouver solution

	//Create one copy for each possible moves
	for (int i = 0; i < 10; ++i)
	{
		grid n = grid(g);
		grid s = grid(g);
		grid w = grid(g);
		grid e = grid(g);


		possibleOutcomes.push_back(make_pair (n, (double)direction::NORTH));
		possibleOutcomes.push_back(make_pair (s, (double)direction::SOUTH));
		possibleOutcomes.push_back(make_pair (e, (double)direction::EAST));
		possibleOutcomes.push_back(make_pair (w, (double)direction::WEST));
	}
	/*
		On verifie que des mouvements sont possibles dans les differentes directtions.
	*/
	for(auto &iter : possibleOutcomes)
	{	
		bool canmove = iter.first.action((direction)iter.second);

		if(!canmove)
		{
			iter.second = -1.0;
		}
	}

	vector<double> totalScore;
	totalScore.resize(4);
	direction bestMove = direction::SOUTH;

	for(auto &iter : possibleOutcomes)
	{
		if(iter.second != -1.0)
		{
			bestMove = (direction) iter.second;
			break;
		}
	}
	
	double largest = 0;
	double score = 0;
//	int reseed = print_current_time_with_ns();
	for(pair<grid, int> iter : possibleOutcomes)
	{
		if (iter.second == -1.0)
		{
			continue;
		}
//		srand(reseed);
	

		score =	compMaxTileCrit(iter.first)
				+ compMaxSumCrit(iter.first)
				+ compHoriMonoto(iter.first)
				+ compVertMonoto(iter.first)
				+ compCorner(iter.first)
				+ compNbTileCrit(iter.first);

		//On tente d'eviter les mouvements vers le haut.
		if((direction)iter.second == direction::NORTH)
		{
			score *= 0.8;
		}
		totalScore[iter.second] += score;
	}
	
	for(unsigned int i = 0; i < totalScore.size();++i)
	{
		score = totalScore[i];
		if (score > largest)
		{
			largest = score ;
			bestMove = (direction) i;
		}
	}
//	srand(reseed);
	return bestMove;
}

double agent::compMaxTileCrit(grid g)
{
#if NEW
	//return crits_[MAX_TILE]*(double)((log2(g.largest())/11.0));
	return crits_[MAX_TILE]*(double)((double)g.largest()/4096.0);
#else
	return crits_[MAX_TILE]*(double)(g.largest());
#endif
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
#if NEW
	return crits_[TOTAL_SUM]*(double)((sum/8192.0));
#else
	return crits_[TOTAL_SUM]*(double)sum;
#endif
}

double agent::compNbTileCrit(grid g)
{
	double nb = 0.0;
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

#if NEW
	return crits_[NB_TILE] *(double)(1-(nb/16.0));
#else
    return crits_[NB_TILE] *(double)(16 - nb);
#endif
}
double agent::compHoriMonoto(grid g)
{

	uint nbSwitch = 0;
	//g.print();
	for (int j = 0; j < g.size(); ++j)
    {
		bool isIncreasing = false;
		uint nbComp = 0;
        for (int i = 0; i < g.size()-1; ++i)
        {
        	// if both number are the same it's okay
        	//cout<< g.get(i,j)<<","<<g.get(i+1,j)<<endl;
        	if (g.get(i,j) == g.get(i+1,j)) 
			{
				continue;
			}

			if( nbComp < 1 )
			{
				isIncreasing = g.get(i,j) < g.get(i+1,j);
			}
			else
			{
				bool isIncreasingTmp = g.get(i,j) < g.get(i+1,j);
				if(isIncreasingTmp != isIncreasing )
				{
					isIncreasing = isIncreasingTmp;
					nbSwitch++;
				}
			}
			nbComp++;
		}
	}
#if NEW
	return crits_[HORI_MONO] * (double)(1-(nbSwitch/8.0));
#else
	return crits_[HORI_MONO] * (double)(8-nbSwitch);
#endif
}
double agent::compVertMonoto(grid g)
{

	uint nbSwitch = 0;
	//g.print();
	
    for (int i = 0; i < g.size(); ++i)
    {
		bool isIncreasing = false;
		uint nbComp = 0;
		for (int j = 0; j < g.size()-1; ++j)
        {
        	// if both number are the same it's okay
        	//cout<< g.get(i,j)<<","<<g.get(i,j+1)<<endl;
        	int first = g.get(i,j), second =g.get(i,j+1);
        	if (first == second) 
			{
				continue;
			}

			if( nbComp < 1 )
			{
				isIncreasing = first < second;
			}
			else
			{
				bool isIncreasingTmp = first < second;
				if(isIncreasingTmp != isIncreasing )
				{
					isIncreasing = isIncreasingTmp;
					nbSwitch++;
				}
			}
			nbComp++;
		}
	}
#if NEW
	return crits_[VERT_MONO] * (double)(1-(nbSwitch/8.0));
#else
	return crits_[VERT_MONO] * (double)(8-nbSwitch);
#endif
}

double agent::compCorner(grid g)
{
	int largest = g.largest();
	double result = 0.0;
	if (g.get(0,0) == largest) result = 1.0;
	else if (g.get(0,g.size()-1) == largest) result = 1.0;
	else if (g.get(g.size()-1,0) == largest) result = 1.0;
	else if (g.get(g.size()-1,g.size()-1) == largest) result = 1.0;

	return crits_[CORNER] * result;
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
