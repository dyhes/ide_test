#include "matching.h"
#include<QDebug>
#include<QStack>
#include<QList>
Matching::Matching()
{
    head = new Brackets();
    rankLS = 1;//小括号
    rankLM = 1;//中括号
    rankLL = 1;//大括号
    rankRS = 1;
    rankRM = 1;
    rankRL = 1;
}

void Matching::SetString(QString str){
    s = str;
}

//Brackets(int m, int dir, int r, int p)
/*int mode;//1=大括号，2=中括号，3=小括号
    int direction;//1=左括号，2=右括号
    int rank;//是第几个括号
    int position;//在全部文本中的位置
*/
void Matching::matching(){
//    qDebug()<<"Matching";
    QString::const_iterator cit = NULL;
    QString strTemp;
    int number = 1;//行数
    while(!matchL.empty()){
        matchL.pop();
    }
    while(!matchS.empty()){
        matchS.pop();
    }
    while(!matchM.empty()){
        matchM.pop();
    }
    while(ListL.size()){
        ListL.clear();
    }
    while(ListM.size()){
        ListM.clear();
    }
    while(ListS.size()){
        ListS.clear();
    }
    int num=0;
//    qDebug()<<"Match begin";
    int ranks = 0,rankm=0,rankl=0;
    for(cit = s.cbegin();cit<s.cend();cit++){
//        qDebug()<<"insert";
        num++;
        strTemp = (*cit);//逐个读取字符,构建括号的链表
//        qDebug()<<strTemp; //输出一下看看读取的字符对不对
        if(strTemp == "{"){
            Brackets newBracket;
            newBracket.Set(1,1,rankLL++,num);
            newBracket.row = number;
            matchL.push(newBracket);
        }
        else if(strTemp == "\n"){
            number++;
        }
        else if(strTemp == "}"){
            Brackets newBracket;
            newBracket.Set(1,2,rankRL++,num);
            newBracket.row = number;
            if(!matchL.empty()){
                newBracket.match = rankl;
                ListL.append(newBracket);
                matchL.top().match = rankl++;
                ListL.append(matchL.top());
                matchL.pop();
            }
        }
        else if(strTemp == "["){
            Brackets newBracket;
            newBracket.Set(2,1,rankLM++,num);
            matchM.push(newBracket);
        }
        else if(strTemp == "]"){
            Brackets newBracket;
            newBracket.Set(2,2,rankRM++,num);
            if(!matchM.empty()){
                newBracket.match = rankm;
                ListM.append(newBracket);
                matchM.top().match = rankm++;
                ListM.append(matchM.top());
                matchM.pop();
            }
        }
        else if(strTemp == "("){
            Brackets newBracket;
            newBracket.Set(3,1,rankLS++,num);
            matchS.push(newBracket);
        }
        else if(strTemp == ")"){
            Brackets newBracket;
            newBracket.Set(3,2,rankRM++,num);
            if(!matchS.empty()){
                newBracket.match = ranks;
                ListS.append(newBracket);
                matchS.top().match = ranks++;
                ListS.append(matchS.top());
                matchS.pop();
            }
        }
        else{
            continue;
        }
    }
}



int Matching::finding(int pos){//pos是鼠标光标点选位置
//    qDebug()<<"Finding";
    int direction = 0;
    int result = -1;
    int match = 0;
    for(int i=0;i<ListL.size();i++){
        if(ListL[i].position == pos){
            match = ListL[i].match;
            direction = ListL[i].direction;
            for(int j=0;j<ListL.size();j++){
                if(ListL[j].match == match&&ListL[j].direction+direction == 3){
                    result = ListL[j].position;
                    return result;
                }
            }
            break;
        }
    }
    for(int i=0;i<ListM.size();i++){
        if(ListM[i].position == pos){
            match = ListM[i].match;
            direction = ListM[i].direction;
            for(int j=0;j<ListM.size();j++){
                if(ListM[j].match == match&&ListM[j].direction+direction == 3){
                    result = ListM[j].position;
                    return result;
                }
            }
            break;
        }
    }

    for(int i=0;i<ListS.size();i++){
        if(ListS[i].position == pos){
            match = ListS[i].match;
            direction = ListS[i].direction;
            for(int j=0;j<ListS.size();j++){
                if(ListS[j].match == match&&ListS[j].direction+direction == 3){
                    result = ListS[j].position;
                    return result;
                }
            }
            break;
        }
    }
    return result;//返回匹配到的位置
}

