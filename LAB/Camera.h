#pragma once
#include "Matrix.h"
#include "SupportingMath.h"
#include "CoordinateSystems.h"
#include "Debugging.h"

namespace lab{
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
    //both Projection and Ortho functions copied from here

    struct VULKAN_API_PROJECTION_HELPER{};
    struct DIRECTX_API_PROJECTION_HELPER{};
    struct OPENGL_API_PROJECTION_HELPER{};

    namespace Perspective { 
        enum API{
            Vulkan = 0,
            DirectX = 1,
            OpenGL = 2,
        };
    }
    //idk if this is correct
    template<Perspective::API PerspectiveAPI, std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> ProjectionMatrix(F const field_of_view_radians, F const aspectRatio, F const near, F const far)  { 
        Matrix<F, 4, 4> ret{0.f};

        const F scale = F(1) / Tan(field_of_view_radians * F(0.5));
        ret.columns[0][0] = scale / aspectRatio;
        if constexpr (PerspectiveAPI == Perspective::API::Vulkan){
            ret.columns[1][1] = -scale;
        }
        else{
            ret.columns[1][1] = scale;
        }
        ret.columns[3][2] = -far * near / (far - near);

        if(PerspectiveAPI == Perspective::API::DirectX){
            ret.columns[2][2] = far / (far - near);
            ret.columns[2][3] = F(1);
        }
        else{
            ret.columns[2][2] = -far / (far - near);
            ret.columns[2][3] = F(-1);
        }
        return ret;
    }

    template<Perspective::API PerspectiveAPI, std::floating_point F>
    LAB_constexpr void ProjectionMatrix(Matrix<F, 4, 4>& projMat, F const field_of_view_radians, F const aspectRatio, F const near, F const far)  {
        //if the projection mat is gonna be created over and over, resetting every value to 0 is inefficient
        //but realistically, projection should be changed maybe once per scene? less? not a big deal
        projMat = ProjectionMatrix(field_of_view_radians, aspectRatio, near, far);
    }
    template<Perspective::API PerspectiveAPI, std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> PerspectiveMatrix(F const field_of_view_radians, F const aspectRatio, F const near, F const far)  { 
        return ProjectionMatrix(field_of_view_radians, aspectRatio, near, far);
    }

    template<Perspective::API PerspectiveAPI, std::floating_point F>
    LAB_constexpr void PerspectiveMatrix(Matrix<F, 4, 4>& projMat, F const field_of_view_radians, F const aspectRatio, F const near, F const far)  {
        //if the projection mat is gonna be created over and over, resetting every value to 0 is inefficient
        //but realistically, projection should be changed maybe once per scene? less? not a big deal
        projMat = ProjectionMatrix(field_of_view_radians, aspectRatio, near, far);
    }

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> OrthographicMatrix(F const bottom, F const top, F const left, F const right, F const near, F const far) {

        const F rMl = right - left;
        const F tMb = top - bottom;
        const float fMn = far - near;
        Matrix<F, 4, 4> ret;
        ret.columns[0][0] = F(2) / rMl;
        ret.columns[0][1] = F(0);
        ret.columns[0][2] = F(0);
        ret.columns[0][3] = F(0);

        ret.columns[1][0] = F(0);
        ret.columns[1][1] = F(2) / tMb;
        ret.columns[1][2] = F(0);
        ret.columns[1][3] = F(0);

        ret.columns[2][0] = F(0);
        ret.columns[2][1] = F(0);
        ret.columns[2][2] = F(-2) / fMn;
        ret.columns[2][3] = F(0);

        ret.columns[3][0] = -(right + left) / rMl;
        ret.columns[3][1] = -(top + bottom) / tMb;
        ret.columns[3][2] = -(far + near) / fMn;
        ret.columns[3][3] = F(1);

        return ret;
    }
    template<std::floating_point F>
    LAB_constexpr void OrthographicMatrix(Matrix<F, 4, 4>& orthoMat, F const bottom, F const top, F const left, F const right, F const near, F const far)  {
        //if the projection mat is gonna be created over and over, resetting every value to 0 is inefficient
        //but realistically, projection should be changed maybe once per scene? less? not a big deal
        orthoMat = OrthographicMatrix(bottom, top, left, right, near, far);
    }
    
