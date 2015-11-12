#ifndef _LIBSVM_H_
#define _LIBSVM_H_
#include "header.h"
#include <iostream>
//#include <stdarg.h>
//#include "float.h"
//#include "color.h"
#define LIBSVM_VERSION 320

extern int libsvm_version;


struct svm_node
{
	double value;


	friend std::ostream& operator << (std::ostream& out, const svm_node* sn)
	{
		out << sn->value;
		return out;
	}
};

struct svm_problem
{
	int l;
	double *y;
	struct svm_node **x;

	friend std::ostream& operator << (std::ostream& out, const svm_problem& sp)
	{
		for (int i = 0; i < sp.l; i++) {
			out << sp.y[i] << "(" << (sp.x[i][0]).value;
			for (int j = 1; j < VARS; j++)
				out << ", " << (sp.x[i][j]).value;
			out << ") ";
		}
		return out;
	}
};



enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */

struct svm_parameter
{
	int svm_type;
	int kernel_type;
	int degree;	/* for poly */
	double gamma;	/* for poly/rbf/sigmoid */
	double coef0;	/* for poly/sigmoid */

	/* these are for training only */
	double cache_size; /* in MB */
	double eps;	/* stopping criteria */
	double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
	int nr_weight;		/* for C_SVC */
	int *weight_label;	/* for C_SVC */
	double* weight;		/* for C_SVC */
	double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p;	/* for EPSILON_SVR */
	int shrinking;	/* use the shrinking heuristics */
	int probability; /* do probability estimates */
};

//
// svm_model
// 
struct svm_model
{
	struct svm_parameter param;	/* parameter */
	int nr_class;		/* number of classes, = 2 in regression/one class svm */
	int l;			/* total #SV */
	struct svm_node **SV;		/* SVs (SV[l]) */
	double **sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
	double *probA;		/* pariwise probability information */
	double *probB;
	int *sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

	/* for classification only */

	int *label;		/* label of each class (label[k]) */
	int *nSV;		/* number of SVs for each class (nSV[k]) */
	/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
	/* XXX */
	int free_sv;		/* 1 if svm_model is created by svm_load_model*/
	/* 0 if svm_model is created by svm_train */
};

struct svm_model *svm_train(const struct svm_problem *prob, const struct svm_parameter *param);
void svm_cross_validation(const struct svm_problem *prob, const struct svm_parameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const struct svm_model *model);
struct svm_model *svm_load_model(const char *model_file_name);

int svm_get_svm_type(const struct svm_model *model);
int svm_get_nr_class(const struct svm_model *model);
void svm_get_labels(const struct svm_model *model, int *label);
void svm_get_sv_indices(const struct svm_model *model, int *sv_indices);
int svm_get_nr_sv(const struct svm_model *model);
double svm_get_svr_probability(const struct svm_model *model);

double svm_predict_values(const struct svm_model *model, const struct svm_node *x, double* dec_values);
double svm_predict(const struct svm_model *model, const struct svm_node *x);
double svm_predict_probability(const struct svm_model *model, const struct svm_node *x, double* prob_estimates);

void svm_free_model_content(struct svm_model *model_ptr);
void svm_free_and_destroy_model(struct svm_model **model_ptr_ptr);
void svm_destroy_param(struct svm_parameter *param);

const char *svm_check_parameter(const struct svm_problem *prob, const struct svm_parameter *param);
int svm_check_probability_model(const struct svm_model *model);

void svm_set_print_string_function(void (*print_func)(const char *));

int svm_model_visualization(const svm_model *model, Equation* equ);

void print_svm_samples(const svm_problem *sp);

struct svm_model *svm_I_train(const struct svm_problem *prob, const struct svm_parameter *param);


//#ifdef __cplusplus
//}
//#endif

//void print_null(const char *s) {}
class SVM // : public ClassifyAlgo
{
	public:
		svm_model* model;
		Equation* main_equation;
		svm_parameter param;
		svm_problem problem;

