import os
import re
from pathlib import Path
from collections import defaultdict

# --- Configuration ---
# The base directory of the project, assuming this script is in src/python/
BASE_DIR = Path(__file__).parent.parent
SOURCE_DIRS = [
    BASE_DIR / "include",
    BASE_DIR / "src/sourcepawn",
]
OUTPUT_DIR = BASE_DIR / "src/python/include"

SP_TO_PY_TYPE_MAP = {
    "float": "float", "int": "int", "bool": "bool", "char": "str",
    "void": "None", "any": "Any", "String": "str", "Handle": "Any",
    "Action": "Any", "Plugin": "Any", "Database": "Any", "DBDriver": "Any",
    "DBStatement": "Any", "DBResultSet": "Any", "ConVar": "Any", "Command": "Any",
    "Client": "int", "TFClassType": "int", "TFTeam": "int", "CEntity": "Any",
    "CBaseEntity": "Any", "Vector": "list[float]", "File": "Any", "Menu": "Any",
    "Panel": "Any", "Timer": "Any", "Event": "Any", "KeyValues": "Any",
    "Protobuf": "Any", "Function": "Callable",
}

PYTHON_KEYWORDS = {
    'and', 'as', 'assert', 'break', 'class', 'continue', 'def', 'del', 'elif', 'else', 'except',
    'False', 'finally', 'for', 'from', 'global', 'if', 'import', 'in', 'is', 'lambda', 'None',
    'nonlocal', 'not', 'or', 'pass', 'raise', 'return', 'True', 'try', 'while', 'with', 'yield'
}

# --- Regex Definitions ---
METHOD_FUNC_REGEX = re.compile(
    r"^\s*(?:public|native|stock|static|normal)\s+(?:native\s+)?(?P<return_type>[\w:\[\]]+)\s+(?P<name>\w+)\s*\((?P<params>.*?)\);",
    re.MULTILINE | re.DOTALL
)
GLOBAL_FUNC_REGEX = re.compile(
    r"^\s*(?:forward|public|native|stock|static|normal)\s+(?:native\s+)?(?P<return_type>[\w:\[\]]+)\s+(?P<name>\w+)\s*\((?P<params>.*?)\)\s*(?:;|\{)",
    re.MULTILINE | re.DOTALL
)
METHODMAP_REGEX = re.compile(
    r"methodmap\s+(?P<name>\w+)(?:\s*<\s*(?P<parent>\w+))?\s*__nullable__\s*\{(?P<content>.*?)\};",
    re.DOTALL
)
DESTRUCTOR_REGEX = re.compile(r"public\s+native\s+~(?P<name>\w+)\s*\((?P<params>.*?)\);", re.MULTILINE)
IMPORT_REGEX = re.compile(r'^\s*(?:#include|import)\s*(?:<|")(?P<path>[\w\d/._-]+)(?:>|")', re.MULTILINE)
TYPESET_REGEX = re.compile(r"typeset\s+(?P<name>\w+)(?!s*\{)") # Avoid matching block typesets
# Corrected regex to not require a semicolon and be non-greedy.
TYPESET_BLOCK_REGEX = re.compile(r"typeset\s+(?P<name>\w+)\s*\{(?P<content>.*?)\n\s*\}", re.DOTALL)
FUNC_SIGNATURE_REGEX = re.compile(r"function\s+(?P<return_type>[\w:\[\]]+)\s*\((?P<params>.*?)\);", re.MULTILINE)
ENUM_REGEX = re.compile(r"enum\s*(?P<name>\w*)\s*\{(?P<content>.*?)\}", re.DOTALL)
STRUCT_REGEX = re.compile(r"struct\s+(?P<name>\w+)\s*\{(?P<content>.*?)\};", re.DOTALL)
CONST_REGEX = re.compile(r"^\s*const\s+(?P<type>[\w:\[\]]+)\s+(?P<name>\w+)\s*=\s*(?P<value>.*?);", re.MULTILINE)
DEFINE_REGEX = re.compile(r"^\s*#define\s+([A-Z][A-Z0-9_]*)\s+(.*)", re.MULTILINE)
# More generic regex for global variables, matching arrays and struct instances.
GLOBAL_VAR_REGEX = re.compile(r"^\s*(?:public\s+)?(?:new|decl)?\s*(?:const\s+)?(?P<type>\w+)\s+(?P<name>\w+)(?:\[[^\]]*\])?\s*=\s*.*?(?:;|\{)", re.MULTILINE)
DOCSTRING_REGEX = re.compile(r"/\*\*.*?\*/", re.DOTALL)


