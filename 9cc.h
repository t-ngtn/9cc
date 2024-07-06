#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tokenize.c

typedef enum {
  TK_RESERVED,  // 記号
  TK_IDENT,     // 識別子
  TK_NUM,       // 整数トークン
  TK_RETURN,    // return
  TK_IF,        // if
  TK_ELSE,      // else
  TK_FOR,       // for
  TK_WHILE,     // while
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident();
bool consume_by_kind(TokenKind tk);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswitch(char *p, char *q);
Token *tokenize(char *p);

extern Token *token;
extern char *user_input;

// parse.c
typedef enum {
  ND_ADD,      // +
  ND_SUB,      // -
  ND_MUL,      // *
  ND_DIV,      // /
  ND_EQ,       // ==
  ND_NE,       // !=
  ND_LT,       // <
  ND_LE,       // <=
  ND_ASSIGN,   // =
  ND_LVAR,     // local variable
  ND_NUM,      // Integer
  ND_RETURN,   // return
  ND_IF,       // if
  ND_FOR,      // for or while
  ND_BLOCK,    // { ... }
  ND_FUNCALL,  // function call
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  Node *init;  // for
  Node *cond;  // if or for
  Node *then;  // if or for
  Node *els;   // else
  Node *inc;   // for

  Node *body;  // block
  Node *next;  // block

  char *funcname;  // funcall
  Node *args;      // funcall

  int val;     // num
  int offset;  // lvar
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

typedef struct LVar LVar;
extern LVar *locals;

struct LVar {
  LVar *next;  // 次の変数かNULL
  char *name;  // 変数の名前
  int len;     // 名前の長さ
  int offset;  // RBPからのオフセット
};
LVar *find_lvar(Token *tok);
char *strndup(const char *s, size_t n);

// codegen.c
void gen(Node *node);
extern Node *code[100];
extern int jump_index;