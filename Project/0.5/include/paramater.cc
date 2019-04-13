#include "./paramater.h"

int Paramters::getVal(string var) {
    if (memory.find(var) == memory.end()) {
        if (father == nullptr) throw runtime_error("unknown paramters!");
        return father->getVal(var);
    }
    return memory[var];
}

int Paramters::setVal(string var, int val) { //仅实现不可以初始化
    if (memory.find(var) == memory.end()) {
        if (father == nullptr) throw runtime_error("unknown paramters!");
        return father->setVal(var, val);
    }
    return (memory[var] = val);
}

int Paramters::setVal(string var) {//默认初始化
    return (memory[var] = 0);
}

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

bool BinaryMaster::Test(pANTLR3_BASE_TREE tree) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if(tok) {
        switch(tok->type) {
            case MOD:
            case DIV:
            case OR:
            case AND:
            case GE:
            case NE:
            case LITTLE:
            case LE:
            case GT:
            case EQ:
            case PLUS:
            case MINUS:
            case TIMES:
            case ASSIGN:
                return true;
        }
    }
    return false;
}

int BinaryMaster::run(pANTLR3_BASE_TREE tree, Paramters *param) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    switch (tok->type) {
        case MOD: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
           return a % b;
        }
        case OR: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            return a || b;
        }
        case AND: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            return a && b;
            
        }
        case GE: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a >= b) return true;
            else false;
            
        }
        case NE: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a != b)  return true;
            else return false;
        }
        case LITTLE: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a < b) return true;
            else return false;
        }
        case LE: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a <= b) return true;
            else return false;
        }
        case GT: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a > b) return true;
            else return false;
        }
        case EQ: {
            int a = IMaster::MasterRun(getChild(tree, 0), param);
            int b = IMaster::MasterRun(getChild(tree, 1), param);
            if (a == b) return true;
            else return false; 
        }
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
        case ASSIGN: {//赋值操作：先判断变量var是否合法
            string var(getText(getChild(tree,0)));
            int val = IMaster::MasterRun(getChild(tree, 1), param);
            param->getVal(var);
            param->setVal(var, val);
            return val;
        }
        default : throw runtime_error("unknown token type!");
    }
    return -1;
}

bool ProgramMaster::Test(pANTLR3_BASE_TREE tree) {
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

int ProgramMaster::run(pANTLR3_BASE_TREE tree, Paramters *param) {
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
            for (int i = 0; i < k; i++) {
                string var(getText(getChild(tree, i)));
                int val = IMaster::MasterRun(getChild(getChild(tree, i), 0), param);
                param->setVal(var);
                param->setVal(var, val);
            }
            return 0;
        }
        case PRINT: {//输出PRINT下面的孩子节点
            int k = tree->getChildCount(tree);
            for (int i = 0; i < k; i++) {
                string var(getText(getChild(tree, i)));
                int val = IMaster::MasterRun(getChild(tree, i), param);
                if (i != 0) cout << " ";
                cout << val;
            }
            cout << endl;
            return 0; 
        }
        default : throw runtime_error("unknown token type!");
    }
   return 0;
}

bool LoopMaster::Test(pANTLR3_BASE_TREE tree) {
    pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
    if(tok) {
        switch(tok->type) {
            case FOR:
            case WHILE:
            case DOWHILE:
                return true;
        }
    }
    return false;
}

int LoopMaster::run(pANTLR3_BASE_TREE tree, Paramters *param) {
        pANTLR3_COMMON_TOKEN tok = tree->getToken(tree);
        switch (tok->type) {
            case FOR: {
                Paramters new_param(param);
                int k = tree->getChildCount(tree);
                int r = 0;
                IMaster::MasterRun(getChild(tree, 0), param);
                while (IMaster::MasterRun(getChild(tree, 1), param)) {
                    IMaster::MasterRun(getChild(tree, 3), param);
                    IMaster::MasterRun(getChild(tree, 2), param);
                    r++;
                }
                return r;
            }
            case WHILE: {
                Paramters new_param(param);
                int r = 0;
                while (IMaster::MasterRun(getChild(tree, 0), param)) {
                    r = IMaster::MasterRun(getChild(tree, 1), param);
                }
                return r;
            }
            case DOWHILE: {
                Paramters new_param(param);
                int r = 0;
                while (IMaster::MasterRun(getChild(tree, 0), param)) {
                    r = IMaster::MasterRun(getChild(tree, 1), param);
                }
                return r;
            }
            default : throw runtime_error("unknown token type!");
        }
    return 0;
}

bool ValueMaster::Test(pANTLR3_BASE_TREE tree) {
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

int ValueMaster::run(pANTLR3_BASE_TREE tree, Paramters *param) {
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


pANTLR3_BASE_TREE getChild(pANTLR3_BASE_TREE tree, unsigned i) {
    assert(i < tree->getChildCount(tree));
    return (pANTLR3_BASE_TREE) tree->getChild(tree, i);
}

const char* getText(pANTLR3_BASE_TREE tree) {
    return (const char*) tree->getText(tree)->chars;
}

