#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op)
{
    if(token->kind != TK_RESERVED ||
       strlen(op) != token->len   ||
       memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンが期待しているトークンのときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_kind(TokenKind kind)
{
    if(token->kind != kind) {
        //fprintf(stderr, "[consume_kind] false %d\n", kind);
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンが期待している識別子のときには、トークンを1つ読み進めて
// トークンを返す。それ以外の場合にはNULLを返す。
Token *consume_ident()
{
    Token *tok = token;
    if(token->kind != TK_IDENT) {
        return false;
    }
    token = token->next;
    return tok;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op)
{
    if(token->kind != TK_RESERVED ||
       strlen(op) != token->len   ||
       memcmp(token->str, op, token->len)) {
        error_at(token->str, "'%s'ではありません", op);
    }
    token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number()
{
    if(token->kind != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok)
{
    for(LVar *var = locals; var; var = var->next) {
        if(var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
            return var;
        }
    }
    return NULL;
}

// 関数を名前で検索する。見つからなかった場合はNULLを返す。
Function *find_function(Token *tok)
{
    for(Function *func = functions; func; func = func->next) {
        if(func->len == tok->len && !memcmp(tok->str, func->name, func->len)) {
            return func;
        }
    }
    return NULL;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p) {
        // 空白文字をスキップ
        if(isspace(*p)) {
            p++;
            continue;
        }

        if(strncmp("==", p, 2) == 0 ||
           strncmp("!=", p, 2) == 0 ||
           strncmp("<=", p, 2) == 0 ||
           strncmp(">=", p, 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }
        if(strncmp("<", p, 1) == 0 ||
           strncmp(">", p, 1) == 0) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if(strchr("+-*/();={},&", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if(strncmp(p, "return", 6) == 0 && !(isalnum(p[6]) || p[6] == '_')) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }
        if(strncmp(p, "if", 2) == 0 && !(isalnum(p[2]) || p[2] == '_')) {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        }
        if(strncmp(p, "else", 4) == 0 && !(isalnum(p[4]) || p[4] == '_')) {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
            continue;
        }
        if(strncmp(p, "while", 5) == 0 && !(isalnum(p[5]) || p[5] == '_')) {
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
            continue;
        }
        if(strncmp(p, "for", 3) == 0 && !(isalnum(p[3]) || p[3] == '_')) {
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
            continue;
        }
        if(strncmp(p, "int", 3) == 0 && !(isalnum(p[3]) || p[3] == '_')) {
            cur = new_token(TK_INT, cur, p, 3);
            p += 3;
            continue;
        }

        //if(strchr("abcdefghijklmnopqrstuvwxyz", *p)) {
        //if('a' <= *p && *p <= 'z') {
        if(isalpha(*p)) {
            cur = new_token(TK_IDENT, cur, p, 0);
            char *q = p;
            p++;
            do {
                if(isalnum(*p) || *p == '_') {
                    p++;
                } else {
                    break;
                }
            } while(1);
            cur->len = p - q;
            continue;
        }

        if(isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

Node *new_node(NodeKind kind, Node*lhs, Node*rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_arg(Token *tok, int p_num)
{
    Type *t;
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_ARG;

    LVar *lvar = find_lvar(tok);
    if(lvar) {
        error("引数名が重複しています。");
    } else {
        lvar = calloc(1, sizeof(LVar));
        lvar->next = locals;
        lvar->name = tok->str;
        lvar->len = tok->len;
        lvar->offset = locals->offset + 8;
        t = &(lvar->type);
        for(int i = p_num; i > 0; i--) {
            t->ty = PTR;
            t->ptr_to = calloc(1, sizeof(Type));
            t = t->ptr_to;
        }
        t->ty = INT;
        //fprintf(stderr, "lvar : name %s, len %d, offset %d\n", lvar->name, lvar->len, lvar->offset);
        node->offset = lvar->offset;
        locals = lvar;
    }
    return node;
}

Node *new_node_ident(Token *tok)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;

    LVar *lvar = find_lvar(tok);
    if(lvar) {
        node->offset = lvar->offset;
        node->type   = lvar->type;
    } else {
        error("未定義の変数です。");
    }
    return node;
}

Node *new_node_ident_decl(Token *tok, int p_num)
{
    Type *t;
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;

    LVar *lvar = find_lvar(tok);
    if(lvar) {
        node->offset = lvar->offset;
        node->type   = lvar->type;
    } else {
        lvar = calloc(1, sizeof(LVar));
        lvar->next = locals;
        lvar->name = tok->str;
        lvar->len = tok->len;
        lvar->offset = locals->offset + 8;
        t = &(lvar->type);
        for(int i = p_num; i > 0; i--) {
            t->ty = PTR;
            t->ptr_to = calloc(1, sizeof(Type));
            t = t->ptr_to;
        }
        t->ty = INT;
        //lvar->offset = (lvar->name[0] - 'a' + 1) * 8;
        node->offset = lvar->offset;
        node->type   = lvar->type;
        locals = lvar;
    }
    return node;
}

/* CALLER, CALLEE共通なので後で直さないといけなくなりそう */
Node *new_node_function(Token *tok)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNC;
    node->lhs = NULL;
    node->rhs = NULL;

    Function *function = find_function(tok);
    if(function) {
        node->name = strndup(tok->str, tok->len);
    } else {
        function = calloc(1, sizeof(Function));
        function->next = functions;
        function->name = tok->str;
        function->len = tok->len;
        functions = function;

        node->name = strndup(tok->str, tok->len);
    }
    return node;
}


// primary =  num
//          | ident ("(" (expr ",")* ")")?
//          | "(" expr ")"
Node *primary()
{
    // 次のトークンが"("なら、"(" expr ")"のはず
    if(consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if(tok) {
        if(consume("(")) {
            Node *node = new_node_function(tok);
            if(consume(")")) {
                return node;
            } else {
                node->rhs = calloc(1, sizeof(Node));
                Node *n = node->rhs;
                while(1) {
                    n->lhs = expr();
                    if(consume(",")) {
                        n->rhs = calloc(1, sizeof(Node));
                        n->rhs->lhs = NULL;
                        n->rhs->rhs = NULL;
                        n = n->rhs;
                    } else {
                        n->rhs = NULL;
                        break;
                    }
                }
                expect(")");
                return node;
            }
        } else {
            return new_node_ident(tok);
        }
    }

    // そうでなければ数値のはず
    return new_node_num(expect_number());
}

// unary =  "+"? primary
//        | "-"? primary
//        | "*" unary
//        | "&" unary
Node *unary()
{
    if(consume("*")) {
        return new_node(ND_DEREF, unary(), NULL);
    } else if(consume("&")) {
        return new_node(ND_ADDR, unary(), NULL);
    } else if(consume("+")) {
        return unary();
    } else if(consume("-")) {
        return new_node(ND_SUB, new_node_num(0), unary());
    }
    return primary();
}

// mul = unary ("*" unary | "/" unary)*
Node *mul()
{
    Node *node = unary();

    for(;;) {
        if(consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if(consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

// add = mul ("+" mul | "-" mul)*
Node *add()
{
    Node *node = mul();

    for(;;) {
        if(consume("+")) {
            Node *adder;
            if(node->kind == ND_LVAR && (node->type).ty == PTR) {
                if((node->type).ptr_to->ty == INT) {
                    adder = new_node(ND_MUL, mul(), new_node_num(4));
                } else {
                    adder = new_node(ND_MUL, mul(), new_node_num(8));
                }
            } else {
                adder = mul();
            }

            node = new_node(ND_ADD, node, adder);
        } else if(consume("-")) {
            Node *suber;
            if(node->kind == ND_LVAR && (node->type).ty == PTR) {
                if((node->type).ptr_to->ty == INT) {
                    suber = new_node(ND_MUL, mul(), new_node_num(4));
                } else {
                    suber = new_node(ND_MUL, mul(), new_node_num(8));
                }
            } else {
                suber = mul();
            }

            node = new_node(ND_SUB, node, suber);
        } else {
            return node;
        }
    }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node *relational()
{
    Node *node = add();

    for(;;) {
        if(consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if(consume("<=")) {
            node = new_node(ND_LE, node, add());
        } else if(consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else if(consume(">=")) {
            node = new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}

// equality = relational ("==" relational | "!=" relational)*
Node *equality()
{
    Node *node = relational();

    for(;;) {
        if(consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if(consume("!=")) {
            node = new_node(ND_NE, node, relational());
        } else {
            return node;
        }
    }
}

// assign = equality ("=" assign)?
Node *assign()
{
    Node *node = equality();

    if(consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

// expr = assign
Node *expr()
{
    Node *node = assign();

    return node;
}

// stmt =  expr ";"
//       | "{" stmt* "}"
//       | "return" expr ";"
//       | "if" "(" expr ")" stmt ("else" stmt)?
//       | "while" "(" expr ")" stmt
//       | "for" "(" expr? ";" expr? ";" expr? ")" stmt
//       | "int" "*"* ident ";" // 後でstmtと別にdeclをつくる?
Node *stmt()
{
    Node *node;

    if(consume_kind(TK_RETURN)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();

        expect(";");
    } else if(consume_kind(TK_IF)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        expect("(");
        node->lhs = expr();
        expect(")");
        node->rhs = calloc(1, sizeof(Node));
        node->rhs->lhs = stmt();
        if(consume_kind(TK_ELSE)) {
            node->rhs->rhs = stmt();
        } else {
            node->rhs->rhs = NULL;
        }
    } else if(consume_kind(TK_WHILE)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        expect("(");
        node->lhs = expr();
        expect(")");
        node->rhs = stmt();
    } else if(consume_kind(TK_FOR)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        expect("(");
        bool clause1 = consume(";");
        if(clause1) {
            node->lhs = NULL;
        } else {
            node->lhs = expr();
            expect(";");
        }
        bool expression2 = consume(";");
        node->rhs = calloc(1, sizeof(Node));
        if(expression2) {
            node->rhs->lhs = NULL;
        } else {
            node->rhs->lhs = expr();
            expect(";");
        }
        bool expression3 = consume(")");
        node->rhs->rhs = calloc(1, sizeof(Node));
        if(expression3) {
            node->rhs->rhs->lhs = NULL;
        } else {
            node->rhs->rhs->lhs = expr();
            expect(")");
        }
        node->rhs->rhs->rhs = stmt();
    } else if(consume_kind(TK_INT)) {
        int p_num = 0;
        while(consume("*")) {
            p_num += 1;
        }
        // ident
        Token *tok = consume_ident();
        node = new_node_ident_decl(tok, p_num);
        expect(";");
    } else if(consume("{")) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        node->lhs = NULL;
        node->rhs = NULL;
        Node *cur = node;
        while(!consume("}")) {
            cur->lhs = stmt();
            cur->rhs = calloc(1, sizeof(Node));
            cur = cur->rhs;
            cur->lhs = NULL;
            cur->rhs = NULL;
        }
    } else {
        node = expr();

        expect(";");
    }
    return node;
}

// function = "int" ident "(" ("int" ident ",")* ")" "{" stmt* "}"
Node *function()
{
    int i = 0;

    if(!consume_kind(TK_INT)) {
        error("intが足りません。");
    }

    Token *tok = consume_ident();
    Node *root = new_node_function(tok);
    Node *node = root;
    node->lhs = NULL;
    node->rhs = NULL;

    expect("(");
    if(consume_kind(TK_INT)) {
        int p_num = 0;
        while(consume("*")) {
            p_num += 1;
        }
        tok = consume_ident();
        //fprintf(stderr, "[%s]\n", tok->str);
        node->lhs = new_node_arg(tok, p_num);
        node->rhs = calloc(1, sizeof(Node));
        node->rhs->lhs = NULL;
        node->rhs->rhs = NULL;

        Node *n = node->lhs;
        while(1) {
            if(consume(",")) {
                consume_kind(TK_INT);
                int p_num = 0;
                while(consume("*")) {
                    p_num += 1;
                }
                tok = consume_ident();
                //fprintf(stderr, "[%s]\n", tok->str);
                if(tok) {
                    n->lhs = new_node_arg(tok, p_num);
                    n->rhs = NULL;
                    n = n->lhs;
                } else {
                    n->lhs = NULL;
                    error("引数が必要です。");
                }
            } else {
                break;
            }
        }

        node = root->rhs;
    } else {
        ;
    }
    expect(")");
    expect("{");
    while(!consume("}")) {
        //fprintf(stderr, "[code] %s\n", token->str);
        node->lhs = stmt();
        node->rhs = calloc(1, sizeof(Node));
        node->rhs->lhs = NULL;
        node->rhs->rhs = NULL;
        node = node->rhs;
    }
    //fprintf(stderr, "[code] Done\n");

    return root;
}

Node *func_def[100];
LVar *func_locals[100];

// program = function*
Node *program()
{
    int i = 0;
    while(!at_eof()) {
        // ローカル変数の起点作成
        locals = calloc(1, sizeof(LVar));
        locals->next = NULL;
        locals->name = "*";
        locals->len = 1;
        locals->offset = 0;

        func_def[i] = function();
        func_locals[i] = locals;
        i++;
    }
    func_def[i] = NULL;
}
