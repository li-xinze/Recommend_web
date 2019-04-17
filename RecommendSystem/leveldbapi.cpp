#include "leveldbapi.h"
#include<QStringList>
#include<QString>

LeveldbAPI::LeveldbAPI(string name)
{
    _DBname = name;
    _DBDir = "/" + _DBname;     //存放路径即为数据库名称
    tcpServer = new QTcpServer();
    tcpSocket = new QTcpSocket();
    if (!tcpServer->listen(QHostAddress::Any, 8888)) {
                cout<< "false";
   }
    cout<<"success";
    connect(tcpServer,SIGNAL(newConnection()),
            this,SLOT(newConnect()));
}

void LeveldbAPI::ImportData(){

    if(_DBname == "Movie"){
        ImportMovie();
        ImportLink();
        ImportTmdb_Title();
    }

    if (_DBname == "User"){
        ImportUser();
    }
}
void LeveldbAPI::newConnect()
{
    qDebug() << "connected";
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << "binding";
    connect(tcpSocket,SIGNAL(readyRead()),
            this,SLOT(readMessage()));
}
void LeveldbAPI::readMessage()
{
    QString str = tcpSocket->readAll();
    string strs = str.toStdString();
    cout<<strs;
    sendMessage(str);
}
void LeveldbAPI::sendMessage(QString keyword){
    vector<string>number;
    int size =Info[1].size();
    cout<<size;
    for( int a = 0; a < size; a = a + 1 ) {
       string s= Info[1][a];
       int position;
       string Keyword;
       Keyword=keyword.toStdString();
       position= s.find(Keyword);
       if (position!= -1){
            number.insert(number.end(),Info[0][a]);
       }
    }
    int n_size = number.size();
    for(int i =0 ;i<n_size; i++){
       cout<<number[i]<<endl;
    }
    QStringList id_list;
    for(int i = 0 ; i<n_size; i++){
        QString id = QString::fromStdString(number[i]);
        id_list<<id;
    }
    QString Id_List = id_list.join(",");
    QByteArray datagram = Id_List.toUtf8();
    tcpSocket->write(datagram);
    cout<<"already sent";
    id_list.clear();
    number.clear();
}
void LeveldbAPI::ImportMovie(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = true;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, getDir(), &db);
    assert(status.ok());

    ifstream fin("../ml-20m/ml-20m/newnewmovies.txt");
    string line;
    while (getline(fin, line)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        int len = fields.size();    //计算fields里的元素个数
        cout << len <<endl;
        string MovieID = fields[0]; //MovieID为第一个逗号前的内容
        stringstream MT;
        for (int i = 1; i < (len-2) ;i++){
            MT << fields[i] << ","; //保留原标题中的逗号，不多不少
        }
        MT << fields[len-2];
        string MovieTitle = MT.str();       //MovieTitle包含第一个逗号到最后一个逗号之间的全部内容
        string MovieGenres = fields[len-1]; //MovieGenres为最后一个逗号后的内容

        stringstream ss1;
        ss1 << "Movie" << MovieID << ":" << "Title";
        string key1 = ss1.str();
        stringstream ss2;
        ss2 << "Movie" << MovieID << ":" << "Genres";
        string key2 = ss2.str();

        std::string value1 = MovieTitle;
        std::string value2 = MovieGenres;
        std::string get_value1;
        std::string get_value2;


        /*cout<<MovieID<<endl;
        cout<<MovieTitle<<endl;
        cout<<MovieGenres<<endl;
        cout<<key1<<key2<<endl;*/  //检测模块


        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);
        leveldb::Status s2 = db->Put(leveldb::WriteOptions(), key2, value2);

        // 写入成功，就读取 key:people 对应的 value
        if ( (s1.ok()) && (s2.ok()) )
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);
            s2 = db->Get(leveldb::ReadOptions(), key2, &get_value2);

        // 读取成功就输出
        if ( (s1.ok()) && (s2.ok()) ){
            cout << get_value1 << endl;
            cout << get_value2 << endl;

        }
        else
            cout << s1.ToString() << s2.ToString() << endl;
    }
    delete db;
}
void LeveldbAPI::ImportLink(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = true;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, "/Link", &db);
    assert(status.ok());

    ifstream fin("../ml-20m/ml-20m/links.txt");
    string line;
    while (getline(fin, line)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        string MovieID = fields[0];     //MovieID为第一个逗号前的内容
        string MovieLink = fields[2];       //MovieLink包含tmdb号码

        string key1 = MovieID;
        string value1 = MovieLink;
        string get_value1;

        /*cout<<MovieID<<endl;
        cout<<MovieTitle<<endl;
        cout<<MovieGenres<<endl;
        cout<<key1<<key2<<endl;*/  //检测模块

        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);

        // 写入成功，就读取 key:people 对应的 value
        if ( s1.ok() ){
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);
        }
        // 读取成功就输出
        if ( s1.ok() ){
            cout << get_value1 << endl;
        }
        else
            cout << s1.ToString() << endl;
    }
    delete db;
}

