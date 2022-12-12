#ifndef MATCHING_H
#define MATCHING_H
#include<QString>
#include<brackets.h>
#include<QObject>
#include<QList>
#include<QWidget>
#include<QStack>
class Matching :public QWidget
{
    Q_OBJECT
public:
    Matching();
    int finding(int pos);
    void SetString(QString str);
    QList <Brackets> ListL;
    QList <Brackets> ListM;
    QList <Brackets> ListS;
private:
    Brackets * head;
    QString s;
    int rankLS ;//小括号
    int rankRS;
    int rankLM ;//中括号
    int rankRM;
    int rankLL ;//大括号
    int rankRL;
    QStack <Brackets> matchL;
    QStack <Brackets> matchM;
    QStack <Brackets> matchS;
public slots:
    void matching();
};

#endif // MATCHING_H
