##
## EPITECH PROJECT, 2025
## Corewar
## File description:
## auto_tester.sh
##

#!/bin/bash

champions_dir="champions"
outputs_dir="outputs_tester"
champions=($(ls $champions_dir/*.cor))
total_champions=${#champions[@]}

mkdir -p $outputs_dir

print_colored() {
    local result=$1
    local champ1=$2
    local champ2=$3
    
    if [ "$result" == "OK" ]; then
        echo -e "$champ1 $champ2 : \033[1;32mOK\033[0m"
    else
        echo -e "$champ1 $champ2 : \033[1;31mKO\033[0m"
    fi
}

for ((i=0; i<$total_champions; i++)); do
    for ((j=i+1; j<$total_champions; j++)); do
        champ1=$(basename ${champions[$i]})
        champ2=$(basename ${champions[$j]})
                
        champ1_name=${champ1%.cor}
        champ2_name=${champ2%.cor}
        attempt_file="$outputs_dir/${champ1_name}_${champ2_name}_attempt.txt"
        output_file="$outputs_dir/${champ1_name}_${champ2_name}_output.txt"
        output_log_file="$outputs_dir/${champ1_name}_${champ2_name}_log_output.txt"

        ./corewar $champions_dir/$champ1 $champions_dir/$champ2 2> "$output_log_file" | tee file_all.txt > "$output_file"
        
        if diff -q "$attempt_file" "$output_file" >/dev/null; then
            print_colored "OK" "$champ1" "$champ2"
        else
            print_colored "KO" "$champ1" "$champ2"
        fi
    done
    echo "---------------------------"
done