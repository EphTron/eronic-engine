#pragma comment (lib, "eronic-engine.lib")
#pragma comment (lib, "ws2_32.lib")

#include <iostream>
#include <thread>
#include <cstring>
#include <sstream>
#include <assert.h>

#include "TCPClient.h"
#include "UDPClient.h"
#include "TCPListener.h"
#include "UDPListener.h"
#include "Socket_WIN.h"
#include "P2PNetworkManager.h"
#include "SharedResource.h"
#include "SharedBuffer.h"

#define MAX_CONN	3
#define PACKAGE_SIZE sizeof(eronic::Package)

static std::mutex mtx_cout;

// Asynchronous output
struct acout
{
	std::unique_lock<std::mutex> lk;
	acout()
		:
		lk(std::unique_lock<std::mutex>(mtx_cout))
	{

	}

	template<typename T>
	acout& operator<<(const T& _t)
	{
		std::cout << _t;
		return *this;
	}

	acout& operator<<(std::ostream& (*fp)(std::ostream&))
	{
		std::cout << fp;
		return *this;
	}
};


class A {
public:
	A() :_b(2) {};
	A(int b) :_b(b) {};
	~A() {};

	void func(std::promise<int> p) {
		Sleep(500);
		p.set_value(_b);
	};
	void vec(std::promise<std::vector<int>> p) {
		Sleep(500);
		std::vector<int> a;
		a.push_back(_b);
		a.push_back(_b);
		p.set_value(a);
	};

	void net(std::promise<eronic::Network*> p) {
		Sleep(500);
		eronic::Network * a = new eronic::Network();
		a->network_id = _b;
		p.set_value(a);
	};
	void print() {
		std::cout << " smell" << std::endl;
	};

private:
	int _b;

};

int main() {
	std::cout << "WORK BENCH 2" << std::endl;
	bool flag = true;
	int app_port = 9173;

	SharedBuffer<int> buf;
	std::thread producer([&]() {
		std::vector<int> de = std::vector<int>();
		for (int i = 0; i != 50; ++i) {
			buf.add_data(i);
			de.push_back(i);
			std::ostringstream os;
			os << "push ";
			for (auto p : de) {
				os << p << " ";
			}
			std::string s;
			s = os.str();
			acout() << s << std::endl;
		}
	});
	std::thread consumer([&]() {
		std::vector<int> prev = std::vector<int>();
		prev.push_back(-1);
		for (int i = 0; i != 50; ++i) {
			buf.swap_buffers();
			std::vector<int> item = buf.get_data();
			prev = item;
			std::ostringstream os;
			os << "-back ";
			for (auto p : prev) {
				os << p << " ";
			}
			std::string s;
			s = os.str();
			acout() << s << std::endl;
			Sleep(5);
		}
	});
	producer.join();
	consumer.join();


	//SharedResource<std::vector<int>> buf;
	//std::thread producer([&]() {
	//	for (int i = 0; i != 50; ++i) {
	//		std::vector<int>* item = buf.start_writing();
	//		if (item != nullptr) {      // Always true
	//			
	//			item->push_back(i);
	//			//std::cout << " push " << i << std::endl;
	//			
	//		}
	//		std::ostringstream os;
	//		os << "push ";
	//		for (auto p : *item) {
	//			os << p << " ";
	//		}
	//		std::string s;
	//		s = os.str();
	//		acout() << s << std::endl;
	//		buf.end_writing();
	//	}
	//});
	//std::thread consumer([&]() {
	//	std::vector<int> prev = std::vector<int>();
	//	prev.push_back(-1);
	//	for (int i = 0; i != 50; ++i) {
	//		std::vector<int>* item = buf.start_reading();
	//		if (item != nullptr) {
	//			assert(item->back() > prev.back());
	//			prev = *item;
	//			std::ostringstream os;
	//			os << "-back ";
	//			for (auto p : prev) {
	//				os << p << " ";
	//			}
	//			std::string s;
	//			s = os.str();
	//			acout() << s << std::endl;
	//			
	//		}
	//		buf.end_reading();
	//		Sleep(5);
	//	}
	//});
	//producer.join();
	//consumer.join();

	//A test = A(6);

	//std::promise<int> _search_promise;
	//std::future<int> _search_future = _search_promise.get_future();
	//std::thread t1(&A::func, std::ref(test), std::move(_search_promise));

	//std::promise<std::vector<int>> vec_promise;// = new std::promise<std::vector<int>>();
	//std::future<std::vector<int>> vec_future= vec_promise.get_future();
	//std::thread t2(&A::vec, std::ref(test), std::move(vec_promise));

	//std::future_status status1;
	//status1 = _search_future.wait_for(std::chrono::seconds(2));
	//if (status1 == std::future_status::ready) {
	//	std::cout << "success " << _search_future.get() << std::endl;
	//}
	//std::future_status status2;
	//status2 = vec_future.wait_for(std::chrono::seconds(2));
	//if (status2 == std::future_status::ready) {
	//	std::cout << "success " << vec_future.get()[0] << std::endl;
	//}
	//t1.join();
	//t2.join();

	//eronic::P2PNetworkManager * peer = new eronic::P2PNetworkManager(2, app_port, 10, true);
	//std::promise<int> p;
	//std::future<int> f = p.get_future();
	//std::thread t3(&eronic::P2PNetworkManager::find_future_networks, std::ref(*peer), std::move(p), 6000, true);

	//std::thread t4;
	//std::future_status status;
	//status = f.wait_for(std::chrono::seconds(7));
	//if (status == std::future_status::ready) {
	//	std::cout << "TEEEESSTT success" << f.get() << std::endl;
	//	t3.join();
	//	t4 = std::thread(&eronic::P2PNetworkManager::run_peer_network, std::ref(*peer));
	//}
	////peer->run_peer_network();
	////std::thread t4 = std::thread(&eronic::P2PNetworkManager::run_peer_network, std::ref(*peer));


	//std::cout << " runs threads" << std::endl;

	//while (true) {
	//	Sleep(2500);
	//	std::cout << "main" << std::endl;
	//}

	system("Pause");
	return 0;
}
