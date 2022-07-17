#!/bin/bash

write_to_all_cc=""
write_to_all_h=""

while getopts ":ch:" opt; do
  case $opt in
    h)
      shift
      echo "-h was triggered, Parameter: $1" >&2
      write_to_all_h=$1
      shift
      ;;
    c)
      shift
      echo "-c was triggered, Parameter: $1" >&2
      write_to_all_cc=$1
      shift
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

until [ -z "$1" ]  # Until all parameters used up . . .
do
  touch "${1,,}.cc"
  touch "${1,,}.h"
  cat > "${1,,}.h" <<- EOM
#ifndef URPLUSPLUS_${1^^}_H_
#define URPLUSPLUS_${1^^}_H_
${write_to_all_h}

#endif
EOM

cat > "${1,,}.cc" <<- EOM
${write_to_all_cc}
EOM


  shift
done

exit
