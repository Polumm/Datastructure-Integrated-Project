#pragma once
#include "ID3.h"
class C4D5 :
    public ID3
{
public:
    C4D5();//默认构造
    C4D5(int);//有参构造，传入维度
    double _info_gain_ratio(Map1& att_val, Map2& val_cls, double s, double entropy);//计算信息增益率
    virtual int _get_max_gain(const SampleSpace&) override;//重写关键函数，获取最大的信息增益率对应的属性
};


