//Standards lib
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <float.h>
//Third parties libs

//personal libs
#include "Biome.h"
#include "Runner.h"
#include "Constants.h"
using namespace std;

typedef vector<Biome> World;

World currWorld;

void printWorld(){
	cout<<endl<<"|";
	for(World::iterator iter = currWorld.begin(); iter != currWorld.end();iter++)
	{
		for(int i = 0; i < iter->GetLength();++i)
		{
			if(i == (iter->GetLength()/2)-1 ){
				cout<<iter->GetType();
				if(iter->GetType() >= 10)
				{
					i++;
				}
			}
			else{
				cout<<" ";
			}
		}
		cout<<"|";

	}
	 cout<<endl<<"|";
	for(World::iterator iter = currWorld.begin(); iter != currWorld.end();iter++)
	{
		for(int i = 0; i < iter->GetLength();++i)
		{
			cout<<"-";
		}
		cout<<"|";
	}
	cout<<endl;
}

void printRunner( Runner *r)
{
	cout<<"|";

	for(vector<double>::iterator iter = r->GetChromo()->begin(); iter != r->GetChromo()->end(); iter++)
	{

		cout<<fixed<<setprecision(4); 
		cout<<*iter<<"|";		
	}
	cout<<endl;
}

void printPopulation(set<Runner*,Runner::comparator> &p)
{
	for(set<Runner*>::iterator iter = p.begin(); iter != p.end(); iter++)
	{
		printRunner(*iter);
	}
}

void RunGeneration(World &currWorld, set<Runner*,Runner::comparator> &population)
{
	for(set<Runner*>::iterator iter = population.begin(); iter != population.end(); iter++)
	
	{
		Runner* r = *iter;

		double score= 0;
		int i = 0;
		for (World::iterator w = currWorld.begin(); w != currWorld.end(); w++, i++)
		{
			double currChrom = r->GetChromo()->at(w->GetType());
			if (currChrom != 0){
				score += ((double)w->GetLength())/currChrom;
			}
			else
			{
				r->setStatus(STATUS_DEAD);
//				cout<<"Runner died at "<<score<<"."<<endl;
				break;
			}
		}
		if (r->getStatus() == STATUS_ALIVE	)
		{
		//	cout<< score<<endl;
			r->setLastScore(score);
			population.insert(r);
		}
		else
		{
			population.erase(iter);
			r->setLastScore(DBL_MAX);
		}
	}
//	cout<<"---"<<endl;
}

void selectElites(set<Runner*,Runner::comparator> popu, set<Runner*, Runner::comparator> &matingPool, int nbElite)
{	
	int i = 0;
	for(set<Runner*>::iterator iter = popu.begin(); matingPool.size() < nbElite ; iter++, i++)
	{
		if(iter == popu.end())
		{
			cout<<"All individual of the population are dead. :("<<endl;
			exit(-1);
		}
		matingPool.insert(*iter);
	}
}

void mutation(set<Runner*, Runner::comparator> &matingPool, set<Runner*, Runner::comparator> &offsprings, int  nbMutated)
{
	for(int i = 0; i < nbMutated; ++i)
	{
		set<Runner*, Runner::comparator>::iterator iter = matingPool.begin();
		int randomParent = rand()% matingPool.size();
		advance(iter, randomParent);
		
		Runner* r = new Runner(*(*iter));
		
		double offset = ((double)(rand()%100))/1000;

		if(rand()%2 == 0)
		{
			offset = -offset;	
		}
		int mutatedGene = rand()%r->GetChromo()->size();
		(*r->GetChromo())[mutatedGene] += offset;
		
		if(	(*r->GetChromo())[mutatedGene] < 0 )
		{
			(*r->GetChromo())[mutatedGene] = 0;
		}
		if(	(*r->GetChromo())[mutatedGene] > 1)
		{
			(*r->GetChromo())[mutatedGene] = 1;
		}
		r->normalizeChromo();
		offsprings.insert(r);

	}
}
int main(){
	
	unsigned int nbBiome		= 300;
	unsigned int maxLength		= 9;	
	unsigned int minLength		= 0;	
	unsigned int populationSize = 40;	
	unsigned int nbGeneration	= 10000;	
	unsigned int nbBiomeType	= 20;
	int seed = 42;
	srand(seed);
	for(unsigned int i = 0; i < nbBiome;++i)
	{
		int b = rand()%nbBiomeType;
		int l = (rand()%maxLength-minLength)+minLength;
		currWorld.push_back(Biome(b,l) );
	}

	//	printWorld();

	set<Runner*,Runner::comparator> population;
	for (int i = 0; i < populationSize ; ++i)
	{
		population.insert(new Runner(nbBiomeType));	
	}

	//	printPopulation(population);

	set<Runner*, Runner::comparator> matingPool;
	set<Runner*, Runner::comparator> offsprings;

	//double lastBest = std::numeric_limits<double>::max;
	double lastBest = DBL_MAX; 
	for(unsigned int i = 0; i < nbGeneration ; ++i)
	{
		RunGeneration(currWorld, population);
		//Select winner and drop them in the mating pool
		selectElites(population, matingPool, 4);

		for(set<Runner*>::iterator iter = matingPool.begin(); iter != matingPool.end(); iter++)
		{
			//cout<<(*iter)->getLastScore()<<endl;
			if(lastBest > (*iter)->getLastScore())
			{
				lastBest = (*iter)->getLastScore();
				printRunner(*iter);
				cout<< "Generation #"<<i<<": "<<lastBest<<endl;
			}
//			printRunner(*iter);
		}
		//Generate mutated offsprings
		mutation(matingPool, offsprings, populationSize);
		for(set<Runner*>::iterator iter = offsprings.begin(); iter != offsprings.end(); iter++)
		{
//			printRunner(*iter);
		}

		population.clear();
		population.insert(offsprings.begin(), offsprings.end());
		population.insert(matingPool.begin(), matingPool.end());
		matingPool.clear();
		offsprings.clear();
	}
}
