#pragma once

#include "range.h"

#include <set>

namespace atr {
namespace collections {

	template<typename tp_value>
	class tm_range_set final {
	public:
		using t_this		= tm_range_set;
		using t_value		= tp_value;
		using t_range		= atr::sequences::tm_range<t_value>;
		using t_set			= std::set<t_range>;

	private:
		t_set				set;

	public:
		inline const t_set& get_set(void) const		{ return set; }

	public:
		inline tm_range_set()
		{
		}
		inline tm_range_set(const t_this& obj)
			: set(obj.set)
		{
		}
		inline tm_range_set(t_this&& obj)
			: set(std::move(obj.set))
		{
		}
	};
} // collections
} // atr