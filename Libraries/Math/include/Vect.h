#ifndef VECT_H
#define VECT_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include "Constants.h"
#include "Align16.h"

namespace EngineSpace
{
	//Forward declarations
	class Matrix;
	class Quat;

	class Vect final : public Align16
	{
	public:
		Vect();
		Vect(const Vect& CopyVect) = default;
		Vect& operator = (const Vect & AssignVect) = default;
		~Vect() = default;

		Vect(const float nx, const float ny, const float nz, const float nw = 1.0f);
		Vect(const Quat temp);

		void set(const float nx, const float ny, const float nz, const float nw = 1.0f);
		void set(const Vect newVect);
		float dot(const Vect RHS) const;
		Vect cross(const Vect RHS) const;
		void norm();
		Vect getNorm();
		float mag() const;
		float magSqr() const;
		float getAngle(Vect RHS) const;
		bool isEqual(const Vect EqualCheck, const float Tolerance = 0.0f);
		bool isZero(const float Tolerance = 0.0f);

		float& x();
		float x() const;
		float& y();
		float y() const;
		float& z();
		float z() const;
		float& w();
		float w() const;

		Vect operator+() const;
		Vect operator-() const;
		Vect operator * (const float scale) const;
		Vect operator *= (const float scale);
		Vect operator +(const Vect AddVect) const;
		Vect operator +=(const Vect AddVect);
		Vect operator -(const Vect SubVect) const;
		Vect operator -=(const Vect SubVect);
		Vect operator * (const Matrix m) const;
		Vect operator *= (const Matrix m);
		friend Vect operator * (float scale, const Vect & newVect);

		float& operator [](const x_enum);
		float operator [](const x_enum) const;
		float& operator [](const y_enum);
		float operator [](const y_enum) const;
		float& operator [](const z_enum);
		float operator [](const z_enum) const;
		float& operator [](const w_enum);
		float operator [](const w_enum) const;

	private:

		union
		{
			__m128	_mv;

			// anonymous struct
			struct
			{
				float _vx;
				float _vy;
				float _vz;
				float _vw;
			};
		};
	};
}


#endif
