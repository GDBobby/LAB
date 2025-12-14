#pragma once

#include "LAB/Vector.h"
#include "LAB/Matrix.h"
#include "LAB/Camera.h"
#include "LAB/Quaternion.h"

#include "LAB/SupportingMath.h"

#include <optional>

namespace lab{
//put another DX namespace in here?

    //need a potential warning, d3dx9math is left handed and row major

    struct Plane{
        float a;
        float b;
        float c;
        float d;
    };

    struct Viewport {
        float X;
        float Y;
        float Width;
        float Height; 
        float MinZ;
        float MaxZ;
    };

    LAB_constexpr Plane PlaneFromPoints(Vector<float, 3> const& p0, Vector<float, 3>const& p1, Vector<float, 3> const& p2) {
        const Vector<float, 3> v1 = p1 - p0;
        const Vector<float, 3> v2 = p2 - p0;

        const Vector<float, 3> normal = Cross(v1, v2).Normalized();

        const float d = -Dot(normal, p0);

        return Plane{ normal.x, normal.y, normal.z, d };
    }

    inline [[nodiscard]] Matrix<float, 4, 4> GetDXView(lab::vec3 const& pos, lab::vec3 const& target, lab::vec3 const& up){
	    return lab::ViewTarget<lab::CoordinateSystem<lab::Direction::ZDir<false>, lab::Direction::XDir<true>, lab::Direction::YDir<false>>>(pos, target, up);
    }
    inline [[nodiscard]] lab::mat4 GetDXProjection(float const fov, float const aspectRatio, float const cam_n, float const cam_f) { //n = near, f = far
        return lab::ProjectionMatrix<lab::Perspective::API::DirectX>(fov, aspectRatio, cam_n, cam_f);
    }
    inline [[nodiscard]] lab::mat4 GetDXOrtho(float const width, float const height, float const cam_n, float const cam_f) {
        return lab::OrthographicMatrix(-height / 2.f, height / 2.f, -width / 2.f, width / 2.f, cam_n, cam_f);
    }


    LAB_constexpr Vector<float, 3> TransformCoord(Vector<float, 3> const& vec, Matrix<float, 4, 4> const& mat) {
        const Vector<float, 4> temp = vec4(vec, 1.f) * mat;
        return Vector<float, 3>(temp) / temp.w;
    }
    
    LAB_constexpr lab::vec2 TransformCoord(const lab::vec2& v, const lab::mat4& m) {
        const lab::vec4 temp(v.x, v.y, 0.0f, 1.0f);
        const lab::vec4 result = temp * m;

        if (result.w != 0.0f) {
            return lab::vec2(result.x / result.w, result.y / result.w);
        } else {
            return lab::vec2(result.x, result.y);
        }
    }
    
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> AffineTransformation(
        const F scale,
        const Vector<F, 3>& rotationCenter,
        const Quaternion<F>& rotation,
        const Vector<F, 3>& translation
    ) {
        const F xx = rotation.x * rotation.x;
        const F yy = rotation.y * rotation.y;
        const F zz = rotation.z * rotation.z;
        const F xy = rotation.x * rotation.y;
        const F xz = rotation.x * rotation.z;
        const F yz = rotation.y * rotation.z;
        const F wx = rotation.w * rotation.x;
        const F wy = rotation.w * rotation.y;
        const F wz = rotation.w * rotation.z;

        const F m00 = scale * (F(1) - F(2) * (yy + zz));
        const F m01 = scale * (F(2) * (xy + wz));
        const F m02 = scale * (F(2) * (xz - wy));

        const F m10 = scale * (F(2) * (xy - wz));
        const F m11 = scale * (F(1) - F(2) * (zz + xx));
        const F m12 = scale * (F(2) * (yz + wx));

        const F m20 = scale * (F(2) * (xz + wy));
        const F m21 = scale * (F(2) * (yz - wx));
        const F m22 = scale * (F(1) - F(2) * (xx + yy));

        const lab::Vector<F, 3> rotatedCenter{
            m00 * rotationCenter.x + m01 * rotationCenter.y + m02 * rotationCenter.z,
            m10 * rotationCenter.x + m11 * rotationCenter.y + m12 * rotationCenter.z,
            m20 * rotationCenter.x + m21 * rotationCenter.y + m22 * rotationCenter.z
        };

        const lab::Vector<F, 3> finalTranslation = translation + (rotationCenter - rotatedCenter);

        return Matrix<F, 4, 4>{
            lab::Vector<F, 4>(m00, m01, m02, F(0)),
            lab::Vector<F, 4>(m10, m11, m12, F(0)),
            lab::Vector<F, 4>(m20, m21, m22, F(0)),
            lab::Vector<F, 4>(finalTranslation.x, finalTranslation.y, finalTranslation.z, F(1))
        };
    }

    LAB_constexpr Vector<float, 3> Transform(Vector<float, 3> const& v, Matrix<float, 4, 4> const& m){
        return Vector<float, 3>{
            v.x * m.At(0, 0) + v.y * m.At(1, 0) + v.z * m.At(2, 0) + m.At(3, 0),
            v.x * m.At(0, 1) + v.y * m.At(1, 1) + v.z * m.At(2, 1) + m.At(3, 1),
            v.x * m.At(0, 2) + v.y * m.At(1, 2) + v.z * m.At(2, 2) + m.At(3, 2)
        };
    }

    LAB_constexpr Vector<float, 3> TransformNormal(Vector<float, 3> const& v, Matrix<float, 4, 4> const& m) {
        return Vector<float, 3>(
            v.x * m.At(0, 0) + v.y * m.At(1, 0) + v.z * m.At(2, 0),
            v.x * m.At(0, 1) + v.y * m.At(1, 1) + v.z * m.At(2, 1),
            v.x * m.At(0, 2) + v.y * m.At(1, 2) + v.z * m.At(2, 2)
        );
    }
        
