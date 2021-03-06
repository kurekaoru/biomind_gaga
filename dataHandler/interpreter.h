#include <iostream>

using namespace std;

void MetaTask(int fold,int sol){
	system(("mkdir -p "+string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(sol)).c_str());
	system(("java -cp .:openbiomind.jar task.MetaTask -d " + string(GENE_TUPLET_OUTPUT_DIR) + "/ -o " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(sol)+"/ -classificationMethod " + string(classificationMethod) + " -targetCategory "+string(targetCategory)+" -numberOfTasks " + i2s(NUM_TASKS) +" > "+string(LOG_DIR)+"/MetaTask.log").c_str());

}

void Moses(int fold,int sol){
	system(("mkdir -p "+string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(sol)).c_str());
	system(("python pyutils/mosesTransform.py "+string(GENE_TUPLET_OUTPUT_DIR)+"/train"+i2s(fold)+".tab "+string(GENE_TUPLET_OUTPUT_DIR)+"/train"+i2s(fold)+".moses").c_str());
	system(("python pyutils/mosesTransform.py "+string(GENE_TUPLET_OUTPUT_DIR)+"/test"+i2s(fold)+".tab "+string(GENE_TUPLET_OUTPUT_DIR)+"/test"+i2s(fold)+".moses").c_str());
	//Run single moses on train dataset
	system(("moses -i "+string(GENE_TUPLET_OUTPUT_DIR)+"/train"+i2s(fold)+".moses -u out -C1 --enable-fs=1 -W1 -o " +string(GENE_TUPLET_OUTPUT_DIR)+"/model_"+i2s(fold)+".moses").c_str());

	//Lets replace this by something more stable..
	system(("moses/mosesEval.sh " + string(GENE_TUPLET_OUTPUT_DIR) + "/train"+i2s(fold)+".moses "+string(GENE_TUPLET_OUTPUT_DIR) + "/test"+i2s(fold)+".moses " +string(GENE_TUPLET_OUTPUT_DIR)+"/model_"+i2s(fold)+".moses " + string(GENE_TUPLET_OUTPUT_DIR) + "/model"+i2s(fold)+".moses " + string(GENE_TUPLET_OUTPUT_DIR) + "/out"+i2s(fold)+".moses "+string(GENE_TUPLET_OUTPUT_DIR)+"/result"+i2s(fold)+".temp " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(sol)+"/outfinal.txt").c_str());
	//cout << (("moses/mosesEval.sh " + string(GENE_TUPLET_OUTPUT_DIR) + "/train"+i2s(fold)+".moses "+string(GENE_TUPLET_OUTPUT_DIR) + "/test"+i2s(fold)+".moses " +string(GENE_TUPLET_OUTPUT_DIR)+"/model_"+i2s(fold)+".moses " + string(GENE_TUPLET_OUTPUT_DIR) + "/model"+i2s(fold)+".moses " + string(GENE_TUPLET_OUTPUT_DIR) + "/out"+i2s(fold)+".moses "+string(GENE_TUPLET_OUTPUT_DIR)+"/result"+i2s(fold)+".temp " + string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold)+"/sol_"+i2s(sol)+"/outfinal.txt").c_str()) << endl;

}

string getGene(string header){
	vector <string> vect;
	boost::split(vect, header, boost::is_any_of("#"));
	return vect[1];
}

string getModel(int fold_id, int sol_id){
	string S = os("head -n2 " + string(TASK_OUTPUT_DIR) + "/current/fold_" + i2s(fold_id) + "/sol_" + i2s(sol_id) + "/outfinal.txt | tail -n1").c_str();
	//cout << S << endl;
	return S;
}

float getTrainAcc(int fold_id,int sol_id){
	int ln = atoi(os("echo $((`grep -n \'Global Train Matrix\' "+string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold_id)+"/sol_"+i2s(sol_id)+"/outfinal.txt | sed -n \'s/^\\([0-9]*\\)[:].*/\\1/p\'`+3))").c_str());
	return atof(os("echo `sed -n \'"+i2s(ln)+"p\' "+string(TASK_OUTPUT_DIR)+"/current/fold_"+i2s(fold_id)+"/sol_"+i2s(sol_id)+"/outfinal.txt | sed -n \'s/Accuracy: //p\'`").c_str());
}

float getTestAcc(int fold_id,int sol_id){
	int ln = atoi(os("echo $((`grep -n \'Global Test Matrix\' "+string(TASK_OUTPUT_DIR) +"/current/fold_"+i2s(fold_id)+"/sol_"+i2s(sol_id)+"/outfinal.txt | sed -n \'s/^\\([0-9]*\\)[:].*/\\1/p\'`+3))").c_str());
	return atof(os("echo `sed -n \'"+i2s(ln)+"p\' "+string(TASK_OUTPUT_DIR)+"/current/fold_"+i2s(fold_id)+"/sol_"+i2s(sol_id)+"/outfinal.txt | sed -n \'s/Accuracy: //p\'`").c_str());
}

