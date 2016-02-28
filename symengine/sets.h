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
	
		IMPLEMENT_TYPEID(INTERVAL)
		//! \return Size of the hash
    	virtual std::size_t __hash__() const;

	    /*! Equality comparator
	     * \param o - Object to be compared with
	     * \return whether the 2 objects are equal
	     * */
	    virtual bool __eq__(const Basic &o) const;
	    virtual int compare(const Basic &o) const;
        virtual vec_basic get_args() const { return {}; }


		Interval(mpq_class real, mpq_class end, bool left_open = false, bool right_open = false);
		
		static RCP<const Basic> from_mpq(const mpq_class start, const mpq_class end, const bool left_open = false, const bool right_open = false);
    	
    	static RCP<const Basic> from_nums(const Basic &start, const Basic &end,  const bool left_open = false, const bool right_open = false);

		static RCP<const Basic> open(const Basic &interval);
    	
    	static RCP<const Basic> close(const Basic &interval);
    	
    	static RCP<const Basic> Lopen(const Basic &interval);
    	
    	static RCP<const Basic> Ropen(const Basic &interval);
    	
		std::string __str__() const;

		bool is_canonical(const mpq_class &start, const mpq_class &end, bool left_open, bool right_open) const;

		void interval_union(const Interval &other, Interval &result);

		static RCP<const Basic> interval_intersection(const Basic &f, const Basic &s);
	};

}
/*
inline std::ostream& operator<<(std::ostream& out, const SymEngine::Interval& A)
{
    return out << A.__str__();
}*/
#endif