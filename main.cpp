#include <iostream>
#include <set>
#include <vector>
#include <utility>

#include "agent.h"
#include "grid.hpp"
using namespace std;

typedef unsigned int uint;
typedef std::pair<agent, uint>  individual;

struct indi_sorter{
    bool operator()(const pair<agent, uint> &l, const pair<agent, uint> &r) const
    {
        return l.second == r.second ? &l < &r : l.second > r.second;
    }
};


individual crossing(vector<individual> &parents)
{
    individual offspring;
    offspring.first.critTotalsum_ = parents[rand()%parents.size()].first.critTotalsum_;
    offspring.first.critMaxTile_ = parents[rand()%parents.size()].first.critMaxTile_;
    offspring.first.critnbTile_ = parents[rand()%parents.size()].first.critnbTile_;
    offspring.second = 0;

    return offspring;
}
individual mutation(individual i)
{
    individual mutant;
    int sign = rand()%1 ? -1: 1;
    mutant.first.critTotalsum_  = i.first.critTotalsum_ + (sign)*(i.first.critTotalsum_/(rand()/(double)RAND_MAX*10000));
    mutant.first.critMaxTile_   = i.first.critMaxTile_ + (sign)*(i.first.critMaxTile_/(rand()/(double)RAND_MAX*10000));
    mutant.first.critnbTile_    = i.first.critMaxTile_ + (sign)*(i.first.critnbTile_/(rand()/(double)RAND_MAX*10000));
    mutant.second = 0;
    mutant.first.normalize();

    return mutant;
}


int main()
{
    const int nbGeneration = 10000;
    const int nbRunPerIndi = 3;
    const int nbIndividual = 15;
    const int nbParent = 2;
    
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
            // Nombre d'essais par individu au cours d'une generation
            // pour faire la moyenne
            for(int k = 0; k < nbRunPerIndi; ++k)
            {
                grid g;
                while(g.can_move())
                {
                    direction d = indi.first.chooseDirection(g);
                    g.action(d);
                }
                indi.second += g.score();
            }
            //On fait la moyenne des scores
            indi.second = (double)indi.second/(double)nbRunPerIndi;
            generationScore.insert(indi);
        }

        if(j % 10 == 0) 
        {
            //cout<<"Generation #"<<j<<endl;
            cout<<generationScore.begin()->second<<"\t"<<j<< "\t";
            generationScore.begin()->first.print();
        }
        vector<individual> parents;
        set<individual>::iterator iter = generationScore.begin();
        for (int i = 0; i < nbParent; ++i)
        {
            parents.push_back(*(iter++));
        }
        population.clear();

        population =  vector<individual>(parents);
        for(int i = 0; population.size() < nbIndividual ;++i )
        {
            population.push_back(mutation(crossing(parents)));
            //mutation
        }
    }

    return 0;
}
