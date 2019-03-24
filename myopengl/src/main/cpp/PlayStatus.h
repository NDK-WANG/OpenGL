//
// Created by 王强 on 2019/3/14.
//

#ifndef OPENCAR_PLAYSTATUS_H
#define OPENCAR_PLAYSTATUS_H


class PlayStatus {
public:
    bool exit = false;
    bool load = true;
    bool seek= false;
    bool pause= false;
public:
    PlayStatus();
    ~PlayStatus();
};


#endif //OPENCAR_PLAYSTATUS_H
