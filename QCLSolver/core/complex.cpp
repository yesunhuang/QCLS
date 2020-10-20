/**
 * @ FileName: Complex.c
 * @ Function: implement the function of Complex
 * @ Author: AyajiLin & YesunHuang
 * @ Mail: 493987054@qq.com & 944812203@qq.com
 * @ Github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ Date: 04/01/2020
 *
 */

#include "complex.h"

namespace ayaji {

#ifdef HIGH_PRECISION
	// high precision
	Complex::Complex() {
		mpf_init2(this->real, FLOAT_PRECISION);
		mpf_init2(this->image, FLOAT_PRECISION);
	}

	Complex::Complex(double real, double image) {
		mpf_init2(this->real, FLOAT_PRECISION);
		mpf_init2(this->image, FLOAT_PRECISION);
		mpf_set_d(this->real, real);
		mpf_set_d(this->image, image);
	}

	Complex::Complex(const Complex& o) {
		mpf_init2(this->real, FLOAT_PRECISION);
		mpf_init2(this->image, FLOAT_PRECISION);
		mpf_set(this->real, o.real);
		mpf_set(this->image, o.image);
	}

	Complex::Complex(Complex&& o) noexcept {
		mpf_init2(this->real, FLOAT_PRECISION);
		mpf_init2(this->image, FLOAT_PRECISION);
		mpf_swap(this->real, o.real);
		mpf_swap(this->image, o.image);
	}

	Complex::~Complex() {
		mpf_clears(this->real, this->image, NULL);
	}

	Complex& Complex::operator= (const Complex& o) {
		mpf_set(this->real, o.real);
		mpf_set(this->image, o.image);
		return *this;
	}

	Complex Complex::operator+ (const Complex& o) const {
		Complex ans;
		mpf_add(ans.real, this->real, o.real);
		mpf_add(ans.image, this->image, o.image);
		return ans;
	}

	Complex Complex::operator- (const Complex& o) const {
		Complex ans;
		mpf_sub(ans.real, this->real, o.real);
		mpf_sub(ans.image, this->image, o.image);
		return ans;
	}

	Complex Complex::operator* (const Complex& o) const {
		Complex ans;
		mpf_t tmp1, tmp2;
		mpf_init2(tmp1, FLOAT_PRECISION);
		mpf_init2(tmp2, FLOAT_PRECISION);
		// real part
		mpf_mul(tmp1, this->real, o.real);
		mpf_mul(tmp2, this->image, o.image);
		mpf_sub(ans.real, tmp1, tmp2);
		// image part
		mpf_mul(tmp1, this->real, o.image);
		mpf_mul(tmp2, this->image, o.real);
		mpf_add(ans.image, tmp1, tmp2);
		// free memory
		mpf_clears(tmp1, tmp2, NULL);
		return ans;
	}

	Complex Complex::operator/ (const Complex& o) const {
		Complex ans;
		mpf_t tmp1, tmp2, temp;
		mpf_init2(tmp1, FLOAT_PRECISION);
		mpf_init2(tmp2, FLOAT_PRECISION);
		mpf_init2(temp, FLOAT_PRECISION);
		// calculate temp
		mpf_mul(tmp1, o.real, o.real);
		mpf_mul(tmp2, o.image, o.image);
		mpf_add(temp, tmp1, tmp2);
		mpf_ui_div(temp, 1, temp);
		// real part
		mpf_mul(tmp1, this->real, o.real);
		mpf_mul(tmp2, this->image, o.image);
		mpf_add(tmp1, tmp1, tmp2);
		mpf_mul(ans.real, tmp1, temp);
		// image part
		mpf_mul(tmp1, this->image, o.real);
		mpf_mul(tmp2, this->real, o.image);
		mpf_sub(tmp1, tmp1, tmp2);
		mpf_mul(ans.image, tmp1, temp);
		// free memory
		mpf_clears(tmp1, tmp2, temp, NULL);
		return ans;
	}

	Complex& Complex::operator+= (const Complex& o) {
		mpf_add(this->real, this->real, o.real);
		mpf_add(this->image, this->image, o.image);
		return *this;
	}

	Complex& Complex::operator-= (const Complex& o) {
		mpf_sub(this->real, this->real, o.real);
		mpf_sub(this->image, this->image, o.image);
		return *this;
	}

	Complex& Complex::operator*= (const Complex& o) {
		Complex ans;
		mpf_t tmp1, tmp2;
		mpf_init2(tmp1, FLOAT_PRECISION);
		mpf_init2(tmp2, FLOAT_PRECISION);
		// real part
		mpf_mul(tmp1, this->real, o.real);
		mpf_mul(tmp2, this->image, o.image);
		mpf_sub(this->real, tmp1, tmp2);
		// image part
		mpf_mul(tmp1, this->real, o.image);
		mpf_mul(tmp2, this->image, o.real);
		mpf_add(this->image, tmp1, tmp2);
		// free memory
		mpf_clears(tmp1, tmp2, NULL);
		return *this;
	}

