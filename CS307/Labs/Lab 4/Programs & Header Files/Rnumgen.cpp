#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

#include "Rnumgen.h"

rnumgen::rnumgen(int seedvalue, vector<int> &v) {
	srand(seedvalue);

	F.resize(v.size());  
	partial_sum(v.begin(), v.end(), F.begin());  
	transform(F.begin(), F.end(), F.begin(),  bind2nd(divides<float>(), F.back()));

	//set up CDF
}

int rnumgen::rand() const { 
	const double randmax = RAND_MAX + 1.0;  
	const double p = (double)std::rand()/randmax;  

	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
