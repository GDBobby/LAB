#include "Matrix.h"
#include "SupportingMath.h"

namespace LAB{
    template<std::floating_point F>
    constexpr Matrix<F, 4, 4, 16> CreateProjectionMatrix(F const field_of_view_radians, F const near, F const far)  { 
        // set the basic projection matrix
        Matrix<F, 4, 4, 16> ret;

        const F scale = F(1) / SupportingMath::Tan(field_of_view_radians * F(0.5)); 
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
}