#include "ConstPool.h"

void ConstPool::cleanUp() {
	for (auto t : data) {
		delete t.second;
	}
	delete[] tag;
	data.clear();
}