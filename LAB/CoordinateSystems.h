#pragma once

#include "Vector.h"

namespace lab {
    
    namespace Direction{
        template<bool Positive>
        struct XDir{
            static constexpr uint8_t axis = 0;
            static constexpr bool sign = Positive;
        };
        template<bool Positive>
        struct YDir{
            static constexpr uint8_t axis = 1;
            static constexpr bool sign = Positive;
        };
        template<bool Positive>
        struct ZDir{
            static constexpr uint8_t axis = 2;
            static constexpr bool sign = Positive;
        };
    }

    template<typename ForwardDir, typename RightDir, typename UpDir, std::floating_point F = float>
    requires(
           (ForwardDir::axis != RightDir::axis)
        && (RightDir::axis != UpDir::axis)
        && (ForwardDir::axis != UpDir::axis)
    )
    struct CoordinateSystem {
        static constexpr bool f_sign = ForwardDir::sign;
        static constexpr bool r_sign = RightDir::sign;
        static constexpr bool u_sign = UpDir::sign;
    
        static constexpr int f_axis = static_cast<int>(ForwardDir::axis);
        static constexpr int r_axis = static_cast<int>(RightDir::axis);
        static constexpr int u_axis = static_cast<int>(UpDir::axis);

        static constexpr Vector<F, 3> unitForwardVector = Vector<F, 3>(F(1) * (f_axis == 0), F(1) * (f_axis == 1), F(1) * (f_axis == 2));  
        static constexpr Vector<F, 3> unitUpVector = Vector<F, 3>(F(1)* (u_axis == 0), F(1) * (u_axis == 1), F(1) * (u_axis == 2));
        static constexpr Vector<F, 3> unitRightVector = Vector<F, 3>(F(1) * (r_axis == 0), F(1) * (r_axis == 1), F(1) * (r_axis == 2));
    };

    template<typename T>
    struct IsCoordinateSystem : std::false_type {};
    
    template<typename ForwardDir, typename RightDir, typename UpDir, typename F>
    struct IsCoordinateSystem<CoordinateSystem<ForwardDir, RightDir, UpDir, F>> : std::true_type {};

}