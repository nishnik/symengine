#include <symengine/sets.h>
#include <symengine/integer.h>

namespace SymEngine {

	Interval::Interval(const RCP<const Basic> &start, const RCP<const Basic> &end, bool left_open, bool right_open)
		: start_(start), end_(end), left_open_(left_open), right_open_(right_open) {
			SYMENGINE_ASSERT(is_canonical());
		}

	Interval::Interval() {
		start_ = make_rcp<const Integer>(0);
		end_ = make_rcp<const Integer>(0);
		left_open_ = true;
		right_open_ = true;
	}
	bool Interval::is_canonical() {
		if(is_a<Integer>(*start_) && is_a<Integer>(*end_)) {
			return true;
		}
		else
			return false;
	}

	void Interval::interval_union(const Interval &other, Interval &result) {

	}

	void Interval::interval_intersection(const Interval &other, Interval &result) {
		if(start_->compare(*(other.get_end())) <= 0 && end_->compare(*(other.get_start())) >= 0) {
			if(start_->compare(*(other.get_start())) == -1) {
				result.set_start(other.get_start());
				result.set_left_open(other.is_left_open());
			}
			else if(start_->compare(*(other.get_start())) == 1) {
				result.set_start(start_);
				result.set_left_open(left_open_);
			}
			else {
				result.set_start(start_);
				result.set_left_open(left_open_ || other.is_left_open());
			}

			if(end_->compare(*(other.get_end())) == -1) {
				result.set_end(end_);
				result.set_right_open(right_open_);
			}
			else if(end_->compare(*(other.get_end())) == 1) {
				result.set_end(other.get_end());
				result.set_right_open(other.is_right_open());
			}
			else {
				result.set_end(end_);
				result.set_right_open(right_open_ || other.is_right_open());
			}
		}
		else {
			//Not implemented empty set
		}
	}

	std::string Interval::__str__() const {
	    std::ostringstream o;
	    if(left_open_)
	    	o<<"(";
	    else
	    	o<<"[";
	    o<<*start_<<", "<<*end_;
	    if(right_open_)
	    	o<<")";
	    else
	    	o<<"]";
	    return o.str();
	}

}