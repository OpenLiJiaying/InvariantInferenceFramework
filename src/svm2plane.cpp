#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string.h>
#include<limits>
using namespace std;

const int MAX = 2048;
const int maxDim = 128;
const int maxSV = 2;
int tempint;
char tempchar;
bool Oflag = false;



int nr_class;
double theta[maxDim];
double theta0;
int dimension;
char svm_type[32];
char kernel_type[32];
int total_sv;
double rho;
int label1, label2;
int nr_sv1, nr_sv2;
double label[maxSV];
double x[maxSV][maxDim];
char line[MAX];


bool check() {
	return strcmp(kernel_type, "linear") == 0;
}

int parse_header(ifstream& file) {
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
	return 0;
}


int calcuate_classifier(ifstream& if1){
	if (!check())
		return -1;
	if1 >> label[0];
	theta0 = label[0] > 0? 1 : -1;
	int j = 0;
	double temp = 0;
	if1.getline(line, MAX);
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == ':') {
			x[0][j] = strtod(line + i + 1, NULL);
			theta[j] += label[0] * x[0][j];
			temp += x[0][j] * x[0][j];
			j++;
		}
	}
	temp *= label[0];
	theta0 -= temp;
	dimension = j;
		
	for (int i = 1; i < total_sv; i++) {
		if1 >> label[1];
		temp = 0;
		for (int j = 0; j < dimension; j++) {
			if1 >> tempint >> tempchar >> x[1][j];
			theta[j] += label[1] * x[1][j];
			temp += x[1][j] * x[0][j];
		}

		temp *= label[1];
		theta0 -= temp;
	}
	return 0;
}

int to_string() {
	if (Oflag) {
		cout << "svm_type:[" << svm_type << "] kernel_type:[" << kernel_type << "] "
			" nr_class:[" << nr_class << "] total_sv:[" << total_sv << "] rho: [" << rho << "]" << endl;
		for (int i = 0; i < total_sv; i++) {
			cout << label[i];
			for (int j = 0; j < nr_class; j++)
				cout << " " << x[i][j];
			cout << endl;
		}
		if (!check())
			return 0;
	}
	cout << " " << theta[0] << "[0]";
	for (int j = 1; j < dimension; j++)
		cout << "  +  " << theta[j] << "[" << j << "]";
/*	if (theta0 != 0)
		cout << " + " << theta0; 
	cout << " >= 0";
*/
	cout << " >= " << -theta0;
	return 0;
}


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

	ifstream if1(model_file);
	int ret = parse_header(if1);
	if (ret < 0) {
		if1.close();
		return -2;
	}
	calcuate_classifier(if1);
	to_string();

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

	double label, *x = new double[nr_class];
	int nr_sample = 0, nr_right = 0;
	double predict;

	//cout << "-----------------------------------------------------------" << endl;
	ofstream of3("svm2plane.predict_wrong");
	while (if2 >> label) {
		predict = theta0;
		nr_sample++;
		for (int i = 0; i < dimension; i++) {
			if2 >> tempint >> tempchar >> x[i];
			predict += x[i] * theta[i];
		}
		predict = predict >= 0? 1 : -1;
		of1 << predict << endl;
		if (predict * label > 0)
			nr_right++;
		else {
			of3 << "[" << nr_sample << "] <label:" << label << " predict:" << predict << "> data: " << endl;
			for (int i = 0; i < nr_class; i++) 
				of3 << i+1 << ":" <<  x[i] << " ";
		}
	}
	of3 << "totally misclassified" << nr_sample - nr_right << " / " << nr_sample << " samples." << endl; 
	//	cout << "-----------------------------------------------------------" << endl;
	cout << "\t[" << nr_right * 100.00 / nr_sample << "% (" << nr_right << "/" << nr_sample << ")]" << endl;
	of2.precision(std::numeric_limits<double>::digits10);
//	of2 << nr_right  / (double)nr_sample;
	of2 << dimension << "\t";
	for (int i = 0; i < dimension; i++) {
		of2 << theta[i] << "\t";
	}
	of2 << theta0 << endl;

	if1.close();
	if2.close();
	of1.close();
	of2.close();
	of3.close();

	if (out_file)
		delete out_file;
	return 0;
}

