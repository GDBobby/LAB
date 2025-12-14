#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace lab{
    //this is row major and left handed, while the rest of the lib is right handed and column major. idk when I'll fix it


    template<std::floating_point F>
    struct Quaternion {
        F x;
        F y;
        F z;
        F w;

        LAB_constexpr Quaternion() : x{F(0)}, y{F(0)}, z{F(0)}, w{F(0)} {}
        LAB_constexpr Quaternion(F const x, F const y, F const z, F const w) : x{x}, y{y}, z{z}, w{w} {}

        LAB_constexpr Quaternion(const Matrix<F, 4, 4>& matrix) {
            const F trace = matrix.At(0, 0) + matrix.At(1, 1) + matrix.At(2, 2) + F(1);

            if (trace > F(1)) {
                const F s = F(2) * sqrt(trace);
                x = (matrix.At(1, 2) - matrix.At(2, 1)) / s;
                y = (matrix.At(2, 0) - matrix.At(0, 2)) / s;
                z = (matrix.At(0, 1) - matrix.At(1, 0)) / s;
                w = F(0.25) * s;
            }
            else {
                int maxi = 0;
                if (matrix.At(1, 1) > matrix.At(maxi, maxi)) maxi = 1;
                if (matrix.At(2, 2) > matrix.At(maxi, maxi)) maxi = 2;

                switch (maxi) {
                case 0: {
                    const F s = F(2) * sqrt(F(1) + matrix.At(0, 0) - matrix.At(1, 1) - matrix.At(2, 2));
                    x = F(0.25) * s;
                    y = (matrix.At(0, 1) + matrix.At(1, 0)) / s;
                    z = (matrix.At(0, 2) + matrix.At(2, 0)) / s;
                    w = (matrix.At(1, 2) - matrix.At(2, 1)) / s;
                    break;
                }
                case 1: {
                    const F s = F(2) * sqrt(F(1) + matrix.At(1, 1) - matrix.At(0, 0) - matrix.At(2, 2));
                    x = (matrix.At(0, 1) + matrix.At(1, 0)) / s;
                    y = F(0.25) * s;
                    z = (matrix.At(1, 2) + matrix.At(2, 1)) / s;
                    w = (matrix.At(2, 0) - matrix.At(0, 2)) / s;
                    break;
                }
                default: {
                    const F s = F(2) * sqrt(F(1) + matrix.At(2, 2) - matrix.At(0, 0) - matrix.At(1, 1));
                    x = (matrix.At(0, 2) + matrix.At(2, 0)) / s;
                    y = (matrix.At(1, 2) + matrix.At(2, 1)) / s;
                    z = F(0.25) * s;
                    w = (matrix.At(0, 1) - matrix.At(1, 0)) / s;
                    break;
                }
                }
            }
        }

		static LAB_constexpr Quaternion FromMatrix(Matrix<F, 4, 4> const& matrix) {
			return Quaternion(matrix);
		}

        LAB_constexpr F SquaredMagnitude() const{
            return x * x + y * y + z * z + w * w;
        }
        LAB_constexpr F Magnitude() const{
            return lab::Sqrt(SquaredMagnitude());
        }
        LAB_constexpr void Normalize(){
            const F magnitude = Magnitude();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            w /= magnitude;
        }
        LAB_constexpr Quaternion Normalized() const{
            const F magnitude = Magnitude();
            return {
                x / magnitude,
                y / magnitude,
                z / magnitude,
                w / magnitude
            };
        }

        LAB_constexpr F Dot(Quaternion const& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        LAB_constexpr Quaternion operator+(Quaternion const& other) const {
            return Quaternion{
                x + other.x,
                y + other.y,
                z + other.z,
                w + other.w
            };
        }
        LAB_constexpr Quaternion operator*(F const multiplier) const {
            return Quaternion(
                x * multiplier,
                y * multiplier,
                z * multiplier,
                w * multiplier
            );
        }
        LAB_constexpr Quaternion& operator*=(F const multiplier){
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
            w *= multiplier;
        }
        LAB_constexpr Quaternion operator/(F const dividend) const {
            return Quaternion(
                x / dividend,
                y / dividend,
                z / dividend,
                w / dividend
            );
        }
		LAB_constexpr Quaternion& operator/=(F const dividend) {
			x /= dividend;
			y /= dividend;
			z /= dividend;
			w /= dividend;
		}

        //cml copy
        LAB_constexpr Quaternion operator*(Quaternion const& other) const{
            return Quaternion {

                (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y),
                (other.w * y) - (other.x * z) + (other.y * w) + (other.z * x),
                (other.w * z) + (other.x * y) - (other.y * x) + (other.z * w),
                (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z)
            };
        }
        
        LAB_constexpr Quaternion operator-() const {
            return Quaternion{
                -x,
                -y,
                -z,
                -w
            };
        }

        
        LAB_constexpr Matrix<F, 4, 4> ToMat4() const {
            Matrix<F, 4, 4> ret{};
            const Quaternion reflected = this->operator-();

            const F xx = F(2) * reflected.x * reflected.x;
            const F yy = F(2) * reflected.y * reflected.y;
            const F zz = F(2) * reflected.z * reflected.z;
            const F xy = F(2) * reflected.x * reflected.y;
            const F xz = F(2) * reflected.x * reflected.z;
            const F yz = F(2) * reflected.y * reflected.z;
            const F wx = F(2) * reflected.w * reflected.x;
            const F wy = F(2) * reflected.w * reflected.y;
            const F wz = F(2) * reflected.w * reflected.z;
        
            ret.columns[0].component[0] = F(1) - (yy + zz);
            ret.columns[0].component[1] = (xy + wz);
            ret.columns[0].component[2] = (xz - wy);
            ret.columns[0].component[3] = F(0);
        
            ret.columns[1].component[0] = (xy - wz);
            ret.columns[1].component[1] = F(1) - (xx + zz);
            ret.columns[1].component[2] = (yz + wx);
            ret.columns[1].component[3] = F(0);
        
            ret.columns[2].component[0] = (xz + wy);
            ret.columns[2].component[1] = (yz - wx);
            ret.columns[2].component[2] = F(1) - (xx + yy);
            ret.columns[2].component[3] = F(0);
        
            ret.columns[3].component[0] = F(0);
            ret.columns[3].component[1] = F(0);
            ret.columns[3].component[2] = F(0);
            ret.columns[3].component[3] = F(1);
        
            return ret;
        }
            

		static LAB_constexpr Matrix<F, 4, 4> ToMat4(Quaternion const& quat) {
			return quat.ToMat4();
		}
        //cml copy
        
        static LAB_constexpr Quaternion Mix(Quaternion q1, Quaternion q2, F const weight) {
            if (weight <= F(0)) {
                return q1;
            }
            if (weight >= F(1)) {
                return q2;
            }
            F angle = q1.Dot(q2);
            if (angle < F(0.0)) {
                q1 = -q1;
                angle = -angle;
            }

            if ((angle + F(1.0)) > F(0.05))  {
                if ((F(1.0) - angle) >= F(0.05)) {  // spherical interpolation
                    const F theta = lab::ArcCos(angle);
                    const F invsintheta = F(1.0) / lab::Sin(theta);
                    const F scale = lab::Sin(theta * (F(1.0)-weight)) * invsintheta;
                    const F invscale = lab::Sin(theta * weight) * invsintheta;
                    return (q1*scale) + (q2*invscale);
                }
                else { // linear interploation
                    const F invWeight = F(1.0) - weight;
                    return (q1 * invWeight) + (q2 * weight);
                }
            }
            else  {
                q2 = Quaternion(-q1.y, q1.x, -q1.w, q1.z);
                const F scale = lab::Sin(lab::GetPI(F(0.5) - weight));
                const F invscale = lab::Sin(lab::GetPI(weight));
                return (q1*scale) + (q2*invscale);
            }

        }
        //angle in radians
        static LAB_constexpr Quaternion AngleAxis(const F angle, Vector<F, 3> const axis){
                // Normalize axis to avoid distortion
            const Vector<F, 3> normAxis = axis.Normalized();

            const F halfAngle = angle * 0.5f;
            const F sinHalf = lab::Sin(halfAngle);
            const F cosHalf = lab::Cos(halfAngle);

            return Quaternion{
                normAxis.x * sinHalf,
                normAxis.y * sinHalf,
                normAxis.z * sinHalf,
                cosHalf
            };
        }
        static LAB_constexpr Matrix<F, 4, 4> ToMat4(Quaternion const quat){
            return quat.ToMat4();
        }   
        static LAB_constexpr Quaternion AxisToAxis(Vector<F, 3> const from, Vector<F, 3> const to){
            Vector<F, 3> vA = from.Normalized();
            Vector<F, 3> vHalf = (vA + to.Normalized()).Normalized();
            Vector<F, 3> vAxis = vA.Cross(vHalf);
            return Quaternion(
                vAxis.x,
                vAxis.y,
                vAxis.z,
                vA.Dot(vHalf)
            );
        }

        static LAB_constexpr Quaternion Slerp(Quaternion const& q1, Quaternion const& q2, F const weight) {
            return Mix(q1, q2, weight);
        }
    };

    using Quat = Quaternion<float>;
}