#ifndef MATRIX_H
#define MATRIX_H

#include "Vect.h"

namespace EngineSpace
{
	//Forward declaration
	class Quat;

	class Matrix final : public Align16
	{
	public:

		enum class Special
		{
			Zero,
			Identity
		};

		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Trans
		{
			XYZ
		};

		enum class Scale
		{
			XYZ
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

		enum class Row
		{
			i0,
			i1,
			i2,
			i3
		};

		enum class Rot3
		{
			XYZ
		};

	public:

		Matrix();
		Matrix(const Matrix& CopyVect);
		Matrix& operator = (const Matrix& AssignVect) = default;
		~Matrix() = default;

		Matrix(Vect v0, Vect v1, Vect v2, Vect v3);
		Matrix(const Rot, Vect axis, const float angle);
		Matrix(Special Type);
		Matrix(Trans, float x, float y, float z);
		Matrix(Trans, Vect XYZ);
		Matrix(Scale, float x, float y, float z);
		Matrix(Scale, Vect vScale);
		Matrix(Rot1 Axis, float angle);
		Matrix(Orient Type, Vect Forward, Vect Up);
		Matrix(Rot3, float xAngle, float yAngle, float zAngle);
		Matrix(Quat temp);

		Matrix operator+();
		Matrix operator-();
		Matrix operator+(const Matrix AddM);
		Matrix operator+=(const Matrix AddM);
		Matrix operator-(const Matrix SubM);
		Matrix operator-=(const Matrix SubM);
		Matrix operator*(const float scale);
		Matrix operator*=(const float scale);
		Matrix operator*(const Matrix MMult);
		Matrix operator*=(const Matrix MMult);
		Matrix operator* (const Quat q);
		friend Matrix operator * (float scale, const Matrix& RHs);

		void set(Special Type);
		void set(Trans, float x, float y, float z);
		void set(Trans, Vect vTrans);
		void set(Scale, float x, float y, float z);
		void set(Scale, Vect vScale);
		void set(Rot1 Axis, float angle);
		void set(Row RowNum, const Vect newRow);
		void set(Rot3, float xAngle, float yAngle, float zAngle);
		void set(Orient Type, Vect Forward, Vect Up);
		void set(Rot, Vect Axis, float angle);
		void set(Vect v0, Vect v1, Vect v2, Vect v3);
		void set(Quat temp);
		Vect get(Row RowNum);

		bool isEqual(Matrix RHS);
		bool isIdentity(float Tolerance = 0.0f);
		bool isRotation();
		float det();
		void T();
		Matrix getT();
		void inv();
		Matrix getInv();

		float& operator [](const m0_enum);
		float operator [](const m0_enum) const;
		float& operator [](const m1_enum);
		float operator [](const m1_enum) const;
		float& operator [](const m2_enum);
		float operator [](const m2_enum) const;
		float& operator [](const m3_enum);
		float operator [](const m3_enum) const;
		float& operator [](const m4_enum);
		float operator [](const m4_enum) const;
		float& operator [](const m5_enum);
		float operator [](const m5_enum) const;
		float& operator [](const m6_enum);
		float operator [](const m6_enum) const;
		float& operator [](const m7_enum);
		float operator [](const m7_enum) const;
		float& operator [](const m8_enum);
		float operator [](const m8_enum) const;
		float& operator [](const m9_enum);
		float operator [](const m9_enum) const;
		float& operator [](const m10_enum);
		float operator [](const m10_enum) const;
		float& operator [](const m11_enum);
		float operator [](const m11_enum) const;
		float& operator [](const m12_enum);
		float operator [](const m12_enum) const;
		float& operator [](const m13_enum);
		float operator [](const m13_enum) const;
		float& operator [](const m14_enum);
		float operator [](const m14_enum) const;
		float& operator [](const m15_enum);
		float operator [](const m15_enum) const;

		float& m0();
		float m0() const;
		float& m1();
		float m1() const;
		float& m2();
		float m2() const;
		float& m3();
		float m3() const;
		float& m4();
		float m4() const;
		float& m5();
		float m5() const;
		float& m6();
		float m6() const;
		float& m7();
		float m7() const;
		float& m8();
		float m8() const;
		float& m9();
		float m9() const;
		float& m10();
		float m10() const;
		float& m11();
		float m11() const;
		float& m12();
		float m12() const;
		float& m13();
		float m13() const;
		float& m14();
		float m14() const;
		float& m15();
		float m15() const;

	private:

		union
		{
			struct
			{
				Vect _v0;
				Vect _v1;
				Vect _v2;
				Vect _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};
}


#endif
