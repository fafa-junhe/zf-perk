import ast
import os
from pathlib import Path
import sys
import re


def camel_to_snake(name: str) -> str:
    """将驼峰式命名转换为大写蛇形命名。"""
    name = re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name).upper()


# 将 src/python 添加到 sys.path 以便导入
sys.path.insert(0, str(Path("src/python").resolve()))

# --- 配置 ---
ROOT_DIR = Path(__file__).parent.parent.resolve()
SOURCE_DIR = ROOT_DIR / "src" / "python" / "perks"
SOURCEPAWN_DIR = ROOT_DIR / "src" / "sourcepawn"
OUTPUT_DIR = ROOT_DIR / "src" / "sourcepawn"
PERKS_OUTPUT_DIR = OUTPUT_DIR / "perks"


# --- 类型映射 ---
TYPE_MAP = {
    "int": "int",
    "float": "float",
    "str": "char[]",
    "bool": "bool",
    "void": "void"
}

def map_type(py_type: str) -> str:
    """将Python类型提示转换为SourcePawn类型。"""
    if not py_type:
        return "any"
    if py_type.startswith("list[") and py_type.endswith("]"):
        inner_type = py_type[5:-1]
        return f"{map_type(inner_type)}[]"
    if py_type.startswith("tuple[") and py_type.endswith("]"):
        return "int[]"
    return TYPE_MAP.get(py_type, "any")

class PawnDirectiveParser:
    """
    解析源文件以查找 # P: 指令。
    """
    def __init__(self, source_code: str):
        self.source_lines = source_code.splitlines()
        self.directives = {}
        self._parse_directives()

    def _parse_directives(self):
        """
        遍历所有行以查找和存储 # P: 指令。
        """
        for i, line in enumerate(self.source_lines):
            line_num = i + 1
            stripped_line = line.strip()
            
            # 查找指令 '# P:'
            directive_marker = "# P:"
            marker_pos = line.find(directive_marker)

            if marker_pos != -1:
                pawn_code = line[marker_pos + len(directive_marker):].strip()
                code_part = line[:marker_pos].strip()

                # 如果该行除了指令外没有其他代码，则为注入
                if not code_part:
                    self.directives[line_num] = {"type": "injection", "code": pawn_code}
                # 否则为替换
                else:
                    self.directives[line_num] = {"type": "replacement", "code": pawn_code}

