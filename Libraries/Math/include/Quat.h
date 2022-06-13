#ifndef QUAT_H
#define QUAT_H

#include "Matrix.h"

namespace EngineSpace
{
	class Quat final : public Align16
	{
	public:
		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Special
		{
			Zero,
			Identity
		};

		enum class Rot
		{
			AxisAngle,
		};

		enum class Orient
		{
			LocalToWorld,
			WorldToLocal
		};

		enum class Rot3
		{
			XYZ
		};

	public:
		Quat();
		Quat(const Quat& CopyQuat) = default;
		Quat& operator = (const Quat & AssignQuat) = default;
		~Quat() = default;

		Quat(const float nx, const float ny, const float nz, const float nw = 1.0f);
		Quat(Vect temp, float nw);
		Quat(Matrix temp);
		Quat(Special type);
		Quat(Rot1 RotType, float angle);
		Quat(Rot3 type, float x, float y, float z);
		Quat(Rot type, Vect v, float angle);
		Quat(Orient type, Vect Forward, Vect Up);

		void set(const float nx, const float ny, const float nz, const float nw = 1.0f);
		void set(const Quat newQuat);
		void set(Rot3 type, float x, float y, float z);
		void set(Rot1 type, float angle);
		void set(Rot type, Vect v, float angle);
		void set(Vect temp, float nw);
		void set(Special type);
		void set(Orient type, Vect Forward, Vect Up);


		// Do your magic here
		float& qx();
		float qx() const;
		float& qy();
		float qy() const;
		float& qz();
		float qz() const;
		float& real();
		float real() const;

		float& operator [](const x_enum);
		float operator [](const x_enum) const;
		float& operator [](const y_enum);
		float operator [](const y_enum) const;
		float& operator [](const z_enum);
		float operator [](const z_enum) const;
		float& operator [](const w_enum);
		float operator [](const w_enum) const;

		Quat operator+();
		Quat operator-();
		Quat operator +(const Quat AddQuat);
		Quat operator +=(const Quat AddQuat);
		Quat operator -(const Quat SubQuat);
		Quat operator -=(const Quat SubQuat);
		Quat operator +(const float Adder);
		friend Quat operator + (const float Adder, const Quat & newQuat);
		Quat operator +=(const float Adder);
		Quat operator -(const float Sub);
		friend Quat operator - (const float Sub, const Quat & newQuat);
		Quat operator -=(const float Sub);
		Matrix operator * (const Matrix m);
		Matrix operator *= (const Matrix m);
		Quat operator*(const Quat MultiQuat);
		Quat operator*=(const Quat MultiQuat);
		Quat operator *(const float Scale);
		friend Quat operator * (const float Scale, const Quat & newQuat);
		Quat operator *=(const float Scale);
		Quat operator/ (const Quat DivQuat);
		Quat operator/= (const Quat DivQuat);
		Quat operator/ (const float div);
		Quat operator/= (const float div);
		friend Quat operator / (const float div, const Quat & newQuat);

		Quat norm();
		Quat getNorm();
		void T();
		Quat getT();
		void conj();
		Quat getConj();
		void inv();
		Quat getInv();
		float mag();
		float magSquared();
		float invMag();
		bool isNormalized(const float dev);
		bool isEqual(const Quat temp, const float dev);
		bool isNegEqual(const Quat temp, const float dev);
		bool isEquivalent(const Quat temp, const float dev);
		bool isConjugateEqual(const Quat temp, const float dev);
		bool isIdentity(float dev);
		bool isZero(float dev);
		Quat multByElement(const Quat temp);
		float dot(const Quat temp);
		void Lqcvq(Vect in, Vect & out);
		void Lqvqc(Vect in, Vect & out);
		float getAngle();
		void getAxis(Vect & out);
		void getVect(Vect & out);
		void setVect(Vect temp);

	private:

		// anonymous union
		union
		{
			__m128	_mq;

			// anonymous struct
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				float _qw;
			};
		};

	};
}

#endif