#include <utility>  
#include <list>  
#include <map>  

//#define Type int   //样本数据类型  

#define   Map1        std::map< int, int >    //定义一维map  
#define   Map2        std::map< int, Map1 >    //定义二维map  
#define   Map3        std::map< int, Map2 >    //定义三维map  
#define   Pair        std::pair<int, int>  
#define   List        std::list< Pair >        //一维list  
#define   SampleSpace std::list< List >        //二维list 用于存放样本数据  
#define   Child       std::map< int, Node* >   //定义后继节点集合  
#define   CI          const_iterator  

/*
 *   在ID3算法中，用二维链表存放样本，结构为list< list< pair<int, int> > >，简记为SampleSpace，取名样本空间
 *   样本数据从根节点开始往下遍历。每一个节点的定义如下结构体
 */

struct Node
{
    int index;                    //当前节点样本最大增益对应第index个属性，根据这个进行分类的  
    int type;                     //当前节点的类型  
    Child next;                   //当前节点的后继节点集合  
    SampleSpace sample;           //未分类的样本集合  
};

class ID3
{

public:
    ID3();//默认构造
    ID3(int);//有参构造传入维度
    ~ID3();//析构函数
    void setDimension(int d);//设置维度

    void PushData(const int*, const int);   //将样本数据Push给二维链表  
    void Build();                             //构建决策树  
    int  Match(const int*);                  //根据新的样本预测结果  
    void Print();                             //打印决策树的节点的值  

    void   _clear(Node*);//清理决策树
    void   _build(Node*, int);//构建决策树
    int    _match(const int*, Node*);
    void   _work(Node*);//执行函数
    double _entropy(const Map1&, double);//计算熵值  
    virtual int _get_max_gain(const SampleSpace&);//获取最大的信息增益对应的属性  
    void   _split(Node*, int);//对当前节点的sample进行划分
    void   _get_data(const SampleSpace&, Map1&, Map2&, Map3&);//获取数据，提取出所有样本的y值，x[]属性值，以及属性值和结果值xy。  
    double _info_gain(Map1&, Map2&, double, double);//计算信息增益 
    int    _same_class(const SampleSpace&);//判断当前所有样本是否是同一类，如果不是则返回-1  
    void   _print(Node*);//打印决策树

private:

    int dimension;//维度
    Node* root;//根节点
};
