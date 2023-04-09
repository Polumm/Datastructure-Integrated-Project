#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <string>

const int kMax = 100;
const int kInf = INT_MAX;
/// <summary>
/// 用于求解校园最短路径
/// </summary>
class Graph {
public:
    Graph();
    /// <summary>
    /// 传入顶点数的构造函数
    /// </summary>
    /// <param name="num_vertices">顶点数</param>
    Graph(int num_vertices) : num_vertices(num_vertices) {
        adjacency_list.resize(num_vertices);
        vertex_names.resize(num_vertices);
        vertex_pos.resize(num_vertices);
    }
    /// <summary>
    /// 添加边
    /// </summary>
    /// <param name="from">起点</param>
    /// <param name="to">终点</param>
    /// <param name="weight">权重</param>
    void AddEdge(int from, int to, int weight) {
        adjacency_list[from].push_back(std::make_pair(to, weight));
    }
    /// <summary>
    /// 基于邻接表和优先级队列的Dijkstra最短路径搜索算法
    /// </summary>
    /// <param name="start"></param>
    /// <param name="end"></param>
    /// <returns></returns>
    std::vector<int> Dijkstra(int start, int end) {
        std::vector<int> preNode(num_vertices, -1);//前驱节点表
        std::vector<int> distances(num_vertices, kInf);//最短距离表
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,std::greater<std::pair<int, int>>> queueDis;//距离和编号的队列
        //将起点入队
        queueDis.push(std::make_pair(0, start));
        distances[start] = 0;
        while (!queueDis.empty()) {
            //出队最临近点
            int current = queueDis.top().second;
            queueDis.pop();
            //遍历邻接表
            for (const auto& neighbour : adjacency_list[current]) {
                int next = neighbour.first;
                int weight = neighbour.second;
                //更新距离表、队列和前驱列表
                if (distances[next] > distances[current] + weight) {
                    distances[next] = distances[current] + weight;
                    queueDis.push(std::make_pair(distances[next], next));
                    preNode[next] = current;
                }
            }
        }
        //输出路径
        std::vector<int> path;
        for (int i = end; i != -1; i = preNode[i]) {
            path.insert(path.begin(), i);
        }
        return path;
    }
    /// <summary>
    /// 判断边是否存在，用于安全性检查
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
    /// 获取边的权重
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
    /// 打印边
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
    /// 打印顶点
    /// </summary>
    void PrintVertices() const {
        std::cout << "List of vertices:\n";
        for (int i = 0; i < num_vertices; i++) {
            std::cout << "\tVertex " << i << "\n";
        }
    }
    /// <summary>
    /// 添加顶点名称
    /// </summary>
    /// <param name="vertex"></param>
    /// <param name="name"></param>
    void AddVertexName(int vertex, const std::string& name) {
        vertex_names[vertex] = name;
    }
    /// <summary>
    /// 添加顶点坐标
    /// </summary>
    /// <param name="vertex"></param>
    /// <param name="pos"></param>
    void AddVertexPos(int vertex, const std::string& pos) {
        vertex_pos[vertex] = pos;
    }
    /// <summary>
    /// 获取顶点ID
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
    /// 获取顶点位置
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> GetVertexPos() const {
        return vertex_pos;
    }
    /// <summary>
    /// 获取边
    /// </summary>
    /// <returns></returns>
    std::vector<std::vector<std::pair<int, int>>> GetEdges() const {
        return adjacency_list;
    }
    /// <summary>
    /// 获取顶点数
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
    /// 获取顶点名列表
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> GetVertex_names() const {
        return vertex_names;
    }
    /// <summary>
    /// 获取顶点名
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    std::string GetVertexName(int id) const {
        return vertex_names[id];
    }
private:
    int num_vertices;//顶点数
    std::vector<std::vector<std::pair<int, int>>> adjacency_list; //邻接表实现稀疏存储图，<from<to,weight>>
    std::vector<std::string> vertex_names;//顶点名
    std::vector<std::string> vertex_pos;//顶点坐标
};

//int main() {
//    std::ifstream input_file("input.txt");
//    int num_vertices, num_edges;
//    input_file >> num_vertices >> num_edges;
//    Graph graph(num_vertices);
//    //初始化顶点名
//    for (int i = 0; i < num_vertices; i++) {
//        int id; std::string name;
//        input_file >> id >> name;
//        graph.AddVertexName(id, name);
//    }
//    //初始化顶点坐标
//    for (int i = 0; i < num_vertices; i++) {
//        int id; std::string pos;
//        input_file >> id; std::getline(input_file, pos);
//        graph.AddVertexPos(id, pos);
//    }
//    //添加边对
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


