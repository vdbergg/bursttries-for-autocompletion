#!/usr/bin/env bash

executable_path=$1
cfg_path=$2
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
is_server=${cfg_array[11]}
is_full_query_instrumentation=${cfg_array[12]}
has_relevant_queries=${cfg_array[13]}

on_recovery_mode=0

for dt in $(seq ${dataset} 6)
do
    if [[ "${dt}" != 1 ]] && [[ "${dt}" != 3 ]]; then # No have memory sufficient to experiment MEDLINE datasets

        for st in $(seq ${size_type} 3)
        do

            for ed in $(seq ${edit_distance} 3)
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
                echo "is_server=${is_server}" >> ${cfg_path}
                echo "is_full_query_instrumentation=${is_full_query_instrumentation}" >> ${cfg_path}
                echo "has_relevant_queries=${has_relevant_queries}" >> ${cfg_path}

                echo "<<<<<<<<<< Start Run >>>>>>>>>>>"

                ${executable_path}
                ${free_cache} # Free cache
                [[ $? -eq 0 ]] || exit 1 # break if fail

                echo "<<<<<<<<<< Stop Run >>>>>>>>>>>"

                on_recovery_mode=${recovery_mode}

            done

	          edit_distance=1
        done
        size_type=3
    fi
done
