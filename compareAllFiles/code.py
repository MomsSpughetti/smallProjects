import filecmp
import os
from difflib import Differ


def compare_directories(dir1, dir2):
    """Compares files in two directories recursively, identifying and diffing those with the same name but different contents.

    Args:
        dir1 (str): Path to the first directory.
        dir2 (str): Path to the second directory.
    """

    differences = []  # List to store information about differing files

    if not os.path.exists(dir1):
        print(f"Error: Directory '{dir1}' does not exist.")
        return  # Exit the function if dir1 is not found

    if not os.path.exists(dir2):
        print(f"Error: Directory '{dir2}' does not exist.")
        return  # Exit the function if dir2 is not found

    for name, dirs, files in os.walk(dir1):
        for filename in files:
            full_path1 = os.path.join(name, filename)
            # Check if the file also exists in dir2 with the same name
            full_path2 = os.path.join(dir2, os.path.relpath(full_path1, dir1))
            print(full_path1, " : ", full_path2)
            if not os.path.exists(full_path2):
                continue  # Skip if the corresponding file doesn't exist in dir2

            if not filecmp.cmp(full_path1, full_path2, shallow=False):  # Deep comparison
                differences.append((full_path1, full_path2))
                diff_file_path = os.path.join(os.path.dirname(full_path1), "../" f"diff_{filename}")
                with open(full_path1, 'r') as f1, open(full_path2, 'r') as f2, open(diff_file_path, 'w') as diff_file:
                    differ = Differ()
                    lines1 = f1.readlines()
                    lines2 = f2.readlines()
                    diffs = differ.compare(lines1, lines2)
                    diff_file.write("Differences:\n")
                    line_num = 0
                    for diff_line in diffs:
                        if diff_line.startswith('+') or diff_line.startswith('-'):
                            if diff_line.startswith('-') and line_num < len(lines1) and lines1[line_num].strip():
                                diff_file.write(f"Line {line_num + 1} in {full_path1}: {lines1[line_num].strip()}" + "\n")
                            if diff_line.startswith('+') and line_num < len(lines2):
                                diff_file.write(f"Line {line_num + 1} in {full_path2}: {lines2[line_num].strip()}" + "\n")
                        if not diff_line.startswith('?') and not diff_line.startswith('-'):  # Skip diff indicator lines
                            line_num += 1
                    print(f"Differences found between {full_path1} and {full_path2}. Diff written to {diff_file_path}")

    if not differences:
        print("No differences found between files in the directories.")


# Example usage
dir1 = "dir1"
dir2 = "dir2"
compare_directories(dir1, dir2)