class PawnExpressionVisitor(ast.NodeVisitor):
    """将单个Python表达式AST节点转换为SourcePawn字符串。"""
    def __init__(self, class_node: ast.ClassDef | None, class_db: dict, class_attrs: dict = None, string_params: dict = None, local_string_vars: set = None, current_class_name: str = ""):
        self.class_db = class_db
        self.class_attrs = class_attrs if class_attrs is not None else {}
        self.string_params = string_params if string_params is not None else {}
        self.local_string_vars = local_string_vars if local_string_vars is not None else set()
        self.current_class_name = current_class_name
        self.known_methods = set()
        if class_node:
            self.known_methods = {item.name for item in class_node.body if isinstance(item, ast.FunctionDef)}

    def is_string_expr(self, node: ast.AST) -> bool:
        """检查一个AST节点是否表示一个字符串类型。"""
        if isinstance(node, ast.Constant) and isinstance(node.value, str):
            return True
        if isinstance(node, ast.Name):
            if node.id in self.string_params or node.id in self.local_string_vars:
                return True
        if isinstance(node, ast.Attribute):
            # This is a simplification. A real implementation would need type inference.
            if isinstance(node.value, ast.Name) and node.value.id == 'self':
                if self.class_attrs.get(node.attr) == 'str':
                    return True
        # A BinOp with '+' and a string operand is also a string expression
        if isinstance(node, ast.BinOp) and isinstance(node.op, ast.Add):
            if self.is_string_expr(node.left) or self.is_string_expr(node.right):
                return True
        return False

    def visit_Constant(self, node: ast.Constant):
        if node.value is None:
            return "null"
        if isinstance(node.value, str):
            return f'"{node.value}"'
        if isinstance(node.value, bool):
            return str(node.value).lower()
        return str(node.value)

    def visit_Name(self, node: ast.Name):
        if node.id == "self":
            return "self" # Changed from "this"
        return node.id

    def visit_Attribute(self, node: ast.Attribute):
        value_str = self.visit(node.value)
        if value_str == "self":
            # Method calls are handled in visit_Call, so this is for property access
            if node.attr not in self.known_methods:
                attr_type = self.class_attrs.get(node.attr)
                param_name = "owner" if node.attr == "client" else node.attr
                
                if attr_type == 'str':
                     return f'getParamString_placeholder("{param_name}")'

                return f'getParam(self, "{param_name}")'
        
        # Handle string methods like .lower()
        if node.attr == 'lower':
             # This is a simplification. A real implementation would need a temporary
             # buffer and a call to a string manipulation function.
             # For now, we assume the context handles it.
            return f"{value_str}" # In SP, string comparison can handle this.

        # Handle Enum-like access: ZFStat.ZFStatAtt -> ZFStatAtt
        if node.attr.startswith(value_str):
            return node.attr

        return f"{value_str}.{node.attr}"

    def visit_Call(self, node: ast.Call):
        # Handle this.method() calls
        if isinstance(node.func, ast.Attribute):
            if isinstance(node.func.value, ast.Name) and node.func.value.id == 'self':
                method_name = node.func.attr
                # Check if it's a known method of the current class
                if method_name in self.known_methods:
                    args = ["self"] + [self.visit(arg) for arg in node.args]
                    return f"{self.current_class_name}_{method_name}({', '.join(args)})"
            
            if node.func.attr == 'find':
                obj_str = self.visit(node.func.value)
                args_str = [self.visit(arg) for arg in node.args]
                return f"FindCharInString({obj_str}, {', '.join(args_str)})"

        # Handle class instantiation: MyClass() -> MyClass_new()
        if isinstance(node.func, ast.Name) and node.func.id in self.class_db:
            callee_str = self.visit(node.func)
            args = [self.visit(arg) for arg in node.args]
            return f"{callee_str}_new({', '.join(args)})"

        if isinstance(node.func, ast.Name):
            if node.func.id == 'len':
                arg_str = self.visit(node.args[0])
                return f"strlen({arg_str})"

        # Generic call
        callee_str = self.visit(node.func)
        args = [self.visit(arg) for arg in node.args]
        return f"{callee_str}({', '.join(args)})"

    def visit_UnaryOp(self, node: ast.UnaryOp):
        op_map = {ast.USub: "-", ast.Not: "!"}
        op_str = op_map.get(type(node.op), "")
        operand_str = self.visit(node.operand)
        # Add parentheses for `not` to ensure correct precedence
        if isinstance(node.op, ast.Not):
            return f"{op_str}({operand_str})"
        return f"{op_str}{operand_str}"

    def visit_Compare(self, node: ast.Compare):
        left = node.left
        op = node.ops[0]
        right = node.comparators[0]

        # Check for string operations
        if self.is_string_expr(left) or self.is_string_expr(right):
            left_str = self.visit(left)
            right_str = self.visit(right)
            if isinstance(op, ast.Eq):
                return f"StrEqual({left_str}, {right_str})"
            if isinstance(op, ast.NotEq):
                return f"!StrEqual({left_str}, {right_str})"
            if isinstance(op, ast.In):
                # Python `in` for strings is `substr in str`
                # SourcePawn StrContains is `StrContains(str, substr)`
                return f"(StrContains({right_str}, {left_str}) != -1)"
            if isinstance(op, ast.NotIn):
                return f"(StrContains({right_str}, {left_str}) == -1)"

        # Fallback to default behavior
        left_str = self.visit(node.left)
        op_map = {
            ast.Eq: "==", ast.NotEq: "!=", ast.Lt: "<",
            ast.LtE: "<=", ast.Gt: ">", ast.GtE: ">=",
            ast.Is: "==", ast.IsNot: "!="
        }
        op_str = op_map.get(type(op), "/*?*/")
        right_str = self.visit(right)
        return f"({left_str} {op_str} {right_str})"

    def visit_Subscript(self, node: ast.Subscript):
        value_str = self.visit(node.value)
        if isinstance(node.slice, ast.Slice):
            lower_str = self.visit(node.slice.lower) if node.slice.lower else "0"
            upper_str = self.visit(node.slice.upper) if node.slice.upper else "-1"
            # This is a placeholder for a more complex substring implementation
            return f"String_Substring({value_str}, {lower_str}, {upper_str})"
        
        index_str = self.visit(node.slice)
        return f"{value_str}[{index_str}]"

    def visit_BinOp(self, node: ast.BinOp):
        # Special handling for string concatenation
        if isinstance(node.op, ast.Add) and (self.is_string_expr(node.left) or self.is_string_expr(node.right)):
            # This visitor just returns a representation of the operation.
            # The statement converter is responsible for creating buffers and calling functions.
            left_str = self.visit(node.left)
            right_str = self.visit(node.right)
            return f"({left_str} + {right_str})" # Placeholder

        left_str = self.visit(node.left)
        right_str = self.visit(node.right)
        op_map = {
            ast.Add: "+", ast.Sub: "-", ast.Mult: "*", ast.Div: "/", ast.Mod: "%"
        }
        op_str = op_map.get(type(node.op), "/*?*/")
        return f"({left_str} {op_str} {right_str})"

    def visit_BoolOp(self, node: ast.BoolOp):
        op_map = {ast.And: "&&", ast.Or: "||"}
        op_str = op_map.get(type(node.op), "/*?*/")
        # Ensure parentheses for nested bool ops
        values_str = []
        for v in node.values:
            val_str = self.visit(v)
            if isinstance(v, ast.BoolOp):
                values_str.append(f"({val_str})")
            else:
                values_str.append(val_str)
        return f"({f' {op_str} '.join(values_str)})"

    def visit_IfExp(self, node: ast.IfExp):
        test_str = self.visit(node.test)
        body_str = self.visit(node.body)
        orelse_str = self.visit(node.orelse)
        return f"({test_str} ? {body_str} : {orelse_str})"

    def visit_Dict(self, node: ast.Dict):
        if not node.keys:
            return "new StringMap()"
        return "/* Unsupported Dict expression */"

    def visit_JoinedStr(self, node: ast.JoinedStr):
        # This is a placeholder for f-string support.
        # A full implementation requires Format() and a buffer.
        return '"/* f-string */"'

    def generic_visit(self, node):
        return f"/* Unsupported expression: {type(node).__name__} */"

