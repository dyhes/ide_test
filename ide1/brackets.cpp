#include "brackets.h"

Brackets::Brackets(){
    match = 0;    
    row = 0;
}

Brackets ::Brackets(int m, int dir, int r, int p){
    mode = m;
    direction = dir;
    rank = r;
    position = p;
}

void Brackets::Set(int m, int dir, int r, int p){
    mode = m;
    direction = dir;
    rank = r;
    position = p;
}
