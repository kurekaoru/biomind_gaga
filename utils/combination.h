int next_comb(int comb[], int k, int n) {
	int i = k - 1;
	++comb[i];
	while ((i >= 0) && (comb[i] >= n - k + 1 + i)) {
		--i;
		++comb[i];
	}

	if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
		return 0; /* No more combinations can be generated */

	/* comb now looks like (..., x, n, n, n, ..., n).
	Turn it into (..., x, x + 1, x + 2, ...) */
	for (i = i + 1; i < k; ++i)
		comb[i] = comb[i - 1] + 1;

	return 1;
}

vector<vector<int> > getComb(int n, int k){

	int comb[512]; /* comb[i] is the index of the i-th element in the combination */
	vector<vector<int> > out;
	vector<int> elem;
	/* Setup comb for the initial combination */
	//int i;
	for (int i = 0; i < k; ++i) {
		comb[i] = i;
		elem.push_back(i+1);
	}
	out.push_back(elem);

	/* Print the first combination */
	//printc(comb, k);

	/* Generate and print all the other combinations */
	while (next_comb(comb, k, n)) {
		//printc(comb, k);
		vector<int> elem;
		for (int i = 0; i < k; ++i) {
			elem.push_back(comb[i]+1);
		}
		out.push_back(elem);
	}
	return out;

}

vector<vector<int> > getAllComb(int n, int maxk){

	vector<vector<int> > out;

	for(int i=maxk; i>0; i--){
		vector<vector<int> > item = getComb(n, i);
		for (int j=0; j<=(factorial(n)/(factorial(i)*factorial(n-i))-1); j++){
			out.push_back(item[j]);
		}
	}
	return out;

}


vector<vector<int> > combinations(vector<int> inlist, int maxk){
	vector<vector<int> > out;
	vector<vector<int> > index = getAllComb(inlist.size(), maxk);
	for (int i = 0; i < index.size(); ++i) {
		vector<int> item;
		for (int j = 0; j < index[i].size(); ++j) {
			item.push_back(inlist[index[i][j]-1]);
		}
		out.push_back(item);
	}
	return out;
}


