
# sscanf 函数使用说明

`sscanf` 是 C 语言中的标准库函数，用于**从字符串中读取格式化数据**。可以把它理解成 `scanf` 的字符串版本：`scanf` 从标准输入读取，而 `sscanf` 从指定字符串读取。

## 函数原型

```c
#include <stdio.h>

int sscanf(const char *str, const char *format, ...);
```

- `str`：输入字符串（要解析的源字符串）
- `format`：格式控制字符串
- `...`：变量地址列表，用于存储解析结果
- 返回值：成功匹配的字段个数（整数），如果没有匹配，返回 0；发生错误返回 `EOF`

## 格式说明符

| 说明符 | 作用 |
|--------|------|
| `%d`   | 整数 |
| `%f`   | 浮点数 |
| `%s`   | 字符串（遇空格停止） |
| `%c`   | 单个字符 |
| `%[...]` | 匹配指定字符集合 |
| `%n`   | 保存当前已读字符数，不增加返回值 |

可以指定最大长度，例如：

```c
char name[20];
sscanf(str, "%19s", name); // 最多读取 19 个字符，留一个给 '\0'
```

## 示例 1：解析学生信息

```c
#include <stdio.h>

int main() {
    char line[] = "2024001 ZhangSan 88 90 85";
    char num[20], name[20];
    float c, math, eng;

    int ret = sscanf(line, "%19s %19s %f %f %f", num, name, &c, &math, &eng);
    printf("num=%s, name=%s, c=%.2f, math=%.2f, eng=%.2f\n", num, name, c, math, eng);
    printf("成功解析字段数: %d\n", ret);

    return 0;
}
```

输出示例：

```
num=2024001, name=ZhangSan, c=88.00, math=90.00, eng=85.00
成功解析字段数: 5
```

## 检测多余字段

```c
char line[] = "2024001 ZhangSan 88 90 85 extra";
char extra[256];
int ret = sscanf(line, "%19s %19s %f %f %f %255s", num, name, &c, &math, &eng, extra);
if (ret != 5) {
    printf("行格式错误或多余字段\n");
}
```

## 注意事项

1. **不要越界**：确保数组足够存储字符串并留一个 `\0`
2. **空格处理**：`%s` 遇空格停止，如果字段中允许空格，可用 `%[^\n]` 或 `%[^ ]`
3. **返回值**：用于判断解析字段数，检测格式正确性
4. **安全性**：尽量指定 `%s` 的最大长度

# fprintf 函数使用说明

## 一、函数简介

`fprintf` 是 C 语言中的标准库函数，定义在 `<stdio.h>` 头文件中。

它的作用是：**按照指定格式，把数据输出到文件中**。

可以把它理解为：

- `printf`：输出到屏幕
- `fprintf`：输出到文件

---

## 二、函数原型

```c
#include <stdio.h>

int fprintf(FILE *stream, const char *format, ...);
```

---

## 三、参数说明

| 参数 | 含义 |
|---|---|
| `stream` | 文件指针，表示要写入的文件 |
| `format` | 格式控制字符串 |
| `...` | 要输出的数据 |

---

## 四、返回值

`fprintf` 的返回值是成功写入的字符个数。

如果写入失败，返回一个负数。

示例：

```c
int ret = fprintf(fp, "Hello\n");

if (ret < 0) {
    printf("写入失败\n");
}
```

---

## 五、基本用法

```c
#include <stdio.h>

int main() {
    FILE *fp;

    fp = fopen("data.txt", "w");

    if (fp == NULL) {
        printf("文件打开失败\n");
        return 1;
    }

    fprintf(fp, "Hello, file!\n");

    fclose(fp);

    return 0;
}
```

运行后，会在 `data.txt` 文件中写入：

```
Hello, file!
```

---

## 六、写入学生成绩信息

```c
#include <stdio.h>

int main() {
    FILE *fp;
    char num[20] = "2024001";
    char name[20] = "ZhangSan";
    float cScore = 88.5;
    float mathScore = 90.0;
    float englishScore = 85.5;

    fp = fopen("data.txt", "w");

    if (fp == NULL) {
        printf("文件打开失败\n");
        return 1;
    }

    fprintf(fp, "%s %s %.2f %.2f %.2f\n",
            num, name, cScore, mathScore, englishScore);

    fclose(fp);

    return 0;
}
```

写入文件后的内容为：

```
2024001 ZhangSan 88.50 90.00 85.50
```

---

## 七、常见格式控制符

| 格式符 | 含义 |
|---|---|
| `%d` | 输出整数 |
| `%f` | 输出浮点数 |
| `%.2f` | 输出浮点数，保留两位小数 |
| `%c` | 输出字符 |
| `%s` | 输出字符串 |
| `%10.2f` | 宽度为 10，保留 2 位小数 |
| `%-10s` | 左对齐输出字符串，宽度为 10 |

---

## 八、和 printf 的区别

### printf 输出到屏幕

```c
printf("%s %.2f\n", name, cScore);
```

### fprintf 输出到文件

```c
fprintf(fp, "%s %.2f\n", name, cScore);
```

二者格式基本相同，区别在于：

```c
fprintf(fp, ...);
```

多了一个文件指针 `fp`。

---

## 九、在学生成绩管理系统中的应用

例如保存学生信息：

```c
fprintf(fp, "%s %s %.2f %.2f %.2f\n",
        p->data.num,
        p->data.name,
        p->data.cScore,
        p->data.mathScore,
        p->data.englishScore);
```

这段代码的作用是：

把链表中一个学生的信息写入文件，格式为：

```
学号 姓名 C语言成绩 数学成绩 英语成绩
```

例如：

```
2024001 ZhangSan 88.50 90.00 85.50
```

---

## 十、导出成绩报表

```c
fprintf(fp, "%-15s %-15s %10s %10s %10s\n",
        "Num", "Name", "C", "Math", "English");

fprintf(fp, "%-15s %-15s %10.2f %10.2f %10.2f\n",
        "2024001", "ZhangSan", 88.5, 90.0, 85.5);
```

输出效果：

```
Num             Name                     C       Math    English
2024001         ZhangSan              88.50      90.00      85.50
```

---

## 十一、使用 fprintf 的完整示例

```c
#include <stdio.h>

int main() {
    FILE *fp;
    int id = 1;
    char name[20] = "LiMing";
    float score = 92.5;

    fp = fopen("student.txt", "w");

    if (fp == NULL) {
        printf("文件打开失败\n");
        return 1;
    }

    fprintf(fp, "学生信息如下：\n");
    fprintf(fp, "编号：%d\n", id);
    fprintf(fp, "姓名：%s\n", name);
    fprintf(fp, "成绩：%.2f\n", score);

    fclose(fp);

    return 0;
}
```

文件内容：

```
学生信息如下：
编号：1
姓名：LiMing
成绩：92.50
```

---

## 十二、注意事项

1. 使用 `fprintf` 前必须先打开文件：

```c
FILE *fp = fopen("data.txt", "w");
```

2. 使用完文件后必须关闭文件：

```c
fclose(fp);
```

3. 打开文件后要判断是否成功：

```c
if (fp == NULL) {
    printf("文件打开失败\n");
    return 1;
}
```

4. `"w"` 模式会清空原文件内容，如果想追加，应使用 `"a"` 模式：

```c
fopen("data.txt", "a");
```

---

## 十三、总结

`fprintf` 是 C 语言中常用的文件输出函数，用于按照指定格式把数据写入文件。

在学生成绩管理系统中，主要用于将链表中的学生数据保存到 `data.txt`，或将统计结果导出到报表文件 `export.txt`。
