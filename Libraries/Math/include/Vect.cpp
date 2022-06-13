#include "MathEngine.h"

namespace EngineSpace
{

    Vect::Vect()
        :_vx(0.0f), _vy(0.0f), _vz(0.0f), _vw(1.0f)
    {

    }

    Vect::Vect(const float nx, const float ny, const float nz, const float nw)
        : _vx(nx), _vy(ny), _vz(nz), _vw(nw)
    {

    }

    Vect::Vect(const Quat temp)
        : _vx(temp.qx()), _vy(temp.qy()), _vz(temp.qz()), _vw(1.0f)
    {

    }

    float& Vect::x()
    {
        return this->_vx;
    }

    float Vect::x() const
    {
        return this->_vx;
    }

    float& Vect::y()
    {
        return this->_vy;
    }

    float Vect::y() const
    {
        return this->_vy;
    }

    float& Vect::z()
    {
        return this->_vz;
    }

    float Vect::z() const
    {
        return this->_vz;
    }

    float& Vect::w()
    {
        return this->_vw;
    }

    float Vect::w() const
    {
        return this->_vw;
    }

    Vect Vect::operator+() const
    {
        return Vect(this->_vx, this->_vy, this->_vz);
    }

    Vect Vect::operator-() const
    {
        return Vect(-this->_vx, -this->_vy, -this->_vz);
    }

    float& Vect::operator[](const x_enum)
    {
        return this->_vx;
    }

    float Vect::operator[](const x_enum) const
    {
        return this->_vx;
    }

    float& Vect::operator[](const y_enum)
    {
        return this->_vy;
    }

    float Vect::operator[](const y_enum) const
    {
        return this->_vy;
    }

    float& Vect::operator[](const z_enum)
    {
        return this->_vz;
    }

    float Vect::operator[](const z_enum) const
    {
        return this->_vz;
    }

    float& Vect::operator[](const w_enum)
    {
        return this->_vw;
    }

    float Vect::operator[](const w_enum) const
    {
        return this->_vw;
    }

    Vect Vect::operator*(const float scale) const
    {
        return Vect(this->_vx * scale, this->_vy * scale, this->_vz * scale);
    }

    Vect Vect::operator*=(const float scale) 
    {
        this->_vx *= scale;
        this->_vy *= scale;
        this->_vz *= scale;
        this->_vw = 1.0f;
        return *this;
    }

    Vect Vect::operator+(const Vect AddVect) const
    {
        return(Vect(this->_vx + AddVect._vx, this->_vy + AddVect._vy, this->_vz + AddVect._vz));
    }

    Vect Vect::operator+=(const Vect AddVect) 
    {
        this->_vx += AddVect._vx;
        this->_vy += AddVect._vy;
        this->_vz += AddVect._vz;
        this->_vw = 1.0f;

        return *this;
    }

    Vect Vect::operator-(const Vect SubVect) const
    {
        return(Vect(this->_vx - SubVect._vx, this->_vy - SubVect._vy, this->_vz - SubVect._vz));
    }

    Vect Vect::operator-=(const Vect SubVect)
    {
        this->_vx -= SubVect._vx;
        this->_vy -= SubVect._vy;
        this->_vz -= SubVect._vz;
        this->_vw = 1.0f;

        return *this;
    }

    Vect Vect::operator*(const Matrix m) const
    {
        return Vect((this->_vx * m[m0] + this->_vy * m[m4] + this->_vz * m[m8] + this->_vw * m[m12]),
            (this->_vx * m[m1] + this->_vy * m[m5] + this->_vz * m[m9] + this->_vw * m[m13]),
            (this->_vx * m[m2] + this->_vy * m[m6] + this->_vz * m[m10] + this->_vw * m[m14]),
            (this->_vx * m[m3] + this->_vy * m[m7] + this->_vz * m[m11] + this->_vw * m[m15]));
    }

    Vect Vect::operator*=(const Matrix m)
    {
        Vect Mult = Vect(*this);
        this->_vx = Mult._vx * m[m0] + Mult._vy * m[m4] + Mult._vz * m[m8] + Mult._vw * m[m12];
        this->_vy = Mult._vx * m[m1] + Mult._vy * m[m5] + Mult._vz * m[m9] + Mult._vw * m[m13];
        this->_vz = Mult._vx * m[m2] + Mult._vy * m[m6] + Mult._vz * m[m10] + Mult._vw * m[m14];
        this->_vw = Mult._vx * m[m3] + Mult._vy * m[m7] + Mult._vz * m[m11] + Mult._vw * m[m15];
        return *this;
    }

    Vect operator * (float scale, const Vect& newVect)
    {
        return Vect(newVect._vx * scale, newVect._vy * scale, newVect._vz * scale);
    }



    void Vect::set(const float nx, const float ny, const float nz, const float nw)
    {
        this->_mv = _mm_set_ps(nw, nz, ny, nx);
    }

    void Vect::set(const Vect newVect)
    {
        this->_mv = newVect._mv;
    }

    float Vect::dot(const Vect RHS) const
    {
        return(this->_vx * RHS._vx + this->_vy * RHS._vy + this->_vz * RHS._vz);
    }

    Vect Vect::cross(const Vect RHS) const
    {
        return Vect((this->_vy * RHS._vz - this->_vz * RHS._vy), -(this->_vx * RHS._vz - this->_vz * RHS._vx), (this->_vx * RHS._vy - this->_vy * RHS._vx));
    }

    void Vect::norm()
    {
        float norm = 1 / this->mag();
        this->_vx *= norm;
        this->_vy *= norm;
        this->_vz *= norm;
    }

    Vect Vect::getNorm()
    {
        float norm = 1 / this->mag();
        return(Vect(this->_vx * norm, this->_vy * norm, this->_vz * norm));
    }

    float Vect::mag() const
    {
        return(Trig::sqrt((this->_vx * this->_vx) + (this->_vy * this->_vy) + (this->_vz * this->_vz)));
    }

    float Vect::magSqr() const
    {
        return((this->_vx * this->_vx) + (this->_vy * this->_vy) + (this->_vz * this->_vz));
    }

    float Vect::getAngle(Vect RHS) const
    {
        return(Trig::acos(this->dot(RHS) / (this->mag() * RHS.mag())));
    }

    bool Vect::isEqual(const Vect EqualCheck, const float Tolerance)
    {
        bool Equal = true;

        if (Trig::abs(this->_vx - EqualCheck._vx) > Tolerance)
        {
            Equal = false;
        }
        if (Trig::abs(this->_vy - EqualCheck._vy) > Tolerance)
        {
            Equal = false;
        }
        if (Trig::abs(this->_vz - EqualCheck._vz) > Tolerance)
        {
            Equal = false;
        }

        return Equal;
    }

    bool Vect::isZero(const float Tolerance)
    {
        bool Zero = true;

        if (Trig::abs(this->_vx) > Tolerance)
        {
            Zero = false;
        }
        if (Trig::abs(this->_vy) > Tolerance)
        {
            Zero = false;
        }
        if (Trig::abs(this->_vz) > Tolerance)
        {
            Zero = false;
        }
        if (Trig::abs(this->_vw) > (1.0f + Tolerance))
        {
            Zero = false;
        }

        return Zero;
    }
}