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

#ifndef __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_MEMORY_FIX_H__
#define __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_MEMORY_FIX_H__

#include <memory>

#if defined (__GNUG__)

#if __cplusplus >= 201103L


#if GCC_VERSION < 40900

namespace std {
	
	template<typename T, typename... Args>
	inline unique_ptr<T> make_unique(Args&&... args) 
	{
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}
	
} // namespace std

#endif // GCC_VERSION < 40900

#endif // __cplusplus >= 201103L

#endif // defined(__GNUG__)

#endif // __INCLUDED_LIBSTDCXXFIX_LIBSTDCXX_MEMORY_FIX_H__
