#if !defined(AFX_RESULT_CACHE_H__D378EAC0_3515_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_RESULT_CACHE_H__D378EAC0_3515_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
	map<T, U> m_map;
};

#endif // !defined(AFX_RESULT_CACHE_H__D378EAC0_3515_11D5_B606_0000B4936994__INCLUDED_)
