#pragma once
#include <functional>
#include <math.h>
typedef float TYPE;
typedef std::function<TYPE(TYPE)> OneParamFunc;

std::function<TYPE(TYPE)> Const(TYPE A);
std::function<TYPE(TYPE)> Sum(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B);
std::function<TYPE(TYPE)> Mul(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B);
std::function<TYPE(TYPE)> Div(std::function<TYPE(TYPE)> A, std::function<TYPE(TYPE)> B);


