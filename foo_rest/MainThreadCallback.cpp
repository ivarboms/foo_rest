#include "stdafx.h"
#include "MainThreadCallback.h"


service_ptr_t<MainThreadCallback> runOnMainThread_(const std::function<void()>& func)
{
	static_api_ptr_t<main_thread_callback_manager> callback_manager;
	service_ptr_t<MainThreadCallback> callback = new service_impl_t<MainThreadCallback>(func);
	callback_manager->add_callback(callback);

	return callback;
}

void MainThreadCallback::callback_run()
{
	func();
	isCompleted = true;
}
