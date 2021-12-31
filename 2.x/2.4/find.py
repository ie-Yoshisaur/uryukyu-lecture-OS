import argparse
import re
from pathlib import Path

def find(path_to_execute_find):
    all_files = list(map(str, Path(path_to_execute_find).rglob('*.*')))
    regex_pattern = re.compile(r'.*\.(c|java|py|go)$')
    filtered_files = list(filter(regex_pattern.match, all_files))

    for file in filtered_files:
        print(file)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("path_to_execute_find")
    args = parser.parse_args()
    path_to_execute_find = args.path_to_execute_find
    find(path_to_execute_find)
