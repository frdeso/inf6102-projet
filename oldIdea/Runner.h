#ifndef	_RUNNER_H_
#define	_RUNNER_H_

#include <vector>
#include <iostream>
class Runner{
	public:
		Runner();
		Runner(int nbChrom);
		Runner(Runner &r);
		bool operator< (const Runner& r)const;
		struct comparator{
			bool operator()(Runner* l, Runner* r)
			{	
//				std::cout<<l->getLastScore()<<": " <<r->getLastScore()<<" : "<< (l->getLastScore() < r->getLastScore())<<std::endl;
				return  l->getLastScore() ==  r->getLastScore()? l < r : l->getLastScore() < r->getLastScore();
			}
		};
		std::vector<double> *GetChromo();
		void normalizeChromo();
		void setStatus(int newStatus);
		int getStatus() const;
		double getLastScore() const;
		void setLastScore(double lastScore);
	private:
		int status_;
		double lastScore_;
		std::vector<double> *chromo_;
};
#endif // _RUNNER_H_
