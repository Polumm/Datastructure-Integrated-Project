#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <string>

const int kMax = 100;
const int kInf = INT_MAX;
/// <summary>
/// �������У԰���·��
/// </summary>
class Graph {
public:
    Graph();
    /// <summary>
    /// ���붥�����Ĺ��캯��
    /// </summary>
    /// <param name="num_vertices">������</param>
    Graph(int num_vertices) : num_vertices(num_vertices) {
        adjacency_list.resize(num_vertices);
        vertex_names.resize(num_vertices);
        vertex_pos.resize(num_vertices);
    }
    /// <summary>
    /// ��ӱ�
    /// </summary>
    /// <param name="from">���</param>
    /// <param name="to">�յ�</param>
    /// <param name="weight">Ȩ��</param>
    void AddEdge(int from, int to, int weight) {
        adjacency_list[from].push_back(std::make_pair(to, weight));
    }
    /// <summary>
    /// �����ڽӱ�����ȼ����е�Dijkstra���·�������㷨
    /// </summary>
    /// <param name="start"></param>
    /// <param name="end"></param>
    /// <returns></returns>
    std::vector<int> Dijkstra(int start, int end) {
        std::vector<int> preNode(num_vertices, -1);//ǰ���ڵ��
        std::vector<int> distances(num_vertices, kInf);//��̾����
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,std::greater<std::pair<int, int>>> queueDis;//����ͱ�ŵĶ���
        //��������
        queueDis.push(std::make_pair(0, start));
        distances[start] = 0;
        while (!queueDis.empty()) {
            //�������ٽ���
            int current = queueDis.top().second;
            queueDis.pop();
            //�����ڽӱ�
            for (const auto& neighbour : adjacency_list[current]) {
                int next = neighbour.first;
                int weight = neighbour.second;
                //���¾�������к�ǰ���б�
                if (distances[next] > distances[current] + weight) {
                    distances[next] = distances[current] + weight;
                    queueDis.push(std::make_pair(distances[next], next));
                    preNode[next] = current;
                }
            }
        }
        //���·��
        std::vector<int> path;
        for (int i = end; i != -1; i = preNode[i]) {
            path.insert(path.begin(), i);
        }
        return path;
    }
    /// <summary>
    /// �жϱ��Ƿ���ڣ����ڰ�ȫ�Լ��
    /// </summary>
    /// <param name="from"></param>
    /// <param name="to"></param>
    /// <returns></returns>
    bool EdgeExists(int from, int to) const {
        for (const auto& edge : adjacency_list[from]) {
            if (edge.first == to) {
                return true;
            }
        }
        return false;
    }
    /// <summary>
    /// ��ȡ�ߵ�Ȩ��
    /// </summary>
    /// <param name="from"></param>
    /// <param name="to"></param>
    /// <returns></returns>
    int GetEdgeWeight(int from, int to) const {
        for (const auto& edge : adjacency_list[from]) {
            if (edge.first == to) {
                return edge.second;
            }
        }
        return -1;
    }
    /// <summary>
    /// ��ӡ��
    /// </summary>
    void PrintEdges() const {
        for (int i = 0; i < num_vertices; i++) {
            std::cout << "Vertex " << i << " is connected to:\n";
            for (const auto& edge : adjacency_list[i]) {
                std::cout << "\tVertex " << edge.first << " with weight " << edge.second << "\n";
            }
        }
    }
    /// <summary>
    /// ��ӡ����
    /// </summary>
    void PrintVertices() const {
        std::cout << "List of vertices:\n";
        for (int i = 0; i < num_vertices; i++) {
            std::cout << "\tVertex " << i << "\n";
        }
    }
    /// <summary>
    /// ��Ӷ�������
    /// </summary>
    /// <param name="vertex"></param>
    /// <param name="name"></param>
    void AddVertexName(int vertex, const std::string& name) {
        vertex_names[vertex] = name;
    }
    /// <summary>
    /// ��Ӷ�������
    /// </summary>
    /// <param name="vertex"></param>
    /// <param name="pos"></param>
    void AddVertexPos(int vertex, const std::string& pos) {
        vertex_pos[vertex] = pos;
    }
    /// <summary>
    /// ��ȡ����ID
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    int GetVertexId(const std::string& name) const {
        for (int i = 0; i < num_vertices; ++i) {
            if (vertex_names[i] == name) {
                return i;
            }
        }
        return -1;
    }
    /// <summary>
    /// ��ȡ����λ��
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> GetVertexPos() const {
        return vertex_pos;
    }
    /// <summary>
    /// ��ȡ��
    /// </summary>
    /// <returns></returns>
    std::vector<std::vector<std::pair<int, int>>> GetEdges() const {
        return adjacency_list;
    }
    /// <summary>
    /// ��ȡ������
    /// </summary>
    /// <param name="num"></param>
    void setVerticeNum(int num)
    {
        num_vertices = num;
        adjacency_list.resize(num_vertices);
        vertex_names.resize(num_vertices);
        vertex_pos.resize(num_vertices);
    }
    /// <summary>
    /// ��ȡ�������б�
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> GetVertex_names() const {
        return vertex_names;
    }
    /// <summary>
    /// ��ȡ������
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    std::string GetVertexName(int id) const {
        return vertex_names[id];
    }
private:
    int num_vertices;//������
    std::vector<std::vector<std::pair<int, int>>> adjacency_list; //�ڽӱ�ʵ��ϡ��洢ͼ��<from<to,weight>>
    std::vector<std::string> vertex_names;//������
    std::vector<std::string> vertex_pos;//��������
};

//int main() {
//    std::ifstream input_file("input.txt");
//    int num_vertices, num_edges;
//    input_file >> num_vertices >> num_edges;
//    Graph graph(num_vertices);
//    //��ʼ��������
//    for (int i = 0; i < num_vertices; i++) {
//        int id; std::string name;
//        input_file >> id >> name;
//        graph.AddVertexName(id, name);
//    }
//    //��ʼ����������
//    for (int i = 0; i < num_vertices; i++) {
//        int id; std::string pos;
//        input_file >> id; std::getline(input_file, pos);
//        graph.AddVertexPos(id, pos);
//    }
//    //��ӱ߶�
//    for (int i = 0; i < num_edges; i++) {
//        int from, to, weight;
//        input_file >> from >> to >> weight;
//        graph.AddEdge(from, to, weight);
//    }
//
//    graph.PrintEdges();
//    graph.PrintVertices();
//    int from = 0;
//    int to = 3;
//
//    if (graph.EdgeExists(from, to)) {
//        std::cout << "Edge exists from vertex " << from << " to vertex " << to << " with weight " << graph.GetEdgeWeight(from, to) << "\n";
//    }
//    else {
//        std::cout << "Edge does not exist from vertex " << from << " to vertex " << to << "\n";
//    }
//
//
//    int start;
//    input_file >> start;
//    for (int i = 0; i < num_vertices; i++) {
//        std::vector<int> distances = graph.Dijkstra(start, i);
//        for (int j = 0; j < distances.size(); j++) {
//            std::cout << distances[j] << " ";
//        }
//        std::cout << '\n';
//    }
//
//
//    return 0;
//}