void LeveldbAPI::ImportTmdb_Title(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = true;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, "/Tmdb_Title", &db);
    assert(status.ok());

    ifstream fin("../ml-20m/ml-20m/tmdb_title.txt");
    string line;
    while (getline(fin, line)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        int len = fields.size();
        string MovieTmdb = fields[0];     //MovieTmdb为第一个逗号前的内容
        stringstream MT;
        for (int i = 1; i < (len-1) ;i++){
            MT << fields[i] << ",";     //保留原标题中的逗号，不多不少
        }
        MT << fields[len-1];
        string MovieTitle = MT.str();       //MovieTitle包含第一个逗号后的全部内容

        string key1 = MovieTmdb;
        string value1 = MovieTitle;
        string get_value1;

        /*cout<<MovieID<<endl;
        cout<<MovieTitle<<endl;
        cout<<MovieGenres<<endl;
        cout<<key1<<key2<<endl;*/  //检测模块

        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);

        // 写入成功，就读取 key:people 对应的 value
        if ( s1.ok() ){
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);
        }
        // 读取成功就输出
        if ( s1.ok() ){
            cout << get_value1 << endl;
        }
        else
            cout << s1.ToString() << endl;
    }
    delete db;
}

void LeveldbAPI::ImportUser(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = true;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, getDir(), &db);
    assert(status.ok());

    ifstream fin("../ml-20m/ml-20m/ratings1.txt");
    string line;
    while (getline(fin, line)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        string UserID = Trim(fields[1]); //清除掉向量fields中第一个元素的无效字符，并赋值给变量UserID
        string MovieID = Trim(fields[2]); //清除掉向量fields中第二个元素的无效字符，并赋值给变量MovieID
        string Rating = Trim(fields[3]); //清除掉向量fields中第三个元素的无效字符，并赋值给变量Rating

        stringstream ss1;
        ss1 << "User" << UserID << ":" << "Movie" << MovieID << ":" << "Rating";
        string key1 = ss1.str();


        std::string value1 = Rating;

        std::string get_value1;

        /*cout<<UserID<<endl;
        cout<<MovieID<<endl;
        cout<<Rating<<endl;
        cout<<key1<<endl;*/


        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);


        // 写入成功，就读取 key:people 对应的 value
        if  ( s1.ok() )
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);


        // 读取成功就输出
        if ( s1.ok() ) {
            cout << get_value1 << endl;
        }
        else
            cout << s1.ToString() << endl;
    }

    ifstream fin2("../ml-20m/ml-20m/ratings2.txt");
    string line2;
    while (getline(fin2, line2)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line2);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        string UserID = Trim(fields[1]); //清除掉向量fields中第一个元素的无效字符，并赋值给变量UserID
        string MovieID = Trim(fields[2]); //清除掉向量fields中第二个元素的无效字符，并赋值给变量MovieID
        string Rating = Trim(fields[3]); //清除掉向量fields中第三个元素的无效字符，并赋值给变量Rating

        stringstream ss1;
        ss1 << "User" << UserID << ":" << "Movie" << MovieID << ":" << "Rating";
        string key1 = ss1.str();


        std::string value1 = Rating;

        std::string get_value1;

        /*cout<<UserID<<endl;
        cout<<MovieID<<endl;
        cout<<Rating<<endl;
        cout<<key1<<endl;*/


        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);


        // 写入成功，就读取 key:people 对应的 value
        if  ( s1.ok() )
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);


        // 读取成功就输出
        if ( s1.ok() ) {
            cout << get_value1 << endl;
        }
        else
            cout << s1.ToString() << endl;
    }


    ifstream fin3("../ml-20m/ml-20m/ratings3.txt");
    string line3;
    while (getline(fin3, line3)){      //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
        istringstream sin(line3);    //将整行字符串line读入到字符串流istringstream中
        vector<string> fields;     //声明一个字符串向量
        string field;

        while (getline(sin, field, ',')) {    //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
            fields.push_back(field);    //将刚刚读取的字符串添加到向量fields中
        }

        string UserID = Trim(fields[1]); //清除掉向量fields中第一个元素的无效字符，并赋值给变量UserID
        string MovieID = Trim(fields[2]); //清除掉向量fields中第二个元素的无效字符，并赋值给变量MovieID
        string Rating = Trim(fields[3]); //清除掉向量fields中第三个元素的无效字符，并赋值给变量Rating

        stringstream ss1;
        ss1 << "User" << UserID << ":" << "Movie" << MovieID << ":" << "Rating";
        string key1 = ss1.str();


        std::string value1 = Rating;

        std::string get_value1;

        /*cout<<UserID<<endl;
        cout<<MovieID<<endl;
        cout<<Rating<<endl;
        cout<<key1<<endl;*/


        // 写入 key1 -> value1
        leveldb::Status s1 = db->Put(leveldb::WriteOptions(), key1, value1);


        // 写入成功，就读取 key:people 对应的 value
        if  ( s1.ok() )
            s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);


        // 读取成功就输出
        if ( s1.ok() ) {
            cout << get_value1 << endl;
        }
        else
            cout << s1.ToString() << endl;
    }
    delete db;
}

