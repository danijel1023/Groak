#pragma once
#include <ostream>

template<typename Base>
struct Vector2d {
    Base X = Base(), Y = Base();
    typedef Base Type;

    Vector2d operator+(const Vector2d& Right) const { return { X + Right.X, Y + Right.Y }; }
    Vector2d operator-(const Vector2d& Right) const { return { X - Right.X, Y - Right.Y }; }

    Vector2d& operator+=(const Vector2d& Right) { X += Right.X; Y += Right.Y; return *this; }
    Vector2d& operator-=(const Vector2d& Right) { X -= Right.X; Y -= Right.Y; return *this; }

    template<class Vec>
    Vec Cast() const { return { static_cast<Vec::Type>(X), static_cast<Vec::Type>(Y) }; }
};

template<typename Base>
struct Vector3d {
    Base X = Base(), Y = Base(), Z = Base();
    typedef Base Type;

    Vector3d operator+(const Vector3d& Right) const { return { X + Right.X, Y + Right.Y, Z + Right.Z }; }
    Vector3d operator-(const Vector3d& Right) const { return { X - Right.X, Y - Right.Y, Z - Right.Z }; }

    Vector3d& operator+=(const Vector3d& Right) { X += Right.X; Y += Right.Y; Z += Right.Z; return *this; }
    Vector3d& operator-=(const Vector3d& Right) { X -= Right.X; Y -= Right.Y; Z -= Right.Z; return *this; }

    template<class Vec>
    Vec Cast() const { return { static_cast<Vec::Type>(X), static_cast<Vec::Type>(Y), static_cast<Vec::Type>(Z) }; }
};

template<typename Base>
struct Vector4d {
    Base X = Base(), Y = Base(), Z = Base(), W = Base();
    typedef Base Type;

    Vector4d operator+(const Vector4d& Right) const { return { X + Right.X, Y + Right.Y, Z + Right.Z, W + Right.W }; }
    Vector4d operator-(const Vector4d& Right) const { return { X - Right.X, Y - Right.Y, Z - Right.Z, W - Right.W }; }

    Vector4d& operator+=(const Vector4d& Right) { X += Right.X; Y += Right.Y; Z += Right.Z; W += Right.W; return *this; }
    Vector4d& operator-=(const Vector4d& Right) { X -= Right.X; Y -= Right.Y; Z -= Right.Z; W -= Right.W; return *this; }

    template<class Vec>
    Vec Cast() const { return { static_cast<Vec::Type>(X), static_cast<Vec::Type>(Y), static_cast<Vec::Type>(Z), static_cast<Vec::Type>(W) }; }
};

using GVec2 = Vector2d<float>;
using GVec3 = Vector3d<float>;
using GVec4 = Vector4d<float>;

using GIVec2 = Vector2d<int>;
using GIVec3 = Vector3d<int>;
using GIVec4 = Vector4d<int>;

using GUIVec2 = Vector2d<unsigned int>;
using GUIVec3 = Vector3d<unsigned int>;
using GUIVec4 = Vector4d<unsigned int>;

using GCVec2 = Vector2d<char>;
using GCVec3 = Vector3d<char>;
using GCVec4 = Vector4d<char>;

using GUCVec2 = Vector2d<unsigned char>;
using GUCVec3 = Vector3d<unsigned char>;
using GUCVec4 = Vector4d<unsigned char>;

using GPos = Vector2d<int>;
using GSize = Vector2d<int>;


template<typename T>
std::ostream& operator<<(std::ostream& Stream, const Vector2d<T>& Vec) {
    Stream << "x:" << Vec.X << ", y:" << Vec.Y;
    return Stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& Stream, const Vector3d<T>& Vec) {
    Stream << "x:" << Vec.X << ", y:" << Vec.Y << ", z:" << Vec.Z;
    return Stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& Stream, const Vector4d<T>& Vec) {
    Stream << "x:" << Vec.X << ", y:" << Vec.Y << ", z:" << Vec.Z << ", w:" << Vec.W;
    return Stream;
}
