#include"def.h"

vector<vector<float>> t_P;
vector<vector<float>> t_Q;
vector<Rating> rate;
vector<Rating> rate_bymovie;
int LEN = 100004;
//100004;
int NUser = 671;
//671;
int NMovie = 9125;
//9125;
int NOW = 0, NOW_M = 0;
int *MovieId = new int [NMovie];

bool Tool_sort::judge_movie(Rating rate){
    if(rate.m == NOW_M) return 1;
    return 0;
}
bool Tool_sort::judge_cov(Cov_user cov_u){
    if(cov_u.partner == NOW) return 1;
    return 0;
}
bool Tool_sort::judge_movieId(vector<float> Q){
    if(Q[0] == NOW_M) return 1;
    return 0;
}

bool Tool_sort::judge_user(Rating rate){
    if(rate.u == NOW) return 1;
    return 0;
}

UserCF::UserCF(int a){
    {
        cout<<"s";
        tcpServer = new QTcpServer();
        tcpSocket = new QTcpSocket();
        if (!tcpServer->listen(QHostAddress::Any, 6666)) {
                    cout<< "failse";
       }
        cout<<"success";
        connect(tcpServer,SIGNAL(newConnection()),
                this,SLOT(newConnect()));

    }
}
void UserCF::newConnect()
{
    qDebug() << "connected";
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << "binding";
    connect(tcpSocket,SIGNAL(readyRead()),
            this,SLOT(readMessage()));
    //connect(tcpSocket,SIGNAL(disconnected()),
      //          tcpSocket,SLOT(deleteLater()));
}
void UserCF::readMessage()
{
    QString str = tcpSocket->readAll();
    user_id=str.toInt();
    cout<<user_id;
    set_user(user_id);
    cout<<"32";
    cov_user();
    cout << "user: userId cov" << endl;
    print_user();
    cout << "movie: movieId UserCF_rate LFM_rate mean_rate" << endl;
    print_movie();
    sendMessage();
}
void UserCF::sendMessage()
{
    QString Movie1,Movie2,Movie3,Movie4,Movie5,Movie6,Movie7,Movie8,Movie9,Movie10;
    QString User1,User2,User3,User4,User5,User6,User7,User8,User9,User10;
    User1=QString::number(cov_u[0].partner);
    User2=QString::number(cov_u[1].partner);
    User3=QString::number(cov_u[2].partner);
    User4=QString::number(cov_u[3].partner);
    User5=QString::number(cov_u[4].partner);
    User6=QString::number(cov_u[5].partner);
    User7=QString::number(cov_u[6].partner);
    User8=QString::number(cov_u[7].partner);
    User9=QString::number(cov_u[8].partner);
    User10=QString::number(cov_u[9].partner);
    Movie1=QString::number(pre_movie[0].m);
    Movie2=QString::number(pre_movie[1].m);
    Movie3=QString::number(pre_movie[2].m);
    Movie4=QString::number(pre_movie[3].m);
    Movie5=QString::number(pre_movie[4].m);
    Movie6=QString::number(pre_movie[5].m);
    Movie7=QString::number(pre_movie[6].m);
    Movie8=QString::number(pre_movie[7].m);
    Movie9=QString::number(pre_movie[8].m);
    Movie10=QString::number(pre_movie[9].m);
    rcm_list<<User1<<User2<<User3<<User4<<User5<<User6<<User7<<User8<<User9<<
                 User10<<Movie1<<Movie2<<Movie3<<Movie4<<Movie5<<Movie6<<Movie7<<Movie8<<
                 Movie9<<Movie10;
    RCM = rcm_list.join(",");
    QByteArray datagram = RCM.toUtf8();
    tcpSocket->write(datagram);
    rcm_list.clear();
    pre_movie.clear();
    cov_u.clear();
    u_rating.clear();
}


