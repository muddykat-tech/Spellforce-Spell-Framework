import subprocess
import os

# Specify file paths
input_file = "./conuserinit_template.lua"
output_file = "./assets/script/conuserinit.lua"

try:
    result = subprocess.run(
        ["python", "luaconverter.py", "-f", "../bin/sfsf.asi"],
        check=True,
        text=True,
        capture_output=True
    )
    print("Command executed successfully!")
    print("Output:", result.stdout)
except subprocess.CalledProcessError as e:
    print("Error while executing the command:")
    print("Return code:", e.returncode)
    print("Error message:", e.stderr)

replacement_file = "../bin/sfsf_result.txt"

with open(replacement_file, "r") as file:
    replacement_string = file.read().strip()

search_string = "[WORKSHOP]"

with open(input_file, "r") as file:
    file_data = file.read()

modified_data = file_data.replace(search_string, replacement_string)

with open(output_file, "w") as file:
    file.write(modified_data)

print(f"Modified file saved as {output_file}")

tool_directory = "C:/Program Files (x86)/Steam/steamapps/common/Spellforce Platinum Edition/modding/Filepacker" 
tool_name = "tool_filepacker.exe"
previous_working_directory = os.getcwd()
assets_path = os.path.join(previous_working_directory, "assets")
arguments = ["-notable"]

tool_path = os.path.join(tool_directory, tool_name)
if not os.path.isfile(tool_path):
    raise FileNotFoundError(f"The File Packer was not found at the specified path: {tool_path}")

os.chdir(tool_directory)

try:
    result = subprocess.run(
        ["./" + tool_name, assets_path] + arguments,
        check=True, 
        text=True,
        capture_output=True 
    )
    print("Command executed successfully!")
    print("Output:", result.stdout)
except subprocess.CalledProcessError as e:
    print("Error occurred while executing the command:")
    print("Return code:", e.returncode)
    print("Error message:", e.stderr)
finally:
    os.chdir(previous_working_directory)