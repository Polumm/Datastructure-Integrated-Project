#include "ID3.h"  
#include <iostream> 
#include <sstream>
#include<vector>
#include<map>
#include<fstream>
using namespace std;

enum outlook { SUNNY, OVERCAST, RAIN };
enum temp { HOT, MILD, COOL };
enum hum { HIGH, NORMAL };
enum windy { WEAK, STRONG };

int samples[14][4] =
{
    {SUNNY   ,       COOL ,      HIGH  ,       WEAK  },
    {RAIN   ,       HOT ,      HIGH  ,       STRONG},
    {OVERCAST,       COOL ,      HIGH  ,       WEAK  },
    {RAIN    ,       MILD,      HIGH  ,       WEAK  },
    {RAIN    ,       COOL,      NORMAL,       WEAK  },
    {RAIN    ,       MILD,      NORMAL,       WEAK},
    {OVERCAST,       COOL,      NORMAL,       STRONG},
    {SUNNY   ,       MILD,      HIGH  ,       WEAK  },
    {SUNNY   ,       COOL,      NORMAL,       WEAK  },
    {RAIN    ,       MILD,      NORMAL,       WEAK  },
    {SUNNY   ,       MILD,      NORMAL,       STRONG},
    {OVERCAST,       MILD,      HIGH  ,       STRONG},
    {OVERCAST,       HOT ,      NORMAL,       WEAK  },
    {RAIN    ,       MILD,      HIGH  ,       STRONG}
};

vector<vector<string>> read_csv(string file_path) {
    ifstream file(file_path);
    vector<vector<string>> data;
    string line;
    while (getline(file, line)) {
        stringstream line_stream(line);
        string cell;
        vector<string> cells;
        while (getline(line_stream, cell, ',')) {
            cells.push_back(cell);
        }
        data.push_back(cells);
    }
    return data;
}

vector<vector<int>> getFirstNColumns(vector<vector<int>>& originalData, int n)
{
    vector<vector<int>> newData;
    for (auto& row : originalData)
    {
        vector<int> newRow(row.begin(), row.begin() + n);
        newData.push_back(newRow);
    }
    return newData;
}


//int main()
//{
//
//    vector<vector<string>> strs = read_csv("DT_data.csv");
//    vector<vector<int>> featureVectors(strs.size(), vector<int>(strs[0].size()));
//    vector<map<string, int>> mdict;
//    vector<map<int, string>> reverse_dict;
//    vector<string> names = strs[0];
//    strs.erase(strs.begin());
//    for (int j = 0; j < strs[0].size(); j++) {
//        map<string, int> strToInt;
//        map<int, string> intToStr;
//        int nextInt = 0;
//        for (int i = 0; i < strs.size(); i++) {
//            const string& str = strs[i][j];
//            if (strToInt.count(str) == 0) {
//                strToInt[str] = nextInt;
//                intToStr[nextInt++] = str;
//            }
//            featureVectors[i][j] = strToInt[str];
//        }
//        reverse_dict.push_back(intToStr);
//        mdict.push_back(strToInt);
//    }
//
//
//
//
//    for (const auto& intRow : featureVectors) {
//        for (int i : intRow) {
//            std::cout << i << " ";
//        }
//        cout << endl;
//    }
//
//
//
//    ID3 Tree(5);
//    vector<vector<int>> input = getFirstNColumns(featureVectors, featureVectors[0].size()-1);
//    for (int i = 0; i < input.size(); i++)
//    {
//        Tree.PushData(&input[i][0], featureVectors[i][featureVectors[0].size()-1]);
//    }
//
//    Tree.Build();
//    Tree.Print();
//    cout << endl;
//    for (int i = 0; i < 14; ++i)
//    //int i = 1;
//        cout << "predict value :    " << reverse_dict[reverse_dict.size() - 1][Tree.Match((int*)&samples[i])] << endl;
//    return 0;
//}