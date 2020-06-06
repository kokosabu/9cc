/* コードジェネレータ */
#include <stdbool.h>
#include <stdio.h>
#include "9cc.h"

int labelnum;

void gen_lval(Node *node)
{
    if(node->kind != ND_LVAR) {
        error("代入の左辺値が変数ではありません");
    }

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

void gen_args(Node *node, int argnum)
{
    gen(node->lhs); // exprが処理され結果がスタックに追加されている
    if(node->rhs) {
        // 次の引数がある
        gen_args(node->rhs, argnum+1);
    } else {
        // 次の引数がない
        ;
    }

    if(argnum == 1) {
        printf("  pop rax\n");
        printf("  mov rdi, rax\n");
        //printf("  mov rdi, 5\n");
        //printf("  mov edi, eax\n");
    } else if(argnum == 2) {
        printf("  pop rax\n");
        printf("  mov rsi, rax\n");
    } else if(argnum == 3) {
        printf("  pop rax\n");
        printf("  mov rdx, rax\n");
    } else if(argnum == 4) {
        printf("  pop rax\n");
        printf("  mov rcx, rax\n");
    } else if(argnum == 5) {
        printf("  pop rax\n");
        printf("  mov r8, rax\n");
    } else if(argnum == 6) {
        printf("  pop rax\n");
        printf("  mov r9, rax\n");
    } else {
        /* 7以上はスタックに残しておけば良い */;
    }
}

void gen_args_def(Node *node, int argnum)
{
    //fprintf(stderr, "args_def: offset %d\n", node->offset);
    //fprintf(stderr, "args_def: name %s\n", node->name);
    if(node->lhs) {
        // 次の引数がある
        gen_args_def(node->lhs, argnum+1);
    } else {
        // 次の引数がない
        ;
    }

    // レジスタやスタックからローカル変数用のスタックへコピー
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");

    //fprintf(stderr, "argnum %d: name %s, offset %d\n", argnum, node->name, node->offset);
    if(argnum == 1) {
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
    } else if(argnum == 2) {
        printf("  pop rax\n");
        printf("  mov [rax], rsi\n");
    } else if(argnum == 3) {
        printf("  pop rax\n");
        printf("  mov [rax], rdx\n");
    } else if(argnum == 4) {
        printf("  pop rax\n");
        printf("  mov [rax], rcx\n");
    } else if(argnum == 5) {
        printf("  pop rax\n");
        printf("  mov [rax], r8\n");
    } else if(argnum == 6) {
        printf("  pop rax\n");
        printf("  mov [rax], r9\n");
    } else {
#if 0
        printf("  mov rax, rbp\n");
        printf("  add rax, %d\n", 0x10);
        printf("  mov rdi, [rax]\n");
        printf("  push rdi\n");

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
#else
        printf("  pop rax\n");
        //printf("  pop rdi\n");
        printf("  mov [rax], rdi\n");
#endif
    }
}

void gen(Node *node)
{
    int locallabel;
    Node *cur;
    switch(node->kind) {
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
        case ND_RETURN:
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  mov rsp, rbp\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            return;
        case ND_IF:
            locallabel = labelnum;
            labelnum += 1;

            gen(node->lhs);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je  .Lelse%d\n", locallabel);
            gen(node->rhs->lhs);
            printf("  jmp .Lend%d\n", locallabel);
            printf(".Lelse%d:\n", locallabel);
            if(node->rhs->rhs) {
                gen(node->rhs->rhs);
            }
            printf(".Lend%d:\n", locallabel);
            return;
        case ND_WHILE:
            locallabel = labelnum;
            labelnum += 1;

            printf(".Lbegin%d:\n", locallabel);
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je  .Lend%d\n", locallabel);
            gen(node->rhs);
            printf("  jmp .Lbegin%d\n", locallabel);
            printf(".Lend%d:\n", locallabel);
            return;
        case ND_FOR:
            locallabel = labelnum;
            labelnum += 1;

            /* A */
            if(node->lhs) {
                gen(node->lhs);
            }
            printf(".Lbegin%d:\n", locallabel);
            /* B */
            if(node->rhs->lhs) {
                gen(node->rhs->lhs);
            }
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je  .Lend%d\n", locallabel);
            gen(node->rhs->rhs->rhs); /* D */
            /* C */
            if(node->rhs->rhs->lhs) {
                gen(node->rhs->rhs->lhs);
            }
            printf("  jmp .Lbegin%d\n", locallabel);
            printf(".Lend%d:\n", locallabel);
            return;
        case ND_BLOCK:
            cur = node;
            while(cur->rhs) {
                gen(cur->lhs);
                cur = cur->rhs;
            }
            //printf("  pop rax\n");
            return;
        case ND_FUNC:
            locallabel = labelnum;
            labelnum += 1;


            //printf("  push rsp\n");

            // 8の倍数かチェック。8の倍数ならzero flag 0, 8の倍数でなければ zero flag 1
            //printf("  test rsp, 8\n");
            //printf("  jne .Lend%d\n", locallabel);
            //printf("  sub rsp, 8\n");
            //printf(".Lend%d:\n", locallabel);

            if(node->rhs != NULL) {
                gen_args(node->rhs, 1);
            }
            printf("  call %s\n", node->name);
            printf("  push rax\n");
            //printf("  pop rsp\n");
            return;
        case ND_ARG:
            gen_args_def(node, 1);
            printf("  push rax\n");
            return;
        case ND_ADDR:
            gen_lval(node->lhs);
            return;
        case ND_DEREF:
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        default:
            ;
    }

    /* expr */

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch(node->kind) {
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

void codegen()
{
    Node *code;

    printf(".intel_syntax noprefix\n");

    // 先頭の関数定義から順にコード生成
    for(int i = 0; func_def[i]; i++) {
        // アセンブリの前半部分を出力
        printf(".global %s\n", func_def[i]->name);
        printf("%s:\n", func_def[i]->name);

        int count = 0;
        for(LVar* v = func_locals[i]; v != NULL; v = v->next) {
            count++;
        }

        // プロローグ
        // 変数分の領域を確保する
        printf("  push rbp\n");
        printf("  mov rbp, rsp\n");
        printf("  sub rsp, %d\n", (count-1)*8);

        // 先頭の式から順にコード生成
        code = func_def[i];
        while(code->lhs) {
            gen(code->lhs);
            // 式の評価結果としてスタックに一つの値が残っている
            // はずなので、スタックが溢れないようにポップしておく
            printf("  pop rax\n");

            code = code->rhs;
        }

        // エピローグ
        // 最後の式の結果がRAXに残っているのでそれが返り値になる
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
    }
}
