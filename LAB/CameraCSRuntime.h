#pragma once
#include "CoordinateSystems.h"
#include <type_traits>

#include "Matrix.h"

//this is for quickly testing runtimes, not recommended for deployment

namespace lab {
    namespace Runtime{
        struct CoordinateSystem{
            bool f_sign = true;
            bool r_sign = true;
            bool u_sign = true;
        
            Direction::Axis f_axis = Direction::X;
            Direction::Axis r_axis = Direction::Y;
            Direction::Axis u_axis = Direction::Z;

            bool Validate(){
                return f_axis != r_axis && f_axis != u_axis && u_axis != r_axis;
            }
        };
        
        template<std::floating_point F>
        Matrix<F, 4, 4> ViewDirection(CoordinateSystem const csR, Vector<F, 3> const position, Vector<F, 3> const forward, Vector<F, 3> const upDir = vec3(F(0), F(1), F(0))){
            Matrix<F, 4, 4> ret{};
            const Vector<float, 3> right = Cross(forward, upDir).Normalized();
            const Vector<float, 3> up = Cross(right, forward).Normalized();
            
            if (csR.f_sign){
                ret.columns[0][csR.f_axis] = -forward.x;
                ret.columns[1][csR.f_axis] = -forward.y;
                ret.columns[2][csR.f_axis] = -forward.z;
                ret.columns[3][csR.f_axis] = position.Dot(forward);
            }
            else{
                ret.columns[0][csR.f_axis] = forward.x;
                ret.columns[1][csR.f_axis] = forward.y;
                ret.columns[2][csR.f_axis] = forward.z;
                ret.columns[3][csR.f_axis] = -position.Dot(forward);
            }
            if (csR.u_sign) {
                ret.columns[0][csR.u_axis] = -up.x;
                ret.columns[1][csR.u_axis] = -up.y;
                ret.columns[2][csR.u_axis] = -up.z;
                ret.columns[3][csR.u_axis] = position.Dot(up);
            }
            else{
                ret.columns[0][csR.u_axis] = up.x;
                ret.columns[1][csR.u_axis] = up.y;
                ret.columns[2][csR.u_axis] = up.z;
                ret.columns[3][csR.u_axis] = -position.Dot(up);
            }
            if (csR.r_sign) {
                ret.columns[0][csR.r_axis] = -right.x;
                ret.columns[1][csR.r_axis] = -right.y;
                ret.columns[2][csR.r_axis] = -right.z;
                ret.columns[3][csR.r_axis] = position.Dot(right);
            }
            else{
                ret.columns[0][csR.r_axis] = right.x;
                ret.columns[1][csR.r_axis] = right.y;
                ret.columns[2][csR.r_axis] = right.z;
                ret.columns[3][csR.r_axis] = -position.Dot(right);
            }
            ret.columns[0][3] = F(0);
            ret.columns[1][3] = F(0);
            ret.columns[2][3] = F(0);
            ret.columns[3][3] = F(1);
            return ret;
        }

