#pragma once

#include "range.h"

#include <vector>

namespace atr {
namespace collections {

	template<typename tp_value>
	class tm_range_set final {
	public:
		using t_this					= tm_range_set;
		using t_value					= tp_value;
		using t_range					= atr::sequences::tm_range<t_value>;
		using t_array					= std::vector<t_range>;

		using t_index					= typename t_array::size_type;
		using t_citerator				= typename t_array::const_iterator;
		using t_criterator				= typename t_array::const_reverse_iterator;

		using size_type					= typename t_array::size_type;
		using const_iterator			= typename t_array::const_iterator;
		using const_reverse_iterator	= typename t_array::const_reverse_iterator;
		
	private:
		t_index find_index(t_index first, t_index last, const t_value& value) const;
		bool previos_append_value(t_index index, const t_value& value, const t_value& append);

	private:
		t_array				array;

	public:
		inline bool is_empty(void) const								{ return array.empty(); }
		inline t_index get_size(void) const								{ return array.size(); }
		inline const t_range* get_data(void) const						{ return array.data(); }
		inline t_citerator get_begin(void) const						{ return array.begin(); }
		inline t_citerator get_end(void) const							{ return array.end(); }
		inline t_criterator get_begin(void) const						{ return array.rbegin(); }
		inline t_criterator get_end(void) const							{ return array.rend(); }

		inline bool empty(void) const									{ return array.empty(); }
		inline size_type size(void) const								{ return array.size(); }
		inline const t_range* data(void) const							{ return array.data(); }
		inline const_iterator begin(void) const							{ return array.begin(); }
		inline const_iterator end(void) const							{ return array.end(); }
		inline const_reverse_iterator rbegin(void) const				{ return array.rbegin(); }
		inline const_reverse_iterator rend(void) const					{ return array.rend(); }

		inline bool test_index(t_index index) const						{ return t_index(0) <= index && index < get_size(); }

		inline const t_range& operator [] (t_index index) const			{ return array[index]; }
		inline const t_range& get_at(t_index index) const				{ return array[index]; }

	public:
		t_index find(const t_value& value) const						{ t_index i = find_index(value); return (i < get_size() && array[i].test_right_or_equal_value(value)) ? i : t_index(-1); }
		inline bool contains(const t_value& value) const				{ return find(value) != t_index(-1); }

	public:
		bool add(const t_value& value);
		bool add(const t_range& range);
		inline bool add(const t_this& obj)								{ return add(obj.array.begin(), obj.array.end()); }
		template<typename T>
		inline bool add(T first, T last)								{ bool ret = false; if(first != last) do { ret |= add(*first); } while(++first != last); return ret; }
/*		bool remove(const t_value& value);								// request realization
		bool remove(const t_range& range);								// request realization
		bool remove(const t_this& obj)									{ return remove(obj.array.begin(), obj.array.end()); }
		template<typename T>
		bool remove(T first, T last)									{ bool ret = false; if(first != last) do { ret |= remove(*first); } while(++first != last); return ret; }*/

		inline bool clear(void)											{ t_index t = get_size(); array.clear(); return t != t_index(0); }

	public:
		inline t_this& operator = (const t_this& obj)					{ array = obj.array; return *this; }
		inline t_this& operator = (t_this&& obj)						{ array = std::move(obj.array); return *this; }

	public:
		inline tm_range_set()
		{
		}
		inline tm_range_set(const t_this& obj)
			: array(obj.array)
		{
		}
		inline tm_range_set(t_this&& obj)
			: array(std::move(obj.array))
		{
		}
	};

	template<typename tp_value>
	typename tm_range_set<tp_value>::t_index tm_range_set<tp_value>::find_index(t_index first, t_index last, const t_value& value) const
	{
		if (first < last)
		{
			t_index			m;
			const t_range*	r;
			do {
				m = first + (last - first) >> 1;
				r = array.data() + m;
				if (r->test_left_or_equal_value(value))
					last = m;
				else
					first = m + 1;
			} while (first != last);
		}
		return first;
	}

	template<typename tp_value>
	bool tm_range_set<tp_value>::previos_append_value(t_index index, const t_value& value, const t_value& append)
	{
		if (index != 0)
		{
			t_range* p = array.data() + (index - 1);
			if (p->test_near_right_value(value))
			{
				p->set_max(append);
				return true;
			}
		}
		return false;
	}

	template<typename tp_value>
	bool tm_range_set<tp_value>::add(const t_value& value)
	{
		t_index c = get_size();
		t_index i = find_index(0, c, value);
		if (i < c)
		{
			t_range* r = array.data() + i;
			if (r->test_right_or_equal_value(value))
				return false;
			if (r->test_near_left_value(value))
			{
				if (previos_append_value(i, value, r->acc_max()))
				{
					array.erase(array.begin() + i);
					return true;
				}
				r->set_min(value);
				return true;
			}
			if (previos_append_value(i, value, value))
				return true;
			array.insert(array.begin() + i, t_range(value));
			return true;
		}
		if (!previos_append_value(i, value, value))
			array.push_back(t_range(value));
		return true;
	}

	template<typename tp_value>
	bool tm_range_set<tp_value>::add(const t_range& range)
	{
		if (range.is_single_value())
			return add(range.acc_min());
		t_index c = get_size();
		t_index b = find_index(0, c, range.acc_min());
		if (b < c)
		{
			t_range* r = array.data() + b;
			t_index  e = find_index(b, c, range.acc_max());
			if (e < c)
			{
				if (b == e)
				{
					if (r->test_right_or_equal_value(range.acc_min()))
						return false;
					if (r->test_right_or_equal_value(range.make_right_near_value()))
					{
						if (previos_append_value(b, range.acc_min(), r->acc_max()))
						{
							array.erase(array.begin() + b);
							return true;
						}
						r->set_min(range.acc_min());
						return true;
					}
					array.insert(array.begin() + b, range);
					return true;
				}
				t_range* p = array.data() + e;
				t_value& m = range.acc_max();
				if (p->test_right_or_equal_value(range.make_right_near_value()))
				{
					m = p->acc_max();
					++e;
				}
				if (r->test_right_or_equal_value(range.acc_min()))
				{
					r->set_max(m);
					array.erase(array.begin() + b + 1, array.begin() + e);
					return true;
				}
				if (previos_append_value(b, range->acc_min(), m))
				{
					array.erase(array.begin() + b, array.begin() + e);
					return true;
				}
				r->assign(range.acc_min(), m);
				array.erase(array.begin() + b + 1, array.begin() + e);
				return true;
			}
			if (previos_append_value(b, range->acc_min(), r->acc_max()))
			{
				array.erase(array.begin() + b, array.begin() + e);
				return true;
			}
			*r = range;
			array.erase(array.begin() + b + 1, array.begin() + e);
			return true;
		}
		if (!previos_append_value(b, range->acc_min(), range->acc_max()))
			array.push_back(range);
		return true;
	}
} // collections
} // atr