class BufferCheckVisitor(ast.NodeVisitor):
    """检查一个函数体是否需要一个临时的字符串缓冲区。"""
    def __init__(self, class_attrs):
        self.requires_buffer = False
        self.class_attributes = class_attrs if class_attrs is not None else {}

    def visit_AugAssign(self, node: ast.AugAssign):
        # The buffer is needed for string concatenations on attributes
        if isinstance(node.target, ast.Attribute) and \
           isinstance(node.target.value, ast.Name) and \
           node.target.value.id == 'self':
            attr_type = self.class_attributes.get(node.target.attr)
            if attr_type == 'str':
                self.requires_buffer = True
        self.generic_visit(node)

class PawnClassGenerator:
    """
    生成单个类的SourcePawn函数代码。
    """
    def __init__(self, class_db: dict, directives: dict = None):
        self.class_db = class_db
        self.directives = directives if directives else {}
        self.current_class_name = ""
        self.expression_visitor = PawnExpressionVisitor(None, class_db, current_class_name=self.current_class_name)
        self.class_attributes = {}
        self.temp_string_var_count = 0
        self.string_params = {}
        self.local_string_vars = set()
        self.current_function_returns_string = False

    def _get_temp_string_var(self):
        self.temp_string_var_count += 1
        return f"__temp_str_{self.temp_string_var_count}"

    def generate_for_class(self, class_name: str) -> tuple[str, str]:
        self.current_class_name = class_name
        class_info = self.class_db.get(class_name)
        if not class_info:
            return "", f"// Error: Class '{class_name}' not found in database.\n"

        class_node, _ = class_info
        self.class_attributes = self._parse_class_attributes(class_node)
        self.expression_visitor = PawnExpressionVisitor(class_node, self.class_db, self.class_attributes, self.string_params, self.local_string_vars, class_name)

        base_name, _ = self._get_base_class_info(class_node)

        defines, getters = self._generate_dunder_attributes(class_node)

        code = "" # No more methodmap
        
        # 将方法转换为按行号索引的字典，以便处理指令
        statements = {stmt.lineno: stmt for stmt in class_node.body}
        last_line = 0
        if statements:
            last_line = max(getattr(stmt, 'end_lineno', stmt.lineno) for stmt in statements.values())
        
        # 生成构造函数和方法
        constructor_node = next((item for item in class_node.body if isinstance(item, ast.FunctionDef) and item.name == "__init__"), None)
        if constructor_node:
            code += self._generate_constructor(constructor_node, base_name, class_node.name)
        
        code += getters

        methods_to_generate = self._collect_methods(class_node)
        for func_node in methods_to_generate.values():
            self.temp_string_var_count = 0
            code += self._generate_method(func_node)

        return defines, code


    def _get_base_class_info(self, class_node: ast.ClassDef) -> tuple[str, Path | None]:
        """获取基类名及其文件路径。"""
        if class_node.bases:
            base_node = class_node.bases[0]
            if isinstance(base_node, ast.Name) and base_node.id in self.class_db:
                _, base_path = self.class_db[base_node.id]
                return base_node.id, base_path
        return "StringMap", None

    def _parse_class_attributes(self, class_node: ast.ClassDef) -> dict[str, str]:
        """从类定义中解析带有类型注解的属性。"""
        attrs = {}
        for node in class_node.body:
            if isinstance(node, ast.AnnAssign):
                if isinstance(node.target, ast.Name) and isinstance(node.annotation, ast.Name):
                    attrs[node.target.id] = node.annotation.id
        return attrs
    
    def _generate_function_body(self, node: ast.FunctionDef, indent: str, is_constructor: bool, class_name_for_constructor: str = "") -> str:
        """重构的函数/方法体生成逻辑，以处理指令。"""
        code = ""
        known_locals = {arg.arg for arg in node.args.args if arg.arg != 'self'}
        
        # 预扫描 AnnAssign 字符串变量，在顶部声明它们
        for stmt in node.body:
            if isinstance(stmt, ast.AnnAssign) and isinstance(stmt.target, ast.Name):
                py_type_str = ""
                if stmt.annotation:
                    try:
                        py_type_str = ast.unparse(stmt.annotation)
                    except AttributeError:
                        if isinstance(stmt.annotation, ast.Name):
                            py_type_str = stmt.annotation.id
                
                if map_type(py_type_str) == 'char[]':
                    self.local_string_vars.add(stmt.target.id)
                    code += f"{indent}char {stmt.target.id}[256];\n"

        # 将语句映射到它们的起始行号
        stmt_map = {stmt.lineno: stmt for stmt in node.body}
        
        # 确定函数体的行范围
        start_line = node.lineno
        end_line = getattr(node, 'end_lineno', start_line)
        if node.body:
            end_line = max(getattr(s, 'end_lineno', s.lineno) for s in node.body)
        
        last_processed_line = start_line

        # 逐行处理函数体
        for line_num in range(start_line, end_line + 1):
            directive = self.directives.get(line_num)
            stmt = stmt_map.get(line_num)

            if directive:
                if directive["type"] == "replacement":
                    code += f'{indent}{directive["code"]}\n'
                    if stmt:
                         last_processed_line = getattr(stmt, 'end_lineno', stmt.lineno)
                    continue # 跳过对该语句的常规转换
                elif directive["type"] == "injection":
                    code += f'{indent}{directive["code"]}\n'
            
            if stmt and line_num > last_processed_line:
                 # `is_constructor` 和 `class_name_for_constructor` 特殊处理
                obj_name = class_name_for_constructor if is_constructor else "self"
                code += self._convert_statement(stmt, indent, is_constructor, known_locals, obj_name)
                last_processed_line = getattr(stmt, 'end_lineno', stmt.lineno)

        return code

    def _generate_constructor(self, init_node: ast.FunctionDef, base_name: str, class_name: str) -> str:
        # 1. 解析 __init__ 的参数
        args_list = []
        for arg in init_node.args.args:
            if arg.arg == "self":
                continue
            
            arg_type = "any"
            if arg.annotation:
                try:
                    py_type_str = ast.unparse(arg.annotation)
                    arg_type = map_type(py_type_str)
                except AttributeError:
                    if isinstance(arg.annotation, ast.Name):
                        arg_type = map_type(arg.annotation.id)

            default_val = self._get_arg_default(init_node, arg.arg)
            if default_val:
                args_list.append(f"{arg_type} {arg.arg} = {default_val}")
            else:
                args_list.append(f"{arg_type} {arg.arg}")
        
        args_str = ", ".join(args_list)
        # The return type of the _new function is the base class type
        code = f"stock StringMap {class_name}_new({args_str}) {{\n"

        # 2. 处理基类构造函数调用 (super)
        super_call_stmt = next((
            stmt for stmt in init_node.body
            if (isinstance(stmt, ast.Expr) and
                isinstance(stmt.value, ast.Call) and
                isinstance(stmt.value.func, ast.Attribute) and
                isinstance(stmt.value.func.value, ast.Call) and
                isinstance(stmt.value.func.value.func, ast.Name) and
                stmt.value.func.value.func.id == 'super' and
                stmt.value.func.attr == '__init__')
        ), None)
        
        if super_call_stmt and base_name != "StringMap":
            super_args = [self.expression_visitor.visit(arg) for arg in super_call_stmt.value.args]
            super_call_args = ", ".join(super_args)
            # Assuming the base class also has a _new function
            code += f"    StringMap sm = {base_name}_new({super_call_args});\n"
        else:
            if base_name == "StringMap":
                 code += f"    StringMap sm = new StringMap();\n"
            else:
                 # This case might need a convention, e.g., Base_new()
                 code += f"    StringMap sm = {base_name}_new();\n"
        
        # 3. 生成函数体
        code += self._generate_function_body(init_node, "    ", True, "sm")
        
        code += f"    return sm;\n"
        code += f"}}\n\n"
        return code

    def _generate_dunder_attributes(self, class_node: ast.ClassDef) -> tuple[str, str]:
        """为 __dunder__ 字符串属性生成 #define 和 getter 方法。"""
        defines = ""
        getters = ""
        class_name_snake = camel_to_snake(class_node.name)

        for node in class_node.body:
            if isinstance(node, ast.Assign):
                if len(node.targets) == 1 and isinstance(node.targets[0], ast.Name):
                    attr_name = node.targets[0].id
                    if (
                        attr_name.startswith("__")
                        and attr_name.endswith("__")
                        and len(attr_name) > 4
                        and isinstance(node.value, ast.Constant)
                        and isinstance(node.value.value, str)
                    ):
                        inner_name = attr_name.strip("_")

                        # Define
                        define_name = f"{class_name_snake}_{inner_name}"
                        define_value = f'"{node.value.value}"'
                        defines += f"#define {define_name} {define_value}\n"

                        # Getter
                        inner_name_parts = inner_name.split("_")
                        getter_name_suffix = "".join(p.capitalize() for p in inner_name_parts)
                        getter_name = f"get{getter_name_suffix}"

                        # Getter that copies into a provided buffer
                        getters += f"stock void {class_node.name}_{getter_name}(char[] buffer, int maxlen) {{\n"
                        getters += f"    strcopy(buffer, maxlen, {define_name});\n"
                        getters += f"}}\n\n"
                        
        return defines, getters

    def _collect_methods(self, class_node: ast.ClassDef, collected=None) -> dict:
        if collected is None:
            collected = {}

        for base in reversed(class_node.bases):
            if isinstance(base, ast.Name) and base.id in self.class_db:
                parent_node, _ = self.class_db[base.id]
                self._collect_methods(parent_node, collected)

        for item in class_node.body:
            if isinstance(item, ast.FunctionDef) and not item.name.startswith("__"):
                collected[item.name] = item
        
        return collected

    def _generate_function_signature(self, node: ast.FunctionDef, is_method: bool) -> str:
        """生成SourcePawn的函数或方法签名。"""
        func_name = node.name
        if is_method:
            func_name = f"{self.current_class_name}_{func_name}"

        return_type = "void"
        self.current_function_returns_string = False

        if node.returns:
            try:
                py_type_str = ast.unparse(node.returns)
                mapped_type = map_type(py_type_str)
                if mapped_type == 'char[]':
                    return_type = 'void'
                    self.current_function_returns_string = True
                else:
                    return_type = mapped_type
            except AttributeError: # Fallback for older python
                if isinstance(node.returns, ast.Name):
                    mapped_type = map_type(node.returns.id)
                    if mapped_type == 'char[]':
                        return_type = 'void'
                        self.current_function_returns_string = True
                    else:
                        return_type = mapped_type
                elif isinstance(node.returns, ast.Constant):
                    return_type = map_type(str(node.returns.value))
                elif isinstance(node.returns, ast.Subscript):
                    return_type = "any"  # Typically a generic container or self

        args_list = []
        if is_method:
            class_info = self.class_db.get(self.current_class_name)
            base_name, _ = self._get_base_class_info(class_info[0])
            args_list.append(f"StringMap self")

        # Clear and rebuild string params for the current function
        self.string_params.clear()
        for arg in node.args.args:
            if arg.arg == "self":
                continue
            
            py_type_str = "any"
            if arg.annotation:
                try:
                    py_type_str = ast.unparse(arg.annotation)
                except AttributeError:
                    if isinstance(arg.annotation, ast.Name):
                        py_type_str = arg.annotation.id
            
            arg_type = map_type(py_type_str)
            default_val = self._get_arg_default(node, arg.arg)

            if arg_type == "char[]":
                self.string_params[arg.arg] = f"maxlen_{arg.arg}"
                args_list.append(f"const char[] {arg.arg}") # Make string params const
            else:
                if default_val:
                    args_list.append(f"{arg_type} {arg.arg} = {default_val}")
                else:
                    args_list.append(f"{arg_type} {arg.arg}")
        
        if self.current_function_returns_string:
            args_list.extend(["char[] out", "int maxlen"])

        args_str = ", ".join(args_list)
        
        # All functions are now 'stock'
        return f"stock {return_type} {func_name}({args_str})"

    def _generate_method(self, node: ast.FunctionDef) -> str:
        # Reset local string vars for each method
        self.local_string_vars.clear()
        
        signature = self._generate_function_signature(node, is_method=True)
        self.expression_visitor = PawnExpressionVisitor(self.class_db.get(self.current_class_name)[0], self.class_db, self.class_attributes, self.string_params, self.local_string_vars, self.current_class_name)
        code = f"{signature} {{\n"
        
        # Check if a temp buffer is needed for string operations
        buffer_checker = BufferCheckVisitor(self.class_attributes)
        buffer_checker.visit(node)
        if buffer_checker.requires_buffer:
            code += "    char buffer[256];\n"

        code += self._generate_function_body(node, "    ", False)
        
        code += f"}}\n\n"
        return code

    def _get_arg_default(self, func_node: ast.FunctionDef, arg_name: str) -> str | None:
        # Find the argument in the function's defaults
        for i, arg in enumerate(reversed(func_node.args.args)):
            if arg.arg == arg_name:
                defaults_index = len(func_node.args.defaults) - 1 - i
                if 0 <= defaults_index < len(func_node.args.defaults):
                    return self.expression_visitor.visit(func_node.args.defaults[defaults_index])
        return None

    def _convert_statement(self, stmt: ast.stmt, indent="        ", is_constructor=False, known_locals: set = None, obj_name = "self") -> str:
        # This function now only converts a single statement, assuming no directive applies.
        # The obj_name is passed in to handle constructor context ('sm' vs 'self')

        if isinstance(stmt, ast.Return):
            if self.current_function_returns_string:
                if stmt.value:
                    if isinstance(stmt.value, ast.Call) and isinstance(stmt.value.func, ast.Name) and stmt.value.func.id == 'str':
                        arg_str = self.expression_visitor.visit(stmt.value.args[0])
                        return f"{indent}IntToString({arg_str}, out, maxlen);\n{indent}return;\n"
                    
                    if isinstance(stmt.value, ast.IfExp):
                        test_str = self.expression_visitor.visit(stmt.value.test)
                        body_str = self.expression_visitor.visit(stmt.value.body)
                        orelse_str = self.expression_visitor.visit(stmt.value.orelse)
                        ternary_expr = f"({test_str} ? {body_str} : {orelse_str})"
                        return f"{indent}strcopy(out, maxlen, {ternary_expr});\n{indent}return;\n"

                    value_str = self.expression_visitor.visit(stmt.value)
                    return f"{indent}strcopy(out, maxlen, {value_str});\n{indent}return;\n"
                return f"{indent}return;\n"
            else:
                if stmt.value:
                    value_str = self.expression_visitor.visit(stmt.value)
                    return f"{indent}return {value_str};\n"
                return f"{indent}return;\n"
        
        if isinstance(stmt, ast.Expr) and isinstance(stmt.value, ast.Call):
            call_str = self.expression_visitor.visit(stmt.value)
            # Skip super().__init__() calls as they are handled separately
            if "super" in call_str and "__init__" in call_str:
                return ""
            if is_constructor and call_str.startswith("self."):
                call_str = f"{obj_name}." + call_str[len("self."):]
            return f"{indent}{call_str};\n"

        if isinstance(stmt, ast.Pass):
            return ""
        if isinstance(stmt, ast.Expr) and isinstance(stmt.value, ast.Constant) and isinstance(stmt.value.value, str):
            return ""  # Docstring
        
        if isinstance(stmt, ast.AnnAssign):
            target_node = stmt.target
            if isinstance(target_node, ast.Name) and target_node.id in self.local_string_vars:
                if stmt.value:
                    if isinstance(stmt.value, ast.BinOp) and isinstance(stmt.value.op, ast.Add) and self.expression_visitor.is_string_expr(stmt.value):
                        left_str = self.expression_visitor.visit(stmt.value.left)
                        right_str = self.expression_visitor.visit(stmt.value.right)
                        target_str = target_node.id
                        code = f"{indent}strcopy({target_str}, sizeof({target_str}), {left_str});\n"
                        code += f"{indent}StrCat({target_str}, sizeof({target_str}), {right_str});\n"
                        return code

                    if isinstance(stmt.value, ast.Call) and isinstance(stmt.value.func, ast.Name) and stmt.value.func.id == 'str':
                        arg_str = self.expression_visitor.visit(stmt.value.args[0])
                        return f"{indent}IntToString({arg_str}, {target_node.id}, sizeof({target_node.id}));\n"
                    
                    value_str = self.expression_visitor.visit(stmt.value)
                    return f"{indent}strcopy({target_node.id}, sizeof({target_node.id}), {value_str});\n"
                return ""
            else:
                target_str = self.expression_visitor.visit(target_node)
                value_str = self.expression_visitor.visit(stmt.value)
                return f"{indent}any {target_str} = {value_str};\n"

        if isinstance(stmt, ast.Assign):
            target = stmt.targets[0]
            target_str = self.expression_visitor.visit(target)

            if isinstance(stmt.value, ast.Call) and isinstance(stmt.value.func, ast.Name) and stmt.value.func.id == 'str':
                arg_str = self.expression_visitor.visit(stmt.value.args[0])
                return f"{indent}IntToString({arg_str}, {target_str}, sizeof({target_str}));\n"

            if isinstance(stmt.value, ast.BinOp) and isinstance(stmt.value.op, ast.Add) and self.expression_visitor.is_string_expr(stmt.value):
                left_str = self.expression_visitor.visit(stmt.value.left)
                right_str = self.expression_visitor.visit(stmt.value.right)
                
                code = f"{indent}strcopy({target_str}, sizeof({target_str}), {left_str});\n"
                code += f"{indent}StrCat({target_str}, sizeof({target_str}), {right_str});\n"
                return code

            value_str = self.expression_visitor.visit(stmt.value)

            if isinstance(target, ast.Name) and target.id not in known_locals and target.id not in self.local_string_vars:
                known_locals.add(target.id)
                return f"{indent}any {target.id} = {value_str};\n"

            if isinstance(target, ast.Attribute) and isinstance(target.value, ast.Name) and target.value.id == 'self':
                if self.expression_visitor.is_string_expr(stmt.value):
                    return f'{indent}setParamString({obj_name}, "{target.attr}", {value_str});\n'
                else:
                    return f'{indent}setParam({obj_name}, "{target.attr}", {value_str});\n'
            
            target_str = self.expression_visitor.visit(target)
            if self.expression_visitor.is_string_expr(stmt.value):
                 return f"{indent}strcopy({target_str}, sizeof({target_str}), {value_str});\n"

            return f"{indent}{target_str} = {value_str};\n"

        if isinstance(stmt, ast.AugAssign):
            value_str = self.expression_visitor.visit(stmt.value)
            op_map = {ast.Add: "+=", ast.Sub: "-="}
            op_str = op_map.get(type(stmt.op), "/*?*/")

            if isinstance(stmt.target, ast.Attribute) and isinstance(stmt.target.value, ast.Name) and stmt.target.value.id == 'self':
                attr_type = self.class_attributes.get(stmt.target.attr)
                if attr_type == 'str':
                    code = f"{indent}getParamString({obj_name}, \"{stmt.target.attr}\", buffer, sizeof(buffer));\n"
                    code += f"{indent}FormatEx(buffer, sizeof(buffer), \"%s%s\", buffer, {value_str});\n"
                    code += f"{indent}setParamString({obj_name}, \"{stmt.target.attr}\", buffer);\n"
                    return code
                else:
                    op_char = op_str.replace("=", "")
                    get_str = f'getParam({obj_name}, "{stmt.target.attr}")'
                    return f'{indent}setParam({obj_name}, "{stmt.target.attr}", {get_str} {op_char} {value_str});\n'

            target_str = self.expression_visitor.visit(stmt.target)
            return f"{indent}{target_str} {op_str} {value_str};\n"

        if isinstance(stmt, ast.If):
            test_str = self.expression_visitor.visit(stmt.test)
            
            if isinstance(stmt.test, ast.Compare) and isinstance(stmt.test.left, ast.Call) and isinstance(stmt.test.left.func, ast.Attribute) and stmt.test.left.func.attr == 'lower':
                left = self.expression_visitor.visit(stmt.test.left.func.value)
                right = self.expression_visitor.visit(stmt.test.comparators[0])
                op_type = type(stmt.test.ops[0])
                if op_type is ast.Eq:
                    test_str = f"StrEqual({left}, {right}, false)"
                elif op_type is ast.NotEq:
                    test_str = f"!StrEqual({left}, {right}, false)"

            code = f"{indent}if ({test_str}) {{\n"
            for s in stmt.body:
                code += self._convert_statement(s, indent + "    ", is_constructor, known_locals, obj_name)
            code += f"{indent}}}\n"
            if stmt.orelse:
                code += f"{indent}else {{\n"
                for s in stmt.orelse:
                    code += self._convert_statement(s, indent + "    ", is_constructor, known_locals, obj_name)
                code += f"{indent}}}\n"
            return code
        
        if isinstance(stmt, ast.For):
            target_str = self.expression_visitor.visit(stmt.target)
            iter_str = self.expression_visitor.visit(stmt.iter)
            code = f"{indent}for (int i = 0; i < sizeof({iter_str}); i++) {{\n"
            code += f"{indent}    any {target_str} = {iter_str}[i];\n"
            for s in stmt.body:
                code += self._convert_statement(s, indent + "    ", is_constructor, known_locals, obj_name)
            code += f"{indent}}}\n"
            return code

        if isinstance(stmt, ast.Continue):
            return f"{indent}continue;\n"

        return f"{indent}// Unsupported statement: {type(stmt).__name__}\n"

