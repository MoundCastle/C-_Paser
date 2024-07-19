# 构造C- 语言的词法分析器和语法分析器
> 词法分析通过手工实现，预处理文本后通过扫描器每次取出一个合法词素进行处理。
> 语法分析器通过自顶向下LL(1)分析，构建First集、Follow集、预测分析表配合分析栈、符号队列完成文法分析。

**未完成部分**：生成语法树，没有生成语法树，后续接不上。不过打出了详细的预测分析、字符与产生式匹配过程。

**词法分析输出：**

![image](https://github.com/user-attachments/assets/5b56ba7e-9177-409d-9c6c-9c55fab710d2)

**语法分析输出：**存储了First集、Follow集、预测分析表等

![image](https://github.com/user-attachments/assets/97eba6fc-57bf-4d57-a742-e21d5f3489aa)
