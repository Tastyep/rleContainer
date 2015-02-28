#include <cstdlib>
#include <iostream>
#include <ctime>
#include "rleContainer.hpp"

int main(int argc, char *argv[])
{
  rleContainer<unsigned int>	rle;
  unsigned int	counter = 2000;
  std::vector<unsigned int>	vec;

  std::srand(std::time(0));
  for (unsigned int i = 0; i < counter; ++i)
    {
      unsigned int nb = std::rand() % 100;
      if (nb > 75)
	nb = 1;
      else
	nb = 0;
      // std::cout << nb;
      vec.push_back(nb);
      rle.push_back(nb);
    }
  // std::cout << std::endl;
  // for (unsigned int i = 0; i < counter; ++i)
  //   std::cout << rle[i];
  // std::cout << std::endl;
  std::cout << "memory used: " << counter * sizeof(unsigned int)  << " vs "
	    << rle.memory_used() << std::endl;
  // for (auto &i : vec)
  //   std::cout << i;
  // std::cout << std::endl;
  return 0;
}
