#pragma once

template <class T, class U>
typename T::mapped_type* find_ptr(T& c, U v)
{
	auto i = c.find(v);
	return i == c.end() ? NULL : &i->second;
}

template <class T, class U>
const typename T::mapped_type* find_ptr(const T& c, U v)
{
	auto i = c.find(v);
	return i == c.end() ? NULL : &i->second;
}
