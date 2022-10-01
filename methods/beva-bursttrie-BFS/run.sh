#!/usr/bin/env bash

executable_path=$1
cfg_path=$2
cachegrind_script="bash $3 $executable_path"
free_cache="echo 3 > /proc/sys/vm/drop_caches"

declare -a cfg_array

IFS=$'\n' read -d '' -r -a lines_cfg_file < ${cfg_path}

for i in "${lines_cfg_file[@]}"
do
    IFS='=' read -ra tokens <<< "$i"
	cfg_array+=(${tokens[1]})
done

edit_distance=${cfg_array[0]}
dataset=${cfg_array[1]}
queries_size=${cfg_array[2]}
recovery_mode=${cfg_array[3]}
qry_number_start=${cfg_array[4]}
qry_number_end=${cfg_array[5]}
size_type=${cfg_array[6]}
alg=${cfg_array[7]}
dataset_basepath=${cfg_array[8]}
query_basepath=${cfg_array[9]}
experiments_basepath=${cfg_array[10]}
index_level_threshold=${cfg_array[11]}
max_second_level_size=${cfg_array[12]}
is_server=${cfg_array[13]}
is_full_query_instrumentation=${cfg_array[14]}
has_relevant_queries=${cfg_array[15]}

on_recovery_mode=0

for dt in $(seq ${dataset} 6)
do
    if [[ "${dt}" != 1 ]]; then

        for st in $(seq ${size_type} 3)
        do

            for ed in $(seq ${edit_distance} 3)
            do

                for ilt in $(seq ${index_level_threshold} 2 20)
                do

                    for msls in $(seq ${max_second_level_size} 7)
                    do

                        if [[ "${recovery_mode}" == 1 ]] && [[ "${on_recovery_mode}" == 1 ]] ; then
                            recovery_mode=0
                            qry_number_start=0
                            on_recovery_mode=0
                        fi

                        echo -n > ${cfg_path} # Clear file

                        echo "edit_distance=${ed}" >> ${cfg_path}
                        echo "dataset=${dt}" >> ${cfg_path}
                        echo "queries_size=${queries_size}" >> ${cfg_path}
                        echo "recovery_mode=${recovery_mode}" >> ${cfg_path}
                        echo "qry_number_start=${qry_number_start}" >> ${cfg_path}
                        echo "qry_number_end=${qry_number_end}" >> ${cfg_path}
                        echo "size_type=${st}" >> ${cfg_path}
                        echo "alg=${alg}" >> ${cfg_path}
                        echo "dataset_basepath=${dataset_basepath}" >> ${cfg_path}
                        echo "query_basepath=${query_basepath}" >> ${cfg_path}
                        echo "experiments_basepath=${experiments_basepath}" >> ${cfg_path}
                        echo "index_level_threshold=${ilt}" >> ${cfg_path}
                        echo "max_second_level_size=${msls}" >> ${cfg_path}
                        echo "is_server=${is_server}" >> ${cfg_path}
                        echo "is_full_query_instrumentation=${is_full_query_instrumentation}" >> ${cfg_path}
                        echo "has_relevant_queries=${has_relevant_queries}" >> ${cfg_path}

                        echo "<<<<<<<<<< Start Run >>>>>>>>>>>"

                        if [ -z "$3" ]
                        then
                          ${executable_path}
                        else
                          index_type="dfs"
                          if [[ $executable_path == *"bfs"* ]]; then
                            index_type="bfs"
                          fi

                          cachegrind_filename="${experiments_basepath}cachegrid_dataset_${dt}_size_type_${st}_tau_${ed}_"
                          cachegrind_filename="${cachegrind_filename}index_level_threshold_${ilt}_"
                          cachegrind_filename="${cachegrind_filename}max_second_level_size_${msls}_index_type_${index_type}_"
                          cachegrind_filename="${cachegrind_filename}alg_${alg}.txt"

                          output=$($cachegrind_script 2>&1)

                          echo "${output}" > "${cachegrind_filename}"
                        fi

                        ${free_cache} # Free cache
                        if [[ "${dt}" != 4 ]]; then # Ignore DBLP dataset
                          [[ $? -eq 0 ]]  || exit 1 # break if fail
                        fi

                        echo "<<<<<<<<<< Stop Run >>>>>>>>>>>"

                        on_recovery_mode=${recovery_mode}

                    done

                    max_second_level_size=0

                done

                index_level_threshold=8

            done

            edit_distance=1
        done
        size_type=3
    fi
done