void UserCF::create_u(vector<Rating>::iterator &iter){
    float total_rate = 0;
    for(int i = 0; i < n_u; i ++ ){
        total_rate = total_rate + iter -> rating;
        Rating r{u, iter -> m, iter -> rating};
        u_rating.push_back(r);

        iter ++;
    }
    mean_u = 1.0 * total_rate / n_u;
}

void UserCF::create_i(vector<Rating>::iterator &iter){
    int j = 0, count = 0, temp_n = 0, temp_user = iter -> u;
    cout<<"temp_user"<<temp_user;
    float total_rate = 0, mean_i;
    //临时使用的用户i信息
    vector<Cross_rating> cross_rating;

    while(iter -> u == temp_user){

        if(u_rating[j].m > iter -> m || j > n_u - 1){
            total_rate = total_rate + iter -> rating;
            iter ++;
            temp_n ++;

        }else if(u_rating[j].m == iter -> m || j > n_u - 1){
            total_rate = total_rate + iter -> rating;

            Cross_rating r{iter -> m, u_rating[j].rating, iter -> rating};
            cross_rating.push_back(r);

            temp_n ++;
            count ++;
            iter ++;
            j ++;
        }else {
            j ++;
        }
        if(iter -> u == 0) break;
        //判断iter为空
    }
    if(count < 10) return;
    mean_i = 1.0 * total_rate / temp_n;

    float rt_u = 0, rt_i = 0, rt = 0, temp_cov;
    Rate temp_u, temp_i;
    for (j = count - 1; j >=0; j--){
        temp_u = cross_rating[j].u_rate - mean_u;
        temp_i = cross_rating[j].i_rate - mean_i;
        rt_u = rt_u + temp_u * temp_u;
        rt_i = rt_i + temp_i * temp_i;
        rt = rt + temp_u * temp_i;
    }
    temp_cov = 1.0 * rt / sqrt(rt_u * rt_i);

    Cov_user c{temp_user, temp_cov, mean_i};
    cov_u.push_back(c);
}

void UserCF::cov_user(){
    int i;
    NOW = u;
    vector<Rating>::iterator iter = find_if ( rate.begin (), rate.end (), Tool_sort::judge_user);
    n_u = count_if(rate.begin(), rate.end(), Tool_sort::judge_user);
    cout<<"2"<<endl;
    create_u(iter);
    iter = rate.begin();
    cout<<"3"<<endl;
    for(i = 1; i <= NUser; i++){
        if(i == u){
            iter = iter + n_u;
            cout<<"3.0"<<endl;
            continue;
        }
        cout<<"3.5"<<endl;
        create_i(iter);
    }
    cout<<"4"<<endl;
    sort(cov_u.begin(), cov_u.end(), Tool_sort::sort_cov);
    cout<<"5"<<endl;
    pre_ratings();
    n_u = 0;
}


int UserCF::judge_new(Movie v){
    NOW_M = v;
    if(find_if(u_rating.begin(), u_rating.end(), Tool_sort::judge_movie) == u_rating.end() ){
        return 1;
    }//未找到且结尾不是v则为新电影
    return 0;
}

int UserCF::find_cov(float &tmp_cov, float &tmp_mean){//电影v和本条iter_u中的userid是否在 cov_u中
    vector<Cov_user>::iterator iter_cov;
    iter_cov = find_if(cov_u.begin(), cov_u.end(), Tool_sort::judge_cov);

    if(iter_cov == cov_u.end() ){
        return 0;
    }//未找到且结尾不是u则未评分

    tmp_cov = iter_cov -> cov;
    tmp_mean = iter_cov -> mean_rate;
    return 1;
}



