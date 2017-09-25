Starting process at boot instead of on-demand

This example shows starting of our dummy service "example" at boot, or
more accurately, when systemd at boot up reaches the multi-user target.

The changes from 02.example to 03.atboot are:

 * remove the example.socket unit since we no longer have systemd make
   the socket for us, since we no longer use socket activation.
 * change srv.c (the source code for the example server) to bind the
   socket. 
 * add this line to the Install section of the .service file:

    WantedBy=multi-user.target

Now upon reaching the multi-user target the example daemon is started.

    make
    sudo cp srv /usr/bin/example
    sudo cp example.service /etc/systemd/system
    sudo systemctl enable example.service
    sudo systemctl start example.service

If you reboot now, then when the shell is ready, run:

    ps aux | grep example

Note the example service is already running. This is what makes it distinct
from on-demand startup.

Shut down and clean up:

    sudo systemctl stop example.service
    sudo systemctl disable example.service
    sudo rm -f /etc/systemd/system/example.service
    sudo rm -f /usr/bin/example
