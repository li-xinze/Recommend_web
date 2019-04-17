#ifndef LEVELDBAPI_H
#define LEVELDBAPI_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <leveldb/db.h>
#include <QApplication>
#include <QtNetwork>
#include <QDialog>

using namespace std;

class LeveldbAPI : public QObject
{
    Q_OBJECT
public:
    LeveldbAPI(string name);
    void ImportData();  //导入所有数据
    void ImportMovie(); //导入电影数据库的数据
    void ImportLink();  //导入电影链接号
    void ImportTmdb_Title();    //储存Tmdb和Title的关系
    void ImportUser();  //导入用户数据库的数据
    void StoreInformation();    //提取特定信息到vector数组中

    void Get();     //获取（未完善）
    void Put();
    void Delete();
    void Search();
    void ShowAll();
    void ShowPart();

    void setName(string name);  //设置数据库名称
    string getName();       //获取数据库名称
    string getDir();        //获取数据库存放路径
    void setAllNames();     //判断现在的_DBName是否在_DBNames数组里，若没有，则存入
    static void getAllNames(); //获取所有的数据库名称
    static void NamestoTxt();  //vector数组存入文件
    static void TxttoVec();    //文件导入vector数组

    bool isExist();     //判断数据库是否已经存在
    string Trim(string &str);

private slots:
    void newConnect();
    void readMessage();

private:
    void StoreNewRating(string& a,string& b,string& c);
    void sendMessage();
    string SwitchTmdb_Movie(string a);
    vector< vector<string> > Info;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    string _DBname;     //数据库名称
    string _DBDir;      //数据库存储路径
    static vector<string>_DBNames;  //所有的数据库名称
};


#endif // LEVELDBAPI_H
