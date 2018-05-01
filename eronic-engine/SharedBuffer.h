#pragma once

#include <mutex>
#include <atomic>
#include <vector>

template <typename T>
class SharedBuffer
{

public:
	SharedBuffer() :
		_buffer{std::vector<T>(), std::vector<T>() },
		_back_buffer(0),
		_front_buffer(1)
	{ 
	}
	~SharedBuffer() { }

	void add_data(T data) {
		while (true) {
			std::unique_lock<std::mutex> ul(_mx, std::try_to_lock_t());
			if (ul) {
				_buffer[_back_buffer].push_back(data);
				//_mx.unlock();
				break;
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}

	void swap_buffers() {
		while (true) {
			std::unique_lock<std::mutex> ul(_mx, std::try_to_lock_t());
			if (ul) {
				_buffer[_front_buffer].clear();
				if (_back_buffer == 0) {
					_back_buffer = 1;
					_front_buffer = 0;
				}
				else {
					_back_buffer = 0;
					_front_buffer = 1;
				}
				//_mx.unlock();
				break;
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}

	std::vector<T> const& get_data() const{
		return _buffer[_front_buffer];
	}

private:
	std::vector<T> _buffer[2];
	
	int _back_buffer;
	int _front_buffer;

	std::mutex _mx;
	std::condition_variable _cv;
};

