//LIBARIES
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <algorithm>

//LOCAL PACKAGES
#include "dataHandler/loadData.h"
#include "dataHandler/interpreter.h"
#include "utils/verify.h"
#include "utils/combination.h"
#include "utils/batch.h"
#include "boost/foreach.hpp"

//uuid
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

int main(int argc, char *argv[]){
	int bid, sid;
	initiateBatch(string(argv[1]),bid);
	return 0;
}
