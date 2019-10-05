#include <iostream>
using namespace std;

typedef signed long long ll64;
typedef unsigned long long ull64;

class Int128
{
private:
	ull64 low;
	ll64 high;
public:
	Int128(ll64 _low = 0);
	Int128(const Int128& I) :low(I.low), high(I.high) {};
	Int128(const ll64& _high, const ull64& _low) :high(_high), low(_low) {};
	Int128& operator = (const Int128& I);
	friend bool operator == (const Int128& I1, const Int128& I2);
	friend bool operator != (const Int128& I1, const Int128& I2);
	friend bool operator < (const Int128& I1, const Int128& I2);
	friend bool operator > (const Int128& I1, const Int128& I2);
	friend bool operator <= (const Int128& I1, const Int128& I2);
	friend bool operator >= (const Int128& I1, const Int128& I2);
	friend Int128 operator + (const Int128& I1, const Int128& I2);
	friend Int128 operator - (const Int128& I1, const Int128& I2);
	Int128 operator-() const;
	operator double() const;
	Int128& operator += (const Int128& I);
	Int128& operator -= (const Int128& I);
	friend ostream& operator << (ostream& out, const Int128& I);
};

Int128::Int128(ll64 _low)
{
	low = _low;
	high = (_low < 0) ? -1 : 0;
}

Int128& Int128::operator=(const Int128& I)
{
	low = I.low;
	high = I.high;
	return *this;
}

Int128 Int128::operator-() const
{
	if (low == 0)
		return Int128(-high, 0);
	else
		// Complement
		return Int128(~high, ~low + 1);
}

Int128::operator double() const
{
	const double shift64 = 18446744073709551616.0; //2^64
	if (high < 0)
	{
		if (low == 0) return (double)high * shift64;
		else return -(double)(~low + ~high * shift64);
	}
	else
		return (double)(low + high * shift64);
}

Int128& Int128::operator+=(const Int128& I)
{
	*this = *this + I;
	return *this;
}

Int128& Int128::operator-=(const Int128& I)
{
	*this = *this - I;
	return *this;
}


bool operator==(const Int128& I1, const Int128& I2)
{
	return (I1.low == I2.low) && (I1.high == I2.high);
}

bool operator!=(const Int128& I1, const Int128& I2)
{
	return !(I1 == I2);
}

bool operator<(const Int128& I1, const Int128& I2)
{
	if (I1.high != I2.high)
		return I1.high < I2.high;
	return I1.low < I2.low;
}

bool operator>(const Int128& I1, const Int128& I2)
{
	if (I1.high != I2.high)
		return I1.high > I2.high;
	return I1.low > I2.low;
}

bool operator<=(const Int128& I1, const Int128& I2)
{
	return (I1 == I2) || (I1 < I2);
}

bool operator>=(const Int128& I1, const Int128& I2)
{
	return (I1 == I2) || (I1 > I2);
}

Int128 operator+(const Int128& I1, const Int128& I2)
{
	Int128 I;
	I.high = I1.high + I2.high;
	I.low = I1.low + I2.low;
	// Carry
	if (I.low < I1.low)
		I.high++;
	return I;
}

Int128 operator-(const Int128& I1, const Int128& I2)
{
	return I1 + (-I2);
}

ostream& operator<<(ostream& out, const Int128& I)
{
	out << I.high << I.low;
	return out;
}

Int128 Int128Mul(ll64 lhs, ll64 rhs)
{
	bool negate = (lhs < 0) != (rhs < 0);

	if (lhs < 0) lhs = -lhs;
	ull64 int1Hi = ull64(lhs) >> 32;
	ull64 int1Lo = ull64(lhs & 0xFFFFFFFF);

	if (rhs < 0) rhs = -rhs;
	ull64 int2Hi = ull64(rhs) >> 32;
	ull64 int2Lo = ull64(rhs & 0xFFFFFFFF);

	//because the high (sign) bits in both int1Hi & int2Hi have been zeroed,
	//there's no risk of 64 bit overflow in the following assignment
	//(ie: $7FFFFFFF*$FFFFFFFF + $7FFFFFFF*$FFFFFFFF < 64bits)

	ull64 a = int1Hi * int2Hi;
	ull64 b = int1Lo * int2Lo;
	ull64 c = int1Hi * int2Lo + int1Lo * int2Hi;

	ll64 T_high, T_low;
	T_high = ll64(a + (c >> 32));
	T_low = ll64(c << 32);
	T_low += ll64(b);
	if (T_low < b) T_high++;
	Int128 T(T_high, T_low);
	if (negate) T = -T;
	return T;
};

int main()
{
	Int128 A, B, C;
	A = 1234567891011121314;
	B = 1413121110987654321;
	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout << (double)(A * B) << endl;
	return 0;
}