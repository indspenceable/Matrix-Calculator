#ifndef __C_PTR_H__
#define __C_PTR_H__

class Counter {
public:
	Counter() {
		count = 1;
	}
 	void attach() {
		count++;
	}
	int detach() {
		return --count;
	}
private:
	int count;
};

template<typename T>
class C_Ptr {
public:
	explicit C_Ptr(): ptr(0), c(0) {}
	explicit C_Ptr(T* const p): ptr(p), c(0)
	{
		if (p) c = new Counter;
	}
	
	
	C_Ptr(const C_Ptr<T>& r): ptr(r.ptr), c(r.c) {
		if (c) c->attach();
	}
	~C_Ptr() {
		release();
	}
	//Instead of a copy constructor, which is DANGEROUS
	//spawn() makes a copy in a differnet type of C_Ptr
	template <typename Y> C_Ptr<Y> spawn(){
		C_Ptr<Y> rtn;
		rtn.ptr = dynamic_cast<Y*>(ptr);
		if (rtn.ptr) rtn.c = c;
		else rtn.c = 0;
		if ( rtn.c )
			rtn.c->attach();
		return rtn;
	}
	//bool operator==(const C_Ptr& r) {
	//	return (ptr == r.ptr);
	//}
	template <typename Y> bool operator==(const C_Ptr<Y>& r) {
		return (ptr == r.ptr);
	}
	void release() {
		if (c && c->detach() == 0) {
			delete ptr;
			delete c;
		}
	}
	const C_Ptr& operator=(const C_Ptr& r) {
		if (*this == r) return *this;
		release();
		ptr = r.ptr;
		c = r.c;
		if ( c != 0 ) c->attach();
		return *this;
	}
	
	T* operator->() {
		return ptr;
	}
	T& operator*() {
		return *ptr;
	}
	bool equals(T* p) {
		return (ptr == p);
	}
	bool null() {
		return (ptr == 0);
	}
private:
	template<typename Y> friend class C_Ptr;
	T* ptr;
	Counter* c;
};

#endif