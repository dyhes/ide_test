#ifndef BRACKETS_H
#define BRACKETS_H
#include<QObject>
#include<QWidget>
class Brackets
{
public:
    Brackets();
    Brackets(int m,int dir,int r,int p);
    void Set(int m,int dir,int r,int p);
    int mode;//1=大括号，2=中括号，3=小括号
    int direction;//1=左括号，2=右括号
    int rank;//是第几个括号
    int position;//在全部文本中的位置
    int match;//配对括号
    int row;//行号
};

#endif // BRACKETS_H
