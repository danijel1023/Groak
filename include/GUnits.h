#pragma once

template<typename T>
struct Template_Vec2 {
    T X = T(), Y = T();

    Template_Vec2 operator+(Template_Vec2 Right) { return { (X + Right.X), (Y + Right.Y) }; }
    Template_Vec2 operator-(Template_Vec2 Right) { return { (X - Right.X), (Y - Right.Y) }; }

    Template_Vec2& operator+=(Template_Vec2 Right) { X += Right.X; Y += Right.Y; return *this; }
    Template_Vec2& operator-=(Template_Vec2 Right) { X -= Right.X; Y -= Right.Y; return *this; }
};

template<typename T>
struct Template_Vec3 {
    T X = T(), Y = T(), Z = T();

    Template_Vec3 operator+(Template_Vec3 Right) { return { (X + Right.X), (Y + Right.Y), (Z + Right.Z) }; }
    Template_Vec3 operator-(Template_Vec3 Right) { return { (X - Right.X), (Y - Right.Y), (Z - Right.Z) }; }

    Template_Vec3& operator+=(Template_Vec3 Right) { X += Right.X; Y += Right.Y; Z += Right.Z; return *this; }
    Template_Vec3& operator-=(Template_Vec3 Right) { X -= Right.X; Y -= Right.Y; Z -= Right.Z; return *this; }
};

template<typename T>
struct Template_Vec4 {
    T X = T(), Y = T(), Z = T(), W = T();

    Template_Vec4 operator+(const Template_Vec4& Right) { return { (X + Right.X), (Y + Right.Y), (Z + Right.Z), (W + Right.W) }; }
    Template_Vec4 operator-(const Template_Vec4& Right) { return { (X - Right.X), (Y - Right.Y), (Z - Right.Z), (W - Right.W) }; }

    Template_Vec4& operator+=(const Template_Vec4& Right) { X += Right.X; Y += Right.Y; Z += Right.Z; W += Right.W; return *this; }
    Template_Vec4& operator-=(const Template_Vec4& Right) { X -= Right.X; Y -= Right.Y; Z -= Right.Z; W -= Right.W; return *this; }
};

using GVec2 = Template_Vec2<float>;
using GVec3 = Template_Vec3<float>;
using GVec4 = Template_Vec4<float>;

using GIVec2 = Template_Vec2<int>;
using GIVec3 = Template_Vec3<int>;
using GIVec4 = Template_Vec4<int>;

using GUIVec2 = Template_Vec2<unsigned int>;
using GUIVec3 = Template_Vec3<unsigned int>;
using GUIVec4 = Template_Vec4<unsigned int>;

using GPos = Template_Vec2<int>;
using GSize = Template_Vec2<int>;
