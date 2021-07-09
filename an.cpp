/*
 * @Author: HWG
 * @Date: 2021-05-05 21:42:39
 * @LastEditTime: 2021-07-09 02:49:28
 * @LastEditors: Please set LastEditors
 * @Description: 验证cc老师的猜想
 * @FilePath: /pre/an.cpp
 */

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <regex>
#include <string>
#include <map>
#include <set>
#include "fileRead.h"
using namespace std;
// #define NO_DEBUG

struct node{
    int id;
    int weight;
};

/*
 * 从dot格式的文件中读取图，未完成
 */
void readGraphFromDot(char * filePath,vector<int> & weight);

/*
 * 从ibm格式的文件中读取图
 */
void readGraphFromIbm(ifstream & ibmFile,vector<int> & inStart,vector<int> & inEnd,vector<int> & inEdge,vector<int> & inWeight,vector<int> & outStart,vector<int> & outEnd,vector<int> & outEdge,vector<int> & outWeight,vector<int> & weight,int & nodeNum,int & edgeNum);
/*
 * 计算分区权重
 */
void getPartWeight(const vector<int> & partInfo,const vector<int> & nodeweight,map<int,int> & pw);
/*
 * 计算分区间的边权
 */
void getPartEdgeWeight(const vector<int> & partInfo,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & inEdge,const vector<int> & inWeight,vector<vector<int> > & pew,int & pen);
/*
 *计算图的分布 输出csv文件，包括出度分布，入读分布，总分布
 */
void getGraphDistribution(const char * filePath,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & outStart,const vector<int> & outEnd);
void getPartEdgeWeightNew(const vector<int> & partInfo,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & inEdge,const vector<int> & inWeight,vector<vector<int> > & pew,int & pen);
void getPartWeightnew(const vector<int> & partInfo,const vector<int> & nodeweight,vector<int> & pw);


