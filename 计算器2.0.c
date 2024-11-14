#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define MAXSIZE 100

typedef struct {
    double data[MAXSIZE];
    int top ;
} Stack;

// 初始化栈
void initStack(Stack *stack) {
    stack -> top = -1;
}

// 判断栈是否为空
int isEmpty(Stack *stack) {
    return stack -> top == -1;
}

// 判断栈是否已满
int isFull(Stack *stack) {
    return stack -> top == MAXSIZE - 1;   
}
 
// 压栈操作 
void push (Stack *stack, double value) {
    if (isFull(stack)){
        printf("Error\n");
        exit(-1);
    }
    stack -> data[++stack -> top] = value;
}

// 弹栈操作
double pop (Stack *stack) {
    if (isEmpty(stack)){
        printf("Error\n");
        exit(-1);
    }
    return stack -> data[stack -> top--];
}

// 查看栈顶元素 
double peek (Stack*stack) {
    if (isEmpty(stack)){
        printf("Error\n");
        exit(-1);
    }
    return stack -> data[stack -> top];    
}


// 检查运算符的优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1; 
    if (op == '*' || op == '/') return 2; 
    return 0; 
}

// 将中缀表达式转换为后缀表达式（逆波兰表达式）
void infixToPostfix(char* infix, char* postfix) {
    Stack stack;
    initStack(&stack);
    int j = 0; //用于索引后缀表达式

    for(int i = 0; infix[i] != '\0'; i++) {
        // 数字和小数点直接加入后缀表达式
        if (infix[i]=='.' || isdigit(infix[i]) ) {
            while (isdigit(infix[i]) || infix[i]=='.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; 
            i--; // 回退一步
        }
        // 左括号直接压栈
        else if (infix[i] == '(') {
            push(&stack, infix[i]);
        } 
        // 处理右括号
        else if (infix[i] == ')') {
            while (!isEmpty(&stack) && peek(&stack)!= '(') {
                postfix[j++] = (char)pop(&stack);
            }
            pop (&stack); // 弹出左括号
        } else if (strchr("+-*/", infix[i])) {
            // 运算符压栈
            while (!isEmpty(&stack) && precedence((char)peek(&stack)) >= precedence(infix[i])) {
                postfix[j++] = (char)pop(&stack);
            } // 栈顶运算符优先级大于当前运算符，则弹出栈顶运算符
            push(&stack, infix[i]); // 压栈当前运算符
        }


    }
    // 弹出剩余的运算符
    while (!isEmpty(&stack)) {
        postfix[j++] = (char)pop(&stack);
    }
    postfix[j] = '\0'; // 加入结束符    
}

// 计算后缀表达式的值
double evaluatePostfix(char* postfix) {
    Stack stack;
    initStack(&stack);
    
    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isdigit(postfix[i]) || postfix[i]=='.') { // 将数字压栈
            char A[MAXSIZE]; // 用于存放多位数
            int j = 0;
            while (isdigit(postfix[i]) || postfix[i]=='.') {
                A[j++] = postfix[i++];
            }
            A[j] = '\0';
            push(&stack, atof(A));
            i--; // 回退一步
        } else if (strchr("+-*/", postfix[i])) { // 运算符
        double b = pop(&stack);
        double a = pop(&stack);
        switch (postfix[i]) {
            case '+': push(&stack, a + b); break;
            case '-': push(&stack, a - b); break;
            case '/': push(&stack, a / b); break;
            case '*': push(&stack, a * b); break;
        }
        }
    }
    return pop(&stack); // 栈顶的值就是最后的运算结果
}
 
int main () {
    char infix[MAXSIZE];
    char postfix[MAXSIZE];
    fgets(infix, MAXSIZE, stdin);


    infixToPostfix(infix, postfix); // 转换为后缀表达式
    printf("Postfix expression: %s\n", postfix);

    double result = evaluatePostfix(postfix); // 计算后缀表达式
    printf("Result: %.2lf\n", result); // 输出计算结果

    return 0;
}

