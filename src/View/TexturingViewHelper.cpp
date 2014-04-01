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

#include "TexturingViewHelper.h"

#include "PreferenceManager.h"
#include "Preferences.h"
#include "Hit.h"
#include "Assets/Texture.h"
#include "Model/BrushFace.h"
#include "Model/BrushVertex.h"
#include "Model/TexCoordSystemHelper.h"
#include "Renderer/ShaderManager.h"
#include "Renderer/OrthographicCamera.h"
#include "View/TexturingView.h"

namespace TrenchBroom {
    namespace View {
        TexturingViewHelper::TexturingViewHelper() :
        m_face(NULL),
        m_cameraZoom(1.0f),
        m_subDivisions(1, 1) {}
        
        bool TexturingViewHelper::valid() const {
            return m_face != NULL;
        }
        
        Model::BrushFace* TexturingViewHelper::face() const {
            return m_face;
        }
        
        const Assets::Texture* TexturingViewHelper::texture() const {
            if (!valid())
                return NULL;
            return m_face->texture();
        }
        
        const Vec3& TexturingViewHelper::origin() const {
            assert(valid());
            return m_origin;
        }
        
        const Vec3& TexturingViewHelper::xAxis() const {
            assert(valid());
            return m_xAxis;
        }
        
        const Vec3& TexturingViewHelper::yAxis() const {
            assert(valid());
            return m_yAxis;
        }
        
        const Vec3& TexturingViewHelper::zAxis() const {
            assert(valid());
            return m_zAxis;
        }
        
        BBox3 TexturingViewHelper::computeBounds() const {
            assert(valid());
            
            BBox3 result;
            const Model::BrushVertexList& vertices = m_face->vertices();
            result.min = result.max = transformToFace(vertices[0]->position);
            for (size_t i = 1; i < vertices.size(); ++i)
                result.mergeWith(transformToFace(vertices[i]->position));
            return result;
        }
        
        Vec3 TexturingViewHelper::transformToFace(const Vec3& point) const {
            assert(valid());
            return m_toFaceTransform * point;
        }
        
        Vec3 TexturingViewHelper::transformFromFace(const Vec3& point) const {
            assert(valid());
            return m_fromFaceTransform * point;
        }
        
        Vec2f TexturingViewHelper::textureCoords(const Vec3f& point) const {
            assert(valid());
            return m_face->textureCoords(Vec3(point));
        }
        
        Vec3 TexturingViewHelper::computeTexPoint(const Ray3& ray) const {
            assert(valid());
            
            const Plane3& boundary = m_face->boundary();
            const FloatType facePointDist = boundary.intersectWithRay(ray);
            const Vec3 facePoint = ray.pointAtDistance(facePointDist);
            return transformToTex(facePoint);
        }
        
        Vec3 TexturingViewHelper::transformToTex(const Vec3& worldPoint, const bool withOffset) const {
            assert(valid());
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setScale();
            helper.setTranslate(withOffset);
            helper.setProject();
            return helper.worldToTex(worldPoint);
        }
        
        Vec3::List TexturingViewHelper::transformToTex(const Vec3::List& worldPoints, const bool withOffset) const {
            assert(valid());
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setScale();
            helper.setTranslate(withOffset);
            helper.setProject();
            return helper.worldToTex(worldPoints);
        }
        
        Vec2f computeDistance(const Vec3& position, const Assets::Texture* texture, const Vec2i& subDivisions) {
            const FloatType width  = static_cast<FloatType>(texture->width() / subDivisions.x());
            const FloatType height = static_cast<FloatType>(texture->height() / subDivisions.y());
            const FloatType x = Math::remainder(position.x(), width);
            const FloatType y = Math::remainder(position.y(), height);
            
            return Vec2f(x, y);
        }
        
        Vec2f combineDistances(const Vec2f& r1, const Vec2f& r2) {
            Vec2f result;
            for (size_t i = 0; i < 2; ++i) {
                if (Math::abs(r1[i]) < Math::abs(r2[i]))
                    result[i] = r1[i];
                else
                    result[i] = r2[i];
            }
            return result;
        }
        
        Vec2f snap(const Vec2f& delta, const Vec2f& distance, const float cameraZoom) {
            Vec2f result;
            for (size_t i = 0; i < 2; ++i) {
                if (Math::abs(distance[i]) < 4.0f / cameraZoom)
                    result[i] = delta[i] - distance[i];
                else
                    result[i] = Math::round(delta[i]);
            }
            return result;
        }
        
