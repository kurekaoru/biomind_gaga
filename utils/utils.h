#include <boost/algorithm/string.hpp>
#include <iostream>
#include <cstdarg>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
	class make_vector {
		public:
		typedef make_vector<T> my_type;
		my_type& operator<< (const T& val) {
			data_.push_back(val);
			return *this;
		}
		operator std::vector<T>() const {
			return data_;
		}
	private:
	std::vector<T> data_;
};

template <class T>
T mean(vector<T> V){
	T out = (T)0;
	for (unsigned int i=0; i<V.size(); i++)
		out += V[i];
return (T)out/(T)V.size();
}

int stoi(string X){
	istringstream buffer(X);
	int out;
	buffer >> out;
	return out;
}

vector<int> vs2i(vector<string> X){
	vector<int> out;
	for (unsigned int i = 0; i<X.size(); i++){
		out.push_back(stoi(X[i]));
	}
	return out;
}

vector<int> s2c(string cluster){
	vector <string> out;
	string X = cluster.substr(1,cluster.size()-2);
	boost::split(out, X, boost::is_any_of("|"));
	return vs2i(out);
}	

double rDouble(double min, double max){
	return min+((double)rand()/(double)RAND_MAX)*max;
}


vector<char*> mgetCol( vector<vector<char*> > v, int index){
	vector<char*> output;
	for (unsigned int i = 0; i<v.size(); i++){
		output.push_back(v[i][index]);
	}
	return output;
}

template<class T>
T sum(vector<T> list){
	T out = 0;
	for (unsigned int i = 0; i<list.size(); i++){
		out+=list[i];
	}
	return out;
}

float sum(vector<char*> list){
	float out = 0;
	for (unsigned int i = 0; i<list.size(); i++){
		out+=atof(list[i]);
	}
	return out;
}

int weighted_choice(vector<char*> inList){
	vector<float> evalList;
	vector<float> templist;
	float cumulative = 0;
	for (unsigned int i=0; i<inList.size(); i++)
		templist.push_back(atof(inList[i]));//+rDouble(0.000000,0.000001));
	for (unsigned int i=0; i<inList.size(); i++){
		float interval = atof(inList[i])/sum(inList);
		evalList.push_back(cumulative+interval);
		cumulative += interval;
	}
	double randNum = rDouble(0,1);
	int out = 0;
	while (evalList[out] < randNum) {
		out++;}
	return out;
}

int weighted_choice(vector<float> inList){
	vector<float> evalList;
	vector<float> templist;
	float cumulative = 0;
	for (unsigned int i=0; i<inList.size(); i++)
		templist.push_back(inList[i]);//+rDouble(0.000000,0.000001));
	for (unsigned int i=0; i<templist.size(); i++){
		float interval = templist[i]/sum(inList);
		evalList.push_back(cumulative+interval);
		cumulative += interval;
	}
	double randNum = rDouble(0,1);
	int out = 0;
	while (evalList[out] < randNum) {
		out++;}
	return out;
}


float avg( int Count, ... ){
	va_list Numbers;
	va_start(Numbers, Count); 
	int Sum = 0;
	for(int i = 0; i < Count; ++i )
		Sum += va_arg(Numbers, int);
	va_end(Numbers);
	return (Sum/Count);
}

std::string i2s(int i) {
	std::ostringstream buffer;
	buffer << i;
	return buffer.str();
}

bool in(string x, vector<string> v) {
	if(std::find(v.begin(), v.end(), x) != v.end()) {
		return true;
	} else {
		return false;
	}
}

bool in(int x, vector<int> v) {
	if(std::find(v.begin(), v.end(), x) != v.end()) {
		return true;
	} else {
		return false;
	}
}

void readlines(const char* file, vector<string> &listObj){
	ifstream filestream(file);
	string buffer;
	while (getline(filestream,buffer))
		listObj.push_back(buffer);
}

template<class T>
void printlist(vector<T> listobj){
	for (unsigned int i = 0; i < listobj.size(); i++) {
		cout << " " << listobj[i];
	}
}