		SVM(void (*f) (const char*) = NULL)
		{
			problem.l = 0;

			main_equation = NULL;
			model = NULL;

			param.svm_type = C_SVC;
			param.kernel_type = LINEAR;
			param.degree = 3;
			param.gamma = 0;	// 1/num_features
			param.coef0 = 0;
			param.nu = 0.5;
			param.cache_size = 100;
			//	param.C = 1;
			//param.C = DBL_MAX;
			param.C = 1000;
			param.eps = 1e-3;
			param.p = 0.1;
			param.shrinking = 1;
			param.probability = 0;
			param.nr_weight = 0;
			param.weight_label = NULL;
			param.weight = NULL;
			if (f != NULL)
				svm_set_print_string_function(f);
		}

		~SVM()
		{
			if (model != NULL)
				delete model;
			//if (problem.y != NULL)
			//	delete problem.y;
			// here we should check x[i] for each.
			// be careful about whether it is imported from double trace set or int trace set.
			// these two cases should be handled separatly.
		}

		virtual int prepare_training_data(States* gsets, int& pre_positive_size, int& pre_negative_size)
		{
			int cur_positive_size = gsets[POSITIVE].size();
			int cur_negative_size = gsets[NEGATIVE].size();

			std::cout << "+[";
			std::cout << cur_positive_size - pre_positive_size << "|";
			std::cout << cur_negative_size - pre_negative_size << "";
			std::cout << "] ==> [";
			std::cout << cur_positive_size << "+|";
			std::cout << cur_negative_size << "-";
			std::cout << "]";


			// prepare new training data set
			// training set & label layout:
			// data :  0 | positive states | negative states | ...
			// label:    | 1, 1, ..., 1, . | -1, -1, ..., -1, -1, -1, ...
			// move the negative states from old OFFSET: [pre_positive_size] to new OFFSET: [cur_positive_size]
			double** training_set = (double**)(problem.x);
			double* training_label = (double*)(problem.y);

			memmove(training_set + cur_positive_size, training_set + pre_positive_size, pre_negative_size * sizeof(double*));
			// add new positive states at OFFSET: [pre_positive_size]
			for (int i = pre_positive_size; i < cur_positive_size; i++) {
				training_set[i] = gsets[POSITIVE].values[i];
				training_label[i] = 1;
			}
			// add new negative states at OFFSET: [cur_positive_size + pre_negative_size]
			for (int i = pre_negative_size; i < cur_negative_size; i++) {
				training_set[cur_positive_size + i] = gsets[NEGATIVE].values[i];
			}
			problem.l = cur_positive_size + cur_negative_size;
			pre_positive_size = cur_positive_size;
			pre_negative_size = cur_negative_size;
			return 0;
		}


		virtual int train() 
		{
			if (problem.y == NULL || problem.x == NULL)
				return -1;
			const char* error_msg = svm_check_parameter(&problem, &param);
			if (error_msg) {
				std::cout << "ERROR: " << error_msg << std::endl;
				exit(-1);
			}
			model = svm_train(&problem, &param);
			/*for (int i = 0; i < 100000; i++)
				if (sin(i) + cos(i) > 1.414)
					std::cout << ".";
					*/
			main_equation = new Equation();
			svm_model_visualization(model, main_equation);
			svm_free_and_destroy_model(&model);
			return 0;
		}


		int predict(double* v)
		{
			if (main_equation == NULL) return -2;
			if (v == NULL) return -2;
			double res = Equation::calc(main_equation, v);
			if (res >= 0) return 1;
			else return -1;
		}


		virtual double predict_on_training_set()
		{
			if (problem.l <= 0) return 0;
			int pass = 0;
			for (int i = 0; i < problem.l; i++) {
				pass += (Equation::calc(main_equation, (double*)problem.x[i]) * problem.y[i] > 0) ? 1 : 0;
			}
			return static_cast<double>(pass) / problem.l;
		}


		friend std::ostream& operator << (std::ostream& out, const SVM& svm) {
			out << *svm.main_equation; // << std::endl;
			return out;
		}


		virtual int size()
		{
			return problem.l;
		}
	private:
};

#endif /* _LIBSVM_H */