def discover_types_and_map_files(all_files: list[Path]) -> tuple[set[str], dict[str, str]]:
    """First pass: Scans all files to discover all globally defined types and their source files."""
    discovered = set()
    type_to_file_map = {}
    
    for file_path in all_files:
        try:
            content = file_path.read_text(encoding='utf-8', errors='ignore')
            file_stem = file_path.stem
            
            for match in METHODMAP_REGEX.finditer(content):
                name = match.group('name')
                discovered.add(name)
                type_to_file_map[name] = file_stem
            for match in ENUM_REGEX.finditer(content):
                if name := match.group('name'):
                    discovered.add(name)
                    type_to_file_map[name] = file_stem
            for match in TYPESET_REGEX.finditer(content):
                name = match.group('name')
                discovered.add(name)
                type_to_file_map[name] = file_stem
            for match in TYPESET_BLOCK_REGEX.finditer(content): # Added this
                name = match.group('name')
                discovered.add(name)
                type_to_file_map[name] = file_stem
            for match in STRUCT_REGEX.finditer(content):
                name = match.group('name')
                discovered.add(name)
                type_to_file_map[name] = file_stem
        except Exception:
            pass
            
    return discovered, type_to_file_map

def convert_sp_type_to_py(sp_type: str, known_types: set, type_to_file_map: dict, current_file_stem: str, used_external_types: set) -> str:
    """Converts a SourcePawn type string to a Python type string."""
    sp_type = sp_type.strip()
    if not sp_type: return "Any"
    
    is_array = sp_type.endswith('[]')
    if is_array: sp_type = sp_type[:-2].strip()
    if ':' in sp_type: sp_type = sp_type.split(':')[-1]

    # Check if it's a known custom type from another file
    if sp_type in type_to_file_map and type_to_file_map[sp_type] != current_file_stem:
        used_external_types.add(type_to_file_map[sp_type])
        
    py_type = SP_TO_PY_TYPE_MAP.get(sp_type, sp_type if sp_type in known_types else "Any")

    if is_array:
        return 'str' if py_type == 'str' and sp_type == 'char' else f"list[{py_type}]"
    return py_type

def format_docstring(doc_block: str) -> str:
    """Cleans and formats a C-style docstring into a Python one."""
    if not doc_block: return ""
    doc_block = re.sub(r'^\s*/\*\*', '', doc_block)
    doc_block = re.sub(r'\*/\s*$', '', doc_block)
    lines = [re.sub(r'^\s*\*\s?', '', line) for line in doc_block.split('\n')]
    while lines and not lines[0].strip(): lines.pop(0)
    while lines and not lines[-1].strip(): lines.pop()
    
    if not lines: return ""
    
    # Escape triple quotes inside the docstring
    cleaned_lines = [lines[0].replace('"""', r'\"\"\"')]
    for line in lines[1:]:
        cleaned_lines.append(f"{line}") # Docstrings for type aliases aren't indented
        
    return "\n".join(cleaned_lines) if lines else ""

def _robust_split_params(params_str: str) -> list[str]:
    """Splits a parameter string by commas, respecting nesting of brackets."""
    if not params_str: return []
    parts, balance, current = [], 0, []
    for char in params_str:
        if char in '({[': balance += 1
        elif char in ')}]': balance -= 1
        if char == ',' and balance == 0:
            parts.append("".join(current).strip())
            current = []
        else:
            current.append(char)
    if current: parts.append("".join(current).strip())
    return [p for p in parts if p]

