#!/bin/bash
#
# makeStopRelease.sh
# Caleb J. Smith
# September 27, 2018
# 
# Required to run this script:
# - StopCfg repo
# - StopCfg branch to base release off
# - directory containing supplementary files (root files, etc)
# - config file containing one supplementary file name per line
# - git token: source a script that does "export GITHUB_TOKEN=<git_token_created_on_github>"
# - github-release command: softlink /uscms/home/caleb/bin/github-release -> /uscms/home/pastika/bin/github-release
# - put makeStopRelease.sh in bin: ~caleb/bin/makeStopRelease.sh
# - put github-release and makeStopRelease.sh in path: export PATH="$PATH:~caleb/bin"


GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg
USER=susy2015

SUPP_FILE_CFG=supplementaryFiles.cfg
SUPP_FILE_DIR=supplementaryFiles
SUPP_FILES=
TARBALL=SUPP_FILE_TARBALL.tar.gz
TAG=
MESSAGE=
BRANCH=



# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

STOP_CFG_NAME=sampleSets.cfg

function print_help {
    echo ""
    echo "Usage:"
    echo "    makeStopRelease.sh -t RELEASE_TAG -b BRANCH [-d SUPP_FILE_DIR] [-m MESSAGE]"
    echo ""
    echo "Options:"
    echo "    -t RELEASE_TAG :       This is the github release tag which will be created (Required)"
    echo "    -b BRANCH :            Git branch to base release off. This branch must exist. It will be pushed to github. (Required)"
    echo "    -d SUPP_FILE_DIR :     The folder where the supplementary files can be found (default $SUPP_FILE_DIR)"
    echo "    -m MESSAGE :           Commit message for tag (Default empty)"
    echo ""
    echo "Description:"
    echo "    This script automatically makes a release of the Stop search config files"
    echo "    Run this script from the $REPO_NAME directory"
    echo "    Source a script that does \"export GITHUB_TOKEN=<git_token_created_on_github>\" before running this script."
    echo ""
}


# Initialize our own variables:

while getopts "h?t:d:m:b:" opt; do
    case "$opt" in
    h|\?)
        print_help
        exit 0
        ;;
    t) TAG=$OPTARG
        ;;
    d) SUPP_FILE_DIR=$OPTARG
        ;;
    m) MESSAGE=$OPTARG
        ;;
    b) BRANCH=$OPTARG
        ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

if [[ -z $TAG ]]
then
    print_help
    exit 0
fi

if [[ -z $BRANCH ]]
then
    print_help
    exit 0
fi

if [[ -z $GITHUB_TOKEN ]]
then
    echo "ERROR: GITHUB_TOKEN is empty; this needs to be set with a script"
    echo "Source a script that does \"export GITHUB_TOKEN=<git_token_created_on_github>\""
    exit 1
fi

echo "GITHUB_TOKEN: $GITHUB_TOKEN"


# make list of files to tar
while read -r line || [[ -n "$line" ]]
do
    #echo "$line"
    # -z: returns true if length of string is zero
    # this just makes sure there is not leading space
    if [[ -z $SUPP_FILES ]]
    then
        SUPP_FILES="$SUPP_FILE_DIR/$line"
    else
        SUPP_FILES="$SUPP_FILES $SUPP_FILE_DIR/$line"
    fi
done < $SUPP_FILE_CFG

echo "SUPP_FILES: $SUPP_FILES"

for FILE in $SUPP_FILES
do
    if [ ! -f $FILE ]
    then
        echo "ERROR: File not found: $FILE"
        exit 1
    fi
done

echo "Making tarball: $TARBALL"
tar cfz $TARBALL $SUPP_FILES

if [ -f $TARBALL ]
then
    # creating branch and release
    echo "Creating branch and release"
    git push origin $BRANCH
    github-release release -u $USER -r $REPO_NAME -t $TAG -n $TAG -c $BRANCH -d "$MESSAGE"

    #Upload tarball here
    echo "Uploading tarball"
    github-release upload -u $USER -r $REPO_NAME -t $TAG -n $TARBALL -f $TARBALL -l "Stop supplementary files"
else
    echo "ERROR: Tarball not found: $TARBALL"
    exit 1
fi


# exit at the end; next section should be commented out
exit 0


