grammar hl;

options {
    language = C;
    output = AST;
    ASTLabelType=pANTLR3_BASE_TREE;
}

@header {
    #include <assert.h>
}

// The suffix '^' means make it a root.
// The suffix '!' means ignore it.

defid
    : DEF! ID (','! ID)* ';'!
    ;


ifexpr
    : IF^ '('! expr ')'! block
    ;

expr: multExpr ((PLUS^ | MINUS^) multExpr)*
    ;

multExpr
    : atom (TIMES^ atom)*
    ;

atom: INT
    | ID
    | '('! expr ')'!
    ;

code_block:
    '{'! (stmt | block)+ '}'!
    ;

stmt: expr ';' NEWLINE -> expr // tree rewrite syntax
    | defid NEWLINE -> ^(DEF defid)
    | ID ASSIGN expr ';' NEWLINE -> ^(ASSIGN ID expr) // tree notation
    | ifexpr
    | NEWLINE ->   // ignore
    ;

block:
    code_block -> ^(BLOCK code_block)
    ;

prog
    : (block {
            pANTLR3_STRING s = $block.tree->toStringTree($block.tree);
            assert(s->chars);
            printf("tree \%s\n", s->chars);
    })+
    ;

PLUS: '+';
MINUS: '-';
TIMES: '*';
DOT : ',';
ASSIGN: '=';
IF: 'if';
BLOCK: '{}';
DEF: 'd' 'e' 'f';
ID: ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'0'..'9'|'A'..'Z'|'_')*;
INT: '~'? '0'..'9'+ ;
NEWLINE: '\r'? '\n' ;
WS : (' '|'\t')+ {$channel = HIDDEN;};
