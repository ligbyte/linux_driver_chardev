cmd_/home/jeek/Documents/linux_driver_chardev/modules.order := {   echo /home/jeek/Documents/linux_driver_chardev/chardev.ko; :; } | awk '!x[$$0]++' - > /home/jeek/Documents/linux_driver_chardev/modules.order