    //view matrix is recreated at least once per frame
    //i need to be a bit more careful with this

    template<typename CS, std::floating_point F>
    requires(IsCoordinateSystem<CS>::value)
    LAB_constexpr Matrix<F, 4, 4> ViewDirection(Vector<F, 3> const position, Vector<F, 3> const forward, Vector<F, 3> const up = CS::unitUpVector){
        Matrix<F, 4, 4> ret{};
        const Vector<float, 3> right = Cross(forward, up).Normalized();
        const Vector<float, 3> up = Cross(right, forward).Normalized();
        
        if constexpr(CS::f_sign){
            ret.columns[0][CS::f_axis] = -forward.x;
            ret.columns[1][CS::f_axis] = -forward.y;
            ret.columns[2][CS::f_axis] = -forward.z;
            ret.columns[3][CS::f_axis] = position.Dot(forward);
        }
        else{
            ret.columns[0][CS::f_axis] = forward.x;
            ret.columns[1][CS::f_axis] = forward.y;
            ret.columns[2][CS::f_axis] = forward.z;
            ret.columns[3][CS::f_axis] = -position.Dot(forward);
        }
        if constexpr(CS::u_sign){
            ret.columns[0][CS::u_axis] = -up.x;
            ret.columns[1][CS::u_axis] = -up.y;
            ret.columns[2][CS::u_axis] = -up.z;
            ret.columns[3][CS::u_axis] = position.Dot(up);
        }
        else{
            ret.columns[0][CS::u_axis] = up.x;
            ret.columns[1][CS::u_axis] = up.y;
            ret.columns[2][CS::u_axis] = up.z;
            ret.columns[3][CS::u_axis] = -position.Dot(up);
        }
        if constexpr(CS::r_sign){
            ret.columns[0][CS::r_axis] = -right.x;
            ret.columns[1][CS::r_axis] = -right.y;
            ret.columns[2][CS::r_axis] = -right.z;
            ret.columns[3][CS::r_axis] = position.Dot(right);
        }
        else{
            ret.columns[0][CS::r_axis] = right.x;
            ret.columns[1][CS::r_axis] = right.y;
            ret.columns[2][CS::r_axis] = right.z;
            ret.columns[3][CS::r_axis] = -position.Dot(right);
        }
        ret.columns[0][3] = F(0);
        ret.columns[1][3] = F(0);
        ret.columns[2][3] = F(0);
        ret.columns[3][3] = F(1);
        return ret;
    }

    //the final row needs to be set to 0,0,0,1 outside of this function
    template<typename CS, std::floating_point F>
    requires(IsCoordinateSystem<CS>::value)
    LAB_constexpr void ViewDirection(Matrix<F, 4, 4>& viewMat, Vector<F, 3> const position, Vector<F, 3> const forward, Vector<F, 3> const up = CS::unitUpVector){
        const Vector<float, 3> right = Cross(forward, up).Normalized();
        const Vector<float, 3> up = Cross(right, forward).Normalized();

        if constexpr(CS::f_sign){
            viewMat.columns[0][CS::f_axis] = -forward.x;
            viewMat.columns[1][CS::f_axis] = -forward.y;
            viewMat.columns[2][CS::f_axis] = -forward.z;
            viewMat.columns[3][CS::f_axis] = position.Dot(forward);
        }
        else{
            viewMat.columns[0][CS::f_axis] = forward.x;
            viewMat.columns[1][CS::f_axis] = forward.y;
            viewMat.columns[2][CS::f_axis] = forward.z;
            viewMat.columns[3][CS::f_axis] = -position.Dot(forward);
        }
        if constexpr(CS::u_sign){
            viewMat.columns[0][CS::u_axis] = -up.x;
            viewMat.columns[1][CS::u_axis] = -up.y;
            viewMat.columns[2][CS::u_axis] = -up.z;
            viewMat.columns[3][CS::u_axis] = position.Dot(up);
        }
        else{
            viewMat.columns[0][CS::u_axis] = up.x;
            viewMat.columns[1][CS::u_axis] = up.y;
            viewMat.columns[2][CS::u_axis] = up.z;
            viewMat.columns[3][CS::u_axis] = -position.Dot(up);
        }
        if constexpr(CS::r_sign){
            viewMat.columns[0][CS::r_axis] = -right.x;
            viewMat.columns[1][CS::r_axis] = -right.y;
            viewMat.columns[2][CS::r_axis] = -right.z;
            viewMat.columns[3][CS::r_axis] = position.Dot(right);
        }
        else{
            viewMat.columns[0][CS::r_axis] = right.x;
            viewMat.columns[1][CS::r_axis] = right.y;
            viewMat.columns[2][CS::r_axis] = right.z;
            viewMat.columns[3][CS::r_axis] = -position.Dot(right);
        }
#if LAB_DEBUGGING_FLOAT_ANOMALY
        //just make it a warning since setting the final values later is fine
#endif
    }

