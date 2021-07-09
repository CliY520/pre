/*
 * @Author: your name
 * @Date: 2021-07-05 22:47:36
 * @LastEditTime: 2021-07-06 07:50:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /hwg/pre/fileRead.cpp
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

void readFromDot(char * filePath,vector<int> & weight){
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

void readFromIbm(ifstream & ibmFile,
vector<int> & inStart,vector<int> & inEnd,vector<int> & inEdge,vector<int> & inWeight,
vector<int> & outStart,vector<int> & outEnd,vector<int> & outEdge,vector<int> & outWeight,
vector<int> & weight,int & nodeNum,int & edgeNum){
    
    ibmFile >> nodeNum >> edgeNum;
    // clog << "read node :"<<nodeNum<<"\tedge :"<<edgeNum<<endl;
    inStart.assign(nodeNum+1,0);
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
                if(outStart[sourceNode] == -1){
                    outStart[sourceNode] = outEdge.size();
                }
                ss.clear();
                ss << match[2].str();
                ss >> targetNode;
                // clog<<targetNode<<" ";
                outEdge.push_back(targetNode);
                ss.clear();
                ss << match[3].str();
                ss >> temp;
                // clog<<temp<<endl;
                outWeight.push_back(temp);
                edgeNum++;
                outEnd[sourceNode] = outEdge.size();
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
            if(outStart[sourceNode] == -1){
                outStart[sourceNode] = outEdge.size();
            }
            ss.clear();
            ss << match[2].str();
            ss >> targetNode;
            outEdge.push_back(targetNode);
            ss.clear();
            ss << match[3].str();
            ss >> temp;
            outWeight.push_back(temp);

            outEnd[sourceNode] = outEdge.size();
            edgeNum++;
        }
        
    }

    /*  计算out  */
    
    inEdge.assign(outEdge.size(),-1);
    inWeight.assign(outEdge.size(),-1);
    //count
    for(int j = 1;j<outStart.size();j++){
        if(outStart[j]==-1) continue;
        for(int i=outStart[j];i<outEnd[j];i++){
            inStart[outEdge[i]]++;
        }
    }
    //prefix sum
    for(int j =1 ;j<inStart.size();j++){
        inStart[j]+=inStart[j-1];
    }

    int at =-1;
    //fill
    for(int j =1;j<outStart.size();j++){
        for( int i = outStart[j];i<outEnd[j];i++){
            if(inEnd[outEdge[i]]==-1)
                inEnd[outEdge[i]]=inStart[outEdge[i]];
            at = --inStart[outEdge[i]];
            inEdge[at] = j;
            inWeight[at] = outWeight[i];
        }
    }
    for(int j =1;j<inEnd.size();j++){
        if(inEnd[j]==-1) inStart[j]=-1;
    }
    clog<<"finished ibm file read"<<endl;
}
