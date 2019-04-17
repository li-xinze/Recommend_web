// 包含必要的头文件
#include <leveldb/db.h>
#include "leveldbapi.h"

using namespace std;

vector<string> LeveldbAPI::_DBNames(0);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LeveldbAPI User("User");
    return a.exec();
}
