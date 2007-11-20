#pragma once

template <class T, class U>
class Cresult_cache
{
public:
	bool has(T id) const
	{
		return m_map.find(id) != m_map.end();
	}

	const U& get(T id) const
	{
		return m_map.find(id)->second;
	}

	const U& set(T id, const U& v)
	{
		return m_map[id] = v;		
	}
private:
	std::map<T, U> m_map;
};