void UserCF::create_movie(vector<Rating>::iterator &iter){
    Movie v = iter -> m;
    float tmp_cov = 0, tmp_mean = 0, total_rate = 0, total_wr = 0,total_w = 0;
    float pre, mean_movie;
    int count = 0, num = 0;
    //临时使用的用户i信息
    vector<Rating> tmp_movie_rating;
    vector<Cov_user>::iterator iter_cov;
    vector<Rating>::iterator tmp_iter;


    if(!judge_new(v)){
        while (iter -> m == v ) {
            iter ++;
            if(iter -> u == 0) return ;
        }
        return ;
    }

    while (iter -> m == v ) {
        count ++;
        total_rate = total_rate + iter -> rating;

        Rating r{iter -> u, iter -> m, iter -> rating};
        tmp_movie_rating.push_back(r);

        if((iter + 1) -> u == 0) break;
        iter ++;
    }

    iter_cov = cov_u.begin();

    while (1) {
        NOW = iter_cov -> partner;
        tmp_iter = find_if(tmp_movie_rating.begin(), tmp_movie_rating.end(), Tool_sort::judge_user);

        if(tmp_iter == tmp_movie_rating.end() ){//未找到且结尾不是,则未评分

        }else{
            total_wr = total_wr + iter_cov -> cov * (tmp_iter -> rating - iter_cov -> mean_rate);
            if(iter_cov -> cov > 0){
                total_w = total_w + iter_cov -> cov;
            }else{
                total_w = total_w - iter_cov -> cov;
            }
            //t_cov.push_back(*iter_cov);
            num ++;
        }
        if((iter_cov + 1) == cov_u.end() || num == 10) break;
        iter_cov ++;

    }

    if(num < 10) return;
    pre = mean_u + 1.0 * total_wr / total_w;

    Pre_movie p{v, pre, 0, 0};
    pre_movie.push_back(p);
}

void UserCF::pre_ratings(){
    vector<Rating>::iterator iter = rate_bymovie.begin();
    while (1) {
        create_movie(iter);
        if((iter + 1) -> u == 0 || iter -> u == 0) break;
    }

    pre_LFM();
    sort(pre_movie.begin(), pre_movie.end(), Tool_sort::sort_pre);
}


void UserCF::pre_LFM(){
    vector<Pre_movie>::iterator iter = pre_movie.begin(); //遍历评分表
    vector<vector<float>>::iterator q;
    Movie v;
    int k;
    float pre;

    while (1) {
        v = iter -> m;
        /////////////////////读取PQ
        NOW_M = v;
        q = find_if(t_Q.begin(), t_Q.end(), Tool_sort::judge_movieId);

        pre = 0;
        for(k = 0; k < 10; k++){
            pre = pre + t_P[u][k] * *(q[0].data() + k + 1);
        }

        iter -> LFM_rate = pre;
        iter -> mean_rate = (iter -> pre_rate + pre) / 2;

        iter ++;
        if(iter  == pre_movie.end()) break;
    }
}


void UserCF::print_user(){
    int i;
    for(i = 0; i < 10; i++){
        cout << cov_u[i].partner << " " << cov_u[i].cov << endl;
    }
}
void UserCF::print_movie(){
    int i;
    for(i = 0; i < 10; i++){
        cout << pre_movie[i].m << " " << pre_movie[i].pre_rate << " " << pre_movie[i].LFM_rate << " " << pre_movie[i].mean_rate << endl;
    }
}

LFM::LFM(){
    int i, j;
    P.resize(NUser);
    Q.resize(NMovie);
    srand(time(NULL));

    for(i = 0; i < NUser; i++){
        P[i].resize(K);
        for(j = 0; j < K; j++){
            P[i][j] = rand()%(N_w+1)/(float)(N_w+1) * 3.76 / sqrt(K);
        }
    }

    for(i = 0; i < NMovie; i++){
        Q[i].resize(K + 1);
        Q[i][0] = MovieId[i];

        for(j = 1; j < K + 1; j++){
            Q[i][j] = rand()%(N_w+1)/(float)(N_w+1) * 3.76 / sqrt(K);
        }
    }
    print_P_Q();
}



