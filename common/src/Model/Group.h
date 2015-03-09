/*
 Copyright (C) 2010-2014 Kristian Duske
 
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

#ifndef __TrenchBroom__Group__
#define __TrenchBroom__Group__

#include "TrenchBroom.h"
#include "VecMath.h"
#include "StringUtils.h"
#include "Hit.h"
#include "Model/ModelTypes.h"
#include "Model/Node.h"
#include "Model/Object.h"

namespace TrenchBroom {
    namespace Model {
        class PickResult;
        
        class Group : public Node, public Object {
        public:
            static const Hit::HitType GroupHit;
        private:
            String m_name;
            mutable BBox3 m_bounds;
            mutable bool m_boundsValid;
        public:
            Group(const String& name);
            
            void setName(const String& name);
        private: // implement methods inherited from Node
            const String& doGetName() const;
            
            Node* doClone(const BBox3& worldBounds) const;
            NodeSnapshot* doTakeSnapshot();

            bool doCanAddChild(const Node* child) const;
            bool doCanRemoveChild(const Node* child) const;
            bool doRemoveIfEmpty() const;

            void doDescendantWasAdded(Node* node);
            void doDescendantWasRemoved(Node* oldParent, Node* node);
            void doDescendantDidChange(Node* node);
            
            bool doSelectable() const;
            
            void doGenerateIssues(const IssueGenerator* generator, IssueList& issues);
            void doAccept(NodeVisitor& visitor);
            void doAccept(ConstNodeVisitor& visitor) const;
        private: // implement methods inherited from Object
            const BBox3& doGetBounds() const;
            void doPick(const Ray3& ray, PickResult& pickResult) const;
            FloatType doIntersectWithRay(const Ray3& ray) const;

            Node* doGetContainer() const;
            Layer* doGetLayer() const;
            Group* doGetGroup() const;
            
            void doTransform(const Mat4x4& transformation, bool lockTextures, const BBox3& worldBounds);
            bool doContains(const Node* node) const;
            bool doIntersects(const Node* node) const;
        private:
            void invalidateBounds();
            void validateBounds() const;
        private:
            Group(const Group&);
            Group& operator=(const Group&);
        };
    }
}

#endif /* defined(__TrenchBroom__Group__) */
