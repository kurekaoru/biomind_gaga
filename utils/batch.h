using namespace std;

//uuid
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>


void initiateBatch(string infile, int &batchId){
	//Construct mysql tables
	system(("mysql -u "+string(USER)+" -p"+string(PASS)+" < const/schema_batch.sql").c_str());
	//Batch ID
	boost::uuids::uuid BID = boost::uuids::random_generator()();
	cout << " GENERATE BATCH UUID\t\t    [" << BID << "]" << endl;
	system(("touch temp/"+boost::lexical_cast<std::string>(BID)).c_str());
	if(!BATCH) REPEATS=1;
	//register batch on MySQL
	registerBatch(boost::lexical_cast<std::string>(BID), string(shexec("hostname | awk /./")) , string(infile), REPEATS, batchId);
	//seed daughter processes
	cout << " SEEDING DAUGHTER UUIDS\t\t\t\t\t         [SUCCESS]" << endl;
	boost::uuids::uuid DBID;
	if (!BATCH) REPEATS = 1;
	for(unsigned int i = 0; i < REPEATS; i++){
		DBID = boost::uuids::random_generator()();
		registerSession(boost::lexical_cast<std::string>(DBID), batchId);
	}

}
