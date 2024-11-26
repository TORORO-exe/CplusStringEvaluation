#pragma once

template<typename _T>

class Singleton
{
private:
	Singleton& operator=(const Singleton& r) = default;
	Singleton(const Singleton& r) = default;

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	static _T* getInst() {
		static _T inst;
		return &inst;
	}
};