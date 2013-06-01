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

#ifndef __TrenchBroom__Face__
#define __TrenchBroom__Face__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "StringUtils.h"
#include "Model/BrushFaceTypes.h"

#include <vector>

namespace TrenchBroom {
    namespace Model {
        class BrushFace {
        public:
            static const String NoTextureName;
        private:
            BrushFacePoints m_points;
            Plane3 m_boundary;
            String m_textureName;
            float m_xOffset;
            float m_yOffset;
            float m_rotation;
            float m_xScale;
            float m_yScale;
            size_t m_lineNumber;
            size_t m_lineCount;
        private:
            BrushFace(const Vec3& point0, const Vec3& point1, const Vec3& point2, const String& textureName);
        public:
            static BrushFacePtr newBrushFace(const Vec3& point0, const Vec3& point1, const Vec3& point2, const String& textureName = NoTextureName);
            
            inline const BrushFacePoints& points() const {
                return m_points;
            }
            
            bool arePointsOnPlane(const Plane3& plane) const;
            
            inline const String& textureName() const {
                return m_textureName;
            }
            
            inline const Plane3& boundary() const {
                return m_boundary;
            }
            
            inline const float xOffset() const {
                return m_xOffset;
            }
            
            inline const float yOffset() const {
                return m_yOffset;
            }
            
            inline const float rotation() const {
                return m_rotation;
            }
            
            inline const float xScale() const {
                return m_xScale;
            }
            
            inline const float yScale() const {
                return m_yScale;
            }
            
            inline void setXOffset(const float xOffset) {
                m_xOffset = xOffset;
            }
            
            inline void setYOffset(const float yOffset) {
                m_yOffset = yOffset;
            }
            
            inline void setRotation(const float rotation) {
                m_rotation = rotation;
            }
            
            inline void setXScale(const float xScale) {
                m_xScale = xScale;
            }
            
            inline void setYScale(const float yScale) {
                m_yScale = yScale;
            }
            
            inline void setFilePosition(const size_t lineNumber, const size_t lineCount) {
                m_lineNumber = lineNumber;
                m_lineCount = lineCount;
            }
        private:
            void setPoints(const Vec3& point0, const Vec3& point1, const Vec3& point2);
        };
    }
}

#endif /* defined(__TrenchBroom__Face__) */