        Vec2f TexturingViewHelper::snapOffset(const Vec2f& delta) const {
            assert(valid());
            
            if (delta.null())
                return delta;
            
            const Assets::Texture* texture = m_face->texture();
            if (texture == NULL)
                return delta;
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setOverrideTranslate(m_face->offset() - delta); // I don't know why this has to be subracted, but it works :-(
            helper.setScale();
            helper.setProject();
            
            const Model::BrushVertexList& vertices = m_face->vertices();
            Vec2f distance = computeDistance(helper.worldToTex(vertices[0]->position), texture, m_subDivisions);
            
            for (size_t i = 1; i < vertices.size(); ++i)
                distance = combineDistances(distance, computeDistance(helper.worldToTex(vertices[i]->position), texture, m_subDivisions));
            
            return snap(delta, -distance, m_cameraZoom);
        }
        
        Vec2f computeDistance(const Vec3& point, const Vec2f& newHandlePosition) {
            return Vec2f(newHandlePosition.x() - point.x(),
                         newHandlePosition.y() - point.y());
        }
        
        Vec2f TexturingViewHelper::snapHandle(const Vec2f& delta) const {
            assert(valid());
            
            if (delta.null())
                return delta;
            
            const Vec2f newPosition = m_handlePosition + delta;
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setTranslate();
            helper.setScale();
            helper.setProject();
            
            const Model::BrushVertexList& vertices = m_face->vertices();
            Vec2f distance = computeDistance(helper.worldToTex(vertices[0]->position), newPosition);
            
            for (size_t i = 1; i < vertices.size(); ++i)
                distance = combineDistances(distance, computeDistance(helper.worldToTex(vertices[i]->position), newPosition));
            
            const Assets::Texture* texture = m_face->texture();
            if (texture != NULL)
                distance = combineDistances(distance, computeDistance(Vec3(newPosition), texture, m_subDivisions));
            
            return snap(delta, distance, m_cameraZoom);
        }
        
        void TexturingViewHelper::computeScaleHandles(Line3& xHandle, Line3& yHandle) const {
            assert(valid());

            Model::TexCoordSystemHelper helper(m_face);
            helper.setTranslate();
            helper.setScale();
            helper.setProject();

            const Vec3 texHandlePosition(m_handlePosition, 0.0);
            xHandle.point = yHandle.point = helper.texToWorld(texHandlePosition);
            
            const Vec3 xHandlePoint2 = helper.texToWorld(texHandlePosition + Vec3::PosY);
            const Vec3 yHandlePoint2 = helper.texToWorld(texHandlePosition + Vec3::PosX);
            
            xHandle.direction = (xHandlePoint2 - xHandle.point).normalized();
            yHandle.direction = (yHandlePoint2 - yHandle.point).normalized();
        }
        
        void TexturingViewHelper::computeScaleHandleVertices(const Renderer::OrthographicCamera& camera, Vec3& x1, Vec3& x2, Vec3& y1, Vec3& y2) const {
            assert(valid());
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setProject();

            const Vec3::List viewportVertices = helper.worldToTex(camera.viewportVertices());
            const BBox3 viewportBounds(viewportVertices);
            const Vec3& min = viewportBounds.min;
            const Vec3& max = viewportBounds.max;
            
            x1 = Vec3(m_handlePosition.x(), min.y(), 0.0);
            x2 = Vec3(m_handlePosition.x(), max.y(), 0.0);
            
            y1 = Vec3(min.x(), m_handlePosition.y(), 0.0);
            y2 = Vec3(max.x(), m_handlePosition.y(), 0.0);
            
            x1 = helper.texToWorld(x1);
            x2 = helper.texToWorld(x2);
            y1 = helper.texToWorld(y1);
            y2 = helper.texToWorld(y2);
        }
        
        void TexturingViewHelper::computeHLineVertices(const Renderer::OrthographicCamera& camera, const FloatType y, Vec3& v1, Vec3& v2) const {
            Model::TexCoordSystemHelper helper(m_face);
            helper.setTranslate();
            helper.setScale();
            helper.setProject();

            const Vec3::List viewportVertices = helper.worldToTex(camera.viewportVertices());
            const BBox3 viewportBounds(viewportVertices);
            const Vec3& min = viewportBounds.min;
            const Vec3& max = viewportBounds.max;
            
            v1 = Vec3(min.x(), y, 0.0);
            v2 = Vec3(max.x(), y, 0.0);

            v1 = helper.texToWorld(v1);
            v2 = helper.texToWorld(v2);
        }
        
