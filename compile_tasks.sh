#!/bin/bash

echo "Compiling all tasks..."

for file in tasks/*.cpp tasks/*.c; do
    if [ -f "$file" ]; then
        filename=$(basename -- "$file")
        filename="${filename%.*}"

        if [ "$filename" == "processViewer" ]; then
            g++ "$file" kernel.cpp scheduler.cpp resource.cpp ready_queue.cpp -o "tasks/$filename" -I.. -pthread
        else
            g++ "$file" -o "tasks/$filename"
        fi

        echo "Compiled: $filename"
    fi
done

echo "Done! All tasks compiled."