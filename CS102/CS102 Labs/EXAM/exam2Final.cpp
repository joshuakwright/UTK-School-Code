//function definition
vector<double> MultVector(const vector<vector<double>> &m, const vector<double> &v) {
	//declare return vector
	vector<double> multiples;
	//temp variable to hold sum
	double sum = 0;
	/*nested for loops that multiply the vector and the matrix, with 4 final terms being put in the multiples vector*/
	for (unsigned int i = 0; i < v.size(); i++) {
		for (unsigned int j = 0; j < m.at(i).size(); j++) {
			//iterative addition into the sum variable
			sum = sum + v.at(i) * m.at(i).at(j);		
		}
		//puts new sum in multiples and resets sum variable
		multiples.push_back(sum);
		sum = 0;
	}
	return multiples;
}
