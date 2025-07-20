import subprocess
import shutil
from pathlib import Path
import sys

# --- 配置 ---
TOOLS_DIR = Path(__file__).parent.resolve()
ROOT_DIR = TOOLS_DIR.parent
SOURCEPAWN_SOURCE_DIR = ROOT_DIR / "src" / "sourcepawn"
OUTPUT_DIR = Path("/home/gameserver/Team_Fortress_2_server/tf/addons/sourcemod/plugins")

def run_generator():
    """
    执行 generate_pawn_classes.py 脚本。
    """
    generator_script = TOOLS_DIR / "generate_pawn_classes.py"
    print("--- Running Python to SourcePawn generator ---")
    
    if not generator_script.exists():
        print(f"Error: Generator script not found at {generator_script}")
        return False
        
    try:
        # 使用 sys.executable 确保我们用的是同一个 Python 解释器
        result = subprocess.run(
            [sys.executable, str(generator_script)],
            check=True,
            capture_output=True,
            text=True,
            encoding='utf-8'
        )
        print(result.stdout)
        if result.stderr:
            print("Generator script stderr:")
            print(result.stderr)
        print("--- Generator finished successfully ---")
        return True
    except subprocess.CalledProcessError as e:
        print("--- Generator script failed ---")
        print(e.stdout)
        print(e.stderr)
        return False
    except FileNotFoundError:
        print(f"Error: Could not find '{sys.executable}'. Make sure Python is in your PATH.")
        return False



def compile_plugin():
    """
    编译 aombie_fortress_perk.sp 插件。
    """
    print("\n--- Compiling SourcePawn plugin ---")
    
    # --- 选择编译器 ---
    if sys.platform == "win32":
        compiler_name = "spcomp64.exe"
    else:
        compiler_name = "spcomp64"
    compiler_path = ROOT_DIR / "bin" / compiler_name

    if not compiler_path.exists():
        print(f"Error: Compiler not found at {compiler_path}")
        return False

    # --- 准备编译参数 ---
    source_file = SOURCEPAWN_SOURCE_DIR / "zombie_fortress_perk.sp"
    output_dir = ROOT_DIR / "target"
    output_dir.mkdir(exist_ok=True)
    
    output_file = OUTPUT_DIR / "zombie_fortress_perk.smx"

    # 编译命令
    command = [
        str(compiler_path),
        str(source_file),
        f"-o{output_file}",              # 输出文件
        "-w203"
    ]

    print(f"Compiler command: {' '.join(command)}")

    try:
        result = subprocess.run(
            command,
            check=True,
            capture_output=True,
            text=True,
            encoding='utf-8'
        )
        print(result.stdout)
        if result.stderr:
            print("Compiler stderr:")
            print(result.stderr)
        print(f"--- Compilation successful! Output at {output_file} ---")
        return True
    except subprocess.CalledProcessError as e:
        print("--- Compilation failed ---")
        print(e.stdout)
        print(e.stderr)
        return False

def main():
    """主构建函数。"""
    print("Starting build process...")

    if not run_generator():
        print("\nBuild failed during class generation.")
        sys.exit(1)
        
    if not compile_plugin():
        print("\nBuild failed during plugin compilation.")
        sys.exit(1)

    print("\nBuild process completed successfully!")

if __name__ == "__main__":
    main()