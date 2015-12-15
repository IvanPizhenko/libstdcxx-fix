/********************************************************************************
 * 
 * Fixes for the libstdc++ shipped with GCC 4.8
 * 
 * Copyright (c) 2015 Ivan Pizhenko <ivan [dot] pizhenko [at] gmail [dot] com>
 * Published under GPL v3 license with GCC Runtime Library Exception 
 * (Section 7 of GPL version 3).
 * 
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 ********************************************************************************
 * 
 * This code is bases on the code licensed under following conditions:
 * 
 * Copyright (C) 2007-2015 Free Software Foundation, Inc.
 * 
 * This file is part of the GNU ISO C++ Library.  This library is free
 * software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * Under Section 7 of GPL version 3, you are granted additional
 * permissions described in the GCC Runtime Library Exception, version
 * 3.1, as published by the Free Software Foundation.
 * 
 * You should have received a copy of the GNU General Public License and
 * a copy of the GCC Runtime Library Exception along with this program;
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 ********************************************************************************/


#if defined(__GNUC__) || defined(_MSC_VER)
#pragma once 
#endif


#ifndef __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_IOMANIP_FIX_H__
#define __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_IOMANIP_FIX_H__

#include <iomanip>

#if defined (__GNUG__)

#if __cplusplus >= 201103L

#include <istream>
#include <ostream>
#include <iterator>
#include <locale>
#include <stdexcept>
#include <utility>
#include <memory>
#include <ctime>


#if GCC_VERSION < 50100

namespace libstdcxx_locale_fix {
	
template<class CharT, class InputIt = std::istreambuf_iterator<CharT> >
class time_get_fix: public std::time_get<CharT, InputIt>
{
public:
	using typename std::time_get<CharT, InputIt>::char_type;
	using typename std::time_get<CharT, InputIt>::iter_type;
	
	using std::time_get<CharT, InputIt>::time_get; 
	virtual ~time_get_fix() {}
	
	iter_type get(iter_type beg, iter_type end, std::ios_base& str, std::ios_base::iostate& err, std::tm* t,
			   const char_type* fmtbeg, const char_type* fmtend) const;
	
	iter_type get(iter_type beg, iter_type end, std::ios_base& str, std::ios_base::iostate& err, std::tm* t,
				  char_type format, char_type modifier) const
	{
		return this->do_get(beg, end, str, err, t, format, modifier);
	}
	
protected:
	using std::time_get<CharT, InputIt>::_M_extract_via_format;
	
	virtual iter_type do_get(iter_type beg, iter_type end, std::ios_base& str, std::ios_base::iostate& err, std::tm* t,
				  char_type format, char_type modifier) const;
	
};


template<class CharT, class InputIt>
typename time_get_fix<CharT, InputIt>::iter_type 
time_get_fix<CharT, InputIt>::get(
	iter_type beg, iter_type end, std::ios_base& str, std::ios_base::iostate& err, std::tm* t,
	const char_type* fmtbeg, const char_type* fmtend) const
{
	const std::locale& loc = str.getloc();
	typedef std::ctype<CharT> ct_type;
	const ct_type & ct = std::use_facet<ct_type>(loc);
	err = std::ios_base::goodbit;
	
	while (fmtbeg != fmtend && err == std::ios_base::goodbit)
	{
		if (beg == end)
		{
			err = std::ios_base::eofbit | std::ios_base::failbit;
			break;
		}
		else if (ct.narrow(*fmtbeg, 0) == '%')
		{
			char format;
			char mod = 0;
			
			if (++fmtbeg == fmtend)
			{
				err = std::ios_base::failbit;
				break;
			}
			
			const char c = ct.narrow(*fmtbeg, 0);
			
			if (c != 'E' && c != 'O')
				format = c;
			else if (++fmtbeg != fmtend)
			{
				mod = c;
				format = ct.narrow(*fmtbeg, 0);
			}
			else
			{
				err = std::ios_base::failbit;
				break;
			}
			
			beg = this->do_get(beg, end, str, err, t, format, mod);
			++fmtbeg;
		}
		else if (ct.is(ct_type::space, *fmtbeg))
		{
			++fmtbeg;
			while (fmtbeg != fmtend && ct.is(ct_type::space, *fmtbeg))
				++fmtbeg;
			
			while (beg != end && ct.is(ct_type::space, *beg))
				++beg;
		}
		// TODO real case-insensitive comparison
		else if (ct.tolower(*beg) == ct.tolower(*fmtbeg) ||
			ct.toupper(*beg) == ct.toupper(*fmtbeg))
		{
			++beg;
			++fmtbeg;
		}
		else
		{
			err = std::ios_base::failbit;
			break;
		}
	}
	
	return beg;
}
	

template<class CharT, class InputIt>
typename time_get_fix<CharT, InputIt>::iter_type 
time_get_fix<CharT, InputIt>::do_get(
	iter_type beg, iter_type end, std::ios_base& str, 
	std::ios_base::iostate& err, std::tm* t, char_type format, char_type modifier) const
{
	const std::locale& loc = str.getloc();
	typedef std::ctype<CharT> ct_type;
	const ct_type & ct = std::use_facet<ct_type>(loc);
	err = std::ios_base::goodbit;
	
	char_type fmt[4];

	fmt[0] = ct.widen('%');
	if (!modifier)
	{
		fmt[1] = format;
		fmt[2] = char_type();
	}
	else
	{
		fmt[1] = modifier;
		fmt[2] = format;
		fmt[3] = char_type();
	}
	
	beg = _M_extract_via_format(beg, end, str, err, t, fmt);
	
	if (beg == end)
		err |= std::ios_base::eofbit;
	
	return beg;
}

} // namespace libstdcxx_locale_fix


