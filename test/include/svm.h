#ifndef _LIBSVM_H_
#define _LIBSVM_H_
#include "header.h"
#include <iostream>
#include <stdarg.h>
#include "float.h"
#define LIBSVM_VERSION 320


//#ifdef __cplusplus
//extern "C" {
//#endif

extern int libsvm_version;
extern const int vars;
//const int max_items = 1000000;



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

	friend std::ostream& operator << (std::ostream& out, const svm_problem* sp)
	{
		for (int i = 0; i < sp->l; i++) {
			out << sp->y[i] << "(" << (sp->x[i][0]).value;
			for (int j = 1; j < vars; j++)
				out << ", " << (sp->x[i][j]).value;
//				out << ", " << &(sp->x[i][j]);
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
class SVM_algo // : public ClassifyAlgo
{
	public:
		svm_model* model;
		Equation* equation;
		svm_parameter param;
		svm_problem problem;

		SVM_algo(void (*f) (const char*) = NULL)
		{
#ifndef __OPT
			problem.y = new double [max_items];
			problem.x = new svm_node* [max_items];
#endif
			problem.l = 0;
			
			equation = NULL;
			model = NULL;

			param.svm_type = C_SVC;
			param.kernel_type = LINEAR;
			param.degree = 3;
			param.gamma = 0;	// 1/num_features
			param.coef0 = 0;
			param.nu = 0.5;
			param.cache_size = 100;
			//	param.C = 1;
			param.C = DBL_MAX;
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

		~SVM_algo()
		{
			if (model != NULL)
				delete model;
			//if (problem.y != NULL)
			//	delete problem.y;
			// here we should check x[i] for each.
			// be careful about whether it is imported from double trace set or int trace set.
			// these two cases should be handled separatly.
#ifndef __OPT
			if (problem.x != NULL) {
				for (int i = 0; i < problem.l; i++)
					if (problem.x[i] != NULL)
						delete problem.x[i];
					delete []problem.x;
			}
#endif
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
			equation = new Equation();
			svm_model_visualization(model, equation);
			svm_free_and_destroy_model(&model);
			return 0;
		}

/*
		template<class T>
		int insertFromTraceSet(TraceSet<T>* ts)
		{
			int l = 0;
			for (Trace<int>* lt = ts->first; lt != NULL; lt = lt->next)
					l += lt->length;
			if (problem.l + l > max_items) {
				std::cout << "exceed max items SVM can handle." << std::endl;
				return -1;
			}
			int i = problem.l;
			problem.l += l;
			for (Trace<T>* lt = ts->first; lt != NULL; lt = lt->next) {
				for (State<T>* ls = lt->first; ls != NULL; ls = ls->next) {
					problem.y[i] = ls->label;
					problem.x[i] = new svm_node[vars];
					for (int j = 0; j < vars; j++) {
						problem.x[i][j].value = ls->values[j];
					}
					i++;
				}
			}
			return l;
		}
		*/

		template<class T>
		int predict(T* v)
		{
			if (equation == NULL) return -2;
			if (v == NULL) return -2;
			double res = Equation::calc<T>(equation, v);
			if (res >= 0) return 1;
			else return -1;
		}


		virtual double predictOnProblem()
		{
			if (problem.l <= 0) return 0;
			int pass = 0;
			for (int i = 0; i < problem.l; i++) {
				pass += (Equation::calc(equation, (double*)problem.x[i]) * problem.y[i] > 0) ? 1 : 0;
			}
			return (double)pass / problem.l;
		}


		friend std::ostream& operator << (std::ostream& out, const SVM_algo sa) {
			out << "Learnt from SVM...";
			out << sa.equation << std::endl;
			return out;
		}


		virtual int roundoff(Equation* p)
		{
			equation->roundoff(&p[0]);
			return 1;
		}


		virtual int size()
		{
			return problem.l;
		}
	private:
};


const int max_equ = 8;

class SVM_I_algo // : public SVM_algo
{
public:
	svm_model* model;
	Equation* equation[max_equ];
	int equ_num;
	svm_parameter param;
	svm_problem problem1;  // 1
	svm_problem problem2;  // -1

	SVM_I_algo(void(*f) (const char*) = NULL)
	{
#ifndef __OPT
		problem1.y = new double[max_items];
		problem1.x = new svm_node*[max_items];
		problem2.y = new double[max_items];
		problem2.x = new svm_node*[max_items];
#endif
		problem1.l = 0;
		problem2.l = 0;

		equ_num = 0;
		for (int i = 0; i < max_equ; i++)
			equation[i] = new Equation();
		model = NULL;

		param.svm_type = C_SVC;
		param.kernel_type = LINEAR;
		param.degree = 3;
		param.gamma = 0;	// 1/num_features
		param.coef0 = 0;
		param.nu = 0.5;
		param.cache_size = 100;
		//	param.C = 1;
		param.C = DBL_MAX;
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

	~SVM_I_algo()
	{
		if (model != NULL)
			svm_free_and_destroy_model(&model);
	}


/*
	template<class T>
	int insertFromTraceSet(TraceSet<T>* ts)
	{
		svm_problem* problem;
		if (ts->label == 1) problem = &problem1;
		if (ts->label == -1) problem = &problem2;

		int l = 0;
		for (Trace<int>* lt = ts->first; lt != NULL; lt = lt->next)
			l += lt->length;
		if (problem->l + l > max_items) {
			std::cout << "exceed max items SVM can handle." << std::endl;
			return -1;
		}
		int i = problem->l;
		problem->l += l;
		for (Trace<T>* lt = ts->first; lt != NULL; lt = lt->next) {
			for (State<T>* ls = lt->first; ls != NULL; ls = ls->next) {
				problem->y[i] = ls->label;
				problem->x[i] = new svm_node[vars];
				for (int j = 0; j < vars; j++) {
					problem->x[i][j].value = ls->values[j];
				}
				i++;
			}
		}
		return l;
	}
	*/

	template<class T>
	int predict(T* v)
	{
		if (equ_num <= 0) return -2;
		if (v == NULL) return -2;
		/*
		 * We use conjunction of positive as predictor.
		 * For example, (A >= 0) /\ (B >= 0) /\ (C >= 0) /\ ...
		 * Only when the give input pass all the equations, it returns 1;
		 * Otherwise, -1 will be returned.
		*/
		for (int i = 0; i < equ_num; i++) {
			double res = Equation::calc(equation[i], v);
			if (res < 0) return -1;
		}
		return 1;
	}


	double predictOnProblem()
	{
		int total = problem1.l + problem2.l;
		int pass = 0;
		if (problem1.l > 0) {
			for (int i = 0; i < problem1.l; i++) {
				pass += (predict<double>((double*)problem1.x[i]) >= 0) ? 1 : 0;
			}
		}
		if (problem2.l > 0) {
			for (int i = 0; i < problem2.l; i++) {
				pass += (predict<double>((double*)problem2.x[i]) < 0) ? 1 : 0;
			}
		}
		return (double)pass / total;
		return 0;
	}


	double CheckPostive()
	{
		int total = problem1.l;
		int pass = 0;
		std::cout << "\t";
		if (problem1.l > 1) {
			for (int i = 0; i < problem1.l; i++) {
				bool bPass = (Equation::calc(equation[equ_num], (double*)problem1.x[i]) * problem1.y[i] >= 0) ? 1 : 0;
				pass += bPass;
			/*	
				std::cout << "(";
				for (int j = 0; j < vars - 1; j++)
					std::cout << problem1.x[i][j].value << ",";
				std::cout << problem1.x[i][vars-1].value << ")";
				if (bPass) std::cout << "Pass\t";
				else std::cout << "Fail\t";
				if ((i + 1) % 4 == 0)
					std::cout << std::endl << "\t";
			*/
			}
		}
		std::cout << std::endl << pass << "/" << total << "..";
		return (double)pass / total;
	}


	int getMisclassified(int& idx) // negative points may be misclassified.
	{
		if ((problem2.y == NULL) || (problem2.x == NULL))
			return -1;
		if (equ_num <= 0) {
			idx = 0;
			return 0;
		}
		for (int i = 0; i < problem2.l; i++)
			if (predict<double>((double*)problem2.x[i]) >= 0) {
				idx = i;
				return 0;
			}
		idx = -1;
		return 0;
	}


	int train()
	{
		if (problem1.y == NULL || problem1.x == NULL
			|| problem2.y == NULL || problem2.x == NULL)
			return -1;

		
		for (equ_num = 0; equ_num < max_equ; equ_num++) {
			int misidx = -1;
			int ret = getMisclassified(misidx);
			if (ret == -1) return -1;
			if ((ret == 0) && (misidx == -1)) {
				std::cout << "finish classified..." << std::endl;
				return 0;
			}

			int length = problem1.l;
			problem1.y[length] = -1;
			problem1.x[length] = problem2.x[misidx];
			problem1.l++;

			std::cout << "." << equ_num; // << std::endl;

			model = svm_train(&problem1, &param);
			svm_model_visualization(model, equation[equ_num]);
			
			double precision = CheckPostive();
			svm_free_and_destroy_model(&model);
			problem1.l--;
			std::cout << " On training set precision: " << precision * 100 << "%" << std::endl;
			//std::cout << "\n On whole set precision: " << predictOnProblem() * 100 << "%" << std::endl;
		}
		return 0;
	}


	friend std::ostream& operator << (std::ostream& out, const SVM_I_algo si) {
		out << "Learnt from SVM-I...";
		if (si.equ_num <= 0) {
			out << "Having Learnt...\n";
			return out;
		}
		out << std::setprecision(16);
		out << "{ \n\t    " << *(si.equation[0]);
		for (int i = 1; i < si.equ_num; i++) {
			out << " \n\t /\\ " << *(si.equation[i]);
		}
		out << "}\n";
		return out;
	}


	int size()
	{
		return problem1.l + problem2.l;
	}


	 int roundoff(Equation *p)
	{
		for (int i = 0; i < equ_num; i++) {
			equation[i]->roundoff(&p[i]);
		}
		return equ_num;
	}
private:
};


#endif /* _LIBSVM_H */
