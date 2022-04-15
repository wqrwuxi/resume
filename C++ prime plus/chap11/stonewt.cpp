#include "stonewt.h"

#include <iostream>

Stonewt::Stonewt(int sto, double pdsL)
{
    stone_ = sto;
    pdsLetf_ = pdsL;
    ponds_ = stone_ * LBS_PER_STN + pdsLetf_;
}

Stonewt& Stonewt::operator*(double num)
{
    this->ponds_ = num * this->ponds_;
    this->stone_ = this->ponds_ / LBS_PER_STN;
    this->pdsLetf_ = this->ponds_ - this->stone_ * LBS_PER_STN;
    return *this;
}

// 1.如果是2个对象相乘，this和temp都得用上，所以补一个temp。2.return不能用引用
// Stonewt Stonewt::operator*(double num)
// {
//     Stonewt temp;
//     temp.ponds_ = num * this->ponds_;
//     temp.stone_ = this->ponds_ / LBS_PER_STN;
//     temp.pdsLetf_ = this->ponds_ - this->stone_ * LBS_PER_STN;
//     return temp;
// }

void Stonewt::show() const
{
    std::cout << "stone:" << stone_ << "  " << "pdsLetf:" << pdsLetf_ << "  " << "ponds:" << ponds_ << std::endl;
}

Stonewt operator+(double pdsL, const Stonewt &origin)
{
    Stonewt temp;
    temp.ponds_ = origin.ponds_ + pdsL;
    temp.stone_ = temp.ponds_ / temp.LBS_PER_STN;
    temp.pdsLetf_ = temp.ponds_ - temp.stone_ * temp.LBS_PER_STN;
    return temp;
}