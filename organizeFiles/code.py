import os
import shutil
import sys

def separate_files_by_type(src_dir):
    for file in os.listdir(src_dir):
        if os.path.isfile(os.path.join(src_dir, file)):
            if '.' in file:
                file_type = file.split('.')[-1]  # Get the file extension
            else:
                file_type = 'no_extension'
            dest_dir = os.path.join(src_dir, file_type)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
            shutil.move(os.path.join(src_dir, file), os.path.join(dest_dir, file))

# Check if the directory path is provided as an argument
if len(sys.argv) != 2:
    print("Usage: python script.py <directory>")
    sys.exit(1)

src_dir = sys.argv[1]

# Separate files in the specified directory into different directories by type
separate_files_by_type(src_dir)
