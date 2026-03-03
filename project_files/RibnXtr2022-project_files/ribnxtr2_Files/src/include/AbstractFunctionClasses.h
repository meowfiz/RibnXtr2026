#ifndef	ABSTRACT_FUNCTION_CLASSES
#define	ABSTRACT_FUNCTION_CLASSES
//test
/*
	class Function
	{
		public:
			Function(){};
			virtual	double GetFunctionValue(double *)=0;
			
	};
*/		
	class Function
	{
		public:
			Function(){};
		
			virtual	double	GetFunctionValue(double ,double *){return 0;}
			virtual	void	CalculateDerivative(double *Deriv,double *Params){};
			virtual void	PrecalculateVariables(double*){};
			virtual void    intit()=0;
			virtual double	GetFunctionValue(double *){return 0;}
			virtual double	GetLastValue(){return valueFUN;}
	protected:
			double valueFUN;
			//double Dist[3];
			//double r1[3];
	};
	


#endif //   ABSTRACT_FUNCTION_CLASSES
