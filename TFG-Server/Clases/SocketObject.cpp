#include "../Headers/SocketObject.h"
#include <future>
using namespace std;



void SocketObject::launchReadThread() {

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	bool checkThreadFunction = true;


	while (checkThreadFunction) {
		
		valread = read(sendReceiveDataSocket, buffer, 1024);

		string userData(buffer);
		
		cout << userData << endl;
		cout << endl;
		if (userData.find("javi") != string::npos) {
			checkThreadFunction = false;
		}

	}
	removeThread(this_thread::get_id());
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
