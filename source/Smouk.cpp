#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <random>

using namespace std;

// split string to vector<string>
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

// implode vector<string> to string
template <class T, class A>
T join(const A &begin, const A &end, const T &t) {
	T result;
	for (A it = begin; it != end; it++) {
		if (!result.empty()) result.append(t);
		result.append(*it);
	}
	return result;
}

// select random word from vector<string>
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}
template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}

int main(int argc, char const *argv[]) {
	setlocale(LC_ALL, "");
	if (argc < 2) {
	  printf("ERROR: no files specified\nUsage: транссмоукификация.exe [TEXT] [WORDS]");
	  return (-1);
	}

	ifstream fslova(argv[2]);
	ifstream fpasta(argv[1]);
	string slova = "", pasta = "";
	vector<string> pasta_array;
	vector<string> slova_array;
	stringstream buffer;

	// ïèõí¸ì ôàéëû â ñòðîêó
	buffer << fslova.rdbuf(); slova = buffer.str();
	buffer.clear(); buffer.str("");
	buffer << fpasta.rdbuf(); pasta = buffer.str();

	// ðàçáèâàåì ïî ïðîáåëàì
	pasta_array = split(pasta, ' ');
	slova_array = split(slova, '\n');

	// çàìåíÿåì ðàíäîìíûå ñëîâà íà òå, ÷òî â ñëîâàðå ñ âåðîÿòíîñòüþ â 30%
	for (vector<string>::iterator it = pasta_array.begin(); it != pasta_array.end(); ++it) {
		if (rand() % 100 > 70) *it = *select_randomly(slova_array.begin(), slova_array.end());
	}

	

	pasta = join(pasta_array.begin(), pasta_array.end(), string(" "));
	ofstream out("pasta_smouked.txt");
	out << pasta;
	out.close();

	fslova.close();
	fpasta.close();

	return 0;
}
