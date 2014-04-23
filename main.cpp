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
individual crossing(individual &p1, individual &p2)
{
    individual offspring;


    for(int i = 0; i < offspring.first.getCriterion().size(); ++i)
    {
        offspring.first.getCriterion()[i] =  (p1.first.getCriterion()[i] + p2.first.getCriterion()[i])/2;
    }
    offspring.second = 0;

    return offspring;
}

individual crossingAllparents(vector<individual> &parents)
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
    double chiffre = 10.0;
    //for(int j = 0; j < mutant.first.getCriterion().size(); ++j)
    {
	int j = rand()%mutant.first.getCriterion().size();
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
	srand(time(0));
    const int nbGeneration = 10000;
    const int nbRunPerIndi = 20;
    const int nbIndividual = 4;
    const int nbParent = nbIndividual-1;
	int lastPrint = 0;
	int sum = 0;

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
                }
				if (g.largest() >= 2048)
				{
					cout<<j<<"\t"<<g.score()<<"\t";
					indi.first.print();
				//	g.print();
				}
                indi.second += g.score();
			//	cout<<"\t"<<g.score()<<endl;
				//g.print();
            }
            //On fait la moyenne des scores
            indi.second = (double)indi.second/(double)nbRunPerIndi;
            generationScore.insert(indi);
        }

        if(j % 1 == 1) 
        {
            //cout<<"Generation #"<<j<<endl;
            cout<<generationScore.begin()->second<<"\t"<<(int)generationScore.begin()->second - lastPrint<<"\t"<<j<< "\t";
			lastPrint = generationScore.begin()->second;
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
//            parents.push_back(mutation(*(iter)));
//            parents.push_back(mutation(*(iter)));
            iter++;
        }
        population.clear();

        population =  vector<individual>(parents);
        for(int i = 0; population.size() < nbIndividual ;++i )
        {
			uint parentSize = parents.size();
            population.push_back(mutation(crossing(parents[rand()%parentSize],parents[rand()%parentSize])));
            //mutation
        }
    }

    return 0;
}
