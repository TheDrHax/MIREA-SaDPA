#!/bin/bash

$1 2>&1 | while read line; do

    if [ z"$line" == z'digraph G {' ] || [ z"$GRAPH" == z"true" ]; then
        GRAPH=true
        echo "$line" >> /tmp/graph.dot

        if [ z"$line" == z'}' ]; then
            GRAPH=false
            cat /tmp/graph.dot | dot -T png > /tmp/graph.png
            rm /tmp/graph.dot
        fi
    else
        echo "$line"
    fi
done
