#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <utility>

#include "agent.h"
#include "grid.hpp"
using namespace std;

typedef unsigned int uint;
typedef std::pair<agent, double>  individual;

struct indi_sorter{
    bool operator()(const pair<agent, uint> &l, const pair<agent, uint> &r) const
    {
        return l.second == r.second ? &l < &r : l.second > r.second;
    }
};

int nb2048 = 0;	
double getRandomDoubleBetween(double a, double b)
{
	double random = ((double) rand()) / (double) RAND_MAX;
	double diff = b - a;
	double r = random * diff;
	return a + r;
}

individual crossing( individual &p1, individual &p2)
{
    individual offspring;


    for(uint i = 0; i < offspring.first.getCriterion().size(); ++i)
    {
        //offspring.first.getCriterion()[i] = (p1.first.getCriterion()[i], p2.first.getCriterion()[i])/2.0;
		double a = min(p1.first.getCriterion()[i], p2.first.getCriterion()[i]);
		double b = max(p1.first.getCriterion()[i], p2.first.getCriterion()[i]);
        offspring.first.getCriterion()[i] =  getRandomDoubleBetween(a, b);
    }
    offspring.second = 0;

    offspring.first.normalize();
    return offspring;
}

//individual crossingAllparents(vector<individual> &parents)
//{
//    individual offspring;
//
//
//    for(uint i = 0; i < offspring.first.getCriterion().size(); ++i)
//    {
//        offspring.first.getCriterion()[i] = parents[rand()%parents.size()].first.getCriterion()[i];
//    }
//    offspring.second = 0;
//
//    return offspring;
//}
double getRandPercentage()
{
    return (double)rand()/(double)RAND_MAX;
}

individual mutation(individual i)
{
    individual mutant;
    double chiffre = 1.0;
	
	int j = rand()%mutant.first.getCriterion().size();
    double sign = ((rand()%3) -1);
    mutant.first.getCriterion()[j] = 
               i.first.getCriterion()[j] + ((double)sign)*(i.first.getCriterion()[j] * getRandPercentage())/chiffre;

    mutant.second = 0;
    mutant.first.normalize();

    return mutant;
}

void specificAgent(agent &a)
{
	int nb = 0;
	cout<<a;
	for(int j = 0; j < 10000 ; ++j)
    {
		grid g;
		
		while(g.can_move())
		{
			direction d = a.chooseDirection(g);
			g.action(d);
		}	

		if (g.largest() >= 2048)
		{
			nb++;
			g.print();
		}
		cout<<j<<": "<<nb<<" - "<<g.score()<<endl;
	}
	return;
}

void printPopulation(set<individual,indi_sorter> &pop)
{
	cout<<"population "<<nb2048<<endl;
	for(auto i: pop)
	{
		cout<<i.first<<" "<<(int)i.second<<endl;
	}
}



void fitness(individual &indi, int nbRunPerIndi)
{

	indi.second = 0.0;
	// Nombre d'essais par individu au cours d'une generation
	// pour faire la moyenne
	for(int k = 0; k < nbRunPerIndi; ++k)
	{
		grid g;
		while(g.can_move())
		{
			direction d = indi.first.chooseDirection(g);
			g.action(d);
			//srand(print_current_time_with_ns());
		}
		
		if (g.largest() >= 2048)
		{
			nb2048++;
			//cout<<"\t"<<(int)g.score()<<"\t";
			//cout<<indi.first<<"\t*"<<endl;
		}
		indi.second += g.score();
	}
}


int main(int argc, char* argv[] )
{

	srand(time(0));
    
	if(argc > 1)
	{
		ifstream f("agent.txt");
		agent specAgent;
		f >> specAgent;
		specificAgent(specAgent);
	}
	else
	{
		
		const int nbGeneration = 200;
	    const int nbRunPerIndi = 10;
	    const int nbIndividual = 20;
	    const int nbOffspring = 3;
	    
		cout<<"Nombre de generation:"<<nbGeneration<<endl;
	    cout<<"Nombre d'essais par individu:"<<nbRunPerIndi<<endl;
	    cout<<"Nombre d'individu:"<<nbIndividual<<endl;
	    
	    //vector<individual> population;
	    vector<individual> population;
	
	    for(int i = 0; i < nbIndividual ;++i )
	    {
	        agent a;
	        population.push_back(make_pair(a,0));
	    }
	    //Nombre totale de generation
	    for(int j = 0; j < nbGeneration ; ++j)
	    {
	        set<individual,indi_sorter> generationScore;
	        //Pour chaque individu dans la population courante
	        for(auto& indi: population)
	        {
				fitness(indi, nbRunPerIndi);
				//srand(print_current_time_with_ns());
				generationScore.insert(indi);
	        }

			if(j % 50 == 0)
			{
				//printPopulation(generationScore);
			//	nb2048 = 0;
			}
	        if(j % 1 == 0 && true) 
	        {
	            cout<<j<<"\t"<<(int)generationScore.begin()->second/nbRunPerIndi <<"\t";
				generationScore.begin()->first.print();
				cout<<endl;	
	        }
			
			set<individual>::iterator iter = generationScore.begin();
			for (int i = 0; i < nbOffspring; ++i)
			{
				iter = generationScore.begin();
				std::advance(iter, rand()%generationScore.size());
				individual p1 = *iter;
					
				iter = generationScore.begin();
				std::advance(iter, rand()%generationScore.size());
				individual p2 =  *iter;

				//individual offspring = mutation(crossing(p1,p2));
				individual offspring1 = mutation(crossing(p1,p2));
				//individual offspring2 = mutation(p2);
				//fitness(offspring, nbRunPerIndi);
				fitness(offspring1, nbRunPerIndi);
				//fitness(offspring2, nbRunPerIndi);
				//generationScore.insert(offspring);
				generationScore.insert(offspring1);
				//generationScore.insert(offspring2);
			}
			
			vector<individual> parents;
	        iter = generationScore.begin();
	        for (int i = 0; i < nbIndividual; ++i)
	        {
	            parents.push_back(*(iter));
	            iter++;
	        }
	        population.clear();
	
	        population =  vector<individual>(parents);
	    }
	}

    return 0;
}
