#include "../include/paramater.h"

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

