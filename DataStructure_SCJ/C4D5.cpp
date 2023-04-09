#include "C4D5.h"

C4D5::C4D5():ID3() {}

C4D5::C4D5(int dimension) : ID3(dimension) {}

//计算信息增益率
double C4D5::_info_gain_ratio(Map1& att_val, Map2& val_cls, double s, double entropy)
{
    double info_gain = _info_gain(att_val, val_cls, s, entropy);
    double split_info = 0;
    for (Map1::CI it = att_val.begin(); it != att_val.end(); it++)
    {
        double r = it->second / s;
        split_info += r * log2(r);
    }
    split_info = -split_info;
    return info_gain / split_info;
}

int C4D5::_get_max_gain(const SampleSpace& ss)
{
    Map1 y;
    Map2 x;
    Map3 xy;

    _get_data(ss, y, x, xy);
    double s = ss.size();
    double entropy = _entropy(y, s);   //计算熵值  

    int mai = -1;
    double mag = -1;

    for (Map2::iterator it = x.begin(); it != x.end(); it++)
    {
        double g = _info_gain_ratio(it->second, xy[it->first], s, entropy);    //计算信息增益值  
        if (g > mag)
        {
            mag = g;
            mai = it->first;
        }
    }

    if (!x.size() && !xy.size() && y.size())   //如果只有y数据  
        return 0;
    return mai;

}
