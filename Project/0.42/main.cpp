#include "hlLexer.h"
#include "hlParser.h"
#include <cassert>
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

using std::map;
using std::string;
using std::cout;
using std::runtime_error;
using std::endl;
pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE, unsigned);
const char* getText(pANTLR3_BASE_TREE );

class Paramters {
public :
    Paramters(Paramters *father) : father(father) {}
    int getVal(string name) {
        if (memory.find(name) == memory.end()) {
            if (father == nullptr) throw runtime_error("unknown paramters!");
            return father->getVal(name); 
        }
        return memory[name];
    }
    int setVal(string name, int val) {
        if (memory.find(name) == memory.end()) { //当前作用域没有
            if (father == nullptr) throw runtime_error("unknown paramters!");
            return father->setVal(name);//在父作用域定义该变量
        }
        return -1;
    }
    int setVal(string name) {
        return (memory[name] = 0);
    }
private:
    Paramters *father;
    map<string, int> memory;
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

bool BinaryMaster::Test(pANTLR3_BASE_TREE tree) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if(tok) {
        switch(tok->type) {
            case PLUS:
            case MINUS:
            case ASSIGN:
            case TIMES:
            case LITTLE:
                return true;
        }
    }
    return false;
}
int BinaryMaster::run(pANTLR3_BASE_TREE tree, Paramters *param) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    switch (tok->type) {
        case PLUS: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            return a + b;
        }
        case MINUS: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            return a - b;
        }
        case TIMES: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            return a * b;
        }
        case ASSIGN: {
            string var(getText(getChild(tree,0)));
            int val = IMaster::MasterRun(getChild(tree, 1), param);
            param->setVal(var, val);
            return val;
        }
        case LITTLE: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a < b) {
                return true;
            } else
                return false;
        }
        default : throw runtime_error("unknown token type!");
    }
    return -1;
}


class ProgramMaster : public IMaster {
public:
    bool Test(pANTLR3_BASE_TREE tree) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        if(tok) {
            switch(tok->type) {
                case BLOCK:
                case DEF:
                case IF: 
                case PRINT:
                    return true;
            }
        }
        return false;
    }
    int run(pANTLR3_BASE_TREE tree, Paramters *param) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        switch (tok->type) { 
            case BLOCK: {
                Paramters new_param(param);
                int k = tree->getChildCount(tree);
                int r = 0;
                for (int i = 0; i < k; i++) {
                    r = IMaster::MasterRun(getChild(tree, i), &new_param);
                }
                return r;
            }
            case IF: {
                int cond_value = IMaster::MasterRun(getChild(tree, 0), param);
                if (cond_value) {
                    IMaster::MasterRun(getChild(tree, 1), param);
                } else if (tree->getChildCount(tree) == 3){
                    IMaster::MasterRun(getChild(tree, 2), param);
                }
                return cond_value;
            }
            case DEF: {
                int k = tree->getChildCount(tree);
                int val = 0, r;
                for (int i = 0; i < k; i++) {
                        string var(getText(getChild(tree, i)));
                    if (tree->getChildCount(getChild(tree, i))) {
                        val = IMaster::MasterRun(getChild(getChild(tree, i), 0), param);
                        param->setVal(var);//声明变量默认初始化0
                        param->setVal(var, val);//设置变量值
                    }
                    r++;
                }
                return r;
            }
            case PRINT: {
                int k = tree->getChildCount(tree);
                int val = 0, r;
                for (int i = 0; i < k; i++) {
                        string var(getText(getChild(tree, i)));
                    if (tree->getChildCount(getChild(tree, i))) {
                        if (i != 0) cout << " " << endl;
                        val = IMaster::MasterRun(getChild(getChild(tree, i), 0), param);
                        cout << val;
                    }
                    r++;
                }
                return r;
            }
            default : throw runtime_error("unknown token type!");
        }
       return 0; 
    }
};

class LoopMaster : public IMaster {
public : 
    bool Test(pANTLR3_BASE_TREE tree) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        if(tok) {
            switch(tok->type) {
                case FOR:
                case WHILE: 
                    return true;
            }
        }
        return false;
    }
    int run(pANTLR3_BASE_TREE tree, Paramters *param) {
            pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
            switch (tok->type) { 
                case FOR: {
                    Paramters new_param(param);
                    int k = tree->getChildCount(tree), ret = 0;
                    IMaster::MasterRun(getChild(tree, 0), &new_param);
                    while (IMaster::MasterRun(getChild(tree, 1), &new_param)) {
                        IMaster::MasterRun(getChild(tree, 3), &new_param);
                        IMaster::MasterRun(getChild(tree, 2), &new_param);
                        ret++;
                    }
                    return ret;
                }
                case WHILE: {
                    
                }
                default : throw runtime_error("unknown token type!");
            }
        return 0; 
    }
};


class ValueMaster : public IMaster {
public :
    bool Test(pANTLR3_BASE_TREE tree) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        if(tok) {
            switch(tok->type) {
                case INT:
                case ID: 
                return true;
            }
        }
        return false;
    }
    int run(pANTLR3_BASE_TREE tree, Paramters *param) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        switch (tok->type) { 
            case INT: {
                const char* s = getText(tree);
                if(s[0] == '~') {
                    return -atoi(s+1);
                }
                else {
                    return atoi(s);
                }
            }
            case ID: {
                string var(getText(tree));
                return param->getVal(var);
            }
            default : throw runtime_error("unknown token type!");
        }
        return 0;
    }
};


IMaster *IMaster::head = nullptr;

IMaster *getMasterLink() {
    if (IMaster::head == nullptr) {
        IMaster::head = new BinaryMaster();
        IMaster::head->next = new ProgramMaster();
        IMaster::head->next->next = new ValueMaster();
        IMaster::head->next->next->next = new LoopMaster();
    }
    return IMaster::head;
}

int IMaster::MasterRun(pANTLR3_BASE_TREE tree, Paramters *param) {
    IMaster *p = getMasterLink();
    while (p && p->Test(tree) == false) {
        p = p->next;
    }
    if (p == nullptr) throw runtime_error("No Master Handle!");
    return p->run(tree, param);//子类处理
}

pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE, unsigned);
const char* getText(pANTLR3_BASE_TREE tree);

int main(int argc, char* argv[]) {
    pANTLR3_INPUT_STREAM input;
    phlLexer lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    phlParser parser;

    assert(argc > 1);
    input = antlr3FileStreamNew((pANTLR3_UINT8)argv[1],ANTLR3_ENC_8BIT);
    lex = hlLexerNew(input);
    
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
    parser = hlParserNew(tokens);
    printf("get parser done!\n");
    fflush(stdout);
    hlParser_prog_return r = parser->prog(parser);
    printf("parser tree is done!\n");
    fflush(stdout);
    pANTLR3_BASE_TREE tree = r.tree;
    
    Paramters param(nullptr);
    int rr = IMaster::MasterRun(tree, &param);
    
    parser->free(parser);
    tokens->free(tokens);
    lex->free(lex);
    input->close(input);
    
    return 0;
}

pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE tree, unsigned i) {
    assert(i < tree->getChildCount(tree));
    return (pANTLR3_BASE_TREE) tree->getChild(tree, i);
}

const char* getText(pANTLR3_BASE_TREE tree) {
    return (const char*) tree->getText(tree)->chars;
}
