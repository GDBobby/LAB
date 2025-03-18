#include "Matrix.h"
#include "SupportingMath.h"
#include "CoordinateSystems.h"
#include "Debugging.h"

namespace LAB{
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
    //both Projection and Ortho functions copied from here

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> CreateProjectionMatrix(F const field_of_view_radians, F const near, F const far)  { 
        Matrix<F, 4, 4> ret;

        const F scale = F(1) / SupportingMath::Tan(field_of_view_radians * F(0.5));
        //const F scale = F(1) / SupportingMath::Tan(field_of_view_radians * F(0.5)); 
        ret.At(0, 0) = scale;  // scale the x coordinates of the projected point 
        ret.At(1, 1) = scale;  // scale the y coordinates of the projected point 
        ret.At(2, 2) = -far / (far - near);  // used to remap z to [0,1] 
        ret.At(3, 2) = -far * near / (far - near);  // used to remap z [0,1] 
        ret.At(2, 3) = F(-1);  // set w = -z 
        ret.At(3, 3) = F(0); 

        ret.At(0, 1) = F(0);
        ret.At(0, 2) = F(0);
        ret.At(0, 3) = F(0);

        ret.At(1, 0) = F(0);
        ret.At(1, 2) = F(0);
        ret.At(1, 3) = F(0);
        
        ret.At(2, 0) = F(0);
        ret.At(2, 1) = F(0);
        
        ret.At(3, 0) = F(0);
        ret.At(3, 1) = F(0);

        return ret;
    }
    template<std::floating_point F>
    Matrix<F, 4, 4, 16> CreateOrthographicMatrix(F const bottom, F const top, F const left, F const right, F const near, F const far) {

        const F rMl = right - left;
        const F tMb = top - bottom;
        const float fMn = far - near;
        Matrix<F, 4, 4, 16> ret;
        ret.At(0, 0) = F(2) / rMl;
        ret.At(0, 1) = F(0);
        ret.At(0, 2) = F(0);
        ret.At(0, 3) = F(0);

        ret.At(1, 0) = F(0);
        ret.At(1, 1) = F(2) / tMb;
        ret.At(1, 2) = F(0);
        ret.At(1, 3) = F(0);

        ret.At(2, 0) = F(0);
        ret.At(2, 1) = F(0);
        ret.At(2, 2) = F(-2) / fMn;
        ret.At(2, 3) = F(0);

        ret.At(3, 0) = -(right + left) / rMl;
        ret.At(3, 1) = -(top + bottom) / tMb;
        ret.At(3, 2) = -(far + near) / fMn;
        ret.At(3, 3) = F(1);

        return ret;
    }
    
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> CreateViewMatrix(Vector<F, 3> const position, Vector<F, 3> const forward){
        Matrix<F, 4, 4> ret;
        const Vector<float, 3> right = CrossProduct(forward, Vector<float, 3>::Up()).Normalize();
        const Vector<float, 3> up = CrossProduct(right, forward).Normalize();

        constexpr bool f_sign = CoordinateSystem::forward < CoordinateSystem::XPos;
        constexpr bool u_sign = CoordinateSystem::up < CoordinateSystem::XPos;
        constexpr bool r_sign = CoordinateSystem::right < CoordinateSystem::XPos;
        
        constexpr int f_axis = CoordinateSystem::forward - (!f_sign * 3);
        constexpr int u_axis = CoordinateSystem::up - (!u_sign * 3);
        constexpr int r_axis = CoordinateSystem::right - (!r_sign * 3);
        
        if constexpr(f_sign){
            ret.At(0, f_axis) = -forward.x;
            ret.At(1, f_axis) = -forward.y;
            ret.At(2, f_axis) = -forward.z;
            ret.At(3, f_axis) = position.DotProduct(forward);
        }
        else{
            ret.At(0, f_axis) = forward.x;
            ret.At(1, f_axis) = forward.y;
            ret.At(2, f_axis) = forward.z;
            ret.At(3, f_axis) = -position.DotProduct(forward);
        }
        if constexpr(u_sign){
            ret.At(0, u_axis) = -up.x;
            ret.At(1, u_axis) = -up.y;
            ret.At(2, u_axis) = -up.z;
            ret.At(3, u_axis) = position.DotProduct(up);
        }
        else{
            ret.At(0, u_axis) = up.x;
            ret.At(1, u_axis) = up.y;
            ret.At(2, u_axis) = up.z;
            ret.At(3, u_axis) = -position.DotProduct(up);
        }
        if constexpr(r_sign){
            ret.At(0, r_axis) = -right.x;
            ret.At(1, r_axis) = -right.y;
            ret.At(2, r_axis) = -right.z;
            ret.At(3, r_axis) = position.DotProduct(right);
        }
        else{
            ret.At(0, r_axis) = right.x;
            ret.At(1, r_axis) = right.y;
            ret.At(2, r_axis) = right.z;
            ret.At(3, r_axis) = -position.DotProduct(right);
        }
        return ret;
    }
}