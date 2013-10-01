/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_NestedIterator_h
#define TrenchBroom_NestedIterator_h

#include <iterator>

namespace TrenchBroom {
    template <typename OuterIterator, class InnerAdapter>
    class NestedIterator {
    private:
        typedef typename InnerAdapter::InnerIterator InnerIterator;
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef typename InnerIterator::value_type value_type;
        typedef typename InnerIterator::difference_type difference_type;
        typedef typename InnerIterator::pointer pointer;
        typedef typename InnerIterator::reference reference;
    private:
        InnerAdapter m_adapter;
        OuterIterator m_outerCur;
        OuterIterator m_outerEnd;
        InnerIterator m_innerCur;
        InnerIterator m_innerEnd;
    public:
        NestedIterator() {}
        
        NestedIterator(const OuterIterator& outerCur) :
        m_outerCur(outerCur),
        m_outerEnd(m_outerCur) {
            advancePastEmptyInner();
        }
        
        NestedIterator(const OuterIterator& outerCur, const OuterIterator& outerEnd) :
        m_outerCur(outerCur),
        m_outerEnd(outerEnd) {
            advancePastEmptyInner();
        }
        
        reference operator*()  const {
            return *m_innerCur;
        }
        
        pointer operator->() const {
            return &*m_innerCur;
        }
        
        // pre-increment
        NestedIterator& operator++() {
            ++m_innerCur;
            if (m_innerCur == m_innerEnd)
                advanceOuter();
            return *this;
        }
        
        // post-increment
        NestedIterator operator++(int) {
            NestedIterator<OuterIterator, InnerIterator> result(*this);
            ++*this;
            return result;
        }
    
        bool operator==(const NestedIterator<OuterIterator, InnerAdapter>& other) const {
            if (m_outerCur != other.m_outerCur)
                return false;
            if (m_outerCur != m_outerEnd &&
                other.m_outerCur != other.m_outerEnd &&
                m_innerCur != other.m_innerCur)
                return false;
            return true;
        }

        bool operator!=(const NestedIterator<OuterIterator, InnerAdapter>& other) const {
            return !(*this == other);
        }
    private:
        void advanceOuter() {
            ++m_outerCur;
            advancePastEmptyInner();
        }
        
        void advancePastEmptyInner() {
            if (m_outerCur == m_outerEnd)
                return;
            m_innerCur = m_adapter.beginInner(m_outerCur);
            m_innerEnd = m_adapter.endInner(m_outerCur);
            if (m_innerCur == m_innerEnd)
                advanceOuter();
        }
    };
}

#endif