def build_class_database(source_dir: Path) -> dict:
    """遍历所有py文件，构建一个类名到 (AST节点, 文件路径) 的映射。"""
    class_db = {}
    for root, _, files in os.walk(source_dir):
        for file in files:
            if not file.endswith(".py"): continue
            path = Path(root) / file
            with open(path, "r", encoding="utf-8") as f:
                try:
                    tree = ast.parse(f.read(), filename=str(path))
                    for node in ast.walk(tree):
                        if isinstance(node, ast.ClassDef):
                            class_db[node.name] = (node, path)
                except Exception as e:
                    print(f"Error parsing {path}: {e}")
    return class_db

def generate_defines(tree: ast.Module, class_db: dict) -> str:
    """从AST的顶层提取常量赋值作为 #define。"""
    defines = ""
    visitor = PawnExpressionVisitor(None, class_db)
    for node in tree.body:
        if isinstance(node, ast.Assign):
            if len(node.targets) == 1 and isinstance(node.targets[0], ast.Name):
                var_name = node.targets[0].id
                if var_name.isupper():
                    value_str = visitor.visit(node.value)
                    defines += f"#define {var_name} {value_str}\n"
    return defines


def generate_toplevel_functions(tree: ast.Module, class_db: dict, directives: dict) -> str:
    """从AST的顶层提取函数定义并转换为SourcePawn。"""
    code = ""
    generator = PawnClassGenerator(class_db, directives)

    for node in tree.body:
        if isinstance(node, ast.FunctionDef):
            if node.name.startswith("__"):
                continue

            generator.local_string_vars.clear()
            signature = generator._generate_function_signature(node, is_method=False)
            generator.expression_visitor = PawnExpressionVisitor(None, generator.class_db, {}, generator.string_params, generator.local_string_vars, "")
            code += f"{signature} {{\n"
            
            # 检查是否需要临时缓冲区
            buffer_checker = BufferCheckVisitor({})
            buffer_checker.visit(node)
            if buffer_checker.requires_buffer:
                code += "    char buffer[256];\n"

            code += generator._generate_function_body(node, "    ", False)
            code += "}\n\n"
    return code