namespace std {
	
	using libstdcxx_locale_fix::time_get_fix;
	
	template<class CharT>
	class get_time
	{
	public:
		get_time(struct tm* tmb, const CharT* format) : m_data(tmb, format) {}
		struct tm* get_tmb() { return m_data.first; }
		const CharT* get_format() const { return m_data.second; }
	protected:
		pair<std::tm*, const CharT*> m_data;
	};
	
	template< class CharT, class Traits = char_traits<CharT> >
	basic_istream<CharT, Traits>& operator>>(basic_istream<CharT>& is, get_time<CharT>&& t)
	{
		typedef istreambuf_iterator<CharT, Traits> Iter;
		typedef time_get_fix<CharT, Iter> TimeGet;
		typename basic_istream<CharT, Traits>::sentry s(is);
		if(s)
		{
                        locale loc(is.getloc(), new TimeGet());
			const TimeGet& tg = use_facet<TimeGet>(loc);
                        ios_base::iostate err = ios_base::goodbit;
			tg.get(Iter(is.rdbuf()), Iter(), is, err, t.get_tmb(), 
			   t.get_format(), t.get_format() + Traits::length(t.get_format()));
			if (err != std::ios_base::goodbit)
			is.setstate(err);
		}
		return is;
	}

	template<class CharT>
	class put_time
	{
	public:
		put_time(const struct tm* tmb, const CharT* format) : m_data(tmb, format) {}
		const struct tm* get_tmb() const { return m_data.first; }
		const CharT* get_format() const { return m_data.second; }
	protected:
		pair<const std::tm*, const CharT*> m_data;
	};
	
	template< class CharT, class Traits = char_traits<CharT> >
	basic_ostream<CharT>& operator<<(basic_ostream<CharT>& os, const put_time<CharT>& t)
	{
		typedef ostreambuf_iterator<CharT, Traits> Iter;
		typedef time_put<CharT, Iter> TimePut;
		typename basic_ostream<CharT, Traits>::sentry s(os);
		if(s)
		{
			const TimePut& tp = use_facet<TimePut>(os.getloc());
			const Iter end = tp.put(Iter(os.rdbuf()), os, os.fill(), t.get_tmb(), 
								t.get_format(), t.get_format() + Traits::length(t.get_format()));
			if (end.failed())
				os.setstate(std::ios_base::badbit);
		}
		return os;
	}

} // namespace std

#endif // GCC_VERSION < 50100


#endif // __cplusplus >= 201103L

#endif // defined(__GNUG__)

#endif // __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_IOMANIP_FIX_H__