        void TexturingViewHelper::computeVLineVertices(const Renderer::OrthographicCamera& camera, const FloatType x, Vec3& v1, Vec3& v2) const {
            Model::TexCoordSystemHelper helper(m_face);
            helper.setTranslate();
            helper.setScale();
            helper.setProject();
            
            const Vec3::List viewportVertices = helper.worldToTex(camera.viewportVertices());
            const BBox3 viewportBounds(viewportVertices);
            const Vec3& min = viewportBounds.min;
            const Vec3& max = viewportBounds.max;
            
            v1 = Vec3(x, min.y(), 0.0);
            v2 = Vec3(x, max.y(), 0.0);
            
            v1 = helper.texToWorld(v1);
            v2 = helper.texToWorld(v2);
        }
        
        Mat4x4 TexturingViewHelper::worldToTexMatrix() const {
            assert(valid());
            const Vec2f offset(m_face->xOffset(), m_face->yOffset());
            const Vec2f scale(m_face->xScale(), m_face->yScale());
            return Mat4x4::ZerZ * m_face->toTexCoordSystemMatrix(offset, scale);
        }
        
        void TexturingViewHelper::activateTexture(Renderer::ActiveShader& shader) {
            assert(valid());
            Assets::Texture* texture = m_face->texture();
            if (texture != NULL) {
                shader.set("ApplyTexture", true);
                shader.set("Color", texture->averageColor());
                texture->activate();
            } else {
                PreferenceManager& prefs = PreferenceManager::instance();
                shader.set("ApplyTexture", false);
                shader.set("Color", prefs.get(Preferences::FaceColor));
            }
        }
        
        void TexturingViewHelper::deactivateTexture() {
            assert(valid());
            Assets::Texture* texture = m_face->texture();
            if (texture != NULL)
                texture->deactivate();
        }
        
        Hits TexturingViewHelper::pick(const Ray3& pickRay) const {
            assert(valid());
            
            Hits hits;
            const FloatType distance = m_face->intersectWithRay(pickRay);
            if (!Math::isnan(distance)) {
                const Vec3 hitPoint = pickRay.pointAtDistance(distance);
                hits.addHit(Hit(TexturingView::FaceHit, distance, hitPoint, m_face));
            }
            return hits;
        }
        
        void TexturingViewHelper::setFace(Model::BrushFace* face) {
            if (face != m_face) {
                m_face = face;
                if (m_face != NULL) {
                    validate();
                    resetHandlePosition();
                }
            }
        }
        
        void TexturingViewHelper::faceDidChange() {
            if (m_face != NULL)
                validate();
        }
        
        void TexturingViewHelper::setCameraZoom(const float cameraZoom) {
            m_cameraZoom = cameraZoom;
        }
        
        const Vec2i& TexturingViewHelper::subDivisions() const {
            return m_subDivisions;
        }
        
        void TexturingViewHelper::setSubDivisions(const Vec2i& subDivisions) {
            m_subDivisions = subDivisions;
        }
        
        const Vec2& TexturingViewHelper::handlePosition() const {
            return m_handlePosition;
        }
        
        void TexturingViewHelper::setHandlePosition(const Vec2f& handlePosition) {
            m_handlePosition = handlePosition;
        }
        
        void TexturingViewHelper::validate() {
            assert(m_face != NULL);
            
            m_origin = m_face->center();
            m_zAxis = m_face->boundary().normal;
            
            if (Math::lt(Math::abs(Vec3::PosZ.dot(m_zAxis)), 1.0))
                m_xAxis = crossed(Vec3::PosZ, m_zAxis).normalized();
            else
                m_xAxis = Vec3::PosX;
            m_yAxis = crossed(m_zAxis, m_xAxis).normalized();
            
            m_fromFaceTransform = coordinateSystemMatrix(m_xAxis, m_yAxis, m_zAxis, m_origin);
            bool invertible = true;
            m_toFaceTransform = invertedMatrix(m_fromFaceTransform, invertible);
            assert(invertible);
        }
        
        void TexturingViewHelper::resetHandlePosition() {
            assert(m_face != NULL);
            const Model::BrushVertexList& vertices = m_face->vertices();
            const size_t vertexCount = vertices.size();
            
            Vec3::List positions(vertexCount);
            for (size_t i = 0; i < vertexCount; ++i)
                positions[i] = vertices[i]->position;
            
            Model::TexCoordSystemHelper helper(m_face);
            helper.setProject();

            const BBox3 bounds(helper.worldToTex(positions));
            m_handlePosition = Vec2(bounds.min);
        }
    }
}
