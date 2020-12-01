#/!bin/bash

# check if the libraries have the proper dependencies
# load a single library in root and check the return value

# Allow to run the script in th test suite without parameters
# or from the command line passing the proper parameters
SCRIPTDIR=${1:-$VMCWORKDIR/scripts}
LIBDIR=${2:-../lib}

# find all libs
# libraries are real files with the extensions .so and for macosx .dylib
all_libs=$(find $LIBDIR -type f | grep -e \.dylib -e \.so)

tmpfile=$(mktemp)

ok=true
for lib in $all_libs; do
  echo "Loading the library $lib"
  root -l -q -b $SCRIPTDIR/loadlib.C\(\"$lib\"\) &> $tmpfile
  retval=$?
  if [[ retval -ne 0 ]]; then
    echo ""
    echo "Problem loading the library $lib"
    cat $tmpfile
    echo ""
    okay=false
  fi
done

rm $tmpfile

if [[ "$okay" = "false" ]]; then
  echo ""
  echo "Not all libraries could be loaded"
  echo "Test failed"
  exit 1
else
  exit 0
fi