void LeveldbAPI::StoreInformation(){
    Info.resize(2);
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 创建的数据库在 //Tmdb_Title
    leveldb::Status status = leveldb::DB::Open(options, "/Title", &db);
    assert(status.ok());

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    it->SeekToFirst();
    for (it->SeekToFirst(); it->Valid(); it->Next()){
        Info[0].push_back(it->key().ToString());
        Info[1].push_back(it->value().ToString());
    }
    assert(it->status().ok());
    cout<<Info[0][6]<<endl;
    cout<<Info[1][6]<<endl;

    delete it;
    delete db;
}

void LeveldbAPI::Get(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = false;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, getDir(), &db);
    assert(status.ok());

    if(_DBname == "Movie"){
        stringstream ss1;
        ss1 << "131138" ;        //此处增加搜索功能
        string key1 = ss1.str();
        string get_value1;

        cout<<key1;   //临时检测

        leveldb::Status s1 = db->Get(leveldb::ReadOptions(), key1, &get_value1);

        // 读取成功就输出
        if (s1.ok()){
            cout << get_value1  << endl;    
        }
        else{
            cout << s1.ToString() << endl;
        }
    }
    else if (_DBname == "User"){
        stringstream ss2;
        ss2 << "User" << 138493 << ":" << "Movie" << 70286 << ":" << "Rating";   //此处增加搜索功能
        string key2 = ss2.str();
        string get_value2;

        cout<<key2;   //临时检测

        leveldb::Status s2 = db->Get(leveldb::ReadOptions(), key2, &get_value2);

        // 读取成功就输出
        if (s2.ok()){
            cout << get_value2  << endl;
        }
        else{
            cout << s2.ToString() << endl;
        }
    }
    delete db;
}

void LeveldbAPI::Put(){

}

void LeveldbAPI::Delete(){

}

void LeveldbAPI::Search(){

}

void LeveldbAPI::ShowAll(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = false;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, getDir(), &db);
    assert(status.ok());

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;
    delete db;
}

void LeveldbAPI::ShowPart(){
    leveldb::DB *db = nullptr;
    leveldb::Options options;

    // 如果数据库不存在就创建
    options.create_if_missing = false;

    // 创建的数据库在 /_DBDir
    leveldb::Status status = leveldb::DB::Open(options, getDir(), &db);
    assert(status.ok());

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    string start = "User138493:Movie1:Rating";
    string limit = "User138493:Movie71619:Rating";
    for ( it->Seek(start);
          it->Valid() && it->key().ToString() < limit;
          it->Next() ) {
        cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;
    delete db;
}

string LeveldbAPI::getName(){
    return _DBname;
}

void LeveldbAPI::setName(string name){
    _DBname = name;
}

string LeveldbAPI::getDir(){
    return _DBDir;
}

void LeveldbAPI::setAllNames(){
    vector<string>::iterator iter=std::find(_DBNames.begin(),_DBNames.end(),_DBname); //返回的是一个迭代器指针
    if ( iter ==_DBNames.end() ){ //vector数组中没有该元素
        _DBNames.push_back(_DBname);
    }
}

void LeveldbAPI::getAllNames(){
    for (vector<string>::iterator iter=_DBNames.begin();iter!=_DBNames.end();iter++){
            cout<<(*iter)<<endl;
        }
}

void LeveldbAPI::NamestoTxt(){
    ofstream out("out.txt",ios::out);
    int len = LeveldbAPI::_DBNames.size();

    for(int i=0;i<len;++i){     //遍历vector
         out<<LeveldbAPI::_DBNames[i]<<endl;        //存入out.txt
    }
}

void LeveldbAPI::TxttoVec(){
    ifstream file;
    file.open("out.txt"); // 读取
    if(!file) cout<<"error"<<endl;
    string str;

    while (getline(file, str)) {
        LeveldbAPI::_DBNames.push_back(str);
    }
}

bool LeveldbAPI::isExist(){
    vector<string>::iterator iter=std::find(_DBNames.begin(),_DBNames.end(),_DBname);
    if ( iter ==_DBNames.end() ){ //vector数组中没有该元素
        return 0;
    }
    else {
        return 1;
    }
}

string LeveldbAPI::Trim(string& str)
{
    str.find_first_not_of(" \t\r\n");//在字符串str中从索引0开始，返回首次不匹配"\t\r\n"的位置
    str.erase(0,str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}
