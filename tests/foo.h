#ifndef _FOO_H_
#define _FOO_H_

#include <iostream>
#include <dp14/factory.h>

#ifdef _WIN32
    #ifdef foo_EXPORTS
        #define foo_API __declspec(dllexport)
    #else
		#define foo_API __declspec(dllimport)
    #endif
#else
    #ifdef foo_EXPORTS
		#define foo_API __attribute__((visibility("default")))
    #else
        #define foo_API
    #endif
#endif

namespace foo {

class foo_API Base
{
public:
	DEFINE_KEY(Base)
	using factory = dp14::factory<Base, std::string, int>;

	// this let registrations since externals libraries
	static Base::factory& get_factory();

	explicit Base(const std::string& name, int q)
		: _name(name)
		, _q(q)
	{
		std::cout << "constructor " << _name << " - " << _q << std::endl;
	}

protected:
	std::string _name;
	int _q;
};

}

#ifdef _WIN32

#include "Windows.h"

class load_library
{
public:
	load_library(const std::string& libname)
	{
		_handle = LoadLibrary((libname + ".dll").c_str());
		if (!_handle)
		{
			throw std::runtime_error("error loading library");
		}
	}

	~load_library()
	{
		FreeLibrary(_handle);
	}
protected:
	HMODULE _handle;
};

#else

#include <dlfcn.h>

class load_library
{
public:
	load_library(const std::string& libname, const std::string& prefix = "lib")
	{
		printf("loading %s%s\n", prefix.c_str(), libname.c_str());
		_handle = dlopen((prefix + libname + ".so").c_str(), RTLD_NOW); // RTLD_LAZY
		// _handle = dlopen((prefix + libname + ".so").c_str(), RTLD_LAZY); // RTLD_LAZY
		if (_handle == nullptr)
		{
			// fputs (dlerror(), stderr);
			throw std::runtime_error("error loading library");
		}
	}

	~load_library()
	{
		dlclose(_handle);
	}
protected:
	void* _handle;
};

#endif

#endif

