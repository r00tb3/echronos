#
# eChronos Real-Time Operating System
# Copyright (c) 2017, Commonwealth Scientific and Industrial Research
# Organisation (CSIRO) ABN 41 687 119 230.
#
# All rights reserved. CSIRO is willing to grant you a licence to the eChronos
# real-time operating system under the terms of the CSIRO_BSD_MIT license. See
# the file "LICENSE_CSIRO_BSD_MIT.txt" for details.
#
# @TAG(CSIRO_BSD_MIT)
#

# qemu-system-ppc -S -nographic -gdb tcp::18181 -M ppce500 -kernel <SYSTEM_BINARY>
# powerpc-linux-gdb <SYSTEM_BINARY> -x <THIS_FILE>
target remote :18181
# Don't prompt for terminal input
set height 0
b debug_println
b main
b tick_irq
b interrupt_event_wait
c
c
c
c
c
c
c
c
c
c
c
c
c
c
c
c
c
c
# First tick
echo Tick 1-1
c
c
c
c
# Second tick
echo Tick 1-2
c
c
c
c
# Third tick
echo Tick 1-3
c
c
c
c
# Fourth tick
echo Tick 1-4
c
c
c
c
# Fifth tick - b wakes up and does some things
echo Tick 1-5
c
c
c
c
c
c
c
c
c
c
# First tick
echo Tick 2-1
c
c
c
c
# Second tick
echo Tick 2-2
c
c
c
c
# Third tick
echo Tick 2-3
c
c
c
c
# Fourth tick
echo Tick 2-4
c
c
c
c
# Fifth tick - b wakes up and does some things
echo Tick 2-5
c
c
c
c
c
c
c
c
c
c
# Let it tick one more time, then finish up
echo Tick 3, Done.
quit
y