        //the final row needs to be set to 0,0,0,1 outside of this function
        template<std::floating_point F>
        void ViewDirection(CoordinateSystem const csR, Matrix<F, 4, 4>& viewMat, Vector<F, 3> const position, Vector<F, 3> const forward, Vector<F, 3> const upDir = vec3(F(0), F(1), F(0))){
            const Vector<F, 3> right = Cross(forward, upDir).Normalized();
            const Vector<F, 3> up = Cross(right, forward).Normalized();

            if (csR.f_sign){
                viewMat.columns[0][csR.f_axis] = -forward.x;
                viewMat.columns[1][csR.f_axis] = -forward.y;
                viewMat.columns[2][csR.f_axis] = -forward.z;
                viewMat.columns[3][csR.f_axis] = position.Dot(forward);
            }
            else{
                viewMat.columns[0][csR.f_axis] = forward.x;
                viewMat.columns[1][csR.f_axis] = forward.y;
                viewMat.columns[2][csR.f_axis] = forward.z;
                viewMat.columns[3][csR.f_axis] = -position.Dot(forward);
            }
            if (csR.u_sign) {
                viewMat.columns[0][csR.u_axis] = -up.x;
                viewMat.columns[1][csR.u_axis] = -up.y;
                viewMat.columns[2][csR.u_axis] = -up.z;
                viewMat.columns[3][csR.u_axis] = position.Dot(up);
            }
            else{
                viewMat.columns[0][csR.u_axis] = up.x;
                viewMat.columns[1][csR.u_axis] = up.y;
                viewMat.columns[2][csR.u_axis] = up.z;
                viewMat.columns[3][csR.u_axis] = -position.Dot(up);
            }
            if (csR.r_sign) {
                viewMat.columns[0][csR.r_axis] = -right.x;
                viewMat.columns[1][csR.r_axis] = -right.y;
                viewMat.columns[2][csR.r_axis] = -right.z;
                viewMat.columns[3][csR.r_axis] = position.Dot(right);
            }
            else{
                viewMat.columns[0][csR.r_axis] = right.x;
                viewMat.columns[1][csR.r_axis] = right.y;
                viewMat.columns[2][csR.r_axis] = right.z;
                viewMat.columns[3][csR.r_axis] = -position.Dot(right);
            }
    #if LAB_DEBUGGING_FLOAT_ANOMALY
            //just make it a warning since setting the final values later is fine
    #endif
        }

        template<std::floating_point F>
        Matrix<F, 4, 4> ViewRotation(CoordinateSystem const csR, lab::Vector<F, 3> const position, lab::Vector<F, 3> const rotation){
            lab::mat4 view{};
            
            const F c3 = lab::Cos(rotation.z);
            const F s3 = lab::Sin(rotation.z);
            const F c2 = lab::Cos(rotation.x);
            const F s2 = lab::Sin(rotation.x);
            const F c1 = lab::Cos(rotation.y);
            const F s1 = lab::Sin(rotation.y);
        
            const F fx = (c2 * s1);
            const F fy = (-s2);
            const F fz = (c1 * c2);
            if (csR.f_sign) {
                view.columns[0][csR.f_axis] = -fx;
                view.columns[1][csR.f_axis] = -fy;
                view.columns[2][csR.f_axis] = -fz;
                view.columns[3][csR.f_axis] = (fx * position.x + fy * position.y + fz * position.z);
            } else {
                view.columns[0][csR.f_axis] = fx;
                view.columns[1][csR.f_axis] = fy;
                view.columns[2][csR.f_axis] = fz;
                view.columns[3][csR.f_axis] = -(fx * position.x + fy * position.y + fz * position.z);
            }

            const F ux = (c3 * s1 * s2 - c1 * s3);
            const F uy = (c2 * c3);
            const F uz = (c1 * c3 * s2 + s1 * s3);
            if (csR.u_sign) {
                view.columns[0][csR.u_axis] = -ux;
                view.columns[1][csR.u_axis] = -uy;
                view.columns[2][csR.u_axis] = -uz;
                view.columns[3][csR.u_axis] = ux * position.x + uy * position.y + uz * position.z;
            } else {
                view.columns[0][csR.u_axis] = ux;
                view.columns[1][csR.u_axis] = uy;
                view.columns[2][csR.u_axis] = uz;
                view.columns[3][csR.u_axis] = -(ux * position.x + uy * position.y + uz * position.z);
            }

            const F rx = (c1 * c3 + s1 * s2 * s3);
            const F ry = (c2 * s3);
            const F rz = (c1 * s2 * s3 - c3 * s1);
            if (csR.r_sign) {
                view.columns[0][csR.r_axis] = -rx;
                view.columns[1][csR.r_axis] = -ry;
                view.columns[2][csR.r_axis] = -rz;
                view.columns[3][csR.r_axis] = rx * position.x + ry * position.y + rz * position.z;
            } else {
                view.columns[0][csR.r_axis] = rx;
                view.columns[1][csR.r_axis] = ry;
                view.columns[2][csR.r_axis] = rz;
                view.columns[3][csR.r_axis] = -(rx * position.x + ry * position.y + rz * position.z);
            }

            view.columns[0].w = F(0);
            view.columns[1].w = F(0);
            view.columns[2].w = F(0);
            view.columns[3].w = F(1);
            return view;
        }