def find_toplevel_function_dependencies(tree: ast.Module, class_db: dict) -> set[str]:
    """从顶层函数中查找对其他类的依赖。"""
    dependencies = set()
    for func_node in tree.body:
        if not isinstance(func_node, ast.FunctionDef):
            continue
        
        for node in ast.walk(func_node):
            if isinstance(node, ast.Call) and isinstance(node.func, ast.Name):
                if node.func.id in class_db:
                    dependencies.add(node.func.id)
    return dependencies


def find_dependency_modules(tree: ast.Module) -> set[str]:
    """从AST中查找 from include.x import y 这样的导入，并返回模块名 x。"""
    modules = set()
    for node in ast.walk(tree):
        if isinstance(node, ast.ImportFrom) and node.module and node.module.startswith("include."):
            include_name = node.module.split('.', 1)[1]
            modules.add(include_name)
    return modules

def find_dependency_file(module_name: str, search_dir: Path) -> Path | None:
    """在指定目录中递归搜索 .inc 文件。"""
    for root, _, files in os.walk(search_dir):
        for file in files:
            if file == f"{module_name}.inc":
                return Path(root) / file
    return None

def main():
    if not PERKS_OUTPUT_DIR.exists():
        PERKS_OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    print("Building class database...")
    class_db = build_class_database(SOURCE_DIR)
    print(f"Found {len(class_db)} classes.")

    for root, _, files in os.walk(SOURCE_DIR):
        for file in files:
            if not file.endswith(".py") or file.startswith("__"):
                continue

            source_path = Path(root) / file
            print(f"Processing: {source_path}")

            try:
                with open(source_path, "r", encoding="utf-8") as f:
                    source_code = f.read()
                
                # 解析源代码和指令
                tree = ast.parse(source_code)
                directive_parser = PawnDirectiveParser(source_code)
                directives = directive_parser.directives

                # 实例化生成器并传入指令
                generator = PawnClassGenerator(class_db, directives)
                
                include_set = set()
                file_level_defines = generate_defines(tree, class_db)
                all_class_code = ""
                all_class_defines = ""
                toplevel_functions_code = generate_toplevel_functions(tree, class_db, directives)
                
                relative_current_py_path = source_path.relative_to(SOURCE_DIR)
                current_output_path = (PERKS_OUTPUT_DIR / relative_current_py_path).with_suffix(".inc")

                dependency_modules = find_dependency_modules(tree)
                for dep_module_name in dependency_modules:
                    dep_source_path = find_dependency_file(dep_module_name, SOURCEPAWN_DIR)
                    if dep_source_path:
                        relative_dep_path = dep_source_path.relative_to(SOURCEPAWN_DIR)
                        dep_output_path = OUTPUT_DIR / relative_dep_path
                        include_path = os.path.relpath(dep_output_path, current_output_path.parent)
                        include_path_str = str(Path(include_path)).replace("\\", "/")
                        include_set.add(f'"{include_path_str}"')
                    else:
                        print(f"  -> Warning: Dependency '{dep_module_name}.inc' not found in '{SOURCEPAWN_DIR}'.")

                toplevel_deps = find_toplevel_function_dependencies(tree, class_db)
                for dep_class_name in toplevel_deps:
                    if dep_class_name in class_db:
                        _, base_path = class_db[dep_class_name]
                        relative_base_py_path = base_path.relative_to(SOURCE_DIR)
                        base_output_path = (PERKS_OUTPUT_DIR / relative_base_py_path).with_suffix(".inc")
                        include_path = os.path.relpath(base_output_path, current_output_path.parent)
                        include_path_str = str(Path(include_path)).replace("\\", "/")
                        include_set.add(f'"{include_path_str}"')

                for node in ast.walk(tree):
                    if isinstance(node, ast.ClassDef):
                        _, base_path = generator._get_base_class_info(node)
                        if base_path:
                            relative_base_py_path = base_path.relative_to(SOURCE_DIR)
                            base_output_path = (PERKS_OUTPUT_DIR / relative_base_py_path).with_suffix(".inc")
                            include_path = os.path.relpath(base_output_path, current_output_path.parent)
                            include_path_str = str(Path(include_path)).replace("\\", "/")
                            include_set.add(f'"{include_path_str}"')
                        
                        class_defines, class_code = generator.generate_for_class(node.name)
                        all_class_defines += class_defines
                        all_class_code += class_code + "\n"

                # Add helpers for get/set params if any class is used
                if all_class_code or toplevel_deps:
                    include_set.add('<adt_trie>')
                    # Include a new common file for helpers instead of generating them
                    zf_util_base_path = OUTPUT_DIR / "zf_util_base.inc"
                    include_path = os.path.relpath(zf_util_base_path, current_output_path.parent)
                    include_path_str = str(Path(include_path)).replace("\\", "/")
                    include_set.add(f'"{include_path_str}"')


                if all_class_code or toplevel_functions_code:
                    output_path = current_output_path
                    output_path.parent.mkdir(parents=True, exist_ok=True)
                    
                    with open(output_path, "w", encoding="utf-8") as f:
                        f.write(f"// Generated from {source_path}\n\n")
                        f.write(f"#if defined __{relative_current_py_path.stem}_included\n")
                        f.write("#endinput\n")
                        f.write("#endif\n")
                        f.write(f"#define __{relative_current_py_path.stem}_included\n\n")
                        
                        if include_set:
                            for inc in sorted(list(include_set)):
                                f.write(f"#include {inc}\n")
                            f.write("\n")

                        if file_level_defines or all_class_defines:
                            if file_level_defines:
                                f.write(file_level_defines)
                            if all_class_defines:
                                f.write(all_class_defines)
                            f.write("\n")
                        
                        f.write(all_class_code)
                        if toplevel_functions_code:
                            f.write("\n")
                            f.write(toplevel_functions_code)
                    print(f"  -> Generated: {output_path}")

            except Exception as e:
                print(f"  -> Error processing {source_path}: {e}")
                import traceback
                traceback.print_exc()

if __name__ == "__main__":
    main()