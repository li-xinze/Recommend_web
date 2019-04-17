#ifndef DEF_H
#define DEF_H

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <QString>
#include <QApplication>
#include <QtNetwork>
#include <QDialog>
#define K 10
#define N_w 9999 //4位小数。

class QUdpSocket;
using namespace std;
typedef int User;
typedef int Movie;
typedef float Rate;
typedef float Cov;
//定义数据维度

typedef struct rating{       //评分表
    User u;
    Movie m;
    float rating;

}Rating;

typedef struct cross_rating{ //交叉电影评分
    Movie m;
    Rate u_rate;
    Rate i_rate;
}Cross_rating;

typedef struct cov_user{     //用户相关性
    User partner;
    Cov cov;
    Rate mean_rate;
}Cov_user;

typedef struct pre_movie{    //评分预测
    Movie m;
    Rate pre_rate;
    Rate LFM_rate;
    Rate mean_rate;
}Pre_movie;

extern vector<Rating> rate;
extern vector<Rating> rate_bymovie;
extern int *MovieId;

class UserCF : public QObject
{
     Q_OBJECT
public:
    UserCF(int a);
    void cov_user();              //计算用户之间的相关性
    void create_u(vector<Rating>::iterator &iter);       //读取用户u的评分记录
    void create_i(vector<Rating>::iterator &iter);     //读取用户i与u评分相同电影的列表
    void count_similar();         //计算最相似的10个用户
    int judge_new(Movie v);       //判断是否未看过
    void create_movie(vector<Rating>::iterator &iter); //读取一部电影所有评分信息
    void update_movie(Movie v, float pre); //更新推荐电影
    int find_cov(float &tmp_cov, float &tmp_mean);                        ////电影v和本条iter中的userid是否在 cov_u中
    void pre_ratings();
    void pre_LFM();
    void print_user();
    void print_movie();
    void set_user(int userid){
        this->u = userid;
    }
private slots:
    void newConnect();
    void readMessage();
protected:
    User u;
    vector<Rating> u_rating;
    int n_u = 0;              //用户u评分的电影部数
    float mean_u = 0;         //用户u评分均值
    //用户相关性的储存
    vector<Cov_user> cov_u;
    //电影预测储存
    vector<Pre_movie> pre_movie;


private:


    int user_id;
    void sendMessage();
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QStringList rcm_list;
    QString RCM;
};
class LFM{
public:
    LFM();
    void LFM_ALL();
    void LFM_MSE();
    void print_P_Q();
    ////存储P,Q

    vector<vector<float>> P;
    vector<vector<float>> Q;
};

class Tool_sort{
public:
    static bool judge_user(Rating rrr);
    static bool judge_movie(Rating rrr);
    static bool judge_cov(Cov_user cov_u);
    static bool judge_movieId(vector<float> Q);
    static bool sort_cov(Cov_user u, Cov_user v){
        return (u.cov>v.cov);
    }
    static bool sort_pre(Pre_movie m1, Pre_movie m2){
        return (m1.mean_rate > m2.mean_rate);
    }
    static bool sort_bymovie(Rating m1, Rating m2){
        if(m1.m < m2.m) return 1;
        else if(m1.m == m2.m && m1.u < m2.u) return 1;
        return 0;
    }
    static bool sort_byuser(Rating m1, Rating m2){
        if(m1.u < m2.u) return 1;
        else if(m1.u == m2.u && m1.m < m2.m) return 1;
        return 0;
    }
};


#endif // DEF_H
