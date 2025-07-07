set -e

divisor() {
    echo -e "\033[35m----------------------------------\033[0m"
}

# default bitness 64 bits
BITNESS="64"
BITNESS64=ON
# defaulting the mode to Release if no argument is given
MODE=Release
#project name
PROJECT=timemeasurer
#to set as return value from functions
RETURN_VAL=0

manual()
{
    local SCRIPT_NAME=$(basename "$0")
    divisor
    echo "  To build unit tests for ${PROJECT}"
    echo "Execute:"
    echo "\"${SCRIPT_NAME}\"           Release mode with 64 bits bitness"
    echo "\"${SCRIPT_NAME} 32\"        to set 32 bits bitness"
    echo "\"${SCRIPT_NAME} Debug\"     to set Debug mode"
    divisor
}

set_variables() {
    RETURN_VAL=0
    # Iterate through all passed arguments
    for param in "$@"; do
        if [ "$param" == "32" ]; then
            BITNESS="32"
            BITNESS64=OFF
        fi
        if [ "$param" == "Debug" ]; then
            MODE="Debug"
        fi
        if [ "$param" == "--help" ] || [ "$param" == "--man" ] || [ "$param" == "-h" ] || [ "$param" == "--h" ]; then
            manual
            RETURN_VAL=1
        fi
    done
    return
}

set_variables "$@"

if [ ${RETURN_VAL} -eq 0 ]; then
    divisor
    echo "Chosen mode is ${MODE} for ${BITNESS} bits"
    divisor
else
    exit 0
fi

CWD=$(pwd)
SCRIPT_PATH=""

get_script_path() {
    local CWDLocal=$(pwd)
    # Handle symbolic links
    if [ -L "$0" ]; then
        SCRIPT_PATH="$( dirname "$( readlink -f "$0" )" )"
    else
        SCRIPT_PATH="$( cd "$( dirname "$0" )" >/dev/null 2>&1 && pwd )"
    fi
    cd ${CWDLocal}
}

#check cmake presence
command -v cmake >/dev/null 2>&1 || { echo >&2 "CMake is not installed. Aborting."; exit 1; }

get_script_path

# Establish the directory containing the script
PROJECTDIR=${SCRIPT_PATH}
TESTNAME=test${PROJECT}

# rebuilding initiation
cd $PROJECTDIR
rm -rf "buildLinux${MODE}${BITNESS}"
mkdir "buildLinux${MODE}${BITNESS}" && cd "buildLinux${MODE}${BITNESS}"
cmake -DCMAKE_BUILD_TYPE=${MODE} .. -DBITNESS64=${BITNESS64}
cmake --build . --config ${MODE}

divisor

FILENAME="$PROJECTDIR/buildLinux${MODE}${BITNESS}/${TESTNAME}"

if [ -e "$FILENAME" ]; then
    echo "${TESTNAME} built for ${MODE} mode with ${BITNESS} bits bitness is here:"
    echo "$FILENAME"
else
    echo -e "\033[31mERROR\033[0m ${TESTNAME} built for ${MODE} mode with ${BITNESS} bits bitness could not be found at"
    echo "$FILENAME"
fi

divisor

# Restore the initial execution directory
cd $CWD
