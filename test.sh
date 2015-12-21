#! /bin/sh

for shname in `ls`
do
    git add $shname && git ci -m "MTK6580:First Update Code" && git push origin master;
done