    inline bool IsTouchRayTri(vec3 const& v0, vec3 const& v1, vec3 const& v2,
                        vec3 const& vOrig, vec3 const& vDir, 
                        float& pfDist
    ) {
        // Find vectors for two edges sharing vert0
        float t,u,v;
        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;

        // Begin calculating determinant - also used to calculate U parameter
        vec3 pvec = Cross(vDir, edge2);

        // If determinant is near zero, ray lies in plane of triangle
        float det = Dot(edge1, pvec);

        vec3 tvec;
        if( det > 0 )
            tvec = vOrig - v0;
        else
            return false;

        if( det < 0.0001f )
            return false;

        // Calculate U parameter and test bounds
        u = Dot(tvec, pvec);
        if( u < 0.0f || u > det )
            return false;

        // Prepare to test V parameter
        vec3 qvec;
        qvec = Cross(tvec, edge1);

        // Calculate V parameter and test bounds
        v = Dot(vDir, qvec);
        if( v < 0.0f || u + v > det )
            return false;

        // Calculate t, scale parameters, ray intersects triangle
        t = Dot(edge2, qvec);
        float fInvDet = 1.0f / det;
        t *= fInvDet;
        pfDist = t;
    //	u *= fInvDet;
    //	v *= fInvDet;

    //	*pvIntersect = vOrig + t * vDir;
        return true;
    }

    inline bool IntersectTri(
        const vec3& v0, const vec3& v1, const vec3& v2,
        const vec3& rayOrigin, const vec3& rayDir,
        float* pU, float* pV, float* pDist)
    {
        // Moller-Trumbore algorithm
        const float EPSILON = 1e-6f;
        const vec3 edge1 = v1 - v0;
        const vec3 edge2 = v2 - v0;
        const vec3 h = lab::Cross(rayDir, edge2);
        const float a = lab::Dot(edge1, h);
        if (std::abs(a) < EPSILON)
            return false; // Ray is parallel to triangle

        float f = 1.0f / a;
        vec3 s = rayOrigin - v0;
        float u = f * lab::Dot(s, h);
        if (u < 0.0f || u > 1.0f)
            return false;

        vec3 q = lab::Cross(s, edge1);
        float v = f * lab::Dot(rayDir, q);
        if (v < 0.0f || u + v > 1.0f){
            return false;
        }

        float t = f * lab::Dot(edge2, q);
        if (t > EPSILON) {
            if (pU) *pU = u;
            if (pV) *pV = v;
            if (pDist) *pDist = t;
            return true;
        }

        return false; // Line intersects but not ray
    }

    inline Matrix<float, 4, 4> MatrixRotationYawPitchRoll(const float yaw, const float pitch, const float roll) {
        const float sy = lab::Sin(yaw);
        const float cy = lab::Cos(yaw);
        const float sp = lab::Sin(pitch);
        const float cp = lab::Cos(pitch);
        const float sr = lab::Sin(roll);
        const float cr = lab::Cos(roll);

        return Matrix<float, 4, 4>{
            Vector<float, 4>(
                cr * cy + sr * sp * sy,
                sr * cp,
                cr * -sy + sr * sp * cy,
                0.f
            ),
            Vector<float, 4>(
                -sr * cy + cr * sp * sy,
                cr * cp,
                -sr * -sy + cr * sp * cy,
                0.f
            ),
            Vector<float, 4>(
                cp * sy,
                -sp,
                cp * cy,
                0.f
            ),
            Vector<float, 4>(0.f, 0.f, 0.f, 1.f)
        };
    }

    inline lab::Quat QuaternionRotationYawPitchRoll(float yaw, float pitch, float roll) {
        const float cy = lab::Cos(yaw * 0.5f);
        const float sy = lab::Sin(yaw * 0.5f);
        const float cp = lab::Cos(pitch * 0.5f);
        const float sp = lab::Sin(pitch * 0.5f);
        const float cr = lab::Cos(roll * 0.5f);
        const float sr = lab::Sin(roll * 0.5f);

        return lab::Quat{
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        };
    }



    inline Vector<float, 3> Project(
        Vector<float, 3> const& point,
        Viewport const& vp,
        Matrix<float, 4, 4> const& matProj,
        Matrix<float, 4, 4> const& matView,
        Matrix<float, 4, 4> const& matWorld
    ) {
        //world space
        lab::vec4 v = lab::vec4(point, 1.0f) * matWorld;
        //view space
        v = v * matView;
        //clip space
        v = v * matProj;

        if (v.w != 0.0f) {
            v.x /= v.w;
            v.y /= v.w;
            v.z /= v.w;
        }

        // Map to viewport coordinates
        return Vector<float, 3>{
            (v.x + 1.0f) * 0.5f * vp.Width + vp.X,
            (1.0f - v.y) * 0.5f * vp.Height + vp.Y,
            v.z * (vp.MaxZ - vp.MinZ) + vp.MinZ
        };
    }

    inline std::optional<Vector<float, 3>> PlaneIntersectLine(
        const Plane& plane,
        const Vector<float, 3>& v1,
        const Vector<float, 3>& v2
    ) {
        const Vector<float, 3> normal{ plane.a, plane.b, plane.c };
        const Vector<float, 3> dir = v2 - v1;
        const float denom = Dot(normal, dir);

        if (Abs(denom) < lab::epsilon<float>){
            return std::nullopt;
        }

        const float t = (-plane.d - Dot(normal, v1)) / denom;
        return v1 + t * dir;
    }
} //namespace lab
