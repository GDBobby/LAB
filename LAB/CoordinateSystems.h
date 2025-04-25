#pragma once

#include "Vector.h"

namespace lab {
    

    namespace Direction{
        enum Axis : uint8_t{
            X = 0,
            Y = 1,
            Z = 2
        };
        constexpr bool operator==(Axis lhs, Axis rhs){
            return static_cast<std::underlying_type_t<Axis>>(lhs) == static_cast<std::underlying_type_t<Axis>>(rhs);
        }
        constexpr bool operator!=(Axis lhs, Axis rhs){
            return !operator==(lhs, rhs);
        }
        constexpr bool operator==(uint8_t lhs, Axis rhs){
            return lhs == static_cast<std::underlying_type_t<Axis>>(rhs);
        }
        constexpr bool operator==(Axis lhs, uint8_t rhs){
            return static_cast<std::underlying_type_t<Axis>>(lhs) == rhs;
        }

        template<bool Positive>
        struct XDir{
            static constexpr Axis axis = X;
            static constexpr bool sign = Positive;
        };
        template<bool Positive>
        struct YDir{
            static constexpr Axis axis = Y;
            static constexpr bool sign = Positive;
        };
        template<bool Positive>
        struct ZDir{
            static constexpr Axis axis = Z;
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
    
        static constexpr Direction::Axis f_axis = ForwardDir::axis;
        static constexpr Direction::Axis r_axis = RightDir::axis;
        static constexpr Direction::Axis u_axis = UpDir::axis;

        static constexpr Vector<F, 3> unitForwardVector = Vector<F, 3>(F(1) * (f_axis == Direction::Axis::X), F(1) * (f_axis == Direction::Axis::Y), F(1) * (f_axis == Direction::Axis::Z));  
        static constexpr Vector<F, 3> unitUpVector = Vector<F, 3>(F(1)* (u_axis == Direction::Axis::X), F(1) * (u_axis == Direction::Axis::Y), F(1) * (u_axis == Direction::Axis::Z));
        static constexpr Vector<F, 3> unitRightVector = Vector<F, 3>(F(1) * (r_axis == Direction::Axis::X), F(1) * (r_axis == Direction::Axis::Y), F(1) * (r_axis == Direction::Axis::Z));
    };

    template<typename T>
    struct IsCoordinateSystem : std::false_type {};

    template<typename ForwardDir, typename RightDir, typename UpDir, typename F>
    struct IsCoordinateSystem<CoordinateSystem<ForwardDir, RightDir, UpDir, F>> : std::true_type {};

} //namespace lab