# SourcePawn 过渡语法

我们希望为用户提供一种更现代的语言。Pawn 正在老化；手动内存管理、缓冲区、标签和缺乏面向对象的 API 非常令人沮丧。我们无法一次性解决所有问题，但我们可以开始朝着正确的方向迈出步伐。

SourceMod 1.7 引入了一个过渡 API。它建立在 SourcePawn 中新的过渡语法之上，这是一套使 Pawn 感觉更现代的语言工具。特别是，它允许开发人员以面向对象的方式使用旧的 API，而不会破坏兼容性。有朝一日，如果 SourcePawn 能够成为一种功能齐全的现代语言，过渡 API 将使移植工作变得非常轻松。

过渡 API 具有以下主要特性和变化：

*   **新的声明符** - 一种更清晰的变量声明方式，类似于 Java 和 C#。
*   **Methodmaps** - 围绕旧 API 的面向对象包装器。
*   **实类型** - SourcePawn 现在将 "int"、"float"、"bool"、"void" 和 "char" 作为实类型。
*   **null** - 一个新的通用关键字，用于替换 `INVALID_HANDLE`。

## 目录
1. [新的声明符](#新的声明符)
   1. [基本原理](#基本原理)
   2. [数组](#数组)
      1. [基本原理](#数组基本原理)
   3. [示例](#示例)
   4. [视图转换](#视图转换)
   5. [语法](#语法)
2. [Methodmaps](#methodmaps)
   1. [简介](#简介)
   2. [继承](#继承)
   3. [内联方法](#内联方法)
   4. [自定义标签](#自定义标签)
   5. [构造函数和析构函数](#构造函数和析构函数)
   6. [注意事项](#注意事项)
   7. [语法](#methodmaps语法)
3. [Typedefs](#typedefs)
4. [Enum Structs](#enum-structs)
5. [强制使用新语法](#强制使用新语法)

## 新的声明符

熟悉 Pawn 的开发人员会认识到 Pawn 原始的声明风格：

```pawn
new Float:x = 5.0;
new y = 7;
```

在过渡语法中，这可以改写为：

```pawn
float x = 5.0;
int y = 7;
```

以下内置标签现在有了类型：

*   `Float:` 是 `float`。
*   `bool:` 是 `bool`。
*   `_:` (或无标签) 是 `int`。
*   `String:` 是 `char`。
*   `void` 现在可以用作函数的返回类型。

### 基本原理

在旧风格中，带标签的变量不是实类型。`Float:x` 并不表示一个浮点类型的变量，它表示一个标记为浮点数的 32 位“单元”。可以移除标签或更改标签，这虽然灵活，但既危险又令人困惑。语法本身也存在问题。解析器无法识别标签名和冒号之间的字符。在内部，编译器无法表示不完全是 32 位的值。

关键信息是：没有合理的方法来表示像 "int64" 或 "X 是一个表示浮点数数组的类型" 这样的概念。标签语法使其过于笨拙，编译器本身也无法将此类信息附加到标签上。我们无法立即解决这个问题，但我们可以通过更常规的声明语法开始弃用标签系统。

一个简单的例子可以说明为什么这是必要的，即用标签表达以下内容时的怪异之处：

```pawn
native float[3] GetEntOrigin();
```

**关于 `String` 标签的说明**：`char` 的引入最初可能看起来令人困惑。原因是，在未来，我们希望引入一个真正的 "string" 类型，它像大多数其他语言中的字符串一样作为对象。Pawn 中现有的行为是字符数组，这是更底层的。重命名澄清了该类型的真实含义，并为将来更好的类型留下了空间。

### 数组

新的声明风格区分了两种数组。Pawn 有不确定大小的数组（大小未知）和确定大小的数组（大小已知）。我们分别称之为“动态”数组和“固定长度”数组。

固定长度数组通过在变量名后放置方括号来声明。例如：

```pawn
int CachedStuff[1000];
int PlayerData[MAXPLAYERS + 1] = { 0, ... };
int Weapons[] = { WEAPON_AK47, WEAPON_GLOCK, WEAPON_KNIFE };
```

在这些示例中，数组大小是固定的。大小是预先知道的，不能改变。当在此位置使用方括号时，必须指定数组大小，可以通过显式大小或从初始值推断。

动态长度数组的方括号在变量名之前，即在类型之后。最常见的情况是指定将数组作为输入的函数时：

```pawn
native void SetPlayerName(int player, const char[] name);
```

这里，我们指定 `name` 的长度不总是已知的——它可以是任何长度。

动态数组也可以在局部作用域中创建。例如：

```pawn
void FindPlayers()
{
  int[] players = new int[MaxClients + 1];
}
```

这会分配一个给定大小的新数组，并将引用放在 `players` 中。当不再使用时，内存会自动释放。

用不确定数组初始化固定长度数组是非法的，用固定数组初始化动态数组也是非法的。在动态长度数组上指定固定大小也是非法的。

在很大程度上，这不会改变现有的 Pawn 语义。它只是旨在阐明数组工作方式的新语法。

#### 数组基本原理

在原始语法中，数组声明有一个微妙的区别：

```pawn
new array1[MAXPLAYERS + 1];
new array2[MaxClients + 1];
```

这里，`array1` 和 `array2` 是非常不同的类型：前者是 `int[65]`，后者是 `int[]`。然而，没有语法上的区别：编译器必须推断大小表达式是否为常量来确定类型。新语法清晰明确地区分了这些情况，因此在未来我们引入完全动态和灵活的数组时，我们不太可能破坏现有代码。

这可能会导致一些混淆。希望从长远来看这无关紧要，因为一旦我们有了真正的动态数组，固定数组将变得不那么有用和晦涩。

限制初始化器的理由是相似的。一旦我们有了真正的动态数组，这些限制将被解除。同时，我们需要确保我们仅限于在未来不会有细微差异的语义。

### 示例

```pawn
float x = 5.0;    // 替代 "new Float:x = 5.0;"
int y = 4;        // 替代 "new y = 4;"
char name[32];    // 替代 "new String:name[32];" 和 "decl String:name[32];"
 
void DoStuff(float x, int y, char[] name, int length) { // 替代 "DoStuff(Float:x, y, String:name[], length)"
  Format(name, length, "%f %d", x, y); // 这里没有替代
}
```

### 视图转换

一个新的运算符可用于将值中的位重新解释为另一种类型。这个运算符叫做 `view_as`。它不是一个安全的转换，因为即使实际值不符合任一类型，它也可以将一种类型转换为另一种类型。

在过渡语法之前，这被称为“重新标记”。重新标记不支持新式类型，这就是引入此运算符的原因。前后代码示例：

```pawn
// 之前:
float x = Float:array.Get(i);
 
// 之后:
float y = view_as<float>(array.Get(i));
```

值得重申的是，这不是一个类型转换。如果数组中的值不是浮点数，那么当“视为”浮点数时，它可能会看起来非常奇怪。

### 语法

新的和旧的声明语法如下。

```
return-type ::= return-old | return-new
return-new ::= type-expr new-dims?        // 注意，尚不支持 dims。
return-old ::= old-dims? label?

argdecl ::= arg-old | arg-new
arg-new ::= "const"? type-expr '&'? symbol old-dims? ('=' arg-init)?
arg-old ::= "const"? tags? '&'? symbol old-dims? ('=' arg-init)?

vardecl ::= var-old | var-new
var-new ::= var-new-prefix type-expr symbol old-dims?
var-new-prefix ::= "static" | "const"
var-old ::= var-old-prefix tag? symbol old-dims?
var-old-prefix ::= "new" | "decl" | "static" | "const"

global ::= global-old | global-new
global-new ::= storage-class* type-expr symbol old-dims?
global-old ::= storage-class* tag? symbol old-dims?

storage-class ::= "public" | "static" | "const" | "stock"

type-expr ::= (builtin-type | symbol) new-dims?
builtin-type ::= "void"
               | "int"
               | "float"
               | "char"
               | "bool"

tags ::= tag-vector | tag
tag-vector ::= '{' symbol (',' symbol)* '}' ':'
tag ::= label

new-dims ::= ('[' ']')*
old-dims ::= ('[' expr? ']')+

label ::= symbol ':'
symbol ::= [A-Za-z_]([A-Za-z0-9_]*)
```

另请注意，新的声明符语法中没有与 `decl` 等效的语法。`decl` 被认为是危险和不必要的。如果数组的零初始化成本太高，请考虑将其设为 `static` 或 `global`。

## Methodmaps

### 简介

Methodmaps 很简单：它们将方法附加到 `enum` 上。例如，这是我们用于 `Handle` 的旧版 API：

```pawn
native Handle CloneHandle(Handle handle);
native void CloseHandle(Handle handle);
```

这是我们旧版 API 的一个很好的例子。使用它通常看起来像这样：

```pawn
Handle array = CreateAdtArray();
PushArrayCell(array, 4);
CloseHandle(array);
```

太糟糕了！Methodmap 可以通过将函数附加到 `Handle` 标签来清理它，像这样：

```pawn
methodmap Handle {
    public native Handle Clone() = CloneHandle;
    public native void Close() = CloseHandle;
};
```

现在，我们早期的数组代码可以开始看起来像面向对象的了：

```pawn
Handle array = CreateAdtArray();
PushArrayCell(array, 4);
array.Close();
```

例如，对于数组的完整 methodmap，

```pawn
methodmap ArrayList < Handle
{
  public native ArrayList(); // 构造函数
  public native void Push(any value);
};
```

我们可以编写更像对象的代码：

```pawn
ArrayList array = new ArrayList();
array.Push(4);
delete array;
```

（注意：官方 API 不在原始 `Handle` 上公开方法。）

### 继承

`Handle` 系统有一个“弱”层次结构。所有句柄都可以传递给 `CloseHandle`，但只有 `AdtArray` 句柄可以传递给像 `PushArrayCell` 这样的函数。这种层次结构不是通过标签强制执行的（不幸的是），而是通过运行时检查。Methodmaps 允许我们使单个句柄类型面向对象，同时也将类型检查移入编译器。

例如，这是一个用于数组的过渡 API：

```pawn
native AdtArray CreateAdtArray();
 
methodmap AdtArray < Handle {
    public native void PushCell(any value) = PushArrayCell;
};
```

请注意，`CreateAdtArray` 现在返回 `AdtArray` 而不是 `Handle`。通常这会破坏旧代码，但由于 `AdtArray` 继承自 `Handle`，类型系统中有一个特殊规则，允许将 `AdtArray` 强制转换为 `Handle`（但反之则不行）。

现在，API 看起来更面向对象了：

```pawn
AdtArray array = CreateAdtArray();
array.PushCell(4);
array.Close();
```

### 内联方法

Methodmaps 可以声明内联方法和访问器。内联方法可以是 natives 或 Pawn 函数。例如：

```pawn
methodmap AdtArray {
    public native void PushCell(any value);
};
```

这个例子要求在 SourceMod 的某个地方存在一个名为 "AdtArray.PushCell" 的 native。它有一个名为 "this" 的神奇初始参数，所以签名会像这样：

```pawn
native void AdtArray.PushCell(AdtArray this, any value);
```

（当然，这个确切的签名不会出现在包含文件中——但这是 C++ 实现应该期望的签名。）

也可以定义没有 native 的新函数：

```pawn
methodmap AdtArray {
    public native void PushCell(any value);
 
    public void PushCells(any[] list, int count) {
        for (int i = 0; i < count; i++) {
            this.PushCell(i);
        }
    }
};
```

最后，我们还可以定义访问器。例如，

```pawn
methodmap AdtArray {
    property int Size {
        public native get() = GetArraySize;
    }
    property bool Empty {
        public get() {
            return this.Size == 0;
        }
    }
    property int Capacity {
        public native get();
    }
};
```

第一个访问器只是将一个现有函数分配为 "Size" 的访问器。第二个访问器是一个带有隐式 "this" 参数的内联方法。第三个访问器将绑定到一个具有以下名称和签名的 native：

```pawn
native int AdtArray.Capacity.get(AdtArray this);
```

也支持 Setter。例如：

```pawn
methodmap Player {
    property int Health {
        public native get();
        public native set(int health);
    }
}
```

### 自定义标签

Methodmaps 不必与 `Handle` 一起使用。可以在新的或现有的标签上定义自定义 methodmaps。例如：

```pawn
methodmap AdminId {
    public int Rights() {
        return GetAdminFlags(this);
    }
};
```

现在，例如，可以这样做：

```pawn
GetPlayerAdmin(id).Rights()
```

### 构造函数和析构函数

Methodmaps 也可以定义构造函数，如果它们旨在像实际对象一样行事，这将非常有用。例如，

```pawn
methodmap AdtArray {
    public native AdtArray(int blocksize = 1);
    public native void PushCell(any value);
};
```

现在 `AdtArray` 可以以完全面向对象的方式使用：

```pawn
AdtArray array = new AdtArray();
array.PushCell(10);
array.PushCell(20);
array.PushCell(30);
delete array;
```

在 SourcePawn 1.8 中移除了在 methodmaps 上定义析构函数的支持。`Handle` 类型可以在 SourceMod 的核心中作为 native 代码或作为扩展来实现析构函数。

### 注意事项

methodmaps 有一些注意事项：

*   `CloseHandle()` 尚未消失。对于任何以前需要 `CloseHandle()` 的对象，都需要调用 `delete`。
*   一个标签只能有一个 methodmap。
*   使用现有 natives 时，native 的第一个参数必须能强制转换为 methodmap 的标签。 "this" 参数的标签不匹配将导致错误。不是警告！
*   Methodmaps 只能在标签上定义。Pawn 有一些创建实际类型的方法（如通过 `struct` 或 `class`）。Methodmaps 不能在这些类型上创建。
*   Methodmaps 没有强类型。例如，仍然可以执行像 `Float:CreateAdtArray()` 这样的“非法”转换。这对于向后兼容性是必要的，因此 methodmap 值可以流入像 `PrintToServer` 或 `CreateTimer` 这样的 natives。
*   只能从另一个先前声明的 methodmap 继承。
*   Methodmaps 只能在标量上定义——也就是说，“this”参数永远不能是数组。这意味着它们不能用于 enum-structs。
*   析构函数只能是 native。当我们能够实现垃圾回收时，析构函数将被移除。
*   methodmaps 的签名必须使用新的声明语法。
*   Methodmaps 必须在使用前声明。

### Methodmaps语法

methodmaps 的语法是：

```
visibility ::= "public"
method-args ::= arg-new* "..."?

methodmap ::= "methodmap" symbol methodmap-inheritance? "{" methodmap-item* "}" term
methodmap-inheritance ::= "<" symbol
methodmap-item ::=
           visibility "native" "~"? symbol "(" method-args* ")" ("=" symbol)? term
         | visibility "~"? symbol "(" method-args* ")" func-body newline
         | visibility "static"? "native" type-expr symbol "(" method-args* ")" ("=" symbol)? term
         | visibility "static"? type-expr symbol "(" method-args* ")" func-body newline
         | "property" type-expr symbol "{" property-decl* "}" newline
property-decl ::= visibility property-impl
property-impl ::=
           "native" "get" "(" ")" ("=" symbol)? term
         | "get" "(" ")" func-body newline
         | "native" "set" "(" type-expr symbol ")" ("=" symbol)? term
         | "set" "(" type-expr symbol ")" func-body newline
```

## Typedefs

函数标签和函数枚举已被弃用，取而代之的是更现代的语法。目前，它们仍然只能为函数创建标签名。未来版本将支持任意类型。

升级 `functag` 和 `funcenum` 都很简单。以下是两个例子：

```pawn
functag public Action:SrvCmd(args);
 
funcenum Timer {
  Action:public(Handle:Timer, Handle:hndl),
  Action:public(Handle:timer),
};
```

现在，这变成了：

```pawn
typedef SrvCmd = function Action (int args);
 
typeset Timer {
  function Action (Handle timer, Handle hndl);
  function Action (Handle timer);
};
```

新语法的语法是：

```
typedef ::= "typedef" symbol "=" full-type-expr term
full-type-expr ::= "(" type-expr ")"
                 | type-expr
type-expr ::= "function" type-name "(" typedef-args? ")"
typedef-args ::= "..."
               | typedef-arg (", " "...")?
```

请注意，`typedef` 只支持新式类型。

## Enum Structs

Enum structs 是以前不支持的通过数组模拟结构体的机制。从 SourceMod 1.10 开始，该机制通过过渡语法得到完全支持。以下是 enum struct 语法的示例：

```pawn
enum struct Rectangle {
  int x;
  int y;
  int width;
  int height;
 
  int Area() {
    return this.width * this.height;
  }
}
 
void DoStuff(Rectangle r) {
  PrintToServer("%d, %d, %d, %d", r.x, r.y, r.width, r.height);
}
```

Enum structs 是语法糖，内部表示为数组。这意味着它们通过引用传递给函数参数，并且不需要（也不允许）使用 "&" 标记。

请注意，即使 enum structs 实际上是数组，在大多数情况下它们也不能用作数组。例外情况是与不透明数据结构（如 `ArrayList`）交互时。例如，这是有效的：

```pawn
void SaveRectangle(ArrayList list, const Rectangle r) {
  list.PushArray(r, sizeof(r));
}
 
void PopArray(ArrayList list, Rectangle r) {
  list.GetArray(list.Length - 1, r, sizeof(r));
  list.Erase(list.Length - 1);
}
```

但这是不允许的：

```pawn
Rectangle r;
PrintToServer("%d", r[0]);
```

enum structs 的语法如下：

```
enum-struct ::= "enum" "struct" symbol "{" newline enum-struct-entry enum-struct-entry* "}" term
enum-struct-entry ::= enum-struct-field
                    | enum-struct-method
enum-struct-field ::= type-expr symbol old-dims? term
enum-struct-method ::= type-expr symbol "(" method-args ")" func-body term
```
