#ifndef CHATTER_FIRMWARE_WITHLISTENERS_H
#define CHATTER_FIRMWARE_WITHLISTENERS_H

#include <unordered_set>

template<typename T>
class WithListeners {
public:
	void addListener(T* listener) {
		listeners.insert(listener);
	}

	void removeListener(T* listener){
		auto it = listeners.find(listener);
		if(it == listeners.end()) return;
		listeners.erase(it);
	}

protected:
	const std::unordered_set<T*>& getListeners() const {
		return listeners;
	}

private:
	std::unordered_set<T*> listeners;

};

#endif //CHATTER_FIRMWARE_WITHLISTENERS_H
