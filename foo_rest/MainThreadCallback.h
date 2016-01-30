#pragma once

#include <functional>

#include "foobar2000/SDK/foobar2000.h"


//Runs code on the main thread via a callback from foobar.
class MainThreadCallback : public main_thread_callback
{
public:
	MainThreadCallback() {}

	MainThreadCallback(std::function<void()> f) { func = f; }

	virtual void callback_run();

	//These two never seem to be called.
	virtual int service_release() throw() override { return 1; }
	virtual int service_add_ref() throw() override { return 1; }

	bool isCompleted = false;

private:
	std::function<void()> func;
};

template <typename T>
class MainThreadCallback2 : public MainThreadCallback
{
public:
	MainThreadCallback2() {}

	MainThreadCallback2(std::function<T()> f) { func2 = f; }

	virtual void callback_run() override
	{
		result = func2();
		isCompleted = true;
	}

	T result;

private:
	std::function<T()> func2;
};

service_ptr_t<MainThreadCallback> runOnMainThread_(const std::function<void()>& func);

template <typename T>
service_ptr_t<MainThreadCallback2<T>> runOnMainThread_(const std::function<T()>& func)
{
	static_api_ptr_t<main_thread_callback_manager> callback_manager;
	service_ptr_t<MainThreadCallback2<T>> callback = new service_impl_t<MainThreadCallback2<T>>(func);
	callback_manager->add_callback(callback);

	return callback;
}

