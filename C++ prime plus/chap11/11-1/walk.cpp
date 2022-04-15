#include "walk.h"

#include <cmath>
#include <iostream>

namespace Vector{
    const double Rad_to_deg = 45. / atan(1.);

    Vector::Vector(double num1, double num2, Mode form){
        if (form == RECT) {
            x_ = num1;
            y_ = num2;
            CalR();
            CalAng();
        }
        else if (form == POL) {
            r_ = num1;
            ang_ = num2 / Rad_to_deg;
            CalX();
            CalY();
        }
        else{
            std::cout << "wrong Mode" << std::endl;
            x_ = y_ = r_ = ang_ = 0;
        }
    }

    void Vector::Step(double num1, double num2, Mode form){
        if (form == RECT) {
            x_ = num1;
            y_ = num2;
            CalR();
            CalAng();
        }
        else if (form == POL) {
            r_ = num1;
            ang_ = num2 / Rad_to_deg;
            CalX();
            CalY();
        }
        else{
            std::cout << "wrong Mode" << std::endl;
            x_ = y_ = r_ = ang_ = 0;
        }
    }

    Vector Vector::operator+(const Vector& another) const{
        // Vector temp;
        // temp.Step(this->x_ + another.x_, this->y_ + another.y_, RECT);
        // return temp;

        /* return的到底是什么？？ */
        return Vector(this->x_ + another.x_, this->y_ + another.y_);
    }

    void Vector::CalR(){
        //r_ = pow(pow(x_,2) + pow(y_,2),0.5);不好
        r_ = sqrt(x*x + y*y);
    }

    void Vector::CalAng(){
        if (x_ == 0. && y_ == 0.) {
            ang_ = 0;
        }
        else {
            ang_ = atan2(y_, x_);
        }
    }

    void Vector::CalX(){
        x_ = r_ * cos(ang_);
    }

    void Vector::CalX(){
        y_ = r_ * sin(ang_);
    }

    std::ostream& Vector::operator<<(std::ostream& os, Vector& now)
    {
        if (now.mode == RECT) {
            /* 1. std::endl不用加，在main函数里用的时候会加的 
             * 2. std::os错，os是个对象，std::是加在类前面的。加std是因为担心作用域，但是函数参数那已经说明了。
             * 3. 故意用的os，就是个对象的名字，平时用cout是因为C++源码用的cout
             */
            std::os << "(x,y) = (" << now.x_ << ", " << now.y_ << ")" <<; 
        }
        else if (now.mode == POL) {
            std::os << "(m,a) = (" << now.r_ << ", " << now.ang_ << ")" <<;
        }
        else {
            std::os << "Vector object mode is invalid";
        }

        return os; /* 重载的return一直忘 */
    }
    
    /* 注意：不是在Vector类内的。不然就变成了 某对象.cout */
    // std::ostream& operator<<(std::ostream& os, Vector& now)
    // {
    //     if (now.mode == RECT) {
    //         /* 1. std::endl不用加，在main函数里用的时候会加的 
    //          * 2. std::os错，os是个对象，std::是加在类前面的。加std是因为担心作用域，但是函数参数那已经说明了。
    //          * 3. 故意用的os，就是个对象的名字，平时用cout是因为C++源码用的cout
    //          */
    //         std::os << "(x,y) = (" << now.x_ << ", " << now.y_ << ")" <<; 
    //     }
    //     else if (now.mode == POL) {
    //         std::os << "(m,a) = (" << now.r_ << ", " << now.ang_ << ")" <<;
    //     }
    //     else {
    //         std::os << "Vector object mode is invalid";
    //     }
    // }
}  