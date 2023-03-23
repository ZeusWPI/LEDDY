for v in $(cat beemoviescript | tr "\t\n" " " | tr -s " "); do sleep .5; curl 10.1.2.3 -d "Text $v" &>/dev/null; done &
