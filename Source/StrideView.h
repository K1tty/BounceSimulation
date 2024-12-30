#pragma once
#include <cassert>


template<typename T>
class TStrideView
{
public:
	TStrideView(const T& Data, size_t Size, size_t StrideBytes);

	const T& operator[] (size_t Index) const;
	size_t GetSize() const;

private:
	const T* Data;
	size_t Size;
	size_t Stride;
};


template<typename T>
TStrideView<T>::TStrideView(const T& _Data, size_t _Size, size_t _StrideBytes)
	: Data(&_Data)
	, Size(_Size)
	, Stride(_StrideBytes)
{}

template<typename T>
const T& TStrideView<T>::operator[] (size_t Index) const
{
	assert(Index < Size);
	return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(Data) + Index * Stride);
}

template<typename T>
size_t TStrideView<T>::GetSize() const
{
	return Size;
}
