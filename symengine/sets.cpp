#include <symengine/sets.h>
#include <symengine/integer.h>

namespace SymEngine {

	Interval::Interval(mpq_class start, mpq_class end, bool left_open, bool right_open)
		: start_(start), end_(end), left_open_(left_open), right_open_(right_open) {
			SYMENGINE_ASSERT(is_canonical(start_, end_, left_open_, right_open_));
		}

	bool Interval::is_canonical(const mpq_class &start, const mpq_class &end, bool left_open, bool right_open) const {
		if(end < start) {
			throw std::runtime_error("Empty set not implemented yet");
		}
		else if(end == start) {
			if(left_open || right_open)
				throw std::runtime_error("Empty set not implemented yet");
		}
		return true;

	}

	void Interval::interval_union(const Interval &second, Interval &result) {

	}

	std::size_t Interval::__hash__() const
	{
	    // only the least significant bits that fit into "signed long int" are
	    // hashed:
	    std::size_t seed = COMPLEX;
	    hash_combine<long long int>(seed, this->start_.get_num().get_si());
	    hash_combine<long long int>(seed, this->start_.get_den().get_si());
	    hash_combine<long long int>(seed, this->end_.get_num().get_si());
	    hash_combine<long long int>(seed, this->end_.get_den().get_si());
	    hash_combine<bool>(seed, left_open_);
	    hash_combine<bool>(seed, right_open_);
	    return seed;
	}

	bool Interval::__eq__(const Basic &o) const
	{
	    if (is_a<Interval>(o)) {
	        const Interval &s = static_cast<const Interval &>(o);
	        return (((this->start_ == s.start_) and (this->end_ == s.end_)) and
	        		((this->left_open_ == s.left_open_) and (this->right_open_ == s.right_open_)));
	    }
	    return false;
	}

	int Interval::compare(const Basic &o) const {
	    SYMENGINE_ASSERT(is_a<Interval>(o))
	    const Interval &s = static_cast<const Interval &>(o);
	    if ((start_ == s.start_ && end_ == s.end_) && (left_open_ == s.left_open_ && right_open_ == s.right_open_))
	        return 0;
	    else
	    	throw std::runtime_error("Not implemented");
	}

	RCP<const Basic> Interval::from_mpq(const mpq_class start, const mpq_class end, const bool left_open, const bool right_open)
	{
	    return make_rcp<const Interval>(start, end, left_open, right_open);
	}

	RCP<const Basic> Interval::from_nums(const Basic &start, const Basic &end, const bool left_open, const bool right_open)
	{
	    if (is_a<Integer>(start) and is_a<Integer>(end)) {
	        mpq_class start_mpq(static_cast<const Integer&>(start).i, static_cast<const Integer&>(*one).i);
	        mpq_class end_mpq(static_cast<const Integer&>(end).i, static_cast<const Integer&>(*one).i);
	        return Interval::from_mpq(start_mpq, end_mpq);
	    } else if (is_a<Rational>(start) and is_a<Integer>(end)) {
	        mpq_class start_mpq = static_cast<const Rational&>(start).i;
	        mpq_class end_mpq(static_cast<const Integer&>(end).i, static_cast<const Integer&>(*one).i);
	        return Interval::from_mpq(start_mpq, end_mpq);
	    } else if (is_a<Integer>(start) and is_a<Rational>(end)) {
	        mpq_class start_mpq(static_cast<const Integer&>(start).i, static_cast<const Integer&>(*one).i);
	        mpq_class end_mpq = static_cast<const Rational&>(end).i;
	        return Interval::from_mpq(start_mpq, end_mpq);
	    } else if (is_a<Rational>(start) and is_a<Rational>(end)) {
	        mpq_class start_mpq = static_cast<const Rational&>(start).i;
	        mpq_class end_mpq = static_cast<const Rational&>(end).i;
	        return Interval::from_mpq(start_mpq, end_mpq);
	    } else {
	        throw std::runtime_error("Invalid Format: Expected Integer or Rational");
	    }
	
	}

	RCP<const Basic> open(const Basic &interval) {
		SYMENGINE_ASSERT(is_a<Interval>(interval))
		const Interval &i = static_cast<const Interval&>(interval);
		return Interval::from_mpq(i.start_, i.end_, true, true);
	}

	RCP<const Basic> Lopen(const Basic &interval) {
		SYMENGINE_ASSERT(is_a<Interval>(interval))
		const Interval &i = static_cast<const Interval&>(interval);
		return Interval::from_mpq(i.start_, i.end_, true, false);
	}

	RCP<const Basic> Ropen(const Basic &interval) {
		SYMENGINE_ASSERT(is_a<Interval>(interval))
		const Interval &i = static_cast<const Interval&>(interval);
		return Interval::from_mpq(i.start_, i.end_, false, true);
	}

	RCP<const Basic> close(const Basic &interval) {
		SYMENGINE_ASSERT(is_a<Interval>(interval))
		const Interval &i = static_cast<const Interval&>(interval);
		return Interval::from_mpq(i.start_, i.end_, false, false);
	}

	RCP<const Basic> Interval::interval_intersection(const Basic &f,const Basic &s) {
		SYMENGINE_ASSERT(is_a<Interval>(f))
		SYMENGINE_ASSERT(is_a<Interval>(s))
		mpq_class start, end;
		bool left_open, right_open;
		const Interval &first = static_cast<const Interval&>(f);
		const Interval &second = static_cast<const Interval&>(s);
		if((first.start_<=second.end_) && (first.end_ >= second.start_)) {
			if(first.start_ < second.start_) {
				start = second.start_;
				left_open = second.left_open_;
			}
			else if(first.start_ > second.start_) {
				start = first.start_;
				left_open = first.left_open_;
			}
			else {
				start = first.start_;
				left_open = first.left_open_ || second.left_open_ ;
			}

			if(first.end_ < second.end_) {
				end = first.end_;
				right_open = first.right_open_;
			}
			else if(first.end_ > second.end_) {
				end = second.end_;
				right_open = second.right_open_;
			}
			else {
				end = first.end_;
				right_open = first.right_open_ || second.right_open_;
			}
			return Interval::from_mpq(start, end, left_open, right_open);
		}
		else {
			throw std::runtime_error("Empty set not implemented yet");
		}
	}

	

}