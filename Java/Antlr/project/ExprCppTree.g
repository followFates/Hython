grammar ExprCppTree;
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
expr: plus_minusExpr ((COMMA^) plus_minusExpr)*
    ;
PLUS: '+';
MINUS: '-';
plus_minusExpr
    : multExpr ((PLUS^ | MINUS^) multExpr)*
    ;
multExpr
    : atom (TIMES^ atom)* 
    ;
TIMES: '*';
COMMA: ',';
atom: INT
| ID //(ASSIGN^ expr)?
| '('! expr ')'!
| '('! ID ASSIGN^ INT ')'!
;

stmt: expr NEWLINE -> expr  // tree rewrite syntax
    | ID ASSIGN expr NEWLINE -> ^(ASSIGN ID expr) // tree notation
    | NEWLINE ->   // ignore
    ;
ASSIGN: '=';
prog
    : (
	stmt {pANTLR3_STRING s = $stmt.tree->toStringTree($stmt.tree);
             assert(s->chars);
             printf("Haizei tree \%s\n", s->chars);
            }
        )+
    ;
ID: ('a'..'z'|'A'..'Z')+ ;
INT: '~'? '0'..'9'+ ;
NEWLINE: '\r'? '\n' ;
WS : (' '|'\t')+ {$channel = HIDDEN;};