void LFM::LFM_ALL(){
    int time, i, j, k, n;
    float p, para, alpha1, alpha2, y, pre, t1, t2, tmp;
    User user;
    Movie mv;
    vector<Rating>::iterator iter;
    vector<vector<float>>::iterator qv;

    for(time = 1; time < 4; time++){
        iter = rate.begin();
        p = 0.9937607;
        cout << p << endl;
        alpha2 = 0.1;
        int z = 0, zz = 0, point = 1;;

        for(j = 1; j <= NUser; j++){
            z++;
            cout << "User : " << z <<endl;

            NOW = iter -> u;
            n = count_if(rate.begin(), rate.end(), Tool_sort::judge_user);
            para = powf(0.003, 1.0/n);

            alpha1 = 0.073;

            for(i = 0; i < n; i++){
                zz ++ ;
                //cout << "Rating: " << zz << endl;
                user = iter -> u - 1;

                mv = iter -> m;
                y = iter -> rating;

                NOW_M = mv;
                qv = find_if(Q.begin(), Q.end(), Tool_sort::judge_movieId);

                pre = 0;
                for(k = 0; k < 10; k++){
                    //cout << *(qv[0].data() + k + 1);
                    pre = pre + P[user][k] * *(qv[0].data() + k + 1);
                }

                t1 = alpha1 * (y - pre);
                t2 = alpha2 * (y - pre);
                for(k = 0; k < 10; k++){
                    tmp = P[user][k];
                    P[user][k]  =  P[user][k] + t1 * *(qv[0].data() + k + 1);
                    *(qv[0].data() + k + 1) = *(qv[0].data() + k + 1) + t2 * P[user][k];

                }
                alpha1 = alpha1 * para;

                if((iter + 1) -> u == 0) {
                    cout << "True!" << endl;
                    point = 0;
                    break;
                }
                iter ++;
                if(iter -> u != NOW) break;
            }
            if(point == 0) break;
            alpha2 = p * alpha2;
        }
        //LFM_MSE();
    }
    /////////////
    t_P.resize(NUser);
    t_Q.resize(NMovie);

    for(i = 0; i < NUser; i++){
        t_P[i].resize(K);
        for(j = 0; j < K; j++){
            t_P[i][j] = P[i][j];
        }
    }

    for(i = 0; i < NMovie; i++){
        t_Q[i].resize(K + 1);
        for(j =0; j < K + 1; j++){
            t_Q[i][j] = Q[i][j];
        }
    }
    print_P_Q();
    //LFM_MSE();
}

void LFM::print_P_Q(){
    int i, j;
    cout << "矩阵P" << endl;
    for(i = 0; i < 5; i++){
        for(j = 0; j < 10; j++){
            cout << P[i][j] << ", " ;
        }
        cout << endl;
    }
    cout << "矩阵Q" << endl;
    for(i = 0; i < 5; i++){
        for(j = 1; j < 10 + 1; j++){
            cout << Q[i][j] << ", " ;
        }
        cout << endl;
    }
}

void LFM::LFM_MSE(){
    int k;
    float y, pre, mse = 0, count = 0, prt = 1;
    User user;
    Movie mv;
    vector<Rating>::iterator iter;
    vector<vector<float>>::iterator qv;

    iter = rate.begin();
    while (1) {
        count ++ ;
        if(count / 10000 == prt){
            cout << "Row: " << prt << endl;
            prt ++;
        }
        user = iter -> u;
        mv = iter -> m;
        y = iter -> rating;

        NOW_M = mv;
        qv = find_if(Q.begin(), Q.end(), Tool_sort::judge_movieId);

        pre = 0;
        for(k = 0; k < 10; k++){
            //cout << *(qv[0].data() + k + 1);
            pre = pre + P[user - 1][k] * *(qv[0].data() + k + 1);
        }

        mse = mse + (pre - y) * (pre - y);

        if((iter + 1) -> u == 0) break;
        iter ++;
    }

    cout << "MSE: " << mse << endl;
}
