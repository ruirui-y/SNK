#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <functional>
using namespace std;

using Func = function<void()>;

class DeferRaii
{
public:
	template<typename F>
	DeferRaii(F&& func) : m_func(std::forward<F>(func)) {}

	~DeferRaii()
	{
		m_func();
	}

private:
	Func m_func;
};

#endif // _GLOBAL_H_