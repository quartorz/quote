#pragma once

#include <Windows.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <chrono>

#include <memory>

#include <concurrent_queue.h>

#include <quote/win32/creation_params.hpp>
#include <quote/win32/message_loop.hpp>

namespace quote{ namespace win32{

	struct async_message{
		std::condition_variable cv;
		std::mutex mutex;

		enum message_type{
			create,
			destroy,
		}message;

		std::shared_ptr<void> data;
		bool result;
	};

	template <class Derived>
	// メッセージループを非同期で実行する
	class async_message_loop{
		std::thread thread;
		concurrency::concurrent_queue<std::shared_ptr<async_message>> queue;

	public:
		~async_message_loop()
		{
			if(thread.get_id() != std::thread::id()){
				DWORD id = ::GetThreadId(thread.native_handle());
				::PostThreadMessageW(id, WM_QUIT, 0, 0);
				thread.join();
			}
		}

		bool start_message_loop()
		{
			if(thread.get_id() != std::thread::id())
				return false;

			thread = std::thread([this](){
				if(::CoInitialize(nullptr))
					return;

				::quote::win32::message_loop([this](){
					if(queue.empty()){
						::Sleep(1);
						return;
					}

					std::shared_ptr<async_message> message;
					if(queue.try_pop(message)){
						switch(message->message){
						case async_message::message_type::create:
							{
								auto param = reinterpret_cast<creation_params*>(message->data.get());
								HWND hwnd = ::CreateWindowExW(
									param->exstyle,
									param->classname,
									param->title,
									param->style,
									param->x,
									param->y,
									param->w,
									param->h,
									param->hparent,
									nullptr,
									::GetModuleHandleW(nullptr),
									param->data);
								message->result = hwnd != nullptr;

								std::lock_guard<std::mutex> lock(message->mutex);
								message->cv.notify_one();
							}
							break;
						case async_message::message_type::destroy:
							{
								message->result = ::DestroyWindow(reinterpret_cast<HWND>(message->data.get())) != 0;

								std::lock_guard<std::mutex> lock(message->mutex);
								message->cv.notify_one();
							}
							break;
						}
					}
				});

				::CoUninitialize();
			});

			return true;
		}

		void join_message_loop()
		{
			thread.join();
		}

		bool create(creation_params param)
		{
			if(param.w != CW_USEDEFAULT){
				RECT rc = {0, 0, param.w, param.h};
				::AdjustWindowRectEx(&rc, param.style, FALSE, param.exstyle);
				param.w = rc.right - rc.left;
				param.h = rc.bottom - rc.top;
			}

			if(param.data == nullptr){
				param.data = static_cast<Derived*>(this);
			}

			start_message_loop();

			auto message = std::make_shared<async_message>();
			std::unique_lock<std::mutex> lock(message->mutex);

			message->message = async_message::message_type::create;
			message->data = std::make_shared<creation_params>(param);
			queue.push(message);

			message->cv.wait_for(lock, std::chrono::seconds(3));

			return message->result;
		}

		bool create(const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
		{
			creation_params params = {
				0,
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
				((classname != nullptr) ? classname : static_cast<Derived*>(this)->get_class_name()),
				title,
				((x == INT_MAX || y == INT_MAX) ? CW_USEDEFAULT : x),
				y,
				((w == -1 || h == -1) ? CW_USEDEFAULT : w),
				h,
				nullptr,
			};

			return create(params);
		}

		void destroy()
		{
			// 同じスレッドからじゃないとDestroyWindowできない

			auto message = std::make_shared<async_message>();
			std::unique_lock<std::mutex> lock(message->mutex);

			message->message = async_message::message_type::destroy;
			message->data = std::shared_ptr<void>(static_cast<Derived*>(this)->get_hwnd(), [](HWND){});
			queue.push(message);

			message->cv.wait_for(lock, std::chrono::seconds(3));
		}

		// TODO: 今は使わないからそのうち作る
		// bool create_child()
		// void destroy_child()
	};

} }
