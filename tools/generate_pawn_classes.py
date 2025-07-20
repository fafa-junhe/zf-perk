import ast
import os
from pathlib import Path
import sys
import re
from collections import OrderedDict

# ... (camel_to_snake, config, and map_type functions remain the same) ...
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
        return "int[]" # Simplification for now
    return TYPE_MAP.get(py_type, "any")


class PawnExpressionVisitor(ast.NodeVisitor):
    def __init__(self, class_db: dict, class_layout: OrderedDict | None = None, self_replacement: str | None = None):
        self.class_db = class_db
        self.self_replacement = self_replacement
        self.class_layout = class_layout if class_layout is not None else OrderedDict()
        # A list to store setup code (like buffer declarations and getter calls)
        self.pre_expression_code = []
    def _get_sp_type(self, attr_name: str) -> str | None:
        """Looks up the SourcePawn type of an attribute from the layout."""
        if attr_name in self.class_layout:
            info = self.class_layout[attr_name]
            if info['type'] == 'property':
                _, sp_type = PawnClassGenerator._get_type_info(info['node'].annotation)
                return sp_type
        return None
    def visit_Constant(self, node: ast.Constant):
        if node.value is None: return "null"
        if isinstance(node.value, str): return f'"{node.value}"'
        if isinstance(node.value, bool): return str(node.value).lower()
        return str(node.value)

    def visit_Name(self, node: ast.Name):
        if node.id == "self":
            return self.self_replacement if self.self_replacement else "this"
        return node.id

    def visit_Attribute(self, node: ast.Attribute):
        """
        *** UPDATED: Handles attributes that need getter calls. ***
        """
        # Check if this attribute access is part of an assignment target
        # A bit of a hack to determine context: ast.Store is for writing.
        is_write_context = isinstance(node.ctx, ast.Store)
        
        sp_type = self._get_sp_type(node.attr)

        # If it's a string/array, we need to handle it specially.
        if sp_type and (sp_type == 'char[]' or '[]' in sp_type):
            instance_str = self.visit(node.value)
            capitalized_name = node.attr[0].upper() + node.attr[1:]
            
            if is_write_context:
                # Return the setter call structure
                # The visit_Assign method will handle providing the value.
                return f"{instance_str}.set{capitalized_name}"
            else:
                # This is a read. We can't return a value directly.
                # We generate a buffer, call the getter, and return the buffer name.
                buffer_name = f"__buf_{node.attr}"
                getter_name = f"get{capitalized_name}"
                
                # Add setup code to the list
                self.pre_expression_code.append(f"char {buffer_name}[256];")
                self.pre_expression_code.append(f"{instance_str}.{getter_name}({buffer_name}, sizeof({buffer_name}));")
                
                # Return the name of the buffer that now holds the value
                return buffer_name
        
        # For simple properties (int, bool, float), direct access is fine.
        return f"{self.visit(node.value)}.{node.attr}"
    def visit_Call(self, node: ast.Call):
        # *** ENHANCED 'print' HANDLING ***
        if isinstance(node.func, ast.Name) and node.func.id == 'print':
            format_parts = []
            format_args = []
            for arg in node.args:
                if isinstance(arg, ast.Constant) and isinstance(arg.value, str):
                    format_parts.append(arg.value.replace("%", "%%"))
                else:
                    # Use a generic placeholder; FormatEx handles types.
                    format_parts.append("%s") 
                    format_args.append(self.visit(arg))
            
            format_string = '"' + " ".join(format_parts) + '"'
            if format_args:
                # The function body generator will add the 'buffer'
                return f'FormatEx(buffer, sizeof(buffer), {format_string}, {", ".join(format_args)}); PrintToServer(buffer)'
            else:
                return f'PrintToServer({format_string});'

        callee_str = self.visit(node.func)
        args = [self.visit(arg) for arg in node.args]

        if isinstance(node.func, ast.Name) and node.func.id in self.class_db:
            return f"new {callee_str}({', '.join(args)})"

        return f"{callee_str}({', '.join(args)})"

    # *** RESTORED EXPRESSION HANDLERS ***
    def visit_UnaryOp(self, node: ast.UnaryOp):
        op_map = {ast.USub: "-", ast.Not: "!"}
        op_str = op_map.get(type(node.op), "")
        operand_str = self.visit(node.operand)
        return f"{op_str}({operand_str})"

    def visit_Compare(self, node: ast.Compare):
        left = self.visit(node.left)
        op_map = {ast.Eq: "==", ast.NotEq: "!=", ast.Lt: "<", ast.LtE: "<=", ast.Gt: ">", ast.GtE: ">="}
        op_str = op_map.get(type(node.ops[0]), "/*?*/")
        right = self.visit(node.comparators[0])
        return f"({left} {op_str} {right})"

    def visit_BinOp(self, node: ast.BinOp):
        left_str = self.visit(node.left)
        right_str = self.visit(node.right)
        op_map = {ast.Add: "+", ast.Sub: "-", ast.Mult: "*", ast.Div: "/", ast.Mod: "%"}
        op_str = op_map.get(type(node.op), "/*?*/")
        return f"({left_str} {op_str} {right_str})"

    def visit_BoolOp(self, node: ast.BoolOp):
        op_map = {ast.And: "&&", ast.Or: "||"}
        op_str = op_map.get(type(node.op), "/*?*/")
        values_str = [f"({self.visit(v)})" for v in node.values]
        return f" {op_str} ".join(values_str)

    def generic_visit(self, node):
        return f"/* Unsupported expression: {type(node).__name__} */"


