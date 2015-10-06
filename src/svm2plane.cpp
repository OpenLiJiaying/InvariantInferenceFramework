#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits>
using namespace std;

const int MAX = 1000;
int tempint;
char tempchar;
bool Oflag = false;

class MODEL {
	public:
		int getIn(ifstream& file) {
			char line[MAX];
			file >> line >> svm_type;
			file >> line >> kernel_type;
			file >> line >> nr_class;
			if (nr_class < 2) {
				return -1;
			}
			file >> line >> total_sv;
			file >> line >> rho;
			file >> line >> label1 >> label2;
			file >> line >> nr_sv1 >> nr_sv2;
			file >> line;
			label = new double [total_sv];
			x = new double* [total_sv];
			for (int i = 0; i < total_sv; i++) {
				x[i] = new double [nr_class];
			}
			theta = new double [nr_class];
			for (int j = 0; j < nr_class; j++)
				theta[j] = 0;
			for (int i = 0; i < total_sv; i++) {
				file >> label[i];
				for (int j = 0; j < nr_class; j++)
					file >> tempint >> tempchar >> x[i][j];
			}
			return 0;
		}

		int toString() {
			if (Oflag) {
				cout << "svm_type:[" << svm_type << "] kernel_type:[" << kernel_type << "] nr_class:[" << nr_class << "] total_sv:[" << total_sv << "] rho: [" << rho << "]" << endl;
				for (int i = 0; i < total_sv; i++) {
					cout << label[i];
					for (int j = 0; j < nr_class; j++)
						cout << " " << x[i][j];
					cout << endl;
				}
				if (!check())
					return 0;
			}
			//cout << "-----------------------------------------------------------" << endl;
			//			cout << "CLASSIFIER: \n \t Y =";
			// cout << "CLASSIFIER: \t ";
			cout << " (" << theta[0] << ") * X" << 0;
			for (int j = 1; j < nr_class; j++)
				cout << " +  (" << theta[j] << ") * X" << j;
			if (theta0 == 0)
				cout << " >= 0";
			else 
				cout << " >= "<< theta0 * -1;
			return 0;
		}

		int calcuateClassiofier(){
			if (!check())
				return -1;
			theta0 = label[0] > 0? 1 : -1;
			for (int i = 0; i < total_sv; i++) {
				for (int j = 0; j < nr_class; j++) {
					theta[j] += label[i] * x[i][j];
				}
			}
			for (int i = 0; i < total_sv; i++) {
				double temp = 0;
				for (int j = 0; j < nr_class; j++) {
					temp += x[i][j] * x[0][j];
				}
				temp *= label[i];
				theta0 -= temp;
			}
			return 0;
		}

		~MODEL() {
/*			if (label)
				delete label;
			if (theta)
				delete theta;
			if (x) {
				for (int j = 0; j < total_sv; j++)
					if (x[j])
						delete x[j];
				delete x;
			}
*/		}

		int nr_class;
		double* theta;
		double theta0;

	private:
		bool check() {
			return strcmp(kernel_type, "linear") == 0;
		}

		char svm_type[32];
		char kernel_type[32];
		int total_sv;
		double rho;
		int label1, label2;
		int nr_sv1, nr_sv2;
		double* label = NULL;
		double** x = NULL;
};

int main(int argc, char** argv)
{
	cout.precision(std::numeric_limits<double>::digits10);
	char *model_file = NULL, *test_file = NULL, *out_file = NULL, *plane_file = NULL;
	bool mflag = false, tflag = false, oflag = false, pflag = false;
	if (argc <= 1) {
		cout << "svm2plane: missing operand after svm2plane" << endl;
		cout << "svm2plane: try 'svm2plane -help' for more information" << endl;
		return -1;
	}
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "-model") == 0) {
			model_file = argv[++i];
			mflag = true;
			continue;
		}
		if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-test") == 0) {
			test_file = argv[++i];
			tflag = true;
			continue;
		}
		if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0) {
			out_file = argv[++i];
			oflag = true;
			continue;
		}
		if (strcmp(argv[i], "-p") == 0) {
			plane_file = argv[++i];
			pflag = true;
			continue;
		}
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
			cout << "Usage: svm-plain -m [model_file] [OPTIONS]" << endl;
			cout << "OPTION: -t -o -p -h {file}" << endl;
			cout << "-t {FILE}: using the model to predict [FILE]" << endl << endl;
			cout << "-o {FILE}: using the model to predict FILE, the predict and other results are saved in [FILE]" << endl << endl;
			cout << "-p {FILE}: print the hyperplane to [FILE]" << endl << endl;
			cout << "-h       : display the help information" << endl << endl;
			return 1;
		}
	}
	if (model_file == NULL) {
		cout << "svm2plane: missing model file after svm2plane" << endl;
		cout << "svm2plane: try 'svm2plane -help' for more information" << endl;
		return -1;
	}
	class MODEL model;
	ifstream if1(model_file);
	int ret = model.getIn(if1);
	if (ret < 0) {
		if1.close();
		return -2;
	}
	model.calcuateClassiofier();
	model.toString();

	if (tflag == false)
		return 0;

	ifstream if2(test_file);
	if(oflag == false) {
		out_file = new char[128];
		strcpy(out_file, "./svm2plane.predict");
	}
	ofstream of1(out_file);


	if(pflag == false) {
		plane_file = new char[128];
		strcpy(out_file, "./SVMline");
	}
	ofstream of2(plane_file);

	double label, *x = new double[model.nr_class];
	int nr_sample = 0, nr_right = 0;
	double predict;

	//cout << "-----------------------------------------------------------" << endl;
	ofstream of3("svm2plane.predict_wrong");
	while (if2 >> label) {
		predict = model.theta0;
		nr_sample++;
		for (int i = 0; i < model.nr_class; i++) {
			if2 >> tempint >> tempchar >> x[i];
			predict += x[i] * model.theta[i];
		}
		predict = predict >= 0? 1 : -1;
		of1 << predict << endl;
		if (predict * label > 0)
			nr_right++;
		else {
			of3 << "[" << nr_sample << "] <label:" << label << " predict:" << predict << "> data: " << endl;
			for (int i = 0; i < model.nr_class; i++) 
				of3 << i+1 << ":" <<  x[i] << " ";
		}
	}
	of3 << "totally misclassified" << nr_sample - nr_right << " / " << nr_sample << " samples." << endl; 
	//	cout << "-----------------------------------------------------------" << endl;
	cout << "\t[" << nr_right * 100.00 / nr_sample << "% (" << nr_right << "/" << nr_sample << ")]" << endl;
	of2.precision(std::numeric_limits<double>::digits10);
	of2 << nr_right  / (double)nr_sample;
	for (int i = 0; i < model.nr_class; i++) {
		of2 << "\t" << model.theta[i];
	}
	of2 << "\t" << model.theta0 << endl;

	if1.close();
	if2.close();
	of1.close();
	of2.close();
	of3.close();

	if (out_file)
		delete out_file;
	return 0;
}

