#pragma once
namespace lab {
    namespace CoordinateSystem{
        enum Direction : int {
            
            XNeg,
            YNeg,
            ZNeg,

            XPos,
            YPos,
            ZPos,

            //idk if dx is different from GLSL in coordinate systems
        };

        constexpr Direction forward = XPos;
        constexpr Direction right = ZPos;
        constexpr Direction up = YPos;

    }
}