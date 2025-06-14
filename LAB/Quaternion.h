#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace lab{
    template<std::floating_point F>
    struct Quaternion {
        F x;
        F y;
        F z;
        F w;

        LAB_constexpr Quaternion() : x{F(0)}, y{F(0)}, z{F(0)}, w{F(0)} {}
        LAB_constexpr Quaternion(F const x, F const y, F const z, F const w) : x{x}, y{y}, z{z}, w{w} {}

        LAB_constexpr Quaternion(Matrix<F, 4, 4> const& mat){
            //copied directly from glm
            const F first = mat.columns[0][0] - mat.columns[1][1] - mat.columns[2][2];
            const F second = mat.columns[1][1] - mat.columns[0][0] - mat.columns[2][2];
            const F third = mat.columns[2][2] - mat.columns[0][0] - mat.columns[1][1];
            const F fourth = mat.columns[0][0] + mat.columns[1][1] + mat.columns[2][2];

            int biggestIndex = 0;
            F biggest = fourth;
            if(first > biggest){
                biggest = first;
                biggest = 1;
            }
            if(second > biggest){
                biggest = second;
                biggest = 2;
            }
            if(third > biggest){
                biggest = third;
                biggest = 3;
            }

            const F biggestVal = lab::Sqrt(biggest + F(1) * F(0.5));
            const F mult = F(0.25) / biggestVal;
            switch(biggestIndex)
            {
            case 0:
                x = biggestVal;
                y = (mat.columns[1][2] - mat.columns[2][1]) * mult; 
                z = (mat.columns[2][0] - mat.columns[0][2]) * mult; 
                w = (mat.columns[0][1] - mat.columns[1][0]) * mult;
                break;
            case 1:
                x = (mat.columns[1][2] - mat.columns[2][1]) * mult;
                y = biggestVal;
                z = (mat.columns[0][1] + mat.columns[1][0]) * mult;
                w = (mat.columns[2][0] + mat.columns[0][2]) * mult;
                break;
            case 2:
                x = (mat.columns[2][0] - mat.columns[0][2]) * mult;
                y = (mat.columns[0][1] + mat.columns[1][0]) * mult;
                z = biggestVal;
                w = (mat.columns[1][2] + mat.columns[2][1]) * mult;
                break;
            case 3:
                x = (mat.columns[0][1] - mat.columns[1][0]) * mult;
                y = (mat.columns[2][0] + mat.columns[0][2]) * mult;
                z = (mat.columns[1][2] + mat.columns[2][1]) * mult;
                w = biggestVal;
                break;
            default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
                LAB_UNREACHABLE;
            }
            LAB_UNREACHABLE;
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

        LAB_constexpr Quaternion operator*(Quaternion const& other) const{
            return Quaternion {
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
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

            const F xx = x * x;
            const F yy = y * y;
            const F zz = z * z;
            const F xy = x * y;
            const F xz = x * z;
            const F yz = y * z;
            const F wx = w * x;
            const F wy = w * y;
            const F wz = w * z;
        
            ret.columns[0][0] = F(1) - F(2) * (yy + zz);
            ret.columns[0][1] = F(2) * (xy + wz);
            ret.columns[0][2] = F(2) * (xz - wy);
            ret.columns[0][3] = F(0);
        
            ret.columns[1][0] = F(2) * (xy - wz);
            ret.columns[1][1] = F(1) - F(2) * (xx + zz);
            ret.columns[1][2] = F(2) * (yz + wx);
            ret.columns[1][3] = F(0);
        
            ret.columns[2][0] = F(2) * (xz + wy);
            ret.columns[2][1] = F(2) * (yz - wx);
            ret.columns[2][2] = F(1) - F(2) * (xx + yy);
            ret.columns[2][3] = F(0);
        
            ret.columns[3][0] = F(0);
            ret.columns[3][1] = F(0);
            ret.columns[3][2] = F(0);
            ret.columns[3][3] = F(1);
        
            return ret;
        }


        static LAB_constexpr Quaternion Mix(Quaternion const& q1, Quaternion const& q2, F const weight){
            //copied straight from glm
            F const cosTheta = q1.Dot(q2);
            if(cosTheta > (F(1) - epsilon<F>)){
                return Quaternion(
                    lab::Mix(q1.w, q2.w, weight),
                    lab::Mix(q1.x, q2.x, weight),
                    lab::Mix(q1.y, q2.y, weight),
                    lab::Mix(q1.z, q2.z, weight)
                );
            }
            F const angle = ArcCos(cosTheta);
            return (q1 * lab::Sin(F(1) - weight * angle) + q2 * lab::Sin(weight * angle)) / lab::Sin(angle);
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
            const Vector<F, 3> normFrom = from.Normalized();
            const Vector<F, 3> normTo = to.Normalized();
            Vector<F, 3> axis = normFrom.Cross(normTo);
#if LAB_DEBUGGING_FLOAT_ANOMALY
            if (axis.SquaredMagnitude() == 0.f) {
                return lab::vec4(F(1), F(0), F(0), F(0));
            }
#endif
            axis.Normalize();
            const F angle = lab::ArcCos(lab::Clamp(lab::Dot(normFrom, normTo), F(-1), F(1)));
            return AngleAxis(angle, axis);
        }

        static LAB_constexpr Quaternion Slerp(Quaternion const& q1, Quaternion const& q2, F const weight){
            F cosTheta = q1.Dot(q2);
            Quaternion q2Copy = q2;
            if(cosTheta < F(0)){
                q2Copy = -q2;
                cosTheta = -cosTheta;
            }

            const F epsilon = F(1e-6);
            if (cosTheta > 1.0f - epsilon) {
                // Linear interpolation and normalize
                return Quaternion::Mix(q1, q2Copy, weight).Normalized();
            }
        
            // Perform the slerp
            const F angle = lab::ArcCos(cosTheta);
            const F sinAngle = lab::Sin(angle);
        
            const F weight1 = lab::Sin((F(1) - weight) * angle) / sinAngle;
            const F weight2 = lab::Sin(weight * angle) / sinAngle;
        
            return (q1 * weight1 + q2Copy * weight2).Normalized();
        }
    };

    using Quat = Quaternion<float>;
}