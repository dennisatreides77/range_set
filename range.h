#pragma once

#include <type_traits>
#include <algorithm>

namespace atr {
namespace sequences {

	template<typename tp_value>
	class tm_range final {
	public:
		using t_this		= tm_range;
		using t_value		= tp_value;
		using t_difference	= decltype(t_value() - t_value());

	private:
		using t_is_integer	= std::is_integral<t_value>;

	public:
		t_value			min;
		t_value			max;

	private:
		inline static t_difference correct_length(const t_difference& value, const std::false_type&)	{ return value; }
		inline static t_difference correct_length(const t_difference& value, const std::true_type&)		{ return value + 1; }

	private:
		inline bool test_near_left_value(const t_value& value, const std::false_type&) const			{ return false; }
		inline bool test_near_right_value(const t_value& value, const std::false_type&) const			{ return false; }
		inline bool test_near_value(const t_value& value, const std::false_type&) const					{ return false; }

		inline bool test_near_left_value(const t_value& value, const std::true_type&) const				{ return value < min && (value + 1) == min; }
		inline bool test_near_right_value(const t_value& value, const std::true_type&) const			{ return value > max && (max + 1) == value; }
		inline bool test_near_value(const t_value& value, const std::true_type&) const					{ return (value < min) ? (value + 1) == min : ((value > max) ? (max + 1) == value : false); }

		inline static t_value move_left_value(const t_value& value, const std::false_type&)				{ return value; }
		inline static t_value move_right_value(const t_value& value, const std::false_type&)			{ return value; }
		inline static t_value move_left_value(const t_value& value, const std::true_type&)				{ return value - 1; }
		inline static t_value move_right_value(const t_value& value, const std::true_type&)				{ return value + 1; }

	public:
		inline bool is_normalized(void) const															{ return !(max < min); }
		inline bool is_single_value(void) const															{ return min == max; }

		inline t_difference get_length(void) const														{ return correct_length(max - min, t_is_integer()); }

		inline t_value	get_min(void) const																{ return this->min; }
		inline const t_value&	acc_min(void) const														{ return this->min; }
		inline t_value& acc_min(void)																	{ return this->min; }
		inline t_value	set_min(const t_value& value)													{ return this->min = value; }
		inline t_value	get_max(void) const																{ return this->max; }
		inline const t_value& acc_max(void) const														{ return this->max; }
		inline t_value& acc_max(void)																	{ return this->max; }
		inline t_value	set_max(const t_value& value)													{ return this->max = value; }

	public:
		inline bool normalize(void)																		{ if(max < min) { std::swap(min, max); return true; } return false; }

	public:
		inline bool test_near_left_value(const t_value& value) const									{ return t_this::test_near_left_value(value, t_is_integer()); }
		inline bool test_near_right_value(const t_value& value) const									{ return t_this::test_near_right_value(value, t_is_integer()); }
		inline bool test_near_value(const t_value& value) const											{ return t_this::test_near_value(value, t_is_integer()); }
		inline bool test_inner_value(const t_value& value) const										{ return !(max < value) && !(min > value); }
		inline bool test_left_value(const t_value& value) const											{ return value < min; }
		inline bool test_right_value(const t_value& value) const										{ return max < value; }
		inline bool test_left_or_equal_value(const t_value& value) const								{ return !(max < value); }
		inline bool test_right_or_equal_value(const t_value& value) const								{ return !(value < min); }

		inline t_value make_left_near_value(void) const													{ return move_left_value(min, t_is_integer()); }
		inline t_value make_right_near_value(void) const												{ return move_right_value(max, t_is_integer()); }

	public:
		inline static t_this make_unite(const t_this& obj1, const t_this& obj2)							{ return t_this(std::min(obj1.min, obj2.min), std::max(obj1.max, obj2.max)); }
		inline static t_this make_cross(const t_this& obj1, const t_this& obj2)							{ return t_this(std::max(obj1.min, obj2.min), std::min(obj1.max, obj2.max)); }
		inline static bool make_cross(const t_this& obj1, const t_this& obj2, t_this& ret)				{ ret.assign(std::max(obj1.min, obj2.min), std::min(obj1.max, obj2.max)); return ret.is_normalized(); }
		inline static bool test_crossing(const t_this& obj1, const t_this& obj2)						{ return !(std::min(obj1.max, obj2.max) < std::max(obj1.min, obj2.min)); }
		static std::size_t make_exlude(const t_this& obj1, const t_this& obj2, t_this ret[2]);

	public:
		inline void assign(const t_value& min, const t_value& max)										{ this->min = min; this->max = max; return; }
		inline void assign(const t_value&& min, const t_value&& max)									{ this->min = std::move(min); this->max = std::move(max); return; }

	public:
		inline bool operator == (const t_this& obj) const												{ return this->min == obj.min && this->max == obj.max; }
		inline bool operator != (const t_this& obj) const												{ return this->min != obj.min || this->max != obj.max; }

	public:
		inline t_this& operator = (const t_this& obj)													{ this->min = obj.min; this->max = obj.max; return *this; }
		inline t_this& operator = (t_this&& obj)														{ this->min = std::move(obj.min); this->max = std::move(obj.max); return *this; }

	public:
		inline tm_range()
			: t_this::min()
			, t_this::max()
		{
		}
		inline explicit tm_range(const t_value& val)
			: t_this::min(val)
			, t_this::max(val)
		{
		}
		inline tm_range(const t_value& min, const t_value& max)
			: t_this::min(min)
			, t_this::max(max)
		{
		}
		inline tm_range(const t_value&& min, const t_value&& max)
			: t_this::min(min)
			, t_this::max(max)
		{
		}
		inline tm_range(const t_this& obj)
			: t_this::min(obj.min)
			, t_this::max(obj.max)
		{
		}
		inline tm_range(t_this&& obj)
			: t_this::min(std::move(obj.min))
			, t_this::max(std::move(obj.max))
		{
		}
	};

	template<typename tp_value>
	std::size_t tm_range<tp_value>::make_exlude(const t_this& obj1, const t_this& obj2, t_this ret[2])
	{
		t_this cross; (std::max(obj1.min, obj2.min), std::min(obj1.max, obj2.max));
		if (make_cross(obj1, obj2, cross))
		{
			std::size_t cret = 0;
			if (obj1.min < cross.min)
			{
				ret[cret] = t_this(obj1.min, move_left_value(cross.min, t_is_integer()));
				++cret;
			}
			if (cross.max < obj1.max)
			{
				ret[cret] = t_this(move_right_value(cross.max, t_is_integer(), obj1.min));
				++cret;
			}
			return cret;
		}
		ret[0] = obj1;
		return 1;
	}

} // sequence
} // atr
