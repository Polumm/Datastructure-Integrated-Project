#include <utility>  
#include <list>  
#include <map>  

//#define Type int   //������������  

#define   Map1        std::map< int, int >    //����һάmap  
#define   Map2        std::map< int, Map1 >    //�����άmap  
#define   Map3        std::map< int, Map2 >    //������άmap  
#define   Pair        std::pair<int, int>  
#define   List        std::list< Pair >        //һάlist  
#define   SampleSpace std::list< List >        //��άlist ���ڴ����������  
#define   Child       std::map< int, Node* >   //�����̽ڵ㼯��  
#define   CI          const_iterator  

/*
 *   ��ID3�㷨�У��ö�ά�������������ṹΪlist< list< pair<int, int> > >�����ΪSampleSpace��ȡ�������ռ�
 *   �������ݴӸ��ڵ㿪ʼ���±�����ÿһ���ڵ�Ķ������½ṹ��
 */

struct Node
{
    int index;                    //��ǰ�ڵ�������������Ӧ��index�����ԣ�����������з����  
    int type;                     //��ǰ�ڵ������  
    Child next;                   //��ǰ�ڵ�ĺ�̽ڵ㼯��  
    SampleSpace sample;           //δ�������������  
};

class ID3
{

public:
    ID3();//Ĭ�Ϲ���
    ID3(int);//�вι��촫��ά��
    ~ID3();//��������
    void setDimension(int d);//����ά��

    void PushData(const int*, const int);   //����������Push����ά����  
    void Build();                             //����������  
    int  Match(const int*);                  //�����µ�����Ԥ����  
    void Print();                             //��ӡ�������Ľڵ��ֵ  

    void   _clear(Node*);//���������
    void   _build(Node*, int);//����������
    int    _match(const int*, Node*);
    void   _work(Node*);//ִ�к���
    double _entropy(const Map1&, double);//������ֵ  
    virtual int _get_max_gain(const SampleSpace&);//��ȡ������Ϣ�����Ӧ������  
    void   _split(Node*, int);//�Ե�ǰ�ڵ��sample���л���
    void   _get_data(const SampleSpace&, Map1&, Map2&, Map3&);//��ȡ���ݣ���ȡ������������yֵ��x[]����ֵ���Լ�����ֵ�ͽ��ֵxy��  
    double _info_gain(Map1&, Map2&, double, double);//������Ϣ���� 
    int    _same_class(const SampleSpace&);//�жϵ�ǰ���������Ƿ���ͬһ�࣬��������򷵻�-1  
    void   _print(Node*);//��ӡ������

private:

    int dimension;//ά��
    Node* root;//���ڵ�
};
