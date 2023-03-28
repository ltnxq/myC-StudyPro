#ifndef __RAII_WRAPPER_H__
#define __RAII_WRAPPER_H__

template<class T>
class RAIIWrapper {
	RAIIWrapper(T* p)£ºptr(p){}

	virtual ~RAIIWrapper() {
		if (ptr != null) {
			delete ptr;
			ptr = NULL;
		}
	}
	private£º
		T* ptr;
};

#endif 
