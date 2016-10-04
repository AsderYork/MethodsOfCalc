#include "LambdaMath.h"

typedef float TYPE;



OneParamFunc Const(TYPE A)
{
	return [A](TYPE x) {return A;};
}

OneParamFunc Sum(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B)
{
	return [A, B](TYPE x) {return A(x) + B(x);};
}

OneParamFunc Mul(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B)
{
	return [A, B](TYPE x) {return A(x)*B(x);};
}

OneParamFunc Div(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B)
{
	return [A, B](TYPE x) {return A(x) / B(x);};
}