def parse_params(params_str: str, known_types: set, type_to_file_map: dict, current_file_stem: str, used_external_types: set, is_method: bool = False, for_callable: bool = False) -> str:
    """Parses a SourcePawn parameter string into a Python one."""
    params_str = params_str.replace('\n', ' ').replace('\r', '').strip()
    if not params_str or params_str.lower() == 'void':
        if for_callable: return ""
        return "self" if is_method else ""

    variadic = params_str.endswith('...')
    if variadic: params_str = params_str[:-3].strip().rstrip(',')
    param_parts = _robust_split_params(params_str)
    
    if not param_parts and not variadic:
        if for_callable: return ""
        return "self" if is_method else ""

    parsed_params, unnamed_counter = [], 0
    for part in param_parts:
        part = part.strip().replace("const ", "").replace("&", "")
        has_default = '=' in part
        main_part = part.split('=', 1)[0].strip()
        words = main_part.split()
        if not words: continue
        name, type_sp = "", ""
        last_word = words[-1]
        if array_match := re.match(r"^(\w+)(\[.*\])$", last_word):
            name, type_sp = array_match.group(1), " ".join(words[:-1]) + "[]"
        else:
            name, type_sp = last_word, " ".join(words[:-1])
        if not type_sp and name:
             type_sp, name = name, f"_{unnamed_counter}"
             unnamed_counter += 1
        
        if name in PYTHON_KEYWORDS:
            name = f"___{name}"

        py_type = convert_sp_type_to_py(type_sp, known_types, type_to_file_map, current_file_stem, used_external_types)
        parsed_params.append({"name": name, "type": py_type, "has_default": has_default})

    if for_callable:
        # For Callable[[...], ...], we only need the types
        param_types = [p['type'] for p in parsed_params]
        if variadic: param_types.append('...')
        return ", ".join(param_types)

    final_params = ["self"] if is_method else []
    has_seen_default = False
    for p in parsed_params:
        param_str = f"{p['name']}: {p['type']}"
        if p['has_default'] or has_seen_default:
            param_str += " = ..."
        if p['has_default']: has_seen_default = True
        final_params.append(param_str)
    if variadic: final_params.append('*args: Any')
    return ", ".join(final_params)

def parse_imports(content: str, used_external_types: set, current_file_stem: str) -> list[str]:
    """Parses #include and used types into Python wildcard imports."""
    imports = set()
    for match in IMPORT_REGEX.finditer(content):
        path_str = match.group('path')
        module_name = Path(path_str).stem
        if module_name and module_name != current_file_stem:
            imports.add(f"from .{module_name} import *")
    
    for module_name in used_external_types:
        if module_name != current_file_stem:
            imports.add(f"from .{module_name} import *")

    base_imports = ["from typing import Any, list, Callable, Union"]
    return base_imports + sorted(list(imports))

def get_closest_docstring(match_start: int, docstrings: dict[int, tuple[str, int]], original_content: str) -> str:
    """Finds the closest preceding docstring for a given match, ensuring no code is between them."""
    # Iterate through docstrings sorted by position, in reverse
    for pos in sorted(list(docstrings.keys()), reverse=True):
        if pos < match_start:
            _formatted_doc, doc_end = docstrings[pos]
            # Check the text between the docstring and the match
            intervening_text = original_content[doc_end:match_start]
            # If there is anything other than whitespace, this docstring belongs to that code.
            if not intervening_text.strip():
                return docstrings.pop(pos)[0]
            # This docstring belongs to something else, so we can stop searching.
            break
    return ""