	Complex& Complex::operator/= (const Complex& o) {
		Complex ans;
		mpf_t tmp1, tmp2, temp;
		mpf_init2(tmp1, FLOAT_PRECISION);
		mpf_init2(tmp2, FLOAT_PRECISION);
		mpf_init2(temp, FLOAT_PRECISION);
		// calculate temp
		mpf_mul(tmp1, o.real, o.real);
		mpf_mul(tmp2, o.image, o.image);
		mpf_add(temp, tmp1, tmp2);
		mpf_ui_div(temp, 1, temp);
		// real part
		mpf_mul(tmp1, this->real, o.real);
		mpf_mul(tmp2, this->image, o.image);
		mpf_add(tmp1, tmp1, tmp2);
		mpf_mul(this->real, tmp1, temp);
		// image part
		mpf_mul(tmp1, this->image, o.real);
		mpf_mul(tmp2, this->real, o.image);
		mpf_sub(tmp1, tmp1, tmp2);
		mpf_mul(this->image, tmp1, temp);
		// free memory
		mpf_clears(tmp1, tmp2, temp, NULL);
		return *this;
	}

	Complex& Complex::operator++ () {
		mpf_add_ui(this->real, this->real, 1);
		return *this;
	}

	Complex& Complex::operator-- () {
		mpf_sub_ui(this->real, this->real, 1);
		return *this;
	}

	Complex Complex::operator++ (int) {
		Complex c(*this);
		mpf_add_ui(this->real, this->real, 1);
		return c;
	}

	Complex Complex::operator-- (int) {
		Complex c(*this);
		mpf_sub_ui(this->real, this->real, 1);
		return c;
	}

	bool Complex::isZero() const {
		return (mpf_cmp_d(this->real, 0) == 0 && mpf_cmp_d(this->image, 0) == 0);
	}

	double Complex::getReal() const {
		return mpf_get_d(this->real);
	}

	double Complex::getImage() const {
		return mpf_get_d(this->image);
	}

	void Complex::setReal(double real) {
		mpf_set_d(this->real, real);
	}

	void Complex::setImage(double image) {
		mpf_set_d(this->image, image);
	}

	void Complex::set(double real, double image) {
		mpf_set_d(this->real, real);
		mpf_set_d(this->image, image);
	}
#else
	// normal precision
	Complex::Complex() 
		: real(0.0), image(0.0)
	{
	}

	Complex::Complex(double real, double image)
		: real(real), image(image)
	{
	}

	Complex::Complex(const Complex& o) {
		this->real = o.real;
		this->image = o.image;
	}

	Complex::Complex(Complex&& o) noexcept {
		this->real = o.real;
		this->image = o.image;
	}

	Complex::~Complex() {

	}

	Complex& Complex::operator= (const Complex& o) {
		this->real = o.real;
		this->image = o.image;
		return *this;
	}

	Complex Complex::operator+ (const Complex& o) const {
		return Complex(this->real + o.real, this->image + o.image);
	}

	Complex Complex::operator- (const Complex& o) const {
		return Complex(this->real - o.real, this->image - o.image);
	}

	Complex Complex::operator* (const Complex& o) const {
		return Complex(this->real * o.real - this->image * o.image,
			this->real * o.image + this->image * o.real);
	}

	Complex Complex::operator/ (const Complex& o) const {
		double temp = 1.0 / (o.real * o.real + o.image * o.image);
		return Complex((this->real * o.real + this->image * o.image) * temp,
			(this->image * o.real - this->real * o.image) * temp);
	}

	Complex& Complex::operator+= (const Complex& o) {
		this->real += o.real;
		this->image += o.image;
		return *this;
	}

	Complex& Complex::operator-= (const Complex& o) {
		this->real -= o.real;
		this->image -= o.image;
		return *this;
	}

	Complex& Complex::operator*= (const Complex& o) {
		Complex tmp;
		tmp = *this;
		this->real = tmp.real * o.real - tmp.image * o.image;
		this->image = tmp.real * o.image + tmp.image * o.real;
		return *this;
	}

	Complex& Complex::operator/= (const Complex& o) {
		Complex tmp;
		tmp = *this;
		double temp = 1.0 / (o.real * o.real + o.image * o.image);
		this->real = (tmp.real * o.real + tmp.image * o.image) * temp;
		this->image = (tmp.image * o.real - tmp.real * o.image) * temp;
		return *this;
	}

	Complex& Complex::operator++ () {
		this->real += 1;
		return *this;
	}

	Complex& Complex::operator-- () {
		this->real -= 1;
		return *this;
	}

	Complex Complex::operator++ (int) {
		Complex c(*this);
		this->real += 1;
		return c;
	}

	Complex Complex::operator-- (int) {
		Complex c(*this);
		this->real -= 1;
		return c;
	}

	bool Complex::isZero() const {
		return (abs(this->real) < episilon && abs(this->image) < episilon);
	}

	double Complex::getReal() const {
		return this->real;
	}

	double Complex::getImage() const {
		return this->image;
	}

	void Complex::setReal(double real) {
		this->real = real;
	}

	void Complex::setImage(double image) {
		this->image = image;
	}

	void Complex::set(double real, double image) {
		this->real = real;
		this->image = image;
	}
#endif // !HIGH_PRECISION
	
	// common
	std::string Complex::getString() const {
		std::string str;
		str += std::to_string(getReal());
		str += " + ";
		str += std::to_string(getImage());
		str += "i";
		return std::move(str);
	}
}