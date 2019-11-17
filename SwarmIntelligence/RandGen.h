#pragma once
#include <cassert>
#include <optional>
#include <random>

template<typename Engine>
class RandGen
{
public:
   RandGen(long seed = 0)
   {
      eng.seed(seed);
      this->distr = std::make_unique<std::uniform_int_distribution<>>(this->min, this->max);
   }; 

   RandGen(int min, int max, long seed = 0)
   {
      assert(min < max && "Minimum size should be greater than max");

      this->min = min;
      this->max = max;
      eng.seed(seed);
      this->distr = std::make_unique<std::uniform_int_distribution<>>(this->min, this->max);
   }

   void setSeed(long seed)
   {
      this->eng.seed(seed);
   }

   int rand() const
   {
      return static_cast<int>(std::round((*distr)(eng)));
   }

   int rand(int min, int max)
   {
      std::uniform_int_distribution dist(min, max);
      return static_cast<int>(std::round(dist(eng)));
   }

   std::vector<int> randN(int count)
   {
      assert(count > 0 && "Count should be greater than 0");

      std::vector<int> rV;

      for(int i = 0; i < count; ++i)
      {
         const int randomNumber = static_cast<int>(std::round((*distr)(eng)));
         rV.push_back(randomNumber);
      }
      return rV;
   }


private:
   Engine eng;
   std::unique_ptr<std::uniform_int_distribution<>> distr;

   int max = 1;
   int min = 0;
};
