#pragma once
#include "ID3.h"
class C4D5 :
    public ID3
{
public:
    C4D5();//Ĭ�Ϲ���
    C4D5(int);//�вι��죬����ά��
    double _info_gain_ratio(Map1& att_val, Map2& val_cls, double s, double entropy);//������Ϣ������
    virtual int _get_max_gain(const SampleSpace&) override;//��д�ؼ���������ȡ������Ϣ�����ʶ�Ӧ������
};


