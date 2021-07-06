import os
import re
import shutil
from merge import merge

with open('rtconfig.h', 'rt') as f:
    config = f.read()
    
def main():
    module = get_module()
    version = get_version(module)
    path = f'./dist/{module}/{version}'
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

    raw_fw_path = os.path.join(path, 'raw.bin')
    shutil.copyfile('rtthread.bin', raw_fw_path)
    merge(module, os.path.join(path, 'raw.bin'), os.path.join(path, 'merged.bin'))

    ota_fw_path = os.path.join(path, 'ota.bin')
    os.system(f'./scripts/rt_ota_packaging_tool_cli-x64 -f {raw_fw_path} -v {version} -p app -c gzip -o {ota_fw_path}')

def get_module():
    if get_define("USE_LOWER_END"):
        return 'lower'
    elif get_define("USE_UPPER_END"):
        return 'upper'
    else:
        raise ValueError()

def get_version(module: str):
    return get_define(f'{module.upper()}_VERSION')

def get_define(name, default = False):
    m = re.search(f'^#define\\s+{name}\\ *(.*?)$', config, re.MULTILINE)
    if m is None:
        return default
    result = m.group(1)
    if result == '':
        return True
    elif result[0] == '"' and result[-1] == '"':
        return result[1:-1]

    return result

if __name__ == '__main__':
    main()