int main(int argc , char ** argv){
    if(argc != 3 && argc != 2)
    {
        cerr << "The parameter was wrong !"<<endl;
        cout << "——————Usage——————"<<endl;
        cout <<argv[0]<<" graphFile <partFile>"<<endl;
        cout << "——————explain——————"<<endl;
        cout <<"graphFile must provide,the graph distribution file will storage at the same path with suffix \".dtb.csv\""<<endl;
        cout <<"if the partFile was given , a new graph will storage at the same path with suffix \".info\""<<endl;
        return -1;
    }
    
    /* 读取图文件 */
    ifstream ibmFile(argv[1],ios_base::in);
    if(!ibmFile.is_open())
    {
        cerr << "ibm file not found !"<<endl;
        return -1;
    }
    vector<int> nodeWeight,inStart,inEnd,inEdge,inWeight,outStart,outEnd,outEdge,outWeight;
    int nodeNum,edgeNum;
    // readGraphFromIbm(ibmFile,outStart,outEnd,outEdge,outWeight,inStart,inEnd,inEdge,inWeight,nodeWeight,nodeNum,edgeNum);
    // readGraphFromIbm(ibmFile,outStart,outEnd,outEdge,outWeight,inStart,inEnd,inEdge,inWeight,nodeWeight,nodeNum,edgeNum);
    readFromIbm(ibmFile,inStart,inEnd,inEdge,inWeight,
    outStart,outEnd,outEdge,outWeight,nodeWeight,nodeNum,edgeNum);
    clog<<"#node:"<<nodeNum<<"\t#edge:"<<edgeNum<<endl;
    ibmFile.close();


    /* 计算distribution */
    // char  disFilePath[1024];
    // strcpy(disFilePath,argv[1]);
    // strcat(disFilePath,".dtb.csv");
    // getGraphDistribution(disFilePath,inStart,inEnd,outStart,outEnd);
    // clog << "distribution save to "<<disFilePath<<endl;


    /* 读取分区文件 */
    if(argc != 3)
        return 0;


    ifstream partFile(argv[2],ios::in);
    if(!partFile.is_open()){
        clog << "partFile not found,please check if the partFile\""<< argv[2]<<"\" is valid!"<<endl;
        return 0;
    }
    vector<int> partInfo;
    partInfo.push_back(-1);
    int temp;


    while(!partFile.eof())
    {
        partFile >> temp;
        
        partInfo.push_back(temp);
    }
    clog<<"#part:"<<partInfo.size()-1<<endl;
    // #ifndef NO_DEBUG
    // cout << "partInfo"<<endl;
    // for(int i=1;i<partInfo.size();i++)
    //     cout << partInfo.at(i)<< " ";
    // cout << endl;
    // #endif
    partFile.close();


    /* 计算分区权重 */
    // map<int,int> partWeight;
    vector<int> partWeight;
    getPartWeightnew(partInfo,nodeWeight,partWeight);
    // #ifndef NO_DEBUG
    // cout<< "part weight"<<endl;
    // for (auto i = partWeight.begin(); i != partWeight.end(); i++)
    //     cout << "(" << (*i).first << "," << (*i).second <<") ";
    // cout << endl;
    // #endif

    /* 计算分权间边权 */
    vector< vector<int> > partEdgeWeight;
    int partEdgeNum = 0;
    // cout << partEdgeWeight.size()<<endl;
    partEdgeWeight.assign(partWeight.size(),vector<int>());
    clog<<partEdgeWeight.max_size()<<endl;
    for(size_t i = 1;i<partEdgeWeight.size();i++){
        // vector<int> temp ;
        // temp.assign(partWeight.size()+1,-1);
        // partEdgeWeight.push_back(temp);
        partEdgeWeight[i].assign(partWeight.size(),-1);
        clog<<partEdgeWeight.capacity()<<"\t"<<partEdgeWeight.size()<<endl;
    }
    // cout << partEdgeWeight.size()<<endl;
    // getPartEdgeWeightNew(partInfo,outStart,outEnd,outEdge,outWeight,partEdgeWeight,partEdgeNum);
    getPartEdgeWeight(partInfo,outStart,outEnd,outEdge,outWeight,partEdgeWeight,partEdgeNum);
    clog<<"new graph edge#:"<<partEdgeNum<<endl;
    // #ifndef NO_DEBUG
    // cout << "part edge weight "<<endl<<"\t";
    // for(int i =0;i<partEdgeWeight.size();i++)
    //     cout << i << "\t";
    // cout << endl;
    // for(int i =0;i<partEdgeWeight.size();i++){
    //     cout << i << "\t";
    //     for(int j=0;j<partEdgeWeight[i].size();j++){
    //         cout << partEdgeWeight[i][j] << "\t";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // #endif

    // /* 写入文件 待修改*/
    // ofstream partWeightFile(strcpy(argv[2],".pw.csv"),ios::out);
    // partWeightFile << "分区号,分区权重" <<endl;
    // for(auto it = partWeight.begin();it != partWeight.end();it++){
    //     partWeightFile << (*it).first<<","<<(*it).second<<endl;
    // }
    // partWeightFile.close();


    //输出ibm格式的图文件
    //ibm cluster graph file
    char * cibmFilePath = argv[2];
    strcat(cibmFilePath,".info");
    ofstream partInfoFile(cibmFilePath,ios::out);
    partInfoFile <<partWeight.size()-1<< " " << partEdgeNum<<endl;
    //输出边
    for(size_t i=1;i<partEdgeWeight.size();i++){
        // if(inStart[i]== -1)
        //     continue;
        for(size_t j = 1;j<partEdgeWeight[i].size();j++){
            if( i == j  or partEdgeWeight[i][j]==-1)
                continue;
            partInfoFile << i<<" "<<j<<" "<<partEdgeWeight[i][j]<<endl;
        }
    }
    //输出点
     for(int i =1;i<partWeight.size();i++){
        partInfoFile << i<<" "<<partWeight[i]<<endl;
    }
    partInfoFile.close();
    clog << "new graph file save to "<<cibmFilePath<<endl;
    return 0;
}

void readGraphFromDot(char * filePath,vector<int> & weight){
    ifstream dotFile(filePath,ios::in);
    if(!dotFile.is_open())
    {
        cerr << "dot file not found !"<<endl;
        return ;
    }
    regex nodeAndWeight("^([\\d]+)[\\s]*;[\\s]*weight[\\s]*;[\\s]*([0-9]+)[\\s]*;[\\s]*$");
    regex edgeAndWeight("^([\\d]+)[\\s]*->[\\s]*([\\d]+)[\\s]*;[\\s]*weight[\\s]*;[\\s]*([\\d]+);[\\s]*$");
    smatch match;
    string buff;
    int temp=-1;
    bool nodeBegin=false,edgeBegin=false;
    stringstream ss;
    
    while(!dotFile.eof()){
        dotFile >> buff;
        if(edgeBegin)
        {
            break;
        }else if(nodeBegin)
        {
            if(regex_match(buff,match,nodeAndWeight))
            {
                ss << match[2].str();
                ss >> temp;
                weight.push_back(temp);
            }else{
                nodeBegin=false;
            }
        }else{
            if(regex_match(buff,match,nodeAndWeight))
            {
                nodeBegin=true;
                edgeBegin=false;
                ss << match[2].str();
                ss >> temp;
                weight.push_back(temp);
            }else if(regex_match(buff,match,edgeAndWeight)){
                nodeBegin=true;
            }else{
                continue;
            }
                
        }
        
    }
    dotFile.close();
}

