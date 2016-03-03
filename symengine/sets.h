/**
 *  \file sets.h
 *  Polynomial Manipulation
 *
 **/
#ifndef SYMENGINE_SETS_H
#define SYMENGINE_SETS_H

#include <symengine/basic.h>
#include <symengine/functions.h>
#include <symengine/number.h>
#include <symengine/integer.h>
#include <symengine/rational.h>


namespace SymEngine {

class Interval : public Basic  {
	public:
		mpq_class start_;
		mpq_class end_;
		bool left_open_, right_open_;
	public:
		IMPLEMENT_TYPEID(INTERVAL)
    	virtual std::size_t __hash__() const;
	    virtual bool __eq__(const Basic &o) const;
	    virtual int compare(const Basic &o) const;
        virtual vec_basic get_args() const { return {}; }

		Interval(mpq_class real, mpq_class end, bool left_open = false, bool right_open = false);
		
		static RCP<const Interval> from_mpq(const mpq_class start, const mpq_class end, const bool left_open = false, const bool right_open = false);
    	
    	static RCP<const Interval> from_nums(const Basic &start, const Basic &end,  const bool left_open = false, const bool right_open = false);

		RCP<const Interval> open() const;
    	
    	RCP<const Interval> close() const;
    	
    	RCP<const Interval> Lopen() const;
    	
    	RCP<const Interval> Ropen() const;
    	
		bool is_canonical(const mpq_class &start, const mpq_class &end, bool left_open, bool right_open) const;

		RCP<const Interval> interval_union(const Basic &s) const;

		RCP<const Interval> interval_intersection(const Basic &s) const;
	};

//! Canonicalize Interval:
RCP<const Basic> interval(const RCP<const Basic> &arg);

}
#endif