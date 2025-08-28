for v in $(cat beemoviescript | tr "\t\n" " " | tr -s " "); do sleep .5; curl leddy -d "Text $v" &>/dev/null; done &
