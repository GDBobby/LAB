#include "Matrix.h"
#include "SupportingMath.h"
#include "CoordinateSystems.h"
#include "Debugging.h"

namespace lab{
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
    //both Projection and Ortho functions copied from here


    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> CreateProjectionMatrix(F const field_of_view_radians, F const aspectRatio, F const near, F const far)  { 
        Matrix<F, 4, 4> ret{0.f};

        const F scale = F(1) / SupportingMath::Tan(field_of_view_radians * F(0.5));
        ret.columns[0][0] = scale / aspectRatio;  // scale the x coordinates of the projected point 
        ret.columns[1][1] = scale;  // scale the y coordinates of the projected point 
        ret.columns[2][2] = -far / (far - near);  // used to remap z to [0,1] 
        ret.columns[3][2] = -far * near / (far - near);  // used to remap z [0,1] 

        ret.columns[2][3] = F(-1);  // set w = -z
        return ret;
    }

    template<std::floating_point F>
    LAB_constexpr void CreateProjectionMatrix(Matrix<F, 4, 4>& projMat, F const field_of_view_radians, F const aspectRatio, F const near, F const far)  {
        //if the projection mat is gonna be created over and over, resetting every value to 0 is inefficient
        //but realistically, projection should be changed maybe once per scene? less? not a big deal
        projMat = CreateProjectionMatrix(field_of_view_radians, aspectRatio, near, far);
    }

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> CreateOrthographicMatrix(F const bottom, F const top, F const left, F const right, F const near, F const far) {

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
    LAB_constexpr void CreateOrthographicMatrix(Matrix<F, 4, 4>& orthoMat, F const bottom, F const top, F const left, F const right, F const near, F const far)  {
        //if the projection mat is gonna be created over and over, resetting every value to 0 is inefficient
        //but realistically, projection should be changed maybe once per scene? less? not a big deal
        orthoMat = CreateOrthographicMatrix(bottom, top, left, right, near, far);
    }
    
    //view matrix is recreated at least once per frame
    //i need to be a bit more careful with this

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> CreateViewMatrix(Vector<F, 3> const position, Vector<F, 3> const forward){
        Matrix<F, 4, 4> ret{0.f};
        const Vector<float, 3> right = CrossProduct(forward, Vector<float, 3>::Up()).Normalized();
        const Vector<float, 3> up = CrossProduct(right, forward).Normalized();

        constexpr bool f_sign = CoordinateSystem::forward < CoordinateSystem::XPos;
        constexpr bool u_sign = CoordinateSystem::up < CoordinateSystem::XPos;
        constexpr bool r_sign = CoordinateSystem::right < CoordinateSystem::XPos;
        
        constexpr int f_axis = CoordinateSystem::forward - (!f_sign * 3);
        constexpr int u_axis = CoordinateSystem::up - (!u_sign * 3);
        constexpr int r_axis = CoordinateSystem::right - (!r_sign * 3);
        
        if constexpr(f_sign){
            ret.columns[0][f_axis] = -forward.x;
            ret.columns[1][f_axis] = -forward.y;
            ret.columns[2][f_axis] = -forward.z;
            ret.columns[3][f_axis] = position.DotProduct(forward);
        }
        else{
            ret.columns[0][f_axis] = forward.x;
            ret.columns[1][f_axis] = forward.y;
            ret.columns[2][f_axis] = forward.z;
            ret.columns[3][f_axis] = -position.DotProduct(forward);
        }
        if constexpr(u_sign){
            ret.columns[0][u_axis] = -up.x;
            ret.columns[1][u_axis] = -up.y;
            ret.columns[2][u_axis] = -up.z;
            ret.columns[3][u_axis] = position.DotProduct(up);
        }
        else{
            ret.columns[0][u_axis] = up.x;
            ret.columns[1][u_axis] = up.y;
            ret.columns[2][u_axis] = up.z;
            ret.columns[3][u_axis] = -position.DotProduct(up);
        }
        if constexpr(r_sign){
            ret.columns[0][r_axis] = -right.x;
            ret.columns[1][r_axis] = -right.y;
            ret.columns[2][r_axis] = -right.z;
            ret.columns[3][r_axis] = position.DotProduct(right);
        }
        else{
            ret.columns[0][r_axis] = right.x;
            ret.columns[1][r_axis] = right.y;
            ret.columns[2][r_axis] = right.z;
            ret.columns[3][r_axis] = -position.DotProduct(right);
        }
        ret.columns[0][3] = F(0);
        ret.columns[1][3] = F(0);
        ret.columns[2][3] = F(0);
        ret.columns[3][3] = F(1);
        return ret;
    }

    //the final row needs to be set to 0,0,0,1 outside of this function
    template<std::floating_point F>
    LAB_constexpr void CreateViewMatrix(Matrix<F, 4, 4>& viewMat, Vector<F, 3> const position, Vector<F, 3> const forward){
        const Vector<float, 3> right = CrossProduct(forward, Vector<float, 3>::Up()).Normalized();
        const Vector<float, 3> up = CrossProduct(right, forward).Normalized();

        constexpr bool f_sign = CoordinateSystem::forward < CoordinateSystem::XPos;
        constexpr bool u_sign = CoordinateSystem::up < CoordinateSystem::XPos;
        constexpr bool r_sign = CoordinateSystem::right < CoordinateSystem::XPos;
        
        constexpr int f_axis = CoordinateSystem::forward - (!f_sign * 3);
        constexpr int u_axis = CoordinateSystem::up - (!u_sign * 3);
        constexpr int r_axis = CoordinateSystem::right - (!r_sign * 3);
        
        if constexpr(f_sign){
            viewMat.columns[0][f_axis] = -forward.x;
            viewMat.columns[1][f_axis] = -forward.y;
            viewMat.columns[2][f_axis] = -forward.z;
            viewMat.columns[3][f_axis] = position.DotProduct(forward);
        }
        else{
            viewMat.columns[0][f_axis] = forward.x;
            viewMat.columns[1][f_axis] = forward.y;
            viewMat.columns[2][f_axis] = forward.z;
            viewMat.columns[3][f_axis] = -position.DotProduct(forward);
        }
        if constexpr(u_sign){
            viewMat.columns[0][u_axis] = -up.x;
            viewMat.columns[1][u_axis] = -up.y;
            viewMat.columns[2][u_axis] = -up.z;
            viewMat.columns[3][u_axis] = position.DotProduct(up);
        }
        else{
            viewMat.columns[0][u_axis] = up.x;
            viewMat.columns[1][u_axis] = up.y;
            viewMat.columns[2][u_axis] = up.z;
            viewMat.columns[3][u_axis] = -position.DotProduct(up);
        }
        if constexpr(r_sign){
            viewMat.columns[0][r_axis] = -right.x;
            viewMat.columns[1][r_axis] = -right.y;
            viewMat.columns[2][r_axis] = -right.z;
            viewMat.columns[3][r_axis] = position.DotProduct(right);
        }
        else{
            viewMat.columns[0][r_axis] = right.x;
            viewMat.columns[1][r_axis] = right.y;
            viewMat.columns[2][r_axis] = right.z;
            viewMat.columns[3][r_axis] = -position.DotProduct(right);
        }
    }

}