#include "../Headers/SocketObject.h"
#include <future>
using namespace std;



void SocketObject::launchReadThread() {

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	bool checkThreadFunction = true;

	spawn();
	mutex mtx;
	while (checkThreadFunction) {

		valread = read(sendReceiveDataSocket, buffer, 1024);

		string userData(buffer);
		
		cout << userData << endl;
		unique_lock<mutex> lock(mtx);
		if (timeOut < maxTimeOut) {
			timeOut = 0;
			if (userData.find("javi") != string::npos) {
				checkThreadFunction = false;
			}
		} else {
			checkThreadFunction = false;
			checkTimeOut = false;
			// Mandará mensaje de time out al cliente y se cierra
			// la conexión
		}
	}

	removeThread(this_thread::get_id());
}

void SocketObject::timeOutData() {
	mutex mtx;
	while (checkTimeOut) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		unique_lock<mutex> lock(mtx);
		timeOut += 1;
	}
}

void SocketObject::spawn() {
	thread timeOutThread(&SocketObject::timeOutData, this);
	timeOutThread.detach();
}

void SocketObject::removeThread(thread::id id) {
	mutex internalMtx;
	lock_guard<mutex> lock(internalMtx);
	close(sendReceiveDataSocket);
	auto iter = find_if(allSockets.begin(), allSockets.end(), [=](thread& t) { return (t.get_id() == id); });
	if (iter != allSockets.end()) {
		iter->detach();
		allSockets.erase(iter);
		//cout << sendReceiveDataSocket << endl;
	}
	cout << allSockets.size() << endl;
}
