/*************************************************************************
	> File Name: paramater.h
	> Author: LHC 
	> Mail: 3115629644@qq.com 
	> Created Time: 2019年03月31日 星期日 17时02分23秒
 ************************************************************************/

#ifndef _PARAMATER_H
#define _PARAMATER_H

#include "hlLexer.h"
#include "hlParser.h"
#include <cassert>
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE, unsigned);

const char* getText(pANTLR3_BASE_TREE tree);

class Paramters {
public :
    Paramters(Paramters *father) : father(father) {}
    int getVal(string );
    int setVal(string, int);
    int setVal(string );
private:
    map<string, int> memory;
    Paramters *father;
};

class IMaster {
public :
    virtual bool Test(pANTLR3_BASE_TREE ) = 0;
    virtual int run(pANTLR3_BASE_TREE, Paramters *) = 0;
    static int MasterRun(pANTLR3_BASE_TREE, Paramters *);
    friend IMaster *getMasterLink();
    static IMaster *head;
    IMaster *next;
private:
};

class BinaryMaster : public IMaster {
public :
    bool Test(pANTLR3_BASE_TREE );
    int run(pANTLR3_BASE_TREE, Paramters *);
};

class ProgramMaster : public IMaster {
public:
    bool Test(pANTLR3_BASE_TREE );
    int run(pANTLR3_BASE_TREE, Paramters *);

class LoopMaster : public IMaster {
public : 
    bool Test(pANTLR3_BASE_TREE );
    int run(pANTLR3_BASE_TREE, Paramters *);
};


class ValueMaster : public IMaster {
public :
    bool Test(pANTLR3_BASE_TREE );
    int run(pANTLR3_BASE_TREE );
};

#endif
