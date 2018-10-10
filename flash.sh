openocd -f /usr/share/openocd/scripts/board/stm32f3discovery.cfg -c "program `pwd`/$1 verify reset exit"
