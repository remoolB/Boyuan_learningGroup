#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// 用结构体封装表达式的元素（数字和运算符）
typedef struct {
    char type; // 运算符（o）或数字(n)  x表示无效元素 
    int value;  //n
    char operator;  //o
} ExpressionElement;

int isOperator(char ch) { // 判断是否为运算符
    switch (ch) {
        case '+': 
        case '-': 
        case '*': 
        case '/': return 1;
        default: return 0;  
    }
}

int isDigit(char ch) { // 判断是否为数字
    return (ch >= '0' && ch <= '9');
}

int parseNumber (const char* expression, int* i) {
            char number[10]; // 暂存数字字符
            int cntNum = 0; // 数字计数器

            //读取完整数字
            while (isdigit(expression[*i])) {
                number[cntNum++] = expression[(*i)++];
            }
            number[cntNum] = '\0';  
            return atoi(number);// 将字符串转换为整数
}



// 读取表达式
void parse_expression(const char* expression, ExpressionElement elements[], int* element_count ){
    int i = 0; //遍历
    int cnt = 0; // 元素计数器

    while (expression[i] != '\0') {   
        
        if (isDigit(expression[i])) { // 数字
            elements[cnt].type = 'n';                
            elements[cnt].value = parseNumber(expression, &i);
            cnt++;  
        }
        
        // 运算符
        else if (isOperator(expression[i])) {
            elements[cnt].type = 'o'; 
            elements[cnt].operator = expression[i++];
            cnt++;   
        }

        // 空格
        else {
            i++;   
        }
    }
    *element_count = cnt;   
}


// 运算模块  （后续可进一步模块化）
int calculate (int left, int right, char operator) {
    switch (operator) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0;
        default: return 0;
    }
}

int evaluate_expression(ExpressionElement elements[], int element_count) {
    // 乘法和除法
    for (int i = 0; i < element_count; i++) {
        if (elements[i].type == 'o' && (elements[i].operator == '*' || elements[i].operator == '/')) {
            // 替换左侧操作数
            elements[i-1].value = calculate(elements[i-1].value, elements[i+1].value, elements[i].operator);
            // 移除该运算符和右侧操作数
            elements[i].type = 'x';
            elements[i+1].type = 'x';
            i++;   
        }
    }
    // 加法和减法
    int result = elements[0].value; // 结果初始化为第一个操作数
    for (int i = 1; i < element_count; i++) {
        if (elements[i].type == 'o' && (elements[i].operator == '+' || elements[i].operator == '-')) {
            int FindValidNumber = i + 1; // 用来寻找下一个有效数字
            while (FindValidNumber < element_count && elements[FindValidNumber].type == 'x') {
                FindValidNumber++; // 跳过无效元素
            }
            if (FindValidNumber < element_count) {
                // 执行运算
                result = calculate(result, elements[FindValidNumber].value, elements[i].operator);
            }

        }

    }
    return result;
}

int main() {
     char expression[100]; // 存储用户输入
    printf("请输入表达式: ");
    fgets(expression, sizeof(expression), stdin); 

     int result;
     ExpressionElement elements[100]; // 表达式元素数组
     int element_count; // 元素计数器

     // 解析表达式
     parse_expression(expression, elements, &element_count);

     // 计算表达式
     result = evaluate_expression(elements, element_count);

     printf("%d\n", result); // 输出结果
     return 0;
}