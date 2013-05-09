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

#include <gtest/gtest.h>

#include "Vec.h"

TEST(VecTest, ConstructVec3fFromValidString) {
    ASSERT_EQ(Vec3f(1.0f, 3.0f, 3.5f), Vec3f("1.0 3 3.5"));
}

TEST(VecTest, ConstructVec3fFromShortString) {
    ASSERT_EQ(Vec3f(1.0f, 3.0f, 0.0f), Vec3f("1.0 3"));
}

TEST(VecTest, ConstructVec3fFromInvalidString) {
    ASSERT_EQ(Vec3f::Null, Vec3f("asdf"));
}

TEST(VecTest, ConstructVec3fFrom1Float) {
    ASSERT_EQ(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(1.0f));
}

TEST(VecTest, ConstructVec3fFrom2Floats) {
    ASSERT_EQ(Vec3f(1.0f, 2.0f, 0.0f), Vec3f(1.0f, 2.0f));
}

TEST(VecTest, ConstructVec3fFrom4Floats) {
    ASSERT_EQ(Vec3f(1.0f, 2.0f, 3.0f), Vec3f(1.0f, 2.0f, 3.0f, 4.0f));
}

TEST(VecTest, ConstructVec4fFrom3Floats) {
    ASSERT_EQ(Vec4f(1.0f, 2.0f, 3.0f, 0.0f), Vec4f(1.0f, 2.0f, 3.0f));
}

TEST(VecTest, ConstructVec2fFromVec2f) {
    const Vec2f v(2.0f, 3.0f);
    ASSERT_EQ(v, Vec2f(v));
}

TEST(VecTest, ConstructVec2fFromVec3f) {
    const Vec3f v(3.0f, 5.0f, 78.0f);
    ASSERT_EQ(Vec2f(v[0], v[1]), Vec2f(v));
}

TEST(VecTest, ConstructVec2fFromVec4f) {
    const Vec4f v(3.0f, 5.0f, 2.0f, 7.0f);
    ASSERT_EQ(Vec2f(v[0], v[1]), Vec2f(v));
}

TEST(VecTest, ConstructVec3fFromVec2f) {
    const Vec2f v(2.0f, 3.0f);
    ASSERT_EQ(Vec3f(v[0], v[1], 0.0f), Vec3f(v));
}

TEST(VecTest, ConstructVec4fFromVec2f) {
    const Vec2f v(2.0f, 3.0f);
    ASSERT_EQ(Vec4f(v[0], v[1], 0.0f, 0.0f), Vec4f(v));
}

TEST(VecTest, ConstructVec4fFromVec2fWithLast1) {
    const Vec2f v(3.0f, 5.0f);
    ASSERT_EQ(Vec4f(v[0], v[1], 0.0f, 2.0f), Vec4f(v, 2.0f));
}

TEST(VecTest, ConstructVec4fFromVec2fWithLast2) {
    const Vec2f v(3.0f, 5.0f);
    ASSERT_EQ(Vec4f(v[0], v[1], 3.0f, 2.0f), Vec4f(v, 3.0f, 2.0f));
}

TEST(VecTest, ConstructVec3fFromVec3fWithLast1) {
    const Vec3f v(3.0f, 5.0f, 8.0f);
    ASSERT_EQ(Vec3f(v[0], v[1], 2.0f), Vec3f(v, 2.0f));
}

TEST(VecTest, ConstructVec3fFromVec3fWithLast2) {
    const Vec3f v(3.0f, 5.0f, 8.0f);
    ASSERT_EQ(Vec3f(v[0], 2.0f, 4.0f), Vec3f(v, 2.0f, 4.0f));
}

TEST(VecTest, AssignVec2fToVec3f) {
    const Vec2f t(2.0f, 3.0f);
    Vec3f v;
    ASSERT_EQ(Vec3f(t), (v = t));
}

TEST(VecTest, AssignVec3fToVec3f) {
    const Vec3f t(2.0f, 3.0f, 5.0f);
    Vec3f v;
    ASSERT_EQ(Vec3f(t), (v = t));
}

TEST(VecTest, AssignVec4fToVec3f) {
    const Vec4f t(2.0f, 3.0f, 5.0f, 6.0f);
    Vec3f v;
    ASSERT_EQ(Vec3f(t), (v = t));
}

TEST(VecTest, InvertVec3f) {
    ASSERT_EQ( Vec3f(-1.0f, -2.0f, -3.0f),
              -Vec3f( 1.0f,  2.0f,  3.0f));
}

TEST(VecTest, AddVec3f) {
    ASSERT_EQ(Vec3f(4.0f, 4.0f, 4.0f),
              Vec3f(1.0f, 2.0f, 3.0f) +
              Vec3f(3.0f, 2.0f, 1.0f));
}

TEST(VecTest, SubtractVec3f) {
    ASSERT_EQ(Vec3f(1.0f, 1.0f, -1.0f),
              Vec3f(2.0f, 3.0f, 1.0f) -
              Vec3f(1.0f, 2.0f, 2.0f));
}

TEST(VecTest, MultiplyVec3fWithScalar) {
    ASSERT_EQ(Vec3f(6.0f, 9.0f, 3.0f),
              Vec3f(2.0f, 3.0f, 1.0f) * 3.0f);
}

TEST(VecTest, DivideVec3fByScalar) {
    ASSERT_EQ(Vec3f(1.0f, 18.0f, 2.0f),
              Vec3f(2.0f, 36.0f, 4.0f) / 2.0f);
}

