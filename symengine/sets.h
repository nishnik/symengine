/**
 *  \file sets.h
 *  Polynomial Manipulation
 *
 **/
#ifndef SYMENGINE_SETS_H
#define SYMENGINE_SETS_H

#include <symengine/basic.h>
#include <symengine/functions.h>


namespace SymEngine {

class Interval {
	private:
		RCP<const Basic> start_, end_;
		bool left_open_, right_open_;
	public:
		//! \return Size of the hash
    	//virtual std::size_t __hash__() const;

	    /*! Equality comparator
	     * \param o - Object to be compared with
	     * \return whether the 2 objects are equal
	     * */
	    //virtual bool __eq__(const Basic &o) const;
	    //virtual int compare(const Basic &o) const;

		Interval(const RCP<const Basic> &start, const RCP<const Basic> &end, bool left_open = false, bool right_open = false);
		
		Interval();

		std::string __str__() const;

		bool is_canonical();

		RCP<const Basic> get_start() const { return start_; }

		RCP<const Basic> get_end() const { return end_; }

		bool is_left_open() const { return left_open_; }

		bool is_right_open() const { return right_open_; }

		void set_start(const RCP<const Basic> &start) { start_ = start;}

		void set_end(const RCP<const Basic> &end) { end_ = end;}

		void set_left_open(bool left_open) { left_open_ = left_open; }

		void set_right_open(bool right_open) { right_open_ = right_open; }

		void interval_union(const Interval &other, Interval &result);

		void interval_intersection(const Interval &other, Interval &result);
	};

}

inline std::ostream& operator<<(std::ostream& out, const SymEngine::Interval& A)
{
    return out << A.__str__();
}
#endif