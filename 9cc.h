/* 型の定義 */

typedef struct Function Function;

// 関数の型
struct Function {
    Function *next; // 次の関数かNULL
    char *name; // 関数の名前
    int len;    // 名前の長さ
};

// 型の種類
typedef enum {
    INT, // INT型
    PTR  // ポインタ型
} TypeKind;

typedef struct Type Type;
struct Type {
    TypeKind ty;
    Type *ptr_to;
};

typedef struct LVar LVar;

// ローカル変数の型
struct LVar {
    LVar *next; // 次の変数かNULL
    char *name; // 変数の名前
    int len;    // 名前の長さ
    int offset; // RBPからのオフセット
    Type type;
};

// トークンの種類
typedef enum {
    TK_RESERVED, // 記号
    TK_RETURN,   // return
    TK_IF,       // if
    TK_ELSE,     // else
    TK_WHILE,    // while
    TK_FOR,      // for
    TK_INT,      // int
    TK_IDENT,    // 識別子
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
    TokenKind kind; // トークンの方
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークン文字列
    int len;        // トークンの長さ
};

// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /
    ND_ADDR,   // &
    ND_DEREF,  // *
    ND_LT,     // <
    ND_LE,     // <=
    ND_EQ,     // ==
    ND_NE,     // !=
    ND_ASSIGN, // =
    ND_RETURN, // return
    ND_IF,     // if ( lhs = expr , rhs = then )
    ND_THEN,   // then ( lhs = true's stmt, rhs = false's stmt )
    ND_WHILE,  // while ( lhs = expr, rhs = stmt )
    ND_FOR,    // for { expr(A), { expr(B), { expr(C), stmt(D) } } }
    ND_BLOCK,  // { stmt, { stmt, { stmt, ... } } }
    ND_FUNC,   // 関数コール
    ND_LVAR,   // ローカル変数
    ND_ARG,    // 引数
    ND_NUM,    // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind; // ノードの種類
    Node *lhs;     // 左辺
    Node *rhs;     // 右辺
    int val;       // kindがND_NUMの場合のみ使う
    int offset;    // kindがND_LVARの場合のみ使う
    Type type;     // kindがND_LVARの場合のみ使う
    char *name;    // kindが
};

/* グローバル変数 */
// 現在着目しているトークン
extern Token *token;

// ローカル変数
LVar *locals;

//
Function *functions;

// 入力プログラム
extern char *user_input;

extern Node* code[];
extern Node* func_def[];

extern int labelnum;
extern LVar *func_locals[100];

/* プロトタイプ宣言 */
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
bool consume_kind(TokenKind kind);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
LVar *find_lvar(Token *tok);
Function *find_function(Token *tok);
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);
Node *new_node(NodeKind kind, Node*lhs, Node*rhs);
Node *new_node_num(int val);
Node *new_node_arg(Token *tok, int p_num);
Node *new_node_ident(Token *tok);
Node *new_node_ident_decl(Token *tok, int p_num);
Node *new_node_function(Token *tok);
Node *primary();
Node *unary();
Node *mul();
Node *add();
Node *relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
Node *function();
Node *program();
void gen_args(Node *node, int argnum);
void gen_args_def(Node *node, int argnum);
void gen(Node *node);
void codegen();
