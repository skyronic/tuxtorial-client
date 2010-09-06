#!/bin/bash
stepfile="/tmp/tt.txt"
function tt()
{
    commandId=$RANDOM
    oldPwd=$PWD

    progname=$1
    shift;

    # handle all the builtins one by one
    if [ $progname = "cd" ]; then
        builtin cd $@ | tee -a $stepfile
    elif [ $progname = "echo" ]; then
        builtin echo $@ | tee -a $stepfile
    elif [ $progname = "pwd" ]; then
        builtin pwd $@ | tee -a $stepfile
    elif [ $progname = "eval" ]; then
        builtin eval $@ | tee -a $stepfile
    else
        $progname "$@" | tee -a $stepfile
    fi
    echo "[OUTPUT]$commandId" >> $stepfile
    echo "[CMD]$commandId" >> $stepfile
    echo $progname >> $stepfile
    while [ -n "$1" ]; do
        echo $1 |  sed 's/\\/\\\\/g' | sed 's/ /\\ /g' | sed 's/\"/\\\"/g' >> $stepfile
        shift
    done
    echo "[ENV]$commandId" >> $stepfile
    echo $oldPwd >> $stepfile
    echo "[END]$commandId" >> $stepfile
}

function ttq()
{
    commandId=$RANDOM
    oldPwd=$PWD

    bash -c "$@" | tee -a $stepfile
    echo "[OUTPUT]$commandId" >> $stepfile
    echo "[CMD]$commandId" >> $stepfile
    echo $@ >> $stepfile
    echo "[ENV]$commandId" >> $stepfile
    echo $oldPwd >> $stepfile
    echo "[END]$commandId" >> $stepfile
}
