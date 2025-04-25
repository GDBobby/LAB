#pragma once
#include "CoordinateSystems.h"
#include <type_traits>

namespace lab {
    namespace CS_Helper {
    
        template<Direction::Axis axis, bool Sign>
        struct MakeDir;
        
        template<bool Sign> struct MakeDir<Direction::X, Sign> { using type = Direction::XDir<Sign>; };
        template<bool Sign> struct MakeDir<Direction::Y, Sign> { using type = Direction::YDir<Sign>; };
        template<bool Sign> struct MakeDir<Direction::Z, Sign> { using type = Direction::ZDir<Sign>; };
        
        template<Direction::Axis Axis, bool Sign>
        using MakeDir_t = typename MakeDir<Axis, Sign>::type;
    
        using DefaultCS = CoordinateSystem<Direction::XDir<true>, Direction::YDir<true>, Direction::ZDir<true>>;
    } // namespace CS_Helper

    template<typename Func, typename... Args>
    requires(std::is_invocable_v<Func, DefaultCS, Args...>)
    void DispatchCSFunction(
        Direction::Axis forwardAxis, bool forwardSign,
        Direction::Axis rightAxis, bool rightSign,
        Direction::Axis upAxis, bool upSign,
        Func&& func, Args&&... args)
    {
        if (forwardAxis == rightAxis || rightAxis == upAxis || forwardAxis == upAxis){
            //potentially use a default CS
            return;
        }
        else{
            using ForwardDir = CS_Helper::MakeDir_t<forwardAxis, forwardSign>;
            using RightDir   = CS_Helper::MakeDir_t<rightAxis,   rightSign>;
            using UpDir      = CS_Helper::MakeDir_t<upAxis,      upSign>;
            
            func.template operator()<CoordinateSystem<ForwardDir, RightDir, UpDir>>(std::forward<Args>(args)...);
        }
    }
} // namespace lab