TEST(VecTest, AddVec3fAndAssign) {
    Vec3f v(1.0f, 2.0f, 3.0f);
    ASSERT_EQ(Vec3f(4.0f, 4.0f, 4.0f), (v += Vec3f(3.0f, 2.0f, 1.0f)));
}

TEST(VecTest, SubstractVec3fAndAssign) {
    Vec3f v(2.0f, 3.0f, 1.0f);
    ASSERT_EQ(Vec3f(1.0f, 1.0f, -1.0f), (v -= Vec3f(1.0f, 2.0f, 2.0f)));
}

TEST(VecTest, MultiplyVec3fWithScalarAndAssign) {
    Vec3f v(2.0f, 3.0f, 1.0f);
    ASSERT_EQ(Vec3f(6.0f, 9.0f, 3.0f), (v *= 3.0f));
}

TEST(VecTest, DivideVec3fByScalarAndAssign) {
    Vec3f v(2.0f, 36.0f, 4.0f);
    ASSERT_EQ(Vec3f(1.0f, 18.0f, 2.0f), (v /= 2.0f));
}

TEST(VecTest, SubscriptAccess) {
    Vec4f v(1.0f, 2.0f, 3.0f, 4.0f);
    ASSERT_EQ(1.0f, v[0]);
    ASSERT_EQ(2.0f, v[1]);
    ASSERT_EQ(3.0f, v[2]);
    ASSERT_EQ(4.0f, v[3]);
}

TEST(VecTest, Accessors) {
    Vec4f v(1.0f, 2.0f, 3.0f, 4.0f);
    ASSERT_EQ(v[0], v.x());
    ASSERT_EQ(v[1], v.y());
    ASSERT_EQ(v[2], v.z());
    ASSERT_EQ(v[3], v.w());
    ASSERT_EQ(Vec2f(1.0f, 2.0f), v.xy());
    ASSERT_EQ(Vec3f(1.0f, 2.0f, 3.0f), v.xyz());
    ASSERT_EQ(v, v.xyzw());
}

TEST(VecTest, Vec4fOverLast) {
    Vec4f v(2.0f, 4.0f, 8.0f, 2.0f);
    ASSERT_EQ(Vec3f(1.0f, 2.0f, 4.0f), v.overLast());
}

TEST(VecTest, Vec3fDot) {
    ASSERT_FLOAT_EQ(-748013.6097f, Vec3f(2.3f, 8.7878f, -2323.0f).dot(Vec3f(4.333f, -2.0f, 322.0f)));
}

TEST(VecTest, Vec3fDotNull) {
    ASSERT_FLOAT_EQ(0.0f, Vec3f(2.3f, 8.7878f, -2323.0f).dot(Vec3f::Null));
}

TEST(VecTest, Length) {
    ASSERT_FLOAT_EQ(0.0f, Vec3f::Null.length());
    ASSERT_FLOAT_EQ(1.0f, Vec3f::PosX.length());
    ASSERT_FLOAT_EQ(std::sqrt(5396411.51542884f), Vec3f(2.3f, 8.7878f, -2323.0f).length());
}

TEST(VecTest, LengthSquared) {
    ASSERT_FLOAT_EQ(0.0f, Vec3f::Null.squaredLength());
    ASSERT_FLOAT_EQ(1.0f, Vec3f::PosX.squaredLength());
    ASSERT_FLOAT_EQ(5396411.51542884f, Vec3f(2.3f, 8.7878f, -2323.0f).squaredLength());
}

TEST(VecTest, DistanceTo) {
    const Vec3f v1(2.3f, 8.7878f, -2323.0f);
    const Vec3f v2(4.333f, -2.0f, 322.0f);
    ASSERT_FLOAT_EQ(0.0f, v1.distanceTo(v1));
    ASSERT_FLOAT_EQ(v1.length(), v1.distanceTo(Vec3f::Null));
    ASSERT_FLOAT_EQ((v1 - v2).length(), v1.distanceTo(v2));
}

TEST(VecTest, SquaredDistanceTo) {
    const Vec3f v1(2.3f, 8.7878f, -2323.0f);
    const Vec3f v2(4.333f, -2.0f, 322.0f);
    ASSERT_FLOAT_EQ(0.0f, v1.squaredDistanceTo(v1));
    ASSERT_FLOAT_EQ(v1.squaredLength(), v1.squaredDistanceTo(Vec3f::Null));
    ASSERT_FLOAT_EQ((v1 - v2).squaredLength(), v1.squaredDistanceTo(v2));
}

TEST(VecTest, Normalize) {
    ASSERT_EQ(Vec3f::PosX, Vec3f::PosX.normalized());
    ASSERT_EQ(Vec3f::NegX, Vec3f::NegX.normalized());
    
    const Vec3f v1(2.3f, 8.7878f, -2323.0f);
    const Vec3f v2(4.333f, -2.0f, 322.0f);
    ASSERT_TRUE((v1 / v1.length()).equals(v1.normalized()));
    ASSERT_TRUE((v2 / v2.length()).equals(v2.normalized()));
}

TEST(VecTest, Null) {
    ASSERT_TRUE(Vec3f::Null.null());
    ASSERT_FALSE(Vec3f::PosX.null());
}

TEST(VecTest, ParallelTo) {
    ASSERT_TRUE(Vec3f::PosX.parallelTo(Vec3f::PosX));
    ASSERT_FALSE(Vec3f::PosX.parallelTo(Vec3f::NegX));
    ASSERT_FALSE(Vec3f::PosX.parallelTo(Vec3f::PosY));
}