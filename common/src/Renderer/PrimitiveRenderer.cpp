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

#include "PrimitiveRenderer.h"

#include "Renderer/RenderUtils.h"

namespace TrenchBroom {
    namespace Renderer {
        void PrimitiveRenderer::renderLine(const Color& color, const float lineWidth, const Vec3f& start, const Vec3f& end) {
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLine(Vertex(start, color), Vertex(end, color));
            m_lineMeshes[lineWidth].endLines();
        }
        
        void PrimitiveRenderer::renderLines(const Color& color, const float lineWidth, const Vec3f::List& positions) {
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLines(Vertex::fromLists(positions, Color::List(1, color), positions.size(), 0, 1, 0, 0));
            m_lineMeshes[lineWidth].endLines();
        }

        void PrimitiveRenderer::renderCoordinateSystemXY(const Color& x, const Color& y, float lineWidth, const BBox3f& bounds) {
            const Vertex::List vertices = BuildCoordinateSystem::xy(x, y).vertices(bounds);
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLines(vertices);
            m_lineMeshes[lineWidth].endLines();
        }
        
        void PrimitiveRenderer::renderCoordinateSystemXZ(const Color& x, const Color& z, float lineWidth, const BBox3f& bounds) {
            const Vertex::List vertices = BuildCoordinateSystem::xz(x, z).vertices(bounds);
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLines(vertices);
            m_lineMeshes[lineWidth].endLines();
        }
        
        void PrimitiveRenderer::renderCoordinateSystemYZ(const Color& y, const Color& z, float lineWidth, const BBox3f& bounds) {
            const Vertex::List vertices = BuildCoordinateSystem::yz(y, z).vertices(bounds);
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLines(vertices);
            m_lineMeshes[lineWidth].endLines();
        }

        void PrimitiveRenderer::renderCoordinateSystem3D(const Color& x, const Color& y, const Color& z, const float lineWidth, const BBox3f& bounds) {
            const Vertex::List vertices = BuildCoordinateSystem::xyz(x, y, z).vertices(bounds);
            m_lineMeshes[lineWidth].beginLines();
            m_lineMeshes[lineWidth].addLines(vertices);
            m_lineMeshes[lineWidth].endLines();
        }

        void PrimitiveRenderer::renderPolygon(const Color& color, float lineWidth, const Vec3f::List& positions) {
            m_lineMeshes[lineWidth].beginLineLoop();
            m_lineMeshes[lineWidth].addLineLoop(Vertex::fromLists(positions, Color::List(1, color), positions.size(), 0, 1, 0, 0));
            m_lineMeshes[lineWidth].endLineLoop();
        }

        void PrimitiveRenderer::renderFilledPolygon(const Color& color, const Vec3f::List& positions) {
            m_triangleMesh.beginTriangleFan();
            m_triangleMesh.addTriangleFan(Vertex::fromLists(positions, Color::List(1, color), positions.size(), 0, 1, 0, 0));
            m_triangleMesh.endTriangleFan();
        }

        void PrimitiveRenderer::renderCircle(const Color& color, const float lineWidth, const Vec3f& position, const Math::Axis::Type normal, const size_t segments, const float radius, const Vec3f& startAxis, const Vec3f& endAxis) {
            const std::pair<float, float> angles = startAngleAndLength(normal, startAxis, endAxis);
            renderCircle(color, lineWidth, position, normal, segments, radius, angles.first, angles.second);
        }
        
        void PrimitiveRenderer::renderCircle(const Color& color, const float lineWidth, const Vec3f& position, const Math::Axis::Type normal, const size_t segments, const float radius, const float startAngle, const float angleLength) {
            const Vec3f::List positions = circle2D(radius, normal, startAngle, angleLength, segments) + position;
            
            m_lineMeshes[lineWidth].beginLineStrip();
            m_lineMeshes[lineWidth].addLineStrip(Vertex::fromLists(positions, Color::List(1, color), positions.size(), 0, 1, 0, 0));
            m_lineMeshes[lineWidth].endLineStrip();
        }
        
        void PrimitiveRenderer::renderFilledCircle(const Color& color, const Vec3f& position, const Math::Axis::Type normal, const size_t segments, const float radius, const Vec3f& startAxis, const Vec3f& endAxis) {
            const std::pair<float, float> angles = startAngleAndLength(normal, startAxis, endAxis);
            renderFilledCircle(color, position, normal, segments, radius, angles.first, angles.second);
        }
        
        void PrimitiveRenderer::renderFilledCircle(const Color& color, const Vec3f& position, const Math::Axis::Type normal, const size_t segments, const float radius, const float startAngle, const float angleLength) {
            const Vec3f::List positions = circle2D(radius, normal, startAngle, angleLength, segments) + position;

            m_triangleMesh.beginTriangleFan();
            m_triangleMesh.addTriangleFan(Vertex::fromLists(positions, Color::List(1, color), positions.size(), 0, 1, 0, 0));
            m_triangleMesh.endTriangleFan();
        }

        void PrimitiveRenderer::doPrepare(Vbo& vbo) {
            LineMeshMap::iterator it, end;
            for (it = m_lineMeshes.begin(), end = m_lineMeshes.end(); it != end; ++it) {
                const float lineWidth = it->first;
                LineMesh<Vertex::Spec>& mesh = it->second;
                LineMeshRenderer renderer(mesh);
                renderer.prepare(vbo);
                m_lineRenderers[lineWidth] = renderer;
            }
            
            m_triangleRenderer = SimpleTriangleMeshRenderer(m_triangleMesh);
            m_triangleRenderer.prepare(vbo);

            m_lineMeshes.clear();
            m_triangleMesh.clear();
        }
        
        void PrimitiveRenderer::doRender(RenderContext& renderContext) {
            glDisable(GL_DEPTH_TEST);
            renderLines(renderContext);
            renderTriangles(renderContext);
            glEnable(GL_DEPTH_TEST);
        }

        void PrimitiveRenderer::renderLines(RenderContext& renderContext) {
            LineRendererMap::iterator it, end;
            for (it = m_lineRenderers.begin(), end = m_lineRenderers.end(); it != end; ++it) {
                const float lineWidth = it->first;
                LineMeshRenderer& renderer = it->second;
                glLineWidth(lineWidth);
                renderer.render();
            }
            glLineWidth(1.0f);
        }
        
        void PrimitiveRenderer::renderTriangles(RenderContext& renderContext) {
            m_triangleRenderer.render();
        }
    }
}