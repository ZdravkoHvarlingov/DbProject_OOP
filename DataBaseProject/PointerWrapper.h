#ifndef POINTER_WRAPPER
#define POINTER_WRAPPER

template <typename T>
class PointerWrapper
{
public:
	PointerWrapper(T* _value = nullptr);
	PointerWrapper(const PointerWrapper<T>& other) = delete;
	PointerWrapper<T>& operator=(const PointerWrapper<T>& other) = delete;
	PointerWrapper(PointerWrapper<T>&& other); //move functionality
	PointerWrapper<T>& operator=(PointerWrapper<T>&& other); //move-and-swap idiom can be used
	
	void SetValue(T* _value);
	T* operator ->();
	T& operator*();

	~PointerWrapper();

private:
	T* value;
};

template<typename T>
inline PointerWrapper<T>::PointerWrapper(T * _value)
	: value(_value)
{
}

template<typename T>
inline PointerWrapper<T>::PointerWrapper(PointerWrapper<T>&& other) //rValue reference
	: PointerWrapper<T>()
{
	value = other.value;
	other.value = nullptr;
}

template<typename T>
inline PointerWrapper<T>& PointerWrapper<T>::operator=(PointerWrapper<T>&& other)
{
	if (this != &other)
	{
		delete value;
		value = other.value;
		other.value = nullptr;
	}

	return *this;
}

template<typename T>
inline void PointerWrapper<T>::SetValue(T * _value)
{
	if (value != nullptr)
	{
		delete value;
	}

	value = _value;
}

template<typename T>
inline T * PointerWrapper<T>::operator->()
{
	return value;
}

template<typename T>
inline T& PointerWrapper<T>::operator*()
{
	return *value;
}

template<typename T>
inline PointerWrapper<T>::~PointerWrapper()
{
	delete	value;
}

#endif // !POINTER_WRAPPER