class PawnClassGenerator:
    """
    生成单个类的SourcePawn methodmap代码。
    采用 DataPack 支持方法重写和属性。
    """
    def __init__(self, class_db: dict):
        self.class_db = class_db
        self.expression_visitor = PawnExpressionVisitor(None, class_db)

    def generate_for_class(self, class_name: str) -> tuple[str, str]:
        class_node, _ = self.class_db[class_name]
        base_name, _ = self._get_base_class_info(class_node)

        # 1. *** NEW: Get the unified layout for both methods and properties ***
        class_layout = self._get_class_layout(class_name)

        # 2. Generate constructor to initialize the DataPack
        constructor_code = self._generate_constructor(class_name, class_layout)

        # 3. Generate method stubs and property accessors
        parent_layout = OrderedDict()
        if base_name != "DataPack":
            parent_layout = self._get_class_layout(base_name)
        
        members_code = ""
        for name, info in class_layout.items():
            if name in parent_layout:
                continue # Inherited, no need to generate stub/accessor again
            
            if info['type'] == 'method':
                members_code += self._generate_method_stub(info['node'], info['index'])
            elif info['type'] == 'property':
                members_code += self._generate_property_accessor(info['node'], info['index'])

        # 4. Assemble the methodmap
        methodmap_code = f"methodmap {class_name} < {base_name} {{\n"
        methodmap_code += constructor_code
        methodmap_code += members_code
        methodmap_code += "}\n"

        # 5. Generate global functions for methods defined in *this* class
        global_functions_code = ""
        for item in class_node.body:
            if isinstance(item, ast.FunctionDef) and not item.name.startswith("__"):
                global_functions_code += self._generate_global_function(class_name, item)

        return methodmap_code, global_functions_code

    def _get_base_class_info(self, class_node: ast.ClassDef) -> tuple[str, Path | None]:
        # ... (This function is unchanged) ...
        if class_node.bases:
            base_node = class_node.bases[0]
            if isinstance(base_node, ast.Name) and base_node.id in self.class_db:
                _, base_path = self.class_db[base_node.id]
                return base_node.id, base_path
        return "DataPack", None

    def _get_class_layout(self, class_name: str) -> OrderedDict:
        """
        *** NEW: The core logic for layout management. ***
        Recursively collects all members (properties and methods)
        and assigns a unique DataPack index to each.
        """
        class_node, _ = self.class_db[class_name]
        
        layout = OrderedDict()
        
        # First, inherit layout from parent
        base_name, _ = self._get_base_class_info(class_node)
        if base_name != "DataPack":
            layout = self._get_class_layout(base_name)

        # Then, add/overwrite with members from the current class
        for item in class_node.body:
            member_info = None
            if isinstance(item, ast.AnnAssign) and isinstance(item.target, ast.Name):
                member_info = {'type': 'property', 'node': item}
                name = item.target.id
            elif isinstance(item, ast.FunctionDef) and not item.name.startswith("__"):
                member_info = {'type': 'method', 'node': item, 'owner': class_name}
                name = item.name

            if member_info:
                if name not in layout:
                    # New member, assign a new index
                    new_index = len(layout)
                    layout[name] = {**member_info, 'index': new_index}
                else:
                    # Overwriting a member (e.g., a method), keep the index
                    # but update the node and owner info.
                    index = layout[name]['index']
                    layout[name] = {**member_info, 'index': index}
        
        return layout

    def _generate_constructor(self, class_name: str, layout: OrderedDict) -> str:
        """
        *** FINAL & CORRECTED VERSION ***
        Generates constructors that properly call the parent constructor (super().__init__).
        """
        class_node, _ = self.class_db[class_name]
        base_name, _ = self._get_base_class_info(class_node)
        init_node = next((item for item in class_node.body if isinstance(item, ast.FunctionDef) and item.name == "__init__"), None)

        # 1. Parse constructor signature from the current class's __init__
        init_args_list = []
        if init_node:
            for arg in init_node.args.args:
                if arg.arg == "self": continue
                _, sp_type = self._get_type_info(arg.annotation)
                init_args_list.append(f"{sp_type} {arg.arg}")
        args_str = ", ".join(init_args_list)
        code = f"    public {class_name}({args_str}) {{\n"

        # 2. *** THE CORE IMPROVEMENT: Call the parent constructor ***
        super_call_args = ""
        if init_node:
            # Find the super().__init__(...) call to extract arguments for the parent constructor
            super_call_node = next((stmt.value for stmt in init_node.body if
                                    isinstance(stmt, ast.Expr) and isinstance(stmt.value, ast.Call) and
                                    isinstance(stmt.value.func, ast.Attribute) and stmt.value.func.attr == '__init__' and
                                    isinstance(stmt.value.func.value, ast.Call) and
                                    isinstance(stmt.value.func.value.func, ast.Name) and stmt.value.func.value.func.id == 'super'), None)
            if super_call_node:
                visitor = PawnExpressionVisitor(None, self.class_db)
                super_call_args = ", ".join([visitor.visit(arg) for arg in super_call_node.args])

        if base_name == "DataPack":
            # If there is no user-defined parent, create a fresh DataPack
            code += f"        DataPack sm_base = new DataPack();\n"
        else:
            # Call the parent's constructor with the extracted arguments
            code += f"        {base_name} sm_base = new {base_name}({super_call_args});\n"
        
        # We now have a base object. Cast it to its actual (more specific) type.
        code += f"        {class_name} sm = view_as<{class_name}>(sm_base);\n\n"

        # 3. *** Apply this class's specific modifications (overrides and new members) ***
        visitor = PawnExpressionVisitor(None, self.class_db)
        if init_node:
            # Process assignments like 'self.test = "okkk"'
            for stmt in init_node.body:
                if isinstance(stmt, ast.Assign) and len(stmt.targets) == 1 and \
                isinstance(stmt.targets[0], ast.Attribute) and \
                isinstance(stmt.targets[0].value, ast.Name) and stmt.targets[0].value.id == 'self':
                    
                    attr_name = stmt.targets[0].attr
                    # Find the layout info for this attribute to get its index and type
                    if attr_name in layout:
                        info = layout[attr_name]
                        index = info['index']
                        value_str = visitor.visit(stmt.value)
                        
                        if info['type'] == 'property':
                            _, sp_type = self._get_type_info(info['node'].annotation)
                            code += f"        sm.Position = DP({index});\n"
                            if sp_type == "int" or sp_type == "bool": code += f"        sm.WriteCell({value_str});\n"
                            elif sp_type == "float": code += f"        sm.WriteFloat({value_str});\n"
                            elif sp_type == "char[]": code += f"        sm.WriteString({value_str});\n"
                            else: code += f"        sm.WriteCell({value_str});\n"

        # Override any methods defined in this class
        for name, info in layout.items():
            if info['type'] == 'method' and info.get('owner') == class_name:
                index = info['index']
                func_name = f"{class_name}_{name}"
                code += f"        sm.Position = DP({index});\n"
                code += f"        sm.WriteFunction({func_name});\n"

        code += f"\n        return sm;\n"
        code += f"    }}\n"
        return code

    def _generate_property_accessor(self, prop_node: ast.AnnAssign, index: int) -> str:
        """
        *** NEW: Generates 'property' blocks or Getter/Setter pairs. ***
        """
        prop_name = prop_node.target.id
        py_type, sp_type = self._get_type_info(prop_node.annotation)

        # For strings and arrays, use Getter/Setter methods instead of property syntax
        if sp_type == "char[]" or "[]" in sp_type and sp_type != "char[]":
            capitalized_name = prop_name[0].upper() + prop_name[1:]
            getter = f"get{capitalized_name}"
            setter = f"set{capitalized_name}"
            
            code = ""
            # Getter
            code += f"\n    public void {getter}({sp_type} buffer, int maxlen) {{\n"
            code += f"        this.Position = DP({index});\n"
            if sp_type == "char[]":
                 code += f"        this.ReadString(buffer, maxlen);\n"
            else: # Array
                 code += f"        this.ReadCellArray(buffer, maxlen);\n" # Assuming cell array
            code +=  "    }\n"
            # Setter
            code += f"    public void {setter}(const {sp_type} value) {{\n"
            code += f"        this.Position = DP({index});\n"
            if sp_type == "char[]":
                code += f"        this.WriteString(value);\n"
            else: # Array
                code += f"        this.WriteCellArray(value, sizeof(value));\n" # Assuming cell array
            code +=  "    }\n"
            return code

        # For simple types, use the 'property' syntax
        code = f"\n    property {sp_type} {prop_name} {{\n"
        # Getter
        code += "        public get() {\n"
        code += f"            this.Position = DP({index});\n"
        if sp_type == "int" or sp_type == "bool" or sp_type == "any":
            code += "            return this.ReadCell();\n"
        elif sp_type == "float":
            code += "            return this.ReadFloat();\n"
        code += "        }\n"
        # Setter
        code += f"        public set({sp_type} value) {{\n"
        code += f"            this.Position = DP({index});\n"
        if sp_type == "int" or sp_type == "bool" or sp_type == "any":
            code += "            this.WriteCell(value);\n"
        elif sp_type == "float":
            code += "            this.WriteFloat(value);\n"
        code += "        }\n"
        code += "    }\n"
        return code
    
    @staticmethod # <--- FIX 1: Add the staticmethod decorator
    def _get_type_info(annotation_node: ast.AST | None) -> tuple[str, str]:
        """Helper to get python type string and mapped SourcePawn type."""
        py_type_str = "any"
        if annotation_node:
            try:
                py_type_str = ast.unparse(annotation_node)
            except AttributeError: # Fallback for older python
                if isinstance(annotation_node, ast.Name):
                    py_type_str = annotation_node.id
        
        sp_type = map_type(py_type_str)
        return py_type_str, sp_type

    def _generate_method_stub(self, func_node: ast.FunctionDef, index: int) -> str:
        """为 methodmap 生成方法存根（包装器）"""
        return_type, args_list, _ = self._parse_function_signature(func_node)
        args_str = ", ".join(f"{atype} {aname}" for aname, atype in args_list.items())
        
        code = f"\n    public {return_type} {func_node.name}({args_str}) {{\n"
        if return_type != "void":
            code += f"        {return_type} __retval;\n"
        
        code += f"        this.Position = DP({index});\n"
        code += f"        Function func = this.ReadFunction();\n"
        code += f"        this.Reset();\n"
        
        code += f"        Call_StartFunction(INVALID_HANDLE, func);\n"
        
        # Push arguments to the call
        for arg_name, arg_type in args_list.items():
            if arg_type == "int" or arg_type == "bool" or arg_type == "any":
                code += f"        Call_PushCell({arg_name});\n"
            elif arg_type == "float":
                code += f"        Call_PushFloat({arg_name});\n"
            elif arg_type == "char[]":
                code += f"        Call_PushString({arg_name});\n"
            else: # Arrays
                code += f"        Call_PushArray({arg_name}, sizeof({arg_name}));\n"

        if return_type != "void":
             code += f"        Call_Finish(__retval);\n"
             code += f"        return __retval;\n"
        else:
             code += f"        Call_Finish();\n"

        code += f"    }}\n"
        return code
    
    def _generate_global_function(self, class_name: str, func_node: ast.FunctionDef) -> str:
        """
        *** UPDATED: Generates global functions with a safe instance parameter name. ***
        """
        return_type, args_list, has_self = self._parse_function_signature(func_node)
        
        # *** KEY CHANGE: Use a safe parameter name instead of 'this' ***
        instance_param_name = "_inst"
        
        global_args = []
        if has_self:
            # The first parameter is the object instance itself
            global_args.append(f"{class_name} {instance_param_name}")
        
        global_args.extend([f"{atype} {aname}" for aname, atype in args_list.items()])
        
        signature = f"public {return_type} {class_name}_{func_node.name}({', '.join(global_args)})"
        
        code = f"{signature} {{\n"
        
        # *** NEW: DOCSTRING HANDLING LOGIC ***
        docstring = ast.get_docstring(func_node, clean=True)
        if docstring:
            # Format the docstring into a Javadoc-style comment block
            comment_lines = docstring.split('\n')
            code += f"    /**\n"
            for line in comment_lines:
                code += f"     * {line.strip()}\n"
            code += f"     */\n"
        
        # Buffer for FormatEx/print, if needed
        if any(isinstance(node, ast.Expr) and isinstance(node.value, ast.Call) and isinstance(node.value.func, ast.Name) and node.value.func.id == 'print' for node in ast.walk(func_node)):
            code += "    char buffer[256];\n"

        # *** NEW: GET THE BODY STATEMENTS, SKIPPING THE DOCSTRING ***
        body_stmts = func_node.body
        if ast.get_docstring(func_node, clean=False): # Check if a docstring node exists
            # The docstring is always the first node in the body if it exists
            body_stmts = func_node.body[1:]

        # Create a visitor with the correct context for this function
        class_layout = self._get_class_layout(class_name)
        self.expression_visitor = PawnExpressionVisitor(self.class_db, class_layout, self_replacement=instance_param_name)
        
        known_locals = set(args_list.keys())
        code += self._convert_function_body(body_stmts, "    ", known_locals)
        code += "}\n\n"
        return code
    def _convert_function_body(self, body: list, indent: str, known_locals: set) -> str:
        """Helper to convert a list of statements (a function body)."""
        code = ""
        for stmt in body:
            code += self._convert_statement(stmt, indent, known_locals)
        return code
    def _parse_function_signature(self, node: ast.FunctionDef) -> tuple[str, dict, bool]:
        """解析函数签名，返回 (返回类型, 参数字典, 是否有self)"""
        # 1. Return Type
        return_type = "void"
        if node.returns:
            try:
                py_type_str = ast.unparse(node.returns)
                return_type = map_type(py_type_str)
            except AttributeError:
                 if isinstance(node.returns, ast.Name):
                    return_type = map_type(node.returns.id)
        
        # 2. Arguments
        args_list = OrderedDict()
        has_self = False
        for arg in node.args.args:
            if arg.arg == "self":
                has_self = True
                continue
            
            arg_type = "any"
            if arg.annotation:
                try:
                    py_type_str = ast.unparse(arg.annotation)
                    arg_type = map_type(py_type_str)
                except AttributeError:
                    if isinstance(arg.annotation, ast.Name):
                        arg_type = map_type(arg.annotation.id)
            
            args_list[arg.arg] = arg_type
        
        return return_type, args_list, has_self

    def _convert_statement(self, stmt: ast.stmt, indent="    ", known_locals: set = None) -> str:
        if known_locals is None:
            known_locals = set()

        if isinstance(stmt, ast.Return):
            if stmt.value:
                value_str = self.expression_visitor.visit(stmt.value)
                return f"{indent}return {value_str};\n"
            return f"{indent}return;\n"
        
        if isinstance(stmt, ast.Expr):
            value_str = self.expression_visitor.visit(stmt.value)
            return f"{indent}{value_str};\n"

        if isinstance(stmt, ast.Pass):
            return ""
        if isinstance(stmt, ast.Expr) and isinstance(stmt.value, ast.Constant) and isinstance(stmt.value.value, str):
            return ""  # Docstring
        
        if isinstance(stmt, ast.Assign):
            # 变量声明已在函数开头处理
            target_str = self.expression_visitor.visit(stmt.targets[0])
            value_str = self.expression_visitor.visit(stmt.value)
            if isinstance(stmt.targets[0], ast.Name) and stmt.targets[0].id not in known_locals:
                # 这是一个新的局部变量声明
                known_locals.add(stmt.targets[0].id)
                # 简单的类型推断
                var_type = "any"
                if self.expression_visitor.is_string_expr(stmt.value):
                     var_type = "char[256]"
                return f"{indent}{var_type} {target_str} = {value_str};\n"
            return f"{indent}{target_str} = {value_str};\n"

        if isinstance(stmt, ast.AugAssign):
            target_str = self.expression_visitor.visit(stmt.target)
            value_str = self.expression_visitor.visit(stmt.value)
            op_map = {ast.Add: "+=", ast.Sub: "-="}
            op_str = op_map.get(type(stmt.op), "/*?*/")
            return f"{indent}{target_str} {op_str} {value_str};\n"

        if isinstance(stmt, ast.If):
            test_str = self.expression_visitor.visit(stmt.test)
            code = f"{indent}if ({test_str}) {{\n"
            for s in stmt.body:
                code += self._convert_statement(s, indent + "    ", known_locals)
            code += f"{indent}}}\n"
            if stmt.orelse:
                code += f"{indent}else {{\n"
                for s in stmt.orelse:
                    code += self._convert_statement(s, indent + "    ", known_locals)
                code += f"{indent}}}\n"
            return code
        
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

