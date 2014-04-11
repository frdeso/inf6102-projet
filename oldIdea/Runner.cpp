#include "Runner.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>

#include "Constants.h"

using namespace std;
Runner::Runner(){
	chromo_ = new std::vector<double>();
}
Runner::Runner(int nbChrom)
{
	this->setStatus(STATUS_ALIVE);
	lastScore_ = DBL_MAX;
	chromo_ = new std::vector<double>();
	for (int i = 0; i < nbChrom ; ++i)
	{

		double curr =(double) (rand()%100);
		this->chromo_->push_back(curr);
	}
	this->normalizeChromo();	
}

Runner::Runner(Runner &r)
{
	lastScore_ = r.getLastScore();
	chromo_ = new std::vector<double>(r.GetChromo()->begin(), r.GetChromo()->end());	
}

bool Runner::operator< (const Runner &r)const 
{	
	
	cout<< this->lastScore_ <<": "<< r.lastScore_<<endl;
	return this->lastScore_ < r.lastScore_;
}

std::vector<double> *Runner::GetChromo()
{
	 return chromo_;
}

void Runner::normalizeChromo()
{
	double sum = 0;
	for (int i = 0; i < this->GetChromo()->size() ; ++i)
	{
		sum += (*chromo_)[i];
	}
	
	for (int i = 0; i < this->GetChromo()->size(); ++i)
	{
		(*chromo_)[i] = (*chromo_)[i]/sum;
	}
}

void Runner::setStatus(int newStatus)
{
	this->status_ =  newStatus;
}

int Runner::getStatus() const
{
	return status_;
}

double Runner::getLastScore() const
{
	return this->lastScore_;
}

void Runner::setLastScore(double lastScore)
{
	this->lastScore_ = lastScore;
}
