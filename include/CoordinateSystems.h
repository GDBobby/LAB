#pragma once
namespace LAB{
    namespace CoordinateSystem{
        enum Enum : int {
            //something like 24 iterations, there's gotta be a better way
            GLSL_XForwardYUpZRight = 0,
            GLSL_XForwardYUpZLeft,
            GLSL_XForwardYDownZRight,
            GLSL_XForwardYDownZLeft,

            GLSL_XBackwardYUpZRight,
            GLSL_XBackwardYUpZLeft,
            GLSL_XBackwardYDownZRight,
            GLSL_XBackwardYDownZLeft,

            //GLSL_XUpYForwardZRight,

            GLSL_XRightYDownZBackward,

            XPos,
            XNeg,
            YPos,
            YNeg,
            ZPos,
            ZNeg,

            //idk if dx is different from GLSL in coordinate systems
        };

        constexpr Enum coordinateSystem = GLSL_XRightYDownZBackward;
        constexpr Enum forward = XPos;
        constexpr Enum right = ZPos;
        constexpr Enum up = YPos;

    }
}