def parse_inc_file(content: str, file_stem: str, global_known_types: set, type_to_file_map: dict) -> str:
    """Parses the content of a .inc file and returns Python stub code."""
    
    used_external_types = set()
    # Preserve offsets by replacing comments with spaces
    cleaned_content = re.sub(r'(?<!:)//.*', lambda m: ' ' * len(m.group(0)), content)
    cleaned_content = re.sub(r'/\*(?!\*).*?\*/', lambda m: ' ' * len(m.group(0)), cleaned_content, flags=re.DOTALL)
    
    # Store formatted docstring and original end position
    docstrings = {m.start(): (format_docstring(m.group(0)), m.end()) for m in DOCSTRING_REGEX.finditer(content)}
    
    # A list of (start, end) tuples for processed content
    processed_spans = []

    def mark_processed(match):
        processed_spans.append(match.span())
        return match

    local_known_types = global_known_types.union({m.group('name') for m in TYPESET_REGEX.finditer(cleaned_content)})

    parsed_constructs = defaultdict(lambda: {"type": "class", "parent": None, "doc": "", "members": [], "methods": []})
    global_elements = []

    # --- Pass 1: Aggregate data for classes, structs, enums, and typesets ---

    # Methodmaps
    for match in METHODMAP_REGEX.finditer(cleaned_content):
        mark_processed(match)
        name, parent, class_content = match.group("name", "parent", "content")
        construct = parsed_constructs[name]
        construct['type'] = 'class'
        if parent: construct['parent'] = parent
        construct['doc'] = get_closest_docstring(match.start(), docstrings, content)
        for d_match in DESTRUCTOR_REGEX.finditer(class_content):
            params_py = parse_params(d_match.group('params'), local_known_types, type_to_file_map, file_stem, used_external_types, is_method=False)
            construct['methods'].append(f"    def __del__(self{(', ' + params_py) if params_py else ''}) -> None:\n        pass")
        for m_match in METHOD_FUNC_REGEX.finditer(class_content):
            return_type_sp, method_name, params_sp = m_match.groups()
            return_type_py = convert_sp_type_to_py(return_type_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
            params_py = parse_params(params_sp, local_known_types, type_to_file_map, file_stem, used_external_types, is_method=True)
            doc = "" # Docstrings inside methodmaps are tricky, this is a simplification
            method_lines = [f"    def {method_name}({params_py}) -> {return_type_py}:"]
            if doc: method_lines.append(f'        """{doc}"""')
            method_lines.append("        pass")
            construct['methods'].append("\n".join(method_lines))

    # Structs
    for match in STRUCT_REGEX.finditer(cleaned_content):
        mark_processed(match)
        name, struct_content = match.group("name", "content")
        construct = parsed_constructs[name]
        construct['type'] = 'class'
        construct['doc'] = get_closest_docstring(match.start(), docstrings, content)
        for member_line in struct_content.strip().split(';'):
            # Remove inline doc comments like /**< ... */
            member_line = re.sub(r"/\*\*<.*?\*/", "", member_line).strip()
            member_line = member_line.strip().replace("public", "").replace("const", "").strip()
            if not member_line: continue
            words = member_line.split()
            if len(words) >= 2:
                member_name, type_sp = words[-1], " ".join(words[:-1])
                if '[]' in member_name: type_sp += '[]'; member_name = member_name.replace('[]', '')
                type_py = convert_sp_type_to_py(type_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
                construct['members'].append(f"    {member_name}: {type_py} = ...")
    
    # Enums
    for match in ENUM_REGEX.finditer(cleaned_content):
        mark_processed(match)
        name, enum_content = match.group("name", "content")
        items = [item.split('=')[0].strip() for item in re.sub(r"/\*\*.*?\*/", "", enum_content.split('}')[0], flags=re.DOTALL).split(',') if item.strip()]
        if not items: continue
        if not name:
            for item_name in items: global_elements.append(f"{item_name}: int = ...")
        else:
            construct = parsed_constructs[name]
            construct['type'] = 'class'
            if not construct['doc']: construct['doc'] = get_closest_docstring(match.start(), docstrings, content)
            for item_name in items: construct['members'].append(f"    {item_name}: int = ...")

    # Typeset Blocks (Function Signatures)
    for match in TYPESET_BLOCK_REGEX.finditer(cleaned_content):
        mark_processed(match)
        name, typeset_content = match.group("name", "content")
        
        # Get docstring immediately before the typeset block
        doc = get_closest_docstring(match.start(), docstrings, content)
        
        # Find all docstrings inside the typeset block content
        inner_doc_matches = DOCSTRING_REGEX.finditer(typeset_content)
        inner_docs = [format_docstring(m.group(0)) for m in inner_doc_matches]
        
        # Combine them
        all_docs = []
        if doc:
            all_docs.append(doc)
        if inner_docs:
            # We'll present each inner doc as a separate point for clarity
            all_docs.append("Accepts one of the following function signatures:\n- " + "\n- ".join(inner_docs))
        
        final_doc = "\n\n".join(all_docs)

        callable_signatures = []
        for sig_match in FUNC_SIGNATURE_REGEX.finditer(typeset_content):
            return_type_sp, params_sp = sig_match.groups()
            return_type_py = convert_sp_type_to_py(return_type_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
            param_types_py = parse_params(params_sp, local_known_types, type_to_file_map, file_stem, used_external_types, for_callable=True)
            callable_signatures.append(f"Callable[[{param_types_py}], {return_type_py}]")

        if not callable_signatures: continue

        if len(callable_signatures) == 1:
            type_alias = f"{name} = {callable_signatures[0]}"
        else:
            type_alias = f"{name} = Union[\n    " + ",\n    ".join(callable_signatures) + "\n]"
        
        full_alias_def = ""
        if final_doc:
            full_alias_def += f'"""{final_doc}"""\n'
        full_alias_def += type_alias
        global_elements.append(full_alias_def)

    # --- Pass 2: Process remaining global elements ---
    # Create a string with processed parts blanked out to avoid re-matching
    remaining_content = list(cleaned_content)
    processed_spans.sort()
    for start, end in processed_spans:
        for i in range(start, end):
            if i < len(remaining_content) and remaining_content[i] != '\n':
                remaining_content[i] = ' '
    remaining_content_str = "".join(remaining_content)

    for match in GLOBAL_FUNC_REGEX.finditer(remaining_content_str):
        return_type_sp, name, params_sp = match.groups()
        return_type_py = convert_sp_type_to_py(return_type_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
        params_py = parse_params(params_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
        doc = get_closest_docstring(match.start(), docstrings, content)
        func_lines = [f"def {name}({params_py}) -> {return_type_py}:"]
        if doc: func_lines.append(f'    """{doc}"""')
        func_lines.append("    pass")
        global_elements.append("\n".join(func_lines))

    for match in CONST_REGEX.finditer(remaining_content_str):
        type_sp, name, value = match.groups()
        py_type = convert_sp_type_to_py(type_sp, local_known_types, type_to_file_map, file_stem, used_external_types)
        global_elements.append(f"{name}: {py_type} = ...  # {value.strip()}")

    for match in DEFINE_REGEX.finditer(remaining_content_str):
        name, value = match.groups()
        value = re.sub(r'//.*', '', value).strip()
        global_elements.append(f"{name}: Any = ...  # {value}")
        
    for match in GLOBAL_VAR_REGEX.finditer(remaining_content_str):
        var_type, var_name = match.group("type", "name")
        py_type = convert_sp_type_to_py(var_type, local_known_types, type_to_file_map, file_stem, used_external_types)
        global_elements.append(f"{var_name}: {py_type} = ...")

    # --- Pass 3: Assemble the final output string ---
    output_lines = parse_imports(content, used_external_types, file_stem)
    output_lines.append("\n")

    for name, data in sorted(parsed_constructs.items()):
        parent_class = f"({data['parent']})" if data['parent'] else ""
        output_lines.append(f"class {name}{parent_class}:")
        class_body = []
        if data['doc']: class_body.append(f'    """{data["doc"]}"""')
        class_body.extend(sorted(data['members']))
        class_body.extend(sorted(data['methods']))
        if not class_body: output_lines.append("    pass")
        else: output_lines.extend(class_body)
        output_lines.append("\n")
        
    # Avoid adding duplicates
    final_global_elements = []
    processed_global_names = {c for c in parsed_constructs} # Start with class/struct names
    for ge_str in global_elements:
        # A bit of a hack to get the name of the defined element
        first_line = ge_str.strip().split('\n')[0]
        name = None
        if first_line.startswith('def '):
            name = first_line.split('def ')[1].split('(')[0]
        elif ' = ' in first_line:
            # Handles 'var: type = ...' and 'var = ...'
            name = first_line.split(' = ')[0].split(':')[0].strip()
        elif ':' in first_line: # Handles 'var: type'
            name = first_line.split(':')[0].strip()

        if name and name not in processed_global_names:
            final_global_elements.append(ge_str)
            processed_global_names.add(name)
        elif not name: # For things without a clear name (like multiline defs), just add them
             final_global_elements.append(ge_str)

    output_lines.extend(final_global_elements)
    return "\n".join(output_lines)

def main():
    """Main function to run the generation process."""
    if not OUTPUT_DIR.exists():
        OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    all_inc_files = [p for source_dir in SOURCE_DIRS if source_dir.exists() for p in source_dir.rglob("*.inc")]
    
    print("--- Pass 1: Discovering types and mapping to files ---")
    global_known_types, type_to_file_map = discover_types_and_map_files(all_inc_files)
    print(f"Discovered {len(global_known_types)} global types across {len(all_inc_files)} files.")
    
    print("\n--- Pass 2: Generating Python stubs ---")
    for inc_file in all_inc_files:
        try:
            output_file = OUTPUT_DIR / f"{inc_file.stem}.pyi"
            output_file.parent.mkdir(parents=True, exist_ok=True)
            print(f"Processing: {inc_file} -> {output_file}")
            
            content = inc_file.read_text(encoding="utf-8", errors="ignore")
            pyi_content = parse_inc_file(content, inc_file.stem, global_known_types, type_to_file_map)
            output_file.write_text(pyi_content, encoding="utf-8")
        except Exception as e:
            print(f"Error processing {inc_file}: {e}")
            import traceback
            traceback.print_exc()

    print("\nGeneration complete.")

if __name__ == "__main__":
    main()