#include "utils.hpp"
#include <iostream>
std::mt19937& rand_gen()
{
	std::cout<<"problem"<<std::endl;
	exit(-1);
    static std::mt19937 gen(seed);
    return gen;
}

int rand_pos()
{
   // static std::uniform_int_distribution<> dis(0, grid_size - 1);

    return rand()%(grid_size);
}

bool double_initial_value()
{
//    static std::uniform_int_distribution<> dis(0, 99);

    return (rand()%100) < initial_double_value_percent;
}

direction rand_action()
{
//    static std::uniform_int_distribution<> dis(0, action_count - 1);

    return static_cast<direction>(rand()%(action_count));
}