# (The functions below this point are toplevel helpers and the main execution loop)
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

    generator = PawnClassGenerator(class_db)

    for root, _, files in os.walk(SOURCE_DIR):
        for file in files:
            if not file.endswith(".py") or file.startswith("__"):
                continue

            source_path = Path(root) / file
            print(f"Processing: {source_path}")

            try:
                with open(source_path, "r", encoding="utf-8") as f:
                    tree = ast.parse(f.read())
                
                include_set = set()
                all_methodmaps_code = ""
                all_globals_code = ""
                
                relative_current_py_path = source_path.relative_to(SOURCE_DIR)
                current_output_path = (PERKS_OUTPUT_DIR / relative_current_py_path).with_suffix(".inc")

                # --- Dependency resolution ---
                dependency_modules = find_dependency_modules(tree)
                for dep_module_name in dependency_modules:
                    dep_source_path = find_dependency_file(dep_module_name, SOURCEPAWN_DIR)
                    if dep_source_path:
                        relative_dep_path = dep_source_path.relative_to(SOURCEPAWN_DIR)
                        dep_output_path = OUTPUT_DIR / relative_dep_path
                        include_path = os.path.relpath(dep_output_path, current_output_path.parent)
                        include_path_str = str(Path(include_path)).replace("\\", "/")
                        include_set.add(f'"{include_path_str}"')

                for node in ast.walk(tree):
                    if isinstance(node, ast.ClassDef):
                        _, base_path = generator._get_base_class_info(node)
                        if base_path:
                            # *** THE FIX IS HERE ***
                            # Only include if the dependency is in a DIFFERENT file.
                            if base_path.resolve() != source_path.resolve():
                                relative_base_py_path = base_path.relative_to(SOURCE_DIR)
                                base_output_path = (PERKS_OUTPUT_DIR / relative_base_py_path).with_suffix(".inc")
                                include_path = os.path.relpath(base_output_path, current_output_path.parent)
                                include_path_str = str(Path(include_path)).replace("\\", "/")
                                include_set.add(f'"{include_path_str}"')
                        
                        methodmap_code, globals_code = generator.generate_for_class(node.name)
                        all_methodmaps_code += methodmap_code + "\n"
                        all_globals_code += globals_code

                # Always include datapack for the new structure
                include_set.add('<datapack>')

                if all_methodmaps_code or all_globals_code:
                    output_path = current_output_path
                    output_path.parent.mkdir(parents=True, exist_ok=True)
                    
                    with open(output_path, "w", encoding="utf-8") as f:
                        f.write(f"// Generated from {source_path}\n\n")
                        f.write(f"#if defined __{relative_current_py_path.stem}_included\n")
                        f.write("#endinput\n")
                        f.write("#endif\n")
                        f.write(f"#define __{relative_current_py_path.stem}_included\n\n")
                        
                        # Add the user-provided macros
                        f.write("#define DP(%1) view_as<DataPackPos>(%1)\n")

                        if include_set:
                            for inc in sorted(list(include_set)):
                                f.write(f"#include {inc}\n")
                            f.write("\n")
                        
                        f.write(all_methodmaps_code)
                        if all_globals_code:
                            f.write("\n")
                            f.write(all_globals_code)
                    print(f"  -> Generated: {output_path}")

            except Exception as e:
                print(f"  -> Error processing {source_path}: {e}")
                import traceback
                traceback.print_exc()

if __name__ == "__main__":
    main()