#include "9cc.h"

static char *argreg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR) error("代入の左辺値が変数ではありません");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node) {
  switch (node->kind) {
    case ND_RETURN:
      gen(node->lhs);
      printf("  pop rax\n");
      printf("  mov rsp, rbp\n");
      printf("  pop rbp\n");
      printf("  ret\n ");
      return;
    case ND_IF:
      gen(node->cond);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      int tmp_jump_index = jump_index++;
      printf("  je .Lelse%d\n", tmp_jump_index);
      gen(node->then);
      printf("  jmp .Lend%d\n", tmp_jump_index);
      printf(".Lelse%d:\n", tmp_jump_index);
      if (node->els) gen(node->els);
      printf(".Lend%d:\n", tmp_jump_index);
      return;
    case ND_FOR:
      if (node->init) gen(node->init);
      int tmp_jump_index2 = jump_index++;
      printf(".Lbegin%d:\n", tmp_jump_index2);
      if (node->cond) {
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", tmp_jump_index2);
      }
      gen(node->then);
      if (node->inc) gen(node->inc);
      printf("  jmp .Lbegin%d\n", tmp_jump_index2);
      printf(".Lend%d:\n", tmp_jump_index2);
      return;
    case ND_BLOCK:
      for (Node *n = node->body; n; n = n->next) gen(n);
      return;
    case ND_NUM:
      printf("  push %d\n", node->val);
      return;
    case ND_LVAR:
      gen_lval(node);
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;
    case ND_ASSIGN:
      gen_lval(node->lhs);
      gen(node->rhs);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  mov [rax], rdi\n");
      printf("  push rdi\n");
      return;
    case ND_FUNCALL:
      int arg_cnt = 0;
      for (Node *arg = node->args; arg; arg = arg->next) {
        gen(arg);
        arg_cnt++;
      }
      for (int i = arg_cnt - 1; i >= 0; i--) printf("  pop %s\n", argreg[i]);

      printf("  mov rax, 0\n");
      printf("  call %s\n", node->funcname);
      printf("  push rax\n");
      return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NE:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  printf("  push rax\n");
}