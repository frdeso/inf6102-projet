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


    for(int i = 0; i < offspring.first.getCriterion().size(); ++i)
    {
        offspring.first.getCriterion()[i] = parents[rand()%parents.size()].first.getCriterion()[i];
    }
    offspring.second = 0;

    return offspring;
}
double getRandPercentage()
{
    return (double)rand()/(double)RAND_MAX;
}

individual mutation(individual i)
{
    individual mutant;
    double chiffre = 100000;
    for(int j = 0; j < mutant.first.getCriterion().size(); ++j)
    {
        double sign = ((rand()%3) -1);
        mutant.first.getCriterion()[j] = 
                    i.first.getCriterion()[j] + ((double)sign)*(i.first.getCriterion()[j] * getRandPercentage())/chiffre;
    }

    mutant.second = 0;
    mutant.first.normalize();

    return mutant;
}


int main()
{
    const int nbGeneration = 10000;
    const int nbRunPerIndi = 5;
    const int nbIndividual = 100;
    const int nbParent = nbIndividual*0.10;

    cout<<"Nombre de generation:"<<nbGeneration<<endl;
    cout<<"Nombre d'essais par individu:"<<nbRunPerIndi<<endl;
    cout<<"Nombre d'individu:"<<nbIndividual<<endl;
    cout<<"Nombre de parents:"<<nbParent<<endl;
    
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
				//g.print();
            }
            //On fait la moyenne des scores
            indi.second = (double)indi.second/(double)nbRunPerIndi;
            generationScore.insert(indi);
        }

        if(j % 2 == 0) 
        {
            //cout<<"Generation #"<<j<<endl;
            cout<<generationScore.begin()->second<<"\t"<<j<< "\t";
            generationScore.begin()->first.print();
            // for(auto i: generationScore)
            // {
            //     i.first.print();
            // }
            // cout<<"-----------"<<endl;
        }
        vector<individual> parents;
        set<individual>::iterator iter = generationScore.begin();
        for (int i = 0; i < nbParent; ++i)
        {
            parents.push_back(*(iter));
            parents.push_back(mutation(*(iter)));
            parents.push_back(mutation(*(iter)));
            iter++;
        }
        population.clear();

        population =  vector<individual>(parents);
        for(int i = 0; population.size() < nbIndividual ;++i )
        {
            population.push_back(crossing(parents));
            //mutation
        }
    }

    return 0;
}
