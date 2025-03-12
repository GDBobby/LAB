#include "Matrix.h"
#include "SupportingMath.h"
#include "CoordinateSystems.h"

namespace LAB{
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
    //both Projection and Ortho functions copied from here

    template<std::floating_point F>
    constexpr Matrix<F, 4, 4, 16> CreateProjectionMatrix(F const field_of_view_radians, F const near, F const far)  { 
        Matrix<F, 4, 4, 16> ret;

        //until i get a better function for calculating tan
        const F halfFOV = field_of_view_radians * F(0.5);
        const F scale = SupportingMath::Cos(halfFOV) / SupportingMath::Sin(halfFOV);
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
    constexpr Matrix<F, 4, 4, 16> CreateViewMatrix(Vector<float, 3> const position, Vector<float, 3> forward){
        Matrix<F, 4, 4, 16> ret;

        if constexpr(CoordinateSystem::coordinateSystem == CoordinateSystem::GLSL_XRightYDownZBackward){
            const Vector<float, 3> right = CrossProduct(forward, Vector<float, 3>::Up()).Normalize();
            const Vector<float, 3> up = CrossProduct(right, forward).Normalize();

            ret.At(0, 0) = right.x;
            ret.At(1, 0) = right.y;
            ret.At(2, 0) = right.z;

            ret.At(0, 1) = -up.x;
            ret.At(1, 1) = -up.x;
            ret.At(2, 1) = -up.x;

            ret.At(0, 2) = -forward.x;
            ret.At(1, 2) = -forward.y;
            ret.At(2, 2) = -forward.z;

            ret.At(3, 0) = -position.DotProduct(right);
            ret.At(3, 1) = position.DotProduct(up);
            ret.At(3, 2) = position.DotProduct(forward);
            
            ret.At(0, 3) = 0.f;
            ret.At(1, 3) = 0.f;
            ret.At(2, 3) = 0.f;
            ret.At(3, 3) = 1.f;
        }
        else{
            static_assert(false && "other coordinate systems not supported yet");
        }

        return ret;
    }
}