void readGraphFromIbm(ifstream & ibmFile,vector<int> & inStart,vector<int> & inEnd,vector<int> & inEdge,vector<int> & inWeight,vector<int> & outStart,vector<int> & outEnd,vector<int> & outEdge,vector<int> & outWeight,vector<int> & weight,int & nodeNum,int & edgeNum){
    
    ibmFile >> nodeNum >> edgeNum;
    // clog << "read node :"<<nodeNum<<"\tedge :"<<edgeNum<<endl;
    inStart.assign(nodeNum+1,-1);
    inEnd.assign(nodeNum+1,-1);
    outStart.assign(nodeNum+1,-1);
    outEnd.assign(nodeNum+1,-1);
    weight.assign(nodeNum+1,-1);
    inWeight.clear();
    outWeight.clear();
    inEdge.clear();
    outEdge.clear();
    nodeNum=0;edgeNum=0;
    regex edgeAndWeight("^([\\d]+)[\\s]{1}([\\d]+)[\\s]{1}([\\d]+)[\\s]*$");
    regex nodeAndWeight("^([\\d]+)[\\s]{1}([\\d]+)[\\s]*$");
    smatch match;
    string buff;
    char tempBuff[1024];
    int temp=-1,sourceNode=-1,targetNode=-1,nodeid=-1;
    bool nodeBegin=false,edgeBegin=false;
    stringstream ss;
    
    while(!ibmFile.eof()){
        // ibmFile >> buff;
        ibmFile.getline(tempBuff,1024);
        #ifndef NO_DEBUG
        if(nodeNum % 5000 == 1 || edgeNum % 10000 == 1){
            clog << "read node :"<<nodeNum<<"\tedge :"<<edgeNum<<endl;
        }
        #endif
        buff = string(tempBuff);
        if(edgeBegin){
            if(regex_match(buff,match,edgeAndWeight)){
                
                // clog<<buff<<endl;
                ss.clear();
                ss << match[1].str();
                ss >> sourceNode;
                // clog<<sourceNode<<" ";
                if(inStart[sourceNode] == -1){
                    inStart[sourceNode] = inEdge.size();
                }
                ss.clear();
                ss << match[2].str();
                ss >> targetNode;
                // clog<<targetNode<<" ";
                inEdge.push_back(targetNode);
                ss.clear();
                ss << match[3].str();
                ss >> temp;
                // clog<<temp<<endl;
                inWeight.push_back(temp);
                edgeNum++;
                inEnd[sourceNode] = inEdge.size();
                continue ;
            }else{
                edgeBegin=false;
            }
        }
        if(nodeBegin){
            if(regex_match(buff,match,nodeAndWeight)){
                
                // clog<<buff<<endl;
                ss.clear();
                ss << match[2].str();
                ss >> temp;
                ss.clear();
                ss<<match[1].str();
                ss>>nodeid;
                weight[nodeid]=temp;
                nodeNum++;
                continue;
            }else{
                nodeBegin=false;
            }
        }
        if(regex_match(buff,match,nodeAndWeight)){
            nodeBegin=true;
            edgeBegin=false;
            ss.clear();
            ss << match[2].str();
            ss >> temp;
            ss.clear();
            ss<<match[1].str();
            ss>>nodeid;
            weight[nodeid]=temp;
            nodeNum++;
        }else if(regex_match(buff,match,edgeAndWeight)){
            edgeBegin=true;
            ss << match[1].str();
            ss >> sourceNode;
            if(inStart[sourceNode] == -1){
                inStart[sourceNode] = inEdge.size();
            }
            ss.clear();
            ss << match[2].str();
            ss >> targetNode;
            inEdge.push_back(targetNode);
            ss.clear();
            ss << match[3].str();
            ss >> temp;
            inWeight.push_back(temp);

            inEnd[sourceNode] = inEdge.size();
            edgeNum++;
        }
        
    }

    //计算out
    #ifndef NO_DEBUG
        clog<<"get parameters"<<endl;
    #endif
    //寻找每一个节点的out
    for(int i = 1;i<inStart.size();i++){
        for(int j = 1;j<inStart.size();j++){
            if(inStart[j] == -1||i==j)
                continue;
            for(int range = inStart[j] ; range< inEnd[j] ; range++){
                if(inEdge[range] == i){
                    if(outStart[i] == -1){
                        outStart[i] = outEdge.size();
                    }
                    outEdge.push_back(j);
                    outWeight.push_back(inWeight[range]);
                }
            }
            outEnd[i] = outEdge.size();
        }
        #ifndef NO_DEBUG
        if(i%100 == 0)
            clog<<"finish "<<i<<"\t tatal "<<nodeNum<<endl;
        #endif
    }
    #ifndef NO_DEBUG
        clog<<"finish parameters"<<endl;
    #endif

}