        template<std::floating_point F>
        void ViewRotation(CoordinateSystem const csR, Matrix<F, 4, 4>& view, lab::Vector<F, 3> const position, lab::Vector<F, 3> const rotation) {
            const F c3 = lab::Cos(rotation.z);
            const F s3 = lab::Sin(rotation.z);
            const F c2 = lab::Cos(rotation.x);
            const F s2 = lab::Sin(rotation.x);
            const F c1 = lab::Cos(rotation.y);
            const F s1 = lab::Sin(rotation.y);
        
            const F fx = (c2 * s1);
            const F fy = (-s2);
            const F fz = (c1 * c2);
            if (csR.f_sign) {
                view.columns[0][csR.f_axis] = -fx;
                view.columns[1][csR.f_axis] = -fy;
                view.columns[2][csR.f_axis] = -fz;
                view.columns[3][csR.f_axis] = (fx * position.x + fy * position.y + fz * position.z);
            } else {
                view.columns[0][csR.f_axis] = fx;
                view.columns[1][csR.f_axis] = fy;
                view.columns[2][csR.f_axis] = fz;
                view.columns[3][csR.f_axis] = -(fx * position.x + fy * position.y + fz * position.z);
            }

            const F ux = (c3 * s1 * s2 - c1 * s3);
            const F uy = (c2 * c3);
            const F uz = (c1 * c3 * s2 + s1 * s3);
            if (csR.u_sign) {
                view.columns[0][csR.u_axis] = -ux;
                view.columns[1][csR.u_axis] = -uy;
                view.columns[2][csR.u_axis] = -uz;
                view.columns[3][csR.u_axis] = ux * position.x + uy * position.y + uz * position.z;
            } else {
                view.columns[0][csR.u_axis] = ux;
                view.columns[1][csR.u_axis] = uy;
                view.columns[2][csR.u_axis] = uz;
                view.columns[3][csR.u_axis] = -(ux * position.x + uy * position.y + uz * position.z);
            }

            const F rx = (c1 * c3 + s1 * s2 * s3);
            const F ry = (c2 * s3);
            const F rz = (c1 * s2 * s3 - c3 * s1);
            if (csR.r_sign) {
                view.columns[0][csR.r_axis] = -rx;
                view.columns[1][csR.r_axis] = -ry;
                view.columns[2][csR.r_axis] = -rz;
                view.columns[3][csR.r_axis] = rx * position.x + ry * position.y + rz * position.z;
            } else {
                view.columns[0][csR.r_axis] = rx;
                view.columns[1][csR.r_axis] = ry;
                view.columns[2][csR.r_axis] = rz;
                view.columns[3][csR.r_axis] = -(rx * position.x + ry * position.y + rz * position.z);
            }
        }


        template<std::floating_point F>
        Matrix<F, 4, 4> ViewTarget(CoordinateSystem const csR, lab::Vector<F, 3> const position, lab::Vector<F, 3> target, Vector<F, 3> const up = vec3(F(0), F(1), F(0))){
    #if LAB_DEBUGGING_FLOAT_ANOMALIES
            //warning for target == position
    #endif
            const lab::Vector<F, 3> direction = (target - position).Normalize();
            return ViewDirection(position, direction, up);
        }
        
        template<std::floating_point F>
        void ViewTarget(CoordinateSystem const csR, Matrix<F, 4, 4>& viewMat, lab::Vector<F, 3> const position, lab::Vector<F, 3> target, Vector<F, 3> const up = vec3(F(0), F(1), F(0))){
    #if LAB_DEBUGGING_FLOAT_ANOMALIES
            //warning for target == position
    #endif
            const lab::Vector<F, 3> direction = (target - position).Normalize();
            ViewDirection(viewMat, position, direction, up);
        }
    } //namespace Runtime
} // namespace lab