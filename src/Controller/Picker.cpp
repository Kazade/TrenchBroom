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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Picker.h"

#include "Model/Filter.h"

#include <algorithm>

namespace TrenchBroom {
    namespace Controller {
        Hit::HitType Hit::freeHitType() {
            static HitType currentType = 1;
            const HitType result = currentType;
            currentType = currentType << 1;
            return result;
        }
        
        const Hit Hit::NoHit = Hit(NoType, 0.0f, Vec3f::Null, false);

        Hit::HitType Hit::type() const {
            return m_type;
        }
        
        bool Hit::hasType(const HitType typeMask) const {
            return (m_type & typeMask) != 0;
        }

        float Hit::distance() const {
            return m_distance;
        }
        
        const Vec3f& Hit::hitPoint() const {
            return m_hitPoint;
        }

        PickResult::FirstHit::FirstHit(const bool i_matches, const Hit& i_hit) :
        matches(i_matches),
        hit(i_hit) {}

        bool PickResult::CompareHits::operator() (const Hit& left, const Hit& right) const {
            return left.distance() < right.distance();
        }
        
        PickResult::FirstHit PickResult::firstHit(HitFilter& filter, const bool ignoreOccluders) const {
            if (!m_hits.empty()) {
                Hit::List::const_iterator it = m_hits.begin();
                Hit::List::const_iterator end = m_hits.end();
                if (!ignoreOccluders) {
                    const Hit& first = *it;
                    if (filter.matches(first))
                        return FirstHit(true, first);
                    
                    const float closest = (it++)->distance();
                    while (it != end) {
                        const Hit& hit = *it;
                        if (hit.distance() > closest)
                            break;
                        if (filter.matches(hit))
                            return FirstHit(true, hit);
                        ++it;
                    }
                } else {
                    while (it != end) {
                        const Hit& hit = *it;
                        if (filter.matches(hit))
                            return FirstHit(true, hit);
                        ++it;
                    }
                }
            }
            return FirstHit(false, Hit::NoHit);
        }
        
        Hit::List PickResult::hits(HitFilter& filter) const {
            Hit::List result;
            Hit::List::const_iterator it, end;
            for (it = m_hits.begin(), end = m_hits.end(); it != end; ++it) {
                const Hit& hit = *it;
                if (filter.matches(hit))
                    result.push_back(hit);
            }
            return result;
        }
        
        Hit::List PickResult::allHits() const {
            return m_hits;
        }

        void PickResult::addHit(const Hit& hit) {
            m_hits.push_back(hit);
        }
        
        void PickResult::sortHits() {
            std::sort(m_hits.begin(), m_hits.end(), CompareHits());
        }

        PickResult Picker::pick(const Ray3& ray) {
            PickResult result;
            const Pickable::List candidates = m_octree.findObjects(ray);
            Pickable::List::const_iterator it, end;
            for (it = candidates.begin(), end = candidates.end(); it != end; ++it) {
                Pickable::Ptr object = *it;
                object->pick(ray, result);
            }
            result.sortHits();
            return result;
        }
    }
}
