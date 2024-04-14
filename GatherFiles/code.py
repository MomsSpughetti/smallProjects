import os
import shutil
import sys
def copy_all_files(src_dir, dest_dir):
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            src_file_path = os.path.join(root, file)
            dest_file_path = os.path.join(dest_dir, file)
            try:
                shutil.copy(src_file_path, dest_file_path)
            except shutil.Error as e:
                print(f"Error copying file {src_file_path}: {e}")

def copy_all_files_with_dir_in_name(src_dir, dest_dir):
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            src_file_path = os.path.join(root, file)
            # Get the parent directory name
            parent_dir_name = os.path.basename(os.path.normpath(root))
            # Prepend the directory name to the file name
            dest_file_name = f"{parent_dir_name}_{file}"
            dest_file_path = os.path.join(dest_dir, dest_file_name)
            try:
                shutil.copy(src_file_path, dest_file_path)
            except shutil.Error as e:
                print(f"Error copying file {src_file_path}: {e}")

if len(sys.argv) != 2:
    print("Usage: python script.py <directory>")
    sys.exit(1)

src_dir = sys.argv[1]

# Copy all files from the current directory to a new directory named 'allFiles'
copy_all_files_with_dir_in_name(src_dir, f'../allFiles')
print(f"copied files to dir: ../{src_dir}")