void getPartWeight(const vector<int> & partInfo,const vector<int> & nodeweight,map<int,int> & pw){

    // pair<int,int>::iterator it;
    for(int i =1;i < nodeweight.size() ; i++){
        auto it = pw.find(partInfo[i]);
        if(it==pw.end()){
            pw.insert({partInfo[i],nodeweight[i]});
            continue;
        }
        pw[partInfo[i]] = (*it).second + nodeweight[i];
    }
}

void getPartWeightnew(const vector<int> & partInfo,const vector<int> & nodeweight,vector<int> & pw){

    int temp=0;
    for(int i =1;i<partInfo.size();i++){
        temp = temp>partInfo[i] ?  temp:partInfo[i];
    }
    pw.assign(temp+1,-1);
    for(int i =1;i < nodeweight.size() ; i++){

        if(pw[partInfo[i]]==-1){
            pw[partInfo[i]]=nodeweight[i];
        }else{
            pw[partInfo[i]] = nodeweight[i]+pw[partInfo[i]];
        }
    }
}

void getPartEdgeWeight(const vector<int> & partInfo,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & inEdge,const vector<int> & inWeight,vector<vector<int> > & pew,int & pen){
    
    int targetNode=-1;
    for(size_t i=1;i<inStart.size();i++){
        if(inStart[i]== -1)
            continue;
        for(size_t j = inStart[i];j<inEnd[i];j++){
            targetNode = inEdge[j];
            if(partInfo[i]==partInfo[targetNode])
                continue;
            if(pew[partInfo[i]][partInfo[targetNode]] == -1){
                pew[partInfo[i]][partInfo[targetNode]] = inWeight[j];
                pen++;
            }else{
                pew[partInfo[i]][partInfo[targetNode]] +=1;
            }
        }
    }
}

void getPartEdgeWeightNew(const vector<int> & partInfo,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & inEdge,const vector<int> & inWeight,vector<vector<int> > & pew,int & pen){
    
    int targetNode=-1;
    for(size_t i=1;i<inStart.size();i++){
        if(inStart[i]== -1)
            continue;
        for(size_t j = inStart[i];j<inEnd[i];j++){
            targetNode = inEdge[j];
            if(partInfo[i]==partInfo[targetNode])
                continue;
            if(pew[partInfo[i]][partInfo[targetNode]] == -1){
                pew[partInfo[i]][partInfo[targetNode]] = inWeight[j];
                pen++;
            }else{
                if(pew[partInfo[i]][partInfo[targetNode]]<inWeight[j]){
                    pew[partInfo[i]][partInfo[targetNode]] = inWeight[j];
                }
            }
        }
    }
}

void getGraphDistribution(const char * filePath,const vector<int> & inStart,const vector<int> & inEnd,const vector<int> & outStart,const vector<int> & outEnd){
    ofstream disfile(filePath,ios_base::out);
    disfile << "node,out,in,degree" <<endl;
    int out,in;
    
    for(size_t i = 1;i<outStart.size();i++){
        // disfile << i<<",";
        if(outStart[i]==-1)
            out = 0;
        else
            out = outEnd[i]-outStart[i];
        
        if(inStart[i]==-1)
            in = 0;
        else
            in = inEnd[i]-inStart[i];
        disfile << i <<","<<out<<","<<in<<","<<out+in<<endl;
    }
    disfile.close();
}







