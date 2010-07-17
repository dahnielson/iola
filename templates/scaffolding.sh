#!/bin/sh

if [ -z "$1" ]; then
    echo "usage: scaffolding.sh <template> <element>"
    exit 1
else
    template=$1
    template_dir="`dirname $0`"
    template_h=${template_dir}/${template}.h
    template_cpp=${template_dir}/${template}.cpp
fi

if [ -z "$2" ]; then
    echo "usage: scaffolding.sh <template> <element>"
    exit 1
else
    element=$2
    ELEMENT=$(echo $element | tr [:lower:] [:upper:])
fi

if [ ! -e "${template_h}" ]; then
    echo "template file ${template_h} does not exist."
    exit 1
fi

if [ ! -e "${template_cpp}" ]; then
    echo "template file ${template_h} does not exist."
    exit 1
fi

sed -e 's/template/'"$element"'/g' -e 's/TEMPLATE/'"$ELEMENT"'/g' ${template_h} > ${element}_element.h
sed -e 's/template/'"$element"'/g' -e 's/TEMPLATE/'"$ELEMENT"'/g' ${template_cpp} > ${element}_element.cpp
