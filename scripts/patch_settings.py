import os

os.system(f"sed -i 's/\"cppStandard\": \"c++11\"/\"cppStandard\": \"c++17\"/' .vscode/c_cpp_properties.json")
os.system(f"sed -i 's#{os.getcwd()}#${{workspaceFolder}}#' .vscode/c_cpp_properties.json")
