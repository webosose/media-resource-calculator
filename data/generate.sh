#!/bin/sh
#
# Converts the JSON to a C header file to be used as a resource file.

print_usage() {
cat << EOF
USAGE:
    $0 h INPUT

DESCRIPTION:
    Outputs JSON data to another format.

EOF
}

to_h() {
cat << EOF
#ifndef ${1}_H
#define ${1}_H

static const char* $1 =
"$(cat "$2" | sed 's/"/\\"/g' | sed ':a;N;$!ba;s/\n/"\n"/g')";

#endif // ${1}_H
EOF
}

case "$1" in
h)
  if [ $# -eq 4 ] ; then
    to_h $2 "$3" > $4
  elif [ $# -eq 3 ] ; then
    to_h $2 "$3"
  else
    echo "no input file specified" 1>&2
  fi
  ;;
*)
  print_usage
  ;;
esac

exit 0
