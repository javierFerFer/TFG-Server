#include "../Headers/SocketObject.h"
using namespace std;



void SocketObject::launchReadThread() {

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		send(sendReceiveDataSocket, "mensaje", strlen("mensaje"), 0);
		removeThread(this_thread::get_id());
	
	//cout << allSockets.size() << endl;
	//allSockets.erase(remove(allSockets.begin(), allSockets.end(), this_thread::get_id), allSockets.end());
}

void SocketObject::removeThread(std::thread::id id) {
	mutex internalMtx;
	close(sendReceiveDataSocket);
	std::lock_guard<std::mutex> lock(internalMtx);
	
	auto iter = std::find_if(allSockets.begin(), allSockets.end(), [=](std::thread& t) { return (t.get_id() == id); });
	if (iter != allSockets.end()) {
		iter->detach();
		allSockets.erase(iter);
		//cout << sendReceiveDataSocket << endl;
	}

	cout << allSockets.size() << endl;
}
