/* 型の定義 */

typedef struct LVar LVar;

// ローカル変数の型
struct LVar {
    LVar *next; // 次の変数かNULL
    char *name; // 変数の名前
    int len;    // 名前の長さ
    int offset; // RBPからのオフセット
};

// トークンの種類
typedef enum {
    TK_RESERVED, // 記号
    TK_RETURN,   // return
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
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_LT,  // <
    ND_LE,  // <=
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_ASSIGN, // =
    ND_RETURN, // return
    ND_LVAR, // ローカル変数
    ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind; // ノードの種類
    Node *lhs;     // 左辺
    Node *rhs;     // 右辺
    int val;       // kindがND_NUMの場合のみ使う
    int offset;    // kindがND_LVARの場合のみ使う
};

/* グローバル変数 */
// 現在着目しているトークン
extern Token *token;

// ローカル変数
LVar *locals;

// 入力プログラム
extern char *user_input;

extern Node* code[];

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
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);
Node *new_node(NodeKind kind, Node*lhs, Node*rhs);
Node *new_node_num(int val);
Node *new_node_ident(Token *tok);
Node *primary();
Node *unary();
Node *mul();
Node *add();
Node *relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
Node *program();
void gen(Node *node);
void codegen();