template<class T>
void printmatrix(vector<vector<T> > listobj){
	for (unsigned int i = 0; i < listobj.size(); i++) {
		for (unsigned int j = 0; j < listobj[i].size(); j++) {
			cout << " " << listobj[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

string f2s(float flt){
	char str[1024]  = "";
	sprintf(str, "%f", flt);
	return str;
}

int countlines(const std::string& path) {
	// Open the file.
	std::ifstream file(path.c_str());
	if (!file.is_open()) {
		cout << " READING FILE FAILED FOR " << path.c_str() << endl;
		exit(0);
	}
	// Count the lines.
	int count = 0;
	for (std::string line; std::getline(file,line);){
			if (!line.empty()) {
			    count++;
		}
	}

	return count;
}

string os(string cmd) {
	string data;
	FILE *stream;
	int MAX_BUFFER = 2048;
	char buffer[MAX_BUFFER];
	cmd.append(" 2>&1");
	stream = popen(cmd.c_str(), "r");
	if (!stream){
		exit(1);
	}
	while (!feof(stream)){
		if (fgets(buffer, MAX_BUFFER, stream) != NULL){
			data.append(buffer);
		}
	}
	pclose(stream);
	return data;
}

vector<int> range(int start, int end){
	vector<int> output;
	for (unsigned int i = start; i < end; i++) {
		output.push_back(i);
	}
	return output;
}

vector<int> sample(int elems, int samSize){
	vector<int> output;
	while (output.size() < samSize) {
		int index = rand() %elems;
		if (!in(index, output))
		output.push_back(index);
	}
	return output;
}

int sampleI(int N){
	return rand() %N;
}

vector<int> subset(vector<char*> vect, vector<int> index){
	vector<int> output;
	for (unsigned int i = 0; i < index.size(); i++) {
		output.push_back(atoi(vect[index[i]]));
	}
	return output;
}

int factorial(int X){
	int Y = 1;
	for(int i=X; i>0; i--)
		Y *= i;
	return Y;
}

vector<int> vc2i(vector<char*> v){
	vector<int> output;
	for(unsigned int i = 0; i < v.size(); i++) {
		output.push_back(atoi(v[i]));
	}   
	return output;
}

vector<float> vc2f(vector<char*> v){
	vector<float> output;
	for(unsigned int i = 0; i < v.size(); i++) {
		output.push_back(atof(v[i]));
	}   
	return output;
}

template<class T>
T sigmoid(T x, T a) {
	return 1/(1+exp(a*x));
}

template<class T>
void sigmoidTransform(vector<T> V, float a){
	for (unsigned int i = 0; i < V.size(); i++)
		V[i]=sigmoid(V[i],a);
}

template<class T>
void normalize(vector<T> &V){
	T S = sum(V);
	for (unsigned int i = 0; i < V.size(); i++)
		V[i]/=S;
}


template<class X>
void deviationTransform(map<X, float> &M, float mean){
	for( typename map<X,float>::iterator ii=M.begin(); ii!=M.end(); ++ii)
		M[(*ii).first] = M[(*ii).first]-mean;
}

template<class X, class Y>
vector<X> getKeys( map<X,Y> M ){
	vector<X> out;
	for( typename map<X,Y>::iterator ii=M.begin(); ii!=M.end(); ++ii) {
		out.push_back((*ii).first);
	}
	return out;
}

template<class X, class Y>
vector<Y> getVals( map<X,Y> M ){
	vector<Y> out;
	for( typename map<X,Y>::iterator ii=M.begin(); ii!=M.end(); ++ii) {
		out.push_back((*ii).second);
	}
	return out;
}

template<class X, class Y>
void printmap( map<X,Y> M ){
	for( typename map<X,Y>::iterator ii=M.begin(); ii!=M.end(); ++ii) {
		//printlist((*ii).first);
		cout << (*ii).first << " : " << (*ii).second << endl;
	}
}

template<class X, class Y>
void printmap2( map<X,Y> M ){
	for( typename map<X,Y>::iterator ii=M.begin(); ii!=M.end(); ++ii) {
		printlist((*ii).first);
		cout << " : " << (*ii).second << endl;
	}
}

template<class T>
bool in(T E, vector<T> V){
	for(unsigned int i=0; i<V.size(); i++){
		if(V[i] == E) return true;
	}
	return false;
}

template<class T>
bool isUniq(vector<T> V){
	map<T, bool> M;
	for(unsigned int i=0; i<V.size(); i++){
		M[V[i]] = true;
	}
	if (getKeys(M).size() != V.size()) return false;
	else return true;
} 