    template<typename CS>
    requires(IsCoordinateSystem<CS>::value)
    LAB_constexpr void ViewRotation(Matrix<float, 4, 4>& view, lab::vec3 const position, lab::vec3 const rotation) {
        const float c3 = lab::Cos(rotation.z);
        const float s3 = lab::Sin(rotation.z);
        const float c2 = lab::Cos(rotation.x);
        const float s2 = lab::Sin(rotation.x);
        const float c1 = lab::Cos(rotation.y);
        const float s1 = lab::Sin(rotation.y);

        if constexpr (CS::f_sign) {
            view.columns[0][CS::f_axis] = -(c2 * s1);
            view.columns[1][CS::f_axis] = s2;
            view.columns[2][CS::f_axis] = -(c1 * c2);
            view.columns[3][CS::f_axis] = fx * position.x + fy * position.y + fz * position.z;
        } else {
            view.columns[0][CS::f_axis] = (c2 * s1);
            view.columns[1][CS::f_axis] = -s2;
            view.columns[2][CS::f_axis] = (c1 * c2);
            view.columns[3][CS::f_axis] = -(fx * position.x + fy * position.y + fz * position.z);
        }

        if constexpr (CS::u_sign) {
            view.columns[0][CS::u_axis] = -(c3 * s1 * s2 - c1 * s3);
            view.columns[1][CS::u_axis] = -(c2 * c3);
            view.columns[2][CS::u_axis] = -(c1 * c3 * s2 + s1 * s3);
            view.columns[3][CS::u_axis] = ux * position.x + uy * position.y + uz * position.z;
        } else {
            view.columns[0][CS::u_axis] = (c3 * s1 * s2 - c1 * s3);
            view.columns[1][CS::u_axis] = (c2 * c3);
            view.columns[2][CS::u_axis] = (c1 * c3 * s2 + s1 * s3);
            view.columns[3][CS::u_axis] = -(ux * position.x + uy * position.y + uz * position.z);
        }

        // Right axis
        if constexpr (CS::r_sign) {
            view.columns[0][CS::r_axis] = -(c1 * c3 + s1 * s2 * s3);
            view.columns[1][CS::r_axis] = -(c2 * s3);
            view.columns[2][CS::r_axis] = -(c1 * s2 * s3 - c3 * s1);
            view.columns[3][CS::r_axis] = rx * position.x + ry * position.y + rz * position.z;
        } else {
            view.columns[0][CS::r_axis] = (c1 * c3 + s1 * s2 * s3);
            view.columns[1][CS::r_axis] = (c2 * s3);
            view.columns[2][CS::r_axis] = (c1 * s2 * s3 - c3 * s1);
            view.columns[3][CS::r_axis] = -(rx * position.x + ry * position.y + rz * position.z);
        }
    }
}