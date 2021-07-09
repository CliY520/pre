/*
 * @Author: your name
 * @Date: 2021-07-05 22:46:36
 * @LastEditTime: 2021-07-05 23:41:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /hwg/pre/fileRead.h
 */
#ifndef FILE_READ_H
#define FILE_READ_H
using namespace std;
/*
 * 从dot格式的文件中读取图，未完成
 */
void readFromDot(char * filePath,vector<int> & weight);

/*
 * 从ibm格式的文件中读取图
 */
void readFromIbm(ifstream & ibmFile,vector<int> & inStart,vector<int> & inEnd,vector<int> & inEdge,vector<int> & inWeight,vector<int> & outStart,vector<int> & outEnd,vector<int> & outEdge,vector<int> & outWeight,vector<int> & weight,int & nodeNum,int & edgeNum);

#endif

