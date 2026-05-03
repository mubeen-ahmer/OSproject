#!/bin/bash

echo "Compiling all tasks..."

# Loop through all .cpp and .c files in the tasks directory
for file in tasks/*.cpp tasks/*.c; do
    # Check if the file actually exists (in case there are no .c or .cpp files)
    if [ -f "$file" ]; then
        # Get the filename without the path and extension
        filename=$(basename -- "$file")
        filename="${filename%.*}"
        
        # Compile it
        g++ "$file" -o "tasks/$filename"
        echo "Compiled: $filename"
    fi
done

echo "Done! All tasks compiled."

# chmod +x compile_tasks.sh
# ./compile_tasks.sh