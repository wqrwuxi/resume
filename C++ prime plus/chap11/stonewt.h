#ifndef STONEWT_H
#define STONEWT_H

class Stonewt{
    friend Stonewt operator+(double pdsL, const Stonewt &origin);

public:
    Stonewt() : stone_(0), pdsLetf_(0), ponds_(0)  {}
    Stonewt(int sto, double pdsL);
    virtual ~Stonewt() {}
    Stonewt& operator*(double num);
    void show() const;

private:
    enum {
        LBS_PER_STN = 14
    };
    /* 
     * 报错error: two or more types in declaration of `stone_'
     * 原因：enum的}后没加;
     */
    int stone_;
    double pdsLetf_;
    double ponds_;
};

Stonewt operator+(double pdsL, const Stonewt &origin);

#endif /* STONEWT_H */