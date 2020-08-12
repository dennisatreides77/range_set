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

		inline const t_range& operator [] (size_type index) const		{ return array[index]; }
		inline const t_range& get_at(size_type index) const				{ return array[index]; }

	public:
		t_index find(const t_value& value) const;						// request realization
		inline bool contains(const t_value& value) const				{ return find(value) != t_index(-1); }

	public:
		bool add(const t_value& value);									// request realization
		bool add(const t_range& range);									// request realization
		bool add(const t_this& obj);									// request realization
		template<typename T>
		bool add(T first, T last);										// request realization
		bool remove(const t_value& value);								// request realization
		bool remove(const t_range& range);								// request realization
		bool remove(const t_this& obj);									// request realization
		template<typename T>
		bool remove(T first, T last);									// request realization

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
} // collections
} // atr